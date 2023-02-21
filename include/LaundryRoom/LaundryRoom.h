#ifndef LAUNDRY_ROOM_H
#define LAUNDRY_ROOM_H

#include "LaundryRoom/LaundryConfig.h"
#include "LaundryRoom/LaundryMachine.h"

namespace laundry {
    class Room {
    private:
        config_t config;
        unsigned long lastUpdated = 0;
        std::unordered_map<MachineID, Machine, helper::EnumClassHash> machines;
        std::vector<MachineID> ids;
        MachineState identifyState(int high, int low);

    public:
        Room(std::vector<MachineID> _ids = {}, config_t _config = laundry::config_t());
        void claim(MachineID id, User user);
        bool unclaim(MachineID id, User user);
        void refreshMachines(String* debug);
        std::vector<MachineID> getClaimedMachines(String userId);
        std::vector<Machine> getCompletedMachines();
        unsigned long getLastUpdated();
        String getRoomStatus();
        std::vector<MachineID> getMachineIds();
        void setMachineStatus(MachineID id, MachineState state);
    };
}

#endif  // LAUNDRY_ROOM_H
