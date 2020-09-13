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
#include "linkedlist.h"

int main(int argc, char *argv[])
{
	FILE *fptr;
	int sockfd, n;
	socklen_t length;
	struct sockaddr_in serverAddress, clientAddress;
	char buf[63000];
	char send_buffer[10000];
	int packetNum, fileOffset;

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

	length = sizeof(struct sockaddr_in);

	int fileSize;
	ssize_t numRead;

	printf("waiting at port: %d\n", portNum);

	while (1)
	{
		if ((numRead = recvfrom(sockfd, fileName, sizeof(fileName), 0, (struct sockaddr *)&clientAddress, &length)) == -1)
		{
			fprintf(stderr, "filename is incorrect, please try agian\n");
			exit(-1);
		}
		printf("Receiving file: %s\n", fileName);

		if (recvfrom(sockfd, buf, sizeof(struct packetI), 0, (struct sockaddr *)&clientAddress, &length) < 0)
		{
			fprintf(stderr, "Error: creating socket\n");
			exit(-1);
		}

		struct packetI *temp = (struct packetI *)buf;
		if (temp->offset == 0)
		{
			fileSize = temp->file_size;
			break;
		}
	}
	if ((fileSize % BUF) != 0)
	{
		packetNum = fileSize / BUF + 1;
	}
	else
	{
		packetNum = fileSize / BUF;
	}
	char **buffer = (char **)malloc(packetNum * sizeof(char *));
	int count = 0;
	for (count = 0; count < packetNum - 1; count++)
	{
		buffer[count] = (char *)malloc(BUF * sizeof(char));
	}
	fileOffset = fileSize - (packetNum - 1) * BUF;
	buffer[count] = (char *)malloc(fileOffset * sizeof(char));

	ack.offset = 4;
	memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));

	if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, length) < 0)
	{
		fprintf(stderr, "Error: creating socket\n");
		exit(-1);
	}

	while (1)
	{
		n = recvfrom(sockfd, buf, BUF, 0, (struct sockaddr *)&clientAddress, &length);
		struct packetLoss *packet = (struct packetLoss *)buf;
		offset = packet->offset;

		if (offset == 0)
		{
			ack.offset = 4;
			memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));
			if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, length) < 0)
			{
				fprintf(stderr, "Error: creating socket\n");
				exit(-1);
			};
		}

		else if (offset == 1)
		{

			struct packetLoss *packet = (struct packetLoss *)buf;

			if (ack.index[packet->ID] <= 0)
			{
				memcpy(buffer[packet->ID], packet->data, BUF);
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
				if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, length) < 0)
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
				if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, length) < 0)
				{
					fprintf(stderr, "Error: creating socket\n");
					exit(-1);
				}
			}
		}
	}

	while (1)
	{
		n = recvfrom(sockfd, buf, fileOffset, 0, (struct sockaddr *)&clientAddress, &length);
		struct packetLoss *packet = (struct packetLoss *)buf;

		if (packet->offset == 6)
		{
			memcpy(buffer[packet->ID], packet->data, fileOffset - 1);

			for (int i = 0; i < 10; i++)
			{
				ack.offset = 5;
				memcpy(send_buffer, (unsigned char *)&ack, sizeof(ack));
				if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, length) < 0)
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
			if (sendto(sockfd, send_buffer, sizeof(ack), 0, (struct sockaddr *)&clientAddress, length) < 0)
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
