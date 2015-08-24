#pragma once

#include <time.h>
#include <stdio.h>

// ----- DLL export -----
#ifndef EXPORT_FUNCTION
	#define EXPORT_FUNCTION __declspec(dllexport)
#endif

// ----- Type size -----
#define sizeof_array(array) \
    ((int)(sizeof(array) / sizeof(*(array))))

#define sizeof_struct_member(struct, member) \
   (sizeof(((struct *)0)->member))


// ----- Enumeration string association -----
typedef struct EnumerationStringAssociation {
	int enumeration;
	char *string;
} EnumerationStringAssociation;
#define associate(x) [x] = {x, STRINGIFY(x)}


// ----- Boolean -----
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0500

#ifndef BOOL
#ifdef WIN32
#include <windows.h>
#else
#define BOOL char
#endif // WIN32
#endif // BOOL

#ifndef bool
#define bool BOOL
#endif

#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef false
#define false (!(TRUE))
#endif

#ifndef true
#define true TRUE
#endif

#ifndef false
#define false FALSE
#endif

#define __out__
#define __in__
#define __opt__
#define __inout__

// Stringify
#define STRINGIFY(x) # x

int
str_is_in_array (char *value, char **array, int size);

/*
 * Description : Return a time_t structure containing the current date detected
 * Return : The current date
 */
time_t
get_now (
	void
);

/*
 * Description : Writes in a buffer the current date detected
 * Return : A string containing the current date
 */
char *
get_now_str (
	void
);


/*
 * Description : Writes in a buffer the current date detected
 * char *buffer : A buffer *WITH A LEAST 20 BYTES ALLOCATED*
 */
void
get_now_buffer (
	__out__ char *buffer
);


/*
 * Description : Check if the position is inside of the boundaries
 * Return : True on success, false on failure
 */
bool
in_bound (
	int x, int y,
	int x1, int y1, int x2, int y2
);
