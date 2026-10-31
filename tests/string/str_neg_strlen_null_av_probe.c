/* TC
 * id:    str_neg_strlen_null_av_probe
 * area:  string
 * funcs:
 * procs: strlen strcpy strcat strcmp strchr strstr strspn strcspn strpbrk
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.24.1p2 (undefined for invalid pointers); legacy non-_s functions perform no parameter validation
 * note:  PROBE: calling the legacy functions with a NULL pointer dereferences it, so the child dies with STATUS_ACCESS_VIOLATION (0xC0000005) and the invalid-parameter handler is not involved (the child would return 77 if it survived). One child per function; the loop reports each deviating function by name. Functions are called through pointers from GetProcAddress so no compiler folding or intrinsic is possible.
 */
#include "tc.h"

static const char *const names[] = { "strlen", "strcpy", "strcat", "strcmp", "strchr", "strstr", "strspn", "strcspn", "strpbrk" };
#define NN 9
typedef void *(__cdecl *f2_t)(void *, const void *);
typedef void *(__cdecl *f1_t)(const void *);
typedef void *(__cdecl *fc_t)(const void *, int);

static void child(int k)
{
    static char one[4] = "a";
    void *volatile np = NULL;
    FARPROC p = tc_crt_proc(names[k]);
    if (!p) ExitProcess(78);
    switch (k) {
    case 0: ((f1_t)p)(np); break;                          /* strlen(NULL) */
    case 1: ((f2_t)p)(np, "a"); break;                     /* strcpy(NULL,"a") */
    case 2: ((f2_t)p)(np, "a"); break;                     /* strcat(NULL,"a") */
    case 3: ((f2_t)p)(one, np); break;                     /* strcmp("a",NULL) */
    case 4: ((fc_t)p)(np, 'a'); break;                     /* strchr(NULL,'a') */
    case 5: ((f2_t)p)(one, np); break;                     /* strstr("a",NULL) */
    case 6: ((f2_t)p)(np, "a"); break;                     /* strspn(NULL,"a") */
    case 7: ((f2_t)p)(one, np); break;                     /* strcspn("a",NULL) */
    case 8: ((f2_t)p)(one, np); break;                     /* strpbrk("a",NULL) */
    }
}
TC_CASE(null_pointer_access_violation)
{
    int i;
    for (i = 0; i < NN; i++) {
        char nm[8]; DWORD code = 0; int ran;
        nm[0] = 'n'; nm[1] = (char)('0' + i); nm[2] = 0;
        ran = tc_spawn_self(nm, &code, 20000);
        if (!ran || code != TC_STATUS_ACCESS_VIOLATION) tc_fail_expr(__FILE__, __LINE__, names[i]);
    }
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_install();
        child(c[1] - '0');
        return 77;
    }
    TC_RUN(null_pointer_access_violation);
    return tc_end();
}
