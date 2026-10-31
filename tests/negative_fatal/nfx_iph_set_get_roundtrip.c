/* TC
 * id:    nfx_iph_set_get_roundtrip
 * area:  negative_fatal
 * kind:  positive
 * funcs: _set_invalid_parameter_handler
 * procs: _get_invalid_parameter_handler
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler, _get_invalid_parameter_handler
 * note:  Process starts with no handler installed; tc_begin does not install one.
 */
#include "tc.h"
#include <stdlib.h>

static void __cdecl h1(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
static void __cdecl h2(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
typedef _invalid_parameter_handler iph_t;
TC_DECL_PROC(iph_t, _get_invalid_parameter_handler, (void));

TC_CASE(roundtrip)
{
    iph_t orig;
    if (!tc_p__get_invalid_parameter_handler) TC_SKIP("getter not exported");
    orig = _set_invalid_parameter_handler(h1);
    CHECK_EQ_P(tc_p__get_invalid_parameter_handler(), h1);
    CHECK_EQ_P(_set_invalid_parameter_handler(h2), h1);
    CHECK_EQ_P(tc_p__get_invalid_parameter_handler(), h2);
    CHECK_EQ_P(_set_invalid_parameter_handler(NULL), h2);
    CHECK_EQ_P(tc_p__get_invalid_parameter_handler(), NULL);
    _set_invalid_parameter_handler(orig);
}
TC_CASE(restore_prev)
{
    _invalid_parameter_handler p = _set_invalid_parameter_handler(h1);
    CHECK_EQ_P(_set_invalid_parameter_handler(p), h1);
    CHECK_EQ_P(_set_invalid_parameter_handler(NULL), p);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_get_invalid_parameter_handler); TC_RUN(roundtrip); TC_RUN(restore_prev); return tc_end(); }
