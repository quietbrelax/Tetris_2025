#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstdint>
#include <cstdio>  

#define Max 3
#define BUFSZ 1024

int main(){
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        return 1; 
    }

    uint16_t port = 8000;
    int total = 0; // total de sockets conectados
    char buf[BUFSZ]; //buffer de mensagem

    SOCKET sock;
    struct sockaddr_in6 addr;
    socklen_t addr_size;

    // Criação do socket
    sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return 1; 
    }

    // Reutilização da porta
    int enable = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(int));

    //Endereço IPV6 também aceitar IPV4
    int ipv6only = 0;
    setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ipv6only, sizeof(ipv6only));

    //Atributos
    addr.sin6_family = AF_INET6;       
    addr.sin6_addr = in6addr_any;      
    addr.sin6_port = htons(port);  

    // Vinculação
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Bind falhou: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    // Escuta
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen falhou: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Trataiva para função select
    fd_set fds, readfds; // File descriptors monitorados pelo select()
    FD_ZERO(&fds); // limpa os file descriptors
    FD_SET(sock, &fds); // Adiciona o socket do cliente aos file descriptors
    int fdmax = sock;
    
    int fila[Max]; // fila de sockets para conexão
    for (int i = 0; i <= Max; i++)
    {
	    fila[i] = 0;
    }

    fila[0] = sock;
    total++;

    while(1){
        // Cópia dos sockets para overwrite
        readfds = fds;
        printf("Aguardando select...\n");
        if(select(fdmax + 1, &readfds, NULL, NULL, NULL) == -1){ // retorna os sockets prontos para leitura
            printf("Select falhou");
            closesocket(sock);
            return 0;
        }

        //Controle da fila de mensagens
        for (int fd = 0; fd < total; fd++){
            if(FD_ISSET(fila[fd],&readfds)){
               if(fd == 0){ // fd = 0, alguém está tentando se conectar
                    addr_size = sizeof(struct sockaddr_storage);
                    SOCKET fd_new;

                    fd_new = accept(sock, (struct sockaddr*)&addr, &addr_size);
                    if (fd_new == INVALID_SOCKET) {
                        printf("Accept falhou: %d\n", WSAGetLastError());
                        closesocket(sock);
                        WSACleanup();
                        return 1;
                    }

                    FD_SET(fd_new, &fds);
                    fila[total] = fd_new;
                    total++;
                    if (fd_new > fdmax){
                        fdmax = fd_new;
                    }

                    if(total <= Max){ // conexão aceita
                        const char *msg = "Ok";
                        send(fd_new, msg, (int)strlen(msg), 0);
                    }else{ // máximo de sockets atingido
                        printf("Client limit exceeded\n");
                        const char *msg = "ERROR";
                        send(fd_new, msg, (int)strlen(msg), 0);
                        total--;
                        FD_CLR(fd_new, &fds);
                    }
                }
                else{ //Mensagens
                }
            }
        }
    }
    closesocket(sock);
    WSACleanup();
    return 0;
}