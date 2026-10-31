/* TC
 * id:    ext_set_get_invalid_parameter_handler
 * area:  exit_signal
 * funcs: _set_invalid_parameter_handler _get_invalid_parameter_handler
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler, _get_invalid_parameter_handler (get: VS2010+)
 * note:  _get_invalid_parameter_handler does not exist in msvcr90/80, hence >=100. Whether get() returns NULL after set(NULL)
 *        is probed in ext_probe_get_iph_after_null.
 */
#include "tc.h"
#include <stdlib.h>

static volatile int h1_hits;
static void __cdecl h1(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; h1_hits += 1; }
static volatile int h2_hits;
static void __cdecl h2(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; h2_hits += 2; }
static _invalid_parameter_handler volatile vh1 = h1, vh2 = h2;

TC_CASE(set_returns_previous)
{
    _invalid_parameter_handler orig = _get_invalid_parameter_handler();
    _invalid_parameter_handler prev;
    prev = _set_invalid_parameter_handler(vh1);
    CHECK_EQ_P(prev, orig);
    prev = _set_invalid_parameter_handler(vh2);
    CHECK_EQ_P(prev, h1);
    _set_invalid_parameter_handler(orig);
    CHECK_EQ_P(_get_invalid_parameter_handler(), orig);
}
TC_CASE(get_returns_current)
{
    _invalid_parameter_handler orig = _get_invalid_parameter_handler();
    _set_invalid_parameter_handler(vh1);
    CHECK_EQ_P(_get_invalid_parameter_handler(), h1);
    _set_invalid_parameter_handler(vh2);
    CHECK_EQ_P(_get_invalid_parameter_handler(), h2);
    _set_invalid_parameter_handler(orig);
}
TC_CASE(set_null_returns_previous)
{
    _invalid_parameter_handler orig = _get_invalid_parameter_handler();
    _invalid_parameter_handler prev;
    _set_invalid_parameter_handler(vh1);
    prev = _set_invalid_parameter_handler(NULL);
    CHECK_EQ_P(prev, h1);
    prev = _set_invalid_parameter_handler(vh2);
    CHECK(prev != h2);   /* h2 was never installed before this call */
    _set_invalid_parameter_handler(orig);
}
int main(void)
{
    tc_begin();
    TC_RUN(set_returns_previous); TC_RUN(get_returns_current); TC_RUN(set_null_returns_previous);
    return tc_end();
}
