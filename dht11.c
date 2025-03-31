#include <reg52.h>
#include <stdio.h>
#include <intrins.h>

// DHT11 Connection
sbit DHT11_PIN = P1^0;

// LCD Interface (4-bit mode)
sbit RS = P2^5;
sbit EN = P2^6;
#define LCD_DATA P3

// Global Variables
unsigned char humidity_int, humidity_dec;
unsigned char temp_int, temp_dec;
unsigned char checksum;
char lcd_buffer[16];

// Function Prototypes
void LCD_Init_4bit(void);
void LCD_Command_4bit(unsigned char);
void LCD_Write_4bit(unsigned char, bit);
void LCD_String_4bit(char*);
void LCD_Clear(void);
void delay_ms(unsigned int);
void delay_us(unsigned int);
bit DHT11_Start(void);
unsigned char DHT11_Read_Byte(void);
bit DHT11_Read_Data(void);
void Update_Display(void);

void main() {
    // Initialize LCD
    LCD_Init_4bit();
    LCD_Clear();
    LCD_String_4bit("DHT11 Sensor");
    
    while(1) {
        if(DHT11_Read_Data()) {
            Update_Display();
        } else {
            LCD_Command_4bit(0xC0);
            LCD_String_4bit("Read Error!");
        }
        delay_ms(2000); // Wait 2 seconds between readings
    }
}

/* DHT11 Communication Functions */
bit DHT11_Start(void) {
    DHT11_PIN = 0;      // Pull low for 18ms
    delay_ms(18);
    DHT11_PIN = 1;      // Release bus
    delay_us(30);       // Wait 20-40us
    
    // Check DHT11 response
    if(!DHT11_PIN) {
        delay_us(80);   // Wait for low pulse (80us)
        if(DHT11_PIN) {
            delay_us(80); // Wait for high pulse (80us)
            return 1;   // DHT11 responded
        }
    }
    return 0;           // No response
}

unsigned char DHT11_Read_Byte(void) {
    unsigned char i, byte = 0;
    
    for(i=0; i<8; i++) {
        while(!DHT11_PIN);  // Wait for high
        delay_us(30);       // Wait 30us
        
        // If still high after 30us, it's a '1'
        if(DHT11_PIN) {
            byte |= (1 << (7-i));
            while(DHT11_PIN); // Wait for low
        }
    }
    return byte;
}

bit DHT11_Read_Data(void) {
    if(!DHT11_Start()) return 0;
    
    humidity_int = DHT11_Read_Byte();
    humidity_dec = DHT11_Read_Byte();
    temp_int = DHT11_Read_Byte();
    temp_dec = DHT11_Read_Byte();
    checksum = DHT11_Read_Byte();
    
    // Verify checksum
    if((humidity_int + humidity_dec + temp_int + temp_dec) != checksum)
        return 0;
    
    return 1;
}

/* Display Functions */
void Update_Display(void) {
    LCD_Command_4bit(0xC0); // Second line
    sprintf(lcd_buffer, "H:%d%% T:%dC", humidity_int, temp_int);
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