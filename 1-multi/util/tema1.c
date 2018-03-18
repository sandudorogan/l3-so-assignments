
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "list.h"
#include "utils.h"
#include "stdbool.h"
#include "hashtable.h"

#define HASH_SIZE_IDX 1
#define COMM_FILE_IDX 2
#define ARGUMENTS_NEEDED 2
#define MAX_LINE_SIZE 20001

#define ADD 0
#define FIND 7
#define PRINT 4
#define CLEAR 2
#define REMOVE 1
#define PRINT_BUCKET 3
#define RESIZE_DOUBLE 5
#define RESIZE_HALVE 6


int parse_line(char *line, char **command, char **frst_arg, char **scnd_arg);
int parse_command(char *command, char *frst_arg);
int execute_command(list ***hashtable, int hash_size, char *file_line);
void find(list **hashtable, int hash_size, char *frst_arg, char *scnd_arg);
int print_bucket(list **hashtable, int hash_size, char *index,
		 char *file_name);
void print(list **hashtable, int hash_size, char *file_name);
void resize(list ***hashtable, int *hash_size, int how);


int main(int argc, char const *argv[])
{
	char *file_line;
	size_t line_size = MAX_LINE_SIZE;

	int command_file = COMM_FILE_IDX;

	unsigned int hash_size;

	int ed;

	list **hashtable;


	if (argc < ARGUMENTS_NEEDED) {
		fprintf(stderr, "Usage: %s nr_of_buckets [command_files]\n",
			argv[0]);
		exit(ERROR_CODE);
	}

	hash_size = atoi(argv[HASH_SIZE_IDX]);
	DIE(!hash_size, "Somethings wrong with the size provided");
	ed = init_hashtable(&hashtable, hash_size);
	DIE(ed == ERROR_CODE, "Unable to initialise the hashtable");

	file_line = malloc(sizeof(char) * line_size);
	DIE(!file_line, "Not enough memory");

	// Looks if there are command files.
	// If not, it'll take the info from stdout.
	// It ignores the first file in the processing loop.
	if (argc > ARGUMENTS_NEEDED) {
		freopen(argv[command_file], "r", stdin);
	}

	do {
		ed = execute_command(&hashtable, hash_size, file_line);

		if (ed == ERROR_CODE) {
			free_hashtable(hashtable, hash_size);
			free(file_line);
			DIE(1, "Somethings wrong with the given command");
		}

	} while (freopen(argv[++command_file], "r", stdin) != NULL);

	free_hashtable(hashtable, hash_size);
	free(file_line);

	return 0;
}

int execute_command(list ***hashtable, int hash_size, char *file_line) 
{
	int command_id;

	size_t line_size = MAX_LINE_SIZE;

	char *command  = NULL;
	char *frst_arg = NULL;
	char *scnd_arg = NULL;

	int ed;


	while (!feof(stdin)) {
		ed = getline(&file_line, &line_size, stdin);
		DIE(ed == -1, "Input failure");
		// ed = fscanf(stdin, "%s", file_line);

		parse_line(file_line, &command, &frst_arg, &scnd_arg);

		command_id = parse_command(command, frst_arg);
		if (command_id == ERROR_CODE) {
			return ERROR_CODE;
		}

		switch (command_id) {
		case ADD:
			ed = add_in_hash(*hashtable, hash_size, frst_arg);
			if (ed == ERROR_CODE) {
				return ERROR_CODE;
			}
			break;

		case REMOVE:
			remove_from_hash(*hashtable, hash_size, frst_arg,
					 &strcmp);
			break;

		case FIND:
			find(*hashtable, hash_size, frst_arg, scnd_arg);
			break;

		case CLEAR:
			empty_hashtable(*hashtable, hash_size);
			break;

		case PRINT_BUCKET:
			ed = print_bucket(*hashtable, hash_size, frst_arg,
					  scnd_arg);
			if (ed == ERROR_CODE) {
				return ERROR_CODE;
			}
			break;

		case PRINT:
			print(*hashtable, hash_size, frst_arg);
			break;

		case RESIZE_DOUBLE:
			resize(hashtable, &hash_size, RESIZE_DOUBLE);
			break;

		case RESIZE_HALVE:
			resize(hashtable, &hash_size, RESIZE_HALVE);
			break;
		}

		if (command) {
			free(command);
		}
		if (frst_arg) {
			free(frst_arg);
		}
		if (scnd_arg) {
			free(scnd_arg);
		}
	}

	return SUCCESS_CODE;
}


int parse_line(char *line, char **command, char **frst_arg, char **scnd_arg)
{
	char *token;
	char *delim = " \t\n";


	token = strtok(line, delim);
	if (token == NULL)
		return ERROR_CODE;

	*command = strdup(token);
	if (*command == NULL)
		return ERROR_CODE;

	token = strtok(NULL, delim);
	if (token == NULL)
		return 0;

	*frst_arg = strdup(token);
	if (*frst_arg == NULL)
		return ERROR_CODE;

	token = strtok(NULL, delim);
	if (token == NULL)
		return 0;

	*scnd_arg = strdup(token);
	if (*scnd_arg == NULL)
		return ERROR_CODE;

	return 0;
}

int parse_command(char *command, char *frst_arg)
{
	if (!strncmp(command, "add", strlen("add"))) {
		return ADD;
	}
	if (!strncmp(command, "remove", strlen("remove"))) {
		return REMOVE;
	}
	if (!strncmp(command, "find", strlen("find"))) {
		return FIND;
	}
	if (!strncmp(command, "clear", strlen("clear"))) {
		return CLEAR;
	}
	if (!strncmp(command, "print_bucket", strlen("print_bucket"))) {
		return PRINT_BUCKET;
	}
	if (!strncmp(command, "print", strlen("print"))) {
		return PRINT;
	}
	if (!strncmp(command, "resize", strlen("resize"))) {
		if (!strncmp(frst_arg, "double", strlen("double"))) {
			return RESIZE_DOUBLE;
		}
		if (!strncmp(frst_arg, "halve", strlen("halve"))) {
			return RESIZE_HALVE;
		}
	}

	return ERROR_CODE;
}

void find(list **hashtable, int hash_size, char *word, char *file_name)
{
	FILE *result_file;
	int ed;


	result_file = stdout;
	if (file_name) {
		result_file = fopen(file_name, "a");
		DIE(!result_file, "Could not open the file");
	}


	ed = find_in_hash(hashtable, hash_size, word,
			  &strcmp);
	if (ed == FOUND) {
		fprintf(result_file, "%s\n", "True");
	} else {
		fprintf(result_file, "%s\n", "False");
	}

	fflush(result_file);
	if (result_file != stdout)
		fclose(result_file);
}

int print_bucket(list **hashtable, int hash_size, char *index,
		 char *file_name)
{
	FILE *result_file;
	int idx;


	if (!is_number(index)) {
		return ERROR_CODE;
	}
	idx = atoi(index);
	if (idx < 0 || idx >= hash_size) {
		return ERROR_CODE;
	}


	result_file = stdout;
	if (file_name) {
		result_file = fopen(file_name, "a");
		DIE(!result_file, "Could not open the file");
	}

	print_list_in_file(hashtable[idx], result_file);

	fflush(result_file);
	if (result_file != stdout)
		fclose(result_file);


	return SUCCESS_CODE;
}

void print(list **hashtable, int hash_size, char *file_name) 
{
	FILE *result_file;


	result_file = stdout;
	if (file_name) {
		result_file = fopen(file_name, "a");
		DIE(!result_file, "Could not open the file");
	}

	print_hash_in_file(hashtable, hash_size, result_file);

	fflush(result_file);
	if (result_file != stdout)
		fclose(result_file);
}

void resize(list ***hashtable, int *old_size, int how)
{
	int idx;
	int new_size;
	int ed;

	entry *proxy;
	list **old_hash;


	old_hash = *hashtable;
	if (how == RESIZE_DOUBLE) {
		new_size = *old_size * 2;
	} else {
		new_size = *old_size / 2;
	}


	ed = init_hashtable(hashtable, new_size);
	DIE(ed == ERROR_CODE, "Could not init the new hashtable");

	for (idx = 0; idx < *old_size; idx++) {
		while (!is_empty_list(old_hash[idx])) {
			proxy = take_first(old_hash[idx]);
			append_in_hash(*hashtable, new_size, proxy);
		}
	}

	free_hashtable(old_hash, *old_size);
	*old_size = new_size;
}
