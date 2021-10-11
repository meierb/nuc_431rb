/* -------------------------------------------------------------
 *
 *  file:        cmd_nucleo.h
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
 
 HELP_CAT("nuc");
 
CMD_REG(test, "", "First Command")
CMD_REG(info, "<n>", "Command with parameter <n>")
CMD_REG(list, "<range>", "list range")
CMD_REG(pon, "", "LED on")
CMD_REG(poff, "", "LED off")
CMD_REG(led, "<br>", "Set LED brightness (0..100)")
CMD_REG(ttytest, "", "Test TTY Screen Commands")
CMD_REG(menu, "", "Menu")
