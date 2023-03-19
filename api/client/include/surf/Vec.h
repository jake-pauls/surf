#ifndef Vec_h
#define Vec_h

/// @brief Represents a two float vector from the surf interpreter
typedef struct surf_V2
{
	float x;
	float y;
} surf_V2;

/// @brief Represents a three float vector from the surf interpreter
typedef struct surf_V3
{
	float x;
	float y;
	float z;
} surf_V3;

/// @brief Represents a four float vector from the surf interpreter
typedef struct surf_V4
{
	float x;
	float y;
	float z;
	float w;
} surf_V4;

/// @brief Converts a two float vector string from the surf interpreter into the APIs 'surf_V2' struct
/// @param str String in the form of the interpreter's 'v2' - '(%f,%f)'
/// @param out Address of the vector struct to assign the parsed components into
/// @return True if the parsing succeeded, false if it did not
int surf_VecV2FromStr(char* str, surf_V2* out);

/// @brief Converts a three float vector string from the surf interpreter into the APIs 'surf_V3' struct
/// @param str String in the form of the interpreter's 'v3' - '(%f,%f,%f)'
/// @param out Address of the vector struct to assign the parsed components into
/// @return True if the parsing succeeded, false if it did not
int surf_VecV3FromStr(char* str, surf_V3* out);

/// @brief Converts a four float vector string from the surf interpreter into the APIs 'surf_V4' struct
/// @param str String in the form of the interpreter's 'v4' - '(%f,%f,%f,%f)'
/// @param out Address of the vector struct to assign the parsed components into
/// @return True if the parsing succeeded, false if it did not
int surf_VecV4FromStr(char* str, surf_V4* out);

#endif /* Vec_h */