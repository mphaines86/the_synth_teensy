// LCD MODULES void lcdCmd(char cmd)

#include <cstdint>

#define NSDELAY nsdelay = 1 / F_CPU * 1000000000;

char Swap_Bits(char data)
{
	char result= 0;
	char finalresult=0;

	for(char i = 0; i <= 7; ++i)
	{
		if (data & 1 << i) result+= (256 >> (i+1));
	}
	finalresult = 0x0F & (result >>4);
	finalresult += (0xF0 & (result << 4));
	return finalresult;
}

void DelayCycle(){

	uint8_t nsdelay =  1000000000 / F_CPU;

	while (nsdelay != 0) {
		asm("NOP");
		nsdelay--;
	}
}

void Delay_ns(int delayns){
	while (delayns != 0){
		DelayCycle();
		delayns--;
	}
}

void Delay_us(int delayus){
	while (delayus != 0){
		Delay_ns(1000);
		delayus--;
	}

}

void Delay_ms(int delayms){
	while (delayms != 0){
		Delay_us(1000);
		delayms--;
	}
}
