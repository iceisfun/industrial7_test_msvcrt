/* TC
 * id:    pfp_ptr_wide
 * area:  stdio_printf
 * funcs: swprintf
 * crt:   >=140
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs swprintf, p type
 * note:  ucrt swprintf has the C99 (buf,count,fmt) signature; x64 only.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#ifdef _WIN64
TC_CASE(basic)  { wchar_t b[64]; void *volatile p = (void *)(size_t)0xDEADBEEF00112233ULL; int n = swprintf(b, 64, L"%p", p); CHECK_WSTR(b, L"DEADBEEF00112233"); CHECK_EQ_I(n, 16); }
TC_CASE(null_ptr) { wchar_t b[64]; void *volatile p = NULL; int n = swprintf(b, 64, L"%p", p); CHECK_WSTR(b, L"0000000000000000"); CHECK_EQ_I(n, 16); }
TC_CASE(mix)    { wchar_t b[64]; void *volatile p = (void *)(size_t)0x1F; const wchar_t *volatile s = L"ab"; int n = swprintf(b, 64, L"%ls:%p:%ls", s, p, s); CHECK_WSTR(b, L"ab:000000000000001F:ab"); CHECK_EQ_I(n, 22); }
#else
TC_CASE(skip32) { TC_SKIP("x64 only"); }
#endif

int main(void)
{
    tc_begin();
#ifdef _WIN64
    TC_RUN(basic); TC_RUN(null_ptr); TC_RUN(mix);
#else
    TC_RUN(skip32);
#endif
    return tc_end();
}
