/* TC
 * id:    fio_neg_fsopen_null_args
 * area:  stdio_file
 * kind:  negative
 * funcs: _fsopen fclose
 * procs: 
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fsopen, _wfsopen (invalid parameter handling)
 * note:  Missing-file case with valid args must not invoke the handler.
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
#include <share.h>
#define F "tc_fio_neg_fsopen_null_" V ".tmp"
static const char * volatile fname = F;
static const char * volatile nul = NULL, * volatile m_r = "r";
static void bad(const char *n, const char *m)
{
    FILE *f; int n0 = tc_iph_count; errno = 0;
    f = _fsopen(n, m, _SH_DENYNO);
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
    if (f) fclose(f);
}
TC_CASE(null_name) { bad(nul, m_r); }
TC_CASE(null_mode) { bad(fname, nul); }
TC_CASE(missing_valid_args)
{
    FILE *f; int n0 = tc_iph_count; errno = 0;
    f = _fsopen(fname, m_r, _SH_DENYNO);
    CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count - n0, 0);
    if (f) fclose(f);
}
int main(void)
{
    tc_begin(); DeleteFileA(fname);
    CHECK(tc_iph_install());
    TC_RUN(null_name); TC_RUN(null_mode); TC_RUN(missing_valid_args);
    return tc_end();
}
