#include <reg52.h>  // Header file for 89C52

// LED connection - change this to your actual pin
sbit LED = P1^0;    // LED connected to P1.0

// Function prototype
void delay_ms(unsigned int ms);

void main() {
    while(1) {  // Infinite loop
        LED = 1;         // Turn LED ON
        delay_ms(500);   // 500ms delay
        
        LED = 0;         // Turn LED OFF
        delay_ms(500);   // 500ms delay
    }
}

// Delay function in milliseconds
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++); // Adjust based on clock frequency
}
