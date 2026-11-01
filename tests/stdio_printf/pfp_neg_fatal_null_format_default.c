/* TC
 * id:    pfp_neg_fatal_null_format_default
 * area:  stdio_printf
 * funcs: sprintf sprintf_s
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: default invalid parameter handler terminates; installed handler returning -1
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

static int child_main(const char *name)
{
    char b[16]; const char *volatile f = NULL; volatile void *p = (void *)(size_t)0xABC; int r;
    if (name[0] == 'r') {            /* returning handler control */
        if (!tc_iph_install()) return 1;
        r = sprintf(b, f);
        if (r != -1) return 2;
        r = sprintf_s(b, 4, "%p", p);
        return r == -1 ? 77 : 3;
    }
    tc_iph_default(); /* mingw startup installs a no-op handler; restore the CRT default (fatal) */
    if (name[0] == 'n') { sprintf(b, f); return 77; }
    sprintf_s(b, 4, "%p", p);        /* small_buf */
    return 77;
}
TC_CASE(null_fmt)       { DWORD c = 0; CHECK(tc_spawn_self("null_fmt", &c, 20000)); CHECK(c != 0); CHECK(c != 77); }
TC_CASE(small_buf)      { DWORD c = 0; CHECK(tc_spawn_self("small_buf", &c, 20000)); CHECK(c != 0); CHECK(c != 77); }
TC_CASE(returning_handler) { DWORD c = 0; CHECK(tc_spawn_self("returning_handler", &c, 20000)); CHECK_EQ_U(c, 77); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(null_fmt); TC_RUN(small_buf); TC_RUN(returning_handler);
    return tc_end();
}
