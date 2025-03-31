#include <reg52.h>  // Header file for 89C52

// Motor Driver Pins (L293D or L298N)
sbit MOTOR_IN1 = P1^0;  // Motor Input 1 (Clockwise)
sbit MOTOR_IN2 = P1^1;  // Motor Input 2 (Anti-clockwise)
sbit MOTOR_ENA = P1^2;  // Enable Pin for Motor Driver

// Switch Pins
sbit SWITCH1 = P3^0;    // Start Clockwise
sbit SWITCH2 = P3^1;    // Start Anti-clockwise
sbit SWITCH3 = P3^2;    // Stop Motor

// Function prototypes
void delay_ms(unsigned int);
void Motor_Stop();
void Motor_CW();
void Motor_CCW();

void main() {
    // Initialize motor control pins
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;
    MOTOR_ENA = 1;  // Enable motor driver
    
    // Initialize switch pins as input
    SWITCH1 = 1;
    SWITCH2 = 1;
    SWITCH3 = 1;
    
    while(1) {
        if(SWITCH1 == 0) {       // Clockwise Rotation
            Motor_CW();
            while(SWITCH1 == 0); // Wait for switch release
        }
        else if(SWITCH2 == 0) {  // Anti-clockwise Rotation
            Motor_CCW();
            while(SWITCH2 == 0); // Wait for switch release
        }
        else if(SWITCH3 == 0) {  // Stop Motor
            Motor_Stop();
            while(SWITCH3 == 0); // Wait for switch release
        }
    }
}

void Motor_CW() {
    MOTOR_IN1 = 1;
    MOTOR_IN2 = 0;
}

void Motor_CCW() {
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 1;
}

void Motor_Stop() {
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;
}

// Simple delay function in milliseconds
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++); // Adjust based on clock frequency
}