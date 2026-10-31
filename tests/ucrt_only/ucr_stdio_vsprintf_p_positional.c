/* TC
 * id:    ucr_stdio_vsprintf_p_positional
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf_p __stdio_common_vswprintf_p
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p, _printf_p positional parameters (%n$)
 */
#include "tc.h"
#include <stdarg.h>

typedef int (__cdecl *vsp_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *vswp_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsp_t p_vsp; static vswp_t p_vswp;

static int call(char *b, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsp(0, b, 64, f, NULL, ap); va_end(ap); return r; }
static int callw(wchar_t *b, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vswp(0, b, 64, f, NULL, ap); va_end(ap); return r; }

TC_CASE(reorder)
{
    char b[64]; int r;
    if (!p_vsp) TC_SKIP("not exported");
    r = call(b, "%2$s %1$s", "world", "hello");
    CHECK_STR(b, "hello world"); CHECK_EQ_I(r, 11);
}
TC_CASE(reuse)
{
    char b[64]; int r;
    if (!p_vsp) TC_SKIP("not exported");
    r = call(b, "%1$d %1$d", 7);
    CHECK_STR(b, "7 7"); CHECK_EQ_I(r, 3);
}
TC_CASE(mixed_types)
{
    char b[64];
    if (!p_vsp) TC_SKIP("not exported");
    call(b, "%3$s=%2$d/%1$x", 255, 10, "k");
    CHECK_STR(b, "k=10/ff");
}
TC_CASE(wide)
{
    wchar_t b[64]; int r;
    if (!p_vswp) TC_SKIP("not exported");
    r = callw(b, L"%2$ls %1$ls", L"world", L"hello");
    CHECK_WSTR(b, L"hello world"); CHECK_EQ_I(r, 11);
    r = callw(b, L"%1$d-%1$d", 12);
    CHECK_WSTR(b, L"12-12"); CHECK_EQ_I(r, 5);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vsp = (vsp_t)tc_crt_proc("__stdio_common_vsprintf_p");
    p_vswp = (vswp_t)tc_crt_proc("__stdio_common_vswprintf_p");
    TC_RUN(reorder); TC_RUN(reuse); TC_RUN(mixed_types); TC_RUN(wide);
    return tc_end();
}
