/* TC
 * id:    fio_tmpfile_s_basic
 * area:  stdio_file
 * funcs:
 * procs: tmpfile_s, fwrite, rewind, fread, fclose
 * crt:   >=140
 * tier:  p1
 * conf:  documented
 * ref:   MS docs tmpfile_s
 * note:  Narrowed to ucrtbase: older CRTs create the file in the drive root, which may fail for non-admin users (see fio_probe_tmpfile_legacy).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, tmpfile_s, (FILE **));

TC_CASE(returns_zero)
{
    FILE *f = (FILE *)1; int r;
    if (!tc_p_tmpfile_s) TC_SKIP("tmpfile_s not exported");
    r = tc_p_tmpfile_s(&f);
    CHECK_EQ_I(r, 0);
    CHECK(f != NULL && f != (FILE *)1);
    if (f && f != (FILE *)1) fclose(f);
}
TC_CASE(round_trip)
{
    FILE *f = NULL; char buf[16] = {0};
    if (!tc_p_tmpfile_s) TC_SKIP("tmpfile_s not exported");
    if (tc_p_tmpfile_s(&f) != 0 || !f) { CHECK(0); return; }
    CHECK_EQ_U(fwrite("abcdef", 1, 6, f), 6);
    rewind(f);
    CHECK_EQ_U(fread(buf, 1, 6, f), 6);
    CHECK_MEM(buf, "abcdef", 6);
    CHECK_EQ_I(fclose(f), 0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(tmpfile_s); CHECK(tc_p_tmpfile_s != NULL); TC_RUN(returns_zero); TC_RUN(round_trip); return tc_end(); }
