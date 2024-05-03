#include <stdio.h>
#include <unistd>
#include <stdlib.h>
#include <string.h>
//чтение файлов запись и закрытие
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//структура хост-сервера
#include <netdb.h>

//функция ошибок
void error(const char* msg) {
    perror(msg);
    exit(1);
}
//параметры имя файла, ip_server и номер порта
int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "uncorrect number of parametr");
        exit(1);
    }
    //файловый дескриптор, номер порта, проверки
    int sockfd, portno, n;
    struct hostent *server;
    
    //буффер
    char buffer[255];
    //серверная структура
    struct sockaddr_in serv_addr;
    
    //create socket
    portno = aroi(argv[2]); //номер порта
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening Socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host");
    }
    bzero((char*)&serv_addr, sizeof(serv_addr));
    //определяем параметры
    serv_addr.sin_family = AF_INET;
    //копирует данные для передачи
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_lenght);

    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))< 0) {
        error("Connection Failed");
    }    

    while (1) {
        bzero(buffer, 255);
        fgerts(beffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            error("Error on writing");
        }
        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            error("Error on reading");
        }
        printf("Server: %s", buffer);

        int i = strncmp("Bye", buffer, 3);
        if(i == 0) {
            break;
        }

    }
    
    
    //окончание работы
    close(sockfd);
    return 0;
}