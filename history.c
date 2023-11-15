#include "shell.h"

/**
 * history_file - gets the history file
 * @zone: parameter struct
 *
 * Return: allocated string containg history file
 */

char *history_file(info_t *zone)
{
	char *buffer, *dir;

	dir = _getenv(zone, "HOME=");
	if (!dir)
		return (NULL);
	buffer = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buffer)
		return (NULL);
	buffer[0] = 0;
	_strcpy(buffer, dir);
	_strcat(buffer, "/");
	_strcat(buffer, HIST_FILE);
	return (buffer);
}

/**
 * write_history - creates a file, or appends to an existing file
 * @zone: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history(info_t *zone)
{
	ssize_t fd;
	char *filename = history_file(zone);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = zone->history; node; node = node->next)
	{
		put_fd(node->str, fd);
		putfd('\n', fd);
	}
	putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - reads history from file
 * @zone: the pairameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_history(info_t *zone)
{
	int i, result = 0, linecount = 0;
	ssize_t fd, rd, filesize = 0;
	struct stat st;
	char *buf = NULL, *filename = history_file(zone);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		filesize = st.st_size;
	if (filesize < 2)
		return (0);
	buf = malloc(sizeof(char) * (filesize + 1));
	if (!buf)
		return (0);
	rd = read(fd, buf, filesize);
	buf[filesize] = 0;
	if (rd <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < filesize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			history_list(zone, buf + result, linecount++);
			result = i + 1;
		}
	if (result != i)
		history_list(zone, buf + result, linecount++);
	free(buf);
	zone->history_count = linecount;
	while (zone->history_count-- >= HIST_MAX)
		delete_node_at_index(&(zone->history), 0);
	rehistory(zone);
	return (zone->history_count);
}

/**
 * history_list - adds entry to a history linked list
 * @zone: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int history_list(info_t *zone, char *buf, int linecount)
{
	list_t *node = NULL;

	if (zone->history)
		node = zone->history;
	add_node_end(&node, buf, linecount);

	if (!zone->history)
		zone->history = node;
	return (0);
}

/**
 * rehistory - renumbers the history linked list after changes
 * @zone: Structure containing potential arguments. Used to maintain
 * Return: the new history_count
 */
int rehistory(info_t *zone)
{
	list_t *node = zone->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (zone->history_count = i);
}
