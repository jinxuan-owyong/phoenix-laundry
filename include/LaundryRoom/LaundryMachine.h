#ifndef LAUNDRY_MACHINE_H
#define LAUNDRY_MACHINE_H

#include "LaundryRoom/LaundryConfig.h"
#include "LaundryRoom/LaundryUser.h"

namespace laundry {
    class Machine {
    private:
        MachineID id;
        int gpioPin = -1;
        MachineState prevState = FINISHING;
        MachineState currState = READY;
        unsigned long lastUpdated = 0;
        unsigned long cycleStartTime = 0;
        User currUser;
        User prevUser;

    public:
        Machine(MachineID _id = UNDEFINED, int _gpioPin = -1);
        static String getNameById(MachineID id);
        static String getStatusById(MachineState state);
        User getCurrUser();
        int getGpioPin();
        String getName();
        MachineState getState();
        String getStatusString(int cycleDuration);
        void setState(MachineState state);
        void setUserNotified();
        bool hasCompletedCycle();
        bool isNewCycle();
        void claim(User user);
        void unclaim();
    };
}

#endif  // LAUNDRY_MACHINE_H
