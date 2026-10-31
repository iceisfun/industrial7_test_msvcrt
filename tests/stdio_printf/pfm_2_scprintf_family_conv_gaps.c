/* TC
 * id:    pfm_2_scprintf_family_conv_gaps
 * area:  stdio_printf
 * funcs: _scprintf _vscprintf _scwprintf _vscwprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf, _scwprintf, _vscprintf, _vscwprintf (return number of characters that would be written, excluding NUL); printf format specification
 * note:  %Z is not asserted here (see pfm_2_scprintf_z_probe). %F is only used on ucrtbase. Exponent digits: 3 on pre-ucrt, 2 on ucrt.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static int vsc(const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }
static int vscw(const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = _vscwprintf(f, ap); va_end(ap); return r; }

TC_CASE(int_classes)
{
    const char * volatile f = "%i|%o|%X|%u|%x";
    /* "-7|17|FF|3000000000|ab" */
    CHECK_EQ_I(_scprintf(f, -7, 15, 255, 3000000000u, 0xab), 22);
    CHECK_EQ_I(vsc(f, -7, 15, 255, 3000000000u, 0xab), 22);
}
TC_CASE(char_string_classes)
{
    const char * volatile f = "%c|%C|%s|%S|%%";
    static const wchar_t ws[] = L"wide";
    /* "a|b|nar|wide|%" */
    CHECK_EQ_I(_scprintf(f, 'a', 'b', "nar", ws), 14);
    CHECK_EQ_I(vsc(f, 'a', 'b', "nar", ws), 14);
}
TC_CASE(float_classes)
{
    const char * volatile f = "%e|%E|%G|%g";
    volatile double d = 1.5;
    /* 1.500000e+000 | 1.500000E+000 | 1.5 | 1.5 (ucrt: two-digit exponent) */
    int want = (TC_AT_LEAST(140) ? 12 : 13) * 2 + 9;
    CHECK_EQ_I(_scprintf(f, d, d, d, d), want);
    CHECK_EQ_I(vsc(f, d, d, d, d), want);
}
TC_CASE(pointer_class)
{
    const char * volatile f = "%p|%%";
    void * volatile p = (void *)(uintptr_t)0x1234;
    int want = (int)sizeof(void *) * 2 + 2;
    CHECK_EQ_I(_scprintf(f, p), want);
    CHECK_EQ_I(vsc(f, p), want);
}
#if TC_CRT_VER >= 140
TC_CASE(cap_f_ucrt)
{
    const char * volatile f = "%F|%a";
    volatile double d = 1.5;
    char b[64]; int n = sprintf(b, f, d, d);
    CHECK_EQ_I(_scprintf(f, d, d), n);
    CHECK_EQ_I(vsc(f, d, d), n);
    CHECK(n > 9); /* exact %a length is probed in pfm_2_flag_hexfloat_default_prec_probe */
}
#endif
TC_CASE(wide_counterpart)
{
    const wchar_t * volatile f = L"%d|%ls|%hs|%lc|%%|%5.2f";
    static const wchar_t w[] = L"wd";
    volatile double d = 3.14159;
    /* "9|wd|nr|Q|%| 3.14" */
    CHECK_EQ_I(_scwprintf(f, 9, w, "nr", (wint_t)L'Q', d), 17);
    CHECK_EQ_I(vscw(f, 9, w, "nr", (wint_t)L'Q', d), 17);
}
TC_CASE(wide_int_float)
{
    const wchar_t * volatile f = L"%o %X %x %E";
    volatile double d = 1.5;
    int want = 2 + 1 + 2 + 1 + 2 + 1 + (TC_AT_LEAST(140) ? 12 : 13);
    CHECK_EQ_I(_scwprintf(f, 8, 255, 171, d), want);
    CHECK_EQ_I(vscw(f, 8, 255, 171, d), want);
}
int main(void)
{
    tc_begin();
    TC_RUN(int_classes); TC_RUN(char_string_classes); TC_RUN(float_classes); TC_RUN(pointer_class);
#if TC_CRT_VER >= 140
    TC_RUN(cap_f_ucrt);
#endif
    TC_RUN(wide_counterpart); TC_RUN(wide_int_float);
    return tc_end();
}
