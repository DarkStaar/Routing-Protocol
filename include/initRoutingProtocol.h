#ifndef INIT_ROUTINGPROTOCOL
#define INIT_ROUTINGPROTOCOL
#include "sys/socket.h"
#include "sys/types.h"
#include "../include/protocolStructure.h"

int initUserModel(unsigned int portNumber, char ipAddress[16], userModel* um);

int connectUserModelToNetwork(userModel * user, routerModel * destinationRouter);

int initRouterModel(unsigned int portNumber, char ipAddress[16], RPAddress rpAddress, routerModel* rm);

int connectRouterModelToNetwork(routerModel * sourceRouter, routerModel * destinationRouter);

int closeUserModel(userModel * um);

int closeRouterModel(routerModel * rm);

#endif