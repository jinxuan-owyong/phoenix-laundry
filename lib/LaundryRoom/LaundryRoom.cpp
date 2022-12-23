#include "LaundryRoom.h"

namespace laundry {
    /**
     * @brief Construct a new User:: User object.
     *
     * @param n User's name.
     * @param i User's ID.
     * @param u User's username.
     */
    User::User(String n, String i, String u) {
        name = n;
        id = i;
        username = u;
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
        std::unordered_map<int, String> MACHINE_NAME = {
            {ID_DRYER_A, "Coin Dryer"},
            {ID_DRYER_B, "QR Dryer"},
            {ID_WASHER_A, "QR Washer"},
            {ID_WASHER_B, "Coin Washer"}};
        return MACHINE_NAME[id];
    };

    /**
     * @brief Get the string corresponding to the status' ID.
     *
     * @return String
     */
    String Machine::get_status() {
        std::unordered_map<int, String> MACHINE_STATUS = {
            {ID_IN_USE, "In use"},
            {ID_FINISHING, "Finishing"},
            {ID_DONE, "Done"},
            {ID_READY, "Ready"},
            {ID_OUT_OF_ORDER, "Out of order"}};

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
}
