#include "Telegram.h"

namespace telegram {
    String response_claim() {
        return "Which machine would you like to claim?";
    }

    String keyboard_claim(laundry::Room& rm) {
        String keyboardJson = "[";
        for (int i = 0; i < rm.machines.size(); ++i) {
            keyboardJson += "[{\"text\" : ";
            keyboardJson += "\"" + rm.machines[i].get_name() + "\",";
            keyboardJson += "\"callback_data\" : ";
            keyboardJson += "\"claim-" + String(rm.machines[i].id) + "\"";
            keyboardJson += "}]";

            if (i != rm.machines.size() - 1) {
                keyboardJson += ",";
            }
        }
        keyboardJson += "]";
        return keyboardJson;
    }

    String response_help() {
        return "\
*Laundry Bot Help Menu*\n\
Commands:\n\
/claim - Tag a dryer/washer to your telegram handle\n\
/help - Show this message\n\
/start - Initialise bot\n\
/help - Show this message\n";
    }

    String response_start(String name) {
        return "Hello " + name + ", welcome to the Laundry Bot! ";
    }
}
