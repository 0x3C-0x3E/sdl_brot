#pragma once
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include <pthread.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024


int init_wsa();

int init_client();

int start_client(const char * server_ip, int port);

void * revc_thread(void * arg);

void send_message();

void set_message(const char * message);

void stop_client();

extern bool client_running;

extern WSADATA wsa_data;

extern SOCKET client_socket;

extern struct sockaddr_in server;

extern const char * SERVER_IP;

extern int PORT;

extern char recv_buffer[BUFFER_SIZE];

extern bool recv_change;

extern char send_buffer[BUFFER_SIZE];
