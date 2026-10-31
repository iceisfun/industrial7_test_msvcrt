/* TC
 * id:    dir_neg_getcwd_small
 * area:  dir
 * funcs: _getcwd _wgetcwd
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _getcwd: ERANGE if the path is longer than maxlen
 * note:  ERANGE path does not assert tc_iph_count (differs by CRT)
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static void cur(char *b) { DWORD n = GetCurrentDirectoryA(MAX_PATH, b); if (!n || n >= MAX_PATH) b[0] = 0; }
static size_t slen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }
static volatile int v_two = 2;

TC_CASE(len2)
{
    char buf[8]; char *p; buf[0] = 'Q';
    tc_iph_install();
    errno = 0; p = _getcwd(buf, v_two);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(no_room_for_nul)
{
    char ref[MAX_PATH], buf[MAX_PATH]; char *p; size_t n; cur(ref); n = slen(ref);
    tc_iph_install();
    errno = 0; p = _getcwd(buf, (int)n);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(wide_small)
{
    wchar_t ref[MAX_PATH], buf[MAX_PATH]; wchar_t *p; DWORD n = GetCurrentDirectoryW(MAX_PATH, ref);
    tc_iph_install();
    errno = 0; p = _wgetcwd(buf, v_two);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ERANGE);
    errno = 0; p = _wgetcwd(buf, (int)n);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(control_len_plus1)
{
    char ref[MAX_PATH], buf[MAX_PATH]; char *p; size_t n; cur(ref); n = slen(ref);
    p = _getcwd(buf, (int)(n + 1));
    CHECK_EQ_P(p, buf);
    CHECK_STR(buf, ref);
}
int main(void)
{
    tc_begin();
    TC_RUN(len2); TC_RUN(no_room_for_nul); TC_RUN(wide_small); TC_RUN(control_len_plus1);
    return tc_end();
}
