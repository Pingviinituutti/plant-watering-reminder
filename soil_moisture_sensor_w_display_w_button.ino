// Display stuff

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// DISPLAY
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// TIME
// from https://forum.arduino.cc/t/arduino-timer-convert-millis-to-days-hours-min/42323
// macros from DateTime.h 
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)
 
/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  

// BUTTON
const byte ledPin = LED_BUILTIN;
const byte interruptPin = 2;
volatile byte state = LOW;

// SOIL MOISTURE STUFF
const int moisturePin = A2;
const int AirValue = 950;
const int WaterValue = 600;
int soilMoistureValue = 0;
int soilMoisturePercent = 0;
unsigned long soilMeasurementTimestamp = 0;
unsigned long soilMeasurementInterval = 2000;

int soilMoisturePercentageMin = 100;
unsigned long wateringTimestamp = 0;

void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps

  //  button stuff
  pinMode(moisturePin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonPress, CHANGE);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  soilMeasurementTimestamp = millis();
}

void buttonPress() {
  state = !state;
}

void displayMoisture(void) {
  display.clearDisplay();

  display.setCursor(0,0); 
  display.setTextSize(1.2);             // Draw 1.2X-scale text
  display.setTextColor(SSD1306_WHITE);

  display.print("Moist. %: ");
  display.print(soilMoisturePercent);
  display.println("%");
  
  display.print("Sensor value ");
  display.println(soilMoistureValue);
}

void displayTimeSinceWatering(int days, int hours, int minutes) {
  display.print("Time since watering: ");
  display.print(days);
  display.print(" days, ");
  display.print(hours);
  display.print("h ");
  display.print(minutes);
  display.println("min");
}

void measureMoisture(void) {
    soilMoistureValue = analogRead(moisturePin); 
    soilMoisturePercent = constrain(map(soilMoistureValue, AirValue, WaterValue, 0, 100), 0 , 100);
    // Interpret a 10% increase from the minimum measurement as the sensor being watered
    if (soilMoisturePercent > soilMoisturePercentageMin + 10) {
      Serial.println("Soil has been watered!");
      soilMoisturePercentageMin = soilMoisturePercent;
      wateringTimestamp = millis();
    } else {
      soilMoisturePercentageMin = (soilMoisturePercent < soilMoisturePercentageMin ? soilMoisturePercent : soilMoisturePercentageMin);
    }
 
    Serial.print("Soil moisture value: ");
    Serial.println(soilMoistureValue);
}

void time(long val){  
  int days = elapsedDays(val);
  int hours = numberOfHours(val);
  int minutes = numberOfMinutes(val);
  displayTimeSinceWatering(days, hours, minutes);

  // digital clock display of current time
  Serial.print(days,DEC);  
  Serial.print(minutes);
  Serial.print(hours);
  Serial.println();  
}

void loop() {
  digitalWrite(ledPin, state);
  if (state) {
    display.ssd1306_command(SSD1306_DISPLAYON);
    measureMoisture();
    displayMoisture();
    time((millis() - wateringTimestamp) / 1000);
    display.display();
    
    delay(5000);
    int button_reading = digitalRead(interruptPin);
    Serial.print("Button value: ");
    Serial.println((button_reading == HIGH ? "high" : "low"));
    state = (button_reading == HIGH ? HIGH : LOW);
  } else {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    if (millis() - soilMeasurementTimestamp > soilMeasurementInterval) {
      measureMoisture();
      soilMeasurementTimestamp = millis();
    }
  }
}
