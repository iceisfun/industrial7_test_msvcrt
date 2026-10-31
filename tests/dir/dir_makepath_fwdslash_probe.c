/* TC
 * id:    dir_makepath_fwdslash_probe
 * area:  dir
 * funcs: _makepath
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _makepath (appends a trailing '\' to dir when missing; docs treat '/' as valid separator)
 * note:  Probes what _makepath appends to a dir ending in neither separator when dir already uses '/': expected
 *        the backslash ("a/b" -> "a/b\f.e"), as the CRT source appends '\' unconditionally. Probe-only.
 */
#include "tc.h"
#include <stdlib.h>

static const char *vs(const char *s) { static volatile const char *v; v = s; return (const char *)v; }

TC_CASE(fwd_dir_no_trailing)
{
    char b[_MAX_PATH];
    _makepath(b, NULL, vs("a/b"), vs("f"), vs(".e"));
    CHECK_STR(b, "a/b\\f.e");
}
int main(void) { tc_begin(); TC_RUN(fwd_dir_no_trailing); return tc_end(); }
