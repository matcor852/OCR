#include <stdio.h>
#include <stdarg.h>

static int verbose = 0;

void setVerbose(int v) {
    verbose = v;
}

int getVerbose() {
    return verbose;
}

void vPrintf(const char *format, ...) {
    if (verbose) {
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
    }
}

/*
int main() {
    setVerbose(1);
    for (int i = 0; i < 10; i++)
        vPrintf("verbose = %i\n", getVerbose());
    setVerbose(0);
    for (int i = 0; i < 10; i++)
        vPrintf("verbose = %i\n", getVerbose());
    return 0;
}
*/
