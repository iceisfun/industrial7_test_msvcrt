/* TC
 * id:    fio_neg_remove_missing_dir_path
 * area:  stdio_file
 * kind:  negative
 * funcs: remove
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs remove, _wremove (returns -1 and sets errno to ENOENT if file or path not found)
 * note:  Trailing-separator names are deliberately not asserted.
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
#define F "tc_fio_neg_remove_md_" V ".tmp"
static const char * volatile fname = F;
static const char * volatile bad = "tc_nodir_xx\\f.tmp";
TC_CASE(missing_dir) { int r; errno = 0; r = remove(bad); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(double_remove)
{
    int r; put(fname, "x", 1); CHECK(exists(fname));
    r = remove(fname); CHECK_EQ_I(r, 0); CHECK(!exists(fname));
    errno = 0; r = remove(fname); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}
int main(void)
{
    tc_begin(); DeleteFileA(fname);
    TC_RUN(missing_dir); TC_RUN(double_remove);
    DeleteFileA(fname);
    return tc_end();
}
