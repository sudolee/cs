#ifndef _CS_H_
#define _CS_H_

#define SERVER_PORT 8080
#define SERVER_LISTEN_BACKLOG 511
#define SERVER_ADDR "127.0.0.1"

#define BUFFER_SIZE 2048

inline void dump_addr(struct sockaddr *addr)
{
	struct sockaddr_in *addrin = (struct sockaddr_in *)addr;

	printf("> famliy : %u\n", addrin->sin_family);
	printf("> port   : %u\n", addrin->sin_port);
	printf("> addr   : %s\n", inet_ntoa(addrin->sin_addr));
}

inline int do_send(int sock, void *buffer, int len)
{
	int rtval;

	while(len > 0)
	{
		rtval = send(sock, buffer, len, 0);
		if(rtval == -1)
		{
			perror(":: send ");
			return -1;
		}
		else
		{
			buffer += rtval;
			len    -= rtval;
		}
	}

	return 0;
}

#endif /* _CS_H_ */
