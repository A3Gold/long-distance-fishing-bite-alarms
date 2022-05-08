// Purpose: Long Range Fishing Bite Alarms Receiver
// Reference: https://bit.ly/3rg4lQ5
// Author: A. Goldman
// Date: March 26, 2021
// Status: Working

#define RX (1 << PA0)            // Defines Rx data pin
#define LED1 (1 << PB1)          // Defines rod 1 LED pin
#define LED2 (1 << PB0)          // Defines rod 2 LED pin
#define BUZZER (1 << PB2)        // Defines buzzer pin
unsigned long txDuration = 0;    // Measubres transmission duration
uint8_t rxState = HIGH;          // Holds initial state of Rx pin
uint8_t lastRxState = LOW;       // Holds previous state of Rx pin
unsigned long lastNoiseTime = 0; // Holds last noise filter time
uint8_t noiseDelay = 70;         // Holds minimum data width to filter noise
uint8_t ifCount = 0;             // Used so if() statement only runs once
volatile uint8_t count = 0;      // Counts pulses from transmitters

void setup()
{
    DDRA &= ~(RX);                // Sets Rx pin as input
    PORTA |= RX;                  // Activates internal pull up resistor
    DDRB |= LED1 | LED2 | BUZZER; // Sets output pins as outputs
}

void loop()
{
    uint8_t reading = PINA & RX; // Current state of Rx pin as 1 or 0
    if (reading != lastRxState)
    {                             // If state has changed...
        lastNoiseTime = millis(); // capture start time
    }
    // If noise filter time has passed...
    if ((millis() - lastNoiseTime) > noiseDelay)
    {
        if (reading != rxState)
        {                      // If state has changed...
            rxState = reading; // True Rx state is the current reading
            if (rxState == HIGH)
            {            // If that state is high...
                count++; // Increase count
                if (ifCount == 0)
                {                          // Only runs for first data pulse
                    txDuration = millis(); // Capture start time
                }
                ifCount++; // Increase so above statement only runs once
            }
        }
    }
    lastRxState = reading; // Record last Rx state
    // If minimum time it takes for all data to be sent is passed...
    if (millis() - txDuration > 2600)
    {
        if (count > 4 && count < 10)
        {                           // and rod 1 pulses sent...
            PORTB |= LED1 | BUZZER; // set rod 1 LED and buzzer high
            while (1)
                ; // Wait until user resets system
        }
        if (10 < count)
        { // and rod 2 pulses sent...
            while (1)
            {                           // Repeat forever...
                PORTB |= LED2 | BUZZER; // set rod 2 LED and buzzer high
                delay(500);             // wait half a second
                PORTB &= ~(BUZZER);     // turn buzzer off (beeping)
                delay(500);             // wait half a second
            }
        }
    }
    if (count == 1)
    { // If one lone pulse sent...
        if (millis() - txDuration > 1000)
        {                // and none follows...
            count = 0;   // it is noise; set count back to 0
            ifCount = 0; // make sure time capture runs again
        }
    }
}
