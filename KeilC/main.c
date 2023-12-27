#include <main.h>
#include <uartmode1.h>
#include <lcd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

sbit r1 = P2^0;					  // Relay 1: Chan P2_0
sbit r2 = P2^1;					  // Relay 2: Chan P2_1
sbit r3 = P2^2;						// Relay 3: Chan P2_3
			
void gsm_Init();
void send_sms(unsigned char *sms);
void hang_call();
void rst_state();
void extract_sms();
void do_action();
void nhiet_do();
idata char buff[100];										// Bo nho dem buff, Luu bien vung idata
volatile int status_flag = 0;						// Co bao nhan Uart
volatile int pointer =0;								// Bien Volatile: co the thay doi ma khong bao trc 											
																				// Bien toan cuc dc truy xuat tu ct con xu ly ngat (ISR)
char *control_number = "+84967992298";  // Sdt dieu khien
idata char number[15];									// Bien luu sdt tach ra tu tin nhan
int nhietdo;
void main()
{
	r1 = r2 = r3 = 0;											// Tat het relay
	LCD_Init();
	delay_ms(20);
	LCD_Clear();
	LCD_Gotoxy(0,0);
	LCD_Puts("Initializing.");
	delay_ms(1000);
	LCD_Clear();
	LCD_Gotoxy(0,0);
	LCD_Puts("Initializing..");
	delay_ms(1000);
	LCD_Clear();
	LCD_Gotoxy(0,0);
	LCD_Puts("Initializing...");
	delay_ms(1000);
	uart_Init(); 
	Ext_int_Init();
	gsm_Init();
	while(1)
	{
			LCD_Clear();
			LCD_Gotoxy(0,0);
			LCD_Puts("Waiting...");
			LCD_Gotoxy(0,1);
			LCD_Puts("LEDUNG - 1912950");
			if(status_flag==1)
			{
					delay_ms(500);
					if(strstr(buff,"+CMT"))
			{
					extract_sms();
					if(strstr(number,control_number)!=0)
					{
					do_action();
					}
			}
			if(strstr(buff,"RING"))			// Chuoi con khac null => Co RING	
			{
			  hang_call();
				
				// Hien thi hieu ung
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("Incoming");
				delay_ms(100);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("Incoming.");
				delay_ms(800);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("Incoming..");
				delay_ms(800);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("Incoming...");
				delay_ms(500);
				LCD_Gotoxy(0,1);
				LCD_Puts("Cacelled");
				delay_ms(3000);
			}
		}
		nhiet_do();
	}
}
void nhiet_do()
{
			if(nhietdo>=40)
			{ 
						r3 = 1;
						send_sms("Nhiet Do Cao, Da Tu Dong Bat Quat ");
						LCD_Clear();
						LCD_Gotoxy(0.0);
						LCD_Puts(" Qua Nhiet Do!");
						delay_ms(100);
						LCD_Clear();
						LCD_Gotoxy(0,0);
						LCD_Puts(" Qua Nhiet Do!!");
						delay_ms(200);
						LCD_Clear();
						LCD_Gotoxy(0,0);
						LCD_Puts(" Qua Nhiet Do!!!"); 
						delay_ms(2000);
			}
}
void rst_state()
{
	memset(buff,0,strlen(buff));
	pointer = 0;
	status_flag = 0;
}
void gsm_Init()
{
		send_str("ATE0\r\n");		    			// Tat echo
		//send_char(0x0D); 								// <CR> : Ve dau dong = \r
		//send_char(0x0A);		    				// <LF> : New line	  = \n
		delay_ms(200);
			
		send_str("AT+IPR=9600\r\n");			// Set baud = 9600
		delay_ms(200);
	
		send_str("AT+CMGF=1\r\n");  			// Text mode
		delay_ms(200);
	
		send_str("AT+CNMI=2,2,0,0\r\n");  // Hien thi noi dung khi co tin nhan toi
		delay_ms(200);
		//memset(buff,0,strlen(buff));
		send_str("AT&W\r\n");	  					// Luu cac thiet lap
		delay_ms(2000);		
		
		//if(strstr(buff,"OK"))
		send_sms("Da Khoi Dong Thanh Cong\n\r");
		LCD_Clear();
		LCD_Gotoxy(0,0);
		LCD_Puts("Ready...");
		delay_ms(2000);
}
void send_sms(unsigned char *sms)
{
	send_str("AT+CMGS=");
	send_char(0x22);				// 0x22: "
	send_str(control_number);
	send_char(0x22);				// 0x22: "
	send_char(0x0D);			
	send_char(0x0A);
	delay_ms(200);					// Doi dau >
	send_str(sms);
	send_char(0x1A);			 	// Ctrl + Z de gui tin nhan
  delay_ms(300);					// Delay 1s doi SIM gui tin nhan
	rst_state();
} 
void hang_call()
{
	send_str("ATH\r\n");
	delay_ms(200);
	rst_state();
}
void do_action()
{
	if(buff[pointer]=='B'&&buff[pointer+1]=='a'&&buff[pointer+2]=='t'&&buff[pointer+3]=='1')
	{	
				r1=1;
				rst_state();
				send_sms("Relay1: ON\r\n");
			
				// LCD Animation
		
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task");
				delay_ms(100);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task.");
				delay_ms(500);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task..");
				delay_ms(500);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task...");
				delay_ms(500);
				LCD_Gotoxy(0,1);
				LCD_Puts("Relay1 is ON");
				delay_ms(2000);

	}
	else if(buff[pointer]=='T'&&buff[pointer+1]=='a'&&buff[pointer+2]=='t'&&buff[pointer+3]=='1')	
	{	
			r1=0;
			rst_state();
			send_sms("Relay1: OFF\r\n");
			
			LCD_Clear();
			LCD_Gotoxy(0,0);
			LCD_Puts("1 new task");
			delay_ms(100);
			LCD_Clear();
			LCD_Gotoxy(0,0);
			LCD_Puts("1 new task.");
			delay_ms(500);
			LCD_Clear();
			LCD_Gotoxy(0,0);
			LCD_Puts("1 new task..");
			delay_ms(500);
			LCD_Clear();
			LCD_Gotoxy(0,0);
			LCD_Puts("1 new task...");
			delay_ms(500);
			LCD_Gotoxy(0,1);
			LCD_Puts("Relay1 is OFF");
			delay_ms(2000);
	}	
	else if(buff[pointer]=='B'&&buff[pointer+1]=='a'&&buff[pointer+2]=='t'&&buff[pointer+3]=='2')
	{	
				r2=1;
				rst_state();
				send_sms("Relay2: ON\r\n");
			
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task");
				delay_ms(100);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task.");
				delay_ms(500);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task..");
				delay_ms(500);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task...");
				delay_ms(500);
				LCD_Gotoxy(0,1);
				LCD_Puts("Relay2 is ON");
				delay_ms(2000);
	}
	else if(buff[pointer]=='T'&&buff[pointer+1]=='a'&&buff[pointer+2]=='t'&&buff[pointer+3]=='2')
	{
				r2=0;
				rst_state();
				send_sms("Relay2: OFF\r\n");
			
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task");
				delay_ms(100);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task.");
				delay_ms(500);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task..");
				delay_ms(500);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("1 new task...");
				delay_ms(500);
				LCD_Gotoxy(0,1);
				LCD_Puts("Relay2 is OFF");
				delay_ms(2000);
	}
	else send_sms("Syntax Error!");		
}
void extract_sms()
{
	int i=0;
	pointer=0;
	while(1)
	{
	 	if(buff[pointer]=='\r'||buff[pointer]=='\n')	// Loai bo \n \r truoc chuoi buffer luu gia tri nhan uart
	 	{
			pointer++;
		}
		else
			break;                              				// +CMT: "+84967992298","","21/12/28,01:35:03+28"
																									// Bat1
	}
																									// Phan hoi tu sim co dang:
	while(buff[pointer]!='"')				  							
	{										  	
		pointer++;
	}
	pointer=pointer+1;				  		  							// i<12 : sdd 10 so, i<=12: 11 so
	for(i=0;i<12;i++)
	{
		number[i]=buff[pointer];
		pointer++;
	}
	//pointer =0;									    
	while(buff[pointer]!='\n')      
	{				
		pointer++;	  
	}																								//	#CR#LF+CMT: "+84967992298","","21/12/28,23:57:16+28"#CR#LFtat1#CR#LF
  	pointer=pointer+1;
}
void ISR_UART(void) interrupt 4
{
	if(RI!=0)									// Co RI = 1
	{
		EA = 0;									// Khong co ngat nao duoc dap ung nua
		buff[pointer] = SBUF;		// Copy SBUF(received value) to buffer 
		pointer++;							// Tang con tro len
		status_flag = 1;				// Flag for new uart arrival 
		RI = 0;									// Set RI = 0 de nhan byte du lieu tiep theo
		EA = 1;									// Tat ca moi ngat deu dc bat lai
	}
}