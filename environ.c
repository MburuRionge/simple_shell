#include "shell.h"

/**
 * get_zone_environ - returns the string array copy of our environ
 * @zone: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
char **get_zone_environ(info_t *zone)
{
	if (!zone->environ || zone->env_changed)
	{
		zone->environ = list_strings(zone->env);
		zone->env_changed = 0;
	}

	return (zone->environ);
}

/**
 * unsetenv_in_zone - Remove an environment variable
 * @zone: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int unsetenv_in_zone(info_t *zone, char *var)
{
	list_t *node = zone->env;
	size_t m = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = is_prefix(node->str, var);
		if (p && *p == '=')
		{
			zone->env_changed = delete_node_at_index(&(zone->env), m);
			m = 0;
			node = zone->env;
			continue;
		}
		node = node->next;
		m++;
	}
	return (zone->env_changed);
}

/**
 * setenv_in_zone - Initialize a new environment variable,
 *             or modify an existing one
 * @zone: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int setenv_in_zone(info_t *zone, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = zone->env;
	while (node)
	{
		p = is_prefix(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			zone->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(zone->env), buf, 0);
	free(buf);
	zone->env_changed = 1;
	return (0);
}
