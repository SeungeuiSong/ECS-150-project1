      char *eof;

	/* Read command line */
	eof = fgets(cmd, CMD_LINE_MAX, stdin);

	/* Handles EOF properly: input an exit command */
	if (!eof) {
		strncpy(cmd, "exit\n", CMD_LINE_MAX);
	}
