#ifndef LAUNDRY_ROOM_H
#define LAUNDRY_ROOM_H

#include <Arduino.h>

#include <unordered_map>
#include <vector>

#include "LaundryRoom/LaundryConfig.h"
#include "LaundryRoom/LaundryMachine.h"

namespace laundry {
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
}
#endif  // LAUNDRY_ROOM_H
