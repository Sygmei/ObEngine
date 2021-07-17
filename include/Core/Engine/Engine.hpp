#pragma once

#include <Audio/AudioManager.hpp>
#include <Config/Config.hpp>
#include <Engine/ResourceManager.hpp>
#include <Input/InputManager.hpp>
#include <Scene/Scene.hpp>
#include <System/Cursor.hpp>
#include <System/Plugin.hpp>
#include <System/Window.hpp>
#include <Time/FramerateManager.hpp>
#include <Triggers/TriggerManager.hpp>
#include <sol/sol.hpp>

namespace obe::Bindings
{
    void IndexAllBindings(sol::state_view state);
}

namespace obe::Engine
{
    class Engine
    {
    protected:
        bool m_initialized = false;
        std::vector<std::unique_ptr<System::Plugin>> m_plugins;
        std::unique_ptr<sol::state> m_lua;
        std::unique_ptr<Scene::Scene> m_scene;
        std::unique_ptr<System::Cursor> m_cursor;
        std::unique_ptr<System::Window> m_window;

        // Managers
        Audio::AudioManager m_audio {};
        Config::ConfigurationManager m_config {};
        std::unique_ptr<ResourceManager> m_resources {};
        std::unique_ptr<Input::InputManager> m_input {};
        std::unique_ptr<Time::FramerateManager> m_framerate;
        std::unique_ptr<Triggers::TriggerManager> m_triggers;

        // TriggerGroups
        Triggers::TriggerGroupPtr t_game {};

        // Initialization
        void initConfig();
        void initLogger() const;
        void initScript();
        void initTriggers();
        void initInput();
        void initFramerate();
        void initResources();
        void initWindow();
        void initCursor();
        void initPlugins();
        void initScene();

        // Main loop
        void handleWindowEvents() const;
        void update() const;
        void render();

        // Cleaning
        void clean() const;
        void purge();

    public:
        Engine();
        ~Engine();
        Engine& operator=(Engine&&) = delete; //no copy nor move

        void init();
        void run();

        /**
         * \bind{Audio}
         * \asproperty
         */
        Audio::AudioManager& getAudioManager();
        /**
         * \bind{Configuration}
         * \asproperty
         */
        Config::ConfigurationManager& getConfigurationManager();
        /**
         * \bind{Resources}
         * \asproperty
         */
        ResourceManager& getResourceManager();
        /**
         * \bind{Input}
         * \asproperty
         */
        Input::InputManager& getInputManager() const;
        /**
         * \bind{Framerate}
         * \asproperty
         */
        Time::FramerateManager& getFramerateManager() const;
        /**
         * \bind{Triggers}
         * \asproperty
         */
        Triggers::TriggerManager& getTriggerManager() const;

        /**
         * \bind{Scene}
         * \asproperty
         */
        Scene::Scene& getScene() const;
        /**
         * \bind{Cursor}
         * \asproperty
         */
        System::Cursor& getCursor() const;
        /**
         * \bind{Window}
         * \asproperty
         */
        System::Window& getWindow() const;
    };
}
