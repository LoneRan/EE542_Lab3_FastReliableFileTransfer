
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
//#include "linkedlist.h"

#define BUF 1400

struct packetLoss
{
	uint8_t offset;
	int ID;
	char data[BUF];
};

struct packetI
{
	uint8_t offset;
	u_int file_size;
	u_int num;
};

struct packetA
{
	uint8_t offset;
	uint8_t index[BUF];
};

double calTime(struct timeval time1, struct timeval time2)
{
	long elap = (time1.tv_sec - time2.tv_sec) * 1000000 + time1.tv_usec - time2.tv_usec;
	long res = elap / 1000;
	return res;
}

int main(int argc, char *argv[])
{
	FILE *fptr;

	long fileSize;

	char data[60000];
	size_t InPacket;
	int packetNum;
	int fileOffset;
	struct packetLoss packet = {0};
	char *packetSent;
	char buffer1[256];
	bool state_ch = 1;

	struct packetI packetI = {0};
	char *send_buffer;
	struct packetA *ack1;
	//struct packetA ack2;

	int sockfd, n;
	unsigned int length;
	struct sockaddr_in serverAddress, clientAddress;
	struct hostent *hp;
	char fileName[20];

	//char *serverName = malloc(256 * (sizeof(char)));

	int opt;

	while ((opt = getopt(argc, argv, "d:p:f:")) != -1)
	{
		switch (opt)
		{
		case 'd':

			serverAddress.sin_addr.s_addr = inet_addr(optarg);
			break;
		case 'p':
			serverAddress.sin_port = htons(atoi(optarg));
			break;
		case 'f':
			strcpy(fileName, optarg);
			break;
		case ':':
			fprintf(stderr, "malformed command\nOption -%c requires an aperand\n", optopt);
			exit(-1);
			break;
		case '?':
			printf("unknown option: %c\n", optopt);
			fprintf(stderr, "Usage: %s [-d destinantion_IP_addr] [-p port_number] [-f file_name]\n", argv[0]);
			exit(-1);
			break;
		}
	}

	// hp = gethostbyname(BAD_SERVER_ADDR);
	// if (hp == 0)
	// {
	// 	fprintf(stderr, "Error: socket error\n");
	// 	exit(-1);
	// }

	// bcopy((char *)hp->h_addr, (char *)&serverAddress.sin_addr, hp->h_length);

	serverAddress.sin_family = AF_INET;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		fprintf(stderr, "Error: creating socket\n");
		exit(-1);
	}

	length = sizeof(struct sockaddr_in);

	fptr = fopen(fileName, "rb");
	if (fptr == NULL)
	{
		fputs("File error", stderr);
		exit(1);
	}

	fseek(fptr, 0, SEEK_END);
	fileSize = ftell(fptr);
	rewind(fptr);
	if ((fileSize % BUF) != 0)
	{
		packetNum = fileSize / BUF + 1;
	}
	else
	{
		packetNum = fileSize / BUF;
	}
	//packetNum = ceil(fileSize / (float)BUF);

	char **buffer = (char **)malloc(packetNum * sizeof(char *));
	int i = 0;
	for (i = 0; i < packetNum - 1; i++)
	{
		buffer[i] = (char *)malloc(BUF * sizeof(char));
		InPacket = fread(buffer[i], 1, BUF, fptr);
	}
	fileOffset = fileSize - (packetNum - 1) * BUF;
	buffer[i] = (char *)malloc(fileOffset * sizeof(char));
	InPacket = fread(buffer[i], 1, fileOffset, fptr);

	printf("------------------------------------------------------------------\n");
	printf("Number of packets to be sent: %ld   | 	File size: %ld\n", fileSize, InPacket);
	printf("------------------------------------------------------------------\n");

	packetI.offset = 0;
	packetI.file_size = fileSize;
	packetI.num = BUF;
	send_buffer = (unsigned char *)malloc(sizeof(struct packetI));

	if (sendto(sockfd, fileName, sizeof(fileName), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
	{
		fprintf(stderr, "Error: failure connect to the server\n");
		exit(-1);
	}

	memset(send_buffer, 0, sizeof(struct packetI));
	memcpy(send_buffer, (const unsigned char *)&packetI, sizeof(packetI));

	if (sendto(sockfd, send_buffer, sizeof(packetI), 0, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		fprintf(stderr, "Error: failure connect to the server\n");
		exit(-1);
	}

	free(send_buffer);

	int send_counter = 0;
	struct timeval timeOut = {0, 10000};
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeOut, sizeof(struct timeval));

	struct timeval start_time;
	gettimeofday(&start_time, NULL);
	double total;

	while (1)
	{
		if (recvfrom(sockfd, buffer1, sizeof(struct packetI), 0, (struct sockaddr *)&clientAddress, &length) > 0)
		{
			ack1 = (struct packetA *)buffer1;
			if (ack1->offset == 4)
			{
				break;
			}
		}
		else
		{
			for (send_counter = 0; send_counter < 1; send_counter++)
			{
				packetI.offset = 0;
				packetI.file_size = fileSize;
				packetI.num = BUF;
				send_buffer = (unsigned char *)malloc(sizeof(struct packetI));
				memset(send_buffer, 0, sizeof(struct packetI));
				memcpy(send_buffer, (const unsigned char *)&packetI, sizeof(packetI));
				if (sendto(sockfd, send_buffer, sizeof(packetI), 0, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
				{
					fprintf(stderr, "Error: failure connect to the server\n");
					exit(-1);
				}
				free(send_buffer);
			}
		}
	}

	packetSent = (unsigned char *)malloc(sizeof(struct packetLoss));
	while (1)
	{
		if (state_ch == 1)
		{
			int send_count = 0;
			for (send_count = 0; send_count < packetNum - 1; send_count++)
			{
				if (ack1->index[send_count] == 0)
				{
					packet.offset = 1;
					packet.ID = send_count;
					memcpy(packet.data, buffer[send_count], BUF);
					memset(packetSent, 0, sizeof(struct packetLoss));
					memcpy(packetSent, (const unsigned char *)&packet, sizeof(packet));
					if (sendto(sockfd, packetSent, sizeof(packet), 0, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
					{
						fprintf(stderr, "Error: failure connect to the server\n");
						exit(-1);
					}
				}
			}

			state_ch = 0;
			packet.offset = 2;
			packet.ID = -1;
			memset(packetSent, 0, sizeof(struct packetLoss));
			memcpy(packetSent, (const unsigned char *)&packet, sizeof(packet));
			if (sendto(sockfd, packetSent, sizeof(packet), 0, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
			{
				fprintf(stderr, "Error: failure connect to the server\n");
				exit(-1);
			}
		}

		else
		{
			n = recvfrom(sockfd, buffer1, sizeof(struct packetA), 0, (struct sockaddr *)&clientAddress, &length);
			ack1 = (struct packetA *)buffer1;
			if (n > 0)
			{

				if (ack1->offset == 2)
				{

					state_ch = 1;
				}

				else if (ack1->offset == 3)
				{

					break;
				}
			}

			else
			{

				packet.offset = 2;
				packet.ID = -1;
				memset(packetSent, 0, sizeof(struct packetLoss));
				memcpy(packetSent, (const unsigned char *)&packet, sizeof(packet));
				if (sendto(sockfd, packetSent, sizeof(packet), 0, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
				{
					fprintf(stderr, "Error: failure connect to the server\n");
					exit(-1);
				};
			}
		}
	}
	while (1)
	{
		packet.offset = 6;
		packet.ID = packetNum - 1;
		memcpy(packet.data, buffer[packetNum - 1], BUF);
		memset(packetSent, 0, sizeof(struct packetLoss));
		memcpy(packetSent, (const unsigned char *)&packet, sizeof(packet));
		if (sendto(sockfd, packetSent, sizeof(packet), 0, (const struct sockaddr *)&serverAddress, length) < 0)
		{
			fprintf(stderr, "Error: failure connect to the server\n");
			exit(-1);
		}
		if (recvfrom(sockfd, buffer1, sizeof(struct packetA), 0, (struct sockaddr *)&clientAddress, &length) > 0)
		{
			struct packetA *ack_packet3 = (struct packetA *)buffer1;
			if (ack_packet3->offset == 5 || ack_packet3->offset == 3)
			{
				printf("all packets sent \n");
				break;
			}
		}
	}

	struct timeval end_time;
	gettimeofday(&end_time, NULL);
	long elap = calTime(end_time, start_time);
	printf("It takes %ld ms to transfer the file\n", elap);

	close(sockfd);
	fclose(fptr);
	free(buffer);
	return 0;
}
