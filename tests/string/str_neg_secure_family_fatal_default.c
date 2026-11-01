/* TC
 * id:    str_neg_secure_family_fatal_default
 * area:  string
 * funcs:
 * procs: strcat_s strncpy_s strncat_s strtok_s _strlwr_s _strupr_s _strset_s _strnset_s
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs "Parameter validation" and _set_invalid_parameter_handler: without a handler the process is terminated; strcat_s, strncpy_s, strncat_s, strtok_s, _strlwr_s, _strupr_s, _strset_s, _strnset_s
 * note:  One child process per function; asserts abnormal termination only (not 0, not the 77 "returned" marker). Control children with a handler installed must return 77.
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>

TC_DECL_PROC(int, strcat_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));
TC_DECL_PROC(int, strncat_s, (char *, size_t, const char *, size_t));
TC_DECL_PROC(char *, strtok_s, (char *, const char *, char **));
TC_DECL_PROC(int, _strlwr_s, (char *, size_t));
TC_DECL_PROC(int, _strupr_s, (char *, size_t));
TC_DECL_PROC(int, _strset_s, (char *, size_t, int));
TC_DECL_PROC(int, _strnset_s, (char *, size_t, int, size_t));

static const char *const names[] = { "strcat_s", "strncpy_s", "strncat_s", "strtok_s", "lwr_s", "upr_s", "set_s", "nset_s" };
#define NN 8

static void trigger(int k)
{
    char buf[8]; char tok[8]; char nt[4];
    char *volatile np = NULL; const char *volatile ns = NULL;
    buf[0] = 'a'; buf[1] = 0; tok[0] = 'a'; tok[1] = 0;
    nt[0] = 'a'; nt[1] = 'b'; nt[2] = 'c'; nt[3] = 'd'; /* no terminator within size 4 */
    switch (k) {
    case 0: tc_p_strcat_s(np, 8, "x"); break;
    case 1: tc_p_strncpy_s(buf, 0, "x", 1); break;
    case 2: tc_p_strncat_s(buf, 8, ns, 1); break;
    case 3: tc_p_strtok_s(tok, " ", NULL); break;
    case 4: tc_p__strlwr_s(buf, 0); break;
    case 5: tc_p__strupr_s(np, 8); break;
    case 6: tc_p__strset_s(nt, 4, 'x'); break;
    case 7: tc_p__strnset_s(np, 8, 'x', 1); break;
    }
}
static void run_all(int handler)
{
    int i;
    for (i = 0; i < NN; i++) {
        char nm[16]; DWORD code = 0; int ran, j = 0, ok;
        nm[j++] = handler ? 'i' : 'd'; nm[j++] = (char)('0' + i); nm[j] = 0;
        ran = tc_spawn_self(nm, &code, 20000);
        ok = ran && (handler ? code == 77 : (code != 0 && code != 77));
        if (!ok) tc_fail_expr(__FILE__, __LINE__, names[i]);
    }
}
TC_CASE(default_terminates) { run_all(0); }
TC_CASE(handler_returns) { run_all(1); }
int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(strcat_s); TC_LOAD_PROC(strncpy_s); TC_LOAD_PROC(strncat_s); TC_LOAD_PROC(strtok_s);
    TC_LOAD_PROC(_strlwr_s); TC_LOAD_PROC(_strupr_s); TC_LOAD_PROC(_strset_s); TC_LOAD_PROC(_strnset_s);
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'i') tc_iph_install(); else tc_iph_default();
        trigger(c[1] - '0');
        return 77;
    }
    TC_RUN(default_terminates); TC_RUN(handler_returns);
    return tc_end();
}
