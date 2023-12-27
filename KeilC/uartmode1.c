#include <main.h>
#include <uartmode1.h>
void Ext_int_Init()				
{
	EA = 1;								// Enable Global interrupt 
	ES = 1;      					// Enable Ext. interrupt0 		
}
void uart_Init()
{
	SCON = 0x50;					// SCON: mode 1, 8bit-uart, enable receive
	TMOD = 0x20; 					// TMOD: timer 1, mode 2, 8bit, tu nap lai
	TH1 = 0xFD; 					// TH1: Thach Anh 11.0596Mhz, baud 9600           
	TR1 = 1;							// TR1: timer 1 run
			
}
void send_char(unsigned char chr)
{
	SBUF = chr;						// TI: Tranfer Interrupt
	while(TI==0);					// TI flag = 0 tuc la dang truyen du lieu, cho cho chuyen xong
	TI=0;									// TI dc bat len 1 khi bit cuoi cung dc truyen di
}
void send_str(unsigned char *str)
{
	while(*str)
	{
		send_char(*str++);
	}

}
/*
char data_ready()  					// Co RI = 0: chua nhan xong du lieu : Receiver Interrupt
{														// Co RI = 1: Da nhan xong du lieu
	return RI;
}

char read_char()
{
	RI = 0;
	return SBUF;
}
*/			