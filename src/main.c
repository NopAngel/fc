/*
 * FC/(Fucking Calculator) - v0.0.1
 * AUTHOR: NopAngel | @Copyright 2026
 * Desc: Full logic, Loops (DO), and Custom Functions.
 
 *
 *
 *
 * @Copyright 2026 - NopAngel 
 * :v
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <version.h>
#include <math.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ctype.h>

int scale = 2;
int obase = 10;
char *ptr;

typedef struct { char name[32]; double value; } Variable;
Variable vars[200];
int var_count = 0;

typedef struct { char name[32]; char body[256]; char arg_name[32]; } Function;
Function funcs[100];
int func_count = 0;

void 
set_var
(const char* name, double val) 
{
    for(int i=0; i<var_count; i++) if(strcmp(vars[i].name, name) == 0) { vars[i].value = val; return; }
    strncpy(vars[var_count].name, name, 31); vars[var_count++].value = val;
}
double 
get_var
(const char* name) 
{
    for(int i=0; i<var_count; i++) if(strcmp(vars[i].name, name) == 0) return vars[i].value;
    return 0.0;
}

void
skip_spaces
() 
{ while (*ptr && (isspace(*ptr))) ptr++; }
double expression();

double
factor
() 
{
    skip_spaces();
    if (*ptr == '(') { ptr++; double res = expression(); if (*ptr == ')') ptr++; return res; }
    if (isdigit(*ptr) || (*ptr == '-' && isdigit(*(ptr+1)))) { char *next; double val = strtod(ptr, &next); ptr = next; return val; }
    if (isalpha(*ptr)) {
        char name[32]; int i = 0;
        while (isalnum(*ptr)) name[i++] = *ptr++;
        name[i] = '\0';
        skip_spaces();
        if (*ptr == '(') {
            ptr++; double arg = expression(); if (*ptr == ')') ptr++;
            for(int f=0; f<func_count; f++) {
                if(strcmp(funcs[f].name, name) == 0) {
                    double old_val = get_var(funcs[f].arg_name);
                    set_var(funcs[f].arg_name, arg);
                    char *old_ptr = ptr; char temp_body[256]; strcpy(temp_body, funcs[f].body);
                    ptr = temp_body; double res = expression();
                    ptr = old_ptr; set_var(funcs[f].arg_name, old_val); return res;
                }
            }
            if (strcmp(name, "s") == 0) return sin(arg);
            if (strcmp(name, "c") == 0) return cos(arg);
            if (strcmp(name, "sqrt") == 0) return sqrt(arg);
            return 0;
        }
        return get_var(name);
    }
    return 0;
}

double
term
() 
{
    double res = factor(); skip_spaces();
    while (*ptr == '*' || *ptr == '/') {
        char op = *ptr++; double r = factor();
        if (op == '*') res *= r; else if (r != 0) res /= r;
        skip_spaces();
    }
    return res;
}

double
expression
() 
{
    double res = term(); skip_spaces();
    while (*ptr == '+' || *ptr == '-' || *ptr == '>' || *ptr == '<' || (*ptr == '=' && *(ptr+1) == '=')) {
        if (*ptr == '=' && *(ptr+1) == '=') { ptr += 2; res = (res == term()); }
        else {
            char op = *ptr++; double r = term();
            if (op == '+') res += r; else if (op == '-') res -= r;
            else if (op == '>') res = (res > r); else if (op == '<') res = (res < r);
        }
        skip_spaces();
    }
    return res;
}

void 
execute_line
(char *line) 
{
    char clean_line[1024]; strcpy(clean_line, line);
    ptr = clean_line; skip_spaces();
    if (!*ptr || *ptr == '#') return;

    if (strncmp(ptr, "scale", 5) == 0) { sscanf(ptr, "scale = %d", &scale); return; }
    if (strncmp(ptr, "obase", 5) == 0) { sscanf(ptr, "obase = %d", &obase); return; }

    if (strncmp(ptr, "define ", 7) == 0) {
        ptr += 7; char f_name[32], a_name[32];
        sscanf(ptr, "%[^(](%[^)])", f_name, a_name);
        char *eq = strchr(ptr, '=');
        if (eq) {
            strncpy(funcs[func_count].name, f_name, 31);
            strncpy(funcs[func_count].arg_name, a_name, 31);
            strcpy(funcs[func_count].body, eq + 1);
            func_count++; printf("Function %s(%s) defined.\n", f_name, a_name);
        }
        return;
    }

    if (strncmp(ptr, "if", 2) == 0) {
        ptr += 2; skip_spaces();
        if (*ptr == '(') {
            ptr++; double cond = expression();
            if (*ptr == ')') ptr++;
            skip_spaces(); if (cond > 0.5) execute_line(ptr);
        }
        return;
    }

    // DO loop: do(var, limit) action
    if (strncmp(ptr, "do", 2) == 0) {
        ptr += 2; skip_spaces();
        if (*ptr == '(') {
            ptr++; char vname[32]; int i=0; while(isalnum(*ptr)) vname[i++] = *ptr++; vname[i]='\0';
            if (*ptr == ',') ptr++; double limit = expression(); if (*ptr == ')') ptr++;
            skip_spaces(); char action[256]; strcpy(action, ptr);
            for (double j = 1; j <= limit; j++) { set_var(vname, j); execute_line(action); }
        }
        return;
    }

    char *eq = strchr(ptr, '=');
    if (eq && !strstr(ptr, "==")) {
        char name[32]; int i=0; while(isalnum(*ptr)) name[i++] = *ptr++; name[i]='\0';
        ptr = eq + 1; double val = expression(); set_var(name, val);
        if (isatty(0)) { if (obase == 16) printf("%X\n", (int)val); else printf("%.*lf\n", scale, val); }
    } else {
        double res = expression();
        if (obase == 16) printf("%X\n", (int)res); else printf("%.*lf\n", scale, res);
    }
}

int 
main
() 
{
    if (isatty(0)) 
    {
        splash();
        printf("fc > ");
        fflush(stdout);
    }
    char *line;
    char buf[1024];
    while ((line = readline("fc > ")) != NULL) {
        
        if (*line) {
            add_history(line);
            execute_line(line);
        }
        
        free(line);
    } 
    return 0;
}
