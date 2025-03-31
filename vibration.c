#include <reg51.h>

sbit LED = P1^3;       // Define LED at P1.3
sbit Sensor = P1^2;   // Define Sensor at P1.2

void delay(unsigned int time) {
    unsigned int i, j;
    for(i = 0; i < time; i++)
        for(j = 0; j < 1275; j++);
}

void main() {
    P1 = 0xFF; // Set Port 1 as input for sensor
    LED = 0;   // Turn off LED initially
    
    while(1) {
        if (Sensor == 1) {
            LED = 1; // Turn on LED
        } else {
            LED = 0; // Turn off LED
        }
        delay(10);
    }
}