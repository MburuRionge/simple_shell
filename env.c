#include "shell.h"

/**
 * env - prints the current environment
 * @zone: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int env(info_t *zone)
{
	print_str(zone->env);
	return (0);
}

/**
 * _getenv - gets the value of an environ variable
 * @zone: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *_getenv(info_t *zone, const char *name)
{
	list_t *node = zone->env;
	char *p;

	while (node)
	{
		p = is_prefix(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * my_setenv - Initialize a new environment variable,
 * or modify an existing one
 * @zone: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *  Return: Always 0
 */
int my_setenv(info_t *zone)
{
	if (zone->argc != 3)
	{
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (setenv_in_zone(zone, zone->argv[1], zone->argv[2]))
		return (0);
	return (1);
}

/**
 * my_unsetenv - Remove an environment variable
 * @zone: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *  Return: Always 0
 */
int my_unsetenv(info_t *zone)
{
	int i;

	if (zone->argc == 1)
	{
		_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= zone->argc; i++)
		unsetenv_in_zone(zone, zone->argv[i]);

	return (0);
}

/**
 * fill_env_list - fills or populates env linked list
 * @zone: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int fill_env_list(info_t *zone)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	zone->env = node;
	return (0);
}
