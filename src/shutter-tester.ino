#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Version
const String version = "v0.3";
// Pin for IR sensor
const byte interruptPin = 2;
// Pin for reset button
const byte buttonPin = 3;
// Pin for light toggle button
const byte lightButtonPin = 4;
boolean exposed = false;
// Variables for storing beginning and end time of exposure
unsigned long start_time = 0;
unsigned long end_time = 0;
// Exposure counter
int exposure = 0;
// Store the minimum and maximum exposure of a test run
unsigned long mintime = 0;
unsigned long maxtime = 0;
// Sum of times
unsigned long sum = 0;
String mintime_s = "";
String maxtime_s = "";
// Store if LCD light should be on or off
boolean light = false;

void setup()
{
    lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
    lcd.backlight();//To Power ON the back light
    lcd.setCursor(0,0); //Defining positon to write from first row,first column .

    // Initialise serial line for printing CSVs of results.
    Serial.begin(9600);
    // Initialise pin for IR sensor input and attach an interrupt to it
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), exp, CHANGE);
    /*
       Initialise pins for buttons.
       WARNING: don't forget to define it as an input w/ pullup. If you
       don't you'll very likely fry your Arduino!
    */
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(lightButtonPin, INPUT_PULLUP);
}

void loop()
{
    lcd.print(" Tech Maker "); //You can write 16 Characters per line .
    delay(1000);//Delay used to give a dynamic effect
    lcd.setCursor(0,1);  //Defining positon to write from second row,first column .
    lcd.print("Like | Share");
    delay(8000);
    lcd.clear();//Clean the screen
    lcd.setCursor(0,0);
    lcd.print(" SUBSCRIBE ");
    lcd.setCursor(0,1);
    lcd.print(" TECH MAKER ");
    delay(8000);
}
