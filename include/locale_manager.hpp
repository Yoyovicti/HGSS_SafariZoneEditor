#ifndef LOCALE_MANAGER_HPP
#define LOCALE_MANAGER_HPP

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <string>
#include <fstream>
#include <iostream>

class LocaleManager {
private:
    json data_;

    const std::string FILE_PATH = "./data/locale.json";

    LocaleManager() {
        std::ifstream file(FILE_PATH);
        if(!file.is_open()) {
            std::cerr << "Unable to find locale.json file" << std::endl;
        }

        data_ = json::parse(file);
    };

public:
    // Prevent copy and assignment
    LocaleManager(const LocaleManager&) = delete;
    LocaleManager& operator=(const LocaleManager&) = delete;

    static LocaleManager& getInstance() {
        static LocaleManager instance_;
        return instance_;
    }

    bool getTable(json* output, const std::string& key) {
        if(!data_.contains(key))
            return false;
        
        *output = data_[key];
        return true;
    }

    const json& getData() const {
        return data_;
    }
};

#endif
