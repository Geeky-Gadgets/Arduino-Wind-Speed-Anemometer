/*
  Arduino Wind Speed Meter Anemometer mph - Adafruit anemometer (product ID 1733).
  Modified code created March 2016 from original code created by Joe Burg 11th November 2014 at http://www.hackerscapes.com/ with help from Adafruit forum users shirad
*/

//Initialise LCD display

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int serial_in;

//Setup Variables

double x = 0;
double y = 0;
double a = 0;
double b = 0;
const int sensorPin = A1; //Defines the pin that the anemometer output is connected 
const int numReadings = 5; //Defines number of reading to calculate average windspeed
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int totalWind= 0;                  // the running total
int averageWind = 0;                // the average
int inputPin = A1;
int sensorValue = 0; //Variable stores the value direct from the analog pin
float sensorVoltage = 0; //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
float sensorVoltage2 = 0; //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
float windSpeed = 0; // Wind speed in meters per second (m/s)

float voltageConversionConstant = .004882814; //This constant maps the value provided from the analog read function, which ranges from 0 to 1023, to actual voltage, which ranges from 0V to 5V
int sensorDelay = 100; //Delay between sensor readings, measured in milliseconds (ms)

//Anemometer Technical Variables
//The following variables correspond to the anemometer sold by Adafruit, but could be modified to fit other anemometers.

float voltageMin = 0.55; // Mininum output voltage from anemometer in mV.
float windSpeedMin = 2; // Wind speed in meters/sec corresponding to minimum voltage

float voltageMax = 2.0; // Maximum output voltage from anemometer in mV.
float windSpeedMax = 32; // Wind speed in meters/sec corresponding to maximum voltage

void setup()

{
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  //Setup LCD display with welcome screen

  lcd.begin(16, 2);
  lcd.print("Seren Valentine");
  lcd.setCursor(0, 1);
  lcd.print("Windspeed Sensor");
  delay(2500);
  lcd.clear();
  lcd.setCursor(0, 0);
  Serial.begin(9600);  //Start the serial connection

}

//Anemometer calculations

void loop()
{
  
  sensorValue = analogRead(sensorPin); //Get a value between 0 and 1023 from the analog pin connected to the anemometer

   // subtract the last reading:
  totalWind = totalWind - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = sensorValue;
  // add the reading to the total:
  totalWind = totalWind + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
 sensorVoltage2 = sensorValue * voltageConversionConstant; //Convert sensor value to actual voltage
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;

    // calculate the average:
  averageWind = totalWind / numReadings;

  sensorVoltage = averageWind * voltageConversionConstant; //Convert sensor value to actual voltage
 
  //Convert voltage value to wind speed using range of max and min voltages and wind speed for the anemometer
  if (sensorVoltage <= voltageMin) {
    windSpeed = 0; //Check if voltage is below minimum value. If so, set wind speed to zero.
  } else {
    windSpeed = ((sensorVoltage - voltageMin) * windSpeedMax / (voltageMax - voltageMin))*2.232694; //For voltages above minimum value, use the linear relationship to calculate wind speed.
  }
  }
  
  //Max wind speed calculation

  x = windSpeed;
  if (x >= y) {
    y = x;
  } else {
    y = y;
  }

//Max voltage calculation

  a = sensorVoltage;
  if (a >= b) {
    b = a;
  } else {
    b = b;
  }

  //Print voltage and windspeed to serial

  Serial.print("Voltage: ");
  Serial.print(sensorVoltage);
  Serial.print("Average: ");
  Serial.print(averageWind);
  Serial.print("\t");
  Serial.print("Wind speed: ");
  Serial.println(windSpeed);

  //Display Wind Speed results to LCD with Max wind speed

  lcd.setCursor(0, 0);
  lcd.print("Wind Speed=");
  lcd.setCursor(11, 0);
  lcd.print(windSpeed);
  lcd.setCursor(0, 1);
  lcd.print("V:");
  lcd.setCursor(2, 1);
  lcd.print(b);
  lcd.setCursor(7, 1);
  lcd.print("Max=");
  lcd.setCursor(11, 1);
  lcd.print(y);

  delay(sensorDelay);
}
