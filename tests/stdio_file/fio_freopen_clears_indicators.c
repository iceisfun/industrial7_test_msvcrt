/* TC
 * id:    fio_freopen_clears_indicators
 * area:  stdio_file
 * funcs: freopen fopen fclose fread fgetc feof ferror ftell ungetc
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.4 (freopen clears error and end-of-file indicators); 7.21.7.10 ungetc
 */
#include "tc.h"
#include <stdio.h>
#define S2(x) #x
#define S1(x) S2(x)
#define SFX S1(TC_CRT_VER)

static void mk(const char *n, const char *d)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w = 0, l = 0;
    if (h == INVALID_HANDLE_VALUE) return;
    while (d[l]) l++;
    WriteFile(h, d, l, &w, NULL); CloseHandle(h);
}
static int rd(const char *n, char *b, int cap)  /* returns bytes read, -1 if cannot open */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, cap - 1, &r, NULL)) r = 0;
    CloseHandle(h); b[r] = 0; return (int)r;
}

#define F "tc_fio_freopen_clears_" SFX ".tmp"

TC_CASE(eof_cleared)
{
    FILE *fp, *r; char buf[16]; size_t n;
    mk(F, "abc");
    fp = fopen(F, "r"); CHECK(fp != NULL); if (!fp) return;
    n = fread(buf, 1, sizeof buf, fp);
    CHECK_EQ_U(n, 3);
    CHECK(feof(fp) != 0);
    CHECK_EQ_I(ferror(fp), 0);
    r = freopen(F, "r", fp);
    CHECK(r != NULL);
    if (r) {
        CHECK_EQ_I(feof(r), 0);
        CHECK_EQ_I(ferror(r), 0);
        CHECK_EQ_I(ftell(r), 0);
        CHECK_EQ_I(fgetc(r), 'a');
        fclose(r);
    }
    DeleteFileA(F);
}
TC_CASE(ungetc_discarded)
{
    FILE *fp, *r;
    mk(F, "abc");
    fp = fopen(F, "r"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(fgetc(fp), 'a');
    CHECK_EQ_I(ungetc('Z', fp), 'Z');
    r = freopen(F, "r", fp);
    CHECK(r != NULL);
    if (r) {
        CHECK_EQ_I(fgetc(r), 'a');
        CHECK_EQ_I(fgetc(r), 'b');
        fclose(r);
    }
    DeleteFileA(F);
}
int main(void) { tc_begin(); TC_RUN(eof_cleared); TC_RUN(ungetc_discarded); return tc_end(); }
