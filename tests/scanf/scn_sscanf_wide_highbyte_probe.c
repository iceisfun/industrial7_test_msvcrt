/* TC
 * id:    scn_sscanf_wide_highbyte_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   MS docs scanf, mbtowc
 * note:  PROBE: C locale, byte 0xE9 read by %lc. UCRT/msvcr may map 1:1 (return 1, 0xE9) or fail the conversion (return 0). Asserts the 1:1 mapping.
 *        Win10 22H2 ucrtbase (observed 2026-10-31, stable over 2 runs): in the C locale sscanf "%lc" of the byte 0xE9 stores 0xFFE9: the char is sign-extended to wchar_t instead of mapped to U+00E9 (n==1).
 *        Deviates from C11 7.21.6.2 (mbrtowc of a single-byte C-locale char). setlocale(LC_ALL, "C") is now explicit for determinism.
 *        Gated: the observed values apply to ucrtbase only (#ifdef TC_UCRT); other CRTs keep the original (unverified, probe) expectation until they are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>

static volatile const char *v;
#ifdef TC_UCRT
TC_CASE(e9) { wchar_t w = 0; int n; setlocale(LC_ALL, "C"); v = "\xE9"; n = sscanf((const char *)v, "%lc", &w); CHECK_EQ_I(n, 1); CHECK_EQ_U(w, 0xFFE9); }
#else
TC_CASE(e9) { wchar_t w = 0; int n; v = "\xE9"; n = sscanf((const char *)v, "%lc", &w); CHECK_EQ_I(n, 1); CHECK_EQ_U(w, 0xE9); }
#endif
int main(void) { tc_begin(); TC_RUN(e9); return tc_end(); }
