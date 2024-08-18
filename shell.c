#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Declare the environ variable */
extern char **environ;

#define BUFFER_SIZE 1024
#define PROMPT "#cisfun$ "
#define DELIM " \t\r\n\a"

/**
* print_prompt - Prints the shell prompt
*
* Description: This function prints the prompt that appears
*              before the user input in the shell.
*/
void print_prompt(void)
{
	write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
}

/**
* handle_signal - Handles signal interruption
* @sig: Signal number
*
* Description: This function handles the signal when the user
*              types Ctrl+C (SIGINT). It prevents the shell
*              from terminating and instead prints a new line.
*/
void handle_signal(int sig)
{
	if (sig == SIGINT)
	write(STDOUT_FILENO, "\n", 1);
}

/**
* parse_input - Splits the input into arguments
* @input: The input string to be parsed
*
* Description: This function splits the input string into
*              individual arguments that can be passed to execve.
*
* Return: An array of pointers to the parsed arguments.
*/
char **parse_input(char *input)
{
	int bufsize = BUFFER_SIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
    {
	perror("malloc");
	exit(EXIT_FAILURE);
    }

	token = strtok(input, DELIM);
	while (token != NULL)
    {
	tokens[position] = token;
	position++;

	if (position >= bufsize)
        {
	bufsize += BUFFER_SIZE;
	tokens = realloc(tokens, bufsize * sizeof(char *));
	if (!tokens)
            {
		perror("realloc");
		exit(EXIT_FAILURE);
            }
        }

	token = strtok(NULL, DELIM);
    }
	tokens[position] = NULL;
	return tokens;
}

/**
* main - The main function that runs the shell
*
* Description: This function runs an infinite loop where it
*	prompts the user for a command, reads the input,
*	and tries to execute it. If the command cannot
*	be found or executed, it prints an error message.
*
* Return: Always returns 0 on success.
*/
int main(void)
{
	char buffer[BUFFER_SIZE];
	char **args;
	pid_t pid;
	int status;

	/* Set up signal handler for Ctrl+C */
	signal(SIGINT, handle_signal);

	while (1)
    {
	/* Display prompt */
	print_prompt();

	/* Read input from user */
	if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
        {
		/* Check for EOF (Ctrl+D) */
		if (feof(stdin))
            {
		printf("\n");
		break;
            }
		continue;
        }

	/* Remove the newline character from the input */
	buffer[strcspn(buffer, "\n")] = '\0';

	/* Parse the input into arguments */
	args = parse_input(buffer);

	/* If the user types "exit", exit the shell */
	if (args[0] != NULL && strcmp(args[0], "exit") == 0)
        {
	free(args);
	break;
        }

	/* Fork a child process */
	pid = fork();
	if (pid < 0)
        {
	perror("fork failed");
	exit(EXIT_FAILURE);
        }

	if (pid == 0)
        {
		/* In child process */
	if (execve(args[0], args, environ) == -1)
            {
		perror(args[0]);
		exit(EXIT_FAILURE);
            }
        }
	else
        {
	/* In parent process, wait for child to finish */
		wait(&status);
        }

	/* Free the allocated memory for arguments */
	free(args);
    }

	return (0);
}
