/* TC
 * id:    pfm_3_conv_caps_len_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefixes (h, l, w with s/S); hh ll L I I32 I64 j z t are not documented for strings
 * note:  PROBE: robustness only. For each size prefix applied to %S with a wide-string argument the assertion is: no crash and either a
 *        negative return or a return equal to the output length. A TC INFO line records what the runtime actually did (output, iph count).
 *        hS / lS / wS are included since documented, but only with the robustness assertion here.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile vf;
static const wchar_t * const volatile ws = L"ab";
static int slen(const char *s) { int n = 0; while (s[n]) n++; return n; }

static void one(const char *fmt)
{
    char b[48]; int r; char t[24];
    b[0] = 0; tc_iph_count = 0; vf = fmt;
    r = sprintf(b, vf, ws);
    CHECK(r < 0 || r == slen(b));
    tc_write("TC INFO " TC_TEST_ID "::"); tc_write(tc_case_name); tc_write(" fmt="); tc_write(fmt);
    tc_write(" r="); tc_i64(r, t); tc_write(t); tc_write(" iph="); tc_i64(tc_iph_count, t); tc_write(t);
    tc_write(" out=["); tc_write(b); tc_write("]\n");
}
TC_CASE(hhS) { one("[%hhS]"); }
TC_CASE(llS) { one("[%llS]"); }
TC_CASE(LS)  { one("[%LS]"); }
TC_CASE(IS)  { one("[%IS]"); }
TC_CASE(I32S) { one("[%I32S]"); }
TC_CASE(I64S) { one("[%I64S]"); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(IS); TC_RUN(I32S); TC_RUN(I64S); TC_RUN(LS); TC_RUN(hhS); TC_RUN(llS);
    return tc_end();
}
