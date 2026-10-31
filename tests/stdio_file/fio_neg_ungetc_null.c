/* TC
 * id:    fio_neg_ungetc_null
 * area:  stdio_file
 * kind:  negative
 * funcs: ungetc
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs ungetc: stream NULL -> EINVAL, invalid parameter handler; returns EOF
 * note:  invalid-parameter handler installed via tc_iph_install; NULL stream is validated from msvcr80 on (older CRTs would fault, hence >=80).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int slurp(const char *n, char *b, int cap)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
static void del(const char *n) { DeleteFileA(n); }
#define F "tc_fio_neg_ungetc_null.tmp"

static FILE *volatile nul = NULL;
TC_CASE(null_stream)
{
    int n0 = tc_iph_count, r; errno = 0;
    r = ungetc('a', nul);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_stream); return tc_end(); }
