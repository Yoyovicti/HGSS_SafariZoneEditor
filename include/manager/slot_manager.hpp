#ifndef SLOT_MANAGER_HPP
#define SLOT_MANAGER_HPP

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class SlotManager {
public:
    SlotManager() {
        std::ifstream file(CONFIG_PATH);
        if(!file.is_open()) {
            std::cout << "Unable to find " << CONFIG_PATH << " file. Default configuration will be used." << std::endl;
            data_["locale"] = 0;
            data_["default_path"] = ".";
            return;
        }

        data_ = json::parse(file);
    }

    // Prevent copy and assignment
    SlotManager(const SlotManager&) = delete;
    SlotManager& operator=(const SlotManager&) = delete;

    static SlotManager& getInstance() {
        static SlotManager instance_;
        return instance_;
    }

    bool getTables(
            json* base_table,
            json* block_table 
            const std::string& enc_type, 
            const std::string& area_type, 
            const std::string& time) {

        if(!data_.contains(enc_type))
            return false;
        
        if(!data[enc_type].contains(area_type)) {
            return false;
        }

        if(!data[enc_type][area_type].contains(time)) {
            return false;
        }

        *base_table = data[enc_type][area_type][time]["base"]
        *block_table = data[enc_type][area_type][time]["blocks"]
        return true;
    }

private:
    json data_;

    const std::string CONFIG_PATH = "./data/slot_data.json";


};

#endif // SLOT_MANAGER_HPP