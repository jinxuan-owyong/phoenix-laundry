<!-- omit in toc -->
# CAPT Laundry Bot

<!-- omit in toc -->
## Table of Contents
- [Setup](#setup)
  - [Development Environment](#development-environment)
  - [Environment Variables](#environment-variables)

## Setup

### Development Environment
- This project requires [PlatformIO](https://platformio.org/) to build the source code and the required dependencies
- We recommend using the PlatformIO extension on Visual Studio Code

### Environment Variables 
- The environment variables are stored in the header file `include/Config.h`
- An example can be found at `include/Config.example.h`
- Set `WIFI_CONNECTION_TYPE` to either `WPA2_PERSONAL` or `WPA2_ENTERPRISE`, depending on WiFi network, and the corresponding credentials.
- Generate the Telegram bot token from [@BotFather](https://t.me/BotFather)