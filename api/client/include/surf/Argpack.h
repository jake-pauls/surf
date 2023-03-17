#ifndef Argpack_h
#define Argpack_h

int surf_ArgpackGetInt(void** args, int index)
{
    return *((int*) args[index]);
}

float surf_ArgpackGetFlt(void** args, int index)
{
    return *((float*) args[index]);
}

char* surf_ArgpackGetStr(void** args, int index)
{
    return *((char**) args[index]);
}

#endif /* Argpack_h */