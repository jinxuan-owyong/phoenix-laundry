#include "LaundryRoom.h"

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

    /**
     * @brief Construct a new Room:: Room object.
     *
     * @param machine_ids Machines present in the room.
     */
    Room::Room(std::vector<int> machine_ids, config_t config) {
        cfg = config;
        input_pin[ID_DRYER_A] = config.PIN_DRYER_A;
        input_pin[ID_DRYER_B] = config.PIN_DRYER_B;
        input_pin[ID_WASHER_A] = config.PIN_WASHER_A;
        input_pin[ID_WASHER_B] = config.PIN_WASHER_B;

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
     * @brief Fetch the name of the given machine id
     *
     * @param machine_id The machine's ID.
     * @return String
     */
    String Room::get_machine_name(int machine_id) {
        for (auto& m : machines) {
            if (m.id == machine_id) {
                return m.get_name();
            }
        }
        return "Unknown";
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
     * @brief Fetch the user's telegram id the given machine id
     *
     * @param machine_id The machine's ID.
     * @return String
     */
    String Room::get_user_id(int machine_id) {
        for (auto& m : machines) {
            if (m.id == machine_id) {
                return m.get_user_id();
            }
        }
        return "Unknown";
    }

    /**
     * @brief Set the status of the given machine id
     *
     * @param machine_id The machine's ID.
     * @param status The status of the machine
     */
    void Room::set_machine_status(int machine_id, int status) {
        for (auto& m : machines) {
            // reset user data on cycle start
            if (m.has_completed_cycle() && status == ID_IN_USE) {
                m.set_status_updated(READY);
                if (m.users[CURR_USER].notified) {  // new cycle unclaimed
                    m.users[PREV_USER] = m.users[CURR_USER];
                    m.users[CURR_USER] = User();
                }
            }

            if (m.id == machine_id) {
                // store previous status to check cycle completion
                m.prev_status = m.curr_status;
                m.curr_status = status;
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
        for (int i = 0; i < cfg.SCAN_NUM_READINGS; ++i) {
            int reading = analogRead(input_pin[machine_id]);
            if (reading > cfg.SCAN_THRESHOLD) {
                ++count_high;
            } else {
                ++count_low;
            }
            delay(cfg.SCAN_INTERVAL);
        }

        // additional information
        if (debug != NULL) {
            *debug += "High: " + String(count_high) + " ";
            *debug += "Low: " + String(count_low);
        }

        // identify machine state
        if (count_high >= cfg.THRESHOLD_CONSTANT) {
            return laundry::ID_IN_USE;
        } else if (count_low >= cfg.THRESHOLD_CONSTANT) {
            return laundry::ID_READY;
        }
        return laundry::ID_FINISHING;
    }

    /**
     * @brief Get the top reading from n readings
     *
     * @param n Number of readings
     * @param threshold Minimum readings to be considered accurate
     * @param machine_id The machine's ID.
     * @return int
     */
    int Room::get_best_result(int n, int threshold, int machine_id, String* debug) {
        std::unordered_map<int, int> results;
        results[laundry::ID_IN_USE] = 0;
        results[laundry::ID_FINISHING] = 0;
        results[laundry::ID_READY] = 0;
        // results[laundry::ID_OUT_OF_ORDER] = 0;

        for (int i = 0; i < n; ++i) {
            int state = refresh_machine_status(machine_id, debug);
            if (debug != NULL) {
                *debug += "Reading " + String(i) + ": " + String(state) + "\n";
            }
            ++results[state];
            delay(cfg.SCAN_INTERVAL);
        }

        auto best_result = results.begin();
        for (auto it = ++results.begin(); it != results.end(); ++it) {
            if (it->second >= threshold && it->second > best_result->second) {  // count >= threshold
                best_result = it;
            }
        }

        // possible bug: all results are not the best
        return best_result->first;
    }

    /**
     * @brief Get the list of machines which have completed their cycle,
     *        but user has not been informed
     *
     * @return std::vector<Machine>
     */
    std::vector<Machine> Room::get_completed_machines() {
        std::vector<Machine> result;
        for (auto& m : machines) {
            if (m.has_completed_cycle() &&
                m.get_status_updated() == AWAITING &&
                !m.users[CURR_USER].notified) {
                m.set_status_updated(UPDATED);
                // prevent double message if another user forgets to claim machine
                m.users[CURR_USER].notified = true;
                result.emplace_back(m);
            }
        }
        return result;
    }
}
