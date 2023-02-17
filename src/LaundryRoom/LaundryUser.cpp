#include "LaundryRoom/LaundryUser.h"

namespace laundry {
    /**
     * @brief Construct a new User:: User object.
     *
     * @param _name User's name.
     * @param _id User's ID.
     * @param _username User's username.
     */
    User::User(String _name, String _id, String _username) {
        name = _name;
        id = _id;
        username = _username;
    }
}
