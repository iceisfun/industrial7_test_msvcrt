/* TC
 * id:    pfx_probe_neg_fprintf_s_null_arg
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fprintf_s (secure variants validate arguments)
 * note:  Probing that a NULL pointer for %s is rejected (-1, EINVAL, iph 1) by fprintf_s instead of printing "(null)".
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <errno.h>

#define TMP "tc_pfx_probe_neg_fprintf_s_null_arg_" TCV ".tmp"
static const char *volatile g_s_fmt = "%s";
static const char *volatile g_nullstr;

TC_CASE(null_string_arg)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf_s(f, g_s_fmt, g_nullstr);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(null_string_arg);
    return tc_end();
}
