/* TC
 * id:    sec_neg_wcstombs_s_eilseq_probe
 * area:  secure_crt
 * funcs: wcstombs_s wcrtomb_s wctomb_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcstombs_s/wcrtomb_s/wctomb_s (EILSEQ on unconvertible char)
 * note:  PROBE: in the C locale only wchar 0..0xFF believed convertible; U+0100 and U+4E2D fail with EILSEQ (42). 0x80..0xFF deliberately not asserted.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
static volatile wchar_t v_wc;
TC_CASE(U0100_wcstombs_s_EILSEQ) { char b[8]; size_t n = 99; tc_iph_install(); CHECK_EQ_I(wcstombs_s(&n, b, sizeof b, L"\x0100", 4), EILSEQ); }
TC_CASE(U4E2D_wcstombs_s_EILSEQ) { char b[8]; size_t n = 99; tc_iph_install(); CHECK_EQ_I(wcstombs_s(&n, b, sizeof b, L"\x4e2d", 4), EILSEQ); }
TC_CASE(wcrtomb_s_U0100_EILSEQ) { char b[8]; size_t n = 99; mbstate_t st; memset(&st, 0, sizeof st); tc_iph_install(); v_wc = 0x100;
    CHECK_EQ_I(wcrtomb_s(&n, b, sizeof b, v_wc, &st), EILSEQ); }
TC_CASE(wctomb_s_U0100_EILSEQ) { char b[8]; int n = 99; tc_iph_install(); v_wc = 0x100;
    CHECK_EQ_I(wctomb_s(&n, b, sizeof b, v_wc), EILSEQ); }
TC_CASE(errno_EILSEQ) { char b[8]; size_t n = 99; tc_iph_install(); errno = 0; wcstombs_s(&n, b, sizeof b, L"\x0100", 4); CHECK_EQ_I(errno, EILSEQ); }
int main(void) { tc_begin(); TC_RUN(U0100_wcstombs_s_EILSEQ); TC_RUN(U4E2D_wcstombs_s_EILSEQ); TC_RUN(wcrtomb_s_U0100_EILSEQ);
    TC_RUN(wctomb_s_U0100_EILSEQ); TC_RUN(errno_EILSEQ); return tc_end(); }
