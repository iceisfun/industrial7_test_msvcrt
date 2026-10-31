/* TC
 * id:    loc_mbsinit_basic
 * area:  locale
 * funcs:
 * procs: mbsinit mbrtowc
 * crt:   >=100
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.6.2.1 mbsinit, 7.29.6.3.2 mbrtowc
 * note:  mbsinit resolved at run time; skipped when the export is absent. State buffer is oversized and zeroed so it is valid for both the 4-byte and 8-byte mbstate_t layouts.
 */
#include "tc.h"
#include <locale.h>

typedef int    (__cdecl *mbsinit_t)(const void *);
typedef size_t (__cdecl *mbrtowc_t)(wchar_t *, const char *, size_t, void *);
static mbsinit_t p_mbsinit; static mbrtowc_t p_mbrtowc;
typedef struct { unsigned long long a[4]; } st_t;

TC_CASE(null_is_initial) { if (!p_mbsinit) TC_SKIP("no mbsinit"); CHECK(p_mbsinit(NULL) != 0); }
TC_CASE(zero_state_initial) { st_t s = {{0,0,0,0}}; if (!p_mbsinit) TC_SKIP("no mbsinit"); CHECK(p_mbsinit(&s) != 0); }
TC_CASE(after_incomplete_not_initial)
{
    st_t s = {{0,0,0,0}}; wchar_t wc = 0; const char b[] = "\x82"; size_t r;
    volatile const char *n = ".932";
    if (!p_mbsinit || !p_mbrtowc) TC_SKIP("missing proc");
    if (!setlocale(LC_ALL, (const char *)n)) { setlocale(LC_ALL, "C"); TC_SKIP("932 rejected"); }
    r = p_mbrtowc(&wc, b, 1, &s);
    CHECK_EQ_U(r, (size_t)-2);
    CHECK(p_mbsinit(&s) == 0);
    setlocale(LC_ALL, "C");
}
TC_CASE(after_completion_initial)
{
    st_t s = {{0,0,0,0}}; wchar_t wc = 0; const char b[] = "\x82\xA0"; size_t r;
    volatile const char *n = ".932";
    if (!p_mbsinit || !p_mbrtowc) TC_SKIP("missing proc");
    if (!setlocale(LC_ALL, (const char *)n)) { setlocale(LC_ALL, "C"); TC_SKIP("932 rejected"); }
    r = p_mbrtowc(&wc, b, 1, &s);
    CHECK_EQ_U(r, (size_t)-2);
    r = p_mbrtowc(&wc, b + 1, 1, &s);
    CHECK_EQ_U(r, 1);
    CHECK_EQ_U(wc, 0x3042);
    CHECK(p_mbsinit(&s) != 0);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    p_mbsinit = (mbsinit_t)tc_crt_proc("mbsinit"); p_mbrtowc = (mbrtowc_t)tc_crt_proc("mbrtowc");
    TC_RUN(null_is_initial); TC_RUN(zero_state_initial); TC_RUN(after_incomplete_not_initial); TC_RUN(after_completion_initial);
    return tc_end();
}
