#ifndef MSG_H
#define MSG_H

#include <stdio.h>

#define _EMERG    " EMERG "
#define _ERR      " ERR   " 
#define _WARN     " WARN  "
#define _INFO     " INF   "
#define _DEBUG    " DEBUG "

#define _infox(fmt, ...) \
    printf(_INFO fmt, ##__VA_ARGS__)

#define _warnx(fmt, ...) \
    printf(_WARN fmt, ##__VA_ARGS__)

#define _errx(fmt, ...) \
    printf(_ERR fmt, ##__VA_ARGS__)

#define _debugx(fmt, ...) \
    printf(_DEBUG fmt, ##__VA_ARGS__)

#define _emergx(fmt, ...) \
    printf(_EMERG fmt, ##__VA_ARGS__)

#endif
