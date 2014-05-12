#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "cs.h"


int main(int argc, char **argv)
{
	int sock, rtval;
	int reuse = 1;
	struct sockaddr_in addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		perror(":: socket ");
		exit(EXIT_FAILURE);
	}

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = SERVER_PORT;
	inet_aton(SERVER_ADDR, &addr.sin_addr);

	dump_addr((struct sockaddr *)&addr);

	rtval = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
	if(rtval == -1)
	{
		perror(":: connect ");
		exit(EXIT_FAILURE);
	}

	/*
	 * TODO: data transfer
	 */

	close(sock);

	exit(EXIT_SUCCESS);
}
