/* TC
 * id:    cpp_validate_rw_probe
 * area:  cpp_runtime
 * funcs:
 * procs: ?_ValidateRead@@YAHPEBXI@Z ?_ValidateWrite@@YAHPEAXI@Z ?_ValidateExecute@@YAHP6A_JXZ@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   IsBadReadPtr / IsBadWritePtr / IsBadCodePtr (inverted result)
 * note:  PROBE: the _Validate* exports return nonzero when the memory is accessible. Good-memory cases only; the
 *        failing cases are in cpp_neg_validate_invalid. x64 decoration (x86 differs in the mangled names).
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

TC_CASE(read_stack_buffer) { char buf[16] = {0}; if (!p_r) TC_SKIP("missing"); CHECK(p_r(buf, 16) != 0); }
TC_CASE(write_stack_buffer) { char buf[16] = {0}; if (!p_w) TC_SKIP("missing"); CHECK(p_w(buf, 16) != 0); }
TC_CASE(read_zero_len) { char buf[16] = {0}; if (!p_r) TC_SKIP("missing"); CHECK(p_r(buf, 0) != 0); }
TC_CASE(execute_function) { if (!p_x) TC_SKIP("missing"); CHECK(p_x((void *)tc_begin) != 0); }
TC_CASE(read_only_page_read)
{
    void *pg; if (!p_r) TC_SKIP("missing");
    pg = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READONLY);
    CHECK(pg != NULL);
    if (pg) { CHECK(p_r(pg, 4096) != 0); VirtualFree(pg, 0, MEM_RELEASE); }
}

int main(void)
{
    tc_begin();
    p_r = (vr_t)tc_crt_proc(N_R); p_w = (vw_t)tc_crt_proc(N_W); p_x = (vx_t)tc_crt_proc(N_X);
    CHECK_EQ_I(p_r && p_w && p_x, 1);
    TC_RUN(read_stack_buffer); TC_RUN(write_stack_buffer); TC_RUN(read_zero_len); TC_RUN(execute_function); TC_RUN(read_only_page_read);
    return tc_end();
}
