// iobase.h

#ifndef IOBASE_H
#define IOBASE_H

class IoBase
{

public:
	char GetChar();
	void PutChar(char ch);
	void PutStr(const char *s);
};


#endif /* IOBASE_H */
