#include <reg52.h>  // Header file for 89C52
#include <stdio.h>  // For sprintf function

// LCD control pins
sbit RS = P3^0;    // Register Select
sbit RW = P3^1;    // Read/Write
sbit EN = P3^2;    // Enable
sbit CLEAR_BTN = P3^3; // Clear screen button

// LCD data port
#define LCD_PORT P2

// Function prototypes
void LCD_Init();
void LCD_Command(unsigned char);
void LCD_Write(unsigned char);
void LCD_String(char*);
void LCD_Clear();
void delay_ms(unsigned int);
bit isButtonPressed();

unsigned int counter = 0;  // Global counter variable

void main() {
    char buffer[16];  // Buffer for counter display
    
    // Initialize LCD
    LCD_Init();
    
    // Display static message
    LCD_Command(0x80);  // Move cursor to first line
    LCD_String("Hello World!");
    
    while(1) {
        // Check clear button
        if(isButtonPressed()) {
            counter = 0;
            LCD_Clear();
            LCD_Command(0x80);  // Move cursor to first line
            LCD_String("Hello World!");
        }
        
        // Update counter display
        LCD_Command(0xC0);  // Move cursor to second line
        sprintf(buffer, "Count: %04d", counter);
        LCD_String(buffer);
        
        // Increment counter after 1 second
        delay_ms(1000);
        counter++;
    }
}

// Initialize LCD
void LCD_Init() {
    delay_ms(20);      // LCD power-up delay
    LCD_Command(0x38); // 8-bit mode, 2 lines, 5x7 dots
    LCD_Command(0x0C); // Display ON, cursor OFF
    LCD_Command(0x06); // Auto increment cursor
    LCD_Command(0x01); // Clear display
    delay_ms(2);       // Clear display delay
}

// Send command to LCD
void LCD_Command(unsigned char cmd) {
    LCD_PORT = cmd;
    RS = 0;  // Command mode
    RW = 0;  // Write mode
    EN = 1;  // High pulse
    delay_ms(1);
    EN = 0;
    delay_ms(1);
}

// Write data to LCD
void LCD_Write(unsigned char dat) {
    LCD_PORT = dat;
    RS = 1;  // Data mode
    RW = 0;  // Write mode
    EN = 1;  // High pulse
    delay_ms(1);
    EN = 0;
    delay_ms(1);
}

// Display string on LCD
void LCD_String(char *str) {
    while(*str) {
        LCD_Write(*str++);
    }
}

// Clear LCD screen
void LCD_Clear() {
    LCD_Command(0x01);  // Clear display command
    delay_ms(2);        // Clear display delay
}

// Button press detection with debounce
bit isButtonPressed() {
    if(CLEAR_BTN == 0) {  // Button pressed (active low)
        delay_ms(20);     // Debounce delay
        if(CLEAR_BTN == 0) {
            while(CLEAR_BTN == 0); // Wait for release
            return 1;
        }
    }
    return 0;
}

// Simple delay function in milliseconds
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++); // Adjust based on clock frequency
}