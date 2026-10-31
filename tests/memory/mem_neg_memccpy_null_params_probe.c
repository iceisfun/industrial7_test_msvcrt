/* TC
 * id:    mem_neg_memccpy_null_params_probe
 * area:  memory
 * kind:  negative
 * funcs: _memccpy
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _memccpy (parameter validation not documented in detail)
 * note:  PROBES that _memccpy validates NULL dest / NULL src with count > 0: returns NULL, invokes the handler once,
 *        sets errno EINVAL. Believed for ucrtbase; older CRTs (msvcr80-120, whose _memccpy may be unvalidated
 *        and would crash) are deliberately not listed.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static char buf[8] = "abcdefg";
static void *volatile v_null = NULL;
static volatile size_t v_n = 4;

TC_CASE(dest_null_returns_null_handler_called_errno_EINVAL)
{
    void *r; tc_iph_count = 0; errno = 0;
    r = _memccpy(v_null, buf, 'z', v_n);
    CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(src_null_returns_null_handler_called_errno_EINVAL)
{
    void *r; tc_iph_count = 0; errno = 0;
    r = _memccpy(buf, v_null, 'z', v_n);
    CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup no handler API\n"); return 1; }
    TC_RUN(dest_null_returns_null_handler_called_errno_EINVAL); TC_RUN(src_null_returns_null_handler_called_errno_EINVAL);
    return tc_end();
}
