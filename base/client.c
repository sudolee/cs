#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "cs.h"

int msg_loop(int sock)
{
	char *buffer;
	int rtval, len;

	buffer = malloc(BUFFER_SIZE);
	if(!buffer)
	{
		perror(":: malloc ");
		return -1;
	}

	for(len = BUFFER_SIZE;;)
	{
		memset(buffer, 0, len);

		rtval = read(0, buffer, BUFFER_SIZE);
		if(rtval == -1)
		{
			perror(":: read ");
			break;
		}

		len = rtval;

		rtval = do_send(sock, buffer, rtval);
		if(rtval == -1)
		{
			break;
		}

		rtval = recv(sock, buffer, BUFFER_SIZE, 0);
		if(rtval == -1)
		{
			perror(":: recv ");
			break;
		}

		if((len > rtval) || (len = rtval))
			;

		printf("> %s", buffer);
	}

	free(buffer);
	return -1;
}

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

	msg_loop(sock);

	close(sock);

	exit(EXIT_SUCCESS);
}
