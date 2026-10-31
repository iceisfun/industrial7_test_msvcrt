/* TC
 * id:    fio_probe_tmpnam_format
 * area:  stdio_file
 * funcs: tmpnam
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs tmpnam (names are relative to the root: "\s1jo.0" style)
 * note:  PROBE: the generated name starts with a backslash and has no further path separator.
 *        Exact shape beyond that is deliberately not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(leading_backslash) { char b[L_tmpnam + 16]; tmpnam(b); CHECK(b[0] == '\\'); }
TC_CASE(no_more_separators)
{
    char b[L_tmpnam + 16]; tmpnam(b);
    CHECK(strchr(b + 1, '\\') == NULL);
    CHECK(strchr(b, '/') == NULL);
}
int main(void) { tc_begin(); TC_RUN(leading_backslash); TC_RUN(no_more_separators); return tc_end(); }
