/* TC
 * id:    pfx_neg_probe_ferror_after_failed_write
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose ferror clearerr feof
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.10.3 (clearerr), 7.21.10.4 (ferror); MS docs ferror
 * note:  PROBES that a failed fprintf on a "r" stream sets the error indicator (not EOF), clearerr clears it, and a repeated failure sets it again.
 */
#include "tc.h"
#include <stdio.h>
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
static int rd(const char *p, char *b, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); DWORD got = 0;
    if (h == INVALID_HANDLE_VALUE) { b[0] = 0; return -1; }
    ReadFile(h, b, (DWORD)(n - 1), &got, NULL); CloseHandle(h); b[got] = 0; return (int)got;
}
static void wr(const char *p, const char *d, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, (DWORD)n, &w, NULL); CloseHandle(h); }
}

#define TMP "tc_pfx_neg_probe_ferror_after_failed_write_" TCV ".tmp"

TC_CASE(ferror_set_not_eof)
{
    FILE *f; wr(TMP, "abcd", 4);
    f = fopen(TMP, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(TMP); return; }
    CHECK_EQ_I(ferror(f), 0);
    CHECK(fprintf(f, "x") < 0);
    CHECK(ferror(f) != 0);
    CHECK_EQ_I(feof(f), 0);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(clear_and_repeat)
{
    FILE *f; wr(TMP, "abcd", 4);
    f = fopen(TMP, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(TMP); return; }
    CHECK(fprintf(f, "x") < 0);
    clearerr(f);
    CHECK_EQ_I(ferror(f), 0);
    CHECK(fprintf(f, "x") < 0);
    CHECK(ferror(f) != 0);
    fclose(f); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(ferror_set_not_eof); TC_RUN(clear_and_repeat);
    return tc_end();
}
