#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_BOLD          "\x1b[1;37m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char* repeat(char ch, int repeat);
void append_str_raw(char** dest, const char* str);
void append_str(char** dest, const char* fmt, ...);

#endif