#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <fstream>
#include <iostream>

class ConfigManager {
private:
    json data_;

    const std::string CONFIG_PATH = "config.json";

    ConfigManager() {
        std::ifstream file(CONFIG_PATH);
        if(!file.is_open()) {
            std::cout << "Unable to find " << CONFIG_PATH << " file. Default configuration will be used." << std::endl;
            data_["locale"] = 0;
            data_["default_path"] = ".";
            return;
        }

        data_ = json::parse(file);
    }

public:
    // Prevent copy and assignment
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    static ConfigManager& getInstance() {
        static ConfigManager instance_;
        return instance_;
    }

    void saveConfig() {
        std::ofstream file(CONFIG_PATH);
        if(!file.is_open()) {
            std::cerr << "Unable to save config at: " << CONFIG_PATH << std::endl;
            return;
        }

        file << data_.dump(4);
        std::cout << "Configuration saved at: " << CONFIG_PATH << std::endl;
    }

    uint8_t getLocale() const {
        return data_["locale"];
    }

    void setLocale(const uint8_t& locale) {
        data_["locale"] = locale;
    }

    std::string getDefaultPath() const {
        return data_["default_path"];
    }

    void setDefaultPath(const std::string& path) {
        data_["default_path"] = path;
    }
};

#endif