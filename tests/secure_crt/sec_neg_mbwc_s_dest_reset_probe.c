/* TC
 * id:    sec_neg_mbwc_s_dest_reset_probe
 * area:  secure_crt
 * funcs: wcstombs_s mbstowcs_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcstombs_s, mbstowcs_s (destination state after failure not tabulated)
 * note:  PROBE: after ERANGE (buffer too small) and after EILSEQ (U+0100 in the C locale) the destination's first element is believed
 *        to be set to 0, as for the strcpy_s family. Only dest[0] is asserted.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
static volatile size_t v_sz = 4;
TC_CASE(wcstombs_s_ERANGE_dest0) { char b[4]; size_t n = 99; tc_iph_install(); memset(b, 'x', sizeof b);
    CHECK_EQ_I(wcstombs_s(&n, b, v_sz, L"abcdefgh", 8), ERANGE); CHECK_EQ_I(b[0], 0); }
TC_CASE(mbstowcs_s_ERANGE_dest0) { wchar_t b[4]; size_t n = 99; int i; tc_iph_install(); for (i = 0; i < 4; i++) b[i] = L'x';
    CHECK_EQ_I(mbstowcs_s(&n, b, v_sz, "abcdefgh", 8), ERANGE); CHECK_EQ_I(b[0], 0); }
TC_CASE(wcstombs_s_EILSEQ_dest0) { char b[8]; size_t n = 99; tc_iph_install(); memset(b, 'x', sizeof b);
    CHECK_EQ_I(wcstombs_s(&n, b, sizeof b, L"\x0100", 4), EILSEQ); CHECK_EQ_I(b[0], 0); }
int main(void) { tc_begin(); TC_RUN(wcstombs_s_ERANGE_dest0); TC_RUN(mbstowcs_s_ERANGE_dest0); TC_RUN(wcstombs_s_EILSEQ_dest0); return tc_end(); }
