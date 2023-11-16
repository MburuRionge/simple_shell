#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define ENABLE_GETLINE 0
#define ENABLE_STRTOK 0

#define HISTORY_FILE	".simple_shell_history"
#define HISTORY_MAX	5040

extern char **environ;


/**
 * struct list_node - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct list_node
{
	int num;
	char *str;
	struct list_node *next;
} list_t;

/**
 *struct pass_zone - contains pseudo-arguements to pass into a function,
 *allowing uniform prototype for function pointer struct
 *@arg: the string read using getline from user input (arguments)
 *@argv: an array of strings / commands from getline
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@error_num: the error status code for exit function
 *@count_flag: if on count this line of input
 *@file_name: the program filename used in compilation of the program
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from linked list env
 *@history: the history node
 *@aliases: the alias node
 *@env_change: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buffer: address of pointer to cmd_buf, on if chaining
 *@cmd_buffer_type: CMD_type ||, &&, ;
 *@readfiledescriptor: the fd from which to read line input
 *@history_count: the history line number count
 */
typedef struct pass_zone
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int error_num;
	int count_flag;
	char *file_name;
	list_t *env;
	list_t *history;
	list_t *aliases;
	char **environ;
	int env_change;
	int status;

	char **cmd_buffer;
	int cmd_buffer_type;
	int readfiledescriptor;
	int history_count;
} my_info;

#define DEFINE_INFO \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, \
	NULL, NULL, 0, 0, NULL, 0, 0, 0}

/**
 *struct built_in - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct built_in
{
	char *type;
	int (*func)(my_info *);
} built_t;

/* The strings function prototype */
int _strlen(char *);
int _strcmp(char *, char *);
char *is_prefix(const char *, const char *);
char *_strcat(char *, char *);
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);
char **strtow(char *, char *);
char **strtow2(char *, char);
void _eputs(char *);
int _eputchar(char);
int putfd(char c, int fd);
int put_fd(char *str, int fd);
/* The builtin function prototypes*/
int my_exit(my_info *);
int my_cd(my_info *);
int my_help(my_info *);
int my_history(my_info *);
int the_alias(my_info *);
char **get_zone_environ(my_info *);
int unsetenv_in_zone(my_info *, char *);
int setenv_in_zone(my_info *, char *, char *);
/* The memory and memory reallocation function prototypes*/
char *_memset(char *, char, unsigned int);
void free_ptr(char **);
void *_realloc(void *, unsigned int, unsigned int);
int bfree(void **);
/* Functions used with or for performing builtin functions*/
char *_getenv(my_info *, const char *);
int env(my_info *);
int my_setenv(my_info *);
int my_unsetenv(my_info *);
int fill_env_list(my_info *);
/* Function prototype to readline and get input*/
ssize_t get_input(my_info *);
int _getline(my_info *, char **, size_t *);
void sigintHandler(int);
/* Function prototypes that handles the shell history*/
char *history_file(my_info *zone);
int write_history(my_info *zone);
int read_history(my_info *zone);
int history_list(my_info *zone, char *buf, int linecount);
int rehistory(my_info, *zone);
/* Function prototypes that handles singly linked list*/
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);
/* Function prototypes that operate on singly linked list*/
size_t list_length(const list_t *);
char **list_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_begin_at(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);
/* Function prototypes that finds builtins and commands*/
int hsh(info_t *, char **);
int find_builtin(my_info *);
void find_cmd(my_info *);
void fork_cmd(my_info *);
/* Function protptypes that prints error*/
int err_int(char *);
void print_error(my_info *, char *);
int print_integer(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);
/* Function prototypes that replaces, aliases, variables e.t.c*/
int chain_related(my_info *, char *, size_t *);
void chain_check_zone(my_info *, char *, size_t *, size_t, size_t);
int processData(my_info *);
int replace_vars(my_info *);
int string_replace(char **, char *);
/* Function prototype that handles interactivity of the shell*/
int interactive(my_info *);
int is_delim(char, char *);
int is_alpha(int);
int my_atoi(char *);
/* Function prototypes that check if input is a command*/
int is_cmd(my_info *, char *);
char *dup_char(char *, int, int);
char *find_path(my_info *, char *, char *);
/* Function prototype for shell loop*/
int loophsh(char **);
/* Function prototypes that handles clear*/
void clear_zone(my_info *);
void set_zone(my_info *, char **);
void free_zone(my_info *, int);

#endif
