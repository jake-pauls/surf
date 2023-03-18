#include "surf/Argpack.h"

int surf_ArgpackGetInt(surf_argpack_t args, int index)
{
    return *((int*) args[index]);
}

float surf_ArgpackGetFlt(surf_argpack_t args, int index)
{
    return *((float*) args[index]);
}

char* surf_ArgpackGetStr(surf_argpack_t args, int index)
{
    return *((char**) args[index]);
}
