---
layout: default
title: Electronics
---

# Electronics

This section provides details about the electronics used in the project.

Below is the schematic of the circuit used:

![Fritzing Schematic](images/circuit.png){:style="width:100%;max-width:800px;"}

The schematic shows the connections between the different components:

- **Power connections:**
  - GND and 3V3 are connected to the breadboard's rails.

- **Buttons:**
  - Three buttons are used, each connected to GND with the following GPIO pins:
    - **GPIO18**: scroll through the battery selection menu ( and send the measurement over MQTT when a battery was already selected )
    - **GPIO5**: go back to the battery selection menu
    - **GPIO17**: select the battery type

- **OLED screen:**
  - GND → **GND**
  - 3V3 → **3V3**
  - SCK → **GPIO22**
  - SDA → **GPIO21**

- **Battery probe:**
  - **GPIO14**: ADC connection to the positive terminal of the battery.
  - **GND**: Negative terminal of the battery.

### Summary of connections:

| Component           | Pin/Connection        | GPIO Pin         |
|---------------------|-----------------------|------------------|
| **Buttons**         | GND                   | GPIO18, GPIO5, GPIO17 |
| **OLED Screen**     | GND                   | GND              |
|                     | 3V3                   | 3V3              |
|                     | SCK                   | GPIO22           |
|                     | SDA                   | GPIO21           |
| **Battery Probe**   | Positive Terminal     | GPIO14 (ADC)     |
|                     | Negative Terminal     | GND              |

## Usage
Button connected to:
- **GPIO18**: scroll through battery selection menu ( and send measurement over MQTT when a battery was already selected )
- **GPIO5**: go back to battery selection meny
- **GPIO17**: select battery type

<div style="display: flex; justify-content: space-between; text-align: center;">
  <div style="flex: 1; margin: 0 10px;">
    <img src="images/startup.png" alt="Afbeelding 1" style="width: 100%; border-radius: 12px;">
    <p>Startup screen</p>
  </div>
  <div style="flex: 1; margin: 0 10px;">
    <img src="images/menu.png" alt="Afbeelding 2" style="width: 100%; border-radius: 12px;">
    <p>Selection menu 2</p>
  </div>
  <div style="flex: 1; margin: 0 10px;">
    <img src="images/measurement.png" alt="Afbeelding 3" style="width: 100%; border-radius: 12px;">
    <p>Measurement screen 3</p>
  </div>
</div>


