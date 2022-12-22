#ifndef LAUNDRY_ROOM_H
#define LAUNDRY_ROOM_H

#include <Arduino.h>

#include <unordered_map>
#include <vector>

namespace laundry {
    enum {
        PREV_USER = 0,
        CURR_USER = 1,
        // Status IDs
        ID_IN_USE,
        ID_FINISHING,
        ID_DONE,
        ID_READY,
        ID_OUT_OF_ORDER,
        // Machine IDs
        ID_DRYER_A,
        ID_DRYER_B,
        ID_WASHER_A,
        ID_WASHER_B
    };

    class User {
    public:
        User(String n = "", String i = "");
        String name;
        String id;
    };

    class Machine {
    public:
        int id;
        int status = ID_READY;
        std::vector<User> users;
        Machine(int name_id);
        String get_name();
        String get_status();
    };

    class Room {
    public:
        Room(std::vector<int> machine_ids);
        std::vector<Machine> machines;
        std::vector<Machine> get_claimed_machines(String id);
        void claim(int machine_id, User u);
        void unclaim(int machine_id, User u);
    };
}
#endif  // LAUNDRY_ROOM_H
