
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "lights.hpp"

using namespace std;

TEST_CASE("main unit test") {
    joshai::lights l(R"(
        [
        {
            "id": "f06b0ed2-cc50-4edd-a0f8-5c98a3c9d151",
            "name": "Living Room Lamp Left",
            "room": "Living Room"
        },
        {
            "id": "2ed8eb8e-e38b-4f86-8b9d-cbaf37be7275",
            "name": "Kitchen Overheads",
            "room": "Kitchen"
        },
        {
            "id": "2c85bb59-c136-49f1-a429-f02f52b6c765",
            "name": "Pantry Light",
            "room": "Kitchen"
        },
        {
            "id": "edc1b691-a5af-4524-9b57-80341d90bfa2",
            "name": "Office Sconce 2",
            "room": "Office"
        },
        {
            "id": "9df68fec-06ac-46bf-ab61-57e9d4e963e8",
            "name": "Office Downlights",
            "room": "Office"
        },
        {
            "id": "1b1920d5-22f1-43aa-9d35-371b2075d33d",
            "name": "Porch Light",
            "room": "Porch"
        },
        {
            "id": "ad7e8bb4-ff0a-4b9f-b676-e94baf878e8f",
            "name": "Living Room Lamp Right",
            "room": "Living Room"
        },
        {
            "id": "af80e5c2-b235-471d-8df9-490703699eda",
            "name": "Kitchen Chandelier",
            "room": "Kitchen"
        },
        {
            "id": "56a00ec5-e3d5-4a6b-a2cf-6d88c8f6464c",
            "name": "Office Sconce 1",
            "room": "Office"
        },
        {
            "id": "6ff183b0-710d-40de-94a8-81584d8ae3e8",
            "name": "String Lights",
            "room": "Back Yard"
        }
        ]
    )");

    CHECK_EQ(l.get_lights().size(), 10);
    CHECK_NE(l.get_lights().find("af80e5c2-b235-471d-8df9-490703699eda"), l.get_lights().end());
    CHECK_EQ(l.get_lights().find("af80e5c2-b235-471d-8df9-490703699eda")->second.room, "Kitchen");
    CHECK_EQ(l.get_lights().find("af80e5c2-b235-471d-8df9-490703699eda")->second.name, "Kitchen Chandelier");
    CHECK_FALSE(l.get_lights().find("af80e5c2-b235-471d-8df9-490703699eda")->second.on);
    CHECK_EQ(l.get_lights().find("af80e5c2-b235-471d-8df9-490703699eda")->second.brightness, 0);

    CHECK_EQ(l.get_lights().find("doesn't-exist"), l.get_lights().end());

    auto l2 = l;

    l.update_light("af80e5c2-b235-471d-8df9-490703699eda", R"({"name": "Kitchen Chandelier",
                                                               "room": "Kitchen",
                                                               "on":   true,
                                                               "brightness": 120
                                                              })");

    CHECK_EQ(l.get_lights().find("af80e5c2-b235-471d-8df9-490703699eda")->second.brightness, 120);
    CHECK_EQ(l.get_lights().find("af80e5c2-b235-471d-8df9-490703699eda")->second.on, true);

    // hack to get non-const lights list
    using map_type = remove_const_t<remove_reference_t<decltype(declval<joshai::lights>().get_lights())>>;
    auto& lmap = const_cast<map_type&>(l.get_lights());
    lmap.erase("6ff183b0-710d-40de-94a8-81584d8ae3e8");

    auto diff = l2.update(l);

    CHECK_EQ(diff.size(), 2);
    CHECK_EQ(diff[0].id, "af80e5c2-b235-471d-8df9-490703699eda");
    CHECK_EQ(diff[0].changes.size(), 2);
    CHECK(holds_alternative<joshai::lights::on_change>(diff[0].changes[0]));
    CHECK_EQ(get<joshai::lights::on_change>(diff[0].changes[0]).on, true);
    CHECK(holds_alternative<joshai::lights::brightness_change>(diff[0].changes[1]));
    CHECK_EQ(get<joshai::lights::brightness_change>(diff[0].changes[1]).brightness, 120);
    
    CHECK_EQ(diff[1].id, "6ff183b0-710d-40de-94a8-81584d8ae3e8");
    CHECK_EQ(diff[1].changes.size(), 1);
    CHECK(holds_alternative<joshai::lights::removed_change>(diff[1].changes[0]));
    CHECK_EQ(get<joshai::lights::removed_change>(diff[1].changes[0]).name, "String Lights");

    lmap["61493fb5-e837-43c2-8f25-bac09c35043d"] = {.name = "My new light",
                                                    .room = "Living Room",
                                                    .on   = true,
                                                    .brightness = 50};
    
    diff = l2.update(l);

    CHECK_EQ(diff.size(), 1);
    CHECK_EQ(diff[0].id, "61493fb5-e837-43c2-8f25-bac09c35043d");
    CHECK_EQ(diff[0].changes.size(), 4);
    CHECK(holds_alternative<joshai::lights::name_change>(diff[0].changes[0]));
    CHECK_EQ(get<joshai::lights::name_change>(diff[0].changes[0]).name, "My new light");
    CHECK(holds_alternative<joshai::lights::room_change>(diff[0].changes[1]));
    CHECK_EQ(get<joshai::lights::room_change>(diff[0].changes[1]).room, "Living Room");
    CHECK(holds_alternative<joshai::lights::on_change>(diff[0].changes[2]));
    CHECK_EQ(get<joshai::lights::on_change>(diff[0].changes[2]).on, true);
    CHECK(holds_alternative<joshai::lights::brightness_change>(diff[0].changes[3]));
    CHECK_EQ(get<joshai::lights::brightness_change>(diff[0].changes[3]).brightness, 50);
}