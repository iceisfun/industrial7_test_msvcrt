/* TC
 * id:    cpp_neg_new_handler_called_child
 * area:  cpp_runtime
 * funcs:
 * procs: ??2@YAPEAX_K@Z _set_new_handler ?_set_new_handler@@YAP6AH_K@ZP6AH0@Z@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_new_handler, operator new (handler is called on allocation failure; returning 0 ends the retry loop)
 * note:  Child 'zero': handler counts calls and returns 0; then operator new fails: exit 110+count if it threw (vectored handler),
 *        120+count if it returned NULL. The size argument passed to the handler is NOT asserted here (see cpp_neg_new_handler_size_probe_child).
 */
#include "tc.h"

typedef void *(__cdecl *new_t)(size_t);
typedef int (__cdecl *nh_t)(size_t);
typedef nh_t (__cdecl *setnh_t)(nh_t);
static new_t p_new;
static setnh_t p_setnh;
static void resolve_new(void)
{
    p_new = (new_t)tc_crt_proc("??2@YAPEAX_K@Z");
    p_setnh = (setnh_t)tc_crt_proc("?_set_new_handler@@YAP6AH_K@ZP6AH0@Z@Z");
}

static int calls;
static int __cdecl h_zero(size_t n) { (void)n; calls++; return 0; }
static LONG CALLBACK veh(PEXCEPTION_POINTERS ep)
{
    if (ep->ExceptionRecord->ExceptionCode == 0xE06D7363u) ExitProcess(110 + calls);
    return EXCEPTION_CONTINUE_SEARCH;
}

TC_CASE(handler_returning_zero_ends)
{
    DWORD code = 0; int ran = tc_spawn_self("zero", &code, 20000);
    CHECK(ran);
    CHECK(code == 111 || code == 121);
}

int main(void)
{
    const char *c;
    tc_begin();
    resolve_new();
    if ((c = tc_child_name()) != NULL) {
        void *p;
        if (!p_new || !p_setnh) return 90;
        AddVectoredExceptionHandler(1, veh);
        p_setnh(h_zero);
        p = p_new((size_t)-1 / 2);
        return p ? 102 : 120 + calls;
    }
    TC_RUN(handler_returning_zero_ends);
    return tc_end();
}
