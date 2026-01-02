#include "ConfigurationLoader.hpp"


ConfigurationLoader::ConfigurationLoader(const Filename &_filename, const Paths &_paths, const Loaders &_loaders) :
    filename(_filename),
    paths(_paths),
    loaders(_loaders)
{
}

Configuration ConfigurationLoader::load() const {
    for (const Path& path : paths) {
        Path current = path / filename;

        std::optional<Configuration> configuration = load(current);

        if(configuration)
            return configuration.value();
    }

    throw std::runtime_error("file not found");
}

std::optional<Configuration> ConfigurationLoader::load(Path filename) const {
    if(!filename.exists())
        return {};

    for (const Loader& loader : loaders) {
        std::optional<Configuration> configuration = loader.load(filename);

        if(configuration)
            return configuration.get();
    }

    throw std::runtime_error("no loader found");
}
