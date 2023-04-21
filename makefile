all: sshell

sshell: sshell.c
	gcc -O2 -Wall -Wextra -o sshell sshell.c

clean:
	rm -f sshell
