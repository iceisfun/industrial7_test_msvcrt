/* TC
 * id:    hp_probe_aligned_free_null_errno
 * area:  heap
 * funcs: _aligned_free
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _aligned_free; C11 7.22.3.3 (free(NULL) no action)
 * note:  PROBING: _aligned_free(NULL) leaves errno untouched. Docs only say the NULL pointer is ignored; errno
 *        preservation is the expected (but undocumented) consequence of an early return.
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>

static void *volatile v_null = NULL;

TC_CASE(errno_unchanged)
{
    errno = 1234;
    _aligned_free(v_null);
    CHECK_EQ_I(errno, 1234);
}
int main(void) { tc_begin(); TC_RUN(errno_unchanged); return tc_end(); }
