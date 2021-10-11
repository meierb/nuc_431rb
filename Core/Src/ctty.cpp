// ctty.cpp


#include <ctty.h>
#include <stdio.h>
#include <cstdarg>

using namespace std;



#define ESC_CODE_TAB_LENGTH 22
const uint32_t esc_code[ESC_CODE_TAB_LENGTH] =
{
    0x00005B41, //  0: CSI A   up
    0x00005B42, //  1: CSI B   down
    0x00005B43, //  2: CSI C   right
    0x00005B44, //  3: CSI D   left
    0x005B317E, //  4: CSI 1~  home
    0x005B327E, //  5: CSI 2~  insert
    0x005B337E, //  6: CSI 3~  delete
    0x005B347E, //  7: CSI 4~  end
    0x005B357E, //  8: CSI 5~  pgup
    0x005B367E, //  9: CSI 6~  pgdn
    0x5B31317E, // 10: CSI 11~ F1
    0x5B31327E, // 11: CSI 12~ F2
    0x5B31337E, // 12: CSI 13~ F3
    0x5B31347E, // 13: CSI 14~ F4
    0x5B31357E, // 14: CSI 15~ F5
    0x5B31377E, // 15: CSI 17~ F6
    0x5B31087E, // 16: CSI 18~ F7
    0x5B31397E, // 17: CSI 19~ F8
    0x5B32307E, // 18: CSI 20~ F9
    0x5B32317E, // 19: CSI 21~ F10
    0x5B32337E, // 20: CSI 23~ F11
    0x5B32347E  // 21: CSI 24~ F12
};


uint16_t CTTY::FindEscSeq(uint32_t seq)
{
	for (int i=0; i < ESC_CODE_TAB_LENGTH; i++)
		if (seq == esc_code[i]) return i+0x0100;
	return 0;
}


uint16_t CTTY::GetChar()
{
	uint32_t seq;
	uint16_t ch;

	while (true)
	{
		ch = con.GetChar();
		if (ch != 0x1B) return ch;

		ch = con.GetChar();
		if (ch != '[') continue;

		seq = 0x00005B00 + con.GetChar();
		ch = FindEscSeq(seq);
		if (ch) return ch;

		seq = (seq << 8) + con.GetChar();
		ch = FindEscSeq(seq);
		if (ch) return ch;

		seq = (seq << 8) + con.GetChar();
		ch = FindEscSeq(seq);
		if (ch) return ch;
	}
	return 0;
}


void CTTY::CmdClear()
{
	len = 0;
	pos = 0;
	s[0] = 0;
}


bool CTTY::CmdOverwrite(char ch)
{
	if (pos >= maxLen) return false;
	s[pos++] = ch;
	if (pos > len) s[++len] = 0;
	PutChar(ch);
	return true;
}


bool CTTY::CmdInsert(char ch)
{
	if (len >= maxLen) return false;
	len++;
	for (CMDPOS i = len; i > pos; i--) s[i] = s[i-1];
	s[pos++] = ch;
	PutStr(CSI "@");
	PutChar(ch);
	return true;
}


bool CTTY::CmdDelete()
{
	if (pos == 0) return false;
	pos--;
	for (CMDPOS i = pos; i < len; i++) s[i] = s[i+1];
	len--;
	PutStr("\x7F" CSI "P");
	return true;
}


bool CTTY::CmdLeft()
{
	if (pos == 0) return false;
	pos--;
	PutStr(CSI "D");
	return true;
}


bool CTTY::CmdRight()
{
	if (pos >= len) return false;
	pos++;
	PutStr(CSI "C");
	return true;
}


void CTTY::PutInt(uint16_t x)
{
	int i = 0;
	char s[5];
	do
	{
		s[i++] = (x % 10) + '0'; x /= 10;
	} while (x != 0);

	while (i>0) { PutChar(s[--i]); }
}


void CTTY::GetStr(char *str, CMDPOS maxStrLen)
{
	s = str;
	maxLen = maxStrLen;

	CmdClear();
	uint16_t ch;

	while (true)
	{
		ch = GetChar();

		switch (ch)
		{
			case KBD_BS: // Backspace
				CmdDelete();
				break;

			case KBD_LEFT:
				CmdLeft();
				break;

			case KBD_RIGHT:
				CmdRight();
				break;

			case KBD_INSERT:
				insmode = !insmode;
				break;

			case KBD_ENTER: // Enter
//				PutStr("\r\n");
				return;

			default:
				if (ch >= ' ' && ch <= 126)	CmdAdd(ch);
		}
	}
	
	s[0] = 0;
}


void CTTY::Printf(const char *format, ...)
{
	char s[80];
	va_list argptr;
    va_start(argptr, format);
    vsnprintf(s, 80, format, argptr);
    con.PutStr(s);
    va_end(argptr);
}


void CTTY::CurXY(uint16_t x, uint16_t y)
{
	PutStr(CSI);
	PutInt(y);
	PutChar(';');
	PutInt(x);
	PutChar('H');
}


void CTTY::TextColor(uint16_t fg, uint16_t bg)
{
	PutStr(CSI);
	PutInt(bg+10);
	PutChar(';');
	PutInt(fg);
	PutChar('m');
}
