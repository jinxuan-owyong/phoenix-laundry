#ifndef LAUNDRY_CONFIG_H
#define LAUNDRY_CONFIG_H

#include <unordered_map>

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
        ID_WASHER_B,
        // Machine update status
        READY,
        AWAITING,
        UPDATED
    };

    struct config_t {
        int SCAN_INTERVAL = 10;       // time taken for each scan
        int SCAN_THRESHOLD = 1600;    // threshold for LED state (12-bit ADC)
        int SCAN_NUM_READINGS = 100;  // total readings to take
        int THRESHOLD_CONSTANT = 70;

        int PIN_DRYER_A;  // ADC input pins
        int PIN_DRYER_B;  // ADC 2 is reserved for WiFi, use ADC 1 pins
        int PIN_WASHER_A;
        int PIN_WASHER_B;
        config_t() {}
        config_t(int _PIN_DRYER_A, int _PIN_DRYER_B, int _PIN_WASHER_A, int _PIN_WASHER_B) {
            PIN_DRYER_A = _PIN_DRYER_A;
            PIN_DRYER_B = _PIN_DRYER_B;
            PIN_WASHER_A = _PIN_WASHER_A;
            PIN_WASHER_B = _PIN_WASHER_B;
        }
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

#endif
