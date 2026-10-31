/* TC
 * id:    io_neg_pipe_textmode_garbage
 * area:  lowio
 * funcs: _pipe _close _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _pipe (textmode: _O_TEXT or _O_BINARY)
 * note:  Probes: _pipe with an undefined textmode bit (_O_APPEND) fails with -1, errno EINVAL and one invalid-parameter-handler call. Most likely outcome per the _pipe validation; if real Windows instead succeeds, the result is informational.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define N "tc_io_neg_pipe_textmode_garbage_" TC_CRT_DLL ".tmp"

TC_CASE(append_bit)
{
    int f[2] = { -1, -1 }, r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _pipe(f, 0, _O_APPEND);
    if (r == 0) { _close(f[0]); _close(f[1]); }
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_RUN(append_bit);
    return tc_end();
}
