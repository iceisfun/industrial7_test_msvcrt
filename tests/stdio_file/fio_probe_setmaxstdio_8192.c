/* TC
 * id:    fio_probe_setmaxstdio_8192
 * area:  stdio_file
 * funcs: _getmaxstdio _setmaxstdio
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _setmaxstdio (current docs: up to 8192 open files)
 * note:  Probes that _setmaxstdio(8192) succeeds. Restricted to ucrtbase; whether older CRTs cap the stream limit
 *        at 2048 (the lowio handle table size) is unknown, so msvcr* are not listed.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(max8192)
{
    volatile int n = 8192;
    CHECK_EQ_I(_setmaxstdio(n), 8192); CHECK_EQ_I(_getmaxstdio(), 8192);
    n = 512; CHECK_EQ_I(_setmaxstdio(n), 512);
}
int main(void) { tc_begin(); TC_RUN(max8192); return tc_end(); }
