---
title: Software
layout: page
---

# Software

The software section of this project could be done in Arduino IDE, but our advice is to use [PlatformIO](https://docs.platformio.org/en/latest/integration/ide/vscode.html){:target="_blank"} in VS Code to program the ESP32.
You need these two libraries:
- Adafruit_GFX.h
- Adafruit_SSD1306.h
Both can easily be installed within PlatformIO.

# Code
```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define SWITCH_PIN 18
#define SELECT_PIN 5
#define BACK_PIN 17
#define BATTERY_PIN 14

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* menuItems[] = {"AA", "AAA", "CR2032", "LR44", "9V"};
const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
const int maxVisibleItems = 3; // Maximum number of items visible at once

int selectedMenuItem = 0;
int menuOffset = 0; // Offset for scrolling
bool inSubMenu = false;

bool lastSwitchState = HIGH;
bool lastSelectState = HIGH;
bool lastBackState = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

const unsigned char lightning_icon[] PROGMEM = {
  0x08, 0x0C, 0x0E, 0x0F, 0x1F, 0x03, 0x06, 0x04
};

void drawCenteredText(const char* text, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, y);
  display.print(text);
}

void showStartupScreen() {
  display.clearDisplay(); 
  display.setFont(NULL);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  drawCenteredText("Battery", 10);
  display.drawRect(48, 28, 32, 16, WHITE);
  display.drawRect(80, 32, 3, 8, WHITE);
  display.fillRect(50, 30, 28, 12, WHITE);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  drawCenteredText("Wise", 48);
  display.display();
  delay(3000);
}

void updateMenu() {
  display.clearDisplay();
  display.setFont(NULL);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  drawCenteredText("SELECT BATTERY TYPE", 8);
  display.drawFastHLine(0, 16, SCREEN_WIDTH, WHITE);
  display.setFont(&FreeSans9pt7b);
  if (selectedMenuItem < menuOffset) {
    menuOffset = selectedMenuItem;
  } else if (selectedMenuItem >= menuOffset + maxVisibleItems) {
    menuOffset = selectedMenuItem - maxVisibleItems + 1;
  }
  for (int i = 0; i < maxVisibleItems && i + menuOffset < menuLength; i++) {
    int actualIndex = i + menuOffset;
    int yPos = 28 + (i * 14);
    if (actualIndex == selectedMenuItem) {
      display.fillRoundRect(10, yPos - 12, 108, 16, 4, WHITE);
      display.setTextColor(BLACK);
    } else {
      display.setTextColor(WHITE);
    }
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(menuItems[actualIndex], 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, yPos);
    display.print(menuItems[actualIndex]);
  }
  display.setFont(NULL);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  if (menuOffset > 0) {
    display.fillTriangle(120, 20, 116, 24, 124, 24, WHITE);
  }
  if (menuOffset + maxVisibleItems < menuLength) {
    display.fillTriangle(120, 56, 116, 52, 124, 52, WHITE);
  }
  display.display();
}

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(SELECT_PIN, INPUT_PULLUP);
  pinMode(BACK_PIN, INPUT_PULLUP);
  pinMode(BATTERY_PIN, INPUT);  
  Serial.begin(115200);  
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("OLED not found"));
    while (true);
  }
  showStartupScreen();
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
