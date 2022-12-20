#include "LaundryRoom.h"

namespace laundry {
    Machine::Machine(int name_id) {
        id = name_id;
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
            output += prev_user_name;
            output += ")";
        }

        return output;
    }

    Room::Room(std::vector<int> machine_ids) {
        for (auto& id : machine_ids) {
            machines.emplace_back(Machine(id));
        }
    }

    void Room::claim(int machine_id, String user_name, String user_id) {
        for (auto& m : machines) {
            if (m.id == machine_id) {
                m.prev_user_name = user_name;
                m.prev_user_id = user_id;
            }
        }
    }
}
