---
layout: default
title: Electronics
---

# Electronics

This section provides details about the electronics used in the project.

Below is the schematic of the circuit used:

<a href="#circuit">
  <img src="images/circuit.png" alt="Fritzing Schematic" style="width:100%; max-width:800px; border-radius: 12px; cursor: pointer;">
</a>

<div id="circuit" class="lightbox">
  <a href="#">
    <img class="lightbox-content" src="images/circuit.png" alt="Fritzing Schematic">
  </a>
</div>

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

<style>
  .lightbox {
    display: none;
    position: fixed;
    z-index: 999;
    left: 0;
    top: 0;
    width: 100%;
    height: 100%;
    background-color: rgba(0,0,0,0.85);
    justify-content: center;
    align-items: center;
  }

  .lightbox:target {
    display: flex;
  }

  .lightbox a {
    display: flex;
    justify-content: center;
    align-items: center;
    width: 100%;
    height: 100%;
    text-decoration: none;
  }

  .lightbox-content {
    max-width: 90%;
    max-height: 90%;
    border-radius: 12px;
    box-shadow: 0 0 20px rgba(255,255,255,0.2);
  }

  .thumb {
    width: 100%;
    border-radius: 12px;
    cursor: pointer;
  }
</style>

<div style="display: flex; justify-content: space-between; text-align: center;">
  <div style="flex: 1; margin: 0 10px;">
    <a href="#img1">
      <img src="images/startup.png" alt="Afbeelding 1" class="thumb">
    </a>
    <p>Startup screen</p>
  </div>
  <div style="flex: 1; margin: 0 10px;">
    <a href="#img2">
      <img src="images/menu.png" alt="Afbeelding 2" class="thumb">
    </a>
    <p>Selection menu 2</p>
  </div>
  <div style="flex: 1; margin: 0 10px;">
    <a href="#img3">
      <img src="images/measurement.png" alt="Afbeelding 3" class="thumb">
    </a>
    <p>Measurement screen 3</p>
  </div>
</div>

<!-- Lightboxes -->
<div id="img1" class="lightbox">
  <a href="#">
    <img class="lightbox-content" src="images/startup.png" alt="Startup screen">
  </a>
</div>

<div id="img2" class="lightbox">
  <a href="#">
    <img class="lightbox-content" src="images/menu.png" alt="Menu screen">
  </a>
</div>

<div id="img3" class="lightbox">
  <a href="#">
    <img class="lightbox-content" src="images/measurement.png" alt="Measurement screen">
  </a>
</div>


