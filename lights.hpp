#pragma once

#include <exception>
#include <unordered_map>
#include <string>
#include <variant>
#include <nlohmann/json.hpp>

namespace joshai {
class lights {
    struct light_data {
        std::string name;
        std::string room;
        bool        on;
        int         brightness;
    };
    // normally for small sets vector is faster than map
    // however map has more user friendly interface for access by id
    // plus we don't know if that performance optimization going to make any difference for us
    // so on this point I'm using a map and later this could be refactored if needed
    std::unordered_map<std::string, light_data> light_collection;

public:
    struct name_change       { std::string name; };
    struct room_change       { std::string room; };
    struct on_change         { bool on; };
    struct brightness_change { int brightness; };
    struct removed_change    { std::string name; };

private:
    using variant_change = std::variant<name_change, room_change, on_change, brightness_change, removed_change>;
    struct device_changes { std::string id; std::vector<variant_change> changes; };
    using change_list = std::vector<device_changes>;

public:

    lights() = default;
    // takes json string
    lights(const std::string& list_str) {
        auto jlist = nlohmann::json::parse(list_str);
        for (auto& element : jlist) {
            light_collection[element["id"]] = { .name = element["name"],
                                                .room = element["room"] };
        }
    }
    
    inline const auto& get_lights() const {
        return light_collection;
    }

    // updates specific light entry from json string provided
    void update_light(const std::string& id, const std::string& light_str) {
        auto light_data = nlohmann::json::parse(light_str);
        auto el = light_collection.find(id);
        if (el != light_collection.end()) {
            el->second = { .name       = light_data["name"],
                           .room       = light_data["room"],
                           .on         = light_data["on"],
                           .brightness = light_data["brightness"] };
        } else {
            // this should be much pretier in production grade project
            throw std::runtime_error( "update_light" + id + " not found");
        }
    }

    // overwrites lights with updated lights data and returns diff vector
    change_list update(const lights& newlights) {
        change_list diff;

        const auto& lights2 = newlights.get_lights();
        for (auto& [k,v] : lights2) {
            auto old_it = light_collection.find(k);
            if (old_it == light_collection.end()){
                diff.emplace_back( device_changes{
                                    .id = k, 
                                    .changes = {name_change{v.name},
                                        room_change{v.room},
                                        on_change{v.on},
                                        brightness_change{v.brightness}}} );
            } else {
                device_changes cur_change;
                if (v.name != old_it->second.name)
                    cur_change.changes.emplace_back( name_change{v.name} );
                if (v.room != old_it->second.room)
                    cur_change.changes.emplace_back( room_change{v.room} );
                if (v.on != old_it->second.on)
                    cur_change.changes.emplace_back( on_change{v.on} );
                if (v.brightness != old_it->second.brightness)
                    cur_change.changes.emplace_back( brightness_change{v.brightness} );
                
                if (!cur_change.changes.empty()) {
                    cur_change.id = k;
                    diff.emplace_back(cur_change);
                }
            }
        }

        for (auto& [k,v] : light_collection) {
            if (lights2.find(k) == lights2.end())
                diff.emplace_back( device_changes{.id = k, 
                                                  .changes = {removed_change{v.name}}} );
        }

        light_collection = lights2;

        return diff;
    }
};

}