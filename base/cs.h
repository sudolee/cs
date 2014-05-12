#ifndef _CS_H_
#define _CS_H_

#define SERVER_PORT 8080
#define SERVER_ADDR "127.0.0.1"

inline void dump_addr(struct sockaddr *addr)
{
	struct sockaddr_in *addrin = (struct sockaddr_in *)addr;

	printf("> famliy : %u\n", addrin->sin_family);
	printf("> port   : %u\n", addrin->sin_port);
	printf("> addr   : %s\n", inet_ntoa(addrin->sin_addr));
}

#endif /* _CS_H_ */
