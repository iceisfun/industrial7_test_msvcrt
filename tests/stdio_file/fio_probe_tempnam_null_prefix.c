/* TC
 * id:    fio_probe_tempnam_null_prefix
 * area:  stdio_file
 * funcs:
 * procs: _tempnam
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _tempnam
 * note:  PROBE: _tempnam(NULL, NULL) returns a non-NULL name (a NULL prefix is treated as empty). The docs do not say
 *        whether a NULL prefix is permitted.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>

TC_DECL_PROC(char *, _tempnam, (const char *, const char *));
static const char * volatile nulls = NULL;

TC_CASE(null_dir_null_prefix)
{
    char *a;
    if (!tc_p__tempnam) TC_SKIP("_tempnam missing");
    _putenv("TMP=");
    a = tc_p__tempnam(nulls, nulls);
    CHECK(a != NULL);
    free(a);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_tempnam); TC_RUN(null_dir_null_prefix); return tc_end(); }
