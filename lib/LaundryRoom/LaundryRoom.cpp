#include "LaundryRoom.h"

namespace laundry {
    User::User(String n, String i, String u) {
        name = n;
        id = i;
        username = u;
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
        if (users[CURR_USER].name != "") {
            output += " (";
            output += users[CURR_USER].name;
            output += " - @" + users[CURR_USER].username;
            output += ")";
        }

        return output;
    }

    Room::Room(std::vector<int> machine_ids) {
        for (auto& id : machine_ids) {
            machines.emplace_back(Machine(id));
        }
    }

    std::vector<Machine> Room::get_claimed_machines(String id) {
        std::vector<laundry::Machine> claimed;
        for (auto& m : machines) {
            if (m.users[laundry::CURR_USER].id == id) {
                claimed.emplace_back(m);
            }
        }
        return claimed;
    }

    String Room::claim(int machine_id, User u) {
        String claimed = "";
        for (auto& m : machines) {
            if (m.id == machine_id) {
                std::swap(m.users[PREV_USER], m.users[CURR_USER]);
                m.users[CURR_USER] = u;
                claimed = m.get_name();
            }
        }
        return claimed;
    }

    String Room::unclaim(int machine_id, User u) {
        String unclaimed = "";
        for (auto& m : machines) {
            if (m.id == machine_id &&
                u.id == m.users[CURR_USER].id) {  // confirm user's id matches machine
                unclaimed = m.get_name();
                m.users[CURR_USER] = User();
                std::swap(m.users[PREV_USER], m.users[CURR_USER]);
            }
        }
        return unclaimed;
    }
}
