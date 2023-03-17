#ifndef LAUNDRY_CONFIG_H
#define LAUNDRY_CONFIG_H

#include <Helper/Helper.h>

#include <unordered_map>
#include <vector>

#include "Arduino.h"

namespace laundry {
    enum MachineID {
        UNDEFINED,
        ID_DRYER_A,
        ID_DRYER_B,
        ID_WASHER_A,
        ID_WASHER_B
    };

    enum MachineState {
        FINISHING,
        IN_USE,
        READY
    };

    enum UserState {
        AWAITING_COMPLETION,
        CYCLE_COMPLETED,
        NOTIFIED
    };

    static const int CYCLE_LENGTH_WASHER = 30;
    static const int CYCLE_LENGTH_DRYER = 45;

    struct config_t {
        int SCAN_INTERVAL = 10;       // time taken for each scan
        int SCAN_THRESHOLD = 1600;    // threshold for LED state (12-bit ADC)
        int SCAN_NUM_READINGS = 100;  // total readings to take
        int THRESHOLD_CONSTANT = 70;

        // ADC input pins
        // ADC 2 is reserved for WiFi, use ADC 1 pins
        std::unordered_map<MachineID, int, helper::EnumClassHash> pins;

        config_t(){};
        config_t(int _PIN_DRYER_A, int _PIN_DRYER_B, int _PIN_WASHER_A, int _PIN_WASHER_B) {
            pins[ID_DRYER_A] = _PIN_DRYER_A;
            pins[ID_DRYER_B] = _PIN_DRYER_B;
            pins[ID_WASHER_A] = _PIN_WASHER_A;
            pins[ID_WASHER_B] = _PIN_WASHER_B;
        }
    };
}

#endif
