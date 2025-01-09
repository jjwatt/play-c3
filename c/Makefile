hello-caca: hello-caca.c
	gcc hello-caca.c -o hello-caca -lcaca

loadimage3: loadimage3.c
	gcc loadimage3.c -O3 -o loadimage3 -lcaca -lImlib2

%: %.c
	gcc -O3 $< -o $@ -lcaca -lm
