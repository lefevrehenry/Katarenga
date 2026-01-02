#ifndef KATARENGA_COMMON_CONFIG_INI_LOADER_HPP
#define KATARENGA_COMMON_CONFIG_INI_LOADER_HPP

// Katarenga
#include <common/config/Configuration.hpp>
#include <common/config/FileLoader.hpp>

// Ini
#include <ini/ini.h>

// Standard Library
#include <filesystem>
#include <optional>

struct IniLoader : public FileLoader
{
    virtual std::optional<Configuration> load(std::filesystem::path filename) override {
        inih::INIReader ini(filename);

        Configuration configuration;

//        // Get and parse the ini value
//        std::string thread_port = ini.Get<std::string>("thread", "port");
//        std::string processus_port = ini.Get<std::string>("processus", "port");
//        std::string tcp_port = ini.Get<std::string>("tcp", "port");

//        configuration.add("thread.port", thread_port);
//        configuration.add("processus.port", processus_port);
//        configuration.add("tcp.port", tcp_port);

        return configuration;
    }
};

#endif // KATARENGA_COMMON_CONFIG_INI_LOADER_HPP

