#include <elzip/elzip.hpp>
#include <vili/parser/parser.hpp>

#include <System/Package.hpp>
#include <System/Path.hpp>
#include <Utils/FileUtils.hpp>
#include <Utils/VectorUtils.hpp>

namespace obe::System::Package
{
    std::string GetPackageLocation(const std::string& packageName)
    {
        if (PackageExists(packageName))
        {
            return vili::parser::from_file("Package/Packages.vili"_fs)
                .at(packageName)
                .at("path");
        }
        throw Exceptions::UnknownPackage(packageName, ListPackages(), EXC_INFO);
    }

    bool PackageExists(const std::string& packageName)
    {
        return !vili::parser::from_file("Package/Packages.vili"_fs)[packageName]
                    .is_null();
    }

    std::vector<std::string> ListPackages()
    {
        vili::node packages = vili::parser::from_file("Package/Packages.vili"_fs);
        std::vector<std::string> packageNames;
        for (const auto& [packageName, _] : packages.items())
        {
            packageNames.push_back(packageName);
        }
        return packageNames;
    }

    bool Install(const std::string& packageName)
    {
        Debug::Log->info("<Package> Installing Package '{0}'", packageName);
        if (!Utils::Vector::contains(
                packageName + ".opaque", Utils::File::getFileList("Package")))
        {
            throw Exceptions::PackageFileNotFound(
                fmt::format("Package/{}.opaque", packageName), EXC_INFO);
        }

        if (!PackageExists(packageName))
        {
            throw std::runtime_error(
                "Invalid feature: Package installation has been disabled for >v0.4");
        }
        throw Exceptions::PackageAlreadyInstalled(packageName, EXC_INFO);
    }

    bool Load(const std::string& packageName, const unsigned int priority)
    {
        Debug::Log->info(
            "<Package> Loading Package '{0}' with priority", packageName, priority);
        if (PackageExists(packageName))
        {
            MountablePath::Mount(MountablePath(
                MountablePathType::Package, GetPackageLocation(packageName), priority));
            return true;
        }
        throw Exceptions::UnknownPackage(packageName, ListPackages(), EXC_INFO);
    }
} // namespace obe::System::Package
