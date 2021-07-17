#pragma once

#include <Triggers/Trigger.hpp>
#include <map>
#include <memory>
#include <sol/sol.hpp>

namespace obe::Triggers
{
    /**
     * \brief Class used to manage multiple Trigger
     */
    class TriggerGroup
    {
    private:
        std::string m_name;
        std::string m_fromNsp;
        std::map<std::string, std::shared_ptr<Trigger>> m_triggerMap;
        bool m_joinable = false;
        sol::state_view m_lua;
        friend class Trigger;
        friend class TriggerManager;

    public:
        /**
         * \brief Creates a new TriggerGroup
         * \param triggerGroupNamespace Name of the Namespace the TriggerGroup
         *        is in
         * \param triggerGroupName Name of the TriggerGroup
         */
        explicit TriggerGroup(sol::state_view lua,
            const std::string& triggerGroupNamespace,
            const std::string& triggerGroupName);
        ~TriggerGroup();
        TriggerGroup(const TriggerGroup&) = default;
        TriggerGroup& operator=(const TriggerGroup&) = default;
        TriggerGroup(TriggerGroup&&) noexcept = default;
        TriggerGroup& operator=(TriggerGroup&&) noexcept = default;

        /**
         * \brief Sets if the TriggerGroup is joinable or not
         * \param joinable true if the TriggerGroup should be joinable, false
         *        otherwise
         */
        void setJoinable(bool joinable);
        /**
         * \brief Get if the TriggerGroup is joinable or not
         * \return true if the TriggerGroup is joinable, false otherwise
         */
        [[nodiscard]] bool isJoinable() const;
        /**
         * \brief Get a Trigger contained in the TriggerGroup
         * \param triggerName Name of the Trigger to get
         * \return A pointer to the Trigger if found (throws an error otherwise)
         */
        std::weak_ptr<Trigger> get(const std::string& triggerName);
        /**
         * \brief Creates a new Trigger in the TriggerGroup
         * \param triggerName Name of the Trigger to create
         * \return Pointer to the TriggerGroup to chain calls
         */
        TriggerGroup& add(const std::string& triggerName);
        /**
         * \brief Removes a Trigger from the TriggerGroup
         * \param triggerName Name of the Trigger to remove
         * \return Pointer to the TriggerGroup to chain calls
         */
        TriggerGroup& remove(const std::string& triggerName);
        /**
         * \brief Enables a Trigger
         * \param triggerName Name of the Trigger to enable
         * \return Pointer to the TriggerGroup to chain calls
         */
        TriggerGroup& trigger(const std::string& triggerName);
        /**
         * \brief Pushes a Parameter to a Trigger
         * \tparam P Type of the Parameter
         * \param triggerName Name of the Trigger to push the parameter
         * \param parameterName Name of the parameter
         * \param parameter Value of the parameter
         */
        template <typename P>
        void pushParameter(const std::string& triggerName,
            const std::string& parameterName, P parameter);
        /**
         * \brief Pushes a Lua Parameter to a Trigger
         * \param triggerName Name of the Trigger to push the parameter
         * \param parameterName Name of the parameter
         * \param parameter Lua Value of the Parameter
         */
        void pushParameterFromLua(const std::string& triggerName,
            const std::string& parameterName, sol::object parameter);
        /**
         * \brief Get the name of all Trigger contained in the TriggerGroup
         * \return A std::vector of std::string containing the name of all
         * Trigger contained in the TriggerGroup
         */
        std::vector<std::string> getTriggersNames();
        /**
         * \brief Get all the Trigger contained in the TriggerGroup
         * \return A std::vector of Trigger pointers
         */
        std::vector<Trigger*> getTriggers();
        /**
         * \brief Get the name of the namespace the Trigger is in
         * \return A std::string containing the name of the namespace
         */
        [[nodiscard]] std::string getNamespace() const;
        /**
         * \brief Get the name of the TriggerGroup
         * \return A std::string containing the name of the TriggerGroup
         */
        [[nodiscard]] std::string getName() const;

        /**
         * \brief Register a callback for when something calls Trigger::register
         */
        void onRegister(const std::string& triggerName,
            std::function<void(const TriggerEnv&)> callback);
        /**
         * \brief Register a callback for when something calls Trigger::unRegister
         */
        void onUnregister(const std::string& triggerName,
            std::function<void(const TriggerEnv&)> callback);
    };

    using TriggerGroupPtr = std::shared_ptr<TriggerGroup>;

    template <typename P>
    void TriggerGroup::pushParameter(
        const std::string& triggerName, const std::string& parameterName, P parameter)
    {
        m_triggerMap[triggerName]->pushParameter(parameterName, parameter);
    }
} // namespace obe::Triggers
