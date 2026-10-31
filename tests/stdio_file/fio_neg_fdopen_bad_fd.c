/* TC
 * id:    fio_neg_fdopen_bad_fd
 * area:  stdio_file
 * funcs: _fdopen
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _fdopen
 * note:  Invalid descriptor: NULL + EBADF. Handler installed; its count is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static volatile int bad1 = -1, bad2 = 9999;

TC_CASE(minus_one)  { FILE *f; errno = 0; f = _fdopen(bad1, "r"); CHECK(f == NULL); CHECK_EQ_I(errno, EBADF); if (f) fclose(f); }
TC_CASE(too_big)    { FILE *f; errno = 0; f = _fdopen(bad2, "r"); CHECK(f == NULL); CHECK_EQ_I(errno, EBADF); if (f) fclose(f); }
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(minus_one); TC_RUN(too_big);
    return tc_end();
}
