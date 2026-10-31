/* TC
 * id:    fio_neg_tempnam_bad_dir
 * area:  stdio_file
 * kind:  negative
 * funcs:
 * procs: _tempnam
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _tempnam: if dir is NULL or invalid, P_tmpdir is used
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TC_DECL_PROC(char *, _tempnam, (const char *, const char *));
static const char * volatile baddir = "Z:\\no\\such";
static const char * volatile nulls = NULL;
static const char * volatile pfx = "tcb";

TC_CASE(bad_dir_fallback)
{
    char *a;
    _putenv("TMP=");
    a = tc_p__tempnam(baddir, pfx);
    CHECK(a != NULL);
    if (a) {
        CHECK(strncmp(a, "Z:\\no\\such", 10) != 0);
        CHECK(strstr(a, "tcb") != NULL);
        free(a);
    }
}
TC_CASE(null_args)
{
    char *b;
    _putenv("TMP=");
    b = tc_p__tempnam(nulls, pfx);
    CHECK(b != NULL);
    free(b);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_tempnam); CHECK(tc_p__tempnam != NULL); TC_RUN(bad_dir_fallback); TC_RUN(null_args); return tc_end(); }
