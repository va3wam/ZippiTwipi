# ZippiTwipi
This repository contains all of the information regarding a DC motor based two wheeled inverted pendulum robot experimental platform. This initiative is running in parallel with the [TWIPe repository](https://github.com/va3wam/TWIPe/wiki). This repository contains C++ source code intended to run on the Adafruit Huzzah32 development board.  

## Getting Started

Full details about this project can be read on our [wiki](https://github.com/va3wam/ZippiTwipi/wiki). 

### Prerequisites

The software in this repository was written in the C++ (Arduino) language using Microsoft's Visual Source Code IDE and the library management was handled using the plugin PlatformIO. As such, source code files have the extenion "cpp" denoting c++ rather than the extension "ino" denoting arduino.   

### Code Compatability

The Arduino C++ code was written to run on the Espressif ESP WROOM32 "Software On a Chip" (SOC). The development board used for this SOC is the Adafruit Huzzah32 featherboard. Note that some standard Arduino functions such as AnalogWrite() have not been implemented on the ESP32 SOC and alternate functions such as ledcSetup(), ledcAttachPin() and ledcWrite() must be used in their place. There are some additional compatability issues of note as follows:

1. The GPIO pin usage is loosey Adafruit featherboard compliant meaning it will work with some boards such as the OLED board 
2. This code does not use the Twipe pinout configuration or naming convension so will not port over to a Twipe compliant robot unmodified
3. This code will NOT run on an ESP8266 unmodified because of both GPIO incompatability as well as some ESP32 specific libraries that are in use 

### Installing

There are numerous tutorials on how to install Arduino code onto an ESP WROOM32 from Microsoft's Visual Studio Code. Try out one of the following tutorials or find another one that is more to your liking:
* [MakerPro](https://maker.pro/arduino/tutorial/how-to-use-visual-studio-code-for-arduino)
* [Random Nerd](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/)

## Running the tests

At this time there are no automated tests for this application.

## Deployment

This Arduino code was developed using the following set up.

### Built With

* [Visual Studio Code](https://code.visualstudio.com/) - The IDE
* [Platformio](https://platformio.org/) - Editor plugin
* Library Framework - Arduino
* Hardware platform - espressif32
* Board - Adafruit Huzzah32 featherboard

<img src="./img/zippiTwipi.jpeg" alt="ZipiTwipi Robot" width="800" height="500"> 

### Arduino Libraries
The following additional libraries are used in the Arduino C++ code:

* The standard Arduino library that is bundled with PlatformIO
* The standard Wire library that is bundled with PlatformIO
* https://github.com/marcoschwartz/LiquidCrystal_I2C

## Contributing

This project is being developed by a couple of buddies. If you wish to get involved in this project feel free to contact us at one of the email addreses listed in the authors section below. All current and planned work for this project is tracked using the [Github projects feature](https://github.com/va3wam/ZippiTwipi/projects). Here are the standards that we follow for all va3wam projects:

* [Set up your local repo, Git workflow](https://github.com/va3wam/va3wam.github.io/wiki/Software-Version-Control)
* [Recommended Tool chain](https://github.com/va3wam/va3wam.github.io/wiki/Software-Languages-&-Tools)
* [va3wam coding standards](https://github.com/va3wam/va3wam.github.io/wiki/Software-Coding-Standards) 

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/va3wam/ZippiTwipi/tags). 

## Authors

* [Doug](https://github.com/nerdoug) 
* [va3wam](https://github.com/va3wam) 

See also the list of [contributors](https://github.com/va3wam/ZippiTwipi/settings/access) who participated in this project.

## License

This project is lisenced using the MIT Open Source Initiative which you can read [here](https://opensource.org/licenses/MIT) 

## Acknowledgments

* Joop Brokking and his [Balancing Robot](http://www.brokking.net/yabr_main.html)
* Tony DiCola and Adafruit Industries for their MQTT QOS1 Arduino library
* Jeff Rowberg for the MPU6050 DMP logic
* DFRobot & John Rickman for their LiquidCrystal I2C LCD display Arduino library
* The many folks involved in porting the Arduino libraries over to the ESP32