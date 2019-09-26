#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MULTICAST "224.0.0.1"

#define PORT 7777

int fd;

void init_socket();
int my_hton(const char*);

int main()
{
    int ret;
    struct sockaddr_in claddr;

    init_socket();

    claddr.sin_family = AF_INET;
	claddr.sin_port = htons(PORT);
	claddr.sin_addr.s_addr = my_hton(MULTICAST);

    ret = sendto(fd, "Hello", 5, 0, (struct sockaddr*) &claddr,
			        sizeof(struct sockaddr_in));
	if (ret == -1) {
		perror("sendto");
	}

    shutdown(fd, SHUT_RDWR);
    close(fd);

    return 0;
}

void init_socket()
{
    int ret;

    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (fd == -1) {
        perror("socket");
		exit(EXIT_FAILURE);
    }
}

int my_hton(const char* addr)
{
    int s_addr, ret;

    ret = inet_pton(AF_INET, addr, &s_addr);
	if (ret == 0) {
		perror("inet_pton(0)");
		close(fd);
		exit(EXIT_FAILURE);
	} else if (ret == -1) {
		perror("inet_pton(-1)");
		close(fd);
		exit(EXIT_FAILURE);
	}

    return s_addr;
}