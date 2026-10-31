/* TC
 * id:    fio_rmtmp_count
 * area:  stdio_file
 * funcs: _rmtmp tmpfile
 * crt:   win10
 * tier:  p2
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _rmtmp
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
TC_CASE(count)
{
    FILE *a, *b; volatile int r1, r2;
    _rmtmp();  /* normalise */
    a = tmpfile(); b = tmpfile();
    CHECK(a != NULL); CHECK(b != NULL);
    r1 = _rmtmp();
    CHECK_EQ_I(r1, 2);
    r2 = _rmtmp();
    CHECK_EQ_I(r2, 0);
}
int main(void) { tc_begin(); TC_RUN(count); return tc_end(); }
