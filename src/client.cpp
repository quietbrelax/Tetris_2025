#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstdint>
#include <cstdio>  
#include <stdio.h>

#define BUFSZ 1024

int main() {
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        return 1; 
    }

    uint16_t port = 8000;
    char ip[] = "127.0.0.1";
    char buf[BUFSZ];

    SOCKET sock;
    struct sockaddr_storage Sstorage;
    char addrstr[BUFSZ];

    struct sockaddr_in *serverAddr = (struct sockaddr_in *)&Sstorage;

    /* //Atributos
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_port = htons(port); 
    inet_pton(AF_INET, ip, &(serverAddr->sin_addr));
    memset(serverAddr->sin_zero, 0, sizeof(serverAddr->sin_zero));  // Zera padding

    // Criação do socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return 1; 
    } */

    // Tratativa endereço
    struct in_addr addr4;
    struct in6_addr addr6;

    //IPV4
    if (inet_pton(AF_INET, ip, &addr4)) {
        struct sockaddr_in *serverAddr = (struct sockaddr_in *)&Sstorage;
        //Atributos
        serverAddr->sin_family = AF_INET;
        serverAddr->sin_port = htons(port); 
        inet_pton(AF_INET, ip, &(serverAddr->sin_addr));
        memset(serverAddr->sin_zero, 0, sizeof(serverAddr->sin_zero));

        // Criação do socket
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            WSACleanup();
            return 1; 
        }

    }
    //IPV6
    if (inet_pton(AF_INET6, ip, &addr6)) {
        struct sockaddr_in6 *serverAddr = (struct sockaddr_in6 *)&Sstorage;

        //Atributos
        serverAddr->sin6_family = AF_INET6;
        serverAddr->sin6_port = htons(port); 
        inet_pton(AF_INET, ip, &(serverAddr->sin6_addr));

	    // Criação do socket
        sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            WSACleanup();
            return 1; 
        }
    }    

    if(connect(sock, (struct sockaddr*)&Sstorage, sizeof(Sstorage)) == SOCKET_ERROR) {
        printf("Conexão falhou: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    memset(buf, 0, BUFSZ);
    recv(sock, buf, BUFSZ, 0);

	if(strcmp(buf, "Ok") != 0){
		printf("Conexão falhou!");
        return 0;
	}

    while (1)
    {
        /* code */
    }
    
    printf("Conectado ao servidor!");
    closesocket(sock);
    WSACleanup();
    return 0;
}
