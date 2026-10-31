/* TC
 * id:    fio_fseeki64_ftelli64_large
 * area:  stdio_file
 * funcs: fopen fclose fwrite fflush
 * procs: _fseeki64 _ftelli64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fseeki64, _ftelli64
 * note:  msvcrt.dll Win7 also exports them but mingw lib omitted; add later via win7 if needed
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _fseeki64, (FILE *, long long, int));
TC_DECL_PROC(long long, _ftelli64, (FILE *));
static char nm[] = "tc_fio_fseeki64_ftelli64_large_000.tmp";  /* per-CRT suffix: targets may run concurrently */
static void setnm(void) { int v = TC_CRT_VER; char *p = nm + sizeof nm - 9; p[0]=(char)('0'+v/100); p[1]=(char)('0'+v/10%10); p[2]=(char)('0'+v%10); }

TC_CASE(exported) { CHECK(tc_p__fseeki64 != NULL); CHECK(tc_p__ftelli64 != NULL); }
TC_CASE(seek_4g)
{
    FILE *f;
    if (!tc_p__fseeki64 || !tc_p__ftelli64) TC_SKIP("not exported");
    f = fopen(nm, "w+b"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(tc_p__fseeki64(f, 0x100000000LL, SEEK_SET), 0);
    CHECK_EQ_I(tc_p__ftelli64(f), 0x100000000LL);
    CHECK_EQ_I(tc_p__fseeki64(f, 0, SEEK_SET), 0);
    CHECK_EQ_I(tc_p__ftelli64(f), 0);
    fclose(f);
}
TC_CASE(seek_end)
{
    FILE *f;
    if (!tc_p__fseeki64 || !tc_p__ftelli64) TC_SKIP("not exported");
    f = fopen(nm, "w+b"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fwrite("0123456789", 1, 10, f), 10);
    fflush(f);
    CHECK_EQ_I(tc_p__fseeki64(f, 0, SEEK_END), 0);
    CHECK_EQ_I(tc_p__ftelli64(f), 10);
    CHECK_EQ_I(tc_p__fseeki64(f, -4, SEEK_CUR), 0);
    CHECK_EQ_I(tc_p__ftelli64(f), 6);
    fclose(f);
}
int main(void)
{
    tc_begin(); setnm(); TC_LOAD_PROC(_fseeki64); TC_LOAD_PROC(_ftelli64);
    TC_RUN(exported); TC_RUN(seek_4g); TC_RUN(seek_end);
    remove(nm); return tc_end();
}
