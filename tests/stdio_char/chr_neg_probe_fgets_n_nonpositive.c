/* TC
 * id:    chr_neg_probe_fgets_n_nonpositive
 * area:  stdio_char
 * kind:  negative
 * funcs: fgets fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fgets (invalid n); UCRT validates count
 * note:  Probes fgets with n==0 (assert only: returns NULL, buffer untouched, nothing consumed) and n<0 (assert NULL, EINVAL, handler called once).
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
static const char *volatile path = "tc_chr_neg_probe_fgets_n_nonpositive_" TC_CRT_DLL ".tmp";

static volatile int nz = 0, nm = -1;
TC_CASE(n_zero)
{
    FILE *f; char b[4] = "SSS"; mk(path, "abc", 3); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    errno = 0; CHECK(fgets(b, nz, f) == NULL); CHECK_EQ_I(b[0], 'S');
    CHECK_EQ_I((fgetc)(f), 'a'); fclose(f);
}
TC_CASE(n_negative)
{
    FILE *f; char b[4] = "SSS"; mk(path, "abc", 3); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; CHECK(fgets(b, nm, f) == NULL); CHECK_EQ_I(b[0], 'S');
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(n_zero); TC_RUN(n_negative);
    DeleteFileA(path);
    return tc_end();
}
