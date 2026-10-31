/* TC
 * id:    pfm_2_neg_wide_spec_state_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf_s fwprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs swprintf_s (buffer state on failure), fwprintf_s
 * note:  PROBES the state after an invalid specification: (1) swprintf_s(b,N,L"%y") leaves b[0]==0; (2) with the invalid spec after
 *        literal text (L"ab%y") b[0] is also 0 (the whole buffer is reset); (3) fwprintf_s(f, L"%y") writes nothing to a temp file
 *        (file size 0 after flush).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(buffer_reset_plain)
{
    wchar_t b[8]; const wchar_t * volatile f = L"%y"; int i; for (i = 0; i < 8; i++) b[i] = L'Q';
    CHECK_EQ_I(swprintf_s(b, 8, f, 1), -1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(buffer_reset_after_literal)
{
    wchar_t b[8]; const wchar_t * volatile f = L"ab%y"; int i; for (i = 0; i < 8; i++) b[i] = L'Q';
    CHECK_EQ_I(swprintf_s(b, 8, f, 1), -1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(stream_nothing_written)
{
    const char *fn = "tc_pfm_2_neg_wide_spec_state_probe_a.tmp"; FILE *f = fopen(fn, "w+"); const wchar_t * volatile fmt = L"%y"; long sz;
    if (!f) { CHECK(f != NULL); return; }
    CHECK_EQ_I(fwprintf_s(f, fmt, 1), -1);
    fflush(f); fseek(f, 0, SEEK_END); sz = ftell(f);
    CHECK_EQ_I(sz, 0);
    fclose(f); remove(fn);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(buffer_reset_plain); TC_RUN(buffer_reset_after_literal); TC_RUN(stream_nothing_written); return tc_end(); }
