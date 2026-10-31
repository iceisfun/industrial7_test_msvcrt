/* TC
 * id:    pfm_3_neg_p_type_conflict_probe
 * area:  stdio_printf
 * funcs:
 * procs: _sprintf_p _swprintf_p
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf_p positional parameters (an argument may be referenced more than once)
 * note:  PROBE: "%1$d %1$s" references argument 1 as both int and char*. Believed to be rejected (return -1, one handler call, buffer not
 *        overrun). Only the bad-type-reuse is probed here; benign reuse ("%1$d %1$x") is asserted as a control since it is documented.
 */
#include "tc.h"
#include "pfq_shim.h"

#if TC_CRT_VER >= 140
#define SPF pq_sprintf_p
#define SWPF pq_swprintf_p
#else
typedef int (__cdecl *spf_t)(char *, size_t, const char *, ...);
typedef int (__cdecl *swpf_t)(wchar_t *, size_t, const wchar_t *, ...);
static spf_t SPF; static swpf_t SWPF;
#endif

/* the argument is a real string pointer so that an accepting CRT reading it as %s cannot crash */
static const char *volatile sarg = "ab";
static const wchar_t *volatile warg = L"ab";
static const char *volatile f_conf = "%1$d %1$s", *volatile f_ok = "%1$d %1$x";
static const wchar_t *volatile w_conf = L"%1$d %1$ls", *volatile w_ok = L"%1$d %1$x";

TC_CASE(control_same_type_reuse)
{
    char b[32]; wchar_t w[32]; int r;
    r = SPF(b, sizeof b, f_ok, 255); CHECK_EQ_I(r, 6); CHECK_STR(b, "255 ff");
    r = SWPF(w, 32, w_ok, 255); CHECK_EQ_I(r, 6); CHECK_WSTR(w, L"255 ff");
}
TC_CASE(narrow_conflict) { char b[32]; int r; tc_iph_count = 0; r = SPF(b, sizeof b, f_conf, sarg); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wide_conflict)   { wchar_t b[32]; int r; tc_iph_count = 0; r = SWPF(b, 32, w_conf, warg); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    SPF = (spf_t)tc_crt_proc("_sprintf_p"); SWPF = (swpf_t)tc_crt_proc("_swprintf_p");
    if (!SPF || !SWPF) { tc_write("TC FAIL " TC_TEST_ID "::__missing _p export\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(control_same_type_reuse); TC_RUN(narrow_conflict); TC_RUN(wide_conflict);
    return tc_end();
}
