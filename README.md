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
int menuOffset = 0; // Track which items are currently visible
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
    
    // Create a text-based BatteryWise logo
    display.setFont(NULL);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    drawCenteredText("Battery", 10);  // Verplaatst naar boven
    
    // Draw smaller battery icon
    display.drawRect(48, 28, 32, 16, WHITE); // Kleinere batterij body
    display.drawRect(80, 32, 3, 8, WHITE);   // Kleinere batterij terminal
    display.fillRect(50, 30, 28, 12, WHITE); // Kleinere batterij level
    
    // "Wise" text met meer ruimte
    display.setTextSize(2);
    display.setTextColor(WHITE);
    drawCenteredText("Wise", 48);  // Positie ongewijzigd maar nu meer ruimte
    
    display.display();
    delay(3000);
}

void updateMenu() {
    display.clearDisplay();
    
    // Title at top
    display.setFont(NULL);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    drawCenteredText("SELECT BATTERY TYPE", 8);
    
    display.drawFastHLine(0, 16, SCREEN_WIDTH, WHITE);
    
    // Menu items - using smaller font
    display.setFont(&FreeSans9pt7b);
    
    // Update menuOffset to ensure the selected item is visible
    if (selectedMenuItem < menuOffset) {
      menuOffset = selectedMenuItem;
    } else if (selectedMenuItem >= menuOffset + maxVisibleItems) {
      menuOffset = selectedMenuItem - maxVisibleItems + 1;
    }
    
    // Draw visible menu items
    for (int i = 0; i < maxVisibleItems && i + menuOffset < menuLength; i++) {
      int actualIndex = i + menuOffset;
      int yPos = 28 + (i * 14); // Adjusted spacing for items
      
      if (actualIndex == selectedMenuItem) {
        // Highlight selected item with rounded rectangle
        display.fillRoundRect(10, yPos - 12, 108, 16, 4, WHITE);
        display.setTextColor(BLACK);
      } else {
        display.setTextColor(WHITE);
      }
      
      // Center the text within the item
      int16_t x1, y1;
      uint16_t w, h;
      display.getTextBounds(menuItems[actualIndex], 0, 0, &x1, &y1, &w, &h);
      display.setCursor((SCREEN_WIDTH - w) / 2, yPos);
      display.print(menuItems[actualIndex]);
    }
    
    // Draw scroll indicators at the right edge if needed
    display.setFont(NULL);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    // Up arrow if there are items above
    if (menuOffset > 0) {
      display.fillTriangle(120, 20, 116, 24, 124, 24, WHITE);
    }
    
    // Down arrow if there are items below
    if (menuOffset + maxVisibleItems < menuLength) {
      display.fillTriangle(120, 56, 116, 52, 124, 52, WHITE);
    }
        
    display.display();
  }

void showBatteryMeasurement() {
  display.clearDisplay();
  
  // Show selected battery type
  display.setFont(&FreeSansBold9pt7b);
  display.setTextColor(WHITE);
  drawCenteredText(menuItems[selectedMenuItem], 16);
  
  // Initial voltage display
  display.setFont(&FreeMono9pt7b);
  
  while (true) {
    int rawValue = analogRead(BATTERY_PIN);
    float voltage = rawValue * (3.3 / 4095.0);
    
    // Clear measurement area
    display.fillRect(0, 20, 128, 40, SSD1306_BLACK);
    
    // Draw battery icon with level
    display.drawRect(32, 28, 64, 16, SSD1306_WHITE);
    display.drawRect(96, 32, 4, 8, SSD1306_WHITE);
    
    // Calculate battery level based on battery type
    float maxVoltage = 1.5; // Default for AA/AAA/LR44
    
    if (strcmp(menuItems[selectedMenuItem], "CR2032") == 0) {
      maxVoltage = 3.0; // CR2032 is 3V
    } else if (strcmp(menuItems[selectedMenuItem], "9V") == 0) {
      maxVoltage = 9.0; // 9V battery
    }
    // AA, AAA and LR44 are already set to 1.5V by default
    
    // Map voltage to battery level width
    int levelWidth = map(voltage * 100, 0, maxVoltage * 100, 0, 60);
    levelWidth = constrain(levelWidth, 0, 60);
    
    // Fill battery level
    display.fillRect(34, 30, levelWidth, 12, SSD1306_WHITE);
    
    // Draw lightning bolt icon if voltage is high enough
    if (voltage > 0.5) {
      display.drawBitmap(88, 32, lightning_icon, 8, 8, WHITE);
    }
    
    // Display voltage text
    display.setTextColor(SSD1306_WHITE);
    char voltString[10];
    sprintf(voltString, "%0.2fV", voltage);
    
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(voltString, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 54);
    display.print(voltString);
    
    // Add status text based on voltage
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
  
  // Show the startup screen with logo
  showStartupScreen();
  
  // Show the main menu
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
