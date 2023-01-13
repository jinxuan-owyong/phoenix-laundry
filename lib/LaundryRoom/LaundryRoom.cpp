#include "LaundryRoom.h"

namespace laundry {
    /**
     * @brief Construct a new User:: User object.
     *
     * @param n User's name.
     * @param i User's ID.
     * @param u User's username.
     */
    User::User(String _name, String _id, String _username) {
        name = _name;
        id = _id;
        username = _username;
    }

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
     * @brief Get the string corresponding to the status' ID.
     *
     * @return String
     */
    String Machine::get_status() {
        String output = MACHINE_STATUS[status];
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
     * @brief Construct a new Room:: Room object.
     *
     * @param machine_ids Machines present in the room.
     */
    Room::Room(std::vector<int> machine_ids) {
        for (auto& id : machine_ids) {
            machines.emplace_back(Machine(id));
        }
    }

    /**
     * @brief Get machines claimed by user.
     *
     * @param id The user's ID.
     * @return std::vector<Machine>
     */
    std::vector<Machine> Room::get_claimed_machines(String id) {
        std::vector<laundry::Machine> claimed;
        for (auto& m : machines) {
            if (m.users[laundry::CURR_USER].id == id) {
                claimed.emplace_back(m);
            }
        }
        return claimed;
    }

    /**
     * @brief Claim ownership of a machine.
     *
     * @param machine_id The machine's ID.
     * @param u User instance.
     * @return String Name of the machine claimed.
     */
    String Room::claim(int machine_id, User u) {
        String claimed = "";
        for (auto& m : machines) {
            if (m.id == machine_id) {
                std::swap(m.users[PREV_USER], m.users[CURR_USER]);
                m.users[CURR_USER] = u;
                claimed = m.get_name();
            }
        }
        return claimed;
    }

    /**
     * @brief Unclaim ownership of a machine, if owned.
     *
     * @param machine_id The machine's ID.
     * @param u User instance.
     * @return String Name of the machine unclaimed, or message if failed.
     */
    String Room::unclaim(int machine_id, User u) {
        String unclaimed = "";
        for (auto& m : machines) {
            if (m.id == machine_id &&
                u.id == m.users[CURR_USER].id) {  // confirm user's id matches machine
                unclaimed = m.get_name();
                m.users[CURR_USER] = User();
                std::swap(m.users[PREV_USER], m.users[CURR_USER]);
            }
        }
        return unclaimed;
    }

    /**
     * @brief Fetch the status of the given machine id
     *
     * @param machine_id The machine's ID.
     * @return String
     */
    String Room::get_machine_status(int machine_id) {
        for (auto& m : machines) {
            if (m.id == machine_id) {
                return m.get_status();
            }
        }
        return "Unknown";
    }

    /**
     * @brief Fetch the status of the given machine id
     *
     * @param machine_id The machine's ID.
     * @param status The status of the machine
     */
    void Room::set_machine_status(int machine_id, int status) {
        for (auto& m : machines) {
            if (m.id == machine_id) {
                m.status = status;
                break;
            }
        }
    }

    /**
     * @brief Refresh the status of the given machine id
     *
     * @param machine_id The machine's ID.
     * @return int
     */
    int Room::refresh_machine_status(int machine_id, String* debug) {
        int count_high = 0;
        int count_low = 0;
        for (int i = 0; i < SCAN_NUM_READINGS; ++i) {
            int reading = analogRead(MACHINE_INPUT_PIN[machine_id]);
            if (reading > SCAN_THRESHOLD) {
                ++count_high;
            } else {
                ++count_low;
            }
            delay(SCAN_INTERVAL);
        }

        // additional information
        if (debug != NULL) {
            *debug += "High: " + String(count_high) + " ";
            *debug += "Low: " + String(count_low);
        }

        // identify machine state
        if (abs(count_high - count_low) <= THRESHOLD_BLINKING) {
            return laundry::ID_FINISHING;
        } else if (count_high >= THRESHOLD_CONSTANT) {
            return laundry::ID_IN_USE;
        } else if (count_low >= THRESHOLD_CONSTANT) {
            return laundry::ID_READY;
        }
        return laundry::ID_OUT_OF_ORDER;
    }
}
