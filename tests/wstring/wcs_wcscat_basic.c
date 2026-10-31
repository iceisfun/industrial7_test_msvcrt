/* TC
 * id:    wcs_wcscat_basic
 * area:  wstring
 * funcs: wcscat
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.3.1; MS docs strcat, wcscat
 * note:  
 */
#include "tc.h"
#include <wchar.h>

static const wchar_t *volatile pa = L"foo", *volatile pb = L"bar", *volatile pe = L"", *volatile pc = L"baz";

TC_CASE(to_empty) { wchar_t d[16] = { 0 }; wcscat(d, pa); CHECK_WSTR(d, L"foo"); }
TC_CASE(append_empty) { wchar_t d[16] = L"foo"; wcscat(d, pe); CHECK_WSTR(d, L"foo"); CHECK_EQ_I(d[4], 0); }
TC_CASE(normal) { wchar_t d[16] = L"foo"; wcscat(d, pb); CHECK_WSTR(d, L"foobar"); CHECK_EQ_I(d[6], 0); }
TC_CASE(ret) { wchar_t d[16] = L"x"; CHECK_EQ_P(wcscat(d, pa), d); }
TC_CASE(chain) { wchar_t d[16] = { 0 }; wcscat(wcscat(wcscat(d, pa), pb), pc); CHECK_WSTR(d, L"foobarbaz"); }
TC_CASE(high) { wchar_t d[16] = { 'a', 0 }; static const wchar_t s[] = { 0xFFFF, 0x8000, 0 }; const wchar_t *volatile p = s;
    wcscat(d, p); CHECK_EQ_I(d[1], 0xFFFF); CHECK_EQ_I(d[2], 0x8000); CHECK_EQ_I(d[3], 0); }
int main(void) { tc_begin(); TC_RUN(to_empty); TC_RUN(append_empty); TC_RUN(normal); TC_RUN(ret); TC_RUN(chain); TC_RUN(high); return tc_end(); }
