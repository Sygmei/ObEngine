#include <Script/GameObject.hpp>
#include <Triggers/Exceptions.hpp>
#include <Triggers/Trigger.hpp>
#include <Triggers/TriggerGroup.hpp>
#include <Utils/StringUtils.hpp>

namespace obe::Triggers
{
    sol::protected_function makeCallback(
        sol::state_view lua, const std::string& triggerTableName, TriggerEnv& env)
    {
        const sol::protected_function MakeCallback
            = lua["LuaCore"]["MakeCallback"].get<sol::protected_function>();
        env.environment.set_on(MakeCallback);
        sol::protected_function callback
            = lua.safe_script("return " + env.callback, env.environment)
                  .get<sol::protected_function>();
        const sol::protected_function_result makeCallbackResult
            = MakeCallback.call(triggerTableName, callback, env.callback);
        if (!makeCallbackResult.valid())
        {
            const auto errObj = makeCallbackResult.get<sol::error>();
            const auto errMsg = errObj.what();
            throw Exceptions::CallbackCreationError(
                triggerTableName, env.id, env.callback, errMsg, EXC_INFO);
        }
        sol::protected_function callbackWithArgs
            = makeCallbackResult.get<sol::protected_function>();
        env.environment.set_on(callbackWithArgs);
        return callbackWithArgs;
    }

    std::string Trigger::getTriggerLuaTableName() const
    {
        return this->getNamespace() + "__" + this->getGroup() + "__" + m_name;
    }

    Trigger::Trigger(TriggerGroup& parent, const std::string& name, bool startState)
        : m_parent(parent)
        , m_lua(parent.m_lua)
    {
        m_name = name;
        m_parent = parent;
        m_enabled = startState;
        m_fullName = this->getNamespace() + "." + this->getGroup() + "." + m_name;
        m_lua["__TRIGGERS"][this->getTriggerLuaTableName()].get_or_create<sol::table>();
        m_lua["__TRIGGERS"][this->getTriggerLuaTableName()]["ArgTable"]
            .get_or_create<sol::table>();
        Debug::Log->trace(
            "<Trigger> Creating Trigger {0} @{1}", m_fullName, fmt::ptr(this));
    }

    bool Trigger::getState() const
    {
        return m_enabled;
    }

    std::string Trigger::getGroup() const
    {
        return m_parent.getName();
    }

    std::string Trigger::getName() const
    {
        return m_name;
    }

    std::string Trigger::getNamespace() const
    {
        return m_parent.getNamespace();
    }

    void Trigger::registerEnvironment(const std::string& id, sol::environment environment,
        const std::string& callback, bool* active)
    {
        Debug::Log->trace("<Trigger> Registering Lua Environment {0} in "
                          "Trigger {1} associated to callback {2}",
            environment.pointer(), m_name, callback);
        m_registeredEnvs.emplace_back(id, environment, callback, active);

        sol::table triggerRef = environment["__TRIGGERS"][this->getTriggerLuaTableName()]
                                    .get_or_create<sol::table>();
        triggerRef["callback"] = callback;
        if (m_onRegisterCallback)
        {
            Debug::Log->trace(
                "<Trigger> Calling onRegister callback of Trigger {0}", m_fullName);
            m_onRegisterCallback(m_registeredEnvs.back());
        }
    }

    void Trigger::unregisterEnvironment(sol::environment environment)
    {
        Debug::Log->trace("<Trigger> Unregistering Lua Environment {0} from Trigger {1}",
            environment.pointer(), m_fullName);
        for (const TriggerEnv& triggerEnv : m_registeredEnvs)
        {
            if (triggerEnv.environment == environment)
            {
                if (m_onUnregisterCallback)
                {
                    Debug::Log->trace("<Trigger> Calling onUnregister callback "
                                      "of Trigger {0}",
                        m_fullName);
                    m_onUnregisterCallback(triggerEnv);
                }
            }
        }
        if (m_currentlyTriggered)
        {
            m_envsToRemove.push_back(environment);
        }
        else
        {
            m_registeredEnvs.erase(
                std::remove_if(m_registeredEnvs.begin(), m_registeredEnvs.end(),
                    [&environment](
                        TriggerEnv& env) { return env.environment == environment; }),
                m_registeredEnvs.end());
        }
    }

    void Trigger::execute()
    {
        m_currentlyTriggered = true;
        Debug::Log->trace("<Trigger> Executing Trigger {0}", m_fullName);
        for (std::size_t i = 0; i < m_registeredEnvs.size(); i++)
        {
            auto& rEnv = m_registeredEnvs[i];
            if (*rEnv.active)
            {
                Debug::Log->trace("<Trigger> Calling Trigger Callback {0} on "
                                  "Lua Environment {1} from Trigger {2}",
                    rEnv.callback, rEnv.environment.pointer(), m_fullName);
                //sol::function callback = m_lua.get<sol::function>(rEnv.callback);

                if (!rEnv.call)
                {
                    rEnv.call = makeCallback(m_lua, this->getTriggerLuaTableName(), rEnv);
                }

                sol::protected_function_result result = rEnv.call();
                if (!result.valid())
                {
                    const auto errObj = result.get<sol::error>();
                    const std::string errMsg = "\n        \""
                        + Utils::String::replace(errObj.what(), "\n", "\n        ")
                        + "\"";
                    const std::string fullName = this->getNamespace() + "."
                        + this->getGroup() + "." + this->getName();
                    throw Exceptions::TriggerExecutionError(
                        fullName, rEnv.id, rEnv.callback, errMsg, EXC_INFO);
                }
            }
        }
        m_lua["__TRIGGERS"][this->getTriggerLuaTableName()]["ArgTable"].set(
            sol::new_table());
        if (!m_envsToRemove.empty())
        {
            for (const sol::environment& envToRemove : m_envsToRemove)
            {
                m_registeredEnvs.erase(
                    std::remove_if(m_registeredEnvs.begin(), m_registeredEnvs.end(),
                        [&envToRemove](
                            TriggerEnv& env) { return env.environment == envToRemove; }),
                    m_registeredEnvs.end());
            }
        }
        m_currentlyTriggered = false;
    }

    void Trigger::pushParameterFromLua(const std::string& name, sol::object parameter)
    {
        Debug::Log->trace(
            "<Trigger> Pushing parameter {0} (type: {1}) to Trigger {2} (From Lua)", name,
            static_cast<int>(parameter.get_type()), m_fullName);
        m_lua["__TRIGGERS"][this->getTriggerLuaTableName()]["ArgTable"][name] = parameter;
    }

    void Trigger::onRegister(std::function<void(const TriggerEnv&)> callback)
    {
        Debug::Log->trace("<Trigger> Add onRegister callback to Trigger {0}", m_fullName);
        m_onRegisterCallback = callback;
    }

    void Trigger::onUnregister(std::function<void(const TriggerEnv&)> callback)
    {
        Debug::Log->trace(
            "<Trigger> Add onUnregister callback to Trigger {0}", m_fullName);
        m_onUnregisterCallback = callback;
    }
} // namespace obe::Triggers
