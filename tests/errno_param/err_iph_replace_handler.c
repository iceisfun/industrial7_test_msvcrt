/* TC
 * id:    err_iph_replace_handler
 * area:  errno_param
 * funcs: strcpy_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler (returns previous handler; only the current one is called)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile int c1, c2;
static void __cdecl h1(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; c1++; }
static void __cdecl h2(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; c2++; }
static void bad(void) { char d[4]; volatile size_t z = 0; strcpy_s(d, z, "a"); }

TC_CASE(latest_wins)
{
    _invalid_parameter_handler p;
    c1 = c2 = 0;
    _set_invalid_parameter_handler(h1);
    p = _set_invalid_parameter_handler(h2);
    CHECK(p == h1);
    bad();
    CHECK_EQ_I(c2, 1); CHECK_EQ_I(c1, 0);
}
TC_CASE(switch_back)
{
    _invalid_parameter_handler p;
    c1 = c2 = 0;
    p = _set_invalid_parameter_handler(h1);
    CHECK(p == h2);
    bad(); bad();
    CHECK_EQ_I(c1, 2); CHECK_EQ_I(c2, 0);
}
int main(void)
{
    tc_begin();
    _set_invalid_parameter_handler(tc_iph);
    TC_RUN(latest_wins); TC_RUN(switch_back);
    return tc_end();
}
