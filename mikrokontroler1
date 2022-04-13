#include <p24fj128ga010.h>
#include <stdlib.h>
#include <time.h>

//Konfiguracja dla Explorer 16 z progr. icd2
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & FWDTEN_OFF)
_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRI)

#define SCALE 308L

int main(void){
unsigned long i;
unsigned int zadanie=8;
unsigned char display=0;
unsigned int x=0;
unsigned int y=7;
unsigned int snake=0;
unsigned int k=1;
unsigned int J=0;
unsigned int D=0;
unsigned int licznik=0;
unsigned int jeden = 0;
unsigned int dwa = 0;
unsigned int trzy = 0;
unsigned int cztery = 0;
unsigned int piec = 0;
unsigned int szesc = 0;
unsigned int siedem =0;
unsigned char display_dla_8 = 1;
//inicjalizacja
 PORTA=0x0000;
 TRISA=0xFF00;
 TRISD=0xFFFF;
  
again:
	Nop();
	PORTA=(unsigned int) display;
	for(i=500L*SCALE;i>0;i--) Nop();

	if (PORTDbits.RD13 == 0){
		if(zadanie==9)
			zadanie=1;
		else
			zadanie=zadanie+1;
	}
	else if (PORTDbits.RD6==0)
	{
		if(zadanie==1)
			zadanie=9;
		else
			zadanie=zadanie-1;
	}
	switch (zadanie){
	
	case 1:
		display=display+1;
		break;

	case 2:
		display=display-1;
		break;

	case 3:
		display=x^(x>>1);
		x++;
		break;

	case 4:
		display=x^(x>>1);
		x--;
		if(x==0)
			x=255;
		else
			x--;
		break;

	case 5:
		licznik++;
		D=licznik/10;
		J=licznik%10;
		display=(D<<4) | J;
		break;

	case 6:
		licznik--;
		D=licznik/10;
		J=licznik%10;
		display=(D>>4) | J;	
		break;

	case 7:
		display=y;
			if(snake==0)
			{
				y=y<<1;
			}
			else if(snake==1)
			{
				y=y>>1;
			}
			if(y==224)
			{
				snake=1;
			}else if(y==7){
				snake=0;
			}
		break;

	case 8:
		
		
		if(siedem == 1 && display_dla_8 == 255){
			siedem = 0;
			szesc = 0;
			piec = 0;
			cztery = 0;
			trzy = 0;
			dwa = 0;
			jeden = 0;
			display_dla_8 = display_dla_8 << 1;
			display_dla_8 = 1;
		}
		else if(siedem == 1){
			display_dla_8 =display_dla_8 << 1;
		}
		else if(szesc == 1 && display_dla_8 == 254){
			siedem = 1;
			display_dla_8 =display_dla_8 << 1;
			display_dla_8 = 1;
		}
		else if(piec == 1 && display_dla_8 == 252){
			szesc = 1;
			display_dla_8 = display_dla_8 << 1;
			display_dla_8 = 1;
		}
		else if(cztery == 1 && display_dla_8 == 248){
			piec = 1;
			display_dla_8 = display_dla_8 << 1;
			display_dla_8 = 1;
		}
		else if(trzy == 1 && display_dla_8 == 240){
			cztery = 1;
			display_dla_8 =display_dla_8 << 1;
			display_dla_8 = 1;
		}
		else if(dwa == 1 && display_dla_8 == 224){
			trzy = 1;
			display_dla_8 = display_dla_8 << 1;
			display_dla_8 = 1;
		}
		else if(jeden == 1 && display_dla_8 == 192){
			dwa = 1;
			display_dla_8 = display_dla_8 << 1;
			display_dla_8 = 1;
		} else if(display_dla_8 == 128){
			jeden = 1;
			display_dla_8 = display_dla_8 << 1;
			display_dla_8 = 1;
		} else {
			display_dla_8 = display_dla_8 << 1;
		}
	
		if(siedem){
			display_dla_8 = display_dla_8 | 254;
		}
		else if(szesc){
			display_dla_8 =display_dla_8 | 252;
		}
		else if(piec){
			display_dla_8 = display_dla_8 | 248;
		}
		else if(cztery){
			display_dla_8 = display_dla_8 | 240;
		}
		else if(trzy){
			display_dla_8 = display_dla_8 | 224;
		}
		else if(dwa){
			display_dla_8 =display_dla_8 | 192;
		}
		else if(jeden){
			display_dla_8 = display_dla_8 | 128;
		}
		display = display_dla_8;
		break;
	
	case 9:
		srand(time(NULL));
		int i;
		i =rand()%116;
		display=i;
		break;
	}
	goto again;
}
