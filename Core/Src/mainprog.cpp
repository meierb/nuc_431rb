/*
*******************************************************************************
* @file           : mainprog.cpp
* @brief          : Main program body
******************************************************************************
*
******************************************************************************
*/

#include "ctty.h"
#include "iobase.h"
#include "main.h"
#include "cmd.h"

using namespace std;

IoBase io;
CTTY con(io);

#define CSI "\x1B["


int main(void)
{
	main_init();

	while (true)
	{
		con.ScrClear();
		con.CurXY(1,1);
//		con.SetAttribute(ATTR_FG_BLUE);
//		con.SetAttribute(ATTR_BOLD);
//		con.PutStr(CSI "104;93m");
//		con.TextBg(ATTR_BLUE_BOLD);
//		con.TextFg(ATTR_YELLOW_BOLD);
		con.TextColor(ATTR_YELLOW_BOLD, ATTR_BLUE_BOLD);
		con.PutStr("NUCLEO-G431RB ready");
		con.TextColor(ATTR_WHITE_BOLD);

//		con.PutStr(CSI "40;97m");

		cmd();
	}

	return 0;
}
