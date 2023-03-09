#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> 

#define SCLK 13
#define MOSI 11
#define CS 10
#define DC 8
#define BL 7

Adafruit_ST7735 tft = Adafruit_ST7735(CS, DC, MOSI, SCLK, BL);

const int sampleWindow = 50;
unsigned int sample;

#define SENSOR_PIN 14
#define PIN_QUIET 4
#define PIN_MODERATE 5
#define PIN_LOUD 6


void setup() {
  pinMode(SENSOR_PIN, INPUT);
  pinMode(PIN_QUIET, OUTPUT);
  pinMode(PIN_MODERATE, OUTPUT);
  pinMode(PIN_LOUD, OUTPUT);
  digitalWrite(PIN_QUIET, LOW);
  digitalWrite(PIN_MODERATE, LOW);
  digitalWrite(PIN_LOUD, LOW);

  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB); // Initialize the LCD
  tft.fillScreen(ST7735_BLACK); // Clear the screen
  tft.setCursor(0, 0); // Set the cursor position
  tft.setTextSize(2); // Set the text size
  tft.setTextColor(ST7735_WHITE); // Set the text color
  digitalWrite(PIN_LOUD, LOW);
}

void loop()
{
  unsigned long startMillis = millis();
  float peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(SENSOR_PIN);
    if (sample < 1024)
    {
      if (sample > signalMax)
      {
        signalMax = sample;
      }
      else if (sample < signalMin)
      {
        signalMin = sample;
      }
    }
  }
  peakToPeak = signalMax - signalMin;
  int db = map(peakToPeak, 0, 900, 49, 90);
  Serial.print("\t");
  Serial.println(db);
  tft.setCursor(0, 0);
  tft.print("Loudness: ");
  tft.print(db);
  tft.print("dB");
  digitalWrite(PIN_LOUD, LOW);
  if (db <= 55)
  {
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0, 1);
    tft.print("Level:Quite");
    //tft.display();
    digitalWrite(PIN_QUIET, HIGH);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, LOW);
    //  delay(3000);
  }
  else if (db > 60 && db < 85)
  {
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0, 1);
    tft.print("Level:Moderate");
   // tft.display();
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, HIGH);
    digitalWrite(PIN_LOUD, LOW);
  }
  else if (db >= 85 && db <= 90)
  {
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0, 1);
    tft.print("Level:High");
    //tft.display();
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, HIGH);
  }
  else
  {
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, LOW);
  }
  delay(200);
}