/**
 * Operating Systems 2013-2017 - Assignment 2
 *
 * Sandu Dorogan, 335 CA
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define _GNU_SOURCE

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "cmd.h"
#include "utils.h"

#define READ		0
#define WRITE		1

#define STDIN		0
#define STDOUT		1
#define STDERR		2

/**
 * Internal change-directory command.
 */
static bool shell_cd(word_t *dir)
{
	DIE(dir == NULL, "Error cd");

	char *dir_str;
	bool ret;

	dir_str = get_word(dir);
	if (!dir_str)
		return 0;

	ret = chdir(dir_str);

	free(dir_str);
	return ret;
}

/**
 * Internal exit/quit command.
 */
static int shell_exit(void)
{
	/* execute exit/quit */
	return SHELL_EXIT;
}

/**
 * Sets a variable for a properly set @verb.
 */
static int set_var(word_t *verb)
{
	DIE(verb == NULL, "Error set var");

	const char *name;
	const char *value;

	char *command = get_word(verb);

	int ret;

	/* if the word_t got here,
	 * it sure points to the variable name with the first string
	 * the second string points to the '=' char
	 * the third string points to the desired value
	 */
	name = verb->string;
	value = command + strlen(name) + strlen("=");

	ret = setenv(name, value, 1);

	free(command);
	return ret;
}

/**
 * Redirect routines.
 * Makes the @filename file point to the same table entry as the @old_ptr.
 * The @filename file is opened with the mode @mode.
 * The file closes on exit.
 */
static void redirect(int old_ptr, char *filename, int mode)
{
	int rc;
	int new_ptr;

	new_ptr = open(filename, mode | O_CLOEXEC, 0644);
	DIE(new_ptr < 0, "open file");

	rc = dup2(new_ptr, old_ptr);
	DIE(rc < 0, "dup");
}

/**
 * Makes all the necessary redirections, for the given @command.
 *
 * @command  - command that has the filenames properly set.
 */
static void do_redirect(simple_command_t *command)
{
	DIE(command == NULL, "Error redirecting");

	char *filename = NULL;
	char *error_file = NULL;

	int mode = O_RDONLY;


	if (command->in) {
		filename = get_word(command->in);

		redirect(STDIN, filename, mode);
	}

	if (command->out) {
		filename = get_word(command->out);

		if (command->io_flags == IO_OUT_APPEND)
			mode = O_CREAT | O_APPEND | O_WRONLY;
		else
			mode = O_CREAT | O_TRUNC | O_WRONLY;

		redirect(STDOUT, filename, mode);
	}

	if (command->err) {
		error_file = get_word(command->err);

		if (command->out && !strcmp(error_file, filename)) {
			/* if a &> is encoutered,
			 * out and err should point to the same file name.
			 *
			 * In this case, the 1 and 2 index pointers from the
			 * file pointers table, namely stdout and stderr,
			 * should be the same pointer.
			 *
			 * At this point stdout surely points to the right file.
			 */
			dup2(STDOUT, STDERR);
			return;
		}

		if (command->io_flags == IO_ERR_APPEND)
			mode = O_CREAT | O_APPEND | O_WRONLY;
		else
			mode = O_CREAT | O_TRUNC | O_WRONLY;

		redirect(STDERR, error_file, mode);

		if (error_file)
			free(error_file);
	}

	if (filename)
		free(filename);
}

/**
 * Parse a simple command (internal, environment variable assignment,
 * external command).
 */
static int parse_simple(simple_command_t *s, int level, command_t *father)
{
	/* sanity checks */
	DIE(s == NULL, "Error simple");
	DIE(level < 0, "Error simple");


	pid_t pid;
	int status = -1;

	char **argv;
	char *command;

	char error_str[255];

	int i;
	int nr_of_params;


	command = get_word(s->verb);

	if (!strncmp(command, "exit", strlen("exit"))
	    || !strncmp(command, "quit", strlen("quit"))) {
		free(command);
		return shell_exit();
	}

	/* if variable assignment, execute the assignment
	 * and return the exit status
	 */
	if (strchr(command, '=')) {
		free(command);
		return set_var(s->verb);
	}

	pid = fork();
	switch (pid) {
	case -1:
		DIE(1, "Error forking");
		break;
	case 0:
		do_redirect(s);

		/* child process */
		argv = get_argv(s, &nr_of_params);
		execvp(argv[0], argv);

		/* only if exec failed */
		sprintf(error_str, "Execution failed for '%s'\n", command);
		write(STDERR, error_str, strlen(error_str));

		for (i = 0; i < nr_of_params; i++)
			free(argv[i]);

		free(command);
		exit(1);
		break;
	default:
		waitpid(pid, &status, 0);
		break;
	}

	if (!strncmp(command, "cd", strlen("cd"))) {
		/* the cd command: should change shell's current
		 * directory. The user shouldn't be deprived of
		 * redirections. Thus, beside the usual fork, that
		 * will change CHILD process' directory,
		 * shell should also chdir.
		 */
		status = shell_cd(s->params);
	}

	free(command);
	return WEXITSTATUS(status);
}

/**
 * Process two commands in parallel, by creating two children.
 */
static bool do_in_parallel(command_t *cmd1, command_t *cmd2, int level,
			   command_t *father)
{
	DIE(level < 0, "Error in parallel");
	DIE(cmd1 == NULL, "Error in parallel");
	DIE(cmd2 == NULL, "Error in parallel");
	DIE(cmd1->up != father, "Error in parallel");
	DIE(cmd2->up != father, "Error in parallel");

	/* execute cmd1 and cmd2 simultaneously */
	pid_t pid;

	int ret;
	int status;

	pid = fork();
	switch (pid) {
	case -1:
		DIE(1, "Error forking");
		break;
	case 0:
		ret = parse_command(cmd2, level, father);
		exit(ret);
		break;
	default:
		parse_command(cmd1, level, father);

		waitpid(pid, &status, 0);
		break;
	}

	return status;
}

/**
 * Redirect the output of cmd1 to the input of cmd2.
 *
 * Run commands by creating an anonymous pipe (cmd1 | cmd2)
 */
static bool do_on_pipe(command_t *cmd1, command_t *cmd2, int level,
		       command_t *father)
{
	DIE(level < 0, "Error in pipe");
	DIE(cmd1 == NULL, "Error in pipe");
	DIE(cmd2 == NULL, "Error in pipe");
	DIE(cmd1->up != father, "Error in pipe");
	DIE(cmd2->up != father, "Error in pipe");


	pid_t pid;

	int rd;
	int ret;
	int status;

	int pipefd[2];


	/* Making the processes to read and write in the pipe.
	 * To not to interfere with shell's std file entries, forking
	 * two processes. The parent writes, the child reads from the pipe.
	 */
	pid = fork();
	switch (pid) {
	case -1:
		DIE(1, "Error forking");
		break;
	case 0:
		rd = pipe(pipefd);
		DIE(rd < 0, "Error in pipe");


		pid = fork();
		switch (pid) {
		case -1:
			DIE(1, "Error forking");
			break;
		case 0:
			/* close unused end */
			close(pipefd[WRITE]);

			/* make the process read from pipe */
			rd = dup2(pipefd[READ], STDIN);
			DIE(rd < 0, "Dup");

			ret = parse_command(cmd2, level, father);

			/* close used end */
			close(STDIN);
			close(pipefd[READ]);

			exit(ret);
			break;
		}

		/* close unused end */
		close(pipefd[READ]);

		/* make the process write in pipe */
		rd = dup2(pipefd[WRITE], STDOUT);
		DIE(rd < 0, "Dup");

		parse_command(cmd1, level, father);

		/* close used end */
		close(STDOUT);
		close(pipefd[WRITE]);

		waitpid(pid, &status, 0);
		exit(WEXITSTATUS(status));
		break;

	default:
		waitpid(pid, &status, 0);
		break;
	}

	return WEXITSTATUS(status);
}

/**
 * Parse and execute a command.
 */
int parse_command(command_t *c, int level, command_t *father)
{
	/* sanity checks */
	DIE(c == NULL, "Error parsing");
	DIE(level < 0, "Error parsing");
	DIE(c->up != father, "Error parsing");

	int status = false;

	switch (c->op) {
	case OP_NONE:
		/* execute a simple command */
		status = parse_simple(c->scmd, level, father);
		break;
	case OP_SEQUENTIAL:
		/* execute the commands one after the other */
		parse_command(c->cmd1, level + 1, c);
		status = parse_command(c->cmd2, level + 1, c);
		break;

	case OP_PARALLEL:
		/* execute the commands simultaneously */
		status = do_in_parallel(c->cmd1, c->cmd2, level + 1, c);
		break;

	case OP_CONDITIONAL_NZERO:
		/* execute the second command only if the first one
		 * returns non zero
		 */
		status = parse_command(c->cmd1, level + 1, c);

		if (status)
			status = parse_command(c->cmd2, level + 1, c);

		break;

	case OP_CONDITIONAL_ZERO:
		/* execute the second command only if the first one
		 * returns zero
		 */
		status = 0;
		status = parse_command(c->cmd1, level + 1, c);

		if (!status)
			status = parse_command(c->cmd2, level + 1, c);

		break;

	case OP_PIPE:
		/* redirect the output of the first command to the
		 * input of the second
		 */
		status = do_on_pipe(c->cmd1, c->cmd2, level + 1, c);

		break;

	default:
		return SHELL_EXIT;
	}

	return status;
}
