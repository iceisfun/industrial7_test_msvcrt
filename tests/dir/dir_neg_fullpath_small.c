/* TC
 * id:    dir_neg_fullpath_small
 * area:  dir
 * funcs: _fullpath _wfullpath
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _fullpath: NULL if the path is longer than maxLength (errno ERANGE is observed, not documented)
 * note:  ERANGE path does not assert iph count.
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static volatile int v_three = 3;
static const char  apath[]  = "C:\\abcdefghij\\klmnop";   /* already canonical, 20 chars; drive letter need not exist (string op) */
static const wchar_t wpath[] = L"C:\\abcdefghij\\klmnop";
#define PLEN 20

TC_CASE(buf3)
{
    char out[64]; char *p; tc_iph_install(); errno = 0;
    p = _fullpath(out, apath, v_three);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(exact_no_nul)
{
    char out[64]; char *p; tc_iph_install(); errno = 0;
    p = _fullpath(out, apath, PLEN);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(wide)
{
    wchar_t out[64]; wchar_t *p; tc_iph_install(); errno = 0;
    p = _wfullpath(out, wpath, v_three);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
    errno = 0; p = _wfullpath(out, wpath, PLEN);
    CHECK(p == NULL); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(control)
{
    char out[64]; char *p = _fullpath(out, apath, PLEN + 1);
    CHECK_EQ_P(p, out); if (p) CHECK_STR(out, apath);
    { wchar_t wo[64]; wchar_t *w = _wfullpath(wo, wpath, PLEN + 1); CHECK_EQ_P(w, wo); if (w) CHECK_WSTR(wo, wpath); }
}
int main(void) { tc_begin(); TC_RUN(buf3); TC_RUN(exact_no_nul); TC_RUN(wide); TC_RUN(control); return tc_end(); }
