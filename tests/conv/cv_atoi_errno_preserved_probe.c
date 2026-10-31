/* TC
 * id:    cv_atoi_errno_preserved_probe
 * area:  conv
 * funcs: atoi
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.5p3 (errno never set to zero by library; may be set nonzero only on error)
 * note:  PROBE: a successful atoi("17") leaves a pre-set errno value untouched. Not documented by Microsoft.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(untouched) { volatile const char *s = "17"; int r; errno = 12345; r = atoi((const char *)s); CHECK_EQ_I(r, 17); CHECK_EQ_I(errno, 12345); }

int main(void) { tc_begin(); TC_RUN(untouched); return tc_end(); }
