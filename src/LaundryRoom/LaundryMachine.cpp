#include "LaundryRoom/LaundryMachine.h"

namespace laundry {
    /**
     * @brief Construct a new Machine:: Machine object.
     *
     * @param name_id Unique ID of machine
     */
    Machine::Machine(int name_id) {
        id = name_id;
        users.assign(2, User());
    }

    /**
     * @brief Get the string corresponding to the machine's ID.
     *
     * @return String
     */
    String Machine::get_name() {
        return MACHINE_NAME[id];
    };

    /**
     * @brief Get the string corresponding to the status' ID
     *
     * @return String
     */
    String Machine::get_status() {
        String output = MACHINE_STATUS[curr_status];
        // add user's details if machine is claimed
        if (users[CURR_USER].name != "") {
            output += " (";
            output += users[CURR_USER].name;
            output += " - @" + users[CURR_USER].username;
            output += ")";
        }

        return output;
    }

    /**
     * @brief Get the string corresponding to the machine's ID.
     *
     * @return String
     */
    String Machine::get_user_id() {
        return users[CURR_USER].id;
    };

    /**
     * @brief Check if the machine cycle is complete.
     *
     * @return bool
     */
    bool Machine::has_completed_cycle() {
        bool completed = (prev_status == ID_FINISHING) && (curr_status == ID_READY);
        if (completed && status_updated == READY) {
            status_updated = AWAITING;  // sends message to user when awaiting
        }
        return completed;
    }

    /**
     * @brief Get whether the user has been informed on the cycle completion.
     *
     * @return bool
     */
    int Machine::get_status_updated() {
        return status_updated;
    }

    /**
     * @brief Set whether the user has been informed on the cycle completion.
     *
     * @param state The desired state.
     */
    void Machine::set_status_updated(int state) {
        status_updated = state;
    }
}
