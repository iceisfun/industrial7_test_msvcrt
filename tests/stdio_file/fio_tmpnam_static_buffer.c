/* TC
 * id:    fio_tmpnam_static_buffer
 * area:  stdio_file
 * funcs: tmpnam
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs tmpnam; C11 7.21.4.4
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static char * volatile nullbuf = NULL;

TC_CASE(static_buffer)
{
    char *a, *b; char first[300];
    a = tmpnam(nullbuf);
    CHECK(a != NULL);
    if (!a) return;
    CHECK(strlen(a) < L_tmpnam);
    CHECK(strlen(a) > 0);
    if (strlen(a) >= sizeof first) { CHECK(0); return; }
    strcpy(first, a);
    b = tmpnam(nullbuf);
    CHECK(b != NULL);
    CHECK_EQ_P(a, b);
    if (b) {
        CHECK(!tc_strequ(first, b));  /* the shared buffer now holds a new name */
        CHECK(strlen(b) < L_tmpnam);
    }
}
int main(void) { tc_begin(); TC_RUN(static_buffer); return tc_end(); }
