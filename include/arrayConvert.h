#ifndef ARRAY_CONVERT
#define ARRAY_CONVERT
#include "protocolStructure.h"

void convertArrayToPackage(unsigned char buffer[CONVBUFFSIZETP], transferPackage * tp);

void convertPackageToArray(transferPackage * tp, unsigned char buffer[CONVBUFFSIZETP]);

#endif