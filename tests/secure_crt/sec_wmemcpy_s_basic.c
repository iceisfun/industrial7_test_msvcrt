/* TC
 * id:    sec_wmemcpy_s_basic
 * area:  secure_crt
 * funcs: 
 * procs: wmemcpy_s wmemmove_s
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs wmemcpy_s, wmemmove_s
 */
#include "tc.h"
#include <wchar.h>

typedef int (__cdecl *wmc_t)(wchar_t *, size_t, const wchar_t *, size_t);
static wmc_t p_cpy, p_mov;
static volatile size_t v_n;

TC_CASE(copy_elements) { wchar_t d[4] = {0}; static const wchar_t s[3] = {0x41, 0x4e2d, 0x43}; if (!p_cpy) TC_SKIP("not exported"); v_n = 3;
    CHECK_EQ_I(p_cpy(d, 4, s, v_n), 0); CHECK(d[0] == 0x41 && d[1] == 0x4e2d && d[2] == 0x43 && d[3] == 0); }
TC_CASE(destsize_in_wchars) { wchar_t d[3]; static const wchar_t s[3] = {1, 2, 3}; if (!p_cpy) TC_SKIP("not exported"); v_n = 3;
    CHECK_EQ_I(p_cpy(d, 3, s, v_n), 0); CHECK(d[2] == 3); }
TC_CASE(wmemmove_overlap) { wchar_t b[6] = {1, 2, 3, 4, 5, 6}; if (!p_mov) TC_SKIP("not exported"); v_n = 4;
    CHECK_EQ_I(p_mov(b + 1, 5, b, v_n), 0); CHECK(b[0] == 1 && b[1] == 1 && b[2] == 2 && b[3] == 3 && b[4] == 4 && b[5] == 6); }
TC_CASE(count_zero) { wchar_t d[2] = {9, 9}, s[2] = {1, 2}; if (!p_cpy) TC_SKIP("not exported"); v_n = 0;
    CHECK_EQ_I(p_cpy(d, 2, s, v_n), 0); CHECK(d[0] == 9 && d[1] == 9); }

int main(void) { tc_begin(); p_cpy = (wmc_t)tc_crt_proc("wmemcpy_s"); p_mov = (wmc_t)tc_crt_proc("wmemmove_s");
    TC_RUN(copy_elements); TC_RUN(destsize_in_wchars); TC_RUN(wmemmove_overlap); TC_RUN(count_zero); return tc_end(); }
