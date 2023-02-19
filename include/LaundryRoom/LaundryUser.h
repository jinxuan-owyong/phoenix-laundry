#ifndef LAUNDRY_USER_H
#define LAUNDRY_USER_H

#include "LaundryRoom/LaundryConfig.h"

namespace laundry {
    class User {
    private:
        String name;
        String telegramId;
        String username;
        UserState notifyState = AWAITING_COMPLETION;

    public:
        User(String _name = "", String _telegramId = "", String _username = "");
        String getName();
        UserState getNotifyState();
        String getTelegramId();
        String getUsername();
        void setNotifyState(UserState state);
        bool isValidUser();
        bool operator==(User user);
        bool operator!=(User user);
    };
}
#endif  // LAUNDRY_USER_H
