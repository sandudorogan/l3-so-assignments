#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *strdup(const char *string)
{
	// Space for length plus nul
	char *buffer = malloc(strlen(string) + 1);
	// No memory
	if (buffer == NULL)
		return NULL;

	// Copy the characters
	strcpy(buffer, string);

	// Return the new string
	return buffer;
}

int is_number(const char *s)
{
	int i;

	for (i = 0; i < strlen(s); i++) {
		if (s[i] > '9' || s[i] < '0')
			return 0;
	}

	return 1;
}

int is_line_blank(const char *line)
{
	int return_value = 0;
	char *delim = " \t\n";
	char *buffer = strdup(line);

	if (!strtok(buffer, delim))
		return_value = 1;
	else
		return_value = 0;

	free(buffer);
	return return_value;
}

size_t getline(char **lineptr, size_t *n, FILE *stream)
{
	char *bufptr = NULL;
	char *p = bufptr;
	size_t size;
	int c;

	if (lineptr == NULL)
		return -1;
	if (stream == NULL)
		return -1;
	if (n == NULL)
		return -1;

	bufptr = *lineptr;
	size = *n;

	c = fgetc(stream);
	if (c == EOF)
		return -1;

	if (bufptr == NULL)
		bufptr = malloc(128);
		if (bufptr == NULL)
			return -1;

		size = 128;

	p = bufptr;
	while (c != EOF) {
		if ((p - bufptr) > (size - 1)) {
			size = size + 128;
			bufptr = realloc(bufptr, size);
			if (bufptr == NULL)
				return -1;

		}
		*p++ = c;
		if (c == '\n')
			break;

		c = fgetc(stream);
	}

	*p++ = '\0';
	*lineptr = bufptr;
	*n = size;

	return p - bufptr - 1;
}
