#include <unistd.h>
#include <stdio.h>

#include "../include/userHost.h"
#include "../include/initRoutingP.h"
#include "../include/utility.h"

void printTPackage(transferPackage * tp) {
    printf("Transfer package specifications: \n");
    printf("Source address %s\n", tp->sourceAddress);
    printf("Destination address %s\n", tp->destinationAddress);
    printf("Data sent: %d\n", tp->dataSent);
    printf("Data: %s\n", tp->data);
    printf("Package type: %d\n", tp->packageType);
    printf("Current node number: %d\n", tp->currentNode);
    printf("Number of nodes: %d\n", tp->nodeNum);
    printf("Path: ");
    int i;
    for(i = 0; i < MAXROUTERS; i++) {
        printf("%d ", tp->path[i]);
    }
    printf("\n");
}

int sendTPtoRouter(transferPackage * tp, userModel * um)
{
    tp->packageType = 0;
    tp->nodeNum = 0;
    tp->currentNode = 0;

    convertTPPackageToArray(tp, um->sendTPBuffer);

    if(sendto(um->socket, um->sendTPBuffer, CONVBUFFSIZETP, 0, (struct sockaddr *)&(um->homeHost), sizeof(struct sockaddr_in)) == -1)
    {
        perror("Error sending package from User to Router.");
        return -1;
    }

    return 0;
}

int recieveTPfromRouter(transferPackage * tp, userModel * um)
{
    size_t len = sizeof(struct sockaddr_in);

    if(recvfrom(um->socket, um->recieveTPBuffer, CONVBUFFSIZETP, 0, (struct sockaddr*)*(um->homeHost), (socklen_t*)&len) == -1)
    {
        perror("Error recieving package from Router to User.");
        return -1;
    }

    convertArrayToPackage(um->recieveTPBuffer, tp);
    printf("Package recieved: \n");
    printPackage(tp);

    return 0;
}