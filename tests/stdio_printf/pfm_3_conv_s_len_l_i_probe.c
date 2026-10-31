/* TC
 * id:    pfm_3_conv_s_len_l_i_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefixes (I, I32, I64 are for integer types; L is for long double); not documented for %s
 * note:  PROBE: robustness only. %Ls, %Is, %I32s, %I64s with a narrow string argument (sprintf) or, in _snwprintf, a wide one. The argument buffers
 *        are zero-padded so that a mismatched-width read stays inside the object. Assertion: no crash and either a negative return or a
 *        return equal to the output length. A TC INFO line records the observed result.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char * volatile vf;
static const wchar_t * volatile wvf;
static char nbuf[16] = "ab";
static wchar_t wbuf[8] = L"ab";
static int slen(const char *s) { int n = 0; while (s[n]) n++; return n; }
static int wlen(const wchar_t *s) { int n = 0; while (s[n]) n++; return n; }

static void info(const char *fmt, int r, int narrow_out)
{
    char t[24]; tc_write("TC INFO " TC_TEST_ID "::"); tc_write(tc_case_name); tc_write(" fmt="); tc_write(fmt);
    tc_write(" r="); tc_i64(r, t); tc_write(t); tc_write(" iph="); tc_i64(tc_iph_count, t); tc_write(t);
    tc_write(narrow_out ? " (narrow)\n" : " (wide)\n");
}
static void one(const char *fmt, const wchar_t *wfmt)
{
    char b[48]; wchar_t w[48]; int r;
    b[0] = 0; w[0] = 0; tc_iph_count = 0; vf = fmt;
    r = sprintf(b, vf, nbuf);
    CHECK(r < 0 || r == slen(b)); info(fmt, r, 1);
    tc_iph_count = 0; wvf = wfmt;
    r = _snwprintf(w, 48, wvf, wbuf);
    CHECK(r < 0 || r == wlen(w)); info(fmt, r, 0);
}
TC_CASE(Ls)   { one("[%Ls]", L"[%Ls]"); }
TC_CASE(Is)   { one("[%Is]", L"[%Is]"); }
TC_CASE(I32s) { one("[%I32s]", L"[%I32s]"); }
TC_CASE(I64s) { one("[%I64s]", L"[%I64s]"); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(Ls); TC_RUN(Is); TC_RUN(I32s); TC_RUN(I64s);
    return tc_end();
}
