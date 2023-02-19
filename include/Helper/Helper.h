#ifndef HELPER_H
#define HELPER_H

#include "Arduino.h"

namespace helper {
    struct EnumClassHash {
        template <typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
    };
}

#endif  // HELPER_H