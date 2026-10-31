/* TC
 * id:    wcs_neg_wcslen_null_av_probe
 * area:  wstring
 * funcs: wcslen wcscmp wcschr wcsstr wcscpy wcscat
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs: only the "_s" (secure) functions validate parameters; legacy wcs* functions do not
 * note:  Probes that each legacy function crashes with an access violation (exit code 0xC0000005) on NULL input, in a
 *        child process. Does not assert the invalid-parameter handler path (the child terminates).
 */
#include "tc.h"
#include <wchar.h>

static const wchar_t *volatile vnull = NULL;
static wchar_t *volatile vnullw = NULL;
static const wchar_t *volatile vtxt = L"abc";
static wchar_t gbuf[8] = { L'a', 0 };

static int run(const char *n, DWORD *code) { return tc_spawn_self(n, code, 20000); }
TC_CASE(child_wcslen_null) { DWORD code = 0; CHECK(run("wcslen", &code)); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(child_wcscmp_null) { DWORD code = 0; CHECK(run("wcscmp", &code)); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(child_wcschr_null) { DWORD code = 0; CHECK(run("wcschr", &code)); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(child_wcsstr_null_hay) { DWORD code = 0; CHECK(run("wcsstr", &code)); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(child_wcscpy_null_dest) { DWORD code = 0; CHECK(run("wcscpy", &code)); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(child_wcscat_null_src) { DWORD code = 0; CHECK(run("wcscat", &code)); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION); }

static int child_main(const char *c)
{
    if (tc_strequ(c, "wcslen")) return (int)wcslen(vnull);
    if (tc_strequ(c, "wcscmp")) return wcscmp(vnull, vtxt);
    if (tc_strequ(c, "wcschr")) return wcschr(vnull, L'a') != NULL;
    if (tc_strequ(c, "wcsstr")) return wcsstr(vnull, vtxt) != NULL;
    if (tc_strequ(c, "wcscpy")) return wcscpy(vnullw, vtxt) != NULL;
    if (tc_strequ(c, "wcscat")) return wcscat(gbuf, vnull) != NULL;
    return 78;
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { child_main(c); return 77; }
    TC_RUN(child_wcslen_null); TC_RUN(child_wcscmp_null); TC_RUN(child_wcschr_null);
    TC_RUN(child_wcsstr_null_hay); TC_RUN(child_wcscpy_null_dest); TC_RUN(child_wcscat_null_src);
    return tc_end();
}
