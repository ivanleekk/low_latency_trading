main: *.c
	gcc *.c -lcurl -o main.exe

clean:
	rm -f main.exe