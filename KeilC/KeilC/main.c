#include"main.h"
#include"uartmode1.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

sbit relay1 = P2^1;					  // Relay 1: Chan P2_1
sbit relay2 = P2^0;					  // Relay 2: Chan P2_0

void delay_ms(unsigned int time);				
void gsm_Init();
void send_sms(unsigned char *sms);
void hang_call();
//void delete_all_msg();
void extract_sms();
void do_action();

idata char buff[100];					// Bo nho dem buff, Luu bien vung idata
char status_flag = 0;					// Co bao nhan Uart
volatile int pointer;					// Bien Volatile: co the thay doi ma khong bao trc 											
										// Bien toan cuc dc truy xuat tu ct con xu ly ngat (ISR)
char *control_number = "+84967992298";  // Sdt dieu khien
idata char number[15];					// Bien luu sdt tach ra tu tin nhan
//idata char nd[15];					// Bien luu noi dung tach ra tu tin nhan

void main()
{
	relay1 = relay2 = 0;
	pointer = 0;
	uart_Init(); 
	Ext_int_Init();
	gsm_Init();
	while(1)
	{
		if(status_flag==1)
		{
			delay_ms(500);
			if(strstr(buff,"+CMT"))
			{
				extract_sms();
				delay_ms(300);
				if(strstr(number,control_number)!=0)
				{
					do_action();
				}
			
			}
			if(strstr(buff,"RING"))		// Chuoi con khac null => Co RING	
			{
			  	hang_call();
			}
			//delete_all_msg();
			
			pointer=0;
			status_flag=0;
			memset(buff,0,strlen(buff));
      		//memset(nd,0,strlen(nd));
			memset(number,0,strlen(number));
		}						
	}
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
			break;                              // +CMT: "+84967992298","","21/12/28,01:35:03+28"
												// Bat1
	}
												// Phan hoi tu sim co dang:
	while(buff[pointer]!='"')				  	// +CMT: "+84967992298","","DD/MM/YY,gio:phut:giay+tichtac" \n
	{										  	// . NoiDungTinNhan
		pointer++;
	}
	pointer=pointer+1;				  		  	// i<12 : sdd 10 so, i<=12: 11 so
	for(i=0;i<12;i++)
	{
		number[i]=buff[pointer];
		pointer++;
	}
	pointer =0;									    
	while(buff[pointer]!='\n')      //**********************************
	{				
		pointer++;	  
	}								//	#CR#LF+CMT: "+84967992298","","21/12/28,23:57:16+28"#CR#LFtat1#CR#LF
  	pointer=pointer+1;
	
	//send_char(buff[pointer]);
	//send_char(buff[pointer+1]);	
	//send_char(buff[pointer+2]);
	//send_char(buff[pointer+3]);
	//noidung[0] = buff[pointer];
	//noidung[1] = buff[pointer+1];
	//noidung[2] = buff[pointer+2];
	//noidung[3] = buff[pointer+3];
}
void delay_ms(unsigned int time)
{
	int i,j;
	for(i=0;i<time;i++)
	for(j=0;j<124;j++);						// Delay 1ms
}

void gsm_Init()
{
  while(1)
	{
		send_str("ATE0\r\n");		    		// Tat echo
		//send_char(0x0D); 					// <CR> : Ve dau dong = \r
		//send_char(0x0A);		    		// <LF> : New line	  = \n
		delay_ms(200);
			
		send_str("AT+IPR=9600\r\n");			// Set baud = 9600
		delay_ms(200);
	
		send_str("AT+CMGF=1\r\n");  			// Text mode
		delay_ms(200);
	
		send_str("AT+CNMI=2,2,0,0\r\n");  	// Hien thi noi dung khi co tin nhan toi
		delay_ms(200);
		memset(buff,0,strlen(buff));
		send_str("AT&W\r\n");	  				// Luu cac thiet lap
		delay_ms(2000);		
		
		if(strstr(buff,"OK"))
		{
			send_sms("Da Khoi Dong Thanh Cong\n\r");
			break;
	 	}			
	}	
}
void send_sms(unsigned char *sms)
{
	send_str("AT+CMGS=");
	send_char(0x22);				// 0x22: "
	send_str(control_number);
	send_char(0x22);				// 0x22: "
	send_char(0x0D);			
	send_char(0x0A);
	delay_ms(300);					// Doi dau >
	send_str(sms);
	send_char(0x1A);			 	// Ctrl + Z de gui tin nhan
  	delay_ms(1000);					// Delay 1s doi SIM gui tin nhan
	memset(buff,0,strlen(buff));
	pointer = 0;
	status_flag = 0;
} 
/*
void delete_all_msg()
{
	send_str("AT+CMGDA=");
	send_char(0x22);
	send_str("DEL ALL");
	send_char(0x22);
	send_char(0x0D);
	send_char(0x0A);				
}
*/ 
void hang_call()
{
	delay_ms(200);
	send_str("ATH\r\n");
	delay_ms(100);
}
void do_action()
{
	if(buff[pointer]=='B'&&buff[pointer+1]=='a'&&buff[pointer+2]=='t'&&buff[pointer+3]=='1')
	{	
			relay1=1;
			send_sms("Da Bat Relay 1\r\n");
	}
	else if(buff[pointer]=='T'&&buff[pointer+1]=='a'&&buff[pointer+2]=='t'&&buff[pointer+3]=='1')	
	{	
			relay1=0;
			send_sms("Da Tat Relay 1\r\n");
	}	
	else if(buff[pointer]=='B'&&buff[pointer+1]=='a'&&buff[pointer+2]=='t'&&buff[pointer+3]=='2')
	{	
			relay2=1;
			send_sms("Da Bat Relay 2\r\n");
	}	
	else if(buff[pointer]=='T'&&buff[pointer+1]=='a'&&buff[pointer+2]=='t'&&buff[pointer+3]=='2')
	{
			relay2=0;
			send_sms("Da Tat Relay 2\r\n");
	}
	else send_sms("Sai Cu Phap");		
}
/* ISR routine to save responses/new message */

//void Serial_ISR() interrupt 4					// Ngat khi co RI hoac TI dc bat len

void ISR_UART(void) interrupt 4
{
	if(RI!=0)									// Co RI = 1
	{
		EA = 0;									// Khong co ngat nao duoc dap ung nua
		buff[pointer] = SBUF;					// Copy SBUF(received value) to buffer 
		pointer++;								// Tang con tro len
		status_flag = 1;						// Flag for new uart arrival 
		RI = 0;									// Set RI = 0 de nhan byte du lieu tiep theo
		EA = 1;									// Tat ca moi ngat deu dc bat lai
	}
}