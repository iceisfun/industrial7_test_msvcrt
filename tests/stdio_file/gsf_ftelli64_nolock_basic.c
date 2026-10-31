/* TC
 * id:    gsf_ftelli64_nolock_basic
 * area:  stdio_file
 * funcs: _ftelli64_nolock fopen fwrite fgetc fclose
 * procs: _fseeki64
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ftelli64 (and the _nolock variants), _fseeki64
 * note:  Binary mode only (text offsets are covered by other tests). The >4 GiB case only seeks and writes no data;
 *        it skips if the seek fails (file system without sparse/large support).
 */
#include "tc.h"
#include <stdio.h>
__int64 __cdecl _ftelli64_nolock(FILE *);
TC_DECL_PROC(int, _fseeki64, (FILE *, __int64, int));
#define _fseeki64 tc_p__fseeki64

#define S2(x) #x
#define S1(x) S2(x)
static const char *fn = "tc_gsf_ftelli64_nolock_basic_" S1(TC_CRT_VER) ".tmp";
static volatile __int64 vpos;

TC_CASE(fresh_zero)
{
    FILE *f = fopen(fn, "wb"); if (!f) { CHECK(f != NULL); return; }
    fwrite("abc", 1, 3, f); fclose(f);
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_ftelli64_nolock(f), 0);
    fclose(f); remove(fn);
}
TC_CASE(after_write)
{
    FILE *f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fwrite("0123456789", 1, 10, f), 10);
    CHECK_EQ_I(_ftelli64_nolock(f), 10);
    CHECK_EQ_U(fwrite("xyz", 1, 3, f), 3);
    CHECK_EQ_I(_ftelli64_nolock(f), 13);
    fclose(f); remove(fn);
}
TC_CASE(after_read_and_seek)
{
    FILE *f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    fwrite("0123456789", 1, 10, f); fclose(f);
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), '0'); CHECK_EQ_I(fgetc(f), '1'); CHECK_EQ_I(fgetc(f), '2');
    CHECK_EQ_I(_ftelli64_nolock(f), 3);
    vpos = 7;
    CHECK_EQ_I(_fseeki64(f, vpos, SEEK_SET), 0);
    CHECK_EQ_I(_ftelli64_nolock(f), 7);
    CHECK_EQ_I(fgetc(f), '7');
    CHECK_EQ_I(_ftelli64_nolock(f), 8);
    vpos = 0;
    CHECK_EQ_I(_fseeki64(f, vpos, SEEK_END), 0);
    CHECK_EQ_I(_ftelli64_nolock(f), 10);
    fclose(f); remove(fn);
}
TC_CASE(beyond_4gib)
{
    FILE *f = fopen(fn, "w+b"); CHECK(f != NULL); if (!f) return;
    vpos = 0x100000000LL;
    if (_fseeki64(f, vpos, SEEK_SET) != 0) { fclose(f); remove(fn); TC_SKIP("seek beyond 4 GiB refused"); }
    CHECK_EQ_I(_ftelli64_nolock(f), 4294967296LL);
    fclose(f); remove(fn);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_fseeki64); if (!tc_p__fseeki64) { tc_write("TC FAIL " TC_TEST_ID "::__import _fseeki64 missing\n"); return 1; }
    TC_RUN(fresh_zero); TC_RUN(after_write); TC_RUN(after_read_and_seek); TC_RUN(beyond_4gib);
    return tc_end();
}
