/* TC
 * id:    cpp_neg_typeinfo_unequal_compare
 * area:  cpp_runtime
 * funcs:
 * procs: ??8type_info@@QEBA_NAEBV0@@Z ??9type_info@@QEBA_NAEBV0@@Z ?before@type_info@@QEBA_NAEBV1@@Z ?before@type_info@@QEBAHAEBV1@@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs type_info::operator==, operator!=, before (compare by decorated name); C++ [type.info]
 * note:  Negative: DIFFERENT types must compare unequal and before() must be a strict ordering. Fake type_info objects {vptr, _M_data, name[]}
 *        with raw names ".H" (int), ".J" (long) and a second distinct ".H". before() returns bool on msvcr110/120 and int on msvcr90/100;
 *        only the low byte of the result is examined (0 or nonzero).
 */
#include "tc.h"

struct fti { void *vptr; void *data; char name[16]; };
typedef unsigned char (__cdecl *cmp_t)(const void *, const void *);
static cmp_t p_eq, p_ne, p_before;

static struct fti A = { NULL, NULL, ".H" }, B = { NULL, NULL, ".J" }, A2 = { NULL, NULL, ".H" };
#define READY() (p_eq && p_ne && p_before)
static int T(unsigned char r) { return r != 0; }

TC_CASE(exported) { CHECK(READY()); }
TC_CASE(eq_different_false) { if (!READY()) TC_SKIP("n/a"); CHECK_EQ_I(T(p_eq(&A, &B)), 0); CHECK_EQ_I(T(p_eq(&B, &A)), 0); }
TC_CASE(ne_different_true) { if (!READY()) TC_SKIP("n/a"); CHECK_EQ_I(T(p_ne(&A, &B)), 1); CHECK_EQ_I(T(p_ne(&B, &A)), 1); }
TC_CASE(eq_same_name_distinct_objects_true)
{
    if (!READY()) TC_SKIP("n/a");
    CHECK(&A != &A2);
    CHECK_EQ_I(T(p_eq(&A, &A2)), 1);
    CHECK_EQ_I(T(p_ne(&A, &A2)), 0);
}
TC_CASE(eq_self_true) { if (!READY()) TC_SKIP("n/a"); CHECK_EQ_I(T(p_eq(&A, &A)), 1); CHECK_EQ_I(T(p_ne(&A, &A)), 0); }
TC_CASE(before_self_false) { if (!READY()) TC_SKIP("n/a"); CHECK_EQ_I(T(p_before(&A, &A)), 0); CHECK_EQ_I(T(p_before(&B, &B)), 0); }
TC_CASE(before_antisymmetric)
{
    int ab, ba;
    if (!READY()) TC_SKIP("n/a");
    ab = T(p_before(&A, &B)); ba = T(p_before(&B, &A));
    CHECK_EQ_I(ab + ba, 1);
}
TC_CASE(before_equal_names_false_both_ways)
{
    if (!READY()) TC_SKIP("n/a");
    CHECK_EQ_I(T(p_before(&A, &A2)), 0);
    CHECK_EQ_I(T(p_before(&A2, &A)), 0);
}
int main(void)
{
    tc_begin();
    p_eq = (cmp_t)tc_crt_proc("??8type_info@@QEBA_NAEBV0@@Z");
    p_ne = (cmp_t)tc_crt_proc("??9type_info@@QEBA_NAEBV0@@Z");
    p_before = (cmp_t)tc_crt_proc("?before@type_info@@QEBA_NAEBV1@@Z");
    if (!p_before) p_before = (cmp_t)tc_crt_proc("?before@type_info@@QEBAHAEBV1@@Z");
    TC_RUN(exported); TC_RUN(eq_different_false); TC_RUN(ne_different_true); TC_RUN(eq_same_name_distinct_objects_true);
    TC_RUN(eq_self_true); TC_RUN(before_self_false); TC_RUN(before_antisymmetric); TC_RUN(before_equal_names_false_both_ways);
    return tc_end();
}
