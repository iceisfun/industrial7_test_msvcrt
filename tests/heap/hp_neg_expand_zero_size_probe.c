/* TC
 * id:    hp_neg_expand_zero_size_probe
 * area:  heap
 * funcs: _expand malloc free
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _expand (does not document size 0)
 * note:  PROBE: _expand(p, 0) on a live block. Unknown whether Windows returns NULL (with or without errno) or p
 *        (shrunk to a minimal block). Only asserted: result is NULL or p, and the block can still be freed.
 *        Observed values are written to stdout as TC INFO lines.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>

static void info(const char *k, long long v)
{
    char b[32]; tc_write("TC INFO " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n");
}
TC_CASE(expand_zero)
{
    volatile size_t z = 0; void *p, *r;
    tc_iph_install();
    p = malloc(16);
    CHECK(p != NULL);
    if (!p) return;
    tc_iph_count = 0; errno = 0;
    r = _expand(p, z);
    info("result_is_null", r == NULL); info("result_is_p", r == p);
    info("errno", errno); info("iph", tc_iph_count);
    CHECK(r == NULL || r == p);
    free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(expand_zero);
    return tc_end();
}
