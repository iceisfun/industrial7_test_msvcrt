/* TC
 * id:    sec_neg_default_handler_fatal_tok_conv
 * area:  secure_crt
 * funcs: strtok_s wcstok_s _i64toa_s _ultow_s _ltoa_s strncpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation; strncpy_s (_TRUNCATE truncation returns STRUNCATE and does not invoke the handler)
 * note:  Failing calls run in a child with the default handler and must terminate; the last case is the control: _TRUNCATE
 *        truncation is a normal return, so the child reaches 'return 77'.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>

static volatile size_t v3 = 3, v8 = 8;
static volatile int v40 = 40;
static const char *volatile v_nd;
static char *volatile v_nb;
static wchar_t **volatile v_nctx;
static int child_main(const char *n)
{
    char s[8] = "a b", d[3]; wchar_t w[8] = L"a b", wb[16]; char *c = NULL; char b[2]; errno_t r;
    tc_iph_default();
    if (tc_strequ(n, "strtok_s")) strtok_s(s, v_nd, &c);
    else if (tc_strequ(n, "wcstok_s")) wcstok_s(w, L" ", v_nctx);
    else if (tc_strequ(n, "i64toa_s")) _i64toa_s(1234567LL, b, 2, 10);
    else if (tc_strequ(n, "ultow_s")) _ultow_s(5UL, wb, v8, v40);
    else if (tc_strequ(n, "ltoa_s")) _ltoa_s(5L, v_nb, v8, 10);
    else if (tc_strequ(n, "truncate_ok")) { r = strncpy_s(d, v3, "abcdef", (size_t)-1); if (r != STRUNCATE) return 55; }
    (void)b;
    return 77;
}
static void term(const char *n)
{
    DWORD code = 77; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
    CHECK(code != TC_STATUS_ACCESS_VIOLATION); /* a crash from a test bug must not count as the handler terminating */
}
TC_CASE(child_strtok_s_null_delim_terminates) { term("strtok_s"); }
TC_CASE(child_wcstok_s_null_context_terminates) { term("wcstok_s"); }
TC_CASE(child_i64toa_s_small_buffer_terminates) { term("i64toa_s"); }
TC_CASE(child_ultow_s_radix_40_terminates) { term("ultow_s"); }
TC_CASE(child_ltoa_s_null_buffer_terminates) { term("ltoa_s"); }
TC_CASE(child_truncate_strncpy_s_exits_77)
{
    DWORD code = 0; int ran = tc_spawn_self("truncate_ok", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(child_strtok_s_null_delim_terminates); TC_RUN(child_wcstok_s_null_context_terminates);
    TC_RUN(child_i64toa_s_small_buffer_terminates); TC_RUN(child_ultow_s_radix_40_terminates);
    TC_RUN(child_ltoa_s_null_buffer_terminates); TC_RUN(child_truncate_strncpy_s_exits_77);
    return tc_end();
}
