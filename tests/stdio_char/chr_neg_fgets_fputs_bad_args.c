/* TC
 * id:    chr_neg_fgets_fputs_bad_args
 * area:  stdio_char
 * kind:  negative
 * funcs: fgets fputs fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs fputs (NULL str/stream: EOF, EINVAL); MS docs Parameter Validation
 * note:  fputs validation is documented; the fgets NULL-buffer/NULL-stream EINVAL+NULL behaviour (msvcr80+) is known from CRT
 *        validation behaviour rather than a per-function docs citation, hence observed.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void mk(const char *p, const void *d, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, n, &w, NULL); CloseHandle(h); }
}
static DWORD slurp(const char *p, unsigned char *b, DWORD cap)
{
    DWORD r = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { if (!ReadFile(h, b, cap, &r, NULL)) r = 0; CloseHandle(h); }
    return r;
}
static const char *volatile path = "tc_chr_neg_fgets_fputs_bad_args_" TC_CRT_DLL ".tmp";

static FILE *volatile nf = NULL;
static char *volatile nb = NULL;
static const char *volatile ns = NULL;
TC_CASE(fgets_null_buf)
{
    FILE *f; mk(path, "abc", 3); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; CHECK(fgets(nb, 10, f) == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
}
TC_CASE(fgets_null_stream)
{
    char b[8] = "SSSS"; tc_iph_count = 0; errno = 0; CHECK(fgets(b, 8, nf) == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fputs_null_str)
{
    FILE *f = fopen(path, "wb"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(fputs(ns, f), EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
}
TC_CASE(fputs_null_stream) { tc_iph_count = 0; errno = 0; CHECK_EQ_I(fputs("x", nf), EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(fgets_null_buf); TC_RUN(fgets_null_stream); TC_RUN(fputs_null_str); TC_RUN(fputs_null_stream);
    DeleteFileA(path);
    return tc_end();
}
