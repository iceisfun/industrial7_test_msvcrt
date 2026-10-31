/* TC
 * id:    pfx_probe_neg_fprintf_p_bad_position
 * area:  stdio_printf
 * kind:  negative
 * funcs: _fprintf_p fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _printf_p, _fprintf_p (positional parameters)
 * note:  Probing -1 / EINVAL for position number 0 ("%0$d") and for mixing positional and sequential specifiers.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <errno.h>

#define TMP "tc_pfx_probe_neg_fprintf_p_bad_position_" TCV ".tmp"
static const char *volatile g_zero = "%0$d";
static const char *volatile g_mix = "%1$d %d";

TC_CASE(position_zero)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = _fprintf_p(f, g_zero, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(mixed)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = _fprintf_p(f, g_mix, 1, 2);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);
    fclose(f); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(position_zero); TC_RUN(mixed);
    return tc_end();
}
