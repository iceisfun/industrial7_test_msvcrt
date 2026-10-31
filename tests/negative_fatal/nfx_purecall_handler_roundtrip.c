/* TC
 * id:    nfx_purecall_handler_roundtrip
 * area:  negative_fatal
 * kind:  positive
 * funcs: 
 * procs: _set_purecall_handler _get_purecall_handler
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_purecall_handler
 * note:  Fresh process: no purecall handler is installed, so the first set returns NULL.
 */
#include "tc.h"

typedef void (__cdecl *pc_t)(void);
typedef pc_t (__cdecl *set_t)(pc_t);
typedef pc_t (__cdecl *get_t)(void);
static void __cdecl h1(void) { }
static void __cdecl h2(void) { }

TC_CASE(roundtrip)
{
    set_t set = (set_t)tc_crt_proc("_set_purecall_handler");
    get_t get = (get_t)tc_crt_proc("_get_purecall_handler");
    CHECK(set != NULL); CHECK(get != NULL);
    if (!set || !get) return;
    CHECK_EQ_P(set(h1), NULL);
    CHECK_EQ_P(get(), h1);
    CHECK_EQ_P(set(h2), h1);
    CHECK_EQ_P(get(), h2);
    CHECK_EQ_P(set(NULL), h2);
    CHECK_EQ_P(get(), NULL);
}
int main(void) { tc_begin(); TC_RUN(roundtrip); return tc_end(); }
