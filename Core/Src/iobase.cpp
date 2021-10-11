// iobase.cpp


#include <cstring>
#include "iobase.h"
#include "stm32g4xx_hal.h"
#include "main.h"


using namespace std;


char IoBase::GetChar()
{
	uint8_t ch;
	HAL_UART_Receive(&hlpuart1, &ch, 1, -1);
	return ch;
}


void IoBase::PutChar(char ch)
{
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)&ch, 1, 0xffff);
}


void IoBase::PutStr(const char *s)
{
	uint16_t n = std::strlen(s);
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)s, n, 0xffff);
}
