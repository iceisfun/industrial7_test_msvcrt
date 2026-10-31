/* TC
 * id:    fio_neg_fopen_s_missing_file
 * area:  stdio_file
 * kind:  negative
 * funcs: 
 * procs: fopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen_s: returns zero on success, an error code on failure
 * note:  Missing file: return value and errno both ENOENT.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, fopen_s, (FILE **, const char *, const char *));
static const char * volatile fname = "tc_fio_neg_fopen_s_missing_file_a.tmp";

TC_CASE(missing)
{
    FILE *f = (FILE *)(ULONG_PTR)0x1000; int r;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    errno = 0;
    r = tc_p_fopen_s(&f, fname, "r");
    CHECK_EQ_I(r, ENOENT);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(missing_readplus)
{
    FILE *f = (FILE *)(ULONG_PTR)0x1000; int r;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, fname, "r+");
    CHECK_EQ_I(r, ENOENT);
    CHECK(f == NULL);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fname);
    TC_LOAD_PROC(fopen_s);
    CHECK(tc_p_fopen_s != NULL);
    TC_RUN(missing); TC_RUN(missing_readplus);
    return tc_end();
}
