/* TC
 * id:    pfx_neg_vfprintf_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: vfprintf fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vfprintf, vfprintf_l, vfwprintf, vfwprintf_l: NULL stream or format invokes the invalid parameter handler, returns -1, errno EINVAL
 * note:  "_set_invalid_parameter_handler" is installed through tc_iph_install (resolved from the DLL).
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int __cdecl vf(FILE *f, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt); r = vfprintf(f, fmt, ap); va_end(ap);
    return r;
}
static FILE *volatile g_null_stream;
static const char *volatile g_null_fmt;
static const char *volatile g_fmt = "x%d";

TC_CASE(null_stream)
{
    int r; tc_iph_count = 0; errno = 0;
    r = vf(g_null_stream, g_fmt, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    FILE *f = fopen("tc_pfx_neg_vfprintf_null_args_" TCV ".tmp", "w");
    int r;
    CHECK(f != NULL);
    if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = vf(f, g_null_fmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
    DeleteFileA("tc_pfx_neg_vfprintf_null_args_" TCV ".tmp");
}
TC_CASE(both_null)
{
    int r; tc_iph_count = 0; errno = 0;
    r = vf(g_null_stream, g_null_fmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(null_stream); TC_RUN(null_format); TC_RUN(both_null);
    return tc_end();
}
