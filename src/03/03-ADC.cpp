#include <Arduino.h>

void setup()
{
  Serial.begin(9600);           // Initialize UART
  pinMode(8, OUTPUT);           // Set external LED pin as output
  pinMode(LED_BUILTIN, OUTPUT); // Set internal LED pin as output
}

// Global variable to control running state
bool isRunning = false;

void loop()
{
  if (Serial.available() > 0)
  {
    char receivedChar = Serial.read();
    if (receivedChar == 'R')
    {
      isRunning = true;
    }
    else if (receivedChar == 'S')
    {
      isRunning = false;
      digitalWrite(8, LOW);           // Turn off external LED
      digitalWrite(LED_BUILTIN, LOW); // Turn off internal LED
    }
  }

  if (isRunning)
  {
    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    if (currentMillis - previousMillis >= 250)
    {
      previousMillis = currentMillis;
      int adcValue = analogRead(A0);                        // Read ADC value
      Serial.println(adcValue);                             // Send ADC value via UART
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle internal LED

      // Calculate delay for external LED based on ADC value
      float frequency = map(adcValue, 0, 1023, 1, 10); // Map ADC value to frequency range
      long toggleDelay = 1000 / (frequency * 2);       // Calculate toggle delay

      static unsigned long lastToggleMillis = 0;
      if (currentMillis - lastToggleMillis >= toggleDelay)
      {
        lastToggleMillis = currentMillis;
        digitalWrite(8, !digitalRead(8)); // Toggle external LED
      }
    }
  }
}
