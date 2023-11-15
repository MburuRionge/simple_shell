#include "shell.h"

/**
 * err_int - converts a string to an integer
 * @string: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 * -1 on error
 */
int err_int(char *string)
{
	int i = 0;
	unsigned long int result = 0;

	if (*string == '+')
		string++;
	for (i = 0;  string[i] != '\0'; i++)
	{
		if (string[i] >= '0' && string[i] <= '9')
		{
			result *= 10;
			result += (string[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * print_error - prints an error message
 * @zone: the parameter & return info struct
 * @error_message: string containing specified error type
 * Return: 0 if no numbers in string, converted number otherwise
 * -1 on error
 */
void print_error(info_t *zone, char *error_message)
{
	_eputs(zone->file_name);
	_eputs(": ");
	print_integer(zone->line_count, STDERR_FILENO);
	_eputs(": ");
	_eputs(zone->argv[0]);
	_eputs(": ");
	_eputs(error_message);
}

/**
 * print_integer - function prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the filedescriptor to write to
 *
 * Return: number of characters printed
 */
int print_integer(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int _abs_, hold;

	if (fd == STDERR_FILENO)
		__putchar = _eputchar;
	if (input < 0)
	{
		_abs_ = -input;
		__putchar('-');
		count++;
	}
	else
		_abs_ = input;
	hold = _abs_;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			__putchar('0' + hold / i);
			count++;
		}
		hold %= i;
	}
	__putchar('0' + hold);
	count++;

	return (count);
}

/**
 * convert_number - converts integers into strings
 * @num: number
 * @base: base
 * @flags: argument flags
 *
 * Return: string
 */
char *convert_number(long int num, int base, int flags)
{
	static char *ar;
	static char buffer[50];
	char sign = 0;
	char *str;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	ar = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	str = &buffer[49];
	*str = '\0';

	do	{
		*--str = ar[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--str = sign;
	return (str);
}

/**
 * remove_comments - function replaces first instance of '#' with '\0'
 * @buf: address of the string to modify
 *
 * Return: Always 0;
 */
void remove_comments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}
