#ifndef LAUNDRY_ROOM_H
#define LAUNDRY_ROOM_H

#include <Arduino.h>

#include <unordered_map>
#include <vector>

namespace laundry {
    enum {
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

    class Machine {
    private:
        int id;
        int status = ID_READY;
        String prev_user = "";

    public:
        Machine(int name_id);
        String get_name();
        String get_status();
    };

    class Room {
    private:
        std::vector<Machine> machines;

    public:
        Room(std::vector<int> machine_ids);
        int num_machines;
        String generate_status();
    };
}
#endif  // LAUNDRY_ROOM_H
