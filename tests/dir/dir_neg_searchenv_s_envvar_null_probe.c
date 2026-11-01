/* TC
 * id:    dir_neg_searchenv_s_envvar_null_probe
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _searchenv_s _wsearchenv_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _searchenv_s
 * note:  Probes _searchenv_s with envvar == NULL: (a) file present in cwd -> 0 (the cwd is searched before the variable is consulted),
 *        (b) absent file -> ENOENT (or EINVAL if NULL envvar is rejected) with buf[0]==0, and (c) filename "" -> ENOENT or EINVAL, buf[0]==0, no crash. Wide counterparts.
 *        The invalid parameter handler is installed because EINVAL+handler is the alternative outcome.
 *        Win10 22H2 19045 (observed): case null_env_absent terminated the whole test with 0xC0000409 (fast fail) although a returning handler was installed; it now
 *        runs narrow and wide in child processes asserting 0xC0000409 (class C). empty_filename and null_env_present are unchanged and pass.
 *        crt narrowed to ucrtbase: the crash / fast-fail behaviour was observed only on ucrtbase 19045; older CRTs are [unverified] (original expectation not asserted there).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(errno_t, _searchenv_s, (const char *, const char *, char *, size_t));
TC_DECL_PROC(errno_t, _wsearchenv_s, (const wchar_t *, const wchar_t *, wchar_t *, size_t));

#define FN "tc_dir_neg_sev_null.tmp"
static const char *volatile v_fn = FN, *volatile v_absent = "tc_dir_neg_sev_absent.nope", *volatile v_empty = "";
static const wchar_t *volatile v_wfn = L"" FN, *volatile v_wabsent = L"tc_dir_neg_sev_absent.nope", *volatile v_wempty = L"";
static const char *volatile v_nullenv;
static const wchar_t *volatile v_wnullenv;

static void mk(void) { HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); if (h != INVALID_HANDLE_VALUE) CloseHandle(h); }

TC_CASE(null_env_present)
{
    char buf[MAX_PATH]; wchar_t wb[MAX_PATH]; errno_t r;
    if (!tc_p__searchenv_s || !tc_p__wsearchenv_s) TC_SKIP("not exported");
    tc_iph_install(); mk();
    r = tc_p__searchenv_s(v_fn, v_nullenv, buf, MAX_PATH); CHECK_EQ_I(r, 0);
    r = tc_p__wsearchenv_s(v_wfn, v_wnullenv, wb, MAX_PATH); CHECK_EQ_I(r, 0);
    DeleteFileA(FN);
}
/* Windows (class C): with envvar == NULL and a file that is not in the cwd the CRT fast-fails (0xC0000409) even though a returning invalid
 * parameter handler is installed, so each variant runs in a child. */
TC_CASE(null_env_absent)
{
    DWORD code = 0; int ran;
    if (!tc_p__searchenv_s || !tc_p__wsearchenv_s) TC_SKIP("not exported");
    ran = tc_spawn_self("narrow", &code, 20000); CHECK(ran); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
    ran = tc_spawn_self("wide", &code, 20000); CHECK(ran); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(empty_filename)
{
    char buf[MAX_PATH]; wchar_t wb[MAX_PATH]; errno_t r;
    if (!tc_p__searchenv_s || !tc_p__wsearchenv_s) TC_SKIP("not exported");
    tc_iph_install();
    buf[0] = 'X'; wb[0] = L'X';
    r = tc_p__searchenv_s(v_empty, "TC_DIR_NO_SUCH_VAR", buf, MAX_PATH); CHECK(r == ENOENT || r == EINVAL); CHECK_EQ_I(buf[0], 0);
    r = tc_p__wsearchenv_s(v_wempty, L"TC_DIR_NO_SUCH_VAR", wb, MAX_PATH); CHECK(r == ENOENT || r == EINVAL); CHECK_EQ_I(wb[0], 0);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_searchenv_s); TC_LOAD_PROC(_wsearchenv_s);
    if (tc_child_name() != NULL) {
        char buf[MAX_PATH]; wchar_t wb[MAX_PATH];
        if (!tc_p__searchenv_s || !tc_p__wsearchenv_s) return 78;
        tc_iph_install();
        if (tc_child_name()[0] == 'n') tc_p__searchenv_s(v_absent, v_nullenv, buf, MAX_PATH);
        else tc_p__wsearchenv_s(v_wabsent, v_wnullenv, wb, MAX_PATH);
        return 77;
    }
    TC_RUN(null_env_present); TC_RUN(null_env_absent); TC_RUN(empty_filename);
    DeleteFileA(FN);
    return tc_end();
}
