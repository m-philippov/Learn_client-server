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
#include <ctype.h>

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

    //Above is the required tcp sceleton, now the code for Haamming Code starts
    int datatrec[10];
    int test[10];
    int p, p1, p2, p4;
    printf("Please data received is :\n");
    for (int i = 0; i < 7; i++) {
        n = read(sockfd, &data[i], sizeof(int));
        if(n < 0) {
            error("Error read receiving data");
        }
        printf("%d", datatrec[i]);
    }

    //test с изменением бита
    print("Plese enter the data to be tested\n");
    for(int i = 0; i < 7; i++) {
        scanf("%d", &test[i]);
    }

    //Узнаем ошибку
    p1 = test[6]^test[4]^test[2]^test[0];
    p2 = test[5]^test[4]^test[1]^test[0];
    p4 = test[3]^test[2]^test[1]^test[0];
    p = 4*p4 + 2*p2 + p1;
    
    printf("The data for testing is :");
    for (int i = 0; i < 7; i++) {
        printf("%d ", test[i]);
    }
    //исправляем ошибку
    if (p == 0) {
        printf("No Error");
    } else {
        printf("\nThe error is at position %d", p);
        printf("The correct data is : ");
        if (test[7 - p] == 0) {
            test[7 - p] = 1;
        } else {
            test[7 - p] = 0;
        }
        for (int i = 0; i < 7; i++) {
            printf("%d ", test[i]);
        }
    }

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
    close(sockfd);
    return 0;
}