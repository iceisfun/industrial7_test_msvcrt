/* TC
 * id:    sec_fopen_s_ccs
 * area:  secure_crt
 * funcs:
 * procs: fopen_s fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen_s ("ccs=" encodings; BOM written when a new file is opened in write mode)
 * note:  Probes: (1) w+ccs=UTF-8 writes EF BB BF, (2) w+ccs=UTF-16LE writes FF FE (raw bytes read via kernel32),
 *        (3) an unknown ccs name makes fopen_s fail with EINVAL (22) - the errno value is the guess.
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *fopen_s_t)(void **, const char *, const char *);
typedef int (__cdecl *fclose_t)(void *);
typedef int *(__cdecl *errno_t_)(void);
static fopen_s_t p_fopen_s; static fclose_t p_fclose;

static DWORD slurp(const char *n, unsigned char *buf, DWORD cap)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD got = 0; if (h == INVALID_HANDLE_VALUE) return 0xffffffff;
    ReadFile(h, buf, cap, &got, NULL); CloseHandle(h); return got;
}
TC_CASE(utf8_bom_written_EF_BB_BF)
{
    void *f = NULL; unsigned char b[8]; DWORD n; int r;
    DeleteFileA("tc_sec_ccs_a.tmp");
    r = p_fopen_s(&f, "tc_sec_ccs_a.tmp", "w, ccs=UTF-8"); CHECK_EQ_I(r, 0);
    if (f) p_fclose(f);
    n = slurp("tc_sec_ccs_a.tmp", b, 8);
    CHECK_EQ_U(n, 3);
    if (n >= 3) { CHECK_EQ_U(b[0], 0xEF); CHECK_EQ_U(b[1], 0xBB); CHECK_EQ_U(b[2], 0xBF); }
    DeleteFileA("tc_sec_ccs_a.tmp");
}
TC_CASE(utf16le_bom_written_FF_FE)
{
    void *f = NULL; unsigned char b[8]; DWORD n; int r;
    DeleteFileA("tc_sec_ccs_b.tmp");
    r = p_fopen_s(&f, "tc_sec_ccs_b.tmp", "w, ccs=UTF-16LE"); CHECK_EQ_I(r, 0);
    if (f) p_fclose(f);
    n = slurp("tc_sec_ccs_b.tmp", b, 8);
    CHECK_EQ_U(n, 2);
    if (n >= 2) { CHECK_EQ_U(b[0], 0xFF); CHECK_EQ_U(b[1], 0xFE); }
    DeleteFileA("tc_sec_ccs_b.tmp");
}
TC_CASE(invalid_ccs_name_fails_EINVAL)
{
    void *f = (void *)(ULONG_PTR)1; int r;
    errno_t_ pe = (errno_t_)tc_crt_proc("_errno");
    tc_iph_install(); tc_iph_count = 0;
    DeleteFileA("tc_sec_ccs_c.tmp");
    r = p_fopen_s(&f, "tc_sec_ccs_c.tmp", "w, ccs=BOGUS");
    CHECK_EQ_I(r, 22);
    CHECK(f == NULL);
    if (pe) CHECK_EQ_I(*pe(), 22);
    DeleteFileA("tc_sec_ccs_c.tmp");
}
int main(void)
{
    tc_begin();
    p_fopen_s = (fopen_s_t)tc_crt_proc("fopen_s"); p_fclose = (fclose_t)tc_crt_proc("fclose");
    CHECK(p_fopen_s && p_fclose);
    if (!(p_fopen_s && p_fclose)) return tc_end();
    TC_RUN(utf8_bom_written_EF_BB_BF); TC_RUN(utf16le_bom_written_FF_FE); TC_RUN(invalid_ccs_name_fails_EINVAL);
    return tc_end();
}
