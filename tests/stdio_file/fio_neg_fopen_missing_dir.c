/* TC
 * id:    fio_neg_fopen_missing_dir
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * procs: fopen_s
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (returns NULL on error; errno ENOENT for path not found); fopen_s
 * note:  fopen_s part runs only where exported (>=80), via TC_AT_LEAST(80).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define V TC_S1(TC_CRT_VER)
static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int exists(const char *n) { return GetFileAttributesA(n) != INVALID_FILE_ATTRIBUTES; }
static const char * volatile p_bs = "tc_nodir_xx\\f.tmp";
static const char * volatile p_fs = "tc_nodir_xx/f.tmp";
static const char * volatile m_r = "r", * volatile m_w = "w", * volatile m_rp = "r+";
TC_DECL_PROC(int, fopen_s, (FILE **, const char *, const char *));
TC_CASE(read_backslash)  { FILE *f; errno = 0; f = fopen(p_bs, m_r); CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); if (f) fclose(f); }
TC_CASE(write_backslash)
{
    FILE *f; int n0 = tc_iph_count; errno = 0; f = fopen(p_bs, m_w);
    CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count - n0, 0);
    if (f) fclose(f);
}
TC_CASE(rplus_forward_slash) { FILE *f; errno = 0; f = fopen(p_fs, m_rp); CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); if (f) fclose(f); }
TC_CASE(fopen_s_variant)
{
    FILE *f = (FILE *)1; int r;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, p_bs, m_w);
    CHECK_EQ_I(r, ENOENT); CHECK(f == NULL);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(fopen_s);
    if (TC_AT_LEAST(80)) tc_iph_install();
    TC_RUN(read_backslash); TC_RUN(write_backslash); TC_RUN(rplus_forward_slash);
    if (TC_AT_LEAST(80)) TC_RUN(fopen_s_variant);
    return tc_end();
}
