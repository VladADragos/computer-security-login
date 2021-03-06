/* $Header: https://svn.ita.chalmers.se/repos/security/edu/course/computer_security/trunk/lab/login_linux/login_linux.c 585 2013-01-19 10:31:04Z pk@CHALMERS.SE $ */

/* gcc -std=gnu99 -Wall -g -o mylogin login_linux.c -lcrypt */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <sys/types.h>
#include <crypt.h>
/* Uncomment next line in step 2 */
#include "pwent.h" 

#define TRUE 1
#define FALSE 0
#define LENGTH 16
#define AGE_LIMIT 10
#define FAIL_LIMIT 4
void sighandler() {

	printf("reveived signal!\n");
	/* add signalhandling routines here */
	/* see 'man 2 signal' */
}

int main(int argc, char *argv[]) {

	mypwent *passwddata; /* this has to be redefined in step 2 */
	/* see pwent.h */

	char important1[LENGTH] = "**IMPORTANT 1**";

	char user[LENGTH];

	char important2[LENGTH] = "**IMPORTANT 2**";

	//char   *c_pass; //you might want to use this variable later...
	char prompt[] = "password: ";
	char *user_pass;

	sighandler();

	while (TRUE) {
		/* check what important variable contains - do not remove, part of buffer overflow test */
		printf("\tValue of variable 'important1' before input of login name: %s\n",
				important1);
		printf("\tValue of variable 'important2' before input of login name: %s\n",
				important2);

		printf("-----------------\n");
		printf("login: ");
		fflush(NULL); /* Flush all  output buffers */
		__fpurge(stdin); /* Purge any data in stdin buffer */

		
		// if (gets(user) == NULL) /* gets() is vulnerable to buffer */
		if (fgets(user,LENGTH,stdin) == NULL) /* gets() is vulnerable to buffer */
			exit(0); /*  overflow attacks.  */
		user[strlen(user)-1] = '\0';
		/* check to see if important variable is intact after input of login name - do not remove */
		printf("\tValue of variable 'important 1' after input of login name: %*.*s\n",
				LENGTH - 1, LENGTH - 1, important1);
		printf("\tValue of variable 'important 2' after input of login name: %*.*s\n",
		 		LENGTH - 1, LENGTH - 1, important2);

		passwddata = mygetpwnam(user);
		if(passwddata != NULL && passwddata->pwfailed > FAIL_LIMIT-1){
			printf("Too many failed login attempts!!\n");
			break;
		}
		user_pass = getpass(prompt);

		if (passwddata != NULL) {
			/* You have to encrypt user_pass for this to work */
			/* Don't forget to include the salt */


			user_pass = crypt(user_pass,passwddata->passwd_salt);
			if (!strcmp(user_pass, passwddata->passwd)) {

				printf("--------You're in !--------\n");
				printf("number of previous fails: %d\n",passwddata->pwfailed);
				passwddata->pwfailed = 0;
				passwddata->pwage++;
				mysetpwent(user,passwddata);
				if(passwddata->pwage>AGE_LIMIT){
					printf("please change password\n");
					user_pass = getpass(prompt);
					passwddata->passwd = crypt(user_pass, passwddata->passwd_salt);
					passwddata->pwage = 0;
					mysetpwent(user, passwddata);
				} 

				
				signal(SIGINT,SIG_IGN);
				signal(SIGTSTP,SIG_IGN);
				signal(SIGABRT,SIG_IGN);
				// setuid(getuid());
				execve("/bin/sh",NULL,NULL);
				perror("execve");
				
				/*  check UID, see setuid(2) */
				/*  start a shell, use execve(2) */

			}else{
				// update
				printf("Login Incorrect \n");
				passwddata->pwfailed++;
				mysetpwent(user,passwddata);
			
			}
		}else{

			printf("Login Incorrect \n");
		}
		
	}
	return 0;
}
