/* TC
 * id:    pfp_neg_snprintf_s_ptr_nodefault_handler
 * area:  stdio_printf
 * funcs: _snprintf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _snprintf_s: _TRUNCATE truncation does not invoke the invalid parameter handler
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

static int child_main(const char *name)
{
    char b[8]; volatile void *p = (void *)(size_t)0xABC; int r; (void)name;
    r = _snprintf_s(b, 4, _TRUNCATE, "%p", p);
    return (r == -1 && b[3] == 0) ? 77 : 2;
}
TC_CASE(survives) { DWORD c = 0; if (!TC_X64) TC_SKIP("x64 only"); CHECK(tc_spawn_self("trunc", &c, 20000)); CHECK_EQ_U(c, 77); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(survives);
    return tc_end();
}
