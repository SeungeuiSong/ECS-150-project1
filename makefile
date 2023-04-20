all: sshell

sshell: sshell.c
	gcc -O2 -Wall -Wextra -Werror -o sshell sshell.c
		/* Read command line */
	eof = fgets(cmd, CMD_LINE_MAX, stdin);

	/* Handles EOF properly: input an exit command */
	if (!eof) {
		strncpy(cmd, "exit\n", CMD_LINE_MAX);
	}

clean:
	rm -f sshell     
     
     char *eof;


