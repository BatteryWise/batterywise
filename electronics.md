---
layout: default
title: Electronics
---

# Electronics

This section provides details about the electronics used in the project.

Below is the schematic of the circuit used:

![Fritzing Schematic](images/circuit.png)

### Circuit Overview:
The schematic shows the connections between the different components.
Summary:
  - GND and 3V3 to the breadboard's rails
  - Three buttons, each with one pin to GND:
    - GPIO18
    - GPIO5
    - GPIO17
  - For the OLED-screen:
    - GND to GND
    - 3V3 to 3V3
    - SCK to GPIO22
    - SDA to GPIO21
  - For the batteryprobe:
    - GPIO14 for the ADC-connection ( positive terminal of the battery )
    - GND ( negative terminal of the battery )
