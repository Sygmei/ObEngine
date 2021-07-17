#include <Config/Config.hpp>
#include <System/Path.hpp>

#include <Config/Templates/Config.hpp>
#include <Debug/Logger.hpp>

#include <vili/parser/parser.hpp>

namespace obe::Config
{
    ConfigurationManager Config;
    ConfigurationManager::ConfigurationManager()
        : vili::node(vili::object {})
    {
    }
    void ConfigurationManager::load()
    {
        // TODO: Do not modify MountedPaths directly
        auto mountPoints = System::MountablePath::Paths();
        std::reverse(mountPoints.begin(), mountPoints.end());
        const auto loadResult
            = System::Path(mountPoints).set("Data/config.cfg.vili").findAll();
        for (const std::string& path : loadResult)
        {
            Debug::Log->info("Loading config file from {}", path);
            vili::node conf
                = vili::parser::from_file(path, Templates::getConfigTemplates());
            std::cout << "Merging : " << conf.dump() << std::endl;
            this->merge(conf);
            std::cout << "Result : " << this->dump() << std::endl;
        }
    }
} // namespace obe::System
