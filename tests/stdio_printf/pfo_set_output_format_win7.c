/* TC
 * id:    pfo_set_output_format_win7
 * area:  stdio_printf
 * funcs:
 * procs: _set_output_format, _get_output_format, sprintf, _snwprintf
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_output_format, _get_output_format: default prints three exponent digits; _TWO_DIGIT_EXPONENT (1) selects two
 * note:  _set_output_format returns the previous setting. _scprintf is covered by the separate probe pfo_set_output_format_scprintf_probe.
 *        msvcr80 / msvcrt.dll are covered via GetProcAddress in pfo_set_output_format_old_procs.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

/* resolved from the DLL under test (mingw may substitute its own sprintf / static output_format) */
TC_DECL_PROC(unsigned int, _set_output_format, (unsigned int));
TC_DECL_PROC(unsigned int, _get_output_format, (void));
TC_DECL_PROC(int, sprintf, (char *, const char *, ...));
TC_DECL_PROC(int, _snwprintf, (wchar_t *, size_t, const wchar_t *, ...));
#define _set_output_format tc_p__set_output_format
#define _get_output_format tc_p__get_output_format
#define sprintf tc_p_sprintf
#define _snwprintf tc_p__snwprintf

#ifndef _TWO_DIGIT_EXPONENT
#define _TWO_DIGIT_EXPONENT 1
#endif

TC_CASE(default_three_digits)
{
    char b[64]; double volatile d = 1.5;
    CHECK_EQ_U(_get_output_format(), 0);
    sprintf(b, "%.1e", d);
    CHECK_STR(b, "1.5e+000");
}
TC_CASE(set_two_digits)
{
    char b[64]; double volatile d = 1.5;
    CHECK_EQ_U(_set_output_format(_TWO_DIGIT_EXPONENT), 0);
    CHECK_EQ_U(_get_output_format(), _TWO_DIGIT_EXPONENT);
    sprintf(b, "%e", d);
    CHECK_STR(b, "1.500000e+00");
    CHECK_EQ_U(_set_output_format(_TWO_DIGIT_EXPONENT), _TWO_DIGIT_EXPONENT);
    _set_output_format(0);
}
TC_CASE(restore)
{
    char b[64]; double volatile d = 1.5;
    _set_output_format(_TWO_DIGIT_EXPONENT);
    CHECK_EQ_U(_set_output_format(0), _TWO_DIGIT_EXPONENT);
    CHECK_EQ_U(_get_output_format(), 0);
    sprintf(b, "%.1e", d);
    CHECK_STR(b, "1.5e+000");
}
TC_CASE(upper_and_g)
{
    char b[64]; double volatile d = 1.5, s = 1e-5;
    _set_output_format(_TWO_DIGIT_EXPONENT);
    sprintf(b, "%.1E|%g", d, s);
    CHECK_STR(b, "1.5E+00|1e-05");
    _set_output_format(0);
    sprintf(b, "%.1E|%g", d, s);
    CHECK_STR(b, "1.5E+000|1e-005");
}
TC_CASE(wide)
{
    wchar_t w[64]; double volatile d = 1.5;
    _set_output_format(_TWO_DIGIT_EXPONENT);
    _snwprintf(w, 64, L"%.1e", d);
    CHECK_WSTR(w, L"1.5e+00");
    _set_output_format(0);
    _snwprintf(w, 64, L"%.1e", d);
    CHECK_WSTR(w, L"1.5e+000");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_output_format); TC_LOAD_PROC(_get_output_format); TC_LOAD_PROC(sprintf); TC_LOAD_PROC(_snwprintf);
    if (!_set_output_format || !_get_output_format || !sprintf || !_snwprintf) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(default_three_digits); TC_RUN(set_two_digits); TC_RUN(restore); TC_RUN(upper_and_g); TC_RUN(wide);
    return tc_end();
}
