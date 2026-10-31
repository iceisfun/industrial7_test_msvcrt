/* TC
 * id:    gdr_neg_stat32_wildcard_probe
 * area:  dir
 * kind:  negative
 * funcs: _stat32 _wstat32
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _stat (no wildcard support)
 * note:  PROBE: wildcards '*' and '?' are rejected (-1, ENOENT) even when matching files exist; a trailing backslash on a
 *        regular file and a path component that is a file fail with -1; errno only checked as a member of {ENOENT,ENOTDIR,EINVAL}.
 */
#include "tc.h"
#include <errno.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define FN "tc_gdr_wild" TC_S(TC_CRT_VER) ".tmp"
static const char *volatile v_star = "*";
static const char *volatile v_q = "?";
static const char *volatile v_pat = "tc_gdr_wild*";
static const char *volatile v_slash = FN "\\";
static const char *volatile v_under = FN "\\x";
static const wchar_t *volatile v_wstar = L"*";
static const wchar_t *volatile v_wq = L"?";

static int errset(int e) { return e == ENOENT || e == ENOTDIR || e == EINVAL; }
static void mk(void)
{
    HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
}

TC_CASE(wildcards)
{
    struct _stat32 st;
    mk();
    errno = 0; CHECK_EQ_I(_stat32(v_star, &st), -1); CHECK_EQ_I(errno, ENOENT);
    errno = 0; CHECK_EQ_I(_stat32(v_q, &st), -1); CHECK_EQ_I(errno, ENOENT);
    errno = 0; CHECK_EQ_I(_stat32(v_pat, &st), -1); CHECK_EQ_I(errno, ENOENT);
    errno = 0; CHECK_EQ_I(_wstat32(v_wstar, &st), -1); CHECK_EQ_I(errno, ENOENT);
    errno = 0; CHECK_EQ_I(_wstat32(v_wq, &st), -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(file_trailing_slash)
{
    struct _stat32 st; int r;
    mk();
    errno = 0; r = _stat32(v_slash, &st);
    CHECK_EQ_I(r, -1);
    if (r == -1) CHECK(errset(errno));
}
TC_CASE(file_as_dir_component)
{
    struct _stat32 st; int r;
    mk();
    errno = 0; r = _stat32(v_under, &st);
    CHECK_EQ_I(r, -1);
    if (r == -1) CHECK(errset(errno));
}
int main(void) { tc_begin(); TC_RUN(wildcards); TC_RUN(file_trailing_slash); TC_RUN(file_as_dir_component); DeleteFileA(FN); return tc_end(); }
