/* TC
 * id:    fio_neg_rename_dest_dir_missing
 * area:  stdio_file
 * kind:  negative
 * funcs: rename remove fopen fclose
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs rename (returns nonzero and sets errno on failure: ENOENT file or path not found)
 * note:  rename to empty string: only failure asserted (not the errno); source must survive.
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
#define F "tc_fio_neg_rename_dd_" V ".tmp"
static const char * volatile fname = F;
static const char * volatile bad_dst = "tc_nodir_xx\\n.tmp", * volatile bad_src = "tc_nodir_xx\\a.tmp", * volatile b_tmp = "tc_fio_neg_rename_dd_b_" V ".tmp";
static const char * volatile empty = "";
TC_CASE(dest_dir_missing)
{
    char b[8] = {0}; FILE *f; int r;
    put(fname, "orig", 4);
    errno = 0; r = rename(fname, bad_dst);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
    CHECK(exists(fname)); CHECK(!exists(bad_dst));
    f = fopen(fname, "rb");
    CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 8, f), 4); CHECK_MEM(b, "orig", 4); fclose(f); }
}
TC_CASE(src_dir_missing)
{
    int r; errno = 0; r = rename(bad_src, b_tmp);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); CHECK(!exists(b_tmp));
}
TC_CASE(empty_dest)
{
    int r; errno = 0; r = rename(fname, empty);
    CHECK_EQ_I(r, -1);
    CHECK(exists(fname));
}
int main(void)
{
    tc_begin(); DeleteFileA(fname); DeleteFileA(b_tmp);
    if (TC_AT_LEAST(80)) tc_iph_install();
    TC_RUN(dest_dir_missing); TC_RUN(src_dir_missing); TC_RUN(empty_dest);
    remove(fname); DeleteFileA(fname); DeleteFileA(b_tmp);
    return tc_end();
}
