/* TC
 * id:    pfx_neg_probe_fprintf_return_overflow_nul
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fprintf return value; errno EOVERFLOW
 * note:  PROBES that an output count exceeding INT_MAX (width 2147483647 plus one more char) makes ucrt fprintf return -1 with errno EOVERFLOW,
 *        and that exactly INT_MAX chars returns INT_MAX. Writes ~2GB to the NUL device; may take several seconds.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

static const char *volatile g_over = "%2147483647dx";
static const char *volatile g_exact = "%2147483647d";

TC_CASE(overflow)
{
    FILE *f = fopen("NUL", "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf(f, g_over, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EOVERFLOW); CHECK_EQ_I(tc_iph_count, 0);
    fclose(f);
}
TC_CASE(exact_int_max)
{
    FILE *f = fopen("NUL", "w"); int r; CHECK(f != NULL); if (!f) return;
    r = fprintf(f, g_exact, 1);
    CHECK_EQ_I(r, INT_MAX);
    fclose(f);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(overflow); TC_RUN(exact_int_max);
    return tc_end();
}
