/* TC
 * id:    sec_wcstok_s_basic
 * area:  secure_crt
 * funcs: wcstok_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strtok_s, wcstok_s
 * note:  in msvcr<140 wcstok is 2-arg legacy, wcstok_s is always 3-arg
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <wchar.h>
static wchar_t a[32], b[32];
static void set(wchar_t *d, const wchar_t *s) { int i = 0; do d[i] = s[i]; while (s[i++]); }

TC_CASE(simple)
{
    wchar_t *c = NULL, *t; set(a, L"one two three");
    t = wcstok_s(a, L" ", &c); CHECK_WSTR(t, L"one");
    t = wcstok_s(NULL, L" ", &c); CHECK_WSTR(t, L"two");
    t = wcstok_s(NULL, L" ", &c); CHECK_WSTR(t, L"three");
    t = wcstok_s(NULL, L" ", &c); CHECK(t == NULL);
}
TC_CASE(consecutive_delims)
{
    wchar_t *c = NULL, *t; set(a, L",,x,,,y,");
    t = wcstok_s(a, L",", &c); CHECK_WSTR(t, L"x");
    t = wcstok_s(NULL, L",", &c); CHECK_WSTR(t, L"y");
    t = wcstok_s(NULL, L",", &c); CHECK(t == NULL);
}
TC_CASE(only_delims)
{
    wchar_t *c = NULL, *t; set(a, L";;;");
    t = wcstok_s(a, L";", &c); CHECK(t == NULL);
}
TC_CASE(interleaved_contexts)
{
    wchar_t *c1 = NULL, *c2 = NULL, *t; set(a, L"a1 a2"); set(b, L"b1,b2");
    t = wcstok_s(a, L" ", &c1); CHECK_WSTR(t, L"a1");
    t = wcstok_s(b, L",", &c2); CHECK_WSTR(t, L"b1");
    t = wcstok_s(NULL, L" ", &c1); CHECK_WSTR(t, L"a2");
    t = wcstok_s(NULL, L",", &c2); CHECK_WSTR(t, L"b2");
}
int main(void) { tc_begin(); TC_RUN(simple); TC_RUN(consecutive_delims); TC_RUN(only_delims); TC_RUN(interleaved_contexts); return tc_end(); }
