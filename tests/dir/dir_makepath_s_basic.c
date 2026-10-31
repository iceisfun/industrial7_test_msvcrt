/* TC
 * id:    dir_makepath_s_basic
 * area:  dir
 * funcs:
 * procs: _makepath_s _wmakepath_s
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _makepath_s, _wmakepath_s (return 0 on success)
 * note:  resolved at run time because mingw's libmsvcr80 lacks the _s exports. Any failure here on msvcrt.dll
 *        means that DLL does not export the function.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef int (__cdecl *mps_t)(char *, size_t, const char *, const char *, const char *, const char *);
typedef int (__cdecl *wmps_t)(wchar_t *, size_t, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *);
static mps_t p_mps; static wmps_t p_wmps;
static const char *volatile v_s;

TC_CASE(exported) { CHECK(p_mps != NULL); CHECK(p_wmps != NULL); }
TC_CASE(vectors)
{
    char b[260]; v_s = "name";
    if (!p_mps) TC_SKIP("not exported");
    CHECK_EQ_I(p_mps(b, sizeof b, "C:", "\\dir\\sub\\", v_s, ".ext"), 0); CHECK_STR(b, "C:\\dir\\sub\\name.ext");
    CHECK_EQ_I(p_mps(b, sizeof b, "C", "\\dir", v_s, "ext"), 0);           CHECK_STR(b, "C:\\dir\\name.ext");
    CHECK_EQ_I(p_mps(b, sizeof b, NULL, NULL, v_s, NULL), 0);              CHECK_STR(b, "name");
    CHECK_EQ_I(p_mps(b, sizeof b, "", "", v_s, ""), 0);                    CHECK_STR(b, "name");
}
TC_CASE(exact_fit)
{
    char b[16]; v_s = "name";
    if (!p_mps) TC_SKIP("not exported");
    /* "C:\d\name.ext" is 13 chars: needs 14 */
    CHECK_EQ_I(p_mps(b, 14, "C:", "\\d\\", v_s, ".ext"), 0);
    CHECK_STR(b, "C:\\d\\name.ext");
}
TC_CASE(wide)
{
    wchar_t b[260], c[16];
    if (!p_wmps) TC_SKIP("not exported");
    CHECK_EQ_I(p_wmps(b, 260, L"C:", L"\\dir\\", L"name", L".ext"), 0); CHECK_WSTR(b, L"C:\\dir\\name.ext");
    CHECK_EQ_I(p_wmps(b, 260, L"C", L"\\dir", L"\x00e9", L"ext"), 0);   CHECK_WSTR(b, L"C:\\dir\\\x00e9.ext");
    CHECK_EQ_I(p_wmps(c, 14, L"C:", L"\\d\\", L"name", L".ext"), 0);    CHECK_WSTR(c, L"C:\\d\\name.ext");
}
TC_CASE(all_null)
{
    char b[8] = "xxxxxxx"; wchar_t w[8] = L"xxxxxxx";
    if (!p_mps || !p_wmps) TC_SKIP("not exported");
    CHECK_EQ_I(p_mps(b, sizeof b, NULL, NULL, NULL, NULL), 0); CHECK_STR(b, "");
    CHECK_EQ_I(p_wmps(w, 8, NULL, NULL, NULL, NULL), 0);       CHECK_WSTR(w, L"");
}
int main(void)
{
    tc_begin();
    p_mps = (mps_t)tc_crt_proc("_makepath_s"); p_wmps = (wmps_t)tc_crt_proc("_wmakepath_s");
    TC_RUN(exported); TC_RUN(vectors); TC_RUN(exact_fit); TC_RUN(wide); TC_RUN(all_null);
    return tc_end();
}
