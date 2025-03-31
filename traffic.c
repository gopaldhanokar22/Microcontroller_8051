#include <reg52.h>  // Header file for 89C52

// Define LED connections
sbit RedLED = P1^0;
sbit YellowLED = P1^1;
sbit GreenLED = P1^2;

// Function to create delay in seconds
void delay_seconds(unsigned int sec) {
    unsigned int i, j;
    for(i = 0; i < sec; i++) {
        for(j = 0; j < 1000; j++) {
            // Approximate 1ms delay (adjust based on your crystal frequency)
            TMOD = 0x01;        // Timer0 mode 1
            TH0 = 0xFC;         // Initial values for 1ms delay with 11.0592MHz
            TL0 = 0x66;
            TR0 = 1;            // Start timer
            while(TF0 == 0);    // Wait for overflow
            TR0 = 0;            // Stop timer
            TF0 = 0;            // Clear overflow flag
        }
    }
}

void main() {
    while(1) {  // Infinite loop
        // Red LED ON for 5 seconds
        RedLED = 1;
        YellowLED = 0;
        GreenLED = 0;
        delay_seconds(5);
        
        // Yellow LED ON for 2 seconds
        RedLED = 0;
        YellowLED = 1;
        GreenLED = 0;
        delay_seconds(2);
        
        // Green LED ON for 5 seconds
        RedLED = 0;
        YellowLED = 0;
        GreenLED = 1;
        delay_seconds(5);
    }
}