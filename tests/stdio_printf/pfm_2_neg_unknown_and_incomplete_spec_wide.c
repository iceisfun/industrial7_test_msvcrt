/* TC
 * id:    pfm_2_neg_unknown_and_incomplete_spec_wide
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf_s _snwprintf_s fwprintf_s
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Format specification syntax: printf and wprintf functions; swprintf_s ("If format is invalid, the invalid parameter handler is invoked, -1 returned, errno EINVAL"); Parameter Validation
 * note:  Unknown conversion characters and truncated specifications in the secure wide printf functions. Buffer / stream state after failure is in pfm_2_neg_wide_spec_state_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define EXPECT_BAD(call) do { int tc_n; tc_iph_count = 0; errno = 0; tc_n = (call); \
    CHECK_EQ_I(tc_n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

static int sw(const wchar_t *f) { wchar_t b[32]; return swprintf_s(b, 32, f, 1, 2, 3); }

TC_CASE(unknown_y) { EXPECT_BAD(sw(L"%y")); }
TC_CASE(unknown_k) { EXPECT_BAD(sw(L"%k")); }
TC_CASE(unknown_q) { EXPECT_BAD(sw(L"%q")); }
TC_CASE(unknown_v) { EXPECT_BAD(sw(L"%v")); }
TC_CASE(unknown_bang) { EXPECT_BAD(sw(L"%!")); }
TC_CASE(incomplete_specs)
{
    EXPECT_BAD(sw(L"%"));
    EXPECT_BAD(sw(L"%5"));
    EXPECT_BAD(sw(L"%l"));
    EXPECT_BAD(sw(L"%I64"));
    EXPECT_BAD(sw(L"abc%-+#0.3"));
}
TC_CASE(snwprintf_s_bad)
{
    wchar_t b[16]; const wchar_t * volatile f = L"%y";
    EXPECT_BAD(_snwprintf_s(b, 16, 8, f, 1));
    EXPECT_BAD(_snwprintf_s(b, 16, _TRUNCATE, L"%I64", 1));
}
TC_CASE(fwprintf_s_bad)
{
    const char *fn = "tc_pfm_2_neg_unknown_and_incomplete_spec_wide_a.tmp"; FILE *f = fopen(fn, "w+"); const wchar_t * volatile fmt = L"%y";
    if (!f) { CHECK(f != NULL); return; }
    EXPECT_BAD(fwprintf_s(f, fmt, 1));
    fclose(f); remove(fn);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(unknown_y); TC_RUN(unknown_k); TC_RUN(unknown_q); TC_RUN(unknown_v); TC_RUN(unknown_bang);
    TC_RUN(incomplete_specs); TC_RUN(snwprintf_s_bad); TC_RUN(fwprintf_s_bad);
    return tc_end();
}
