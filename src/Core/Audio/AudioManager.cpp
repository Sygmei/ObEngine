#include <Audio/AudioManager.hpp>
#include <Audio/Exceptions.hpp>
#include <Audio/Sound.hpp>
#include <Debug/Logger.hpp>
#include <System/Path.hpp>

#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>
#include <soloud/soloud_wavstream.h>

namespace obe::Audio
{
    AudioManager::AudioManager()
    {
        Debug::Log->debug("<AudioManager> Initializing AudioManager");
        m_engine.init();
    }
    AudioManager::~AudioManager()
    {
        Debug::Log->debug("<AudioManager> Cleaning AudioManager");
        m_engine.deinit();
    }

    Sound AudioManager::load(const System::Path& path, LoadPolicy loadPolicy)
    {
        const std::string filePath = path.find(System::PathType::File);
        Debug::Log->debug("<AudioManager> Loading Audio at '{}'", filePath);
        if (filePath.empty())
        {
            throw Exceptions::AudioFileNotFound(
                path.toString(), System::MountablePath::StringPaths(), EXC_INFO);
        }

        if (loadPolicy == LoadPolicy::Cache && m_cache.find(filePath) == m_cache.end())
        {
            std::shared_ptr<SoLoud::Wav> sample = std::make_shared<SoLoud::Wav>();
            sample->load(filePath.c_str());
            m_cache[filePath] = sample;
        }
        std::shared_ptr<SoLoud::AudioSource> sample;
        if (m_cache.find(filePath) != m_cache.end())
        {
            sample = m_cache[filePath];
        }
        else
        {
            if (loadPolicy == LoadPolicy::Stream)
            {
                sample = std::make_shared<SoLoud::WavStream>();
                dynamic_cast<SoLoud::WavStream*>(sample.get())->load(filePath.c_str());
            }
            else
            {
                sample = std::make_shared<SoLoud::Wav>();
                dynamic_cast<SoLoud::Wav*>(sample.get())->load(filePath.c_str());
            }
        }
        return Sound(m_engine, std::move(sample));
    }
}
