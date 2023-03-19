#include "surf/Argpack.h"

int surf_ArgpackGetInt(surf_argpack_t argpack, int index)
{
    return *((int*) argpack[index]);
}

float surf_ArgpackGetFlt(surf_argpack_t argpack, int index)
{
    return *((float*) argpack[index]);
}

char* surf_ArgpackGetStr(surf_argpack_t argpack, int index)
{
    return *((char**) argpack[index]);
}

surf_V2 surf_ArgpackGetV2(surf_argpack_t argpack, int index)
{ 
    return *((surf_V2*) argpack[index]);
}

surf_V3 surf_ArgpackGetV3(surf_argpack_t argpack, int index)
{
    return *((surf_V3*) argpack[index]);
}

surf_V4 surf_ArgpackGetV4(surf_argpack_t argpack, int index)
{
    return *((surf_V4*) argpack[index]);
}
