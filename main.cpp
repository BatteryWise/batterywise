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
  display.clearDisplay(); // Create a text-based BatteryWise logo
  display.setFont(NULL);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  drawCenteredText("Battery", 10);  // Verplaatst naar boven
  display.drawRect(48, 28, 32, 16, WHITE); // Kleinere batterij body
  display.drawRect(80, 32, 3, 8, WHITE);   // Kleinere batterij terminal
  display.fillRect(50, 30, 28, 12, WHITE); // Kleinere batterij level
  display.setTextSize(2);
  display.setTextColor(WHITE);
  drawCenteredText("Wise", 48);  // Positie ongewijzigd maar nu meer ruimte
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


void showBatteryMeasurement() {
  display.clearDisplay();
  display.setFont(&FreeSansBold9pt7b);
  display.setTextColor(WHITE);
  drawCenteredText(menuItems[selectedMenuItem], 16);
  display.fillRect(0, 20, 128, 40, SSD1306_BLACK);
  display.display();
  while (true) {
    display.setFont(&FreeMono9pt7b);
    int rawValue = analogRead(BATTERY_PIN);
    float voltage = rawValue * (3.3 / 4095.0);
    display.fillRect(0, 20, 128, 40, SSD1306_BLACK);
    display.drawRect(32, 28, 64, 16, SSD1306_WHITE);
    display.drawRect(96, 32, 4, 8, SSD1306_WHITE);
    float maxVoltage = 1.5; // Default for AA/AAA/LR44
    if (strcmp(menuItems[selectedMenuItem], "CR2032") == 0) {
      maxVoltage = 3.0; // CR2032 is 3V
    } else if (strcmp(menuItems[selectedMenuItem], "9V") == 0) {
      maxVoltage = 9.0; // 9V battery
    }
    int levelWidth = map(voltage * 100, 0, maxVoltage * 100, 0, 60);
    levelWidth = constrain(levelWidth, 0, 60);
    display.fillRect(34, 30, levelWidth, 12, SSD1306_WHITE);
    if (voltage > 0.5) {
      display.drawBitmap(88, 32, lightning_icon, 8, 8, WHITE);
    }
    display.setTextColor(SSD1306_WHITE);
    char voltString[10];
    sprintf(voltString, "%0.2fV", voltage);
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(voltString, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 57);
    display.print(voltString);
    display.setFont(NULL);
    display.setTextSize(1);
    float threshold = (maxVoltage * 0.7); // 70% as threshold
    if (voltage >= threshold) {
      display.setCursor(2, 20);
      display.print("STATUS: GOOD");
    } else if (voltage >= (maxVoltage * 0.3)) {
      display.setCursor(2, 20);
      display.print("STATUS: LOW");
    } else {
      display.setCursor(2, 20);
      display.print("STATUS: REPLACE");
    }
    display.display();    
    if (digitalRead(BACK_PIN) == LOW) {
      inSubMenu = false;
      delay(200); // Short delay to prevent bouncing
      updateMenu();
      break;
    }
    delay(300);
  }
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