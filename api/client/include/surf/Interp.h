#ifndef Interp_h
#define Interp_h

#include "surf/SymbolTable.h"

/// @brief Directly submits a line of code to the interpreter through the API, all strings returned must be destroyed with 'surf_InterpDestroyLine'
/// @param line String buffer containing the line of code to submit to the interpreter
/// @return String buffer containing the interpreted result
char* surf_InterpLine(const char* line);

/// @brief Wrapper to destroy results from 'surf_InterpLine'
/// @param line String allocated as a result of 'surf_InterpLine'
void surf_InterpDestroyLine(char* line);

void surf_InterpRegisterSymbol(const char* id, surf_fun_t func);

void surf_InternalParseReflection(const char* buffer);

/// @brief Destroys the APIs symbol table if it has symbols registered
void surf_InternalInterpDestroy();

#endif /* Interp_h */