#include "shell.h"

/**
 * **strtow - splits a string into words. Repeat delimiters are ignored
 * @str: the input string
 * @d: the delimeter string
 * Return: a pointer to an array of strings, or NULL on failure
 */

char **strtow(char *str, char *d)
{
	int i, j, b, n, split = 0;
	char **c;

	if (str == NULL || str[0] == 0)
		return (NULL);
	if (!d)
		d = " ";
	for (i = 0; str[i] != '\0'; i++)
		if (!is_delim(str[i], d) && (is_delim(str[i + 1], d) || !str[i + 1]))
			split++;

	if (split == 0)
		return (NULL);
	c = malloc((1 + split) * sizeof(char *));
	if (!c)
		return (NULL);
	for (i = 0, j = 0; j < split; j++)
	{
		while (is_delim(str[i], d))
			i++;
		b = 0;
		while (!is_delim(str[i + b], d) && str[i + b])
			b++;
		c[j] = malloc((b + 1) * sizeof(char));
		if (!c[j])
		{
			for (b = 0; b < j; b++)
				free(c[b]);
			free(c);
			return (NULL);
		}
		for (n = 0; n < b; n++)
			c[j][n] = str[i++];
		c[j][n] = 0;
	}
	c[j] = NULL;
	return (c);
}

/**
 * **strtow2 - splits a string into words
 * @str: the input string
 * @d: the delimeter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **strtow2(char *str, char d)
{
	int i, j, b, n, numwords = 0;
	char **c;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != d && str[i + 1] == d) ||
		    (str[i] != d && !str[i + 1]) || str[i + 1] == d)
			numwords++;
	if (numwords == 0)
		return (NULL);
	c = malloc((1 + numwords) * sizeof(char *));
	if (!c)
		return (NULL);
	for (i = 0, j = 0; j < numwords; j++)
	{
		while (str[i] == d && str[i] != d)
			i++;
		b = 0;
		while (str[i + b] != d && str[i + b] && str[i + b] != d)
			b++;
		c[j] = malloc((b + 1) * sizeof(char));
		if (!c[j])
		{
			for (b = 0; b < j; b++)
				free(c[b]);
			free(c);
			return (NULL);
		}
		for (n = 0; n < b; n++)
			c[j][n] = str[i++];
		c[j][n] = 0;
	}
	c[j] = NULL;
	return (c);
}
