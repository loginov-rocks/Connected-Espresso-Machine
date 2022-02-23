# Connected Espresso Machine

**Connected Espresso Machine** is a primer on _how to turn "dumb" old coffeemaker/teapot/anything into a smart connected
device_ with a bit of hardware work and programming. For that, a series of [articles](#articles) was written for you to
either have fun or implement your own similar project!

During the implementation of embedded projects, I faced different approaches to code firmware, so in the articles, I
tried to describe _common programming patterns_ that can be accommodated in your hobby projects.

In the current reincarnation, the espresso machine is connected to the home Wi-Fi network and exposes
**[REST API](#openapi)** that is used by the **Web App** to control the espresso machine accessible from the same
network.

**[Web Assets](#web-assets)** (JavaScript and CSS) are served from the **AWS S3 Bucket** and can be updated at any time
without the update of the firmware. While the firmware itself can be flashed _Over-the-Air_ from **PlatformIO IDE**.

While **NodeMCU** is used as a single microcontroller, the implementation is not specific to it and actually, **Arduino
Uno** was used initially to control the AC components of the espresso machine.

## Contents

1. [Articles](#articles)
2. [Architecture](#architecture)
    1. [REST](#rest)
3. [Hardware](#hardware)
    1. [Controller Board Scheme](#controller-board-scheme)
    2. [AC Scheme](#ac-scheme)
4. [Firmware](#firmware)
    1. [Class Diagram](#class-diagram)
5. [OpenAPI](#openapi)
6. [Web Assets](#web-assets)
    1. [Test Firmware](#test-firmware)
7. [Reference](#reference)
8. [Gists](#gists)

## Articles

Series: [Medium](https://loginov-rocks.medium.com/list/diy-connected-espresso-machine-c9576e4bc43a)

1. Archeology: [Medium](https://loginov-rocks.medium.com/diy-connected-espresso-machine-archeology-part-1-3d8c374b0f32)
2. Relays: [Medium](https://loginov-rocks.medium.com/diy-connected-espresso-machine-relays-part-2-2a070d81ffd2)
3. Boiler: [Medium](https://loginov-rocks.medium.com/diy-connected-espresso-machine-boiler-part-3-db0dcd764f6)
4. Toggle: [Medium](https://loginov-rocks.medium.com/diy-connected-espresso-machine-toggle-part-4-689a737b9f36)
5. Main Class and Indicators:
   [Medium](https://loginov-rocks.medium.com/diy-connected-espresso-machine-main-class-and-indicators-part-5-2f7f45d01c31)
6. Over-the-Air Updates:
   [Medium](https://loginov-rocks.medium.com/diy-connected-espresso-machine-over-the-air-updates-part-6-76ae32736f73)
7. Assembly: [Medium](https://loginov-rocks.medium.com/diy-connected-espresso-machine-assembly-part-7-7b590ef343fd)
8. REST and Web App: Medium...

## Architecture

### REST

![REST Architecture](https://raw.githubusercontent.com/loginov-rocks/Connected-Espresso-Machine/main/docs/Architecture/REST.png)

## Hardware

### Controller Board Scheme

![Controller Board Scheme](https://raw.githubusercontent.com/loginov-rocks/Connected-Espresso-Machine/main/docs/Hardware/Schemes/5-Final-NodeMCU.png)

### AC Scheme

![AC Scheme](https://raw.githubusercontent.com/loginov-rocks/Connected-Espresso-Machine/main/docs/Hardware/Schemes/AC-Scheme.png)

## Firmware

### Class Diagram

![Firmware Class Diagram](https://raw.githubusercontent.com/loginov-rocks/Connected-Espresso-Machine/main/docs/Firmware/Class-Diagram.png)

## OpenAPI

Requires [Node.js](https://nodejs.org):

```shell
cd openapi
npm install
npm run build
npm start
```

Open `http://localhost:3000` in browser and explore `openapi.yaml`

## Web Assets

Requires [Node.js](https://nodejs.org):

```shell
cd web-assets
npm install
npm start
```

Open `http://localhost:3001` in browser.

### Test Firmware

```shell
npm run start:test-firmware
```

This will start **Test Firmware** listening at `http://localhost:3002` and sourcing **Web Assets** from
`http://localhost:3001`

Actual **Firmware** can be tested by opening the **Connected Espresso Machine** root page with scripts and styles URLs
overrides in query parameters, like so:

```
http://192.168.1.1:80/?scriptsUrl=http%3A%2F%2Flocalhost%3A3001%2Fscripts.js&stylesUrl=http%3A%2F%2Flocalhost%3A3001%2Fstyles.css
```

Where `192.168.1.1` is *local IP* assigned to **Connected Espresso Machine** and `80` is *HTTP port* configured in
**Firmware**.

## Reference

* [Quick start with NodeMCU v3 (ESP8266), Arduino ecosystem, and PlatformIO IDE](https://loginov-rocks.medium.com/quick-start-with-nodemcu-v3-esp8266-arduino-ecosystem-and-platformio-ide-b8415bf9a038)

## Gists

* DIY Connected Espresso Machine: Relays (Part 2)
    * [Relay.h](https://gist.github.com/loginov-rocks/4bfa4750ed7d8e2273f5402d58d906c6)
    * [Relay.cpp](https://gist.github.com/loginov-rocks/0e8f73218224bcbe169dc9a4ae883bb1)
    * [main.cpp](https://gist.github.com/loginov-rocks/4ea1348e0f6a6a279ee8fb320b93696c)
* DIY Connected Espresso Machine: Boiler (Part 3)
    * [Boiler.h](https://gist.github.com/loginov-rocks/bf00abfa41bdae9f0755f1e1da09a3b7)
    * [Boiler.cpp](https://gist.github.com/loginov-rocks/97928385cdc14b2b8685e260fd0301ec)
    * [main.cpp](https://gist.github.com/loginov-rocks/a6ee1173ae0b32c9ba575e358a4aa1e1)
* DIY Connected Espresso Machine: Toggle (Part 4)
    * [Toggle.h](https://gist.github.com/loginov-rocks/c9e36bb92d1792c3f6f7c3f3043665ad)
    * [Toggle.cpp](https://gist.github.com/loginov-rocks/f31b9a23ab0bc96e5d136551c8843a2f)
    * [main.cpp](https://gist.github.com/loginov-rocks/ca233d7fce4953092067197c9220e12b)
* DIY Connected Espresso Machine: Main Class and Indicators (Part 5)
    * [EspressoMachineCommand](https://gist.github.com/loginov-rocks/4d169d05a190bed989a5177bbab5b3b2)
    * [EspressoMachine Header](https://gist.github.com/loginov-rocks/5bcba13a50df0c58ede9b2d18fece709)
    * [EspressoMachine Implementation, General Part](https://gist.github.com/loginov-rocks/475e93c029d1173bec8faf8ba5853d02)
    * [EspressoMachine Implementation, Commands Part](https://gist.github.com/loginov-rocks/041f81c4351a6c8e26f3fedd84651e93)
    * [EspressoMachine Implementation, Work Part](https://gist.github.com/loginov-rocks/c7e9d3b286da0336fb337f38f9aad5bc)
    * [Make Coffee Command, Header](https://gist.github.com/loginov-rocks/2659beb02cc3ef7a0d4887b0c5a99738)
    * [Make Coffee Command, Implementation](https://gist.github.com/loginov-rocks/ec6bf5b8a4b12d6aa5301c269be3f26c)
    * [main.cpp](https://gist.github.com/loginov-rocks/1fc72c52acc9e6b7471a6ee0421f20cc)
* DIY Connected Espresso Machine: Over-the-Air Updates (Part 6)
    * [setupWiFi](https://gist.github.com/loginov-rocks/9061ed1d774c3a61113ba0d1052c8d8f)
    * [setupOtaUpdates](https://gist.github.com/loginov-rocks/cb05853f007baffa3f6ad5136fe81fc9)
    * [main.cpp](https://gist.github.com/loginov-rocks/40e418a9ec3267f09f5134f2ff425897)
    * [platformio.ini](https://gist.github.com/loginov-rocks/5ff42d9c28257ca0188627bac181f19c)
