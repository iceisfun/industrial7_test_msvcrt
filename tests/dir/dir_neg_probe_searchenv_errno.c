/* TC
 * id:    dir_neg_probe_searchenv_errno
 * area:  dir
 * kind:  negative
 * funcs: _searchenv
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _searchenv (errno not documented for the classic function)
 * note:  Probes that a failed _searchenv leaves errno == ENOENT. The docs only promise an empty buffer.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_fn = "tc_dir_absent_file.nope", *volatile v_path = "PATH";

TC_CASE(errno_enoent)
{
    char buf[_MAX_PATH]; errno = 0; buf[0] = 'x';
    _searchenv(v_fn, v_path, buf);
    CHECK_EQ_I(buf[0], 0);
    CHECK_EQ_I(errno, ENOENT);
}
int main(void) { tc_begin(); TC_RUN(errno_enoent); return tc_end(); }
