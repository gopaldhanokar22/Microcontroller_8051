#include <reg52.h>
#include <stdio.h>

// Servo control pin
sbit SERVO_PIN = P1^0;

// Button pins
sbit BTN_0 = P3^0;   // 0° position
sbit BTN_90 = P3^1;  // 90° position
sbit BTN_180 = P3^2; // 180° position

// LCD control pins
sbit RS = P2^0;
sbit RW = P2^1;
sbit EN = P2^2;
#define LCD_DATA P0

// Variables
unsigned char angle = 90; // Default position
char lcd_buffer[16];

// Function prototypes
void LCD_Init();
void LCD_Command(unsigned char);
void LCD_Write(unsigned char);
void LCD_String(char*);
void LCD_Clear();
void delay_us(unsigned int);
void delay_ms(unsigned int);
void Set_Servo_Angle(unsigned char);
void Update_LCD_Display();

void main() {
    // Initialize buttons as input
    BTN_0 = 1;
    BTN_90 = 1;
    BTN_180 = 1;
    
    // Initialize LCD
    LCD_Init();
    LCD_Clear();
    
    // Initialize servo to default position (90°)
    Set_Servo_Angle(90);
    Update_LCD_Display();
    
    while(1) {
        if(BTN_0 == 0) {       // Move to 0°
            delay_ms(20);       // Debounce delay
            if(BTN_0 == 0) {
                angle = 0;
                Set_Servo_Angle(angle);
                Update_LCD_Display();
                while(BTN_0 == 0); // Wait for button release
            }
        }
        else if(BTN_90 == 0) {  // Move to 90°
            delay_ms(20);
            if(BTN_90 == 0) {
                angle = 90;
                Set_Servo_Angle(angle);
                Update_LCD_Display();
                while(BTN_90 == 0);
            }
        }
        else if(BTN_180 == 0) { // Move to 180°
            delay_ms(20);
            if(BTN_180 == 0) {
                angle = 180;
                Set_Servo_Angle(angle);
                Update_LCD_Display();
                while(BTN_180 == 0);
            }
        }
    }
}

void Set_Servo_Angle(unsigned char angle) {
    unsigned int pulse_width;
    
    // Convert angle to pulse width (500-2500µs)
    pulse_width = 500 + (angle * 2000) / 180;
    
    // Generate PWM signal (20ms period)
    SERVO_PIN = 1;
    delay_us(pulse_width);
    SERVO_PIN = 0;
    delay_ms(20 - (pulse_width / 1000));
}

void Update_LCD_Display() {
    LCD_Command(0x80); // Move to first line
    LCD_String("Servo Position:");
    LCD_Command(0xC0); // Move to second line
    sprintf(lcd_buffer, "Angle: %3d", angle);
    LCD_String(lcd_buffer);
}

// LCD Functions
void LCD_Init() {
    delay_ms(20);
    LCD_Command(0x38); // 8-bit mode, 2 lines, 5x7 dots
    LCD_Command(0x0C); // Display ON, cursor OFF
    LCD_Command(0x06); // Auto increment cursor
    LCD_Command(0x01); // Clear display
    delay_ms(2);
}

void LCD_Command(unsigned char cmd) {
    LCD_DATA = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    delay_ms(1);
    EN = 0;
    delay_ms(1);
}

void LCD_Write(unsigned char dat) {
    LCD_DATA = dat;
    RS = 1;
    RW = 0;
    EN = 1;
    delay_ms(1);
    EN = 0;
    delay_ms(1);
}

void LCD_String(char *str) {
    while(*str) {
        LCD_Write(*str++);
    }
}

void LCD_Clear() {
    LCD_Command(0x01);
    delay_ms(2);
}

// Delay functions
void delay_us(unsigned int us) {
    while(us--) {
        /* Approximately 1µs delay at 12MHz clock */
        /* Adjust based on your crystal frequency */
    }
}

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++); // Adjust for 12MHz
}