/* TC
 * id:    scn_neg_vsscanf_family_small_buf_probe
 * area:  scanf
 * kind:  negative
 * funcs: __stdio_common_vsscanf __stdio_common_vswscanf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs scanf_s (buffer too small); vsscanf_s, vswscanf_s
 * note:  Probes the va_list secure variants with a destination size smaller than the string: guess ret 0, errno ERANGE,
 *        invalid-parameter handler called, and destination[0] set to 0 (docs for scanf_s say first char is set to null).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

static int call_vsscanf_s(const char *in, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = vsscanf_s(in, fmt, ap); va_end(ap); return r; }
static int call_vswscanf_s(const wchar_t *in, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = _vswscanf_s_l(in, fmt, NULL, ap); va_end(ap); return r; }
static const char *volatile vin; static const wchar_t *volatile win;

TC_CASE(vsscanf_s_small_buf)
{
    char b[8]; int r;
    b[0] = 'Z'; tc_iph_install(); tc_iph_count = 0; errno = 0; vin = "abcdef";
    r = call_vsscanf_s(vin, "%s", b, (unsigned)4);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, ERANGE); CHECK(tc_iph_count >= 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(vswscanf_s_small_buf)
{
    wchar_t b[8]; int r;
    b[0] = L'Z'; tc_iph_install(); tc_iph_count = 0; errno = 0; win = L"abcdef";
    r = call_vswscanf_s(win, L"%s", b, (unsigned)4);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, ERANGE); CHECK(tc_iph_count >= 1); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(vsscanf_s_small_buf); TC_RUN(vswscanf_s_small_buf);
    return tc_end();
}
