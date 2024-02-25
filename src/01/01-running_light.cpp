#include <Arduino.h>

// LED pins
const int ledPins[] = {10, 11, 12, LED_BUILTIN};
const int numLeds = 4;

// Button pins
const int buttonForwardPin = 7;
const int buttonBackwardPin = 6;

// Button state tracking
bool lastButtonForwardState = HIGH;
bool lastButtonBackwardState = HIGH;

// Timing for debouncing and long press detection
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long longPressTime = 500;
bool inAutomaticMode = false;
int automaticDirection = 1; // 1 for forward, -1 for backward

// LED control
int currentLed = 0; // Start with the first LED

void setup()
{
    // Initialize LEDs
    for (int i = 0; i < numLeds; i++)
    {
        pinMode(ledPins[i], OUTPUT);
    }

    // Initialize buttons with internal pull-up resistors
    pinMode(buttonForwardPin, INPUT_PULLUP);
    pinMode(buttonBackwardPin, INPUT_PULLUP);

    // Turn off all LEDs initially
    for (int i = 0; i < numLeds; i++)
    {
        digitalWrite(ledPins[i], LOW);
    }
}

void loop()
{
    // Read the state of the buttons
    bool buttonForwardState = digitalRead(buttonForwardPin);
    bool buttonBackwardState = digitalRead(buttonBackwardPin);

    // Check for button state change for debouncing
    if (buttonForwardState != lastButtonForwardState)
    {
        lastDebounceTime = millis();
    }

    if (buttonBackwardState != lastButtonBackwardState)
    {
        lastDebounceTime = millis();
    }

    // After the debounce delay, process the button state
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        // Detect long press for automatic mode
        if (buttonForwardState == LOW && (millis() - lastDebounceTime) > longPressTime)
        {
            inAutomaticMode = true;
            automaticDirection = 1;
        }
        else if (buttonBackwardState == LOW && (millis() - lastDebounceTime) > longPressTime)
        {
            inAutomaticMode = true;
            automaticDirection = -1;
        }
        else if (buttonForwardState == LOW)
        {
            // Short press, manual mode
            changeLed(1);
            inAutomaticMode = false;
        }
        else if (buttonBackwardState == LOW)
        {
            // Short press, manual mode
            changeLed(-1);
            inAutomaticMode = false;
        }
    }

    // Automatic mode
    if (inAutomaticMode)
    {
        static unsigned long lastAutoAdvanceTime = 0;
        if (millis() - lastAutoAdvanceTime > 200)
        { // Advance every 200 ms
            changeLed(automaticDirection);
            lastAutoAdvanceTime = millis();
        }
    }

    // Remember the last button state
    lastButtonForwardState = buttonForwardState;
    lastButtonBackwardState = buttonBackwardState;

    delay(1); // Small delay to avoid missing button presses
}

void changeLed(int direction)
{
    // Turn off the current LED
    digitalWrite(ledPins[currentLed], LOW);

    // Calculate the next LED index
    currentLed += direction;
    if (currentLed >= numLeds)
        currentLed = 0;
    else if (currentLed < 0)
        currentLed = numLeds - 1;

    // Turn on the new LED
    digitalWrite(ledPins[currentLed], HIGH);
}
