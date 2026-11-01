/* TC
 * id:    pfx_neg_probe_fread_from_write_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fread fopen fclose ferror clearerr fseek
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs fread (errno EBADF if stream not open for reading), ferror, clearerr
 * note:  PROBES fread on a stream opened "w" (after fprintf and a rewinding fseek): returns 0, ferror(f) != 0 (observed; the old EBADF guess is wrong: Win10 22H2 leaves errno 0), cleared by clearerr;
 *        the file content is untouched.
 *        Raw evidence (baseline): TC FAIL ...::fread_fails errno == EBADF got=0 want=9. Only verified on ucrtbase.
 *        crt narrowed to ucrtbase: the observed behaviour was measured on Win10 22H2 ucrtbase only (older CRTs [unverified]).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
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

#define TMP "tc_pfx_neg_probe_fread_from_write_stream_" TCV ".tmp"

TC_CASE(fread_fails)
{
    FILE *f = fopen(TMP, "w"); char c = 0; size_t n; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "abc"), 3);
    CHECK_EQ_I(fseek(f, 0, SEEK_SET), 0);
    errno = 0; n = fread(&c, 1, 1, f);
    CHECK_EQ_U(n, 0); CHECK_EQ_I(errno, 0);   /* observed: errno NOT set (ferror is, see next case) */
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(ferror_and_clear)
{
    FILE *f = fopen(TMP, "w"); char c = 0; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "abc"), 3);
    CHECK_EQ_I(fseek(f, 0, SEEK_SET), 0);
    CHECK_EQ_I(ferror(f), 0);
    (void)fread(&c, 1, 1, f);
    CHECK(ferror(f) != 0);
    clearerr(f);
    CHECK_EQ_I(ferror(f), 0);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(content_intact)
{
    FILE *f = fopen(TMP, "w"); char c = 0, b[16]; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "abc"), 3);
    CHECK_EQ_I(fseek(f, 0, SEEK_SET), 0);
    (void)fread(&c, 1, 1, f);
    clearerr(f);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 3); CHECK_STR(b, "abc");
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(fread_fails); TC_RUN(ferror_and_clear); TC_RUN(content_intact);
    return tc_end();
}
