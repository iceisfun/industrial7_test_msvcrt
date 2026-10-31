/* TC
 * id:    cpp_neg_new_huge_null_msvcrt_probe_child
 * area:  cpp_runtime
 * funcs:
 * procs: ??2@YAPEAX_K@Z ?_set_new_handler@@YAP6AH_K@ZP6AH0@Z@Z
 * crt:   msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs operator new (legacy, non-throwing behaviour of the old msvcrt.dll)
 * note:  PROBE: msvcrt.dll's exported operator new returns NULL (does not throw) for an impossible size and no handler.
 *        Child exit 101 = NULL, 100 = C++ exception, 102 = non-NULL.
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

static LONG CALLBACK veh(PEXCEPTION_POINTERS ep)
{
    if (ep->ExceptionRecord->ExceptionCode == 0xE06D7363u) ExitProcess(100);
    return EXCEPTION_CONTINUE_SEARCH;
}

TC_CASE(returns_null)
{
    DWORD code = 0; int ran = tc_spawn_self("huge", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 101);
}

int main(void)
{
    const char *c;
    tc_begin();
    resolve_new();
    if ((c = tc_child_name()) != NULL) {
        void *p;
        if (!p_new) return 90;
        AddVectoredExceptionHandler(1, veh);
        if (p_setnh) p_setnh(NULL);
        p = p_new((size_t)-1 / 2);
        return p ? 102 : 101;
    }
    TC_RUN(returns_null);
    return tc_end();
}
