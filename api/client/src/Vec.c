#include "surf/Vec.h"
#include "surf/Define.h"

#include <stdio.h>

int surf_VecV2FromStr(char* str, surf_V2* out)
{
    float x, y;

    int result = sscanf(str, "(%f,%f)", &x, &y);
    if (!result)
        return SURF_FALSE;

    out->x = x;
    out->y = y;

    return SURF_TRUE;
}

int surf_VecV3FromStr(char* str, surf_V3* out)
{
    float x, y, z;

    int result = sscanf(str, "(%f,%f,%f)", &x, &y, &z);
    if (!result)
        return SURF_FALSE;

    out->x = x;
    out->y = y;
    out->z = z;

    return SURF_TRUE;
}

int surf_VecV4FromStr(char* str, surf_V4* out)
{
    float x, y, z, w;

    int result = sscanf(str, "(%f,%f,%f,%f)", &x, &y, &z, &w);
    if (!result)
        return SURF_FALSE;

    out->x = x;
    out->y = y;
    out->z = z;
    out->w = w;

    return SURF_TRUE;
}
