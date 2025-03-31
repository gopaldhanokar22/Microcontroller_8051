#include <reg52.h>
#include <stdio.h>
#include <intrins.h>

// Fire Sensor and Buzzer Connections
sbit FIRE_SENSOR = P1^0;  // Digital input from fire sensor
sbit BUZZER = P1^1;       // Buzzer output

// LCD Interface (4-bit mode)
sbit RS = P2^5;
sbit EN = P2^6;
#define LCD_DATA P3

char lcd_buffer[16];

// Function Prototypes
void LCD_Init_4bit(void);
void LCD_Command_4bit(unsigned char);
void LCD_Write_4bit(unsigned char, bit);
void LCD_String_4bit(char*);
void LCD_Clear(void);
void delay_ms(unsigned int);
void System_Init(void);
void Check_Fire_Sensor(void);
void Sound_Alarm(bit state);

void main() {
    System_Init();
    
    while(1) {
        Check_Fire_Sensor();
        delay_ms(500); // Check every 500ms
    }
}

/*--- System Initialization ---*/
void System_Init(void) {
    // Initialize I/O pins
    FIRE_SENSOR = 1; // Input mode
    BUZZER = 0;      // Buzzer off
    
    // Initialize LCD
    LCD_Init_4bit();
    LCD_Clear();
    LCD_String_4bit("Fire Detection");
    delay_ms(2000);
    LCD_Clear();
}

/*--- Fire Detection Logic ---*/
void Check_Fire_Sensor(void) {
    if(FIRE_SENSOR == 0) { // Active-low sensor (0 = fire detected)
        LCD_Command_4bit(0x80); // First line
        LCD_String_4bit("Fire Detected!");
        Sound_Alarm(1); // Turn on buzzer
    }
    else {
        LCD_Command_4bit(0x80); // First line
        LCD_String_4bit("No Fire      ");
        Sound_Alarm(0); // Turn off buzzer
    }
}

/*--- Alarm Control ---*/
void Sound_Alarm(bit state) {
    BUZZER = state;
}

/*--- LCD Functions (4-bit mode) ---*/
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
    EN = 1; _nop_(); _nop_(); EN = 0;
    LCD_DATA = (LCD_DATA & 0x0F) | ((dat << 4) & 0xF0);
    EN = 1; _nop_(); _nop_(); EN = 0;
    delay_ms(1);
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

/*--- Delay Function ---*/
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<1275; j++); // For 12MHz
}
