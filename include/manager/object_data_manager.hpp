#ifndef OBJECT_DATA_MANAGER_H
#define OBJECT_DATA_MANAGER_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <fstream>
#include <iostream>

class ObjectDataManager {
public:
    // Prevent copy and assignment
    ObjectDataManager(const ObjectDataManager&) = delete;
    ObjectDataManager& operator=(const ObjectDataManager&) = delete;

    static ObjectDataManager& getInstance() {
        static ObjectDataManager instance_;
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

private:
    json data_;

    inline static const std::string FILE_PATH = "./data/object_data.json";

    ObjectDataManager() {
        std::ifstream file(FILE_PATH);
        if(!file.is_open()) {
            std::cerr << "Unable to find object_data.json file" << std::endl;
        }

        data_ = json::parse(file);
    };
};

#endif // OBJECT_DATA_MANAGER_H
