#ifndef USER_HOST
#define USER_HOST
#include <stdlib.h>
#include "protocolStructure.h"

int sendTPtoRouter(transferPackage* tp, userModel * um);

int recieveTPfromRouter(transferPackage * tp, userModel * um);

#endif