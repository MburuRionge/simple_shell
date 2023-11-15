#include "shell.h"

/**
 * hsh - main shell loop
 * @zone: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *zone, char **av)
{
	ssize_t i = 0;
	int loop = 0;

	while (i != -1 && loop != -2)
	{
		clear_zone(zone);
		if (interactive(zone))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		i = get_input(zone);
		if (i != -1)
		{
			set_zone(zone, av);
			loop = find_builtin(zone);
			if (loop == -1)
				find_cmd(zone);
		}
		else if (interactive(zone))
			_putchar('\n');
		free_zone(zone, 0);
	}
	write_history(zone);
	free_zone(zone, 1);
	if (!interactive(zone) && zone->status)
		exit(zone->status);
	if (loop == -2)
	{
		if (zone->err_num == -1)
			exit(zone->status);
		exit(zone->err_num);
	}
	return (loop);
}

/**
 * find_builtin - finds a builtin command
 * @zone: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *0 if builtin executed successfully,
 *1 if builtin found but not successful,
 *-2 if builtin signals exit()
 */
int find_builtin(info_t *zone)
{
	int i, loop = -1;
	builtin_table builtintbl[] = {
		{"exit", my_exit},
		{"env", env},
		{"help", my_help},
		{"history", my_history},
		{"setenv", my_setenv},
		{"unsetenv", my_unsetenv},
		{"cd", my_cd},
		{"alias", the_alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(zone->argv[0], builtintbl[i].type) == 0)
		{
			zone->line_count++;
			loop = builtintbl[i].func(zone);
			break;
		}
	return (loop);
}

/**
 * find_cmd - finds a command in PATH
 * @zone: the parameter & return info struct
 *
 * Return: void
 */
void find_cmd(info_t *zone)
{
	char *path = NULL;
	int i, p;

	zone->path = zone->argv[0];
	if (zone->linecount_flag == 1)
	{
		zone->line_count++;
		zone->linecount_flag = 0;
	}
	for (i = 0, p = 0; zone->arg[i]; i++)
		if (!is_delim(zone->arg[i], " \t\n"))
			p++;
	if (!p)
		return;

	path = find_path(zone, _getenv(zone, "PATH="), zone->argv[0]);
	if (path)
	{
		zone->path = path;
		fork_cmd(zone);
	}
	else
	{
		if ((interactive(zone) || _getenv(zone, "PATH=")
			|| zone->argv[0][0] == '/') && is_cmd(zone, zone->argv[0]))
			fork_cmd(zone);
		else if (*(zone->arg) != '\n')
		{
			zone->status = 127;
			print_error(zone, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @zone: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmd(info_t *zone)
{
	pid_t child;

	child = fork();
	if (child == -1)
	{
		perror("Error:");
		return;
	}
	if (child == 0)
	{
		if (execve(zone->path, zone->argv, get_zone_environ(zone)) == -1)
		{
			free_zone(zone, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(zone->status));
		if (WIFEXITED(zone->status))
		{
			zone->status = WEXITSTATUS(zone->status);
			if (zone->status == 126)
				print_error(zone, "Permission denied\n");
		}
	}
}
