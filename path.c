#include "shell.h"

/**
 * is_cmd - determines if a file is an executable command
 * @zone: the info struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_cmd(my_info *zone, char *path)
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
 * @pathstring: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *dup_char(char *pathstring, int start, int stop)
{
	static char buffer[1024];
	int i = 0, g = 0;

	for (g = 0, i = start; i < stop; i++)
		if (pathstring[i] != ':')
			buffer[g++] = pathstring[i];
	buffer[g] = 0;
	return (buffer);
}

/**
 * find_path - finds this cmd in the PATH string
 * @zone: the info struct
 * @pathstring: the PATH string
 * @cmd: the cmd to find
 * Return: full path of cmd if found or NULL
 */
char *find_path(my_info *zone, char *pathstring, char *cmd)
{
	int i = 0, pl = 0;
	char *path;

	if (!pathstring)
		return (NULL);
	if ((_strlen(cmd) > 2) && is_prefix(cmd, "./"))
	{
		if (is_cmd(zone, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstring[i] || pathstring[i] == ':')
		{
			path = dup_char(pathstring, pl, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (is_cmd(zone, path))
				return (path);
			if (!pathstring[i])
				break;
			pl = i;
		}
		i++;
	}
	return (NULL);
}
