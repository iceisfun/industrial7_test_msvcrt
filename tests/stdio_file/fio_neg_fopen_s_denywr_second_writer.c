/* TC
 * id:    fio_neg_fopen_s_denywr_second_writer
 * area:  stdio_file
 * kind:  negative
 * funcs: fclose
 * procs: fopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs fopen_s: files opened by fopen_s are not sharable (_SH_DENYWR)
 * note:  Docs only say fopen_s files are not sharable (the CRT opens with _SH_SECURE); the sharing violation -> EACCES
 *        mapping is observed behaviour, not documented.
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
#define F "tc_fio_neg_fopen_s_denywr_" V ".tmp"
static const char * volatile fname = F;
static const char * volatile m_w = "w", * volatile m_a = "a", * volatile m_rp = "r+";
TC_DECL_PROC(int, fopen_s, (FILE **, const char *, const char *));
static void second(const char *mode)
{
    FILE *a = NULL, *b = (FILE *)1; int r;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    CHECK_EQ_I(tc_p_fopen_s(&a, fname, m_w), 0); CHECK(a != NULL);
    r = tc_p_fopen_s(&b, fname, mode);
    CHECK_EQ_I(r, EACCES); CHECK(b == NULL);
    if (b) fclose(b);
    if (a) fclose(a);
    DeleteFileA(fname);
}
TC_CASE(second_w)  { second(m_w); }
TC_CASE(second_a)  { second(m_a); }
TC_CASE(second_rp) { second(m_rp); }
TC_CASE(after_close)
{
    FILE *a = NULL, *b = NULL;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    CHECK_EQ_I(tc_p_fopen_s(&a, fname, m_w), 0);
    if (a) fclose(a);
    CHECK_EQ_I(tc_p_fopen_s(&b, fname, m_w), 0); CHECK(b != NULL);
    if (b) fclose(b);
    DeleteFileA(fname);
}
int main(void)
{
    tc_begin(); DeleteFileA(fname); TC_LOAD_PROC(fopen_s);
    tc_iph_install();
    TC_RUN(second_w); TC_RUN(second_a); TC_RUN(second_rp); TC_RUN(after_close);
    DeleteFileA(fname);
    return tc_end();
}
