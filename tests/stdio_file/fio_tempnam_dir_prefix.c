/* TC
 * id:    fio_tempnam_dir_prefix
 * area:  stdio_file
 * funcs:
 * procs: _tempnam
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _tempnam (TMP environment variable, if set to a valid directory, takes precedence over dir)
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TC_DECL_PROC(char *, _tempnam, (const char *, const char *));
static const char * volatile dir = ".";
static const char * volatile pfx = "tcp";

TC_CASE(dir_prefix)
{
    char *a, *b;
    _putenv("TMP=");
    a = tc_p__tempnam(dir, pfx);
    CHECK(a != NULL);
    if (!a) return;
    CHECK(strstr(a, "tcp") != NULL);
    b = tc_p__tempnam(dir, pfx);
    CHECK(b != NULL);
    if (b) { CHECK(!tc_strequ(a, b)); free(b); }
    free(a);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_tempnam); CHECK(tc_p__tempnam != NULL); TC_RUN(dir_prefix); return tc_end(); }
