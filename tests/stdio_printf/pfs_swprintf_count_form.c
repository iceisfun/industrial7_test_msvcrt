/* TC
 * id:    pfs_swprintf_count_form
 * area:  stdio_printf
 * funcs: swprintf
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs swprintf, _swprintf (UCRT swprintf(buf,count,fmt) is the C99 form); C11 7.29.2.3
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const char *vn = "nar";
static volatile const wchar_t *vw = L"wid";

TC_CASE(s_is_wide) { wchar_t b[16]; int n = swprintf(b, 16, L"%s", (const wchar_t *)vw); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"wid"); }
TC_CASE(variety) { wchar_t b[32]; int n = swprintf(b, 32, L"%ls|%S|%hs", (const wchar_t *)vw, (const char *)vn, (const char *)vn); CHECK_EQ_I(n, 11); CHECK_WSTR(b, L"wid|nar|nar"); }
TC_CASE(exact_fit) { wchar_t b[8]; int n = swprintf(b, 4, L"%ls", (const wchar_t *)vw); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"wid"); }
TC_CASE(chars) { wchar_t b[8]; int n = swprintf(b, 8, L"%c%hc", L'x', (int)'y'); CHECK_EQ_I(n, 2); CHECK_WSTR(b, L"xy"); }
int main(void) { tc_begin(); TC_RUN(s_is_wide); TC_RUN(variety); TC_RUN(exact_fit); TC_RUN(chars); return tc_end(); }
