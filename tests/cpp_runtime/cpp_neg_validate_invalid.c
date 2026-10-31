/* TC
 * id:    cpp_neg_validate_invalid
 * area:  cpp_runtime
 * kind:  negative
 * funcs:
 * procs: ?_ValidateRead@@YAHPEBXI@Z ?_ValidateWrite@@YAHPEAXI@Z ?_ValidateExecute@@YAHP6A_JXZ@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   IsBadReadPtr / IsBadWritePtr / IsBadCodePtr (inverted result)
 * note:  PROBE: the _Validate* exports return 0 for NULL and for pages whose protection forbids the access
 *        (PAGE_NOACCESS, write to PAGE_READONLY, a read range running into a PAGE_NOACCESS page).
 *        x64 decoration (x86 differs in the mangled names).
 */
#include "tc.h"

#ifdef _WIN64
#define N_R "?_ValidateRead@@YAHPEBXI@Z"
#define N_W "?_ValidateWrite@@YAHPEAXI@Z"
#define N_X "?_ValidateExecute@@YAHP6A_JXZ@Z"
#else
#define N_R "?_ValidateRead@@YAHPBXI@Z"
#define N_W "?_ValidateWrite@@YAHPAXI@Z"
#define N_X "?_ValidateExecute@@YAHP6GHXZ@Z"
#endif
typedef int (__cdecl *vr_t)(const void *, unsigned);
typedef int (__cdecl *vw_t)(void *, unsigned);
typedef int (__cdecl *vx_t)(void *);
static vr_t p_r; static vw_t p_w; static vx_t p_x;

TC_CASE(read_null) { if (!p_r) TC_SKIP("missing"); CHECK_EQ_I(p_r(NULL, 16), 0); }
TC_CASE(write_null) { if (!p_w) TC_SKIP("missing"); CHECK_EQ_I(p_w(NULL, 16), 0); }
TC_CASE(execute_null) { if (!p_x) TC_SKIP("missing"); CHECK_EQ_I(p_x(NULL), 0); }
TC_CASE(noaccess_page)
{
    void *pg; if (!p_r || !p_w) TC_SKIP("missing");
    pg = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_NOACCESS);
    CHECK(pg != NULL);
    if (pg) { CHECK_EQ_I(p_r(pg, 16), 0); CHECK_EQ_I(p_w(pg, 16), 0); VirtualFree(pg, 0, MEM_RELEASE); }
}
TC_CASE(readonly_page_write)
{
    void *pg; if (!p_w) TC_SKIP("missing");
    pg = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READONLY);
    CHECK(pg != NULL);
    if (pg) { CHECK_EQ_I(p_w(pg, 16), 0); VirtualFree(pg, 0, MEM_RELEASE); }
}
TC_CASE(straddle_into_noaccess)
{
    char *pg; DWORD old; if (!p_r) TC_SKIP("missing");
    pg = (char *)VirtualAlloc(NULL, 8192, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    CHECK(pg != NULL);
    if (pg) {
        CHECK(VirtualProtect(pg + 4096, 4096, PAGE_NOACCESS, &old));
        CHECK(p_r(pg + 4096 - 16, 16) != 0);        /* control: fully inside the readable page */
        CHECK_EQ_I(p_r(pg + 4096 - 16, 32), 0);     /* runs 16 bytes into the no-access page */
        VirtualFree(pg, 0, MEM_RELEASE);
    }
}

int main(void)
{
    tc_begin();
    p_r = (vr_t)tc_crt_proc(N_R); p_w = (vw_t)tc_crt_proc(N_W); p_x = (vx_t)tc_crt_proc(N_X);
    TC_RUN(read_null); TC_RUN(write_null); TC_RUN(execute_null); TC_RUN(noaccess_page); TC_RUN(readonly_page_write); TC_RUN(straddle_into_noaccess);
    return tc_end();
}
