/* TC
 * id:    cpp_neg_new_handler_size_probe_child
 * area:  cpp_runtime
 * funcs:
 * procs: ??2@YAPEAX_K@Z ?_set_new_handler@@YAP6AH_K@ZP6AH0@Z@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_new_handler (_PNH takes the requested size)
 * note:  PROBE: the new-handler is invoked with the size originally requested from operator new (SIZE_MAX/2 here, not rounded or
 *        adjusted). Child exits 60 if so, 61 if called with another value, 62 if never called (then new returned/threw).
 */
#include "tc.h"

typedef void *(__cdecl *new_t)(size_t);
typedef int (__cdecl *nh_t)(size_t);
typedef nh_t (__cdecl *setnh_t)(nh_t);
static new_t p_new;
static setnh_t p_setnh;
static int __cdecl h_size(size_t n) { ExitProcess(n == (size_t)-1 / 2 ? 60 : 61); return 0; }
static LONG CALLBACK veh(PEXCEPTION_POINTERS ep)
{
    if (ep->ExceptionRecord->ExceptionCode == 0xE06D7363u) ExitProcess(62);
    return EXCEPTION_CONTINUE_SEARCH;
}

TC_CASE(handler_gets_size)
{
    DWORD code = 0; int ran = tc_spawn_self("size", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 60);
}

int main(void)
{
    tc_begin();
    p_new = (new_t)tc_crt_proc("??2@YAPEAX_K@Z");
    p_setnh = (setnh_t)tc_crt_proc("?_set_new_handler@@YAP6AH_K@ZP6AH0@Z@Z");
    if (tc_child_name()) {
        void *p;
        if (!p_new || !p_setnh) return 90;
        AddVectoredExceptionHandler(1, veh);
        p_setnh(h_size);
        p = p_new((size_t)-1 / 2);
        return p ? 102 : 62;
    }
    TC_RUN(handler_gets_size);
    return tc_end();
}
