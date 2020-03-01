#include <dirent.h>
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h>



struct pvtfile //struct to save directoryName and path of the file/folder to be made private
{
	// Storing path of the file/folder with file/folder name to distinguish 
	// between files/folders of same name in different directories
	char path[100];
	char filename[30];
};

struct pvtfile * pvtfiles; //list of structs of private file/folders
int pvtsize;
char * password; //default password for private files

//------------------------------------------------------------------------------

void startShell()
{
	printf("\t\tHi! This is your own Shell\n\n");
	printf("Current Directory is: \n\n");
	currentDirectory();
	printf("This shell supports following commands:\n1. ls \n2. pwd\n3. cd <directoryName>\n4. clear\n5.  mkdir <directory name>\n6.  rmdir <directory name>\n7. exit\n\n");
	
	printf("\n\n Enter the command name you want to execute:  ");
}

void mkdir(char * str)
{
     execl ("/bin/mkdir", "mkdir", str, (char *) NULL);
     printf("Directory has been made for you !");
}

void exermdir(char * str)
{
if(str == NULL)
	{
		printf("ERROR! argument (directory name) missing.\n");
	}
     execl ("/bin/rmdir", "rmdir", str, (char *) NULL);
       printf("Directory deleted successfully");
}

void currentDirectory()	//Display CURRENT DIRECTORY
{
	char array[100];
	getcwd(array, sizeof(array));	//get current directory's path
	
	char *path, *directory;
	//tokenize the path array to get directory name
	path = strtok(array, "/");
	while(path!=NULL)
	{
		directory=path;
		path=strtok(NULL, "/");
	}
	printf(directory);
	printf("\n\n");
}

int getCommand(char *com)	//gets user input as a complete line including command, and argument (if any)
{
	char * str = malloc(30);
	str = gets(str);
	if(strlen(str) == 0)
	{
		free(str);
		return 1;
	}
	strcpy(com, str);	//copies whole command line
	free(str);
	return 0;
}

int checkCommand(char *str, char **command, char ** argument)	
{	
	//check what command has been given by user in command line
	
	
	*command = strtok(str, " ");
	//seperate command and its argument
	*argument = strtok(NULL, " ");
	
	//return specific commandType according to each command
	if(strcmp(*command, "ls")==0)
		return 1;
	else if(strcmp(*command, "pwd")==0)
		return 2;
	else if(strcmp(*command, "cd")==0)
		return 3;
	else if(strcmp(*command, "clear")==0)
		return 4;
	else if(strcmp(*command, "mkdir")==0)
		return 5;
	else if(strcmp(*command, "rmdir")==0)
		return 6;
	else if(strcmp(*command, "exit")==0)
		return 7;
	else
		return -1;

}

void execls(int type, char * arg) //execute functionality of 'ls' command
{
	DIR *directory1 = opendir(".");
	struct dirent * current;
	if(type==0) //for simple 'ls'
	{
		int i;
		while ((current = readdir(directory1)) != NULL) //get contents of current directory and print them
		{
			if(!(strcmp(current->d_name, "..")==0 || strcmp(current->d_name, ".")==0))
			{
				char arr[100];
				for(i=0; i<=pvtsize; i++)
				{
					//print all those files/folders which are not in private list
					if((strcmp(current->d_name, pvtfiles[i].filename)==0) && (strcmp(getcwd(arr, sizeof(arr)), pvtfiles[i].path)==0))
						current = readdir(directory1);
				}
				if(current!=NULL)
					printf("%s\n", current->d_name);
			}
		}
	}
	else
	{
		printf("ERROR! Argument of command 'ls' not valid.\n");
	}
	closedir(directory1);
}

void execpwd() //execute functionality of 'pwd' command
{
	char array[100];
	getcwd(array, sizeof(array)); //gets the complete current path in 'array'
	
	printf("%s\n", array);
}


void execcd(char * str) //execute functionality of 'cd' command
{
	if(str == NULL)
	{
		printf("ERROR! argument (directory name) missing.\n");
	}
	else if(chdir(str) == 0) //chdir(str) is used to change directory
	{
	    printf("Directory Changed!\n");
        printf("Current Directory is: \n\n");
     	currentDirectory();
	
	}
	else if(chdir(str) != 0) 
	 	printf("ERROR! No such directory exists.\n");
}


int main()
{
	
	printf("\e[1;1H\e[2J"); // clear screen
	startShell();	
	
	//------------------------------------------------
	pvtfiles = calloc(20, sizeof(struct pvtfile));
	pvtsize = -1;
	password = calloc(10, sizeof(char));
	strcpy(password, "9090\0"); //set default password to '9090'
	
	char *input = calloc(100, sizeof(char));
	char * command;
	char * argument;
	int i;
	
	//------------------------------------------------
	int commandType;
	while(1)	//runs the shell
	{		
		currentDirectory();	//Prints current directory name
		
		getCommand(input);	//gets user input on command line

		commandType = checkCommand(input, &command, &argument);	//checks command Type entered by user on command line
		
	if(commandType == 1)	//for 'ls'
		{
			int x = 0;
			if(argument != NULL)
				x = 1;
			execls(x, argument);
		}
		else if(commandType == 2)	//for 'pwd'
		{
			execpwd();
		}
		else if(commandType == 3)	//for 'cd'
		{
			execcd(argument);
		}
		
		else if(commandType == 4) //for 'clear' command --- clears the console screen
		{
		printf("\e[1;1H\e[2J");
			startShell();
		}
		else if(commandType == 5) //for making directory
		{
		      mkdir(argument);
			
		}
		else if(commandType == 6) //for removing directory
		{
		      exermdir(argument);
			
		}
	else if(commandType == 7) //for 'exit' command -- exits the custom shell
			break;
	else
			printf("ERROR! '%s' command not supported in this shell.\n", command);
	}
	
	printf("\n");
}


