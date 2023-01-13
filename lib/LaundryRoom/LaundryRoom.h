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
        User(String _name = "", String _id = "", String _username = "");
        String name;
        String id;
        String username;
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
        String claim(int machine_id, User u);
        String unclaim(int machine_id, User u);
        String get_machine_status(int machine_id);
        void set_machine_status(int machine_id, int status);
        int refresh_machine_status(int machine_id, String* debug = NULL);
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

    // ADC 2 is reserved for WiFi
    static std::unordered_map<int, int> MACHINE_INPUT_PIN = {
        {laundry::ID_DRYER_A, 32},  // only dryer a in use
        {laundry::ID_DRYER_B, 0},
        {laundry::ID_WASHER_A, 0},
        {laundry::ID_WASHER_B, 0}};

    static const int SCAN_INTERVAL = 100;     // time taken for each scan
    static const int SCAN_THRESHOLD = 1600;   // threshold for LED state
    static const int SCAN_NUM_READINGS = 10;  // total readings to take
    static const int THRESHOLD_CONSTANT = 7;
    static const int THRESHOLD_BLINKING = 2;  // blink frequency = 1/second
}
#endif  // LAUNDRY_ROOM_H
