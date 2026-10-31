/* TC
 * id:    pfm_3_conv_capc_len_more_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefixes (l and w with c/C); hh ll j z t are not documented for c/C
 * note:  PROBE 1: %lC and %wC print a wide character (documented size prefixes l / w). PROBE 2: for hh, ll, L, I, I32, I64, j, z, t on %C
 *        the only assertion is robustness: no crash and either a negative return (invalid-parameter path) or a return equal to the
 *        output length. Each combination also prints a TC INFO line with what the runtime actually did.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile vf;
static int slen(const char *s) { int n = 0; while (s[n]) n++; return n; }

static void one(const char *fmt)
{
    char b[48]; volatile wint_t c = L'A'; int r; char t[24];
    b[0] = 0; tc_iph_count = 0; vf = fmt;
    r = sprintf(b, vf, c);
    CHECK(r < 0 || r == slen(b));
    tc_write("TC INFO " TC_TEST_ID "::"); tc_write(tc_case_name); tc_write(" fmt="); tc_write(fmt);
    tc_write(" r="); tc_i64(r, t); tc_write(t); tc_write(" iph="); tc_i64(tc_iph_count, t); tc_write(t);
    tc_write(" out=["); tc_write(b); tc_write("]\n");
}

TC_CASE(I32C) { one("[%I32C]"); }
TC_CASE(I64C) { one("[%I64C]"); }
TC_CASE(jC) { one("[%jC]"); }
TC_CASE(zC) { one("[%zC]"); }
TC_CASE(tC) { one("[%tC]"); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(I32C); TC_RUN(I64C); TC_RUN(jC); TC_RUN(zC); TC_RUN(tC);
    return tc_end();
}
