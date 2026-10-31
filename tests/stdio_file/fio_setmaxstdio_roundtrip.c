/* TC
 * id:    fio_setmaxstdio_roundtrip
 * area:  stdio_file
 * funcs: _getmaxstdio _setmaxstdio
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _setmaxstdio, _getmaxstdio (default 512)
 * note:  8192 deliberately not asserted: pre-UCRT CRTs may cap the stream limit at 2048; see fio_probe_setmaxstdio_8192.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(roundtrip)
{
    volatile int n;
    CHECK_EQ_I(_getmaxstdio(), 512);
    n = 2048; CHECK_EQ_I(_setmaxstdio(n), 2048); CHECK_EQ_I(_getmaxstdio(), 2048);
    n = 512;  CHECK_EQ_I(_setmaxstdio(n), 512);  CHECK_EQ_I(_getmaxstdio(), 512);
}
int main(void) { tc_begin(); TC_RUN(roundtrip); return tc_end(); }
