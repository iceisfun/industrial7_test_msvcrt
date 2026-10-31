/* TC
 * id:    gdr_stat_plain_msvcrt60
 * area:  dir
 * funcs:
 * procs: _stat _fstat _open _write _close
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _stat, _fstat (-1 and ENOENT for a missing path; st_size, st_mode)
 * note:  Resolved at run time because the un-suffixed names are version dependent; cases SKIP where the CRT does not
 *        export them (msvcrt.dll is expected to). Hand-typed struct prefix (dev,ino,mode,nlink,uid,gid,rdev,size) is the
 *        same for 32-bit and 64-bit time_t layouts; only st_mode and st_size are asserted. Padding absorbs the time fields.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define F "tc_gdr_stat_plain_f" TC_S(TC_CRT_VER) ".tmp"
#define DIR_ "tc_gdr_stat_plain_d" TC_S(TC_CRT_VER) ".tmp"
struct st_t { unsigned st_dev; unsigned short st_ino; unsigned short st_mode; short st_nlink, st_uid, st_gid; unsigned st_rdev; int st_size; char pad[64]; };
typedef int (__cdecl *stat_t)(const char *, struct st_t *);
typedef int (__cdecl *fstat_t)(int, struct st_t *);
typedef int (__cdecl *open_t)(const char *, int, ...);
typedef int (__cdecl *write_t)(int, const void *, unsigned);
typedef int (__cdecl *close_t)(int);
static stat_t p_stat; static fstat_t p_fstat; static open_t p_open; static write_t p_write; static close_t p_close;
static const char *volatile v_path;

static void cleanup(void) { DeleteFileA(F); RemoveDirectoryA(DIR_); }

TC_CASE(exported)
{
    if (TC_AT_LEAST(80) && !p_stat) TC_SKIP("plain _stat not exported by this CRT");
    CHECK(p_stat != NULL); CHECK(p_fstat != NULL);
}
TC_CASE(stat_file)
{
    struct st_t st; HANDLE h; DWORD w; int r;
    if (!p_stat) TC_SKIP("not exported");
    h = CreateFileA(F, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) TC_SKIP("cannot create file");
    WriteFile(h, "abcdefghij", 10, &w, NULL); CloseHandle(h);
    v_path = F; memset(&st, 0, sizeof st);
    r = p_stat(v_path, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) { CHECK_EQ_I(st.st_size, 10); CHECK(st.st_mode & _S_IFREG); CHECK(!(st.st_mode & _S_IFDIR)); }
    cleanup();
}
TC_CASE(stat_dir)
{
    struct st_t st; int r;
    if (!p_stat) TC_SKIP("not exported");
    CreateDirectoryA(DIR_, NULL); v_path = DIR_;
    r = p_stat(v_path, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) { CHECK(st.st_mode & _S_IFDIR); CHECK(!(st.st_mode & _S_IFREG)); }
    cleanup();
}
TC_CASE(fstat_fd)
{
    struct st_t st; int fd, r;
    if (!p_fstat || !p_open || !p_write || !p_close) TC_SKIP("not exported");
    fd = p_open(F, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd >= 0) {
        CHECK_EQ_I(p_write(fd, "wxyz", 4), 4);
        r = p_fstat(fd, &st); CHECK_EQ_I(r, 0);
        if (r == 0) { CHECK_EQ_I(st.st_size, 4); CHECK(st.st_mode & _S_IFREG); }
        p_close(fd);
    }
    cleanup();
}
TC_CASE(missing)
{
    struct st_t st; int r;
    if (!p_stat) TC_SKIP("not exported");
    v_path = "tc_gdr_stat_plain_no_such_file.tmp";
    errno = 0; r = p_stat(v_path, &st);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}
int main(void)
{
    tc_begin(); cleanup();
    p_stat = (stat_t)tc_crt_proc("_stat"); p_fstat = (fstat_t)tc_crt_proc("_fstat");
    p_open = (open_t)tc_crt_proc("_open"); p_write = (write_t)tc_crt_proc("_write"); p_close = (close_t)tc_crt_proc("_close");
    TC_RUN(exported); TC_RUN(stat_file); TC_RUN(stat_dir); TC_RUN(fstat_fd); TC_RUN(missing);
    cleanup();
    return tc_end();
}
