/* TC
 * id:    wcs_neg_wcscmp_not_equal_lowbyte
 * area:  wstring
 * funcs: wcscmp wcsncmp _wcsicmp
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.4.4; MS docs wcscmp, _wcsicmp
 * note:  0x141 is not a case partner of 0x41 in any locale
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
static const wchar_t a[] = { 0x0161, 0 };
static const wchar_t b[] = { 0x0061, 0 };
static const wchar_t c[] = { 0x0141, 0 };
static const wchar_t d[] = { 0x0041, 0 };
static const wchar_t *volatile pa = a, *volatile pb = b, *volatile pc = c, *volatile pd = d;
TC_CASE(cmp) { CHECK(wcscmp(pa, pb) > 0); CHECK(wcscmp(pb, pa) < 0); }
TC_CASE(ncmp) { CHECK(wcsncmp(pa, pb, 1) > 0); }
TC_CASE(icmp) { CHECK(_wcsicmp(pc, pd) != 0); }
int main(void) { tc_begin(); TC_RUN(cmp); TC_RUN(ncmp); TC_RUN(icmp); return tc_end(); }
