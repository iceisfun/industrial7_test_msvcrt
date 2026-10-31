/* TC
 * id:    pfo_set_output_format_old_procs
 * area:  stdio_printf
 * funcs:
 * procs: _set_output_format, _get_output_format, sprintf
 * crt:   msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_output_format (VS2005+; msvcrt.dll on Windows 7 is believed to export it too)
 * note:  PROBE: msvcr80.dll and msvcrt.dll export _set_output_format / _get_output_format and honour _TWO_DIGIT_EXPONENT.
 */
#include "tc.h"
#include <stdio.h>

typedef unsigned int (__cdecl *setf_t)(unsigned int);
typedef unsigned int (__cdecl *getf_t)(void);
static setf_t p_set; static getf_t p_get;
TC_DECL_PROC(int, sprintf, (char *, const char *, ...));
#define sprintf tc_p_sprintf

TC_CASE(resolves) { CHECK(p_set != NULL); CHECK(p_get != NULL); }
TC_CASE(set_get)
{
    if (!p_set || !p_get) TC_SKIP("not exported");
    CHECK_EQ_U(p_get(), 0);
    CHECK_EQ_U(p_set(1), 0);
    CHECK_EQ_U(p_get(), 1);
    p_set(0);
}
TC_CASE(exponent_digits)
{
    char b[64]; double volatile d = 1.5;
    if (!p_set || !sprintf) TC_SKIP("not exported");
    sprintf(b, "%.1e", d); CHECK_STR(b, "1.5e+000");
    p_set(1);
    sprintf(b, "%.1e", d); CHECK_STR(b, "1.5e+00");
    p_set(0);
    sprintf(b, "%.1e", d); CHECK_STR(b, "1.5e+000");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(sprintf);
    p_set = (setf_t)tc_crt_proc("_set_output_format");
    p_get = (getf_t)tc_crt_proc("_get_output_format");
    TC_RUN(resolves); TC_RUN(set_get); TC_RUN(exponent_digits);
    return tc_end();
}
