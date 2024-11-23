#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "input.h"

int get_a_line(FILE *fp, char *buf) {
    char tmp[BLEN], *ptr;

    if (fgets(tmp, BLEN, fp)) {
        int i;

        ptr = strchr(tmp, '#');
        if (ptr) *ptr = '\0';
        i = strlen(tmp) - 1;
        while (i >= 0 && isspace(tmp[i])) {
            tmp[i--] = '\0';
        }
        ptr = tmp;
        while (*ptr && isspace(*ptr)) { ++ptr; }
        strcpy(buf, ptr);
        return 0;
    } else {
        perror("Problem reading input");
        return -1;
    }
}
