#ifndef LAUNDRY_ROOM_H
#define LAUNDRY_ROOM_H

#include <Arduino.h>

#include <unordered_map>
#include <vector>

#include "LaundryConfig.h"

namespace laundry {
    class User {
    public:
        User(String _name = "", String _id = "", String _username = "");
        String name;
        String id;
        String username;
        bool notified = false;
    };

    class Machine {
    public:
        int id;
        int curr_status = ID_READY;
        int prev_status = ID_FINISHING;
        int status_updated = UPDATED;
        std::vector<User> users;
        Machine(int name_id);
        String get_name();
        String get_status();
        String get_user_id();
        int get_status_updated();
        void set_status_updated(int state);
        bool has_completed_cycle();
    };

    class Room {
    private:
        config_t cfg;
        std::unordered_map<int, int> input_pin;

    public:
        Room(std::vector<int> machine_ids, config_t config);
        std::vector<Machine> machines;
        std::vector<Machine> get_claimed_machines(String id);
        String claim(int machine_id, User u);
        String unclaim(int machine_id, User u);
        String get_machine_name(int machine_id);
        String get_machine_status(int machine_id);
        String get_user_id(int machine_id);
        void set_machine_status(int machine_id, int status);
        int refresh_machine_status(int machine_id, String* debug = NULL);
        int get_best_result(int n, int threshold, int machine_id, String* debug = NULL);
        std::vector<Machine> get_completed_machines();
    };

    static std::unordered_map<int, String> MACHINE_STATUS = {
        {ID_IN_USE, "In use"},
        {ID_FINISHING, "Finishing"},
        {ID_DONE, "Done"},
        {ID_READY, "Ready"},
        {ID_OUT_OF_ORDER, "Out of order"}};

    static std::unordered_map<int, String> MACHINE_NAME = {
        {ID_DRYER_A, "Coin Dryer"},
        {ID_DRYER_B, "QR Dryer"},
        {ID_WASHER_A, "QR Washer"},
        {ID_WASHER_B, "Coin Washer"}};

}
#endif  // LAUNDRY_ROOM_H
