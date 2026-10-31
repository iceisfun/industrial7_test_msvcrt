/* TC
 * id:    pfx_fflush_null_all
 * area:  stdio_printf
 * funcs: fflush fprintf fopen setvbuf fclose _flushall
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fflush (stream NULL: flushes all open output streams, returns 0), _flushall (returns number of open streams)
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#define F1 "tc_pfx_fflush_null_all_1_" TCV ".tmp"
#define F2 "tc_pfx_fflush_null_all_2_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

TC_CASE(fflush_null)
{
    FILE *a = fopen(F1, "wb"), *b = fopen(F2, "wb");
    CHECK(a != NULL && b != NULL);
    if (a && b) {
        CHECK_EQ_I(setvbuf(a, NULL, _IOFBF, 4096), 0);
        CHECK_EQ_I(setvbuf(b, NULL, _IOFBF, 4096), 0);
        fprintf(a, "first-%d", 1);
        fprintf(b, "second-%d", 22);
        CHECK_EQ_I(fflush(NULL), 0);
        CHECK_EQ_I(tc_slurp(F1), 7);
        CHECK_MEM(tc_buf, "first-1", 7);
        CHECK_EQ_I(tc_slurp(F2), 9);
        CHECK_MEM(tc_buf, "second-22", 9);
    }
    if (a) fclose(a);
    if (b) fclose(b);
}
TC_CASE(flushall_count)
{
    FILE *a = fopen(F1, "wb"), *b = fopen(F2, "wb"); int n;
    CHECK(a != NULL && b != NULL);
    if (a && b) {
        CHECK_EQ_I(setvbuf(a, NULL, _IOFBF, 4096), 0);
        CHECK_EQ_I(setvbuf(b, NULL, _IOFBF, 4096), 0);
        fprintf(a, "AAAA"); fprintf(b, "BBBBB");
        n = _flushall();
        CHECK(n >= 2);
        CHECK_EQ_I(tc_slurp(F1), 4);
        CHECK_EQ_I(tc_slurp(F2), 5);
    }
    if (a) fclose(a);
    if (b) fclose(b);
}
int main(void)
{
    tc_begin(); remove(F1); remove(F2);
    TC_RUN(fflush_null); TC_RUN(flushall_count);
    remove(F1); remove(F2); return tc_end();
}
