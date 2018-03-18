#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_number(const char *s)
{
	int i;

	for (i = 0; i < strlen(s); i++) {
		if (s[i] > '9' || s[i] < '0')
			return 0;
	}

	return 1;
}

int main(int argc, char const *argv[])
{
	// FILE *fd = freopen("tema1.c", "r", stdin);
	
	printf("%d\n", is_number(argv[1]));
	printf("%i\n", atoi(argv[1]));
	/*char *string = malloc(255);

	while (!feof(stdin)) {
		scanf("%s", string);
		printf("%s\n", string);		
	}*/
	return 0;
}