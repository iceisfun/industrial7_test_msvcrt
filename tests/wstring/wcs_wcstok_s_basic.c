/* TC
 * id:    wcs_wcstok_s_basic
 * area:  wstring
 * funcs:
 * procs: wcstok_s
 * crt:   >=80
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strtok_s, _strtok_s_l, wcstok_s, _wcstok_s_l
 * note:  _s functions resolved via procs because import libs for msvcr80 lack them
 */
#include "tc.h"
#include <wchar.h>

typedef wchar_t *(__cdecl *wcstok_s_t)(wchar_t *, const wchar_t *, wchar_t **);
static wcstok_s_t tk;

TC_CASE(tokens)
{
    wchar_t b[] = L"a,bb;ccc"; wchar_t *ctx = NULL, *t;
    if (!tk) TC_SKIP("no wcstok_s");
    t = tk(b, L",;", &ctx); CHECK_WSTR(t, L"a");
    t = tk(NULL, L",;", &ctx); CHECK_WSTR(t, L"bb");
    t = tk(NULL, L",;", &ctx); CHECK_WSTR(t, L"ccc");
    t = tk(NULL, L",;", &ctx); CHECK(t == NULL);
}
TC_CASE(null_continuation_uses_context)
{
    wchar_t b[] = L"x y z"; wchar_t *ctx = NULL, *t;
    if (!tk) TC_SKIP("no wcstok_s");
    t = tk(b, L" ", &ctx); CHECK(t == b);
    t = tk(NULL, L" ", &ctx); CHECK(t == b + 2);
    CHECK(ctx != NULL);
    t = tk(NULL, L" ", &ctx); CHECK(t == b + 4);
    CHECK_WSTR(t, L"z");
}
TC_CASE(leading_delims)
{
    wchar_t b[] = L",,;abc,,def,"; wchar_t *ctx = NULL, *t;
    if (!tk) TC_SKIP("no wcstok_s");
    t = tk(b, L",;", &ctx); CHECK_WSTR(t, L"abc"); CHECK(t == b + 3);
    t = tk(NULL, L",;", &ctx); CHECK_WSTR(t, L"def");
    t = tk(NULL, L",;", &ctx); CHECK(t == NULL);
}
TC_CASE(empty_string)
{
    wchar_t b[] = L""; wchar_t c[] = L",,,"; wchar_t *ctx = NULL;
    if (!tk) TC_SKIP("no wcstok_s");
    CHECK(tk(b, L",", &ctx) == NULL);
    ctx = NULL;
    CHECK(tk(c, L",", &ctx) == NULL);
}
TC_CASE(two_contexts_independent)
{
    wchar_t a[] = L"a1 a2 a3"; wchar_t b[] = L"b1 b2 b3"; wchar_t *ca = NULL, *cb = NULL, *t;
    if (!tk) TC_SKIP("no wcstok_s");
    t = tk(a, L" ", &ca); CHECK_WSTR(t, L"a1");
    t = tk(b, L" ", &cb); CHECK_WSTR(t, L"b1");
    t = tk(NULL, L" ", &ca); CHECK_WSTR(t, L"a2");
    t = tk(NULL, L" ", &cb); CHECK_WSTR(t, L"b2");
    t = tk(NULL, L" ", &ca); CHECK_WSTR(t, L"a3");
    t = tk(NULL, L" ", &cb); CHECK_WSTR(t, L"b3");
    CHECK(tk(NULL, L" ", &ca) == NULL);
    CHECK(tk(NULL, L" ", &cb) == NULL);
}
TC_CASE(delimiter_set_changes)
{
    wchar_t b[] = L"ab:cd-ef"; wchar_t *ctx = NULL, *t;
    if (!tk) TC_SKIP("no wcstok_s");
    t = tk(b, L":", &ctx); CHECK_WSTR(t, L"ab");
    t = tk(NULL, L"-", &ctx); CHECK_WSTR(t, L"cd");
    t = tk(NULL, L"", &ctx); CHECK_WSTR(t, L"ef");
}
int main(void)
{
    tc_begin(); tk = (wcstok_s_t)tc_crt_proc("wcstok_s");
    TC_RUN(tokens); TC_RUN(null_continuation_uses_context); TC_RUN(leading_delims);
    TC_RUN(empty_string); TC_RUN(two_contexts_independent); TC_RUN(delimiter_set_changes);
    return tc_end();
}
