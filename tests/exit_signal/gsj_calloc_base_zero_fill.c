/* TC
 * id:    gsj_calloc_base_zero_fill
 * area:  exit_signal
 * funcs:
 * procs: _calloc_base _free_base
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs calloc ("allocates storage space ... initializes to zero"); _calloc_base is the ucrt internal entry point
 */
#include "tc.h"

TC_DECL_PROC(void *, _calloc_base, (size_t, size_t));
TC_DECL_PROC(void, _free_base, (void *));
static volatile size_t v_n, v_s;

static int all_zero(const unsigned char *p, size_t n) { size_t i; for (i = 0; i < n; i++) if (p[i]) return 0; return 1; }

TC_CASE(sixteen_by_four)
{
    unsigned char *p; if (!tc_p__calloc_base) TC_SKIP("no _calloc_base");
    v_n = 16; v_s = 4; p = tc_p__calloc_base(v_n, v_s);
    CHECK(p != NULL);
    if (p) { CHECK(all_zero(p, 64)); tc_p__free_base(p); }
}
TC_CASE(one_byte)
{
    unsigned char *p; if (!tc_p__calloc_base) TC_SKIP("no _calloc_base");
    v_n = 1; v_s = 1; p = tc_p__calloc_base(v_n, v_s);
    CHECK(p != NULL);
    if (p) { CHECK_EQ_I(p[0], 0); tc_p__free_base(p); }
}
TC_CASE(large_zeroed)
{
    unsigned char *p; if (!tc_p__calloc_base) TC_SKIP("no _calloc_base");
    v_n = 1; v_s = 100000; p = tc_p__calloc_base(v_n, v_s);
    CHECK(p != NULL);
    if (p) { CHECK(all_zero(p, 100000)); CHECK_EQ_I(p[99999], 0); tc_p__free_base(p); }
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_calloc_base); TC_LOAD_PROC(_free_base);
    TC_RUN(sixteen_by_four); TC_RUN(one_byte); TC_RUN(large_zeroed);
    return tc_end();
}
