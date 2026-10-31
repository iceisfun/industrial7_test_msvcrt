/* TC
 * id:    fio_probe_ftell_large_overflow
 * area:  stdio_file
 * funcs: ftell fopen fclose
 * procs: _fseeki64
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs ftell (long is 32-bit); no documented overflow behaviour
 * note:  Alternative real behaviour could be a silently truncated long (-2147483648); this probe records which. PROBED: ftell at a position >= 2GiB (not representable in 32-bit long) returns -1 and sets errno to EOVERFLOW or EINVAL. Position is reached by _fseeki64 without writing data.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, _fseeki64, (FILE *, long long, int));
static char nm[] = "tc_fio_probe_ftell_large_overflow_000.tmp";  /* per-CRT suffix: targets may run concurrently */
static void setnm(void) { int v = TC_CRT_VER; char *p = nm + sizeof nm - 9; p[0]=(char)('0'+v/100); p[1]=(char)('0'+v/10%10); p[2]=(char)('0'+v%10); }

TC_CASE(ftell_minus1)
{
    FILE *f; long r;
    if (!tc_p__fseeki64) TC_SKIP("_fseeki64 not exported");
    f = fopen(nm, "w+b"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(tc_p__fseeki64(f, 0x80000000LL, SEEK_SET), 0);
    errno = 0; r = ftell(f);
    CHECK_EQ_I(r, -1);
    fclose(f);
}
TC_CASE(errno_value)
{
    FILE *f; long r;
    if (!tc_p__fseeki64) TC_SKIP("_fseeki64 not exported");
    f = fopen(nm, "w+b"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(tc_p__fseeki64(f, 0x80000000LL, SEEK_SET), 0);
    errno = 0; r = ftell(f);
    if (r == -1) CHECK(errno == EOVERFLOW || errno == EINVAL);
    fclose(f);
}
int main(void)
{
    tc_begin(); setnm(); TC_LOAD_PROC(_fseeki64);
    TC_RUN(ftell_minus1); TC_RUN(errno_value);
    remove(nm); return tc_end();
}
