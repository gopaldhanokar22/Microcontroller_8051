#include <reg52.h>
#include <stdio.h>
#include <intrins.h>

/*--- Hardware Connections ---*/
// ADC0831 Interface (Software SPI)
sbit ADC_CS  = P1^0;   // Chip Select (Active Low)
sbit ADC_CLK = P1^1;   // Clock
sbit ADC_DO  = P1^2;   // Data Out

// LCD Interface (4-bit mode to save pins)
sbit RS = P2^5;        // Register Select
sbit EN = P2^6;        // Enable
#define LCD_DATA P3    // Using P3.4-P3.7 (D4-D7)

/*--- Global Variables ---*/
float temperature = 0.0;
char lcd_buffer[16];

/*--- Function Prototypes ---*/
void System_Init(void);
float Read_Temperature(void);
unsigned char ADC_Read(void);
void Update_Display(void);
void LCD_Init_4bit(void);
void LCD_Write_4bit(unsigned char dat, bit mode);
void LCD_Command_4bit(unsigned char cmd);
void LCD_String_4bit(char *str);
void LCD_Clear(void);
void delay_ms(unsigned int ms);
void delay_us(unsigned int us);

/*--- Main Program ---*/
void main() {
    System_Init();
    
    while(1) {
        temperature = Read_Temperature();
        Update_Display();
        delay_ms(500); // Update every 0.5 seconds
    }
}

/*--- System Initialization ---*/
void System_Init(void) {
    // Initialize ADC pins
    ADC_CS = 1;
    ADC_CLK = 0;
    
    // Initialize LCD
    LCD_Init_4bit();
    LCD_Clear();
    LCD_String_4bit("Temperature:");
}

/*--- Temperature Measurement ---*/
float Read_Temperature(void) {
    unsigned char adc_value = ADC_Read();
    // LM35: 10mV/°C, ADC0831: 5V range, 8-bit (256 steps)
    return (adc_value * 500.0) / 256.0; // (adc_value * 5.0 * 100) / 256
}

/*--- ADC Read Function ---*/
unsigned char ADC_Read(void) {
    unsigned char i, value = 0;
    
    ADC_CS = 0;         // Start conversion
    _nop_(); _nop_();   // Small delay (tCSS)
    
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

/*--- Display Update ---*/
void Update_Display(void) {
    LCD_Command_4bit(0xC0); // Move to second line
    sprintf(lcd_buffer, "%.1f C  ", temperature);
    LCD_String_4bit(lcd_buffer);
}

/*--- LCD Functions (4-bit mode) ---*/
void LCD_Init_4bit(void) {
    delay_ms(50);       // Power-on delay
    
    // Initialization sequence for 4-bit mode
    LCD_Command_4bit(0x33);
    LCD_Command_4bit(0x32);
    LCD_Command_4bit(0x28); // 4-bit, 2-line, 5x8 font
    LCD_Command_4bit(0x0C); // Display ON, cursor OFF
    LCD_Command_4bit(0x06); // Auto increment cursor
    LCD_Clear();
}

void LCD_Write_4bit(unsigned char dat, bit mode) {
    RS = mode;
    
    // High nibble first
    LCD_DATA = (LCD_DATA & 0x0F) | (dat & 0xF0);
    EN = 1; delay_us(1); EN = 0;
    
    // Low nibble
    LCD_DATA = (LCD_DATA & 0x0F) | ((dat << 4) & 0xF0);
    EN = 1; delay_us(1); EN = 0;
    
    delay_us(100); // Most commands need 40µs
}

void LCD_Command_4bit(unsigned char cmd) {
    LCD_Write_4bit(cmd, 0);
}

void LCD_String_4bit(char *str) {
    while(*str) {
        LCD_Write_4bit(*str++, 1);
    }
}

void LCD_Clear(void) {
    LCD_Command_4bit(0x01);
    delay_ms(2); // Clear display needs 1.52ms
}

/*--- Delay Functions ---*/
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++); // For 12MHz
}

void delay_us(unsigned int us) {
    while(us--) {
        _nop_(); _nop_(); _nop_(); _nop_();
    }
}
