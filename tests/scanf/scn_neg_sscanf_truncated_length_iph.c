/* TC
 * id:    scn_neg_sscanf_truncated_length_iph
 * area:  scanf
 * kind:  negative
 * funcs: sscanf swscanf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs scanf width specification / size prefixes (hh h l ll I I32 I64 L j z t); C11 7.21.6.2 (invalid conversion spec is UB)
 * note:  Probes that a format string ending right after a length modifier, or using an invalid I-prefix size (I16, I8), is rejected by the UCRT format parser: ret <= 0 (EOF or 0), destination untouched, invalid-parameter handler called once, errno EINVAL.
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): '%l', '%ll', '%h', '%I64' at end of format: return 0, destination untouched, handler NOT called, errno 0.
 *        '%I', '%I16d', '%I8d' still convert (r==1, value 123), handler NOT called. Stable (2 runs). The I-prefix cases fill the long long with 0xAA.. and assert the exact store width: low 4 bytes == 123, high 4 bytes stay 0xAAAAAAAA (a 4-byte store).
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
    CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 99); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
/* '%I', '%I16d', '%I8d': ucrtbase converts "123" anyway (r==1, no handler call). The destination is a long long pre-filled with 0xAA so the store width is observable. */
#define IGN_CASE(name, fmt) \
TC_CASE(name) { long long a = (long long)0xAAAAAAAAAAAAAAAAULL; int r; tc_iph_install(); tc_iph_count = 0; errno = 0; vin = "123"; \
    r = sscanf((const char *)vin, fmt, &a); \
    CHECK_EQ_I(r, 1); CHECK_EQ_U((unsigned)a, 123); CHECK_EQ_U((unsigned)((unsigned long long)a >> 32), 0xAAAAAAAAu); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
NEG_CASE(pct_l_end, "%l")
NEG_CASE(pct_ll_end, "%ll")
NEG_CASE(pct_h_end, "%h")
IGN_CASE(pct_I_end, "%I")
NEG_CASE(pct_I64_end, "%I64")
IGN_CASE(pct_I16d, "%I16d")
IGN_CASE(pct_I8d, "%I8d")
TC_CASE(wide_pct_l_end) { int a = 99, r; tc_iph_install(); tc_iph_count = 0; errno = 0; vwin = L"123";
    r = swscanf((const wchar_t *)vwin, L"%l", &a);
    CHECK(r <= 0); CHECK_EQ_I(a, 99); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(pct_l_end); TC_RUN(pct_ll_end); TC_RUN(pct_h_end); TC_RUN(pct_I_end); TC_RUN(pct_I64_end);
    TC_RUN(pct_I16d); TC_RUN(pct_I8d); TC_RUN(wide_pct_l_end); return tc_end(); }
