/* TC
 * id:    wcs_wcscmp_unsigned_highunits
 * area:  wstring
 * funcs: wcscmp
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs wcscmp; wchar_t is unsigned short (typedef in wchar.h/corecrt.h)
 * note:  wcscmp compares by wchar_t value as unsigned 16-bit, so UTF-16 code unit order
 */
#include "tc.h"
#include <wchar.h>

static int cmp1(wchar_t a, wchar_t b) { wchar_t x[2], y[2]; x[0] = a; x[1] = 0; y[0] = b; y[1] = 0;
    { const wchar_t *volatile px = x, *volatile py = y; return wcscmp(px, py); } }
TC_CASE(ffff_gt_a) { CHECK(cmp1(0xFFFF, 'a') > 0); CHECK(cmp1('a', 0xFFFF) < 0); }
TC_CASE(u8000_gt_7fff) { CHECK(cmp1(0x8000, 0x7FFF) > 0); }
TC_CASE(ffff_gt_d800) { CHECK(cmp1(0xFFFF, 0xD800) > 0); }
TC_CASE(d800_lt_e000) { CHECK(cmp1(0xD800, 0xE000) < 0); CHECK(cmp1(0xE000, 0xD800) > 0); }
TC_CASE(u0100_gt_00ff) { CHECK(cmp1(0x0100, 0x00FF) > 0); }
int main(void) { tc_begin(); TC_RUN(ffff_gt_a); TC_RUN(u8000_gt_7fff); TC_RUN(ffff_gt_d800); TC_RUN(d800_lt_e000); TC_RUN(u0100_gt_00ff); return tc_end(); }
