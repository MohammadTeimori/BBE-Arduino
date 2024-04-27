#include <Arduino.h>

// Define LED pins
const int ledPins[] = {10, 11, 12, LED_BUILTIN};
const int numOfLeds = 4;

// Define button pins
const int buttonPin1 = 7;
const int buttonPin2 = 6;

// Variables to keep track of the current LED
int currentLed = 0;

// Timing variables
unsigned long lastButtonPress = 0;
const long debounceTime = 50;    // 50 milliseconds debounce period
const long longPressTime = 500;  // 500 milliseconds for a long press
const long autoModePeriod = 200; // 200 milliseconds for automatic mode

// State variables
bool autoMode = false;
unsigned long lastAutoChange = 0;
int autoDirection = 1; // 1 for forward, -1 for backward

// Function declarations
void updateLed();
void checkButtons();
void enterAutoMode(int direction);

void setup()
{
    // Initialize LEDs
    for (int i = 0; i < numOfLeds; i++)
    {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
    }

    // Initialize buttons with internal pull-up
    pinMode(buttonPin1, INPUT_PULLUP);
    pinMode(buttonPin2, INPUT_PULLUP);

    // Set the first LED on
    digitalWrite(ledPins[currentLed], HIGH);
}

void loop()
{
    if (autoMode)
    {
        // In automatic mode, change LEDs based on autoModePeriod
        if (millis() - lastAutoChange >= autoModePeriod)
        {
            currentLed = (currentLed + autoDirection + numOfLeds) % numOfLeds;
            updateLed();
            lastAutoChange = millis();
        }
    }
    checkButtons(); // Always check the button states
    delay(1);       // Small delay to prevent bouncing
}

void updateLed()
{
    // Turn off all LEDs
    for (int i = 0; i < numOfLeds; i++)
    {
        digitalWrite(ledPins[i], LOW);
    }
    // Turn on the current LED
    digitalWrite(ledPins[currentLed], HIGH);
}

void checkButtons()
{
    // Read the state of the buttons
    int buttonState1 = digitalRead(buttonPin1);
    int buttonState2 = digitalRead(buttonPin2);
    static int lastButtonState1 = HIGH;
    static int lastButtonState2 = HIGH;
    unsigned long currentMillis = millis();

    // Check if button 1 was pressed
    if (buttonState1 == LOW && lastButtonState1 == HIGH)
    {
        lastButtonPress = currentMillis;
    }
    if (buttonState1 == HIGH && lastButtonState1 == LOW)
    {
        if (currentMillis - lastButtonPress >= longPressTime)
        {
            enterAutoMode(1); // Enter automatic mode forward
        }
        else
        {
            autoMode = false;
            currentLed = (currentLed + 1) % numOfLeds;
            updateLed();
        }
    }
    lastButtonState1 = buttonState1;

    // Check if button 2 was pressed
    if (buttonState2 == LOW && lastButtonState2 == HIGH)
    {
        lastButtonPress = currentMillis;
    }
    if (buttonState2 == HIGH && lastButtonState2 == LOW)
    {
        if (currentMillis - lastButtonPress >= longPressTime)
        {
            enterAutoMode(-1); // Enter automatic mode backward
        }
        else
        {
            autoMode = false;
            currentLed = (currentLed - 1 + numOfLeds) % numOfLeds;
            updateLed();
        }
    }
    lastButtonState2 = buttonState2;
}

void enterAutoMode(int direction)
{
    autoMode = true;
    autoDirection = direction;
    lastAutoChange = millis();
    // In automatic mode, immediately update to the next LED
    currentLed = (currentLed + autoDirection + numOfLeds) % numOfLeds;
    updateLed();
}
