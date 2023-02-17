#ifndef LAUNDRY_USER_H
#define LAUNDRY_USER_H

#include "Arduino.h"

namespace laundry {
    class User {
    public:
        User(String _name = "", String _id = "", String _username = "");
        String name;
        String id;
        String username;
        bool notified = false;
    };
}
#endif  // LAUNDRY_USER_H
