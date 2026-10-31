/* TC
 * id:    err_neg_iph_handler_swapped_in_callback
 * area:  errno_param
 * kind:  negative
 * funcs: strcpy_s
 * procs: _set_invalid_parameter_handler _get_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler, _get_invalid_parameter_handler
 */
#include "tc.h"
#include <string.h>

typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
TC_DECL_PROC(iph_t, _set_invalid_parameter_handler, (iph_t));
TC_DECL_PROC(iph_t, _get_invalid_parameter_handler, (void));

static int a_hits, b_hits;
static void __cdecl hb(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; b_hits++; }
static void __cdecl ha(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; a_hits++; tc_p__set_invalid_parameter_handler(hb); }

static char * volatile v_null = NULL;

TC_CASE(swap)
{
    int r;
    tc_p__set_invalid_parameter_handler(ha);
    r = strcpy_s(v_null, 10, "x");
    CHECK(r != 0); CHECK_EQ_I(a_hits, 1); CHECK_EQ_I(b_hits, 0);
    r = strcpy_s(v_null, 10, "x");
    CHECK(r != 0); CHECK_EQ_I(a_hits, 1); CHECK_EQ_I(b_hits, 1);
    CHECK(tc_p__get_invalid_parameter_handler() == hb);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_invalid_parameter_handler); TC_LOAD_PROC(_get_invalid_parameter_handler);
    if (!tc_p__set_invalid_parameter_handler || !tc_p__get_invalid_parameter_handler) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(swap);
    return tc_end();
}
