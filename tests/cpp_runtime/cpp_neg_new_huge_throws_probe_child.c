/* TC
 * id:    cpp_neg_new_huge_throws_probe_child
 * area:  cpp_runtime
 * funcs:
 * procs: ??2@YAPEAX_K@Z _set_new_handler ?_set_new_handler@@YAP6AH_K@ZP6AH0@Z@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C++ [new.delete.single]; MS docs operator new
 * note:  PROBE: operator new(SIZE_MAX/2) with no handler raises a C++ exception (code 0xE06D7363, ExceptionInformation[0]
 *        == 0x19930520) in msvcr90..120. Child exit 100 = that exception, 99 = exception with other magic, 101 = NULL returned.
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
    EXCEPTION_RECORD *r = ep->ExceptionRecord;
    if (r->ExceptionCode == 0xE06D7363u)
        ExitProcess(r->NumberParameters >= 1 && r->ExceptionInformation[0] == 0x19930520u ? 100 : 99);
    return EXCEPTION_CONTINUE_SEARCH;
}

TC_CASE(throws_cxx_exception)
{
    DWORD code = 0; int ran = tc_spawn_self("huge", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 100);
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
    TC_RUN(throws_cxx_exception);
    return tc_end();
}
