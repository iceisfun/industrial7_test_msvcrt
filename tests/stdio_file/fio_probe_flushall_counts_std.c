/* TC
 * id:    fio_probe_flushall_counts_std
 * area:  stdio_file
 * funcs: _flushall
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _flushall
 * note:  PROBE: whether stdin/stdout/stderr are included in the _flushall count when no user streams are open.\n */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

TC_CASE(std_counted)
{
    _fcloseall();
    CHECK_EQ_I(_flushall(), 3);
}
int main(void) { tc_begin(); TC_RUN(std_counted); return tc_end(); }
