/* TC
 * id:    hp_new_handler_invoked_by_callnewh
 * area:  heap
 * funcs: _callnewh _set_new_handler
 * crt:   win10
 * tier:  p1
 * conf:  inferred
 * ref:   MS docs _set_new_handler (handler receives the failed size; nonzero return = retry); _callnewh is not documented
 * note:  _callnewh(size) calls the installed new handler with size and returns nonzero iff the handler returned nonzero; with no
 *        handler it returns 0. Declared by hand (not in public headers).
 */
#include "tc.h"
#include <stddef.h>
typedef int (__cdecl *tc_pnh)(size_t);
int __cdecl _set_new_mode(int);
int __cdecl _query_new_mode(void);
tc_pnh __cdecl _set_new_handler(tc_pnh);
tc_pnh __cdecl _query_new_handler(void);

int __cdecl _callnewh(size_t);

static int g_calls; static size_t g_size; static int g_ret;
static int __cdecl h(size_t n) { g_calls++; g_size = n; return g_ret; }

TC_CASE(handler_returns_1)
{
    tc_pnh volatile hp = h; volatile size_t sz = 77; tc_pnh old;
    g_calls = 0; g_ret = 1;
    old = _set_new_handler(hp);
    CHECK(_callnewh(sz) != 0);
    CHECK_EQ_I(g_calls, 1);
    CHECK_EQ_U(g_size, 77);
    _set_new_handler(old);
}
TC_CASE(handler_returns_0)
{
    tc_pnh volatile hp = h; volatile size_t sz = 5; tc_pnh old;
    g_calls = 0; g_ret = 0;
    old = _set_new_handler(hp);
    CHECK_EQ_I(_callnewh(sz), 0);
    CHECK_EQ_I(g_calls, 1);
    _set_new_handler(old);
}
TC_CASE(no_handler)
{
    volatile size_t sz = 9;
    CHECK(_query_new_handler() == NULL);
    CHECK_EQ_I(_callnewh(sz), 0);
}

int main(void) { tc_begin(); TC_RUN(handler_returns_1); TC_RUN(handler_returns_0); TC_RUN(no_handler); return tc_end(); }
