/* TC
 * id:    pfx_neg_fwprintf_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf vfwprintf fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, fwprintf, vfwprintf: NULL stream or format invokes the invalid parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

static int __cdecl vfw(FILE *f, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt); r = vfwprintf(f, fmt, ap); va_end(ap);
    return r;
}
static FILE *volatile g_null_stream;
static const wchar_t *volatile g_null_fmt;
static const wchar_t *volatile g_fmt = L"x%d";
#define TMP "tc_pfx_neg_fwprintf_null_args_" TCV ".tmp"

TC_CASE(fw_null_stream)
{
    int r; tc_iph_count = 0; errno = 0;
    r = fwprintf(g_null_stream, g_fmt, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fw_null_format)
{
    FILE *f = fopen(TMP, "w"); int r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = fwprintf(f, g_null_fmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(vfw_null_stream)
{
    int r; tc_iph_count = 0; errno = 0;
    r = vfw(g_null_stream, g_fmt, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vfw_null_format)
{
    FILE *f = fopen(TMP, "w"); int r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = vfw(f, g_null_fmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(fw_null_stream); TC_RUN(fw_null_format); TC_RUN(vfw_null_stream); TC_RUN(vfw_null_format);
    return tc_end();
}
