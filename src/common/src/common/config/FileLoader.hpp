#ifndef KATARENGA_COMMON_CONFIG_FILELOADER_HPP
#define KATARENGA_COMMON_CONFIG_FILELOADER_HPP

// Standard Library
#include <filesystem>

struct FileLoader
{
    virtual std::optional<Configuration> load(std::filesystem::path filename) = 0;
};

#endif // KATARENGA_COMMON_CONFIG_FILELOADER_HPP

