#include "../include/initRoutingProtocol.h"
#include "../include/arrayConvert.h"
#include "../include/userHost.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/types.h>
#include <sys/unistd.h>

int initRouterModel(unsigned int portNumber, char ipAddress[16], RPAddress rpAddress, routerModel * rm)
{
    memset(&(rm->homeHost), 0, sizeof(struct sockaddr_in));
    rm->homeHost.sin_family = AF_INET;
    rm->homeHost.sin_port = htons(portNumber);

    if((rm->homeHost.sin_addr.s_addr = inet_addr(ipAddress)) == 0)
    {
        return -1;
    }

    if(!rpAddress)
    {
        strcpy(rm->routerAddress, "000.000");
        return 0;
    }

    strcpy(rm->routerAddress, rpAddress);
    setUserNumber(0, rm->routerAddress);
    memset(rm->userHosts, 0, sizeof(struct sockaddr_in)*MAXUSERS);
    memset(rm->routerHosts, 0, sizeof(struct sockaddr_in)*MAXROUTERS);
    memset(rm->users, 0, sizeof(unsigned char)*MAXUSERS);
    memset(rm->routerTable, 0, sizeof(unsigned char)*MAXROUTERS*MAXROUTERS);
    memset(rm->sendTableBuffer, 0, ROUTERBUFFER);
    memset(rm->sendTPBuffer, 0, CONVBUFFSIZETP);
    memset(rm->recieveBuffer, 0, ROUTERBUFFER);


    pthread_mutex_init(&(rm->routerTableMutex), NULL);

    if((rm->socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Socket error. ");
        return -8;
    }

    if(bind(rm->socket, (struct sockaddr *)&(rm->homeHost), (socklen_t)sizeof(struct sockaddr_in)) == -1)
    {
        perror("Binding error.");
        return -5;
    }
}

int initUserModel(unsigned int portNumber, char ipAddress[16], userModel * um)
{
    if((um->socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        return -3;
    }

    um->userHost.sin_family = AF_INET;
    um->userHost.sin_port = htons(portNumber);

    if(inet_aton(ipAddress, &(um->userHost.sin_addr)) == 0)
    {
        return -7;
    }

    memset(&(um->homeHost), 0, sizeof(struct sockaddr_in));
    memset(um->receiveTPBuffer, 0, CONVBUFFSIZETP);
    memset(um->sendTPBuffer, 0, CONVBUFFSIZETP);
    strcpy(um->userAddress, "000.000");

    if(bind(um->socket, (struct sockaddr *)&(um->userHost), (socklen_t)sizeof(struct sockaddr_in)) == -1)
    {
        return -1;
    }

    return 0;
}

int connectRouterModelToNetwork(routerModel * sourceRouter, routerModel * destinationRouter)
{
    transferPackage tp;
    memset(&tp, 0, sizeof(transferPackage));

    tp.packageType = 2;
    tp.dataSent = 0;
    tp.currentNode = 0;

    size_t len = sizeof(struct sockaddr_in);
    strcpy(tp.sourceAddress, sourceRouter->routerAddress);
    strcpy(tp.destinationAddress, destinationRouter->routerAddress);

    convertPackageToArray(&tp, sourceRouter->sendTPBuffer);

    if(sendto(sourceRouter->socket, sourceRouter->sendTPBuffer, CONVBUFFSIZETP, 0, (struct sockaddr *)&(destinationRouter->homeHost), len) == -1)
    {
        perror("Router connection error.");
        return -1;
    }

    return 0;
}

int connectUserModelToNetwork(userModel * user, routerModel * destinationRouter)
{
    transferPackage tp;
    memset(&tp, 0, sizeof(transferPackage));

    tp.packageType = 1;
    tp.dataSent = 0;
    tp.currentNode = 0;

    strcpy(tp.destinationAddress, destinationRouter->routerAddress);
    strcpy(tp.sourceAddress, user->userAddress);

    user->homeHost.sin_addr.s_addr = destinationRouter->homeHost.sin_addr.s_addr;
    user->homeHost.sin_family = destinationRouter->homeHost.sin_family;
    user->homeHost.sin_port = destinationRouter->homeHost.sin_port;

    convertPackageToArray(&tp, user->sendTPBuffer);

    if(sendto(user->socket, user->sendTPBuffer, CONVBUFFSIZETP, 0, (struct sockaddr *)& (user->homeHost), sizeof(struct sockaddr_in)) == -1)
    {
        perror("User conection error.");
        return -1;
    }

    recieveTPfromRouter(&tp, user);
    strcpy(user->userAddress, tp.sourceAddress);

    return 0;
}

int closeUserModel(userModel * um)
{
    close(um->socket);
}

int closeRouterModel(routerModel * rm)
{
    close(rm->socket);
    pthread_mutex_destroy(&rm->routerTableMutex);
}