/* TC
 * id:    pfm_3_neg_p_variants_bad_positional
 * area:  stdio_printf
 * funcs:
 * procs: _sprintf_p _swprintf_p _fprintf_p _printf_p
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf_p positional parameters (positions are 1-based; every conversion must be positional once any is; no gaps)
 * note:  PROBE: malformed positional formats ("%0$d" position zero, "%1$d%3$d" gap at 2, "%1$d %d" mixing) make the _p functions fail:
 *        return -1 and invoke the invalid-parameter handler exactly once. errno is not asserted (EINVAL expected but unsure). On pre-UCRT
 *        DLLs the functions are called through the export (mingw has no declarations); ucrtbase uses the SDK inline equivalents.
 *        The duplicate-position type conflict is in pfm_3_neg_p_type_conflict_probe.
 */
#include "tc.h"
#include "pfq_shim.h"

#define OUT "tc_pfm_3_pb.tmp"

#if TC_CRT_VER >= 140
#define SPF pq_sprintf_p
#define SWPF pq_swprintf_p
#define FPF pq_fprintf_p
#define PPF pq_printf_p
#else
typedef int (__cdecl *spf_t)(char *, size_t, const char *, ...);
typedef int (__cdecl *swpf_t)(wchar_t *, size_t, const wchar_t *, ...);
typedef int (__cdecl *fpf_t)(FILE *, const char *, ...);
typedef int (__cdecl *ppf_t)(const char *, ...);
static spf_t SPF; static swpf_t SWPF; static fpf_t FPF; static ppf_t PPF;
#endif

static const char *volatile f_zero = "%0$d", *volatile f_gap = "%1$d%3$d", *volatile f_mix = "%1$d %d";
static const wchar_t *volatile w_zero = L"%0$d", *volatile w_gap = L"%1$d%3$d", *volatile w_mix = L"%1$d %d";

#define BAD(call) do { int r_; tc_iph_count = 0; r_ = (call); CHECK_EQ_I(r_, -1); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(sprintf_p_zero) { char b[32]; BAD(SPF(b, sizeof b, f_zero, 1)); }
TC_CASE(sprintf_p_gap)  { char b[32]; BAD(SPF(b, sizeof b, f_gap, 1, 2, 3)); }
TC_CASE(sprintf_p_mix)  { char b[32]; BAD(SPF(b, sizeof b, f_mix, 1, 2)); }
TC_CASE(swprintf_p_zero){ wchar_t b[32]; BAD(SWPF(b, 32, w_zero, 1)); }
TC_CASE(swprintf_p_gap) { wchar_t b[32]; BAD(SWPF(b, 32, w_gap, 1, 2, 3)); }
TC_CASE(swprintf_p_mix) { wchar_t b[32]; BAD(SWPF(b, 32, w_mix, 1, 2)); }
TC_CASE(fprintf_p_bad)
{
    FILE *fp = fopen(OUT, "w");
    CHECK(fp != NULL); if (!fp) return;
    BAD(FPF(fp, f_zero, 1)); BAD(FPF(fp, f_gap, 1, 2, 3));
    fclose(fp);
}
TC_CASE(printf_p_bad_child) { DWORD code = 99; CHECK(tc_spawn_self("pp", &code, 20000)); CHECK_EQ_U(code, 0); }

static int child_main(const char *n)
{
    int r1, r2, bad = 0; (void)n;
    if (!freopen(OUT, "w", stdout)) return 70;
    tc_iph_install();
    tc_iph_count = 0; r1 = PPF(f_zero, 1);
    if (r1 != -1 || tc_iph_count != 1) bad |= 1;
    tc_iph_count = 0; r2 = PPF(f_gap, 1, 2, 3);
    if (r2 != -1 || tc_iph_count != 1) bad |= 2;
    return bad;
}
int main(void)
{
    const char *c;
    tc_begin();
#if TC_CRT_VER < 140
    SPF = (spf_t)tc_crt_proc("_sprintf_p"); SWPF = (swpf_t)tc_crt_proc("_swprintf_p");
    FPF = (fpf_t)tc_crt_proc("_fprintf_p"); PPF = (ppf_t)tc_crt_proc("_printf_p");
    if (!SPF || !SWPF || !FPF || !PPF) { tc_write("TC FAIL " TC_TEST_ID "::__missing _p export\n"); return 2; }
#endif
    if ((c = tc_child_name()) != NULL) return child_main(c);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_p_zero); TC_RUN(sprintf_p_gap); TC_RUN(sprintf_p_mix);
    TC_RUN(swprintf_p_zero); TC_RUN(swprintf_p_gap); TC_RUN(swprintf_p_mix);
    TC_RUN(fprintf_p_bad); TC_RUN(printf_p_bad_child);
    remove(OUT);
    return tc_end();
}
