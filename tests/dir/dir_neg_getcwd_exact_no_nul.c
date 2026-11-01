/* TC
 * id:    dir_neg_getcwd_exact_no_nul
 * area:  dir
 * kind:  negative
 * funcs: _getcwd _wgetcwd _getdcwd _wgetdcwd _getdrive
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _getcwd: "ERANGE: Path is longer than maxlen characters" (the terminating NUL counts)
 * note:  Caller-supplied buffer of len+8 bytes but maxlen == strlen(cwd): no room for the NUL, so NULL/ERANGE. maxlen ==
 *        strlen(cwd)+1 is the success control for _getcwd/_getdcwd. Class A (_wgetcwd is not ISO C; the deviation is from the MS docs): OBSERVED on Win10 22H2 ucrtbase (cwd
 *        C:\tc\ucrtbase, 14 chars): _wgetcwd(buf, len+1) and (buf, len+2) return NULL/ERANGE and only maxlen >= len+3 succeeds,
 *        deviating from the MS docs (ERANGE only when the path is longer than maxlen). Cwd-dependence of the +3 margin is
 *        [unverified]: only the cwd C:\tc\ucrtbase of this harness was exercised.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static volatile int v_n;
static size_t slen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }
static size_t wlen(const wchar_t *s) { size_t n = 0; while (s[n]) n++; return n; }

TC_CASE(getcwd_too_small)
{
    char ref[MAX_PATH], buf[MAX_PATH + 8]; char *p; int len;
    GetCurrentDirectoryA(MAX_PATH, ref); len = (int)slen(ref);
    if (len < 3) TC_SKIP("cwd too short");
    errno = 0; p = _getcwd(buf, 2 + v_n);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
    errno = 0; p = _getcwd(buf, len + v_n);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
    errno = 0; p = _getcwd(buf, len + 1 + v_n);
    CHECK_EQ_P(p, buf); CHECK_STR(buf, ref);
}
TC_CASE(wgetcwd_too_small)
{
    wchar_t ref[MAX_PATH], buf[MAX_PATH + 8]; wchar_t *p; int len;
    GetCurrentDirectoryW(MAX_PATH, ref); len = (int)wlen(ref);
    if (len < 3) TC_SKIP("cwd too short");
    errno = 0; p = _wgetcwd(buf, 2 + v_n);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
    errno = 0; p = _wgetcwd(buf, len + v_n);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
    /* OBSERVED on Win10 22H2 ucrtbase: unlike _getcwd, _wgetcwd needs maxlen >= len + 3 (len+1 and len+2 still give ERANGE) */
    errno = 0; p = _wgetcwd(buf, len + 1 + v_n);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
    errno = 0; p = _wgetcwd(buf, len + 2 + v_n);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
    errno = 0; p = _wgetcwd(buf, len + 3 + v_n);
    CHECK_EQ_P(p, buf); CHECK_WSTR(buf, ref);
}
TC_CASE(getdcwd_too_small)
{
    char ref[MAX_PATH], buf[MAX_PATH + 8]; char *p; int len, d = _getdrive();
    GetCurrentDirectoryA(MAX_PATH, ref); len = (int)slen(ref);
    if (len < 3 || d == 0) TC_SKIP("cwd too short or no drive");
    errno = 0; p = _getdcwd(d, buf, 2 + v_n);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
    errno = 0; p = _getdcwd(d, buf, len + v_n);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
    errno = 0; p = _getdcwd(d, buf, len + 1 + v_n);
    CHECK_EQ_P(p, buf); CHECK_STR(buf, ref);
}
TC_CASE(wgetdcwd_too_small)
{
    wchar_t ref[MAX_PATH], buf[MAX_PATH + 8]; wchar_t *p; int len, d = _getdrive();
    GetCurrentDirectoryW(MAX_PATH, ref); len = (int)wlen(ref);
    if (len < 3 || d == 0) TC_SKIP("cwd too short or no drive");
    errno = 0; p = _wgetdcwd(d, buf, 2 + v_n);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
    errno = 0; p = _wgetdcwd(d, buf, len + v_n);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
}

int main(void)
{
    tc_begin();
    TC_RUN(getcwd_too_small); TC_RUN(wgetcwd_too_small); TC_RUN(getdcwd_too_small); TC_RUN(wgetdcwd_too_small);
    return tc_end();
}
