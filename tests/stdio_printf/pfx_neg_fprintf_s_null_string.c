/* TC
 * id:    pfx_neg_fprintf_s_null_string
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fprintf_s, fprintf_s_l: NULL stream or format invoke the invalid parameter handler, return -1, errno EINVAL
 * note:  NULL %s argument is probed in pfx_probe_neg_fprintf_s_null_arg
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <errno.h>

#define TMP "tc_pfx_neg_fprintf_s_null_string_" TCV ".tmp"
static const char *volatile g_s_fmt = "%s";
static const char *volatile g_null_fmt;
static FILE *volatile g_null_stream;

TC_CASE(null_stream)
{
    int r; tc_iph_count = 0; errno = 0; r = fprintf_s(g_null_stream, g_s_fmt, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf_s(f, g_null_fmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(null_stream); TC_RUN(null_format);
    return tc_end();
}
