/* TC
 * id:    fio_neg_wfopen_missing_file
 * area:  stdio_file
 * kind:  negative
 * funcs: _wfopen fclose
 * procs: 
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen, _wfopen; Unicode ccs mode flags
 * note:  ccs=UTF-8 read of missing file fails with ENOENT before any BOM processing.
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
#include <wchar.h>
static wchar_t * volatile w_missing = L"tc_fio_neg_wfopen_missing.tmp";
static wchar_t * volatile w_dir = L"tc_nodir_xx\\x";
static wchar_t * volatile m_r = L"r", * volatile m_rp = L"r+", * volatile m_w = L"w", * volatile m_ccs = L"r, ccs=UTF-8";
static void chk(const wchar_t *n, const wchar_t *m)
{
    FILE *f; int n0 = tc_iph_count; errno = 0;
    f = _wfopen(n, m);
    CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count - n0, 0);
    if (f) fclose(f);
}
TC_CASE(read_missing)  { chk(w_missing, m_r); }
TC_CASE(rplus_missing) { chk(w_missing, m_rp); }
TC_CASE(write_nodir)   { chk(w_dir, m_w); }
TC_CASE(ccs_missing)   { chk(w_missing, m_ccs); }
int main(void)
{
    tc_begin(); DeleteFileW(w_missing);
    CHECK(tc_iph_install());
    TC_RUN(read_missing); TC_RUN(rplus_missing); TC_RUN(write_nodir); TC_RUN(ccs_missing);
    return tc_end();
}
