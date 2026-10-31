/* TC
 * id:    scn_neg_sscanf_truncated_length_iph
 * area:  scanf
 * kind:  negative
 * funcs: sscanf swscanf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs scanf width specification / size prefixes (hh h l ll I I32 I64 L j z t); C11 7.21.6.2 (invalid conversion spec is UB)
 * note:  Probes that a format string ending right after a length modifier, or using an invalid I-prefix size (I16, I8), is rejected by the UCRT format parser: ret <= 0 (EOF or 0), destination untouched, invalid-parameter handler called once, errno EINVAL.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static volatile const char *vin;
static volatile const wchar_t *vwin;

#define NEG_CASE(name, fmt) \
TC_CASE(name) { int a = 99, r; tc_iph_install(); tc_iph_count = 0; errno = 0; vin = "123"; \
    r = sscanf((const char *)vin, fmt, &a); \
    CHECK(r <= 0); CHECK_EQ_I(a, 99); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }

NEG_CASE(pct_l_end, "%l")
NEG_CASE(pct_ll_end, "%ll")
NEG_CASE(pct_h_end, "%h")
NEG_CASE(pct_I_end, "%I")
NEG_CASE(pct_I64_end, "%I64")
NEG_CASE(pct_I16d, "%I16d")
NEG_CASE(pct_I8d, "%I8d")
TC_CASE(wide_pct_l_end) { int a = 99, r; tc_iph_install(); tc_iph_count = 0; errno = 0; vwin = L"123";
    r = swscanf((const wchar_t *)vwin, L"%l", &a);
    CHECK(r <= 0); CHECK_EQ_I(a, 99); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }

int main(void) { tc_begin(); TC_RUN(pct_l_end); TC_RUN(pct_ll_end); TC_RUN(pct_h_end); TC_RUN(pct_I_end); TC_RUN(pct_I64_end);
    TC_RUN(pct_I16d); TC_RUN(pct_I8d); TC_RUN(wide_pct_l_end); return tc_end(); }
