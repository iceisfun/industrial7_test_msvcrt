/* TC
 * id:    cpp_neg_typeinfo_raw_name_not_undecorated
 * area:  cpp_runtime
 * funcs:
 * procs: ?raw_name@type_info@@QEBAPEBDXZ ?name@type_info@@QEBAPEBDPEAU__type_info_node@@@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs type_info::raw_name ("decorated name"), type_info::name ("undecorated name")
 * note:  Negative: raw_name() must NOT return the undecorated name. Fake type_info {vptr, _M_data, name[]} with raw name ".H".
 *        name() is called with a zeroed static __type_info_node (the compiler-support overload).
 */
#include "tc.h"

struct fti { void *vptr; void *data; char name[16]; };
struct node { void *mem; void *next; };
typedef const char *(__cdecl *raw_t)(const void *);
typedef const char *(__cdecl *name_t)(const void *, void *);
static raw_t p_raw; static name_t p_name;
static struct fti T1 = { NULL, NULL, ".H" };
static struct node N1;
#define READY() (p_raw && p_name)

TC_CASE(exported) { CHECK(READY()); }
TC_CASE(raw_name_is_internal_pointer)
{
    const char *r;
    if (!READY()) TC_SKIP("n/a");
    r = p_raw(&T1);
    CHECK_EQ_P(r, &T1.name[0]);
    CHECK_STR(r, ".H");
}
TC_CASE(raw_name_not_int) { if (!READY()) TC_SKIP("n/a"); CHECK(!tc_strequ(p_raw(&T1), "int")); }
TC_CASE(name_int_for_dot_H) { if (!READY()) TC_SKIP("n/a"); CHECK_STR(p_name(&T1, &N1), "int"); }
TC_CASE(name_distinct_from_raw)
{
    const char *n;
    if (!READY()) TC_SKIP("n/a");
    n = p_name(&T1, &N1);
    CHECK(n != NULL);
    CHECK(n != p_raw(&T1));
}
int main(void)
{
    tc_begin();
    p_raw = (raw_t)tc_crt_proc("?raw_name@type_info@@QEBAPEBDXZ");
    p_name = (name_t)tc_crt_proc("?name@type_info@@QEBAPEBDPEAU__type_info_node@@@Z");
    TC_RUN(exported); TC_RUN(raw_name_is_internal_pointer); TC_RUN(raw_name_not_int); TC_RUN(name_int_for_dot_H);
    TC_RUN(name_distinct_from_raw);
    return tc_end();
}
