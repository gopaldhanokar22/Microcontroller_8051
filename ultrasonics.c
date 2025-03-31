#include <reg52.h>
#include <stdio.h>
#include <intrins.h>

// Ultrasonic Sensor Connections
sbit TRIG = P1^0;  // Trigger pin
sbit ECHO = P1^1;  // Echo pin
sbit BUZZER = P1^2; // Buzzer pin

// LCD Interface (4-bit mode)
sbit RS = P2^5;
sbit EN = P2^6;
#define LCD_DATA P3

// Variables
char lcd_buffer[16];
unsigned int distance_cm;
unsigned long echo_time;

// Function Prototypes
void LCD_Init_4bit(void);
void LCD_Command_4bit(unsigned char);
void LCD_Write_4bit(unsigned char, bit);
void LCD_String_4bit(char*);
void LCD_Clear(void);
void delay_ms(unsigned int);
void delay_us(unsigned int);
unsigned int Measure_Distance(void);
void Update_Display(void);
void Trigger_Pulse(void);

void main() {
    // Initialize peripherals
    TRIG = 0;
    ECHO = 1;  // Input mode
    BUZZER = 0;
    
    LCD_Init_4bit();
    LCD_Clear();
    LCD_String_4bit("HC-SR04 Sensor");
    delay_ms(1000);
    LCD_Clear();
    
    while(1) {
        distance_cm = Measure_Distance();
        Update_Display();
        
        // Activate buzzer if distance < 10cm
        if(distance_cm < 10) {
            BUZZER = 1;
        } else {
            BUZZER = 0;
        }
        
        delay_ms(200); // Update every 200ms
    }
}

/* Generate 10µs trigger pulse */
void Trigger_Pulse(void) {
    TRIG = 1;
    delay_us(10);
    TRIG = 0;
}

/* Measure distance in cm */
unsigned int Measure_Distance(void) {
    unsigned int time_us = 0;
    
    // Send trigger pulse
    Trigger_Pulse();
    
    // Wait for echo start
    while(!ECHO);
    
    // Measure echo pulse width
    while(ECHO) {
        time_us++;
        delay_us(1);
        if(time_us > 38000) break; // Timeout (~6.5m)
    }
    
    // Distance = (time * speed_of_sound)/2
    // speed_of_sound = 340 m/s = 0.034 cm/µs
    return (unsigned int)(time_us * 0.034 / 2);
}

/* Update LCD Display */
void Update_Display(void) {
    LCD_Command_4bit(0x80); // First line
    sprintf(lcd_buffer, "Dist: %3d cm", distance_cm);
    LCD_String_4bit(lcd_buffer);
    
    LCD_Command_4bit(0xC0); // Second line
    if(distance_cm < 10) {
        LCD_String_4bit("ALERT! <10cm  ");
    } else {
        LCD_String_4bit("              ");
    }
}

/* LCD Functions (4-bit mode) */
void LCD_Init_4bit(void) {
    delay_ms(50);
    LCD_Command_4bit(0x33);
    LCD_Command_4bit(0x32);
    LCD_Command_4bit(0x28); // 4-bit, 2-line, 5x8
    LCD_Command_4bit(0x0C); // Display ON, cursor OFF
    LCD_Command_4bit(0x06); // Auto increment
    LCD_Command_4bit(0x01); // Clear display
    delay_ms(2);
}

void LCD_Write_4bit(unsigned char dat, bit mode) {
    RS = mode;
    LCD_DATA = (LCD_DATA & 0x0F) | (dat & 0xF0);
    EN = 1; delay_us(1); EN = 0;
    LCD_DATA = (LCD_DATA & 0x0F) | ((dat << 4) & 0xF0);
    EN = 1; delay_us(1); EN = 0;
    delay_us(100);
}

void LCD_Command_4bit(unsigned char cmd) {
    LCD_Write_4bit(cmd, 0);
}

void LCD_String_4bit(char *str) {
    while(*str) LCD_Write_4bit(*str++, 1);
}

void LCD_Clear(void) {
    LCD_Command_4bit(0x01);
    delay_ms(2);
}

/* Delay Functions */
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<1275; j++); // For 12MHz
}

void delay_us(unsigned int us) {
    while(us--) {
        _nop_(); _nop_(); _nop_(); _nop_();
    }
}