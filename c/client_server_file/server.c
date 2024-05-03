#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//функция ошибок
void error(const char* msg) {
    perror(msg);
    exit(1);
}
//параметры имя файла и номер порта
int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "uncorrect number of parametr");
        exit(1);
    }
    //файловый дескриптор, номер порта, проверки
    int sockfd, newsockfd, portno, n;
    //буффер
    char buffer[255];
    //серверная и клиетская структуры
    struct sockaddr_in serv_addr, cli_addr;
    // указывает интернет адрес
    socklen_t clilen;
    //create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening Socket");
    }

    //очистка данных
    bzero((char*)&serv_addr, sizeof(serv_addr));
    portno = aroi(argv[1]); //номер порта

    //получение данных
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //привязка
    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Binding Failed")
    }
    //прослушивание
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    //подключение
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    if (newsockfd < 6) {
        error("Error on Accept");
    }
    //работа с файлом
    FILE *fp;

    int ch = 0;
    fp = fopen("glad_received.txt", "a");
    
    int words;
    read(newsockfd, &words, sizeof(int));

    while(ch != words) {
        read(newsockfd, buffer, 255);
        fprintf(fp, "%s ", buffer);
        ch++;
    }
    printf("The file has been reseived! It saved by the name glad_reseived");


    //окончание работы
Q:    close(newsockfd);
    close(sockfd);
    return 0;
}