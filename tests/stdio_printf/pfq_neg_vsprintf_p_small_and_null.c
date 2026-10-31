/* TC
 * id:    pfq_neg_vsprintf_p_small_and_null
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsprintf_p _vswprintf_p _vsprintf_p_l
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _sprintf_p, _sprintf_p_l, _swprintf_p, _vsprintf_p...: NULL buffer/format or too-small buffer -> invalid parameter handler, returns -1
 * note:  Guard bytes beyond the stated count must stay intact (no overrun).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

#define G 0x5A
static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsprintf_p(b, n, f, ap); va_end(ap); return r; }
static int cw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); 
#ifdef _UCRT
    r = __stdio_common_vswprintf_p(0, b, n, f, NULL, ap);
#else
    r = _vswprintf_p(b, n, f, ap);
#endif
    va_end(ap); return r; }
static int cl(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsprintf_p_l(b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(narrow_small)
{
    char b[16]; volatile size_t n = 4; int i, ok = 1, r;
    for (i = 0; i < 16; i++) b[i] = G;
    tc_iph_count = 0;
    r = cs(b, n, "%1$s", "abcdefgh");
    CHECK(r < 0);
    for (i = 4; i < 16; i++) if ((unsigned char)b[i] != G) ok = 0;
    CHECK(ok);
}
TC_CASE(narrow_null_buffer)
{
    char * volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = cs(b, 16, "%1$d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(wide_small)
{
    wchar_t w[16]; volatile size_t n = 4; int i, ok = 1, r;
    for (i = 0; i < 16; i++) w[i] = G;
    r = cw(w, n, L"%1$s", L"abcdefgh");
    CHECK(r < 0);
    for (i = 4; i < 16; i++) if (w[i] != G) ok = 0;
    CHECK(ok);
}
TC_CASE(l_null_format)
{
    char b[16]; const char * volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = cl(b, sizeof b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(va_end_after_failure)
{
    char b[8]; volatile size_t n = 2; const char * volatile f = "%1$s %2$d";
    int r;
    r = cs(b, n, f, "toolongstring", 1);   /* wrapper does va_start/va_end itself after the failure */
    CHECK(r < 0);
    r = cs(b, sizeof b, "%1$d", 9);        /* CRT still fully usable afterwards */
    CHECK_EQ_I(r, 1);
    CHECK_STR(b, "9");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(narrow_small); TC_RUN(narrow_null_buffer); TC_RUN(wide_small); TC_RUN(l_null_format); TC_RUN(va_end_after_failure);
    return tc_end();
}
