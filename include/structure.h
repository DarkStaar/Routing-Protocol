#ifndef STRUCTURE
#define STRUCTURE
#include<stdio.h>      //printf
#include<string.h>     //strlen
#include<sys/socket.h> //socket
#include<arpa/inet.h>  //inet_addr
#include <fcntl.h>     //for open
#include <unistd.h>    //for close
#define MAXUSERS 100
#define MAXROUTERS 100
#define PACKAGE 1000
#define ROUTERBUFFER MAXROUTERS*MAXROUTERS

typedef unsigned char IPAddress[17];


typedef struct modelRouter{
    IPAddress routerAddress;

    struct sockaddr_in routerHost[MAXROUTERS];
    struct sockaddr_in routerHomeHost;

    unsigned char numUsers[MAXUSERS];
    struct sockaddr_in userHost[MAXUSERS];
    int socket;

    unsigned char recieveBuffer[ROUTERBUFFER];
    unsigned char sendTBuffer[ROUTERBUFFER];
    unsigned char sendTProtocolBuffer[ROUTERBUFFER];

    unsigned char routerTable[MAXROUTERS][MAXROUTERS];
    pthread_mutex_t routerTableThread;


}modelRouter;

typedef struct modelUser{
    IPAddress userAddress;

    struct sockaddr_in userhost[MAXROUTERS];
    struct sockaddr_in homeHost;

    unsigned char sendTProtocolBuffer[ROUTERBUFFER];
    unsigned char recieveTProtocolBuffer[ROUTERBUFFER];
    int socket;

}modelUser;

typedef struct packageTransfer(){
    IPAddress destAddress;
    IPAddress sourAddress;

    char data[PACKAGE];
    unsigned char path[MAXROUTERS];
    unsigned char nodeNumber;
    unsigned char onNode;
    unsigned int dataSent;
    char packageType;
}packageTransfer;


int createRouter(char routerNum, char * address);

int createUser(char userNum, char * address);

char getRouter(char * address);

char getUser(char * address);

#endif