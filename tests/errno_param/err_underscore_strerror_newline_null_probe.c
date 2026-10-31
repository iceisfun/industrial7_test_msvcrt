/* TC
 * id:    err_underscore_strerror_newline_null_probe
 * area:  errno_param
 * funcs: _strerror
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _strerror
 * note:  PROBE: trailing newline is also appended when the prefix is NULL.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(null_newline)
{
    const char *r; size_t n; errno = ENOENT; r = _strerror(NULL);
    CHECK(r != NULL); if (!r) return;
    n = strlen(r); CHECK(n > 0 && r[n - 1] == '\n');
}
int main(void) { tc_begin(); TC_RUN(null_newline); return tc_end(); }
