# ESP32-asyncServer-data
This repository contains an implementation of an asynchronous HTTP webserver for an ESP32 microcontroller. The ESP32, connected to a WiFi network, is used to control a LED as well as read and process the values from a thermistor and a photoresistor.

The communication between the clients and the async webserver is done through [websockets](https://en.wikipedia.org/wiki/WebSocket). [JSON](https://www.json.org/json-en.html) is used as data format and [charts.js](https://www.chartjs.org/) for data visualization.

## Website appearance
<img src="./ESP32-asyncServer-data_screenshot.png" alt="Website appearance" width="100%" height="auto">

## Schematic
The following parts have been used in the electronic circuit. Nevertheless, they can be replaced by other similar ones with little change in the code.
- 1x LED.
- 1x MF52D NTC thermistor.
- 1x photoresistor.
- 2x 1 k<span>&#8486;</span> resistances.
- 1x 10 k<span>&#8486;</span> resistance.

<img src="./ESP32-web-data_schem.svg" alt="Electrical schematic" width="100%" height="auto">
ESP32 pins in the schematic are referred as 3.3V, GND and D*.

*Note: ESP32 ADC2 pins cannot be used when Wi-Fi is used.*

## Requirements
ESP32 libraries:
- [WiFi.h](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)

External libraries:
- [ArduinoJson](https://arduinojson.org). JSON library for embedded C++.
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP). Async TCP Library for ESP32.
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer). Async HTTP and WebSocket Server.

## Usage
1. Build the circuit as shown in the schematic.
2. Install the standard ESP32 libraries either using the Arduino IDE or directly from [Espressif](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/).
3. Install the required external libraries.
4. Download the repository and rename the file *constants.h.TEMPLATE* to *constants.h*.
    - Change the SSID and PASSWORD correspoding to the WiFi network.
    - Change the IP address and gateway if necessary in *constants.h*, *index.h* and *websocketclient.py*.
5. Flash the ESP32:
    - For VSCode, modify the *arduino.json* and *c_cpp_properties.json* files to add the missing libraries.
    - For the Arduino IDE, change the *src* folder to match the name of the *.ino* file.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[GPL-3.0](https://choosealicense.com/licenses/gpl-3.0/)