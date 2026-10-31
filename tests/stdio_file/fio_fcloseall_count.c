/* TC
 * id:    fio_fcloseall_count
 * area:  stdio_file
 * funcs: _fcloseall fopen
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _fcloseall
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

TC_CASE(count)
{
    static const char *n[3] = { "tc_fio_fcloseall_count_a.tmp", "tc_fio_fcloseall_count_b.tmp", "tc_fio_fcloseall_count_c.tmp" };
    FILE *f[3]; int i; volatile int r1, r2;
    _fcloseall();  /* normalise */
    for (i = 0; i < 3; i++) { f[i] = fopen(n[i], "w"); CHECK(f[i] != NULL); }
    r1 = _fcloseall();
    CHECK_EQ_I(r1, 3);
    r2 = _fcloseall();
    CHECK_EQ_I(r2, 0);
    for (i = 0; i < 3; i++) DeleteFileA(n[i]);
}
TC_CASE(std_streams_survive)
{
    _fcloseall();
    CHECK_EQ_I(_fileno(stdout), 1);
    CHECK_EQ_I(_fileno(stdin), 0);
    CHECK_EQ_I(_fileno(stderr), 2);
}
int main(void) { tc_begin(); TC_RUN(count); TC_RUN(std_streams_survive); return tc_end(); }
