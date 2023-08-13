# Repo for using the PMW3901 with ESP8266

### PMW3901 Optical flow breakout board from Pimoroni
https://shop.pimoroni.com/products/pmw3901-optical-flow-sensor-breakout?variant=27869870358611
Arduino Library: https://github.com/bitcraze/Bitcraze_PMW3901



### ESP8266 Dev Board (Double check the pins I use)
start here if you're unfamiliar https://randomnerdtutorials.com/getting-started-with-esp8266-wifi-transceiver-review/
https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/


http://arduino.esp8266.com/stable/package_esp8266com_index.json 

libraries

New Ping - https://www.arduino.cc/reference/en/libraries/newping/



Bitcraze PMW3901 - https://www.arduino.cc/reference/en/libraries/bitcraze-pmw3901/

https://github.com/bitcraze/Bitcraze_PMW3901


working with the arduino-cli for the first time

https://arduino.github.io/arduino-cli/

    arduino-cli lib install "Bitcraze PMW3901"
    arduino-cli lib install "NewPing"
    arduino-cli lib install "ArduinoJson"

Thinking about the networking side of things

    git clone https://github.com/me-no-dev/ESPAsyncTCP.git
    git clone https://github.com/me-no-dev/ESPAsyncWebServer.git

    arduino-cli lib install "WebSockets"   https://github.com/Links2004/arduinoWebSockets
