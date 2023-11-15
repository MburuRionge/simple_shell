#include "shell.h"

/**
 * my_exit - exits the shell
 * @zone: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: exits with a given exit status
 * (0) if info.argv[0] != "exit"
 */
int my_exit(info_t *zone)
{
	int exitcheck;

	if (zone->argv[1])
	{
		exitcheck = err_int(zone->argv[1]);
		if (exitcheck == -1)
		{
			zone->status = 2;
			print_error(zone, "Illegal number: ");
			_eputs(zone->argv[1]);
			_eputchar('\n');
			return (1);
		}
		zone->err_num = err_int(zone->argv[1]);
		return (-2);
	}
	zone->err_num = -1;
	return (-2);
}

/**
 * my_cd - changes the current directory of the process
 * @zone: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *  Return: Always 0
 */
int my_cd(info_t *zone)
{
	char *ph, *dir, buffer[1024];
	int so;

	ph = getcwd(buffer, 1024);
	if (!ph)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!zone->argv[1])
	{
		dir = _getenv(zone, "HOME=");
		if (!dir)
			so = chdir((dir = _getenv(zone, "PWD=")) ? dir : "/");
		else
			so = chdir(dir);
	}
	else if (_strcmp(zone->argv[1], "-") == 0)
	{
		if (!_getenv(zone, "OLDPWD="))
		{
			_puts(ph);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(zone, "OLDPWD=")), _putchar('\n');
		so = chdir((dir = _getenv(zone, "OLDPWD=")) ? dir : "/");
	}
	else
		so = chdir(zone->argv[1]);
	if (so == -1)
	{
		print_error(zone, "can't cd to ");
		_eputs(zone->argv[1]), _eputchar('\n');
	}
	else
	{
		setenv_in_zone(zone, "OLDPWD", _getenv(zone, "PWD="));
		setenv_in_zone(zone, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * my_help - changes the current directory of the process
 * @zone: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *  Return: Always 0
 */
int my_help(info_t *zone)
{
	char **arg_array;

	arg_array = zone->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array);
	return (0);
}
