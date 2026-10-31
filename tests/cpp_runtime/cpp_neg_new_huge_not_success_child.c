/* TC
 * id:    cpp_neg_new_huge_not_success_child
 * area:  cpp_runtime
 * funcs:
 * procs: ??2@YAPEAX_K@Z _set_new_handler ?_set_new_handler@@YAP6AH_K@ZP6AH0@Z@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs operator new (throws bad_alloc on failure; old msvcrt.dll/VC6-era behaviour returns NULL)
 * note:  operator new(SIZE_MAX/2) must never return a block. Throw (exit 100) and NULL return (exit 101) are both acceptable
 *        (old CRTs return NULL). Which one is the next test's probe. x64 decorated names only.
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

TC_CASE(child_never_succeeds)
{
    DWORD code = 0; int ran = tc_spawn_self("huge", &code, 20000);
    CHECK(ran);
    CHECK(code == 100 || code == 101);
    CHECK(code != 102);
}
TC_CASE(child_not_crash)
{
    DWORD code = 0; int ran = tc_spawn_self("huge", &code, 20000);
    CHECK(ran);
    CHECK(code != TC_STATUS_ACCESS_VIOLATION);
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
    TC_RUN(child_never_succeeds); TC_RUN(child_not_crash);
    return tc_end();
}
