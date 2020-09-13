#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
//#include "linkedlist.h"

#define BUF 1400
#define MAX 2048

struct packetI
{
	uint8_t offset;
	u_int file_size;
	u_int num;
};

struct packetLoss
{
	uint8_t offset;
	int ID;
	unsigned char data[BUF];
};

struct packetA
{
	uint8_t offset;
	char index[20000];
};

int main(int argc, char *argv[])
{
	FILE *fptr;
	int sockfd, length, n;
	socklen_t fromlen;
	struct sockaddr_in serverAddress, clientAddress;
	char buf[63000];
	char send_buffer[10000];
	int packetNum, fileOffset;
	bool init_check = 0, mem_alloc_ch = 0, data_check = 0;
	int cons_check = 0;
	int offset;
	struct packetA ack;
	int packet_counter = 0;
	char fileName[20];

	int opt;
	int portNum = 0;
	while ((opt = getopt(argc, argv, "d:p:f:")) != -1)
	{
		switch (opt)
		{

		case 'p':
			serverAddress.sin_port = htons(atoi(optarg));
			portNum = atoi(optarg);
			break;

		case ':':
			fprintf(stderr, "malformed command\nOption -%c requires an aperand\n", optopt);
			exit(-1);
			break;
		case '?':
			printf("unknown option: %c\n", optopt);
			fprintf(stderr, "Usage: %s [-p port_number] \n", argv[0]);
			exit(-1);
			break;
		}
	}
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		fprintf(stderr, "Error: creating socket\n");
		exit(-1);
	}

	if (bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		fprintf(stderr, "Error: creating socket\n");
		exit(-1);
	}

	fromlen = sizeof(struct sockaddr_in);

	int file_sz;
	int send_counter = 0;
	ssize_t numRead;

	printf("waiting at port: %d\n", portNum);

	while (1)
	{
		if ((numRead = recvfrom(sockfd, fileName, sizeof(fileName), 0, (struct sockaddr *)&clientAddress, &fromlen)) == -1)
		{
			fprintf(stderr, "filename is incorrect, please try agian\n");
			exit(-1);
		}
		printf("Receiving file: %s\n", fileName);

		if (recvfrom(sockfd, buf, sizeof(struct packetI), 0, (struct sockaddr *)&clientAddress, &fromlen) < 0)
		{
			fprintf(stderr, "Error: creating socket\n");
			exit(-1);
		}

		struct packetI *recv_init_packet = (struct packetI *)buf;
		if (recv_init_packet->offset == 0)
		{
			init_check = 1;
			file_sz = recv_init_packet->file_size;
			break;
		}
	}
	packetNum = ceil(file_sz / (float)BUF);
	char **buffer = (char **)malloc(packetNum * sizeof(char *));
	int count = 0;
	for (count = 0; count < packetNum - 1; count++)
	{
		buffer[count] = (char *)malloc(BUF * sizeof(char));
	}
	fileOffset = file_sz - (packetNum - 1) * BUF;
	buffer[count] = (char *)malloc(fileOffset * sizeof(char));

	ack.offset = 4;
	memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));
	if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, fromlen) < 0)
	{
		fprintf(stderr, "Error: creating socket\n");
		exit(-1);
	}

	mem_alloc_ch = 1;

	while (1)
	{
		n = recvfrom(sockfd, buf, BUF, 0, (struct sockaddr *)&clientAddress, &fromlen);
		struct packetLoss *packet = (struct packetLoss *)buf;
		offset = packet->offset;
		/*
		switch (offset)
		{
		case 0:
			if (1)
			{
				ack.offset = 4;
				memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));
				if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, fromlen) < 0)
				{
					fprintf(stderr, "Error: creating socket\n");
					exit(-1);
				};
			}
			break;
		case 1:
			if (1)
			{
				struct packetLoss *packet = (struct packetLoss *)buf;
				if (ack.index[packet->ID] <= 0)
				{

					memcpy(buffer[packet->ID], packet->data, BUF);
					ack.index[packet->ID] = 1;
					packet_counter++;
				}
			}

			break;
		case 2:
			if (1)
			{
				if (packet_counter >= (packetNum - 2))
				{
					ack.offset = 3;
					memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));
					if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, fromlen) < 0)
					{
						fprintf(stderr, "Error: creating socket\n");
						exit(-1);
					}
					break;
				}

				else
				{
					ack.offset = 2;
					memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));
					if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, fromlen) < 0)
					{
						fprintf(stderr, "Error: creating socket\n");
						exit(-1);
					}
				}
			}
			break;
		}
		*/

		if (offset == 0)
		{
			ack.offset = 4;
			memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));
			if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, fromlen) < 0)
			{
				fprintf(stderr, "Error: creating socket\n");
				exit(-1);
			};
		}

		else if (offset == 1)
		{

			struct packetLoss *packet = (struct packetLoss *)buf;
			// process only if we haven't received the packetLoss beforehand
			if (ack.index[packet->ID] <= 0)
			{

				memcpy(buffer[packet->ID], packet->data, BUF); // store
				ack.index[packet->ID] = 1;
				packet_counter++;
			}
		}

		else if (offset == 2)
		{
			if (packet_counter >= (packetNum - 2)) //
			{
				ack.offset = 3;
				memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));
				if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, fromlen) < 0)
				{
					fprintf(stderr, "Error: creating socket\n");
					exit(-1);
				}
				break;
			}

			else
			{
				ack.offset = 2;
				memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));
				if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, fromlen) < 0)
				{
					fprintf(stderr, "Error: creating socket\n");
					exit(-1);
				}
			}
		}
	}

	while (1)
	{
		n = recvfrom(sockfd, buf, fileOffset, 0, (struct sockaddr *)&clientAddress, &fromlen);
		struct packetLoss *packet = (struct packetLoss *)buf;

		if (packet->offset == 6)
		{
			memcpy(buffer[packet->ID], packet->data, fileOffset - 1);

			for (int i = 0; i < 10; i++)
			{
				ack.offset = 5;
				memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));
				if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, fromlen) < 0)
				{
					fprintf(stderr, "Error: creating socket\n");
					exit(-1);
				}
			}
			break;
		}

		else if (packet->offset == 2)
		{
			ack.offset = 3;
			memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));
			if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, fromlen) < 0)
			{
				fprintf(stderr, "Error: creating socket\n");
				exit(-1);
			}
		}
	}

	fptr = fopen(fileName, "wb");
	int i = 0;
	for (i = 0; i < packetNum - 1; i++)
	{
		fwrite(buffer[i], sizeof(char), BUF, fptr);
	}
	fwrite(buffer[i], sizeof(char), fileOffset, fptr);

	fclose(fptr);
	close(sockfd);

	return 0;
}
