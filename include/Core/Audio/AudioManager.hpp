#pragma once

#include <unordered_map>

#include <System/Path.hpp>

#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>

namespace obe::Audio
{
    class Sound;

    /**
     * \brief Defines how a sound should be loaded
     */
    enum class LoadPolicy
    {
        /**
         * \brief The sound will be loaded in memory from a file or the cache
         */
        Normal,
        /**
         * \brief The sound will be loaded in memory and cached in the AudioManager
         */
        Cache,
        /**
         * \brief The sound will be streamed from a disk or copied from the cache
         */
        Stream
    };

    /**
     * \brief Class to handle audio playback
     * \bind{AudioManager}
     */
    class AudioManager
    {
    private:
        SoLoud::Soloud m_engine;
        std::unordered_map<std::string, std::shared_ptr<SoLoud::Wav>> m_cache;

    public:
        /**
         * \brief Initializes the underlying audio engine
         */
        AudioManager();
        /**
         * \brief Closes the underlying audio engine
         */
        ~AudioManager();

        AudioManager&& operator=(AudioManager&&) = delete; //no copy nor move

        /**
         * \brief Loads a sound file at a given path
         * \param path Path to the sound file
         * \param loadPolicy The way you want to load the sound file
         * \return A Sound object loaded with the sound file
         */
        Sound load(const System::Path& path, LoadPolicy loadPolicy = LoadPolicy::Normal);
    };
}
