/* TC
 * id:    wcs_wcsncpy_padding_large
 * area:  wstring
 * funcs: wcsncpy
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.2.2 (pads with null wide characters)
 * note:  
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(pad64) { wchar_t d[70]; int i; static const wchar_t s[] = L"ab"; const wchar_t *volatile p = s;
    for (i = 0; i < 70; i++) d[i] = 0x5555;
    wcsncpy(d, p, 64);
    CHECK_EQ_I(d[0], 'a'); CHECK_EQ_I(d[1], 'b');
    for (i = 2; i < 64; i++) if (d[i] != 0) { CHECK_EQ_I(i, -1); break; }
    for (i = 64; i < 70; i++) CHECK_EQ_I(d[i], 0x5555); }
TC_CASE(sentinel) { wchar_t d[300]; int i; static const wchar_t s[] = L"x"; const wchar_t *volatile p = s;
    for (i = 0; i < 300; i++) d[i] = 0x7777;
    wcsncpy(d, p, 256);
    CHECK_EQ_I(d[0], 'x'); CHECK_EQ_I(d[255], 0); CHECK_EQ_I(d[256], 0x7777); CHECK_EQ_I(d[299], 0x7777); }
TC_CASE(empty_src) { wchar_t d[10]; int i; static const wchar_t s[] = L""; const wchar_t *volatile p = s;
    for (i = 0; i < 10; i++) d[i] = 0x5555;
    wcsncpy(d, p, 8);
    for (i = 0; i < 8; i++) if (d[i] != 0) { CHECK_EQ_I(i, -1); break; }
    CHECK_EQ_I(d[8], 0x5555); CHECK_EQ_I(d[9], 0x5555); }
int main(void) { tc_begin(); TC_RUN(pad64); TC_RUN(sentinel); TC_RUN(empty_src); return tc_end(); }
