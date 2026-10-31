/* TC
 * id:    exp_ucrt_pseudo_vars_exported
 * area:  exports
 * funcs: 
 * procs: __p__fmode __p__commode __p___argc __p___argv __p___wargv __p__environ __p__wenviron __p__pgmptr __p__wpgmptr __p__acmdln __p__wcmdln __daylight __timezone __tzname _get_pgmptr _get_wpgmptr _get_fmode _set_fmode
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   UCRT stdlib.h/time.h: _fmode, _environ are macros over __p_* accessors, _daylight/_timezone/_tzname over __daylight/__timezone/__tzname; MS docs _get_fmode/_set_fmode
 * note:  Calls only __p___argc/__p__fmode/__tzname (non-NULL) and the documented _get_fmode/_set_fmode.
 */
#include "tc.h"

static const char *const g_acc[] = { "__p__fmode", "__p__commode", "__p___argc", "__p___argv", "__p___wargv", "__p__environ", "__p__wenviron", "__p__pgmptr", "__p__wpgmptr", "__p__acmdln", "__p__wcmdln", "__daylight", "__timezone", "__tzname", "_get_pgmptr", "_get_wpgmptr", "_get_fmode", "_set_fmode", NULL };
TC_DECL_PROC(void *, __p___argc, (void));
TC_DECL_PROC(void *, __p__fmode, (void));
TC_DECL_PROC(void *, __tzname, (void));
TC_DECL_PROC(int, _get_fmode, (int *));
TC_DECL_PROC(int, _set_fmode, (int));

TC_CASE(p_accessors_resolve) { const char *const *n; for (n = g_acc; *n; n++) if (!tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n); }
TC_CASE(pointers_nonnull_when_called)
{
    if (!tc_p___p___argc || !tc_p___p__fmode) TC_SKIP("missing export");
    CHECK(tc_p___p___argc() != NULL);
    CHECK(tc_p___p__fmode() != NULL);
}
TC_CASE(fmode_get_set_roundtrip)
{
    int old = -1, cur = -1;
    if (!tc_p__get_fmode || !tc_p__set_fmode) TC_SKIP("missing export");
    CHECK_EQ_I(tc_p__get_fmode(&old), 0);
    CHECK_EQ_I(tc_p__set_fmode(0x8000), 0);   /* _O_BINARY */
    CHECK_EQ_I(tc_p__get_fmode(&cur), 0);
    CHECK_EQ_I(cur, 0x8000);
    CHECK_EQ_I(tc_p__set_fmode(0x4000), 0);   /* _O_TEXT */
    CHECK_EQ_I(tc_p__get_fmode(&cur), 0);
    CHECK_EQ_I(cur, 0x4000);
    if (old == 0x8000 || old == 0x4000) CHECK_EQ_I(tc_p__set_fmode(old), 0);
}
TC_CASE(tzname_accessor)
{
    if (!tc_p___tzname) TC_SKIP("missing export");
    CHECK(tc_p___tzname() != NULL);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(__p___argc); TC_LOAD_PROC(__p__fmode); TC_LOAD_PROC(__tzname);
    TC_LOAD_PROC(_get_fmode); TC_LOAD_PROC(_set_fmode);
    TC_RUN(p_accessors_resolve); TC_RUN(pointers_nonnull_when_called); TC_RUN(fmode_get_set_roundtrip); TC_RUN(tzname_accessor);
    return tc_end();
}
