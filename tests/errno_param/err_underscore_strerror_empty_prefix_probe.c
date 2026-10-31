/* TC
 * id:    err_underscore_strerror_empty_prefix_probe
 * area:  errno_param
 * funcs: _strerror strerror
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _strerror
 * note:  PROBE: empty-string prefix yields 'message\n' with no ': ' (CRT tests *prefix as well as prefix != NULL).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile int v_e;

TC_CASE(empty_prefix)
{
    char want[128]; const char *r, *t;
    v_e = EINVAL; t = strerror(v_e); CHECK(t != NULL); if (!t) return;
    strcpy(want, t); strcat(want, "\n");
    errno = EINVAL; r = _strerror("");
    CHECK(r != NULL); if (!r) return;
    CHECK_STR(r, want);
}
int main(void) { tc_begin(); TC_RUN(empty_prefix); return tc_end(); }
