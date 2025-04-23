---
title: Site
layout: page
---

## Requirements

To run this project properly, please ensure the following software is installed on your system:

- [Node.js](https://nodejs.org/)
- [Docker](https://www.docker.com/) or [Docker Desktop](https://www.docker.com/products/docker-desktop)

## Installation and Usage

Follow these steps to correctly start the application:

1. **Start Docker.**  
   Make sure Docker or Docker Desktop is running before proceeding with any other steps.

2. **Navigate to the project's root directory.**  
   Open a terminal and move to the root folder of this project.

3. **Install Node.js dependencies.**

   ```bash
   npm install
   ```

4. **Start the application.**  
   You can use one of the following commands to start the app:

   ```bash
   node ./bin/www
   ```

   or

   ```bash
   npm start
   ```

5. **Stopping the application.**  
   To terminate the process, simply press:

   ```bash
   CTRL + C
   ```

## Recommendations 
The site will recommend other uses for you (almost) empty batteries 

<table style="width:100%; border-collapse: collapse; font-family: Arial, sans-serif; font-size: 14px;">
  <thead>
    <tr style="background-color: #f2f2f2;">
      <th style="border: 1px solid #ccc; padding: 8px;">Battery Type</th>
      <th style="border: 1px solid #ccc; padding: 8px;">Voltage Range</th>
      <th style="border: 1px solid #ccc; padding: 8px;">✅ Still Suitable For</th>
      <th style="border: 1px solid #ccc; padding: 8px;">❌ No Longer Suitable For</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td style="border: 1px solid #ccc; padding: 8px;">AA / AAA</td>
      <td style="border: 1px solid #ccc; padding: 8px;">1.2V – 1.4V</td>
      <td style="border: 1px solid #ccc; padding: 8px;">
        - TV/audio remotes<br>
        - LED tea lights<br>
        - Clocks (wall clocks, alarms, watches)<br>
        - Bike lights<br>
        - Wireless keyboards/mice<br>
        - Alarms / portable thermometers<br>
        - Simple calculators
      </td>
      <td style="border: 1px solid #ccc; padding: 8px;">
        - Wireless speakers<br>
        - Game controllers<br>
        - Digital cameras
      </td>
    </tr>
    <tr>
      <td style="border: 1px solid #ccc; padding: 8px;">LR44</td>
      <td style="border: 1px solid #ccc; padding: 8px;">1.2V – 1.4V</td>
      <td style="border: 1px solid #ccc; padding: 8px;">
        - LED tea lights<br>
        - Bike lights<br>
        - Clocks (wall clocks, alarms, watches)<br>
        - Car/garage remote controls
      </td>
      <td style="border: 1px solid #ccc; padding: 8px;">
        - Laser pointers<br>
        - Digital thermometers
      </td>
    </tr>
    <tr>
      <td style="border: 1px solid #ccc; padding: 8px;">CR2032</td>
      <td style="border: 1px solid #ccc; padding: 8px;">2.5V – 2.8V</td>
      <td style="border: 1px solid #ccc; padding: 8px;">
        - LED tea lights<br>
        - Bike lights<br>
        - Clocks (wall clocks, alarms, watches)<br>
        - Car/garage remote controls
      </td>
      <td style="border: 1px solid #ccc; padding: 8px;">
        - Smart keys<br>
        - Computer BIOS<br>
        - Precision sensors
      </td>
    </tr>
  </tbody>
</table>


