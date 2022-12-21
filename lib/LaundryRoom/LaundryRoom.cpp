#include "LaundryRoom.h"

namespace laundry {
    User::User(String n, String i) {
        name = n;
        id = i;
    }

    Machine::Machine(int name_id) {
        id = name_id;
        users.assign(2, User());
    }

    String Machine::get_name() {
        std::unordered_map<int, String> MACHINE_NAME = {
            {ID_DRYER_A, "Coin Dryer"},
            {ID_DRYER_B, "QR Dryer"},
            {ID_WASHER_A, "QR Washer"},
            {ID_WASHER_B, "Coin Washer"}};
        return MACHINE_NAME[id];
    };

    String Machine::get_status() {
        std::unordered_map<int, String> MACHINE_STATUS = {
            {ID_IN_USE, "In use"},
            {ID_FINISHING, "Finishing"},
            {ID_DONE, "Done"},
            {ID_READY, "Ready"},
            {ID_OUT_OF_ORDER, "Out of order"}};

        String output = MACHINE_STATUS[status];
        if (status == ID_IN_USE || status == ID_DONE) {
            output += " (";
            output += users[CURR_USER].name;
            output += ")";
        }

        return output;
    }

    Room::Room(std::vector<int> machine_ids) {
        for (auto& id : machine_ids) {
            machines.emplace_back(Machine(id));
        }
    }

    void Room::claim(int machine_id, User u) {
        for (auto& m : machines) {
            if (m.id == machine_id) {
                std::swap(m.users[PREV_USER], m.users[CURR_USER]);
                m.users[CURR_USER] = u;
            }
        }
    }
}
