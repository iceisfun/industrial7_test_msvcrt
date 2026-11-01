/* TC
 * id:    pfm_3_neg_sprintf_s_null_format_buf_probe
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s swprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs sprintf_s ("buffer ... set to empty string" is documented for size errors; for a NULL format it is unspecified)
 * note:  PROBE: after sprintf_s / swprintf_s fails because format is NULL, is buffer[0] == 0 (buffer pre-filled with 'x')?
 *        Return value / errno / handler count are documented and asserted in pfm_3_neg_sprintf_null_format_all_families; here only buffer[0]
 *        is probed.
 *        Win10 22H2 (observed): after a NULL-format failure buffer[0] is left UNCHANGED ('x'), not cleared to 0 (old guess). Raw: b[0] == 0 got=120 want=0.
 *        crt narrowed to ucrtbase: the observed behaviour was measured on Win10 22H2 ucrtbase only (older CRTs [unverified]).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#if TC_CRT_VER >= 140
#define P(n) n
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, swprintf_s, (wchar_t *, size_t, const wchar_t *, ...));
#define P(n) tc_p_##n
#endif
static const char *volatile nf = NULL;
static const wchar_t *volatile nwf = NULL;

TC_CASE(narrow_buf0) { char b[8] = "xxxxxxx"; P(sprintf_s)(b, sizeof b, nf, 1); CHECK_EQ_I(b[0], 'x'); }
TC_CASE(wide_buf0)   { wchar_t b[8] = L"xxxxxxx"; P(swprintf_s)(b, 8, nwf, 1); CHECK_EQ_I(b[0], L'x'); }

int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s); TC_LOAD_PROC(swprintf_s);
    if (!tc_p_sprintf_s || !tc_p_swprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__missing export\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(narrow_buf0); TC_RUN(wide_buf0);
    return tc_end();
}
