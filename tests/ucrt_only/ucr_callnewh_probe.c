/* TC
 * id:    ucr_callnewh_probe
 * area:  ucrt_only
 * funcs: _callnewh
 * procs: _set_new_handler
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   CRT internal _callnewh(size_t); MS docs _set_new_handler
 * note:  PROBE: _callnewh returns 0 with no handler or when the handler returns 0, nonzero when the handler returns nonzero.
 */
#include "tc.h"

typedef int (__cdecl *nh_t)(size_t);
TC_DECL_PROC(nh_t, _set_new_handler, (nh_t));
extern int __cdecl _callnewh(size_t);

static volatile int g_cnt, g_ret;
static int __cdecl nh(size_t n) { (void)n; g_cnt++; return g_ret; }

TC_CASE(no_handler)
{
    if (!tc_p__set_new_handler) TC_SKIP("no _set_new_handler");
    tc_p__set_new_handler(NULL);
    CHECK_EQ_I(_callnewh(16), 0);
}
TC_CASE(handler_returns_one)
{
    if (!tc_p__set_new_handler) TC_SKIP("no _set_new_handler");
    g_cnt = 0; g_ret = 1;
    tc_p__set_new_handler(nh);
    CHECK(_callnewh(16) != 0); CHECK_EQ_I(g_cnt, 1);
    tc_p__set_new_handler(NULL);
}
TC_CASE(handler_returns_zero)
{
    if (!tc_p__set_new_handler) TC_SKIP("no _set_new_handler");
    g_cnt = 0; g_ret = 0;
    tc_p__set_new_handler(nh);
    CHECK_EQ_I(_callnewh(16), 0); CHECK_EQ_I(g_cnt, 1);
    tc_p__set_new_handler(NULL);
}

int main(void) { tc_begin(); TC_LOAD_PROC(_set_new_handler); CHECK(tc_p__set_new_handler != NULL); TC_RUN(no_handler); TC_RUN(handler_returns_one); TC_RUN(handler_returns_zero); return tc_end(); }
