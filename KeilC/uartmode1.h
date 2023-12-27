#ifndef _UARTMODE1_H_
#define _UARTMODE1_H_
#include <intrins.h>
#include <regx52.h>
										  
// Cho phep ngat toan cuc va ngat cuc bo
void Ext_int_Init();
// Khoi tao uart mode 1
void uart_Init();
// Ghi 1 ki tu ra uart
void send_char(unsigned char chr);
// Ghi 1 chuoi ra uart
void send_str(unsigned char *str);

#endif
