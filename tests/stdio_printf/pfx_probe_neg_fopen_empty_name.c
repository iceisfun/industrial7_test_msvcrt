/* TC
 * id:    pfx_probe_neg_fopen_empty_name
 * area:  stdio_printf
 * kind:  negative
 * funcs: fopen fclose
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen
 * note:  PROBE: fopen("", "r") returns NULL with errno ENOENT (CreateFile on an empty name fails with
 *        ERROR_PATH_NOT_FOUND which maps to ENOENT). The alternative would be EINVAL via the invalid parameter handler.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile empty = "";

TC_CASE(empty_name)
{
    FILE *f; errno = 0; f = fopen(empty, "r");
    CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); if (f) fclose(f);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(empty_name); return tc_end(); }
