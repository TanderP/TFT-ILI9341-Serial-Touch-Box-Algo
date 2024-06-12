#include <Arduino.h>
#include "SPI.h"
#include "TFT_eSPI.h"

#define BL 4

// TFT display instance
TFT_eSPI tft = TFT_eSPI();

// Touch coordinates
uint16_t t_x, t_y, t_z;

// Function to check if a value is within a specified range
bool isInRange(int value, int min, int max) {
  return (value >= min && value <= max);
}

// Parameters for touch areas
const int numColumns = 3;        // Number of columns of touchable areas
const int numRows = 1;           // Number of rows of touchable areas
const int areaWidth = 50;        // Width of each touchable area
const int areaHeight = 100;      // Height of each touchable area
const int spacingX = 20;         // Horizontal spacing between touchable areas
const int spacingY = 20;         // Vertical spacing between touchable areas

// User-defined touchscreen dimensions
int touchScreenWidth = 400;      // Change this value to match your touchscreen width
int touchScreenHeight = 400;     // Change this value to match your touchscreen height

void setup() {
  Serial.begin(115200);
  Serial.println("ESP BEGIN");

  // Initialize backlight
  pinMode(BL, OUTPUT);
  digitalWrite(BL, HIGH);

  // Initialize TFT display
  tft.init();
  tft.fillScreen(TFT_WHITE);
}

void loop() {
  // Get raw touch coordinates
  tft.getTouchRaw(&t_x, &t_y);
  t_x = t_x / 10; // Scale touch x coordinate
  t_y = t_y / 10; // Scale touch y coordinate
  t_z = tft.getTouchRawZ(); // Get touch pressure

  // Print touch coordinates and pressure to the Serial Monitor
  Serial.printf("x: %i     ", t_x);
  Serial.printf("y: %i     ", t_y);
  Serial.printf("z: %i \n", t_z);

  // Calculate starting X coordinate for the first column of touch areas to position them in the middle
  int totalAreasWidth = numColumns * areaWidth + (numColumns - 1) * spacingX;
  int startX = (touchScreenWidth - totalAreasWidth) / 2;

  // Calculate starting Y coordinate for the first row of touch areas to position them in the middle vertically
  int totalAreasHeight = numRows * areaHeight + (numRows - 1) * spacingY;
  int startY = (touchScreenHeight - totalAreasHeight) / 2;

  // Loop through each touch area to check if the touch is within any of them
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numColumns; col++) {
      int areaStartX = startX + col * (areaWidth + spacingX);
      int areaEndX = areaStartX + areaWidth;
      int areaStartY = startY + row * (areaHeight + spacingY);
      int areaEndY = areaStartY + areaHeight;

      if (isInRange(t_x, areaStartX, areaEndX) && isInRange(t_y, areaStartY, areaEndY)) {
        int areaIndex = row * numColumns + col + 1;
        Serial.printf("Touch detected in area %d\n", areaIndex);
      }
    }
  }

  delay(200); // Wait for 200 milliseconds before the next loop
}
