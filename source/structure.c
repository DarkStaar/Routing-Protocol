#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structure.h"


int setRouter(char routerNum, char *address){
    if(routerNum < 1 || routerNum >=MAXROUTERS) return -1;

    char num[2] = {routerNum/10, routerNum%10};

    address[0] = '1';
    address[1] = '9';
    address[2] = '2';
    address[3] = '.';
    address[4] = '1';
    address[5] = '6';
    address[6] = '8';
    address[7] = '.';
    address[8] = '0';
    address[9] = '.';
    address[10] = num[0];
    address[11] = num[1];
    address[12] = ':';
}

int setUser(char userNum, char *address){
    if(userNum < 1 || userNum >=MAXUSERS) return -1;

    char num[2] = {userNum/10, userNum %10};

    address[13] = '5';
    address[14] = '0';
    address[15] = num[0];
    address[16] = num[1];
}

unsigned char getRouterNumb(char * address) {
    char ret_val[13] = {'0', '0', '0', '0','0', '0', '0', '0','0', '0', '0', '0', '0'};
    for(int i = 0; i < 3; i++) {
        if(address[i] == '.') {
                ret_val[i] = 0;
                break;
        }
        ret_val[i] = address[i];
    }
    return (unsigned char)atoi(ret_val);
}

unsigned char getUserNumb(char * address) {
    char ret_val[4] = {'0', '0', '0', '0'};
    for(int i = 0; i < 3; i++) {
        if(address[i] == '.') {
                ret_val[i] = 0;
                break;
        }
        ret_val[i] = address[i];
    }
    return (unsigned char)atoi(ret_val);
}