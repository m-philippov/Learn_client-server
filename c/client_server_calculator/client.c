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

    int num1, num2, choice, ans;
    //NUM1
S:  bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
        error("error reading from socket");
    }
    printf("Server - %s\n", buffer);
    scanf("%d", &num1);
    write(sockfd, &num1, sizeof(int));
    //NUM2
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
        error("error reading from socket");
    }
    printf("Server - %s\n", buffer);
    scanf("%d", &num2);
    write(sockfd, &num2, sizeof(int));
    //CHOICE
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
        error("error reading from socket");
    }
    printf("Server - %s\n", buffer);
    scanf("%d", &choice);
    write(sockfd, &choice, sizeof(int));
    
    if (choice == 5) {
        goto Q;
    } else {
        read(sockfd, &ans, sizeof(int));
        printf("Server - The answer is: %d\n", ans);
        goto S;
    }
    
    //окончание работы
Q:  printf("You have selected to exit. Exit successful")
    close(sockfd);
    return 0;
}