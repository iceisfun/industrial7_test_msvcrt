/* TC
 * id:    err_sys_nerr_probe
 * area:  errno_param
 * funcs:
 * procs: __sys_nerr strerror
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _sys_nerr (UCRT: macro for *__sys_nerr())
 * note:  PROBE: only a lower bound (>= 43, covering ENOENT..EILSEQ range) is asserted, not the exact value.
 */
#include "tc.h"

typedef int *(__cdecl *sys_nerr_t)(void);
typedef char *(__cdecl *strerror_t)(int);

TC_CASE(value)
{
    sys_nerr_t p = (sys_nerr_t)tc_crt_proc("__sys_nerr");
    strerror_t se = (strerror_t)tc_crt_proc("strerror");
    int *v; char *s;
    CHECK(p != NULL && se != NULL);
    if (!p || !se) return;
    v = p();
    CHECK(v != NULL);
    if (!v) return;
    CHECK(*v >= 43);
    s = se(*v - 1);
    CHECK(s != NULL && s[0] != 0);
}

int main(void)
{
    tc_begin();
    TC_RUN(value);
    return tc_end();
}
