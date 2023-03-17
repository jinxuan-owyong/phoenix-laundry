#include "LaundryRoom/LaundryRoom.h"

namespace laundry {
    /**
     * @brief Construct a new Room:: Room object.
     *
     * @param _ids Machines present in the room.
     * @param _config Configuration for reading machine state and GPIO pins
     */
    Room::Room(std::vector<MachineID> _ids, config_t _config) {
        // filter unconfigured gpio pins
        std::vector<MachineID> valid;
        auto pred = [&_config](MachineID id) {
            return _config.pins[id] > 0;
        };
        std::copy_if(_ids.begin(), _ids.end(), std::back_inserter(valid), pred);

        this->config = _config;
        this->ids = valid;

        // attach gpio pin to machine
        for (auto& id : valid) {
            machines.insert({id, Machine(id, _config.pins[id])});
        }
    }

    /**
     * @brief Get machines claimed by user.
     *
     * @param userId The user's Telegram ID.
     * @return std::vector<MachineID>
     */
    std::vector<MachineID> Room::getClaimedMachines(String userId) {
        std::vector<MachineID> claimed;
        for (auto& pair : machines) {
            auto& id = pair.first;
            auto& machine = pair.second;
            if (machine.getCurrUser().getTelegramId() == userId) {
                claimed.emplace_back(id);
            }
        }
        return claimed;
    }

    /**
     * @brief Claim ownership of a machine.
     *
     * @param id The machine's ID.
     * @param user User instance.
     */
    void Room::claim(MachineID id, User user) {
        machines[id].claim(user);
    }

    /**
     * @brief Unclaim ownership of a machine, if owned.
     *
     * @param id The machine's ID.
     * @param user User instance.
     * @return bool true if successful, false otherwise
     */
    bool Room::unclaim(MachineID id, User user) {
        if (machines[id].getCurrUser() != user) {
            return false;
        }
        machines[id].unclaim();
        return true;
    }

    /**
     * @brief Fetch the status of the given machine id
     *
     * @param id The machine's ID.
     */
    void Room::setMachineStatus(MachineID id, MachineState state) {
        if (machines.find(id) == machines.end()) {
            return;
        }
        machines[id].setState(state);
    }

    MachineState Room::identifyState(int high, int low) {
        // identify machine state
        if (high >= config.THRESHOLD_CONSTANT) {
            return IN_USE;
        } else if (low >= config.THRESHOLD_CONSTANT) {
            return READY;
        }
        return FINISHING;
    }

    /**
     * @brief Read the status of the given machine id
     *
     * @param id The machine's ID.
     * @param debug Pointer to String to extract readings for debugging
     * @return MachineState
     */
    void Room::refreshMachines(String* debug) {
        for (auto& pair : machines) {
            int countHigh = 0, countLow = 0;
            auto& machine = pair.second;

            for (int i = 0; i < config.SCAN_NUM_READINGS; ++i) {
                int reading = analogRead(machine.getGpioPin());
                if (reading > config.SCAN_THRESHOLD) {
                    ++countHigh;
                } else {
                    ++countLow;
                }
                delay(config.SCAN_INTERVAL);
            }

            MachineState prevState = machine.getState();
            machine.setState(identifyState(countHigh, countLow));
            MachineState currState = machine.getState();

            // additional information
            if (debug != NULL && (prevState != currState)) {
                *debug += machine.getName() + " - " + machine.getStatusString(0) + "\n";
                *debug += "High: " + String(countHigh) + " ";
                *debug += "Low: " + String(countLow) + "\n";
            }
        }
        lastUpdated = millis();
    }

    /**
     * @brief Get the list of machines which have completed their cycle,
     *        but user has not been informed
     *
     * @return std::vector<Machine>
     */
    std::vector<Machine> Room::getCompletedMachines() {
        std::vector<Machine> result;
        for (auto& pair : machines) {
            auto& machine = pair.second;
            // user has yet to be notified, runs once
            bool hasCompletedCycle = machine.getCurrUser().getNotifyState() == CYCLE_COMPLETED;
            if (hasCompletedCycle) {
                machine.setUserNotified();
                result.emplace_back(machine);
            }
        }
        return result;
    }

    unsigned long Room::getLastUpdated() {
        return lastUpdated;
    }

    String Room::getRoomStatus() {
        String output = "*Laundry Room Status*";
        for (auto& pair : machines) {
            auto& machine = pair.second;
            bool isWasher = pair.first == ID_WASHER_A || pair.first == ID_WASHER_B;
            output += "\n\u2022 ";
            output += machine.getName();
            output += ": ";
            output += machine.getStatusString(isWasher ? CYCLE_LENGTH_WASHER
                                                       : CYCLE_LENGTH_DRYER);
        }
        return output;
    }

    std::vector<MachineID> Room::getMachineIds() {
        return ids;
    }
}
