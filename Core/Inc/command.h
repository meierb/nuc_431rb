// command.h: Schnittstelle fuer die Klasse CInterpreter.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <string.h>
#include <list>
#include "htable.h"
#include "mainprog.h"


class CInterpreter;


#define CMD_LINE_LENGTH 64
#define MAXSYMLEN  10

struct CSymbol { char name[MAXSYMLEN+1]; int value; };


class CCmdLine
{
	char cmdLine[CMD_LINE_LENGTH];
	char *cmd, *par;
	bool read();
	bool isCmd(const char name[]) { return strcmp(cmd,name) == 0; }

	static const CSymbol symtable[];

	static bool isAlpha(char ch)
	{ return ('A'<=ch && ch<='Z') || ('a'<=ch && ch<='z'); }
	static bool isNumber(char ch) { return '0'<=ch && ch<='9'; }
	static bool isAlphaNum(char ch) { return isAlpha(ch) || isNumber(ch); }
	static bool isWhitespace(char ch) { return ch==' ' || ch=='\t'; }
	static int GetHex(char ch);
	bool getSymbol(int &value);
	bool getNumber(int &value);
public:
	const char* getName() { return cmd; }
	bool getInt(int &value, int min, int max);
	bool getIntRange(int &valuemin, int &valuemax, int skipmin, int skipmax);
	bool getString(char *value, int size);
	bool getStringEOL(char *value, int size);
	friend class CInterpreter;
};


typedef void(*CMDFUNCTION)(CCmdLine &);

class CCommand
{
	const char *m_name;
	const char *m_parameter;
	const char *m_help;
	CMDFUNCTION m_exec;
public:
	friend class CInterpreter;
};


class CHelpCategory
{
	const char *m_name;
	std::list<CCommand> helpList;
public:
	friend class CInterpreter;
};


class CInterpreter
{
	CHelpCategory *currentHelpCat;
	std::list<CHelpCategory> helpCategory;
	CHashTable<CCommand> cmdList;
	CCmdLine cmdline;
	char scriptPath[256];
	void ListHelpCategories();
	void ListHelpText(std::list<CHelpCategory>::iterator cat);
	void help();
public:
	CInterpreter();
	~CInterpreter() {};
	void SetScriptPath(const char path[]);
	void AddHelpCategory(const char name[]);
	void AddCommand(const char name[], CMDFUNCTION f, const char parameter[], const char help[]);
	void ClearAllCommands();
	bool run();
};


bool cmd_not_implemented(CCmdLine &par);

extern CInterpreter cmd_intp;

#define HELP_CAT(name)
#define CMD_REG(name,parameter,helptext) void cmd_##name(CCmdLine &par);
#define CMD_PROC(name) void cmd_##name(CCmdLine &par)

#define CMD_NUL(name, help) cmd_intp.AddCommand(#name, cmd_not_implemented, help)

#define PAR_INT(var,min,max) if (!par.getInt(var,(min),(max))) \
{ con.Printf("illegal integer parameter!\r\n"); return; }
#define PAR_IS_INT(var,min,max) par.getInt(var,(min),(max))

#define PAR_RANGE(varmin,varmax,min,max) if (!par.getIntRange(varmin,varmax,(min),(max))) \
{ con.Printf("illegal range parameter!\r\n"); return; }

#define PAR_STRING(var,size) if (!par.getString(var,size)) \
{ con.Printf("illegal string parameter!\r\n"); return; }

#define PAR_IS_STRING(var,size) (par.getString(var,size))

#define PAR_STRINGEOL(var,size) if (!par.getStringEOL(var,size)) \
{ con.Printf("illegal string parameter!\r\n"); return; }

