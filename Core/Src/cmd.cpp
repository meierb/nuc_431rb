/* -------------------------------------------------------------
 *
 *  file:        command.cpp
 *
 *  description: command line interpreter for Chip/Wafer tester
 *
 *  author:      Beat Meier
 *  modified:    12.09.2021
 *
 *  rev:
 *
 * -------------------------------------------------------------
 */

#include "cmd.h"

#include "cmd_nucleo.h"


void cmdHelp()
{
	 con.PutStr("\r\n");
	 con.PutStr("+-- control commands -------------------+\r\n");
	 con.PutStr("| help               command help       |\r\n");
	 con.PutStr("| exit               exit commander     |\r\n");
	 con.PutStr("+---------------------------------------+\r\n");
}


CMD_PROC(h)
{
	cmdHelp();
}


void cmd()
{
#undef  CMD_REG
#define CMD_REG(name,parameter,helptext) cmd_intp.AddCommand(#name, cmd_##name, parameter, helptext);
#undef HELP_CAT
#define HELP_CAT(name) cmd_intp.AddHelpCategory(name);

#include "cmd_nucleo.h"

	CMD_REG(h, "", "simple help");

	cmdHelp();

	// command loop
	cmd_intp.run();
	cmd_intp.ClearAllCommands();
}
