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
        String output = "*Laundry Bot Help Menu*\n";
        output += "Commands:\n";
        output += "/claim - Tag a dryer/washer to your telegram handle\n";
        output += "/help - Show this message\n";
        output += "/start - Initialise bot\n";
        output += "/status - Get the status of the laundry room";
        return output;
    }

    String response_start(String name) {
        return "Hello " + name + ", welcome to the Laundry Bot! ";
    }

    String response_status(laundry::Room& rm) {
        String output = "*Laundry Room Status*";
        for (auto& machine : rm.machines) {
            output += "\n\u2022 ";
            output += machine.get_name();
            output += ": ";
            output += machine.get_status();
        }
        return output;
    }
}
