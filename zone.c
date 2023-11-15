#include "shell.h"

/**
 * clear_zone - initializes info_t struct
 * @zone: struct address
 */
void clear_zone(info_t *zone)
{
	zone->arg = NULL;
	zone->argv = NULL;
	zone->path = NULL;
	zone->argc = 0;
}

/**
 * set_zone - initializes info_t struct
 * @zone: struct address
 * @av: argument vector
 */
void set_zone(info_t *zone, char **av)
{
	int i = 0;

	zone->file_name = av[0];
	if (zone->arg)
	{
		zone->argv = strtow(zone->arg, " \t");
		if (!zone->argv)
		{

			zone->argv = malloc(sizeof(char *) * 2);
			if (zone->argv)
			{
				zone->argv[0] = _strdup(zone->arg);
				zone->argv[1] = NULL;
			}
		}
		for (i = 0; zone->argv && zone->argv[i]; i++)
			;
		zone->argc = i;

		processData(zone);
		replace_vars(zone);
	}
}

/**
 * free_zone - frees info_t struct fields
 * @zone: struct address
 * @all: true if freeing all fields
 */
void free_zone(info_t *zone, int all)
{
	free_ptr(zone->argv);
	zone->argv = NULL;
	zone->path = NULL;
	if (all)
	{
		if (!zone->cmd_buf)
			free(zone->arg);
		if (zone->env)
			free_list(&(zone->env));
		if (zone->history)
			free_list(&(zone->history));
		if (zone->alias)
			free_list(&(zone->alias));
		free_ptr(zone->environ);
			zone->environ = NULL;
		bfree((void **)zone->cmd_buf);
		if (zone->readfiledescriptor > 2)
			close(zone->readfiledescriptor);
		_putchar(BUF_FLUSH);
	}
}
