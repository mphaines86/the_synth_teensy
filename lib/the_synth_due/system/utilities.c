// LCD MODULES void cmd2LCD(char cmd)

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

void Delay(void){
	float usdelay = 0.0;
	usdelay =(1000/ ( (1.0 / 28000000) * 1000000000 ) );

	while (usdelay != 0) {
		asm("NOP");
		usdelay--;
	}
}

void Delay_us(int delayus){
	while (delayus != 0){
		Delay();
		delayus--;
	}

}

void Delay_ms(int delayms){
	while (delayms != 0){
		Delay_us(1000);
		delayms--;
	}
}
