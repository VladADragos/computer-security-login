all: login_linux

mylogin: mylogin.c pwent.h pwent.c
	gcc -g -Wall pwent.c mylogin.c -lcrypt -o build/mylogin

login_linux: login_linux.c pwent.h pwent.c
	gcc -g -Wall pwent.c login_linux.c -lcrypt -o build/login_linux

makepass: makepass.c
	gcc -o build/makepass makepass.c -lcrypt 
clean:
	rm -f *.o mylogin login_linux
