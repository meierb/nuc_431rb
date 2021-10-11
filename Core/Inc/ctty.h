// ctty.h

#pragma once

#include <iobase.h>
#include "main.h"


#define KBD_BS     0x007F
#define KBD_ENTER  0x000D
#define KBD_TAB    0x0009
#define KBD_UP     0x0100
#define KBD_DOWN   0x0101
#define KBD_RIGHT  0x0102
#define KBD_LEFT   0x0103
#define KBD_HOME   0x0104
#define KBD_INSERT 0x0105
#define KBD_DELETE 0x0106
#define KBD_END    0x0107
#define KBD_PGUP   0x0108
#define KBD_PGDN   0x0109
#define KBD_F1     0x010A
#define KBD_F2     0x010B
#define KBD_F3     0x010C
#define KBD_F4     0x010D
#define KBD_F5     0x010E
#define KBD_F6     0x010F
#define KBD_F7     0x0110
#define KBD_F8     0x0111
#define KBD_F9     0x0112
#define KBD_F10    0x0113
#define KBD_F11    0x0114
#define KBD_F12    0x0115

#define ATTR_BLACK   30
#define ATTR_RED     31
#define ATTR_GREEN   32
#define ATTR_YELLOW  33
#define ATTR_BLUE    34
#define ATTR_MAGENTA 35
#define ATTR_CYAN    36
#define ATTR_WHITE   37
#define ATTR_BLACK_BO     90
#define ATTR_RED_BOLD     91
#define ATTR_GREEN_BOLD   92
#define ATTR_YELLOW_BOLD  93
#define ATTR_BLUE_BOLD    94
#define ATTR_MAGENTA_BOLD 95
#define ATTR_CYAN_BOLD    96
#define ATTR_WHITE_BOLD   97

typedef uint16_t CMDPOS;


#define CSI "\x1B["

class CTTY
{
	IoBase& con;
	bool insmode;
	char *s;
	CMDPOS maxLen;
	CMDPOS len;
	CMDPOS pos;

	uint16_t FindEscSeq(uint32_t seq);

	// command line edit
	void CmdClear();
	void CmdSetInsertMode() { insmode = true; }
	void CmdSetOverwriteMode() { insmode = false; }
	bool CmdOverwrite(char ch);
	bool CmdInsert(char ch);
	bool CmdAdd(char ch) { return insmode ? CmdInsert(ch) : CmdOverwrite(ch); }
	bool CmdDelete();
	bool CmdLeft();
	bool CmdRight();


public:
	void PutInt(uint16_t x);
	CTTY(IoBase &conio) : con(conio), insmode(true), s(0) {}

	uint16_t GetChar();
	void GetStr(char *str, CMDPOS maxStrLen);

	// TTY output
	void PutChar(char ch) { con.PutChar(ch); }
	void PutStr(const char *s) { con.PutStr(s); }
	void Printf(const char *format, ...);

	void CurLeft()  { PutStr(CSI "D"); }
	void CurRight() { PutStr(CSI "C"); }
	void CurUp()    { PutStr(CSI "A"); }
	void CurDown()  { PutStr(CSI "B"); }
	void CurX(uint16_t x) { PutStr(CSI); PutInt(x); PutChar('G'); }
	void CurY(uint16_t y) { PutStr(CSI); PutInt(y); PutChar('d'); }
	void CurXY(uint16_t x, uint16_t y);
	void CurHide() { PutStr(CSI "?25l"); }
	void CurShow() { PutStr(CSI "?25h"); }
	void CurBlink(bool on) { if (on) PutStr(CSI "?12h"); else PutStr(CSI "?12l"); }
	void ScrClear() { PutStr(CSI "2J"); }
	void ScrClearLine() { PutStr(CSI "2K"); }
	void ScrUp(uint16_t n = 1);
	void ScrDown(uint16_t n = 1);
	void ScrSave()    { PutStr(CSI "?1049h"); }
	void ScrRestore() { PutStr(CSI "?1049l"); }

	void TextFg(uint16_t attr) { PutStr(CSI); PutInt(attr); PutChar('m'); }
	void TextBg(uint16_t attr) { PutStr(CSI); PutInt(attr+10); PutChar('m'); }
	void TextColor(uint16_t fg, uint16_t bg = ATTR_BLACK);
};
