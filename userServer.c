#include <stdio.h>
#include "include/initRoutingP.h"
#include "include/userHost.h"
#include <string.h>
#include <pthread.h>

typedef struct recieveStruct_t 
{
    transferPackage * tp;
    userModel * um;
}recieveStruct;

void threadRecievePacketFromRouter(recieveStruct * rs)
{
    while(1)
    {
        recievePackageFromRouter(rs -> tp, rs -> um);
    }
}

void initUser(userModel * uModel)
{
    char ip_address[100];
    unsigned short port;
    while(1)
    {
        printf("Enter user's IP address: \n");
        scanf("%s", ip_address);
        printf("Enter port number: \n");
        scanf("%hu", &port);
        getchar();
        
        if(initUserModel(port, ip_address, uModel) == -7)
        {
            printf("Error. Invalid IP address.\n");
            continue;
        }

        break;
    }

    printUserModel(uModel);
}

int main(int argc, char * argv[])
{
    int c;

    userModel currentUser;
    routerModel destinationRouter;
    transferPackage tp_recv;
    transferPackage tp;

    unsigned short port;
    char ip_address[100];

    memset(&tp, 0, sizeof(transferPackage));
    memset(&tp_recv, 0, sizeof(transferPackage));

    recieveStruct recieve;
    recieve.tp = &tp_recv;
    recieve.um = &currentUser;

    pthread_t recieveDataThread;

    initUser(&currentUser);
    system("clear");

    while(1)
    {
        switch(c)
        {
            case 0:
                printf("Choose option:\n");
                printf("1. Reinitialize user.\n");
                printf("2. Connect user to router from network.\n");
                printf("3. Send data.\n");
                scanf("%d", &c);
                continue;
                break;
            case 1:
                pthread_kill(&recieveDataThread, 0);
                closeUser(&currentUser);
                initUser(&currentUser);
                break;
            case 2:
                printf("Enter destination IP address: \n");
                scanf("%s", ip_address);
                printf("Enter destination port: \n");
                scanf("%hu", &port);

                if(initRouterModel(port, ip_address, NULL, &destinationRouter) == -1)
                {
                    printf("Error. Invalid router IP address.");
                    continue;
                }

                connectUserModelToNetwork(&currentUser, &destinationRouter);

                pthread_create(&recieveDataThread, 0, (void*)threadRecieve);

                break;
            case 3:
                printf("Enter data (max %d): \n", CONVBUFFSIZETP);
                scanf("%s", tp.data);
                getchar();
                printf("Enter destination address:\n");
                scanf("%s", tp.destinationAddress);
                getchar();

                srtcpy(tp.sourceAddress, currentUser.userAddress);
                tp.dataSent = strlen(tp.data) + 1;
                sendTPtoRouter(&tp, &currentUser);

                break;
            default:
                printf("Incorrect option.\n");
                break;
        }

        c = 0;
    }
    return 0;
}