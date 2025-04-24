# JClock Micro

An LED clock, indpired by TIX design, built with an ESP8266 microcontroller and an LED strip.

## Features

- **Automatic Time Synchronization**
  - Retrieves current time from NTP servers and adjusts for local time zone.
- **Customizable LED Mapping**
  - Defines LED indices for a 4-digit (HH:MM) display arrangement.
- **Console Output**
  - Prints a text-based clock representation and active LED indices to the Serial Monitor for debugging.
- **Easy Configuration**
  - Modify WiFi credentials, LED pin assignment, and pixel count directly in the code.

## Hardware Requirements

- **ESP8266-based Board** (e.g., NodeMCU, Wemos D1 Mini)
- **27× WS2812 (NeoPixel) LEDs**
- **5V Power Supply** capable of driving the LEDs
- **Jumper Wires** for connections

## Software Requirements

- **Arduino IDE** (version ≥ 1.8.0)
- **ESP8266 Board Support** installed via Boards Manager
- **Libraries** (install via Library Manager):
  - Adafruit NeoPixel
  - NTPClient
  - Time
  - Timezone

## Installation

1. **Clone the repository**

   ```bash
   git clone https://github.com/JacopoVisentin/JClock_Micro.git
   cd JClock_Micro
   ```

2. **Open the Project**
   - Launch `JClock_Micro.ino` in the Arduino IDE.
3. **Configure WiFi Credentials**

   ```cpp
   const char* SSID = "your_SSID";
   const char* PASS = "your_password";
   ```

4. **Adjust LED Settings**
   - In `clock_leds.cpp`, set the data pin and pixel count:

     ```cpp
     #define PIN        D4
     #define NUMPIXELS  27
     ```

5. **Install Dependencies**
   - Use the Library Manager to install required libraries.
6. **Select Board & Upload**
   - Choose your ESP8266 board and upload the sketch.

## Usage

- After successful upload, the device will connect to WiFi and synchronize the time.
- The LED arrangement will display the current hour and minute in a HH:MM format.
- Open the **Serial Monitor** (baud rate 115200) to view:
  - Text-based clock representation
  - Active LED indices
  - Current time
- **Manual Time Testing**: Uncomment the line in `loop()`:

  ```cpp
  // my_clock.setTime(17, 38); // Manual testing
  ```

## Project Structure

```bash
/JClock_Micro/
├── JClock_Micro.ino       // Main application
├── clock_display.hpp      // ClockDisplay class interface
├── clock_display.cpp      // ClockDisplay class implementation
├── clock_leds.hpp         // LED mapping definitions
├── clock_leds.cpp         // LED mapping implementation
├── utils.hpp              // Utility functions declarations
└── utils.cpp              // Utility functions implementations (NTP, time conversion, system utils)
```

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
