/* TC
 * id:    exp_process_env_exported
 * area:  exports
 * funcs: 
 * procs: getenv _putenv _wgetenv _wputenv system signal raise _getpid _getcwd _chdir _mkdir _rmdir _wgetcwd exit _exit abort _cexit _c_exit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs process and environment control; MS docs _getcwd, _mkdir, _chdir, _rmdir
 * note:  atexit is covered by exp_probe_atexit_export. Functional cases use only the private variable TC_EXP_VAR and a temp directory under the cwd.
 */
#include "tc.h"

typedef int (__cdecl *putenv_t)(const char *);
typedef char *(__cdecl *getenv_t)(const char *);
typedef int (__cdecl *wputenv_t)(const wchar_t *);
typedef wchar_t *(__cdecl *wgetenv_t)(const wchar_t *);
typedef char *(__cdecl *getcwd_t)(char *, int);
typedef int (__cdecl *dir_t)(const char *);
typedef void (__cdecl *sig_h)(int);
typedef sig_h (__cdecl *signal_t)(int, sig_h);
typedef int (__cdecl *raise_t)(int);

TC_CASE(env_group)
{
    putenv_t pe = (putenv_t)tc_crt_proc("_putenv"); getenv_t ge = (getenv_t)tc_crt_proc("getenv");
    wputenv_t wpe = (wputenv_t)tc_crt_proc("_wputenv"); wgetenv_t wge = (wgetenv_t)tc_crt_proc("_wgetenv");
    CHECK(pe && ge && wpe && wge);
    if (!(pe && ge && wpe && wge)) return;
    CHECK_EQ_I(pe("TC_EXP_VAR=hello"), 0);
    CHECK_STR(ge("TC_EXP_VAR"), "hello");
    CHECK_EQ_I(pe("TC_EXP_VAR="), 0);          /* documented: empty value removes the variable */
    CHECK(ge("TC_EXP_VAR") == NULL);
    CHECK_EQ_I(wpe(L"TC_EXP_WVAR=wide"), 0);
    CHECK(wge(L"TC_EXP_WVAR") != NULL);
    if (wge(L"TC_EXP_WVAR")) CHECK_WSTR(wge(L"TC_EXP_WVAR"), L"wide");
    CHECK_EQ_I(wpe(L"TC_EXP_WVAR="), 0);
    CHECK(wge(L"TC_EXP_WVAR") == NULL);
    CHECK(tc_crt_proc("system") != NULL);
}
TC_CASE(dir_group)
{
    getcwd_t gc = (getcwd_t)tc_crt_proc("_getcwd"); dir_t mk = (dir_t)tc_crt_proc("_mkdir");
    dir_t rm = (dir_t)tc_crt_proc("_rmdir"); dir_t cd = (dir_t)tc_crt_proc("_chdir");
    char before[MAX_PATH], now[MAX_PATH];
    int i, ok;
    CHECK(gc && mk && rm && cd); CHECK(tc_crt_proc("_wgetcwd") != NULL); CHECK(tc_crt_proc("_getpid") != NULL);
    if (!(gc && mk && rm && cd)) return;
    CHECK(gc(before, MAX_PATH) == before);
    CHECK_EQ_I(mk("tc_exp_process_env_dir.tmp"), 0);
    CHECK_EQ_I(cd("tc_exp_process_env_dir.tmp"), 0);
    CHECK(gc(now, MAX_PATH) == now);
    { int n = 0, m = 0; while (now[n]) n++; while (before[m]) m++; CHECK(n > m); }
    ok = 1; for (i = 0; before[i]; i++) if (now[i] != before[i]) ok = 0;
    CHECK(ok);
    CHECK_EQ_I(cd(".."), 0);
    CHECK_EQ_I(rm("tc_exp_process_env_dir.tmp"), 0);
}
TC_CASE(exit_group)
{
    static const char *const names[] = { "exit", "_exit", "abort", "_cexit", "_c_exit" };
    unsigned i;
    for (i = 0; i < sizeof names / sizeof names[0]; i++) if (!tc_crt_proc(names[i])) tc_fail_expr(__FILE__, __LINE__, names[i]);
}
static volatile int sig_hits;
static void __cdecl on_term(int s) { (void)s; sig_hits++; }
TC_CASE(signal_group)
{
    signal_t sg = (signal_t)tc_crt_proc("signal"); raise_t rs = (raise_t)tc_crt_proc("raise");
    sig_h old;
    CHECK(sg && rs);
    if (!(sg && rs)) return;
    old = sg(15 /* SIGTERM */, on_term);
    CHECK((intptr_t)old != -1);
    CHECK_EQ_I(rs(15), 0);
    CHECK_EQ_I(sig_hits, 1);
    sg(15, old);
}
int main(void)
{
    tc_begin();
    TC_RUN(env_group); TC_RUN(dir_group); TC_RUN(exit_group); TC_RUN(signal_group);
    return tc_end();
}
