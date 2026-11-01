/* TC
 * id:    scn_swscanf_narrow_h_set_probe
 * area:  scanf
 * funcs: swscanf
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   MS docs scanf width/type: h prefix in wscanf
 * note:  PROBE: %h[a-z] in swscanf stores a narrow-char scanset string (docs list h only for c/s explicitly; assumed to extend to [).
 *        Win10 22H2 ucrtbase (observed 2026-10-31, stable over 2 runs): swscanf with the narrow-destination scanset "%h[a-z]" is a matching failure: r==0, the destination and %n untouched (while the plain wide "%[a-z]" converts). C11 does not define %h[ ; deviates from the MS-docs reading that h means narrow for wide functions.
 *        Gated: the observed values apply to ucrtbase only (#ifdef TC_UCRT); other CRTs keep the original (unverified, probe) expectation until they are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const wchar_t *v;

TC_CASE(hset) { char b[16]; int r, k = -1, i; for (i = 0; i < 16; i++) b[i] = 'Z'; v = L"abcXYZ";
#ifdef TC_UCRT
    r = swscanf((const wchar_t *)v, L"%h[a-z]%n", b, &k); CHECK_EQ_I(r, 0); CHECK_EQ_I(b[0], 'Z'); CHECK_EQ_I(b[4], 'Z'); CHECK_EQ_I(k, -1); }
#else
    r = swscanf((const wchar_t *)v, L"%h[a-z]%n", b, &k); CHECK_EQ_I(r, 1); CHECK_STR(b, "abc"); CHECK_EQ_I(k, 3); CHECK_EQ_I(b[4], 'Z'); }
#endif
int main(void) { tc_begin(); TC_RUN(hset); return tc_end(); }
