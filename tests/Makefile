all: newlink getlink

newlink: newlink.c
	gcc -I /usr/include/libnl3 -I .. -o $@ $^ -l nl-3

getlink: getlink.c
	gcc -I /usr/include/libnl3 -I .. -o $@ $^ -l nl-3
