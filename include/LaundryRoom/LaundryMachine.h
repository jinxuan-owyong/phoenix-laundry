#ifndef LAUNDRY_MACHINE_H
#define LAUNDRY_MACHINE_H

#include <vector>

#include "Arduino.h"
#include "LaundryRoom/LaundryConfig.h"
#include "LaundryRoom/LaundryUser.h"

namespace laundry {
    class Machine {
    public:
        int id;
        int curr_status = ID_READY;
        int prev_status = ID_FINISHING;
        int status_updated = UPDATED;
        unsigned long last_updated = 0;
        std::vector<User> users;
        Machine(int name_id);
        String get_name();
        String get_status();
        String get_user_id();
        int get_status_updated();
        void set_status_updated(int state);
        bool has_completed_cycle();
    };
}
#endif  // LAUNDRY_USER_H
