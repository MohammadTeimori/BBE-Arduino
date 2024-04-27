#include <Arduino.h>

void setup()
{
  Serial.begin(9600);           // init UART
  pinMode(8, OUTPUT);           // set external LED pin as output
  pinMode(LED_BUILTIN, OUTPUT); // set internal LED pin as output
}

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
      digitalWrite(8, LOW);           // off external LED
      digitalWrite(LED_BUILTIN, LOW); // off internal LED
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

      // calc delay for external LED based on ADC value
      float frequency = map(adcValue, 0, 1023, 1, 10); // Map ADC value to freq range
      long toggleDelay = 1000 / (frequency * 2);       // calc toggle delay

      static unsigned long lastToggleMillis = 0;
      // Update toggleDelay regardless of whetther LED is toggled
      if (currentMillis - lastToggleMillis >= toggleDelay)
      {
        lastToggleMillis = currentMillis;
        digitalWrite(8, !digitalRead(8)); // Toggle external LED
      }
    }
  }
}
