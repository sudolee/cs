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
	int sock, new, rtval;
	int reuse = 1;
	struct sockaddr_in addr;
	struct sockaddr addrin;

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
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	rtval = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if(rtval == -1)
	{
		perror(":: bind ");
		exit(EXIT_FAILURE);
	}

	rtval = listen(sock, 50);
	if(rtval == -1)
	{
		perror(":: listen ");
		exit(EXIT_FAILURE);
	}

	printf(":: Listening on port %u...\n", SERVER_PORT);

	while(new = accept(sock, NULL, 0))
	{
		int pid = fork();
		if(pid == -1)
		{
			perror(":: fork ");
			exit(EXIT_FAILURE);
		}
		else if(pid > 0)
		{
			/* - parent - */

			socklen_t len = sizeof(addr);
//			memset(&addrin, 0, sizeof(addrin));
			rtval = getpeername(new, (struct sockaddr *)&addr, &len);
			if(rtval >= 0)
			{
				dump_addr(&addrin);
			}
			else
			{
				perror(":: getpeername ");
			}

			close(new);
			continue;
		}
		else
		{
			/*
			 * - clild -
			 * TODO: data transfer
			 */

			printf(":: forked in child.\n");
			exit(EXIT_SUCCESS);
		}
	}


	close(sock);

	exit(EXIT_SUCCESS);
}
