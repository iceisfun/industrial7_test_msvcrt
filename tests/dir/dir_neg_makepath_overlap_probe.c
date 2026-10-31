/* TC
 * id:    dir_neg_makepath_overlap_probe
 * area:  dir
 * kind:  negative
 * funcs: _makepath
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _makepath (no statement about aliasing; probe only)
 * note:  Probes survivability only: output buffer aliases the dir input. The resulting string is not compared
 *        with any value (an invalid-parameter handler is installed so a CRT that rejects the aliasing is not fatal); only that the call returns and the result is NUL-terminated inside the buffer.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(alias_dir)
{
    char b[MAX_PATH]; int i, term = 0; const char *volatile f = "f";
    for (i = 0; i < MAX_PATH; i++) b[i] = 0;
    b[0] = 'a'; b[1] = '\\'; b[2] = 'b'; b[3] = '\\';
    _makepath(b, NULL, b, f, ".c");
    for (i = 0; i < MAX_PATH; i++) if (b[i] == 0) { term = 1; break; }
    CHECK(term);
}
int main(void) { tc_begin(); if (TC_AT_LEAST(80)) tc_iph_install(); /* secure-CRT paths must not be fatal here */ TC_RUN(alias_dir); return tc_end(); }
