/* TC
 * id:    cpp_neg_exception_copy_semantics
 * area:  cpp_runtime
 * funcs:
 * procs: ??0exception@std@@QEAA@XZ ??0exception@std@@QEAA@AEBQEBD@Z ??0exception@std@@QEAA@AEBV01@@Z ??4exception@std@@QEAAAEAV01@AEBV01@@Z ??1exception@std@@UEAA@XZ ?what@exception@std@@UEBAPEBDXZ
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs std::exception Class (copy constructor, operator=); C++ [exception]/ copy must not alias freed storage
 * note:  Lifetime hazards: copy must outlive the source, self-assignment must not free the message, default (message-less) objects copy as default.
 *        Source objects live in heap blocks (separate from the message storage); "Unknown exception" fallback is real-CRT behaviour.
 */
#include "tc.h"

typedef void *(__cdecl *ctor0_t)(void *);
typedef void *(__cdecl *ctorm_t)(void *, const char *const *);
typedef void *(__cdecl *copy_t)(void *, const void *);
typedef void *(__cdecl *assign_t)(void *, const void *);
typedef void (__cdecl *dtor_t)(void *);
typedef const char *(__cdecl *what_t)(const void *);
static ctor0_t p_ctor0; static ctorm_t p_ctorm; static copy_t p_copy; static assign_t p_assign; static dtor_t p_dtor; static what_t p_what;

static void *obj_new(void) { return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 64); }
static void obj_free(void *p) { HeapFree(GetProcessHeap(), 0, p); }
static void *make_msg(const char *s) { void *o = obj_new(); const char *volatile m = s; p_ctorm(o, (const char *const *)&m); return o; }
#define READY() (p_ctor0 && p_ctorm && p_copy && p_assign && p_dtor && p_what)

TC_CASE(exported) { CHECK(READY()); }
TC_CASE(copy_of_default_what_unknown)
{
    void *a, *b;
    if (!READY()) TC_SKIP("not exported");
    a = obj_new(); b = obj_new();
    p_ctor0(a); p_copy(b, a);
    CHECK_STR(p_what(b), "Unknown exception");
    p_dtor(b); p_dtor(a); obj_free(a); obj_free(b);
}
TC_CASE(copy_of_msg_same_text)
{
    void *a, *b;
    if (!READY()) TC_SKIP("not exported");
    a = make_msg("original message"); b = obj_new();
    p_copy(b, a);
    CHECK_STR(p_what(b), "original message");
    p_dtor(b); p_dtor(a); obj_free(a); obj_free(b);
}
TC_CASE(copy_survives_source_dtor)
{
    void *a, *b; int i;
    if (!READY()) TC_SKIP("not exported");
    a = make_msg("survivor"); b = obj_new();
    p_copy(b, a);
    p_dtor(a); obj_free(a);
    { char *junk = (char *)HeapAlloc(GetProcessHeap(), 0, 16); for (i = 0; i < 16 && junk; i++) junk[i] = 'x'; if (junk) HeapFree(GetProcessHeap(), 0, junk); }
    CHECK_STR(p_what(b), "survivor");
    p_dtor(b); obj_free(b);
}
TC_CASE(self_assign_keeps_message)
{
    void *a, *r;
    if (!READY()) TC_SKIP("not exported");
    a = make_msg("self");
    r = p_assign(a, a);
    CHECK_EQ_P(r, a);
    CHECK_STR(p_what(a), "self");
    p_dtor(a); obj_free(a);
}
TC_CASE(assign_default_over_msg)
{
    void *a, *d;
    if (!READY()) TC_SKIP("not exported");
    a = make_msg("will be replaced"); d = obj_new();
    p_ctor0(d);
    p_assign(a, d);
    CHECK_STR(p_what(a), "Unknown exception");
    p_dtor(a); p_dtor(d); obj_free(a); obj_free(d);
}
TC_CASE(assign_msg_over_default)
{
    void *a, *d;
    if (!READY()) TC_SKIP("not exported");
    a = make_msg("filled in"); d = obj_new();
    p_ctor0(d);
    p_assign(d, a);
    CHECK_STR(p_what(d), "filled in");
    p_dtor(a); obj_free(a);
    CHECK_STR(p_what(d), "filled in");
    p_dtor(d); obj_free(d);
}
int main(void)
{
    tc_begin();
    p_ctor0  = (ctor0_t)tc_crt_proc("??0exception@std@@QEAA@XZ");
    p_ctorm  = (ctorm_t)tc_crt_proc("??0exception@std@@QEAA@AEBQEBD@Z");
    p_copy   = (copy_t)tc_crt_proc("??0exception@std@@QEAA@AEBV01@@Z");
    p_assign = (assign_t)tc_crt_proc("??4exception@std@@QEAAAEAV01@AEBV01@@Z");
    p_dtor   = (dtor_t)tc_crt_proc("??1exception@std@@UEAA@XZ");
    p_what   = (what_t)tc_crt_proc("?what@exception@std@@UEBAPEBDXZ");
    TC_RUN(exported); TC_RUN(copy_of_default_what_unknown); TC_RUN(copy_of_msg_same_text); TC_RUN(copy_survives_source_dtor);
    TC_RUN(self_assign_keeps_message); TC_RUN(assign_default_over_msg); TC_RUN(assign_msg_over_default);
    return tc_end();
}
