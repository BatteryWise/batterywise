# BatteryWise
 Engineering & Sustainability  

 Visit our [site](https://batterywise.github.io/batterywise/)
 
**Beau Forrez**  
**Tano Pannekoucke**  
**Elias Neels**  
**Thibaut Beck**  

Code:
```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>

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

    for (int i = 0; i < menuLength; i++) {
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
}
