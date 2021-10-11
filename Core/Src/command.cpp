// command.cpp

#include "command.h"
#include <string>



CInterpreter cmd_intp;


// === CCmdLine ==========================================================

bool CCmdLine::read()
{
	con.PutChar('>');
	con.GetStr(cmdLine, CMD_LINE_LENGTH-1);
	con.PutStr("\r\n");

	cmd = cmdLine;
	while (isWhitespace(cmd[0])) cmd++;

	par = cmd;
	while (isAlphaNum(*par)) par++;
	if (par[0] != 0) { par[0] = 0; par++; }

	return true;
}


int CCmdLine::GetHex(char ch)
{
	if (isNumber(ch)) return ch - '0';
	if ('A'<=ch && ch<='F') return ch - 'A' + 10;
	if ('a'<=ch && ch<='f') return ch - 'a' + 10;
	return -1;
}


const CSymbol CCmdLine::symtable[] =
{
	{ "vdig",		0x01 },
	{ "vana",		0x02 },
	{ "vsh",		0x03 },
	{ "vcomp",		0x04 },
	{ "leak",		0x05 },
	{ "rgpr",		0x06 },
	{ "wllpr",		0x07 },
	{ "rgsh",		0x08 },
	{ "wllsh",		0x09 },
	{ "hlddel",		0x0A },
	{ "trim",		0x0B },
	{ "",			0    }
};


bool CCmdLine::getSymbol(int &value)
{
	int n = 0;
	char s[MAXSYMLEN+1];

	while (n<MAXSYMLEN && (isAlphaNum(*par) || (*par == '_')))
	{
		s[n++] = par[0];
		par++;
	}
	s[n] = 0;

	if (n == 0) return false;

	int i = 0;
	while (symtable[i].name[0])
	{
		if (strcmp(s,symtable[i].name) == 0)
		{
			value = symtable[i].value;
			return true;
		}
		i++;
	}

	return true;
}


bool CCmdLine::getNumber(int &value)
{
	int i, x;
	int base;
	int cnt;

	bool neg = false;
	if (par[0] == '-') { neg = true; par++; }
	else if (par[0] == '+') par++;

	switch (par[0])
	{
		case 'b':
		case 'B': base =  2; cnt = 31; par++; break;
		case '$': base = 16; cnt =  7; par++; break;
		default:  base = 10; cnt =  9;
	}

	value = GetHex(par[0]);
	if (value < 0 || base <= value) return false;
	par++;

	for (i=0; i<cnt; i++)
	{
		while (par[0] == '_') par++;
		x = GetHex(par[0]);
		if (0 <= x && x < base) value = value*base + x;
		else break;
		par++;
	}

	if (neg) value = -value;

	x = GetHex(par[0]);
	if (0 <= x && x < base) return false;

	return true;
}


bool CCmdLine::getInt(int &value, int min, int max)
{
	int v;

	while (isWhitespace(par[0])) par++;

	if (par[0] == '#') { par++; if (!getSymbol(v)) return false;	}
	else if (!getNumber(v)) return false;

	if (v < min || max < v) return false;

	value = v;
	return true;
}


bool CCmdLine::getIntRange(int &valuemin, int &valuemax, int skipmin, int skipmax)
{  // nn:nn

	int vmin, vmax;

	// scan first number
	while (isWhitespace(par[0])) par++;
	if (par[0] != ':')
	{
		if (!getInt(vmin, -10000, 10000)) return false;
	}
	else vmin = skipmin;

	// scan for ":"
	if (par[0] == ':')
	{ // scan second number
		par++;
		if (isWhitespace(par[0]) || (par[0] == 0)) vmax = skipmax;
		else
		{
			if (!getInt(vmax, -10000, 10000)) return false;
		}

	}
	else vmax = vmin;

	// check range
	if (vmin>vmax) return false;

	if (vmax<skipmin || vmin>skipmax) return false;

	valuemin = (vmin<skipmin)? skipmin : vmin;
	valuemax = (vmax>skipmax)? skipmax : vmax;
	return true;
}


bool CCmdLine::getString(char *value, int size)
{
	while (isWhitespace(par[0])) par++;

	int i=0;
	while ((i < (size-1)) && !isWhitespace(par[0]) && (par[0] != 0))
	{
		value[i] = par[0];
		par++;
		i++;
	}
	value[i] = 0;
	return i > 0;
}


bool CCmdLine::getStringEOL(char *value, int size)
{
	while (isWhitespace(par[0])) par++;

	int i=0;
	while ((i < (size-1)) && (par[0] != 0))
	{
		value[i] = par[0];
		par++;
		i++;
	}
	value[i] = 0;
	return i > 0;
}



// === CInterpreter ======================================================

CInterpreter::CInterpreter()
{
	currentHelpCat = 0;
	memset(scriptPath, 0, 256);
}


void CInterpreter::ListHelpCategories()
{
	con.Printf(" help [<category>]              list commands in category\r\n");
	con.Printf(" exit                           exit commander\r\n");
	con.Printf("categories:\r\n");
	std::list<CHelpCategory>::iterator cat;
	for (cat = helpCategory.begin(); cat != helpCategory.end(); cat++)
	{
		con.Printf("  %s\r\n", cat->m_name);
	}
}


void CInterpreter::ListHelpText(std::list<CHelpCategory>::iterator cat)
{
	std::list<CCommand>::iterator i;
	for (i = cat->helpList.begin(); i != cat->helpList.end(); i++)
	{
		std::string s = i->m_name;
		s += " ";
		s += i->m_parameter;
		con.Printf(" %-30s %s\r\n", s.c_str() , i->m_help);
	}
}


void CInterpreter::help()
{
	char catName[80];
	if (cmdline.getString(catName,78))
	{
		std::list<CHelpCategory>::iterator cat;
		for (cat = helpCategory.begin(); cat != helpCategory.end(); cat++)
		{
			if (strcmp(catName, cat->m_name) == 0)
			{
				ListHelpText(cat);
				return;
			}
		}
	}
	ListHelpCategories();
}


void CInterpreter::SetScriptPath(const char path[])
{
	strncpy(scriptPath, path, 254);
}


void CInterpreter::AddHelpCategory(const char name[])
{
	CHelpCategory h;
	h.m_name = name;
	helpCategory.push_back(h);
	currentHelpCat = &helpCategory.back();
}


void CInterpreter::AddCommand(const char name[], CMDFUNCTION f, const char parameter[], const char help[])
{
	CCommand c;
	c.m_name = name;
	c.m_parameter = parameter;
	c.m_help = help;
	c.m_exec = f;
	cmdList.Add(name, c);

	if (helpCategory.size() == 0) AddHelpCategory("");
	currentHelpCat->helpList.push_back(c);
}


void CInterpreter::ClearAllCommands()
{
	cmdList.DeleteAll();
	helpCategory.clear();

}


bool CInterpreter::run()
{
	while (true)
	{
		if (!cmdline.read()) break;
		if (cmdline.getName()[0] == 0) continue;   // empty line
		if (cmdline.getName()[0] == '-') continue; // comment
		CCommand *p = cmdList.Find(cmdline.getName());
		if (p) { p->m_exec(cmdline); }
		else if (cmdline.isCmd("help")) help();
		else if (cmdline.isCmd("exit")) break;
		else con.Printf("unknown command \"%s\"!\r\n", cmdline.getName());
	}
	return true;
}


bool cmd_not_implemented(CCmdLine &par)
{
	con.Printf("command not implemented!\r\n");
	return true;
}
