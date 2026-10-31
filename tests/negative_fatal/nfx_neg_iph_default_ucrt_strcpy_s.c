/* TC
 * id:    nfx_neg_iph_default_ucrt_strcpy_s
 * area:  negative_fatal
 * kind:  negative
 * funcs: 
 * procs: strcpy_s memcpy_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation", strcpy_s, memcpy_s
 * note:  Probes the exact termination status (fast-fail 0xC0000409) of the default invalid-parameter handler. Only "terminates, nonzero, not 77" is documented; the exact code assertions are in separate cases.
 */
#include "tc.h"

TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));

static DWORD run(const char *n, int *ran) { DWORD c = 0; *ran = tc_spawn_self(n, &c, 20000); return c; }

TC_CASE(terminates_nonzero)
{
    int ran; DWORD c = run("zero_size", &ran);
    CHECK(ran); CHECK(c != 0); CHECK(c != 77);
}
TC_CASE(zero_size_exact) { int ran; DWORD c = run("zero_size", &ran); CHECK(ran); CHECK_EQ_U(c, TC_STATUS_FASTFAIL); }
TC_CASE(memcpy_s_null_dst) { int ran; DWORD c = run("memcpy_null", &ran); CHECK(ran); CHECK_EQ_U(c, TC_STATUS_FASTFAIL); }
TC_CASE(strcpy_s_null_dst) { int ran; DWORD c = run("strcpy_null", &ran); CHECK(ran); CHECK_EQ_U(c, TC_STATUS_FASTFAIL); }

int main(void)
{
    const char *n; char d[8]; static const char s[] = "abcd";
    tc_begin();
    TC_LOAD_PROC(strcpy_s); TC_LOAD_PROC(memcpy_s);
    if ((n = tc_child_name()) != NULL) {
        if (!tc_p_strcpy_s || !tc_p_memcpy_s) return 78;
        if (n[0] == 'z') tc_p_strcpy_s(d, 0, s);
        else if (n[0] == 'm') tc_p_memcpy_s(NULL, 8, s, 4);
        else tc_p_strcpy_s(NULL, 8, s);
        return 77;
    }
    CHECK(tc_p_strcpy_s != NULL);
    TC_RUN(terminates_nonzero); TC_RUN(zero_size_exact); TC_RUN(memcpy_s_null_dst); TC_RUN(strcpy_s_null_dst);
    return tc_end();
}
