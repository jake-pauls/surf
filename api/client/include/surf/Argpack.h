#ifndef Argpack_h
#define Argpack_h

#include "surf/Vec.h"

/// @brief Array of void pointers representing a 'package' of function arguments 
///        sent from reflected functions in the surf interpreter
typedef void** surf_argpack_t;

/// @brief Unpacks an integer argument for a function reflected in the surf interpreter 
/// @param argpack Argpack containing the reflected arguments
/// @param index Zero-based index of the argument to retrieve as an int
/// @return The integer argument
int surf_ArgpackGetInt(surf_argpack_t argpack, int index);

/// @brief Unpacks a float argument for a function reflected in the surf interpreter 
/// @param argpack Argpack containing the reflected arguments
/// @param index Zero-based index of the argument to retrieve as an float
/// @return The float argument
float surf_ArgpackGetFlt(surf_argpack_t argpack, int index);

/// @brief Unpacks a string argument for a function reflected in the surf interpreter
/// @param argpack Argpack containing the reflected arguments
/// @param index Zero-based index of the argument to retrieve as a C string
/// @return The string argument
char* surf_ArgpackGetStr(surf_argpack_t argpack, int index);

/// @brief Unpacks a two float vector argument for a function reflected in the surf interpreter
/// @param argpack Argpack containing the reflected arguments
/// @param index Zero-based index of the argument to retrieve as a C string
/// @return The two float vector argument 
surf_V2 surf_ArgpackGetV2(surf_argpack_t argpack, int index);

/// @brief Unpacks a three float vector argument for a function reflected in the surf interpreter
/// @param argpack Argpack containing the reflected arguments
/// @param index Zero-based index of the argument to retrieve as a C string
/// @return The three float vector argument 
surf_V3 surf_ArgpackGetV3(surf_argpack_t argpack, int index);

/// @brief Unpacks a four float vector argument for a function reflected in the surf interpreter
/// @param argpack Argpack containing the reflected arguments
/// @param index Zero-based index of the argument to retrieve as a C string
/// @return The four float vector argument 
surf_V4 surf_ArgpackGetV4(surf_argpack_t argpack, int index);

#endif /* Argpack_h */