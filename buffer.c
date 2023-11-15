#include "shell.h"

/**
 * input_buffer - buffers chained commands
 * @zone: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buffer(info_t *zone, char **buf, size_t *len)
{
	ssize_t rd = 0;
	size_t size = 0;

	if (!*len)
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		rd = getline(buf, &size, stdin);
#else
		rd = _getline(zone, buf, &size);
#endif
		if (rd > 0)
		{
			if ((*buf)[rd - 1] == '\n')
			{
				(*buf)[rd - 1] = '\0';
				rd--;
			}
			zone->linecount_flag = 1;
			remove_comments(*buf);
			history_list(zone, *buf, zone->history_count++);
			{
				*len = rd;
				zone->cmd_buf = buf;
			}
		}
	}
	return (rd);
}

/**
 * get_input - gets a line minus the newline
 * @zone: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *zone)
{
	static char *buf;
	static size_t i, j, len;
	ssize_t rd = 0;
	char **bp = &(zone->arg), *p;

	_putchar(BUF_FLUSH);
	rd = input_buffer(zone, &buf, &len);
	if (rd == -1)
		return (-1);
	if (len)
	{
		j = i;
		p = buf + i;

		chain_check_zone(zone, buf, &j, i, len);
		while (j < len)
		{
			if (chain_related(zone, buf, &j))
				break;
			j++;
		}

		i = j + 1;
		if (i >= len)
		{
			i = len = 0;
			zone->cmd_buf_type = CMD_NORM;
		}

		*bp = p;
		return (_strlen(p));
	}

	*bp = buf;
	return (rd);
}

/**
 * read_buffer - reads a buffer
 * @zone: parameter struct
 * @buf: buffer
 * @i: size
 * Return: rd
 */
ssize_t read_buffer(info_t *zone, char *buf, size_t *i)
{
	ssize_t rd = 0;

	if (*i)
		return (0);
	rd = read(zone->readfiledescriptor, buf, READ_BUF_SIZE);
	if (rd >= 0)
		*i = rd;
	return (rd);
}

/**
 * _getline - gets the next line of input from STDIN
 * @zone: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _getline(info_t *zone, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t m;
	ssize_t r = 0, s = 0;
	char *p = NULL, *pp = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buffer(zone, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	m = c ? 1 + (unsigned int)(c - buf) : len;
	pp = _realloc(p, s, s ? s + m : m + 1);
	if (!pp)
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(pp, buf + i, m - i);
	else
		_strncpy(pp, buf + i, m - i + 1);

	s += m - i;
	i = m;
	p = pp;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
