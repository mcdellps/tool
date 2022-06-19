main.exe: main.c
	cc -o main.exe main.c

clean:
	rm *.yml *.cfg *.exe ansible_hosts
	echo Cleaning up files
