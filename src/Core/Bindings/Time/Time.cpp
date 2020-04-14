#include <Bindings/Time/Time.hpp>

#include <Time/Chronometer.hpp>
#include <Time/FramerateCounter.hpp>
#include <Time/FramerateManager.hpp>
#include <Time/TimeUtils.hpp>

#include <sol/sol.hpp>

namespace obe::Time::Bindings
{
    void LoadClassChronometer(sol::state_view state)
    {
        sol::table TimeNamespace = state["obe"]["Time"].get<sol::table>();
        sol::usertype<obe::Time::Chronometer> bindChronometer
            = TimeNamespace.new_usertype<obe::Time::Chronometer>(
                "Chronometer", sol::call_constructor, sol::default_constructor);
        bindChronometer["start"] = &obe::Time::Chronometer::start;
        bindChronometer["stop"] = &obe::Time::Chronometer::stop;
        bindChronometer["reset"] = &obe::Time::Chronometer::reset;
        bindChronometer["getTime"] = &obe::Time::Chronometer::getTime;
        bindChronometer["setLimit"] = &obe::Time::Chronometer::setLimit;
        bindChronometer["getLimit"] = &obe::Time::Chronometer::getLimit;
        bindChronometer["over"] = &obe::Time::Chronometer::over;
    }
    void LoadClassFramerateCounter(sol::state_view state)
    {
        sol::table TimeNamespace = state["obe"]["Time"].get<sol::table>();
        sol::usertype<obe::Time::FramerateCounter> bindFramerateCounter
            = TimeNamespace.new_usertype<obe::Time::FramerateCounter>(
                "FramerateCounter", sol::call_constructor, sol::default_constructor);
        bindFramerateCounter["tick"] = &obe::Time::FramerateCounter::tick;
        bindFramerateCounter["uTick"] = &obe::Time::FramerateCounter::uTick;
        bindFramerateCounter["loadFont"] = &obe::Time::FramerateCounter::loadFont;
        bindFramerateCounter["draw"] = &obe::Time::FramerateCounter::draw;
    }
    void LoadClassFramerateManager(sol::state_view state)
    {
        sol::table TimeNamespace = state["obe"]["Time"].get<sol::table>();
        sol::usertype<obe::Time::FramerateManager> bindFramerateManager
            = TimeNamespace.new_usertype<obe::Time::FramerateManager>("FramerateManager",
                sol::call_constructor,
                sol::constructors<obe::Time::FramerateManager(obe::System::Window&)>());
        bindFramerateManager["configure"] = &obe::Time::FramerateManager::configure;
        bindFramerateManager["update"] = &obe::Time::FramerateManager::update;
        bindFramerateManager["doRender"] = &obe::Time::FramerateManager::doRender;
        bindFramerateManager["getDeltaTime"] = &obe::Time::FramerateManager::getDeltaTime;
        bindFramerateManager["getGameSpeed"] = &obe::Time::FramerateManager::getGameSpeed;
        bindFramerateManager["getSpeedCoefficient"]
            = &obe::Time::FramerateManager::getSpeedCoefficient;
        bindFramerateManager["isFramerateLimited"]
            = &obe::Time::FramerateManager::isFramerateLimited;
        bindFramerateManager["getFramerateTarget"]
            = &obe::Time::FramerateManager::getFramerateTarget;
        bindFramerateManager["isVSyncEnabled"]
            = &obe::Time::FramerateManager::isVSyncEnabled;
        bindFramerateManager["setSpeedCoefficient"]
            = &obe::Time::FramerateManager::setSpeedCoefficient;
        bindFramerateManager["limitFramerate"]
            = &obe::Time::FramerateManager::limitFramerate;
        bindFramerateManager["setFramerateTarget"]
            = &obe::Time::FramerateManager::setFramerateTarget;
        bindFramerateManager["setVSyncEnabled"]
            = &obe::Time::FramerateManager::setVSyncEnabled;
    }
    void LoadFunctionEpoch(sol::state_view state)
    {
        sol::table TimeNamespace = state["obe"]["Time"].get<sol::table>();
        TimeNamespace.set_function("epoch", obe::Time::epoch);
    }

    void LoadTimeUnits(sol::state_view state)
    {
        sol::table TimeNamespace = state["obe"]["Time"].get<sol::table>();
        TimeNamespace["microseconds"] = obe::Time::microseconds;
        TimeNamespace["milliseconds"] = obe::Time::milliseconds;
        TimeNamespace["seconds"] = obe::Time::seconds;
        TimeNamespace["minutes"] = obe::Time::minutes;
        TimeNamespace["hours"] = obe::Time::hours;
        TimeNamespace["days"] = obe::Time::days;
        TimeNamespace["weeks"] = obe::Time::weeks;
    }
};