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

