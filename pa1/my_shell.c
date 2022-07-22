#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SZ 1024
#define MAX_TOKENS 64
#define TOKEN_LEN 64

int main() {
	char cmdbuf[BUF_SZ];
	char *tokens[MAX_TOKENS];
	char *token;
	int i, ntokens;
	int rc, rce;
	
	while (1) {
		printf("$ ");
		
		fgets(cmdbuf, BUF_SZ, stdin);
		
		/* user pressed enter without any command */
		if (cmdbuf[0] == '\n') {
			continue;
		}
		
		/* exit built-in for the shell, check for "exit\n" */
		if (strlen(cmdbuf) == 5 && cmdbuf[0] == 'e' && cmdbuf[1] == 'x' && cmdbuf[2] == 'i' && cmdbuf[3] == 't')
			break;
		
		/* fgets also reads and stores the \n, we must remove it */
		i = 0;
		while (cmdbuf[i] != '\n')
			i++;
		cmdbuf[i] = '\0';
		cmdbuf[i+1] = (char) 0;
		
		ntokens = 0;
		token = strtok(cmdbuf, " ");
		
		/* get the options passed to the entered command */
		/* also count the number of tokens in the input */
		while (token != NULL) {
			tokens[ntokens] = token;
			token = strtok(NULL, " ");
			ntokens++;
		}
		tokens[ntokens] = (char *) NULL; /* for execvp() */
		
		rc = fork();
		if (rc < 0) {
			fprintf(stderr, "fork failed!\nexiting...\n");
			exit(EXIT_FAILURE);
		} else if (rc == 0) {
			rce = execvp(tokens[0], tokens);
			if (rce == -1) {
				fprintf(stderr, "error in input\n");
				exit(EXIT_FAILURE);
			}
		} else {
			wait(NULL);
		}
	}
	
	return 0;
}
