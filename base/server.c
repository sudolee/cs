#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "cs.h"

int msg_loop(int sock)
{
	int rtval, len, pid;
	char *buffer;

	buffer = malloc(BUFFER_SIZE);
	if(!buffer)
	{
		perror(":: malloc ");
		return -1;
	}

	pid = getpid();

	for(len = BUFFER_SIZE;;)
	{
		memset(buffer, 0, len);

		rtval = recv(sock, buffer, BUFFER_SIZE, 0);
		if(rtval == -1)
		{
			perror(":: recv ");
			break;
		}
		else if(!rtval)
		{
			/*
			 * rtval = 0, when no message recv in nonblock mode.
			 * rtval = 0, when peer shutdown in block mode.
			 */
			printf(":: Peer shutdown, :(\n");

			free(buffer);
			return 0;
		}
		else
		{
			len = rtval;

			printf("[%d] : %s", pid, buffer);

			rtval = do_send(sock, buffer, len);
			if(rtval == -1)
			{
				break;
			}
		}
	}

	free(buffer);
	return -1;
}

int main(int argc, char **argv)
{
	int sock, newsk, rtval;
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

	rtval = listen(sock, SERVER_LISTEN_BACKLOG);
	if(rtval == -1)
	{
		perror(":: listen ");
		exit(EXIT_FAILURE);
	}

	printf(":: Listening on port %u...\n", SERVER_PORT);

	do
	{
		newsk = accept(sock, NULL, 0);
		if(newsk == -1)
		{
			perror(":: accept ");
			continue;
		}

		int pid = fork();
		if(pid == -1)
		{
			perror(":: fork ");
			exit(EXIT_FAILURE);
		}
		else if(pid > 0)
		{
			/* - parent - */

			socklen_t len = sizeof(addrin);
			memset(&addrin, 0, sizeof(addrin));
			rtval = getpeername(newsk, &addrin, &len);
			if(rtval >= 0)
			{
				dump_addr(&addrin);
			}
			else
			{
				perror(":: getpeername ");
			}

			close(newsk);
			continue;
		}
		else
		{
			printf(":: forked in child.\n");

			close(sock);

			msg_loop(newsk);

			close(newsk);
			exit(EXIT_SUCCESS);
		}
	} while(1);

	close(sock);

	exit(EXIT_SUCCESS);
}
