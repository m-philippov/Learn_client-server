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
    //калькулятор     
    int num1, num2, ans, choice;
    //связь клиента и сервера

    //работа с числом 1
S:    n = write(newsockfd, "Enter Number 1 :", strlen("Enter Number 1"));
    if (n < 0) {
        error("Error writing to socket");
    }
    read(newsockfd, &num1, sizeof(int));
    printf("Client - Number 1 is : %d\n", num1);

    //работа с числом 2
    n = write(newsockfd, "Enter Number 2 :", strlen("Enter Number 2"));
    if (n < 0) {
        error("Error writing to socket");
    }
    read(newsockfd, &num2, sizeof(int));
    printf("Client - Number 2 is : %d\n", num2);

    n = write(newsockfd, "Enter your choice : \n1. Addition\n2. Substraction \n3. Multiply\n4. Division\n5. Exit\n", strlen("Enter your choice : \n1. Addition\n2. Substraction \n3. Multiply\n4. Division\n5. Exit\n"));
    if (n < 0) {
        error("Error writing to socket");
    }
    read(newsockfd, &num2, sizeof(int));
    printf("Client - Choise is : %d\n", choice);

    switch (choice)
    {
    case 1:
        ans =  num1 + num2;
        break;
    case 2:
        ans =  num1 - num2;
        break;
    case 3:
        ans =  num1 * num2;
        break;
    case 4:
        if (num == 0) {
            error("Division By Zero");
        } else {
            ans =  num1 / num2;
        }
        break;
    case 5:
        goto Q;
        break;
    default:
        error("Uncorrect Number");
        break;
    }
    write(newsockfd, &ans, sizeof(int));
    goto S;



    //окончание работы
Q:    close(newsockfd);
    close(sockfd);
    return 0;
}