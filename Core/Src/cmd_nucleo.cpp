/* -------------------------------------------------------------
 *
 *  file:        cmd_nucleo.cpp
 *
 *  description: command line interpreter
 *               nucleo-G431RB functions
 *
 *  author:      Beat Meier
 *  modified:    12.09.2021
 *
 *  rev:
 *
 * -------------------------------------------------------------
 */
 
#include "cmd.h"
#include "mainprog.h"


CMD_PROC(test)
{
	con.PutStr("Dies ist ein Test.\r\n");
}

CMD_PROC(info)
{
	int n;
	PAR_INT(n, 0, 1000)

	con.Printf("Info = %i\r\n", n);
}

CMD_PROC(list)
{
	int a, b;
	PAR_RANGE(a, b, 0, 20)

	for (int i=a; i<=b; i++) con.Printf("%2i\r\n", i);
}

CMD_PROC(pon)
{
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 100);
}

CMD_PROC(poff)
{
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
}

CMD_PROC(led)
{
	int n;
	PAR_INT(n, 0, 100)
	uint32_t pulse_width = 3400*n;
	if (pulse_width > 340000) pulse_width = 340000;
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse_width);
}


CMD_PROC(ttytest)
{
	uint16_t ch;
	con.ScrSave();
	while ((ch = con.GetChar()) != KBD_F1)
	{
		switch (ch)
		{
		case KBD_LEFT:
			con.CurLeft();
			break;
		case KBD_RIGHT:
			con.CurRight();
			break;
		case KBD_UP:
			con.CurUp();
			break;
		case KBD_DOWN:
			con.CurDown();
			break;
		case KBD_F2:
			con.CurXY(5,5);
			break;
		case KBD_F3:
			con.CurXY(10,5);
			break;
		case KBD_F4:
			con.CurXY(5,10);
			break;
		case KBD_F5:
			con.CurXY(10,10);
			break;

		default:
			if (' ' <= ch && ch < 126) con.PutChar(ch);
		}
	}
	con.ScrRestore();
	con.PutStr("\r\n");
}


CMD_PROC(menu)
{
	char inp[6];

	con.ScrSave();

	con.TextColor(ATTR_WHITE);
	con.CurXY(3, 4); con.PutStr("0");
	con.CurXY(3, 5); con.PutStr("1");
	con.CurXY(3, 6); con.PutStr("2");
	con.CurXY(3, 7); con.PutStr("3");
	con.CurXY(3, 8); con.PutStr("4");
	con.CurXY(3, 9); con.PutStr("5");
	con.CurXY(3,10); con.PutStr("6");
	con.CurXY(3,11); con.PutStr("7");
	con.CurXY(3,12); con.PutStr("8");
	con.CurXY(3,13); con.PutStr("9");
	con.CurXY(3,14); con.PutStr("X");

	con.TextColor(ATTR_CYAN_BOLD);
	con.CurXY(6, 4); con.PutStr("Defaults   -");
	con.CurXY(6, 5); con.PutStr("View       -");
	con.CurXY(6, 6); con.PutStr("Edit       -");
	con.CurXY(6, 7); con.PutStr("Utility    -");
	con.CurXY(6, 8); con.PutStr("Assembler  -");
	con.CurXY(6, 9); con.PutStr("User       -");
	con.CurXY(6,10); con.PutStr("TSO        -");
	con.CurXY(6,11); con.PutStr("Tutorial   -");
	con.CurXY(6,12); con.PutStr("Test       -");
	con.CurXY(6,13); con.PutStr("Operator   -");
	con.CurXY(6,14); con.PutStr("Exit       -");

	con.TextColor(ATTR_GREEN_BOLD);
	con.CurXY(20, 4); con.PutStr("Alter / Display session defaults");
	con.CurXY(20, 5); con.PutStr("Display source data or output listings");
	con.CurXY(20, 6); con.PutStr("Update / Create a member or dataset");
	con.CurXY(20, 7); con.PutStr("Enter UTILITY");
	con.CurXY(20, 8); con.PutStr("Foreground ASSEMBLER and LINK");
	con.CurXY(20, 9); con.PutStr("Execute RPF user routine");
	con.CurXY(20,10); con.PutStr("Execute TSO commands");
	con.CurXY(20,11); con.PutStr("Display HELP information");
	con.CurXY(20,12); con.PutStr("Enter TEST mode (Authorized)");
	con.CurXY(20,13); con.PutStr("Enter OPERATOR mode");
	con.CurXY(20,14); con.PutStr("Terminate RPF");

	con.TextColor(ATTR_WHITE);
	con.CurBlink(true);
	while (true)
	{
		con.CurXY(4,16); con.PutStr("Eingabe: ");
		con.GetStr(inp, 5);
		if (inp[0] == 'e') break;
	}

	con.ScrRestore();
	con.PutStr("\r\n");
}
