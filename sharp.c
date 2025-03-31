#include <reg52.h>
#include <stdio.h>
#include <intrins.h>

// ADC Interface (using ADC0831)
sbit ADC_CS = P1^0;   // Chip Select
sbit ADC_CLK = P1^1;  // Clock
sbit ADC_DO = P1^2;   // Data Out

// LCD Interface (4-bit mode)
sbit RS = P2^5;
sbit EN = P2^6;
#define LCD_DATA P3

// Variables
char lcd_buffer[16];
unsigned int distance_cm;
float voltage;
float inv_voltage;  // Moved to global scope
float term1;        // Moved to global scope
float term2;        // Moved to global scope
float approx;       // Moved to global scope

// Function Prototypes
void LCD_Init_4bit(void);
void LCD_Command_4bit(unsigned char);
void LCD_Write_4bit(unsigned char, bit);
void LCD_String_4bit(char*);
void LCD_Clear(void);
void delay_ms(unsigned int);
void delay_us(unsigned int);
unsigned char ADC_Read(void);
unsigned int Read_Distance(void);
float Read_Voltage(void);
void Update_Display(void);

void main() {
    // Initialize peripherals
    ADC_CS = 1;
    ADC_CLK = 0;
    LCD_Init_4bit();
    LCD_Clear();
    LCD_String_4bit("Sharp Sensor");
    delay_ms(1000);
    LCD_Clear();
    
    while(1) {
        distance_cm = Read_Distance();
        voltage = Read_Voltage();
        Update_Display();
        delay_ms(200); // Update every 200ms
    }
}

/* Read distance in cm (10-80cm range) */
unsigned int Read_Distance(void) {
    unsigned char adc_value = ADC_Read();
    voltage = (adc_value * 5.0) / 256.0; // 8-bit ADC with 5V reference
    
    // Empirical conversion for GP2Y0A21YK0F
    if(voltage < 0.4) return 80;  // Minimum detectable distance
    
    // Calculate using approximation
    inv_voltage = 1.0 / voltage;
    term1 = inv_voltage;
    term2 = 0.15 * inv_voltage * inv_voltage;
    approx = term1 - term2;
    
    return (unsigned int)(27.86 * approx);
}

/* Read voltage from sensor */
float Read_Voltage(void) {
    return (ADC_Read() * 5.0) / 256.0;
}

/* ADC Read Function */
unsigned char ADC_Read(void) {
    unsigned char i, value = 0;
    
    ADC_CS = 0;         // Start conversion
    _nop_(); _nop_();   // Small delay
    
    // Read 8 bits MSB first
    for(i = 0; i < 8; i++) {
        ADC_CLK = 1;
        _nop_();
        value <<= 1;
        if(ADC_DO) value |= 0x01;
        ADC_CLK = 0;
        _nop_();
    }
    
    ADC_CS = 1; // End conversion
    return value;
}

/* Update LCD Display */
void Update_Display(void) {
    LCD_Command_4bit(0x80); // First line
    sprintf(lcd_buffer, "Dist: %3d cm", distance_cm);
    LCD_String_4bit(lcd_buffer);
    
    LCD_Command_4bit(0xC0); // Second line
    sprintf(lcd_buffer, "Volt: %.2f V", voltage);
    LCD_String_4bit(lcd_buffer);
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