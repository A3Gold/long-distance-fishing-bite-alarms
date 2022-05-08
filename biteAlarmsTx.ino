// Purpose: Long Range Fishing Bite Alarms Transmitter (R1)
// Reference: https://bit.ly/3rg4lQ5
// Author: A. Goldman
// Date: March 26, 2021
// Status: Working

#define COMPOUT 0     // Defines comparator output pin
#define TX (1 << PB0) // Defines Tx data pin
// (Change to 12 for rod 2)...
#define PULSES 6 // Defines number of data pulses

void setup()
{
    pinMode(COMPOUT, INPUT); // Sets comparator pin as input
    DDRB &= ~(TX);           // Sets Tx pin as input
    PORTB &= ~(TX);          // Sets Tx pin low
}

void loop()
{
    // If comp. output low (~2V; aRead needed) (fish has bit)...
    if (analogRead(COMPOUT) < 400)
    {
        for (uint8_t i = 0; i < PULSES; i++)
        {                   // Runs once each pulse
            PORTB &= ~(TX); // Set Tx pin low
            delay(90);      // Wait pulse width
            PORTB |= TX;    // Set Tx pin high
            delay(90);      // Wait pulse width
        }
        PORTB &= ~(TX); // Transmission over... set Tx low
        while (1); // Wait forever
    }
    PORTB &= ~(TX); // Sets Tx pin low
}
