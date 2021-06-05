// Display stuff

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

const byte ledPin = LED_BUILTIN;
const byte interruptPin = 2;
volatile byte state = LOW;

// Soil moisture stuff
const int moisturePin = A2;
const int AirValue = 950;   //you need to replace this value with Value_1
const int WaterValue = 600;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent = 0;

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
  display.print(soilmoisturepercent);
  display.println("%");
  
  display.print("Sensor value ");
  display.println(soilMoistureValue);

  display.display();
}

void loop() {
  digitalWrite(ledPin, state);
  if (state) {
    soilMoistureValue = analogRead(moisturePin);  //put Sensor insert into soil
    soilmoisturepercent = constrain(map(soilMoistureValue, AirValue, WaterValue, 0, 100), 0 , 100);
    Serial.print("Soil moisture value: ");
    Serial.print(soilMoistureValue);
    display.ssd1306_command(SSD1306_DISPLAYON);
    displayMoisture();
    
    delay(5000);
  } else {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }
}
