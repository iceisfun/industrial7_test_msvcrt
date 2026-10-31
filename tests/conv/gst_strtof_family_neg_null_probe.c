/* TC
 * id:    gst_strtof_family_neg_null_probe
 * area:  conv
 * funcs:
 * procs: _strtof_l _wcstof_l _wcstold_l
 * crt:   msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation
 * note:  Probes that msvcr120 _strtof_l/_wcstof_l/_wcstold_l validate a NULL string: return 0, errno EINVAL, handler called once.
 *        Each call runs in a child process (exit code bit0 ret==0, bit1 errno==EINVAL, bit2 handler==1; expect 7) so a
 *        crash (no validation) cannot take down the parent. Skipped if the export is missing.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <errno.h>

TC_DECL_PROC(float, _strtof_l, (const char *, char **, _locale_t));
TC_DECL_PROC(float, _wcstof_l, (const wchar_t *, wchar_t **, _locale_t));
TC_DECL_PROC(double, _wcstold_l, (const wchar_t *, wchar_t **, _locale_t));

static int run_child(const char *n)
{
    double d = 1.0; int ok;
    if (!tc_iph_install()) return 99;
    tc_iph_count = 0; errno = 0;
    if (n[0] == 's') d = tc_p__strtof_l(NULL, NULL, NULL);
    else if (n[0] == 'f') d = tc_p__wcstof_l(NULL, NULL, NULL);
    else d = tc_p__wcstold_l(NULL, NULL, NULL);
    ok = (d == 0.0 ? 1 : 0) | (errno == EINVAL ? 2 : 0) | (tc_iph_count == 1 ? 4 : 0);
    return ok;
}
static void chk(const char *n, void *p)
{
    DWORD code = 0;
    if (!p) TC_SKIP("export missing");
    CHECK(tc_spawn_self(n, &code, 20000)); CHECK_EQ_U(code, 7);
}
TC_CASE(strtof_l)  { chk("s", (void *)tc_p__strtof_l); }
TC_CASE(wcstof_l)  { chk("f", (void *)tc_p__wcstof_l); }
TC_CASE(wcstold_l) { chk("d", (void *)tc_p__wcstold_l); }
int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_strtof_l); TC_LOAD_PROC(_wcstof_l); TC_LOAD_PROC(_wcstold_l);
    if ((c = tc_child_name()) != NULL) return run_child(c);
    TC_RUN(strtof_l); TC_RUN(wcstof_l); TC_RUN(wcstold_l);
    return tc_end();
}
