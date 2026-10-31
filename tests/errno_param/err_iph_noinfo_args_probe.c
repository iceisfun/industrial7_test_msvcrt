/* TC
 * id:    err_iph_noinfo_args_probe
 * area:  errno_param
 * funcs: _invalid_parameter_noinfo _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _invalid_parameter_noinfo
 * note:  PROBE: the handler receives expression/function/file == NULL and line == 0 from _invalid_parameter_noinfo.
 */
#include "tc.h"
#include <stdlib.h>

static volatile int seen;
static const wchar_t * volatile ge, * volatile gf, * volatile gfl;
static volatile unsigned int gl = 99;
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)r; ge = e; gf = f; gfl = fl; gl = l; seen++; }

TC_CASE(null_args)
{
    seen = 0;
    _invalid_parameter_noinfo();
    CHECK_EQ_I(seen, 1);
    CHECK(ge == NULL); CHECK(gf == NULL); CHECK(gfl == NULL);
    CHECK_EQ_I(gl, 0);
}
int main(void)
{
    tc_begin();
    _set_invalid_parameter_handler(h);
    TC_RUN(null_args);
    return tc_end();
}
