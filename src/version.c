/*
 *  version.c
 
 
 *  functions to display versions
 *
 *
 * @Copyright 2026 <NopAngel> 
 *
 */

#include <stdio.h>
#include <version.h>

#ifndef CONFIG_LANG
#define CONFIG_LANG 0
#endif

void 
version 
(void)
{
    printf("fc, v%s\n", VERSION_STR);
}

void
splash
(void)
{
version();

#if (CONFIG_LANG == 2)
fputs("Una calculadora terminal con operaciones avanzadas\n", stdout);
#elif (CONFIG_LANG == 1)
fputs("高度な操作を備えたターミナル計算機\n", stdout);
#else
fputs("A terminal calculator with advanced operations\n", stdout);
#endif

fputs(" @Copyright 2026 <NopAngel> [FC/Fucking Calculator]\n", stdout);

}
