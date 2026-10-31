/* TC
 * id:    wcs_wcschr_highunits
 * area:  wstring
 * funcs: wcschr wcsrchr
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.1, 7.29.4.5.5
 * note:  values above 0xFFFF are avoided
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const wchar_t *volatile vs;
static volatile wchar_t vc;
static const wchar_t pair[] = { 'a', 0xD83D, 0xDE00, 'b', 0 };
static const wchar_t two[] = { 0xD83D, 0xDE00, 'x', 0xD83D, 0xDE01, 0 };
TC_CASE(ffff) { static const wchar_t s[] = { 'a', 0xFFFF, 'b', 0 }; vs = s; vc = 0xFFFF; CHECK_EQ_P(wcschr(vs, vc), s + 1); CHECK_EQ_P(wcsrchr(vs, vc), s + 1); }
TC_CASE(u8000) { static const wchar_t s[] = { 'a', 0x8000, 0 }; vs = s; vc = 0x8000; CHECK_EQ_P(wcschr(vs, vc), s + 1); }
TC_CASE(lead) { vs = pair; vc = 0xD83D; CHECK_EQ_P(wcschr(vs, vc), pair + 1); }
TC_CASE(trail) { vs = pair; vc = 0xDE00; CHECK_EQ_P(wcschr(vs, vc), pair + 2); }
TC_CASE(rchr_two_pairs) { vs = two; vc = 0xD83D; CHECK_EQ_P(wcsrchr(vs, vc), two + 3); CHECK_EQ_P(wcschr(vs, vc), two); }
TC_CASE(no_narrowing) { static const wchar_t s[] = { 'a', 0x0161, 0 }; vs = s; vc = 0x0161; CHECK_EQ_P(wcschr(vs, vc), s + 1);
    vc = 0x0061; CHECK_EQ_P(wcschr(vs, vc), s); { static const wchar_t t[] = { 0x0161, 0 }; vs = t; vc = 0x0061; CHECK(wcschr(vs, vc) == NULL); } }
int main(void) { tc_begin(); TC_RUN(ffff); TC_RUN(u8000); TC_RUN(lead); TC_RUN(trail); TC_RUN(rchr_two_pairs); TC_RUN(no_narrowing); return tc_end(); }
