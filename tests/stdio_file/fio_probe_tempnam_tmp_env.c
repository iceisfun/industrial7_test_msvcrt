/* TC
 * id:    fio_probe_tempnam_tmp_env
 * area:  stdio_file
 * funcs:
 * procs: _tempnam
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _tempnam
 * note:  PROBE (2): _tempnam(".","p") with TMP unset yields a path whose first char is '.' (dir used verbatim as the path prefix).
 *        PROBE (1): with TMP=. (a valid directory), _tempnam("C:\\nonexistent_dir","p") yields a path starting with '.'
 *        (docs say TMP takes precedence over the dir argument).
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>

TC_DECL_PROC(char *, _tempnam, (const char *, const char *));
static const char * volatile dir = "C:\\nonexistent_dir";
static const char * volatile pfx = "p";

TC_CASE(tmp_wins)
{
    char *a;
    if (!tc_p__tempnam) TC_SKIP("_tempnam missing");
    _putenv("TMP=.");
    a = tc_p__tempnam(dir, pfx);
    CHECK(a != NULL);
    if (a) { CHECK(a[0] == '.'); free(a); }
    _putenv("TMP=");
}
TC_CASE(dir_verbatim)
{
    char *a;
    if (!tc_p__tempnam) TC_SKIP("_tempnam missing");
    _putenv("TMP=");
    a = tc_p__tempnam(".", pfx);
    CHECK(a != NULL);
    if (a) { CHECK(a[0] == '.'); free(a); }
}
int main(void) { tc_begin(); TC_LOAD_PROC(_tempnam); TC_RUN(tmp_wins); TC_RUN(dir_verbatim); return tc_end(); }
