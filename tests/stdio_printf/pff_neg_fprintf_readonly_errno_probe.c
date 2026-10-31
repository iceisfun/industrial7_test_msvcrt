/* TC
 * id:    pff_neg_fprintf_readonly_errno_probe
 * area:  stdio_printf
 * funcs: fprintf
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fprintf / errno
 * note:  Probes: errno after fprintf on a stream opened "r" is EBADF (9). Not documented explicitly.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char tmp[] = "tc_pff_neg_fprintf_readonly_errno_probe.tmp";

TC_CASE(errno_ebadf)
{
    FILE *fp; int n;
    fp = fopen(tmp, "w"); CHECK(fp != NULL);
    if (!fp) return;
    fputs("x", fp); fclose(fp);
    tc_iph_install();
    fp = fopen(tmp, "r"); CHECK(fp != NULL);
    if (fp) {
        errno = 0;
        n = fprintf(fp, "%f", 1.5);
        CHECK(n < 0);
        CHECK_EQ_I(errno, EBADF);
        fclose(fp);
    }
    remove(tmp);
}

int main(void) { tc_begin(); TC_RUN(errno_ebadf); return tc_end(); }
