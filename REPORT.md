# SSHELL: Simple Shell

# Summary/Introduction

The goal of this project was to implement a simple shell in C, in which the 
source code is contained in the **shell.c** file. The shell is an important 
feature for Unix-system navigation and acts as a user interface to access
the OS. 

In other words, the shell allows users to input some command line, 
in which the shell has the capability to interpret this command line.
The overall goal of this project was to understand important UNIX system calls, 
child/parent functions, exec() functions, and how one may program a shell in C.

# Project Implementation

The implementation of the program can be categorized into two main phases:

1. Initializing neccesary variables and making the argumentList[] array by
interpreting the command line via tokenization
2. Feeding the argumentList[] array into a certain function depending on its
contents or on the isRedirect flag

We'll talk about the two below:

1. 
At the beginning of execution, we initialize a cmd input which allows the 
shell to actually get some command from the user via fgets/stdin. 
Along with this, we initialize another set of variables which the simple shell 
requires in order to keep track of everything. What's important 
to note is that some of these variables will be unused 
depending on the type of command line the user inputs (for example, 
redirectionLocation when it's not a redirection).

First, the program starts with printing the prompt 'sshell$', 
along with some other logic such that the prompt and command line can 
be inputted properly. A cmdDisplay variable is made because we use 
strtok on cmd (which is the command line input), and using strtok manipulates 
the cmd variable. This requires us to strcopy cmd into cmdDisplay for display
purposes at the end of each executed command.

We then declare an char* argumentList[] variable, bounded by the
specificed argument max macro plus 1 due to NULL being added to 
the end (in order for this to easily pass into execvp). We then pass the 
command line (as the variable cmd) and the array into the populateArray 
function,which populates the array created. The array becomes virtually the 
same as argv[].

The command line is then properly put into an array using tokenizing 
logic. We declare a char * variable called token, which basically is for the 
purpose of getting individual argments of the command line. We strtok through 
the command line, and upon hitting a white character, token gets set to the
first argument (or better said, word) of the command line. We store token
into the array. We then use strtok again to on token in order
to get the next word as we use another white space delimiter. We repeat this
until we're at the end of the command line.

While doing so, we keep track of the number of arguments we have (argc) 
and if we know whether or not we need to do a redirection 
(setting redirection to 1) or a regular command.

This transitions to part 2 of the program after the array is populated.

2. 
The array then passes into a nested if function only if isError
is 0 (set to 1 if an error occurs anywhere, such as too
many arguments or a token being too long). In other words, a nested if
function guarded by another if.

This nested if function covers all of the possible features of our shell.

These features include:
- The builtin commands of exit, cd, and pwd
- The utility commands of a terminal with zero or multiple arguments (cat,
echo, etc.)
- Output Redirection

argumentList[0] is checked for our builtin commands. Our three builtin 
commands (exit, pwd, and cd) are checked for first, and if successful 
will run one of many functions of our program(see below).

Next, redirection is checked. Redirection is set to 1
based on our array/strtok logic from above. If we detect
a ">" token, we know that the inputted command must be 
a redirection, and is passed into the runRedirection function accordingly.

Lastly (regarding the nested if), if all of these fail, we know that there are
two more possible things it can be: a real utility command of the shell or a 
command that does not exist. executeRegularCommand will take care of this 
command (argumentList[0]) exists or not (see details below)

# List of Global Variables

int isError: 1 when an error occurs, 0 otherwise. Important for error
handling, as an error can occur anywhere across functions (a token too long,
execvp returns -1, etc.).

int argc: The argument count, or size of the argument array. Is needed
for multiple functions, so it makes more sense to declare as a global 
variable rather than passing it as an additional argument for multiple 
functions.

int redirectionLocation: Relevant to the runRedirection function. Knows
where the metacharacter '>' across a command line input, which helps us figure
out what is the command or the output file.

int isRedirect: 1 when the meta character '>' is detected in the command line
input. 0 otherwise.

The reason they are global is because they are required for multiple functions.
The values do not change across function calls and only change when a new
command is interpreted or array is created. They are set to 0 every time
a new command is interpreted (refer to the top the while loop in main)

# List of functions

void runExit(): Runs when argumentList[0] is "exit". Prints bye along with
a completed message.

void runPwd(): Runs when argumentList[0] is "pwd". Prints the current
working directory.

void runCd(char *destination): Runs when argumentList[0] is "cd".
It takes in argumentList[1] (called destination in the function), and uses 
chdir() on destination in order to move the current directory/destination.
If chdir() returns a -1, we will print an error message.

void runRedirection(char *argumentList[]): Runs when isRedirection = 1.
This function changes the output directory into the file name inputted by the
user, specifically after the meta character '>'. It utilizes dup2() and
a temporary array in order to change the output directory and execute the
command properly.

void executeRegularCommand(char* argumentList[]): Takes in an argument
list. Will run execvp(argumentList[0], argumentList) if an error is not
detected beforehand. Utilizes child/parent functions.

void populateArray(char* cmd, char* argumentList[]): Takes the command line
and populates the argumentList[] array.

All of the above functions have a guard clauses to detect a potential error. 
For example, runRedirection if no output file is detected or run.

# Limitations and Possible Design Optimizations

Our simple shell is unable to support piping, simple environment variables,
and combined redirection. This was mainly due to time constraints.

There was also a mistake with our interpretation of output redirection.
We had accidently considered the command "echo hello > myfile.txt"
instead of "echo hello>myfile.txt" and due to time constraints
we weren't able to fix this bug (although, it still passes the autograder
tests).

Our initial design goal was to make a struc called command, which would
encompass everything that is related to command. Examples 
would include argc (the number of arguments of a command), argv[] 
(synonymous to our argumentList[]), and other possible
variables such as if it was a command that required redirection
(similar to our isRedirection variable). 

We would store these struc commands into a data structure
(such as a linked list) and iterate through it in order to implement piping 
commands for example.

We left the implementation of struc command out primarily due to the fact
our program did not support piping, thus making this struc command 
redundant as it really only supports one command at a time.
If we were to do piping, an obvious design optimization 
would be to make this structure.

# Sources
stackoverflow.com
geeksforgeeks.org
linux.die.net
tutorialspoint.com
gnu.org
