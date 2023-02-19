#include "LaundryRoom/LaundryUser.h"

namespace laundry {
    /**
     * @brief Construct a new User:: User object.
     *
     * @param _name User's name.
     * @param _id User's ID.
     * @param _username User's username.
     */
    User::User(String _name, String _telegramId, String _username) {
        this->name = _name;
        this->telegramId = _telegramId;
        this->username = _username;
    }

    String User::getName() {
        return name;
    }

    String User::getTelegramId() {
        return telegramId;
    }

    String User::getUsername() {
        return username;
    }

    UserState User::getNotifyState() {
        return notifyState;
    }

    void User::setNotifyState(UserState state) {
        notifyState = state;
    }

    bool User::isValidUser() {
        return !name.isEmpty();
    }

    bool User::operator==(User user) {
        return telegramId == user.getTelegramId();
    }

    bool User::operator!=(User user) {
        return !(*this == user);
    }
}
