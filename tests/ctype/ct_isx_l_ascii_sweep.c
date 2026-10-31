/* TC
 * id:    ct_isx_l_ascii_sweep
 * area:  ctype
 * funcs: isalpha isdigit isspace ispunct isprint iscntrl isxdigit isalnum isgraph isupper islower
 * procs: _isalpha_l _isdigit_l _isspace_l _ispunct_l _isprint_l _iscntrl_l _isxdigit_l _isalnum_l _isgraph_l _isupper_l _islower_l _create_locale _free_locale
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs isalpha, isalpha_l etc.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

typedef int (__cdecl *isl_t)(int, void *);
typedef int (__cdecl *is_t)(int);
static void *(__cdecl *p_create)(int, const char *);
static void (__cdecl *p_free)(void *);
static void *loc;
static struct { const char *n; isl_t l; is_t f; } T[] = {
    {"_isalpha_l",0,isalpha},{"_isdigit_l",0,isdigit},{"_isspace_l",0,isspace},{"_ispunct_l",0,ispunct},
    {"_isprint_l",0,isprint},{"_iscntrl_l",0,iscntrl},{"_isxdigit_l",0,isxdigit},{"_isalnum_l",0,isalnum},
    {"_isgraph_l",0,isgraph},{"_isupper_l",0,isupper},{"_islower_l",0,islower}};
#define NT ((int)(sizeof T / sizeof T[0]))

TC_CASE(create_C_locale_non_null) { CHECK(p_create != NULL); if (!p_create) return; loc = p_create(LC_ALL, "C"); CHECK(loc != NULL); }
TC_CASE(each_function_matches_non_l_version)
{
    int k; volatile int c;
    if (!loc) TC_SKIP("no locale");
    for (k = 0; k < NT; k++) {
        CHECK(T[k].l != NULL); if (!T[k].l) continue;
        for (c = 0; c < 128; c++)
            if ((T[k].l(c, loc) != 0) != (T[k].f(c) != 0)) { tc_fail_expr(__FILE__, __LINE__, T[k].n); break; }
    }
}
TC_CASE(free_locale_ok) { if (!loc) TC_SKIP("no locale"); p_free(loc); loc = NULL; CHECK(1); }
int main(void)
{
    int k;
    tc_begin();
    p_create = (void *(__cdecl *)(int, const char *))tc_crt_proc("_create_locale");
    p_free = (void (__cdecl *)(void *))tc_crt_proc("_free_locale");
    for (k = 0; k < NT; k++) T[k].l = (isl_t)tc_crt_proc(T[k].n);
    TC_RUN(create_C_locale_non_null); TC_RUN(each_function_matches_non_l_version); TC_RUN(free_locale_ok);
    return tc_end();
}
