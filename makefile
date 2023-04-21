all: sshell

sshell: sshell.c
	gcc -O2 -Wall -Wextra -Werror -o sshell sshell.c
		/* Read command line */
clean:
	rm -f sshell     
     
     char *eof;


