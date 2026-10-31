/* TC
 * id:    cpp_neg_typeinfo_name_bogus_mangling_probe
 * area:  cpp_runtime
 * funcs:
 * procs: ?name@type_info@@QEBAPEBDPEAU__type_info_node@@@Z ??1type_info@@UEAA@XZ
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs type_info::name (undecorates the stored name)
 * note:  PROBE: name() on malformed decorated names ".?!bogus", "." and an all-zero name must not crash. The result may legitimately be NULL
 *        (undecoration failure) or a string; only crash-freedom and, if non-NULL, readability are asserted. Also probes that a second name()
 *        call on a successfully named object returns the same cached pointer, and that ??1type_info (frees any cached name) does not corrupt the heap. Fake type_info {vptr, _M_data, name[]}, zero-padded.
 */
#include "tc.h"

struct fti { void *vptr; void *data; char name[16]; };
struct node { void *mem; void *next; };
typedef const char *(__cdecl *name_t)(const void *, void *);
typedef void (__cdecl *dtor_t)(void *);
static name_t p_name; static dtor_t p_dtor;
static struct node N1;
#define READY() (p_name && p_dtor)
static void touch(const char *s) { volatile char c; if (s) { while (*s) s++; c = *s; (void)c; } CHECK(1); }

TC_CASE(exported) { CHECK(READY()); }
TC_CASE(bogus_name_no_crash)
{
    static struct fti t = { NULL, NULL, ".?!bogus" };
    if (!READY()) TC_SKIP("n/a");
    touch(p_name(&t, &N1));
}
TC_CASE(dot_only_no_crash)
{
    static struct fti t = { NULL, NULL, "." };
    if (!READY()) TC_SKIP("n/a");
    touch(p_name(&t, &N1));
}
TC_CASE(empty_name_no_crash)
{
    static struct fti t = { NULL, NULL, "" };
    const char *r;
    if (!READY()) TC_SKIP("n/a");
    r = p_name(&t, &N1);
    touch(r);
}
TC_CASE(name_second_call_cached)
{
    static struct fti t = { NULL, NULL, ".H" };
    const char *a, *b;
    if (!READY()) TC_SKIP("n/a");
    a = p_name(&t, &N1); b = p_name(&t, &N1);
    CHECK(a != NULL);
    CHECK_EQ_P(a, b);
    p_dtor(&t);
}
TC_CASE(dtor_after_name_no_crash)
{
    static struct fti t = { NULL, NULL, ".H" };
    const char *n; void *p;
    if (!READY()) TC_SKIP("n/a");
    n = p_name(&t, &N1);
    CHECK(n != NULL);
    CHECK(t.data != NULL);
    p_dtor(&t);
    p = HeapAlloc(GetProcessHeap(), 0, 100);
    CHECK(p != NULL);
    if (p) HeapFree(GetProcessHeap(), 0, p);
    CHECK(HeapValidate(GetProcessHeap(), 0, NULL));
}
int main(void)
{
    tc_begin();
    p_name = (name_t)tc_crt_proc("?name@type_info@@QEBAPEBDPEAU__type_info_node@@@Z");
    p_dtor = (dtor_t)tc_crt_proc("??1type_info@@UEAA@XZ");
    TC_RUN(exported); TC_RUN(bogus_name_no_crash); TC_RUN(dot_only_no_crash); TC_RUN(empty_name_no_crash); TC_RUN(name_second_call_cached); TC_RUN(dtor_after_name_no_crash);
    return tc_end();
}
