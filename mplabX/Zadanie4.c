#include <stdio.h>
#include <string.h>
// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF

#include <xc.h>

#define LENA  PORTEbits.RE1
#define LDAT  PORTEbits.RE2
#define LPORT PORTD

#define L_ON	0x0F
#define L_OFF	0x08
#define L_CLR	0x01
#define L_L1	0x80
#define L_L2	0xC0
#define L_CR	0x0F		
#define L_NCR	0x0C	

#define L_CFG   0x38

void delay(unsigned int ms)
{
    unsigned int i;
    unsigned char j;
    
 for (i =0; i< ms; i++)
 {
 
  for (j =0 ; j < 200; j++)
   {
      Nop();
      Nop();
      Nop();
      Nop();
      Nop();
   }
 }
}

unsigned int adc(unsigned char kanal)
{
    switch(kanal)
    {
        case 0: ADCON0=0x01; break; //P1
        case 1: ADCON0=0x05; break; //P2
        case 2: ADCON0=0x09; break; 
    }
    
    ADCON0bits.GO=1;
    while(ADCON0bits.GO == 1);

   return ((((unsigned int)ADRESH)<<2)|(ADRESL>>6));
}

void lcd_wr(unsigned char val)
{
  LPORT=val;
}

void lcd_cmd(unsigned char val)
{
	LENA=1;
        lcd_wr(val);
        LDAT=0;
        //delay(3);
        LENA=0;
        //delay(3);
	LENA=1;
}
 
void lcd_dat(unsigned char val)
{
	LENA=1;
        lcd_wr(val);
        LDAT=1;
        //delay(3);
        LENA=0;
        //delay(3);
	LENA=1;
}

void lcd_init(void)
{
	LENA=0;
	LDAT=0;
	delay(20);
	LENA=1;
	
	lcd_cmd(L_CFG);
	delay(5);
	lcd_cmd(L_CFG);
        delay(1);
	lcd_cmd(L_CFG); //configura
	lcd_cmd(L_OFF);
	lcd_cmd(L_ON); //liga
	lcd_cmd(L_CLR); //limpa
	lcd_cmd(L_CFG); //configura
        lcd_cmd(L_L1);
}

void lcd_str(const char* str)
{
 unsigned char i=0;
  
 while(str[i] != 0 )
 {
   lcd_dat(str[i]);
   i++;
 }  
}

void main(void) {
    
    //Inicjalizacja konwertera analogowo cyfrowego
    ADCON0=0x01;
    ADCON1=0x0B;
    ADCON2=0x01;
    
    TRISA=0xC3;
    TRISB=0x3F;   
    TRISC=0x01;
    TRISD=0x00;
    TRISE=0x00;
    
    lcd_init(); //Inicjalizacja wy?wietlacza
    lcd_cmd(L_CLR); //Czyszczenie wy?wietlacza
   unsigned int time;
   unsigned char tim[3]="0";
   typedef enum { F, T } boolean;
   unsigned char Power[4];
   unsigned char sec[2]="0";
   unsigned char min[3]="0";    
   boolean start=F;
  // { "800W", "600W" , "350W" , "200W"}
    strcpy(Power,"200W");

    lcd_cmd(L_L1); //Przej??ie do drugiej linii
    lcd_str("Moc:        "); //napis
    lcd_str(Power); //napis
    lcd_cmd(L_L2); //Ustawienie karetki w pierwszej linii
    lcd_str("Czas:      "); //napis
    lcd_str(min); //napis
    lcd_str(":");
    lcd_str("0"); //napis

    while(1)
    {
        
       sprintf(min, "%d", time/60);
       sprintf(sec, "%d", time%60);
       if(PORTBbits.RB1 == 0)
       {
           delay(100);
           if(start==F)
           {
               start=T;
           }
           else
           {
               start=F;
           }
       }
       
       if(PORTBbits.RB2 == 0)
       {
           time=0;

        
        lcd_cmd(L_L1); //Przej??ie do drugiej linii
        lcd_str("Moc:        "); //napis
        lcd_str(Power); //napis
        lcd_cmd(L_L2); //Ustawienie karetki w pierwszej linii
        lcd_str("Czas:      "); //napis
        lcd_str("0"); //napis
        lcd_str(":");
        lcd_str("00"); //napis
        lcd_str("   ");   
       }  
       
       if(PORTBbits.RB3 == 0)
       {
        delay(100);
        time+=10;

        lcd_cmd(L_L1); //Przej??ie do drugiej linii
        lcd_str("Moc:        "); //napis
        lcd_str(Power); //napis
        lcd_cmd(L_L2); //Ustawienie karetki w pierwszej linii
        lcd_str("Czas:      "); //napis
        lcd_str(min); //napis
        lcd_str(":");
        lcd_str(sec); //napis
        lcd_str("   ");   
      
       }
       
       if(PORTBbits.RB4 == 0)
       {
        delay(100);
        time+=60;

        
        lcd_cmd(L_L1); //Przej??ie do drugiej linii
        lcd_str("Moc:        "); //napis
        lcd_str(Power); //napis
        lcd_cmd(L_L2); //Ustawienie karetki w pierwszej linii
        lcd_str("Czas:      "); //napis
        lcd_str(min); //napis
        lcd_str(":");
        lcd_str(sec); //napis
        lcd_str("   ");

       }
       if(PORTBbits.RB5 == 0)
       {
           delay(100);
        if(strcmp(Power, "200W") == 0)
        {
        strcpy(Power,"350W");
      
        }
        else if(strcmp(Power, "350W") == 0)
        {
         strcpy(Power,"600W");
        }
        else if(strcmp(Power, "600W") == 0)
        {
        strcpy(Power,"800W");

        }
        else
        {
        strcpy(Power,"200W");
        }
        lcd_cmd(L_L1); //Przej??ie do drugiej linii
        lcd_str("Moc:        "); //napis
        lcd_str(Power); //napis
        lcd_cmd(L_L2); //Ustawienie karetki w pierwszej linii
        lcd_str("Czas:      "); //napis
        lcd_str(min); //napis
        lcd_str(":");
        lcd_str(sec); //napis 
        lcd_str("   ");        
       }

        if(start==T){
            delay(100);
            if(time!=0)
            {
                time-=1;
            }
            
         lcd_cmd(L_L1); //Przej??ie do drugiej linii
        lcd_str("Moc:        "); //napis
        lcd_str(Power); //napis
        lcd_cmd(L_L2); //Ustawienie karetki w pierwszej linii
        lcd_str("Czas:      "); //napis
        lcd_str(min); //napis
        lcd_str(":");
        lcd_str(sec); //napis
        lcd_str("   ");
  
        }

    }
    
    return;
}
