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
    //Above is the required tcp sceleton, now the code for Haamming Code starts
    int data[10];
    printf("Please input 4 bits of data\n");
    scanf("%d", &data[0]);
    scanf("%d", &data[1]);
    scanf("%d", &data[2]);
    scanf("%d", &data[4]);

    //Calculation of even parity for encoding
    data[6] = data[4]^data[2]^data[0];
    data[5] = data[4]^data[1]^data[0];
    data[3] = data[2]^data[1]^data[0];

    //Sending the encoded data to clients
    for (int i = 0; i < 7; i++) {
        n = write(newsockfd, &data[i], sizeof(int));
        if(n < 0) {
            printf("Error while transmitting the data");
        }
    }
    printf("The data sent is: ");
    for (int i = 0; i < 7; i++) {
        printf("%d", data[i]);
    }
    

    //окончание работы
    close(newsockfd);
    close(sockfd);
    return 0;
}