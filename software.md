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
<!DOCTYPE html>
<html>
<head>
<title>Software</title>
<style>
body {
    font-family: monospace;
    background-color: #f4f4f4;
    color: #333;
    line-height: 1.6;
    margin: 20px;
}

h1 {
    color: #0056b3;
}

pre {
    background-color: #f0f0f0;
    padding: 10px;
    border-radius: 5px;
    overflow-x: auto;
}

code {
    display: block;
    white-space: pre-wrap;
    word-break: break-all;
}

.code-block {
    background-color: #e0e0e0;
    padding: 10px;
    border-radius: 5px;
    overflow-x: auto;
}
</style>
</head>
<body>

<h1>Software</h1>

<p>Het softwaregedeelte van dit project kan worden gedaan in Arduino IDE, maar ons advies is om <a href="https://docs.platformio.org/en/latest/integration/ide/vscode.html" target="_blank">PlatformIO</a> in VS Code te gebruiken om de ESP32 te programmeren. Je hebt deze twee bibliotheken nodig:</p>

<ul>
    <li>Adafruit_GFX.h</li>
    <li>Adafruit_SSD1306.h</li>
</ul>

<p>Beide kunnen eenvoudig worden geïnstalleerd binnen PlatformIO.</p>

<h2>Code</h2>

<div class="code-block">
<code>
#include &lt;Arduino.h&gt;<br>
#include &lt;Wire.h&gt;<br>
#include &lt;Adafruit_GFX.h&gt;<br>
#include &lt;Adafruit_SSD1306.h&gt;<br>
#include &lt;Fonts/FreeSansBold9pt7b.h&gt;<br>
#include &lt;Fonts/FreeSans9pt7b.h&gt;<br>
#include &lt;Fonts/FreeMono9pt7b.h&gt;<br>
<br>
#define SCREEN_WIDTH 128<br>
#define SCREEN_HEIGHT 64<br>
#define OLED_RESET -1<br>
#define SCREEN_ADDRESS 0x3C<br>
<br>
#define SWITCH_PIN 18<br>
#define SELECT_PIN 5<br>
#define BACK_PIN 17<br>
#define BATTERY_PIN 14<br>
<br>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);<br>
<br>
const char* menuItems[] = {"AA", "AAA", "CR2032", "LR44", "9V"};<br>
const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);<br>
const int maxVisibleItems = 3; // Maximum number of items visible at once<br>
<br>
int selectedMenuItem = 0;<br>
int menuOffset = 0; // Offset for scrolling<br>
bool inSubMenu = false;<br>
<br>
bool lastSwitchState = HIGH;<br>
bool lastSelectState = HIGH;<br>
bool lastBackState = HIGH;<br>
<br>
unsigned long lastDebounceTime = 0;<br>
const unsigned long debounceDelay = 50;<br>
<br>
const unsigned char lightning_icon[] PROGMEM = {<br>
&nbsp; 0x08, 0x0C, 0x0E, 0x0F, 0x1F, 0x03, 0x06, 0x04<br>
};<br>
<br>
void drawCenteredText(const char* text, int y) {<br>
&nbsp; int16_t x1, y1;<br>
&nbsp; uint16_t w, h;<br>
&nbsp; display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);<br>
&nbsp; display.setCursor((SCREEN_WIDTH - w) / 2, y);<br>
&nbsp; display.print(text);<br>
}<br>
<br>
void showStartupScreen() {<br>
&nbsp; display.clearDisplay(); <br>
&nbsp; display.setFont(NULL);<br>
&nbsp; display.setTextSize(2);<br>
&nbsp; display.setTextColor(WHITE);<br>
&nbsp; drawCenteredText("Battery", 10);<br>
&nbsp; display.drawRect(48, 28, 32, 16, WHITE);<br>
&nbsp; display.drawRect(80, 32, 3, 8, WHITE);<br>
&nbsp; display.fillRect(50, 30, 28, 12, WHITE);<br>
&nbsp; display.setTextSize(2);<br>
&nbsp; display.setTextColor(WHITE);<br>
&nbsp; drawCenteredText("Wise", 48);<br>
&nbsp; display.display();<br>
&nbsp; delay(3000);<br>
}<br>
<br>
void updateMenu() {<br>
&nbsp; display.clearDisplay();<br>
&nbsp; display.setFont(NULL);<br>
&nbsp; display.setTextSize(1);<br>
&nbsp; display.setTextColor(WHITE);<br>
&nbsp; drawCenteredText("SELECT BATTERY TYPE", 8);<br>
&nbsp; display.drawFastHLine(0, 16, SCREEN_WIDTH, WHITE);<br>
&nbsp; display.setFont(&FreeSans9pt7b);<br>
&nbsp; if (selectedMenuItem &lt; menuOffset) {<br>
&nbsp; &nbsp; menuOffset = selectedMenuItem;<br>
&nbsp; } else if (selectedMenuItem &gt;= menuOffset + maxVisibleItems) {<br>
&nbsp; &nbsp; menuOffset = selectedMenuItem - maxVisibleItems + 1;<br>
&nbsp; }<br>
&nbsp; for (int i = 0; i &lt; maxVisibleItems && i + menuOffset &lt; menuLength; i++) {<br>
&nbsp; &nbsp; int actualIndex = i + menuOffset;<br>
&nbsp; &nbsp; int yPos = 28 + (i * 14);<br>
&nbsp; &nbsp; if (actualIndex == selectedMenuItem) {<br>
&nbsp; &nbsp; &nbsp; display.fillRoundRect(10, yPos - 12, 108, 16, 4, WHITE);<br>
&nbsp; &nbsp; &nbsp; display.setTextColor(BLACK);<br>
&nbsp; &nbsp; } else {<br>
&nbsp; &nbsp; &nbsp; display.setTextColor(WHITE);<br>
&nbsp; &nbsp; }<br>
&nbsp; &nbsp; int16_t x1, y1;<br>
&nbsp; &nbsp; uint16_t w, h;<br>
&nbsp; &nbsp; display.getTextBounds(menuItems[actualIndex], 0, 0, &x1, &y1, &w, &h);<br>
&nbsp; &nbsp; display.setCursor((SCREEN_WIDTH - w) / 2, yPos);<br>
&nbsp; &nbsp; display.print(menuItems[actualIndex]);<br>
&nbsp; }<br>
&nbsp; display.setFont(NULL);<br>
&nbsp; display.setTextSize(1);<br>
&nbsp; display.setTextColor(WHITE);<br>
&nbsp; if (menuOffset &gt; 0) {<br>
&nbsp; &nbsp; display.fillTriangle(120, 20, 116, 24, 124, 24, WHITE);<br>
&nbsp; }<br>
&nbsp; if (menuOffset + maxVisibleItems &lt; menuLength) {<br>
&nbsp; &nbsp; display.fillTriangle(120, 56, 116, 52, 124, 52, WHITE);<br>
&nbsp; }<br>
&nbsp; display.display();<br>
}<br>
<br>
void setup() {<br>
&nbsp; pinMode(SWITCH_PIN, INPUT_PULLUP);<br>
&nbsp; pinMode(SELECT_PIN, INPUT_PULLUP);<br>
&nbsp; pinMode(BACK_PIN, INPUT_PULLUP);<br>
&nbsp; pinMode(BATTERY_PIN, INPUT);  <br>
&nbsp; Serial.begin(115200);  <br>
&nbsp; if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {<br>
&nbsp; &nbsp; Serial.println(F("OLED not found"));<br>
&nbsp; &nbsp; while (true);<br>
&nbsp; }<br>
&nbsp; showStartupScreen();<br>
&nbsp; updateMenu();<br>
}<br>
<br>
void loop() {<br>
&nbsp; bool currentSwitchState = digitalRead(SWITCH_PIN);<br>
&nbsp; bool currentSelectState = digitalRead(SELECT_PIN);<br>
&nbsp; bool currentBackState = digitalRead(BACK_PIN);  <br>
&nbsp; if (!inSubMenu && currentSwitchState == LOW && lastSwitchState == HIGH) {<br>
&nbsp; &nbsp; if (millis() - lastDebounceTime &gt; debounceDelay) {<br>
&nbsp; &nbsp; &nbsp; selectedMenuItem = (selectedMenuItem + 1) % menuLength;<br>
&nbsp; &nbsp; &nbsp; updateMenu();<br>
&nbsp; &nbsp; &nbsp; lastDebounceTime = millis();<br>
&nbsp; &nbsp; }<br>
&nbsp; }  <br>
&nbsp; if (!inSubMenu && currentSelectState == LOW && lastSelectState == HIGH) {<br>
&nbsp; &nbsp; if (millis() - lastDebounceTime &gt; debounceDelay) {<br>
&nbsp; &nbsp; &nbsp; inSubMenu = true;<br>
&nbsp; &nbsp; &nbsp; showBatteryMeasurement();<br>
&nbsp; &nbsp; &nbsp; lastDebounceTime = millis();<br>
&nbsp; &nbsp; }<br>
&nbsp; }  <br>
&nbsp; lastSwitchState = currentSwitchState;<br>
&nbsp; lastSelectState = currentSelectState;<br>
&nbsp; lastBackState = currentBackState;<br>
}<br>
</code>
</div>

</body>
</html>
