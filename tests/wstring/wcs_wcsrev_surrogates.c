/* TC
 * id:    wcs_wcsrev_surrogates
 * area:  wstring
 * funcs: _wcsrev
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  inferred
 * ref:   MS docs _wcsrev
 * note:  _wcsrev reverses wchar_t code units and has no surrogate-pair awareness, so pairs end up swapped (invalid UTF-16).
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(pair) { wchar_t b[] = { 0xD83D, 0xDE00, 0 }; wchar_t *volatile p = b; _wcsrev(p); CHECK_EQ_U(b[0], 0xDE00); CHECK_EQ_U(b[1], 0xD83D); CHECK_EQ_U(b[2], 0); }
TC_CASE(pair_between) { wchar_t b[] = { L'a', 0xD83D, 0xDE00, L'b', 0 }; wchar_t *volatile p = b; _wcsrev(p);
    CHECK_EQ_U(b[0], L'b'); CHECK_EQ_U(b[1], 0xDE00); CHECK_EQ_U(b[2], 0xD83D); CHECK_EQ_U(b[3], L'a'); CHECK_EQ_U(b[4], 0); }
TC_CASE(ffff) { wchar_t b[] = { 0xFFFF, 0x0001, 0 }; wchar_t *volatile p = b; _wcsrev(p); CHECK_EQ_U(b[0], 0x0001); CHECK_EQ_U(b[1], 0xFFFF); CHECK_EQ_U(b[2], 0); }

int main(void) { tc_begin(); TC_RUN(pair); TC_RUN(pair_between); TC_RUN(ffff); return tc_end(); }
