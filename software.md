---
layout: default
title: BatteryWise
---

# BatteryWise

<div class="categories">
  <span class="category">Engineering & Sustainability</span>
</div>

<div class="website-link">
  <a href="https://batterywise.github.io/batterywise/" target="_blank">Visit our site</a>
</div>

<div class="team-members">
  <h3>Team Members</h3>
  <ul>
    <li>Beau Forrez</li>
    <li>Tano Pannekoucke</li>
    <li>Elias Neels</li>
    <li>Thibaut Beck</li>
  </ul>
</div>

<h2>Code:</h2>

<div class="code-container">
  <pre><code class="language-cpp">#include &lt;Arduino.h&gt;
#include &lt;Wire.h&gt;
#include &lt;Adafruit_GFX.h&gt;
#include &lt;Adafruit_SSD1306.h&gt;
#include &lt;Fonts/FreeSansBold9pt7b.h&gt;
#include &lt;Fonts/FreeSansBold12pt7b.h&gt;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

#define SWITCH_PIN 18  
#define SELECT_PIN 5   
#define BACK_PIN 17    
#define BATTERY_PIN 14 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* menuItems[] = { "AA", "AAA", "CR2032" };
const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);

int selectedMenuItem = 0;
bool inSubMenu = false;

bool lastSwitchState = HIGH;
bool lastSelectState = HIGH;
bool lastBackState = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void updateMenu() {
    display.clearDisplay();
    display.setFont(&FreeSansBold9pt7b);
    display.setTextColor(SSD1306_WHITE);

    for (int i = 0; i &lt; menuLength; i++) {
        int yPos = 20 + (i * 20);  

        if (i == selectedMenuItem) {
            display.fillRect(5, yPos - 12, 118, 18, SSD1306_WHITE);  
            display.setTextColor(SSD1306_BLACK);  
        } else {
            display.setTextColor(SSD1306_WHITE);
        }
        
        display.setCursor(10, yPos);
        display.println(menuItems[i]);
    }

    display.display();
}

void showBatteryMeasurement() {
    display.clearDisplay();
    display.setFont(&FreeSansBold12pt7b);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println(menuItems[selectedMenuItem]);

    while (true) {
        int rawValue = analogRead(BATTERY_PIN);
        float voltage = rawValue * (3.3 / 4095.0); 

        display.fillRect(10, 40, 100, 20, SSD1306_BLACK);

        display.setCursor(10, 55);
        display.print(voltage, 2);
        display.println(" V");
        display.display();

        if (digitalRead(BACK_PIN) == LOW) {
            inSubMenu = false;
            updateMenu();
            break;
        }

        delay(500);
    }
}

void setup() {
    pinMode(SWITCH_PIN, INPUT_PULLUP);
    pinMode(SELECT_PIN, INPUT_PULLUP);
    pinMode(BACK_PIN, INPUT_PULLUP);
    pinMode(BATTERY_PIN, INPUT);

    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("OLED niet gevonden"));
        while (true);
    }

    display.clearDisplay();
    updateMenu();
}

void loop() {
    bool currentSwitchState = digitalRead(SWITCH_PIN);
    bool currentSelectState = digitalRead(SELECT_PIN);
    bool currentBackState = digitalRead(BACK_PIN);

    if (!inSubMenu && currentSwitchState == LOW && lastSwitchState == HIGH) {
        if (millis() - lastDebounceTime > debounceDelay) {
            selectedMenuItem = (selectedMenuItem + 1) % menuLength;
            updateMenu();
            lastDebounceTime = millis();
        }
    }

    if (!inSubMenu && currentSelectState == LOW && lastSelectState == HIGH) {
        if (millis() - lastDebounceTime > debounceDelay) {
            inSubMenu = true;
            showBatteryMeasurement();
            lastDebounceTime = millis();
        }
    }

    lastSwitchState = currentSwitchState;
    lastSelectState = currentSelectState;
    lastBackState = currentBackState;
}</code></pre>
</div>

<style>
.code-container {
  background-color: #f5f5f5;
  border-radius: 5px;
  padding: 15px;
  overflow-x: auto;
}

.team-members {
  margin: 20px 0;
}

.team-members ul {
  list-style-type: none;
  padding-left: 0;
}

.team-members li {
  margin-bottom: 5px;
}

.category {
  background-color: #e0e0e0;
  padding: 3px 8px;
  border-radius: 3px;
  font-size: 0.9em;
}

.website-link {
  margin: 15px 0;
}
</style>
