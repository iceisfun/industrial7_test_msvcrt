/* TC
 * id:    hp_probe_free_null_errno
 * area:  heap
 * funcs: free
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.3.3
 * note:  PROBING that free(NULL) leaves errno untouched (believed true on all MS CRTs; not documented).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(errno_unchanged)
{
    void *volatile n = NULL; errno = 1234; free(n); CHECK_EQ_I(errno, 1234);
    errno = 0; free(n); free(n); CHECK_EQ_I(errno, 0);
}
int main(void) { tc_begin(); TC_RUN(errno_unchanged); return tc_end(); }
