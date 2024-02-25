#include <Arduino.h>

// Constants for LED pins
const int ledPins[] = {10, 11, 12, LED_BUILTIN};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

// Constants for button pins
const int buttonPinForward = 7;
const int buttonPinBackward = 6;

// Constants for timing
const unsigned long debounceTime = 50;
const unsigned long longPressTime = 500;
const unsigned long autoModeInterval = 200;

// Variables to track LED and button states
int currentLedIndex = 0;
bool inAutoMode = false;
unsigned long lastButtonPressTime = 0;
unsigned long lastAutoStepTime = 0;
bool autoModeDirectionForward = true;

// Setup function
void setup()
{
    // Set LED pins to output
    for (int i = 0; i < numLeds; i++)
    {
        pinMode(ledPins[i], OUTPUT);
    }

    // Set button pins to input with internal pull-up
    pinMode(buttonPinForward, INPUT_PULLUP);
    pinMode(buttonPinBackward, INPUT_PULLUP);
}

// Function to update the LED states
void updateLeds()
{
    for (int i = 0; i < numLeds; i++)
    {
        digitalWrite(ledPins[i], (i == currentLedIndex) ? HIGH : LOW);
    }
}

// Loop function
void loop()
{
    // Handle button presses with debouncing
    static bool lastButtonForwardState = HIGH;
    static bool lastButtonBackwardState = HIGH;
    bool buttonForwardState = digitalRead(buttonPinForward);
    bool buttonBackwardState = digitalRead(buttonPinBackward);
    unsigned long currentTime = millis();

    // Check for forward button press
    if (buttonForwardState == LOW && lastButtonForwardState == HIGH && currentTime - lastButtonPressTime > debounceTime)
    {
        if (!inAutoMode)
        {
            currentLedIndex = (currentLedIndex + 1) % numLeds;
            updateLeds();
        }
        lastButtonPressTime = currentTime;
    }

    // Check for backward button press
    if (buttonBackwardState == LOW && lastButtonBackwardState == HIGH && currentTime - lastButtonPressTime > debounceTime)
    {
        if (!inAutoMode)
        {
            currentLedIndex = (currentLedIndex - 1 + numLeds) % numLeds;
            updateLeds();
        }
        lastButtonPressTime = currentTime;
    }

    // Enter automatic mode after a long press
    if ((buttonForwardState == LOW && currentTime - lastButtonPressTime > longPressTime) ||
        (buttonBackwardState == LOW && currentTime - lastButtonPressTime > longPressTime))
    {
        inAutoMode = true;
        autoModeDirectionForward = buttonForwardState == LOW;
        lastAutoStepTime = currentTime;
    }

    // Exit automatic mode on button press
    if ((buttonForwardState == LOW || buttonBackwardState == LOW) && inAutoMode && currentTime - lastButtonPressTime > debounceTime)
    {
        inAutoMode = false;
    }

    // Handle automatic mode LED switching
    if (inAutoMode && currentTime - lastAutoStepTime >= autoModeInterval)
    {
        currentLedIndex = autoModeDirectionForward ? (currentLedIndex + 1) % numLeds : (currentLedIndex - 1 + numLeds) % numLeds;
        updateLeds();
        lastAutoStepTime = currentTime;
    }

    // Update the last button states
    lastButtonForwardState = buttonForwardState;
    lastButtonBackwardState = buttonBackwardState;

    // Small delay to prevent bouncing
    delay(1);
}
