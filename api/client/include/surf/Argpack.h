#ifndef Argpack_h
#define Argpack_h

/// @brief Array of void pointers representing a 'package' of function arguments 
///        sent from reflected functions in the surf interpreter
typedef void** surf_argpack_t;

/// @brief Unpacks an integer argument for a function reflected in the surf interpreter 
/// @param args Argpack containing the reflected arguments
/// @param index Zero-based index of the argument to retrieve as an int
/// @return The integer argument
int surf_ArgpackGetInt(surf_argpack_t args, int index)
{
    return *((int*) args[index]);
}

/// @brief Unpacks a float argument for a function reflected in the surf interpreter 
/// @param args Argpack containing the reflected arguments
/// @param index Zero-based index of the argument to retrieve as an float
/// @return The float argument
float surf_ArgpackGetFlt(surf_argpack_t args, int index)
{
    return *((float*) args[index]);
}

/// @brief Unpacks a string argument for a function reflected in the surf interpreter
/// @param args Argpack containing the reflected arguments
/// @param index Zero-based index of the argument to retrieve as a C string
/// @return The string argument
char* surf_ArgpackGetStr(surf_argpack_t args, int index)
{
    return *((char**) args[index]);
}

#endif /* Argpack_h */