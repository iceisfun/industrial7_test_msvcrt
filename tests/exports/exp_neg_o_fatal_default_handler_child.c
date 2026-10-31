/* TC
 * id:    exp_neg_o_fatal_default_handler_child
 * area:  exports
 * funcs:
 * procs: _o_strcpy_s _o__itoa_s _o_strtol _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation: default handler terminates the process
 * note:  PROBE: _o_ aliases go through the same validation, so with no handler the child dies (probe: fast-fail 0xC0000409); with a handler it returns normally.
 *        The _o_ aliases are undocumented; expectations mirror the documented behaviour of the underlying function.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>

typedef int (__cdecl *strcpy_s_t)(char *, size_t, const char *);
typedef int (__cdecl *itoa_s_t)(int, char *, size_t, int);
typedef long (__cdecl *strtol_t)(const char *, char **, int);
static strcpy_s_t p_strcpy_s; static itoa_s_t p_itoa_s; static strtol_t p_strtol;
static volatile int radix = 99;

static void check_fatal(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
    CHECK(code == TC_STATUS_FASTFAIL || code == TC_STATUS_INVALID_CRT_PARAM);
}
TC_CASE(strcpy_s_null) { if (!p_strcpy_s) TC_SKIP("not exported"); check_fatal("strcpy"); }
TC_CASE(itoa_s_bad_radix) { if (!p_itoa_s) TC_SKIP("not exported"); check_fatal("itoa"); }
TC_CASE(strtol_bad_base) { if (!p_strtol) TC_SKIP("not exported"); check_fatal("strtol"); }
TC_CASE(control_with_handler)
{
    DWORD code = 0; int ran;
    if (!p_strcpy_s || !p_itoa_s || !p_strtol) TC_SKIP("not exported");
    ran = tc_spawn_self("control", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
static int child_main(const char *c)
{
    char buf[8]; volatile char *nul = NULL;
    if (c[0] == 'c') {
        if (!tc_iph_install() || !p_strcpy_s) return 1;
        p_strcpy_s((char *)nul, 4, "a"); p_itoa_s(1, buf, 8, radix); p_strtol("1", NULL, radix);
        return 77;
    }
    if (c[0] == 's' && c[3] == 'c') p_strcpy_s((char *)nul, 4, "a");
    else if (c[0] == 'i') p_itoa_s(1, buf, 8, radix);
    else if (c[0] == 's') p_strtol("1", NULL, radix);
    return 77;
}
int main(void)
{
    const char *c;
    tc_begin();
    p_strcpy_s = (strcpy_s_t)tc_crt_proc("_o_strcpy_s"); p_itoa_s = (itoa_s_t)tc_crt_proc("_o__itoa_s");
    p_strtol = (strtol_t)tc_crt_proc("_o_strtol");
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(strcpy_s_null); TC_RUN(itoa_s_bad_radix); TC_RUN(strtol_bad_base); TC_RUN(control_with_handler);
    return tc_end();
}
