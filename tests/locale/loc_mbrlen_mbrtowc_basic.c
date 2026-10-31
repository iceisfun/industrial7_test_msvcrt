/* TC
 * id:    loc_mbrlen_mbrtowc_basic
 * area:  locale
 * funcs:
 * procs: mbrlen mbrtowc
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.6.3.1 mbrlen, 7.29.6.3.2 mbrtowc; MS docs mbrtowc
 * note:  wc-untouched check for s==NULL is only built for ucrtbase.
 */
#include "tc.h"
#include <locale.h>
#include <wchar.h>
#include <string.h>

typedef size_t (__cdecl *mbrtowc_t)(wchar_t *, const char *, size_t, mbstate_t *);
typedef size_t (__cdecl *mbrlen_t)(const char *, size_t, mbstate_t *);
static mbrtowc_t p_mbrtowc; static mbrlen_t p_mbrlen;
static const char * volatile v_np = NULL;
static volatile char v_a = 'A';

TC_CASE(ascii)
{
    if (!p_mbrtowc) TC_SKIP("mbrtowc not exported");
    char s[2]; wchar_t wc = 0; mbstate_t st; memset(&st, 0, sizeof st);
    s[0] = v_a; s[1] = 0;
    CHECK_EQ_U(p_mbrtowc(&wc, s, 1, &st), 1);
    CHECK_EQ_I(wc, L'A');
}
TC_CASE(nul_returns_0)
{
    if (!p_mbrtowc) TC_SKIP("mbrtowc not exported");
    char s[2] = { 0, 0 }; wchar_t wc = 5; mbstate_t st; memset(&st, 0, sizeof st);
    CHECK_EQ_U(p_mbrtowc(&wc, s, 1, &st), 0);
    CHECK_EQ_I(wc, 0);
}
TC_CASE(null_string_returns_0)
{
    if (!p_mbrtowc) TC_SKIP("mbrtowc not exported");
    wchar_t wc = 7; mbstate_t st; memset(&st, 0, sizeof st);
    CHECK_EQ_U(p_mbrtowc(&wc, v_np, 1, &st), 0);
#ifdef TC_UCRT
    CHECK_EQ_I(wc, 7);   /* C11: acts as mbrtowc(NULL,"",1,ps); older CRTs not asserted */
#endif
}
TC_CASE(pwc_null_ok)
{
    if (!p_mbrtowc) TC_SKIP("mbrtowc not exported");
    mbstate_t st; memset(&st, 0, sizeof st);
    CHECK_EQ_U(p_mbrtowc(NULL, "A", 1, &st), 1);
}
TC_CASE(mbrlen_ascii)
{
    if (!p_mbrlen) TC_SKIP("mbrlen not exported");
    mbstate_t st; memset(&st, 0, sizeof st);
    CHECK_EQ_U(p_mbrlen("abc", 3, &st), 1);
}
int main(void) { tc_begin(); tc_iph_install(); p_mbrtowc=(mbrtowc_t)tc_crt_proc("mbrtowc"); p_mbrlen=(mbrlen_t)tc_crt_proc("mbrlen"); TC_RUN(ascii); TC_RUN(nul_returns_0); TC_RUN(null_string_returns_0); TC_RUN(pwc_null_ok); TC_RUN(mbrlen_ascii); return tc_end(); }
