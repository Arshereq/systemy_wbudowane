#include <p24fj128ga010.h>

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & FWDTEN_OFF)
_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRI)

#define SCALE 308L

/* numer którym dostajemy się do czujnika temperatury */

#define TSENS 4
#define AINPUTS 0xffcf
#define TIME 3000L
void ADCinit(int amask)

{
	AD1PCFG = amask;
	AD1CON1 = 0x00e0;
	AD1CSSL = 0;
	AD1CON2 = 0;
	AD1CON3 = 0x1f02;
	AD1CON1bits.ADON = 1;
}

int readADC(int ch)

{
	AD1CHS = ch;
	AD1CON1bits.SAMP = 1;
	while (!AD1CON1bits.DONE);
	return ADC1BUF0;
}


int main(void)
{
	TRISA = 0xFF00;
	TRISD = 0xFFFF;

	unsigned long i;
	unsigned char display = 0;
	int temp;
	int timer = TIME;
	unsigned int szybkosc = 500L;

	PORTA = 0x0000;
	ADCinit(AINPUTS); /*inicjalizacja konwertera AD*/

	display = 0;

	while (1) {

		Nop();
		PORTA=(unsigned int) display;
		for (i = szybkosc * SCALE; i > 0; i--) 
			Nop();
		
		temp = readADC(TSENS);
		
		if(temp > 250) {
			display = 255;
		}
		else if( temp && timer > 0) {
			if(display != 1) {
				display = 1;
				timer -= szybkosc;
			}	
			else {
				display = 0;
			}
		}
		else {
			timer = TIME;
			display = 0;
		}
	
		if(PORTDbits.RD6 == 0) {
			timer = TIME;
			display = 0;
		}
	}
}
