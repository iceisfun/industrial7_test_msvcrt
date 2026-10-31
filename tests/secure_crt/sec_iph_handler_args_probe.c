/* TC
 * id:    sec_iph_handler_args_probe
 * area:  secure_crt
 * funcs: strcpy_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler / Parameter Validation
 * note:  PROBE: docs say the retail CRT passes NULL for expression, function and file and 0 for line. Also probes
 *        that the reserved argument is 0. Real Windows result is what we want measured.
 */
#include "tc.h"
#include <string.h>

static const wchar_t *g_e, *g_f, *g_file; static unsigned g_l; static uintptr_t g_r; static int g_n;
static void __cdecl rec(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ g_e = e; g_f = f; g_file = fl; g_l = l; g_r = r; g_n++; }
typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static char *volatile v_nulld;

static void trig(void)
{
    char d[4]; g_n = 0; g_e = g_f = g_file = (const wchar_t *)1; g_l = 99; g_r = 99;
    strcpy_s(v_nulld, sizeof d, "a");
}
TC_CASE(expression_function_file_null_and_line_zero_in_retail)
{
    trig();
    CHECK_EQ_I(g_n, 1);
    CHECK(g_e == NULL); CHECK(g_f == NULL); CHECK(g_file == NULL); CHECK_EQ_U(g_l, 0);
}
TC_CASE(reserved_is_zero) { trig(); CHECK_EQ_U(g_r, 0); }
int main(void)
{
    iph_t (__cdecl *set)(iph_t); tc_begin();
    set = (iph_t (__cdecl *)(iph_t))tc_crt_proc("_set_invalid_parameter_handler");
    if (set) set(rec);
    TC_RUN(expression_function_file_null_and_line_zero_in_retail); TC_RUN(reserved_is_zero);
    return tc_end();
}
