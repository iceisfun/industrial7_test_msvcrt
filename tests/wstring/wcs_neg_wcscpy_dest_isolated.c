/* TC
 * id:    wcs_neg_wcscpy_dest_isolated
 * area:  wstring
 * funcs: wcscpy wcscat
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.4.2.1, 7.29.4.3.1
 * note:  must-not-write guard
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
static const wchar_t *volatile ps = L"abc";
static const wchar_t *volatile pt = L"de";
TC_CASE(guards) { wchar_t m[10]; int i; for (i = 0; i < 10; i++) m[i] = 0x5555; wcscpy(m + 2, ps);
    CHECK_EQ_I(m[0], 0x5555); CHECK_EQ_I(m[1], 0x5555); CHECK_WSTR(m + 2, L"abc"); CHECK_EQ_I(m[6], 0x5555); CHECK_EQ_I(m[9], 0x5555); }
TC_CASE(cat_exact_end) { wchar_t m[8]; int i; for (i = 0; i < 8; i++) m[i] = 0x5555; m[0] = 0x5555;
    wcscpy(m + 1, ps); wcscat(m + 1, pt);   /* uses m[1..6], m[1..6] used, m[7] is the guard */
    CHECK_WSTR(m + 1, L"abcde"); CHECK_EQ_I(m[0], 0x5555); CHECK_EQ_I(m[7], 0x5555); }
int main(void) { tc_begin(); TC_RUN(guards); TC_RUN(cat_exact_end); return tc_end(); }
