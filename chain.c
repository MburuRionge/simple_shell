#include "shell.h"

/**
 * chain_related - test if current char in buffer is a chain delimeter
 * @zone: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int chain_related(info_t *zone, char *buf, size_t *p)
{
	size_t m = *p;

	if (buf[m] == '|' && buf[m + 1] == '|')
	{
		buf[m] = 0;
		m++;
		zone->cmd_buf_type = CMD_OR;
	}
	else if (buf[m] == '&' && buf[m + 1] == '&')
	{
		buf[m] = 0;
		m++;
		zone->cmd_buf_type = CMD_AND;
	}
	else if (buf[m] == ';')
	{
		buf[m] = 0;
		zone->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = m;
	return (1);
}

/**
 * chain_check_zone - checks we should continue chaining based on last status
 * @zone: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void chain_check_zone(info_t *zone, char *buf, size_t *p, size_t i, size_t len)
{
	size_t m = *p;

	if (zone->cmd_buf_type == CMD_AND)
	{
		if (zone->status)
		{
			buf[i] = 0;
			m = len;
		}
	}
	if (zone->cmd_buf_type == CMD_OR)
	{
		if (!zone->status)
		{
			buf[i] = 0;
			m = len;
		}
	}

	*p = m;
}

/**
 * processData - replaces an aliases in the tokenized string
 * @zone: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int processData(info_t *zone)
{
	int i;
	list_t *node;
	char *l;

	for (i = 0; i < 10; i++)
	{
		node = node_begin_at(zone->alias, zone->argv[0], '=');
		if (!node)
			return (0);
		free(zone->argv[0]);
		l = _strchr(node->str, '=');
		if (!l)
			return (0);
		l = _strdup(l + 1);
		if (!l)
			return (0);
		zone->argv[0] = l;
	}
	return (1);
}

/**
 * replace_vars - replaces vars in the tokenized string
 * @zone: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(info_t *zone)
{
	int i = 0;
	list_t *node;

	for (i = 0; zone->argv[i]; i++)
	{
		if (zone->argv[i][0] != '$' || !zone->argv[i][1])
			continue;

		if (!_strcmp(zone->argv[i], "$?"))
		{
			string_replace(&(zone->argv[i]),
				_strdup(convert_number(zone->status, 10, 0)));
			continue;
		}
		if (!_strcmp(zone->argv[i], "$$"))
		{
			string_replace(&(zone->argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_begin_at(zone->env, &zone->argv[i][1], '=');
		if (node)
		{
			string_replace(&(zone->argv[i]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		string_replace(&zone->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * string_replace - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int string_replace(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
