#include "shell.h"

/**
 * my_history - displays the history list, one command by line, preceded
 * with line numbers, starting at 0.
 * @zone: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *  Return: Always 0
 */
int my_history(info_t *zone)
{
	print_list(zone->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @zone: parameter struct
 * @str: the string alias
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(info_t *zone, char *str)
{
	char *m, dh;
	int ret;

	m = _strchr(str, '=');
	if (!m)
		return (1);
	dh = *m;
	*m = 0;
	ret = delete_node_at_index(&(zone->alias),
		get_node_index(zone->alias, node_begin_at(zone->alias, str, -1)));
	*m = dh;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @zone: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(info_t *zone, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(zone, str));

	unset_alias(zone, str);
	return (add_node_end(&(zone->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *k = NULL, *l = NULL;

	if (node)
	{
		k = _strchr(node->str, '=');
		for (l = node->str; l <= k; l++)
			_putchar(*l);
		_putchar('\'');
		_puts(k + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * the_alias - mimics the alias builtin (man alias)
 * @zone: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int the_alias(info_t *zone)
{
	int i = 0;
	char *pop = NULL;
	list_t *node = NULL;

	if (zone->argc == 1)
	{
		node = zone->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; zone->argv[i]; i++)
	{
		pop = _strchr(zone->argv[i], '=');
		if (pop)
			set_alias(zone, zone->argv[i]);
		else
			print_alias(node_begin_at(zone->alias, zone->argv[i], '='));
	}

	return (0);
}
