/* TC
 * id:    dat_neg_osver_null_fatal
 * area:  data_exports
 * funcs: 
 * procs: _get_osplatform _get_osver _get_winmajor _get_winminor
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _get_osplatform etc.: NULL is an invalid parameter
 * note:  PROBE: exports of these getters on msvcr1xx are uncertain (SKIP when absent); asserts only abnormal termination under the default handler.
 */
#include "tc.h"
typedef int (__cdecl *get_t)(unsigned int *);

static void abnormal(const char *name, const char *proc)
{
    DWORD code = 0; int ran;
    if (!tc_crt_proc(proc)) TC_SKIP("getter not exported");
    ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(osplatform) { abnormal("osplatform", "_get_osplatform"); }
TC_CASE(osver)      { abnormal("osver", "_get_osver"); }
TC_CASE(winmajor)   { abnormal("winmajor", "_get_winmajor"); }
TC_CASE(winminor)   { abnormal("winminor", "_get_winminor"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char nm[32] = "_get_"; int i = 5, j = 0; get_t f; unsigned int *volatile np = NULL;
        while (c[j]) nm[i++] = c[j++]; nm[i] = 0;
        f = (get_t)tc_crt_proc(nm);
        if (!f) return 3;
        f(np);
        return 77;
    }
    TC_RUN(osplatform); TC_RUN(osver); TC_RUN(winmajor); TC_RUN(winminor);
    return tc_end();
}
