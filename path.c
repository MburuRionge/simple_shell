#include "shell.h"

/**
 * is_cmd - determines if a file is an executable command
 * @zone: the info struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_cmd(info_t *zone, char *path)
{
	struct stat st;

	(void)zone;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * dup_char - duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *dup_char(char *pathstr, int start, int stop)
{
	static char buffer[1024];
	int i = 0, g = 0;

	for (g = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buffer[g++] = pathstr[i];
	buffer[g] = 0;
	return (buffer);
}

/**
 * find_path - finds this cmd in the PATH string
 * @zone: the info struct
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 * Return: full path of cmd if found or NULL
 */
char *find_path(info_t *zone, char *pathstr, char *cmd)
{
	int i = 0, pl = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && is_prefix(cmd, "./"))
	{
		if (is_cmd(zone, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dup_char(pathstr, pl, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (is_cmd(zone, path))
				return (path);
			if (!pathstr[i])
				break;
			pl = i;
		}
		i++;
	}
	return (NULL);
}
