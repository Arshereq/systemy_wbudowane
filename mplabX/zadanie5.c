//CONFIG1H
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
#include <stdio.h>

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
    
    unsigned int timeLength=0; //Warto?? timeLengthu
    
    int time=0; //Zmienna mowi?ca czy ustawiono czas
    int minP1=0; //Minuty P1
    int secP1=0; //Sekundy P1
    char minStr[50]; //Char przechowuj?cy minuty P1
    char secStr[50]; //Char przechowuj?cy sekundy P1
    
    int minP2=0; //Minuty P2
    int secP2=0;  //Sekundy P2
    char minStr1[50]; //Char przechowuj?cy minuty P2
    char secStr1[50]; //Char przechowuj?cy sekundy P1
    
    int btnP1=0; //Zmienna definiuj?ca start/stop P1
    int btnP2=0; //Zmienna definiuj?ca start/stop P2

    while(1)
    {
        delay(200); //Poczekaj 200ms

        if(PORTBbits.RB3==0)//Czas gracza P1
        {
            time=1; //Czas zosta? ustawiony
            btnP1=0; //Zmienna btnP1 ustawiona na 0
            while(minP1>=0) //Odliczanie zako?czy si? je?eli liczba minut == 0
            {
                for(int i= secP1;i>=0;i--) //Odliczanie sekund
                {
                    delay(500); //Poczekaj pó? sekundy
                    if(PORTBbits.RB5 == 0) //Je?eli podczas odliczania zostanie btnP1ni?ty RB5
                    {
                        btnP1=1; //To zmienna btnP1 = 1
                        break; //I przerywamy dzia?anie p?tli
                    }
                    if(i==0) //Je?eli dojdziemy do "zerowej" sekundy
                    {
                        if(minP1!=0) //I liczba minut jest ró?na od 0
                        {
                            minP1=minP1-1; //To zmniejszamy minuty o 1
                            secP1=59; //A sekundy ustawiamy na 59
                        }    
                   }
                   else //W innym przypadku 
                   {
                        secP1=secP1-1; //Po prostu zmniejszamy liczb? sekund
                   } 
                   delay(500); //Poczekaj pó? sekundy
                   lcd_cmd(L_CLR); //Wyczy?? ekran
                   sprintf(minStr,"%d",minP1); //Zamiana liczby minut P1 na typ char
                   sprintf(secStr,"%d",secP1); //Zamiana liczby sekund P1 na typ char
                   lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
                   lcd_str("Gracz 1    "); //napis
                   lcd_str(minStr); //Wy?wietlenie minut P1
                   lcd_str(":"); //napis
                   lcd_str(secStr); //Wy?wietlenie sekind P1

                   sprintf(minStr1,"%d",minP2); //Zamiana liczby minut P2 na typ char
                   sprintf(secStr1,"%d",secP2); //Zamiana liczby sekund P2 na typ char

                   lcd_cmd(L_L2); //Przej?cie do drugiej linii
                   lcd_str("Gracz 2    "); //napis
                   lcd_str(minStr1); //Wy?wietlenie minut P2
                   lcd_str(":"); //napis
                   lcd_str(secStr1); //Wy?wietlenie sekund P2
                   
                }  
                if(btnP1 == 1) //Je?eli wcze?niej zosta? klini?ty RB5 (btnP1=1)
                {
                    break; //Przerwij odliczanie
                }
                if(minP1==0&&secP1==0) //Je?eli czas gracza si? sko?czy?
                {
                    lcd_cmd(L_CLR); //Wyczy?? ekran
                    lcd_cmd(L_L1);  //Przej?cie do pierwszej linii
                    lcd_str("Gracz1 przegral"); //napis
                    lcd_cmd(L_L2); //Przej?cie do drugiej linii
                    lcd_str("koniec czasu"); //napis
                    minP2=0; //Minuty P2 time na 0
                    secP2=0; //Sekundy P2 time na 0
                    delay(5000); //Poczekaj pó? sekundy
                    time=0; //Zmie? zmienn? 'ustwione' na 0
                    break; //Przerwij
                }
            }
        }
        if(PORTBbits.RB5==0)//Czas gracza P2
        {
            time=1; //Czas zosta? ustawiony
            btnP2=0; //Zmienna btnP1 ustawiona na 0
            while(minP2>=0) //Odliczanie zako?czy si? je?eli liczba minut == 0
            {
                for(int i= secP2;i>=0;i--) //Odliczanie sekund
                {
                    delay(500); //Poczekaj pó? sekundy
                    if(PORTBbits.RB3 == 0) //Je?eli podczas odliczania zostanie btnP1ni?ty RB3
                    {
                        btnP2=1; //To zmienna btnP2 = 1
                        break; //I przerywamy dzia?anie p?tli
                    }
                    if(i==0) //Je?eli dojdziemy do "zerowej" sekundy
                    {
                        if(minP2!=0) //I liczba minut jest ró?na od 0
                        {
                            minP2=minP2-1; //To zmniejszamy minuty o 1
                            secP2=59; //A sekundy ustawiamy na 59
                        }    
                   }
                   else //W innym przypadku 
                   {
                        secP2=secP2-1; //Po prostu zmniejszamy liczb? sekund
                   } 
                   delay(500); //Poczekaj pó? sekundy
                   lcd_cmd(L_CLR); //Wyczy?? ekran
                   sprintf(minStr,"%d",minP1); //Zamiana liczby minut P1 na typ char
                   sprintf(secStr,"%d",secP1); //Zamiana liczby sekund P1 na typ char
                   lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
                   lcd_str("Gracz 1    "); //napis
                   lcd_str(minStr); //Wy?wietlenie minut P1
                   lcd_str(":"); //napis
                   lcd_str(secStr); //Wy?wietlenie sekind P1

                   sprintf(minStr1,"%d",minP2); //Zamiana liczby minut P2 na typ char
                   sprintf(secStr1,"%d",secP2); //Zamiana liczby sekund P2 na typ char

                   lcd_cmd(L_L2); //Przej?cie do drugiej linii
                   lcd_str("Gracz 2    "); //napis
                   lcd_str(minStr1); //Wy?wietlenie minut P2
                   lcd_str(":"); //napis
                   lcd_str(secStr1); //Wy?wietlenie sekund P2
                   
                }  
                if(btnP2 == 1) //Je?eli wcze?niej zosta? klini?ty RB5 (btnP1=1)
                {
                    break; //Przerwij odliczanie
                }
                if(minP2==0&&secP2==0) //Je?eli czas gracza si? sko?czy?
                {
                    lcd_cmd(L_CLR); //Wyczy?? ekran
                    lcd_cmd(L_L1);  //Przej?cie do pierwszej linii
                    lcd_str("Gracz2 przegral"); //napis
                    lcd_cmd(L_L2); //Przej?cie do drugiej linii
                    lcd_str("koniec czasu"); //napis
                    minP1=0; //Minuty P1 time na 0
                    secP1=0; //Sekundy P1 time na 0
                    delay(5000); //Poczekaj pó? sekundy
                    time=0; //Zmie? zmienn? 'ustwione' na 0
                    break; //Przerwij
                }
            }
        }
        
        if(time==0)
        {
        timeLength=((unsigned int)adc(1) / 10); //Warto?? timeLengthu P2
        
        if(timeLength>=0 && timeLength<=(100/3)*1) //Je?eli mie?ci si? w 1/3 zakresu
        {
			//To ustaw czas na 1 minP1:
            minP1=1;
            minP2=1; 
        }
        if(timeLength>(100/3)*1 && timeLength<=(100/3)*2) //Je?eli mie?ci si? w 2/3 zakresu
        {
			//To ustaw czas na 3 minP1:
            minP1=3;
            minP2=3; 
        }
        if(timeLength>(100/3)*2 && timeLength<=(100/3)*3) //Je?eli mie?ci si? w 3/3 zakresu
        {
			//To ustaw czas na 5 minP1:
            minP1=5;
            minP2=5; 
        }
        }
        lcd_cmd(L_CLR); //Wyczyszczenie ekranu
        
        sprintf(minStr,"%d",minP1); //Zamiana liczby minut P1 na typ char
        sprintf(secStr,"%d",secP1); //Zamiana liczby sekund P1 na typ char
        
        lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
        lcd_str("Gracz 1    "); //napis
        lcd_str(minStr); //Wy?wietlanie minut P1
        lcd_str(":"); //napis
        lcd_str(secStr); //Wy?wietlanie sekund P1
        
        sprintf(minStr1,"%d",minP2); //Zamiana liczby minut P2 na typ char
        sprintf(secStr1,"%d",secP2); //Zamiana liczby sekund P2 na typ char
        
        lcd_cmd(L_L2); //Przej?cie do drugiej linii
        lcd_str("Gracz 2    "); //napis
        lcd_str(minStr1); //Wy?wietlanie minut P2
        lcd_str(":"); //napis
        lcd_str(secStr1); //Wy?wietlanie sekund P2
    }
    
    return;
}
