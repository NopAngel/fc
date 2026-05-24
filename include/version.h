#ifndef VERSION_H
#define VERSION_H

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_BUILD 1

#define _STR(x) #x
#define STR(x) _STR(x)

#define VERSION_STR STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_BUILD)
#define VERSION_NUM VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD

#define NAME    "fc"
#define DESC    "Fucking Calculator"

#define AUTHOR "NopAngel"


void 
version(void);

void
splash(void);

#endif
