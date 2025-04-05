#include "client.h"
#include <string.h>


bool client_running;

WSADATA wsa_data;

SOCKET client_socket;

struct sockaddr_in server;

const char * SERVER_IP;

int PORT;

char recv_buffer[BUFFER_SIZE];

bool recv_change = false;

char send_buffer[BUFFER_SIZE];


int init_wsa()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("Failed to initialize Winsock. Error: %d\n", WSAGetLastError());
		return 1;
	}

	return 0;
}


int init_client()
{
	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket == INVALID_SOCKET)
	{
		printf("Could not create socket. Error %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	// convert server ip addr from string to binary form
	if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr.s_addr) <= 0)
	{
		perror("Invalid server IP address");
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}

	if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Failed to connect. Error: %d\n", WSAGetLastError());
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}

	printf("[INFO] Connected to server %s:%d\n", SERVER_IP, PORT);

	return 0;
}

int start_client(const char * server_ip, int port)
{
	SERVER_IP = server_ip;
	PORT = port;

	if (init_wsa() == 1)
	{
		return 1;
	}

	if (init_client() == 1)
	{
		return 1;
	}

	
	int rc;

	pthread_t recv_t;

	rc = pthread_create(&recv_t, NULL, revc_thread, &recv_buffer);

	if (rc) {
		printf("Error creating thread\n");
		return 1; 
	}


	return 0;

}


void * revc_thread(void * arg)
{
	char * recv_buffer = (char * ) arg;

	strncpy(recv_buffer, (char *) arg, BUFFER_SIZE - 1);
	recv_buffer[BUFFER_SIZE - 1] = '\0';

	while (true)
	{
		char buffer[BUFFER_SIZE];

		int bytes_recv = recv(client_socket, buffer, BUFFER_SIZE, 0);
		if (bytes_recv > 0)
		{
			buffer[bytes_recv] = '\0';
		} else {
			printf("Server disconnected.\n");
		}

		memcpy(recv_buffer, buffer, BUFFER_SIZE);
		recv_change = true;

	}
	return NULL;
}

void send_message()
{
	char message[BUFFER_SIZE];
	memcpy(message, send_buffer, BUFFER_SIZE);
	send(client_socket, message, strlen(message), 0);
}

void set_message(const char * message)
{
	strcpy(send_buffer, message);
}


void stop_client()
{
	closesocket(client_socket);
	WSACleanup();
}