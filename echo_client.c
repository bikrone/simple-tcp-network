/*
 * echo_client.c
 *
 *  Created on: May 28, 2015
 *      Author: khanh
 */

#include "library/socket_helper.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
    // socket file descriptor
        int socket_fd;

        // data received from server
        char data_received[MAXLINE+1], data_send[MAXLINE+1];
        int data_received_length, data_send_length;

        // server address
        struct sockaddr_in server_address;

        // server ip, default: localhost
        char *server_ip = "127.0.0.1";
        // server port, default: 1337
        char *server_port = "1337";

        if (argc > 1) {
    	server_ip = argv[1];
    	if (argc > 2) {
    	    server_port = argv[2];
    	}
        }

        // generate sockaddr_in server_address from server_ip
        bzero(&server_address, sizeof server_address);
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(atoi(server_port));

        if (inet_pton(server_address.sin_family, server_ip, &server_address.sin_addr) <=0) {
    	err_quit("inet_pton error for %s\n", server_ip);
        }

        // create socket file descriptor
        socket_fd = Socket(server_address.sin_family, SOCK_STREAM, 0);
        if (socket_fd < 0) {
    	err_sys("socket error");
        }

        if (connect(socket_fd, (SA *)&server_address, sizeof server_address)< 0) {
    	err_sys("connect error");
        }
        while (fgets(data_send, MAXLINE, stdin) != NULL) {
            if (strcmp(data_send, "quit") == 0) exit(0);
            data_send_length = strlen(data_send);
            writen(socket_fd, data_send, data_send_length);

            if ((data_received_length = read(socket_fd, data_received, data_send_length))>0) {
        	data_received[data_received_length] = 0; // mark end of string
        	if (fputs(data_received, stdout) == EOF) {
        	    err_sys("fputs error");
        	}
            }
        }


        if (data_received_length < 0) {
    	err_sys("read error");
        }

        exit(0);
}
