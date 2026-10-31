/* TC
 * id:    wcs_wcsncpy_basic
 * area:  wstring
 * funcs: wcsncpy
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.2.2; MS docs strncpy, wcsncpy
 * note:  
 */
#include "tc.h"
#include <wchar.h>

static void fill(wchar_t *d, int n) { int i; for (i = 0; i < n; i++) d[i] = 0x5555; }
static const wchar_t src[] = L"abc";
static const wchar_t *volatile ps = src;

TC_CASE(pad) { wchar_t d[8]; int i; fill(d, 8); wcsncpy(d, ps, 6);
    CHECK_EQ_I(d[0], 'a'); CHECK_EQ_I(d[2], 'c'); for (i = 3; i < 6; i++) CHECK_EQ_I(d[i], 0); CHECK_EQ_I(d[6], 0x5555); CHECK_EQ_I(d[7], 0x5555); }
TC_CASE(exact) { wchar_t d[6]; fill(d, 6); wcsncpy(d, ps, 3);
    CHECK_EQ_I(d[2], 'c'); CHECK_EQ_I(d[3], 0x5555); }
TC_CASE(truncate) { wchar_t d[6]; fill(d, 6); wcsncpy(d, ps, 2);
    CHECK_EQ_I(d[0], 'a'); CHECK_EQ_I(d[1], 'b'); CHECK_EQ_I(d[2], 0x5555); }
TC_CASE(zero) { wchar_t d[4]; fill(d, 4); CHECK_EQ_P(wcsncpy(d, ps, 0), d); CHECK_EQ_I(d[0], 0x5555); CHECK_EQ_I(d[1], 0x5555); }
TC_CASE(ret) { wchar_t d[8]; fill(d, 8); CHECK_EQ_P(wcsncpy(d, ps, 5), d); }
int main(void) { tc_begin(); TC_RUN(pad); TC_RUN(exact); TC_RUN(truncate); TC_RUN(zero); TC_RUN(ret); return tc_end(); }
