/* TC
 * id:    pfp_neg_fatal_ptr_overflow_default
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s, Parameter validation: overflow under default handler terminates
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

static int child_main(const char *name)
{
    char b[32]; volatile void *p = (void *)(size_t)0xABC; int r;
    if (name[0] == 'o') { r = sprintf_s(b, 17, "%p", p); return r == 16 ? 77 : 2; }
    sprintf_s(b, 10, "%p", p);
    return 77;
}
TC_CASE(ptr_small) { DWORD c = 0; if (!TC_X64) TC_SKIP("x64 only"); CHECK(tc_spawn_self("ptr_small", &c, 20000)); CHECK(c != 0); CHECK(c != 77); }
TC_CASE(ptr_ok)    { DWORD c = 0; if (!TC_X64) TC_SKIP("x64 only"); CHECK(tc_spawn_self("ok", &c, 20000)); CHECK_EQ_U(c, 77); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(ptr_small); TC_RUN(ptr_ok);
    return tc_end();
}
