---
title: Software
layout: page
---

# Software

The software section of this project could be done in Arduino IDE, but our advice is to use [PlatformIO](https://docs.platformio.org/en/latest/integration/ide/vscode.html){:target="_blank"} in VS Code to program the ESP32.

# Battery Menu Code

This is the code used:

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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &amp;Wire, OLED_RESET);

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
    display.setFont(&amp;FreeSansBold9pt7b);
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
    display.setFont(&amp;FreeSansBold12pt7b);
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

    if (!inSubMenu &amp;&amp; currentSwitchState == LOW &amp;&amp; lastSwitchState == HIGH) {
        if (millis() - lastDebounceTime &gt; debounceDelay) {
            selectedMenuItem = (selectedMenuItem + 1) % menuLength;
            updateMenu();
            lastDebounceTime = millis();
        }
    }

    if (!inSubMenu &amp;&amp; currentSelectState == LOW &amp;&amp; lastSelectState == HIGH) {
        if (millis() - lastDebounceTime &gt; debounceDelay) {
            inSubMenu = true;
            showBatteryMeasurement();
            lastDebounceTime = millis();
        }
    }

    lastSwitchState = currentSwitchState;
    lastSelectState = currentSelectState;
    lastBackState = currentBackState;
}</code></pre>

<script>
document.addEventListener("DOMContentLoaded", function() {
    document.getElementById("copyButton").addEventListener("click", function() {
        const codeBlock = document.querySelector("pre code");
        navigator.clipboard.writeText(codeBlock.innerText).then(() => {
            alert("Code copied!");
        }).catch(err => console.error("Copy error:", err));
    });
});
</script>
