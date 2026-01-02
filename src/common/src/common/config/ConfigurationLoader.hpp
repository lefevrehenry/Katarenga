#ifndef KATARENGA_COMMON_CONFIG_CONFIGURATION_LOADER_HPP
#define KATARENGA_COMMON_CONFIG_CONFIGURATION_LOADER_HPP

// Katarenga
#include <common/config/FileLoader.hpp>

// Standard Library
#include <filesystem>
#include <optional>
#include <vector>

/**
 * @brief The ConfigurationLoader class
 *
 * The main interface for loading configuration file based on a list of paths and loaders
 */
class ConfigurationLoader
{
    using Filename = std::string;
    
    using Path = std::filesystem::path;
    using Paths = std::vector<Path>;
    
    using Loader = FileLoader;
    using Loaders = std::vector<Loader>;

public:
    ConfigurationLoader(const Filename& _filename, const Paths& _paths, const Loaders& _loaders);

    /**
     * @brief load the configuration file
     * @return
     */
    Configuration load() const;

private:
    std::optional<Configuration> load(Path filename) const;
    
private:
    Filename filename;
    Paths paths;
    Loaders loaders;
    
};

#endif // KATARENGA_COMMON_CONFIG_CONFIGURATION_LOADER_HPP

