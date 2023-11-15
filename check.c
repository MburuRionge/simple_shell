#include "shell.h"

/**
 * interactive - returns true if shell is interactive mode
 * @zone: struct address
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int interactive(info_t *zone)
{
	return (isatty(STDIN_FILENO) && zone->readfiledescriptor <= 2);
}

/**
 * is_delim - checks if character is a delimeter
 * @c: the char to check
 * @delim: the delimeter string
 * Return: 1 if true, 0 if false
 */
int is_delim(char c, char *delim)
{
	while (*delim)
		if (*delim++ == c)
			return (1);
	return (0);
}

/**
 *is_alpha - checks for alphabetic character
 *@c: The character to input
 *Return: 1 if c is alphabetic, 0 otherwise
 */

int is_alpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 *convert_to_int - converts a string to an integer
 *@string: the string to be converted
 *Return: 0 if no numbers in string, converted number otherwise
 */

int convert_to_int(char *string)
{
	int i, sign = 1, flag = 0, placer;
	unsigned int my = 0;

	for (i = 0;  string[i] != '\0' && flag != 2; i++)
	{
		if (string[i] == '-')
			sign *= -1;

		if (string[i] >= '0' && string[i] <= '9')
		{
			flag = 1;
			my *= 10;
			my += (string[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		placer = -my;
	else
		placer = my;

	return (placer);
}
