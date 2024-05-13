#pragma once

#include "httplib.h"
#include "lights.hpp"

namespace joshai {
    class lights_monitor {
        static bool is_running;
        lights lights_data;
    public:
        static void stop() {
            // this supposed to be called from another thread but
            // lock free approach is fine here
            is_running = false;
        }

        // monitors lights infinitely until stop() is executed
        // hardcoded server is http://localhost:8080
        // hardcoded sleep between fetching requests 0.5 seconds
        void start() {
            using namespace std;

            httplib::Client http_client("http://localhost:8080");
            while (is_running) {
                try {

                    auto all_lights_res = http_client.Get("/lights");
                    if (all_lights_res->status == httplib::StatusCode::OK_200) {
                        lights cur_lights(all_lights_res->body);
                        for (auto& [k,_] : cur_lights.get_lights()) {
                            auto cur_light_res = http_client.Get("/lights/" + k);
                            if (cur_light_res->status == httplib::StatusCode::OK_200) {
                                cur_lights.update_light(k, cur_light_res->body);
                            } else if (cur_light_res->status == httplib::StatusCode::NotFound_404) {
                                // sometimes server returns deleted value, probably server bug so just skip it here
                                continue;
                            } else {
                                // in production grade code there should be used proper logging library 
                                cout << "warning: server returned non-200 code: " << cur_light_res->status << endl;
                            }
                        }

                        auto diff = lights_data.update(cur_lights);
                        for (auto& cur_diff : diff) {
                            if (!cur_diff.changes.empty() 
                             && holds_alternative<lights::removed_change>(cur_diff.changes.front()) ) {
                                cout << get<lights::removed_change>(cur_diff.changes.front()).name << "(" << cur_diff.id << ") has been removed" << endl;
                            } else {
                                nlohmann::json j;
                                j["id"] = cur_diff.id;
                                for (auto& change : cur_diff.changes) {
                                    if (holds_alternative<lights::name_change>(change))
                                        j["name"] = get<lights::name_change>(change).name;
                                    else if (holds_alternative<lights::room_change>(change))
                                        j["room"] = get<lights::room_change>(change).room;
                                    else if (holds_alternative<lights::on_change>(change))
                                        j["on"] = get<lights::on_change>(change).on;
                                    else if (holds_alternative<lights::brightness_change>(change))
                                        j["brightness"] = get<lights::brightness_change>(change).brightness;
                                }

                                cout << j.dump(4) << endl;
                            }
                        }
                    } else {
                        cout << "warning: server returned non-200 code: " << all_lights_res->status << endl;
                    }

                } catch (const nlohmann::detail::exception& e) {
                    cout << "json error: " << e.what() << endl;
                }

                sleep(0.5);
            }
        }
    };
}