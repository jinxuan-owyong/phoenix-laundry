#include "LaundryRoom/LaundryMachine.h"

namespace laundry {
    Machine::Machine(MachineID _id, int _gpioPin) {
        this->id = _id;
        this->gpioPin = _gpioPin;
    }

    MachineState Machine::getState() {
        return currState;
    }

    void Machine::setState(MachineState state) {
        prevState = currState;
        currState = state;
        lastUpdated = millis();

        if (isNewCycle()) {
            cycleStartTime = millis();
        }

        // machine started, new cycle is unclaimed
        if (isNewCycle() && currUser.getNotifyState() == NOTIFIED) {
            claim(User());  // clear previous user
        }

        if (hasCompletedCycle()) {
            // user is notified on cycle completion
            currUser.setNotifyState(CYCLE_COMPLETED);
        }
    }

    bool Machine::hasCompletedCycle() {
        return (prevState == FINISHING) && (currState == READY);
    }

    bool Machine::isNewCycle() {
        return (prevState == READY) && (currState == IN_USE);
    }

    User Machine::getCurrUser() {
        return currUser;
    }

    int Machine::getGpioPin() {
        return gpioPin;
    }

    String Machine::getName() {
        return getNameById(id);
    }

    void Machine::claim(User user) {
        prevUser = currUser;
        currUser = user;
    }

    void Machine::unclaim() {
        currUser = prevUser;
        prevUser = User();
    }

    void Machine::setUserNotified() {
        currUser.setNotifyState(NOTIFIED);
    }

    String Machine::getStatusString(int cycleDuration) {
        String output = "";

        unsigned long cycleTimeMillis = millis() - cycleStartTime;
        int cycleTimeMins = cycleTimeMillis / 1000 / 60;
        if (currState != READY && cycleTimeMins <= cycleDuration) {
            output += String(cycleDuration - cycleTimeMins) + " minute(s) remaining";
        } else {
            output += getStatusById(READY);
        }

        if (currUser.isValidUser()) {
            output += "\n  👤 ";
            output += currUser.getName();
            output += " (@" + currUser.getUsername() + ")";
        }

        return output;
    }

    String Machine::getNameById(MachineID id) {
        switch (id) {
        case ID_DRYER_A:
            return "Coin Dryer";
        case ID_DRYER_B:
            return "QR Dryer";
        case ID_WASHER_A:
            return "QR Washer";
        case ID_WASHER_B:
            return "Coin Washer";
        }
        return "Unknown";
    }

    String Machine::getStatusById(MachineState state) {
        switch (state) {
        case IN_USE:
            return "In use";
        case FINISHING:
            return "Finishing";
        case READY:
            return "Ready";
        }
        return "Unknown";
    }
}
