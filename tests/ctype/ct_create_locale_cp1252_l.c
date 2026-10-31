/* TC
 * id:    ct_create_locale_cp1252_l
 * area:  ctype
 * funcs: 
 * procs: _create_locale _free_locale _isalpha_l _toupper_l _tolower_l _isupper_l _islower_l
 * crt:   >=100
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _create_locale, _isalpha_l, _toupper_l
 * note:  Locale object must not change the global C locale.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef int (__cdecl *cl_t)(int, void *);
static create_t p_create; static free_t p_free;
static cl_t p_isalpha_l, p_toupper_l, p_tolower_l, p_isupper_l, p_islower_l;
static void *loc;

TC_CASE(create_or_skip)
{
    CHECK(p_create && p_free && p_isalpha_l && p_toupper_l && p_tolower_l && p_isupper_l && p_islower_l);
    if (!p_create) TC_SKIP("no _create_locale");
    loc = p_create(LC_ALL, "English_United States.1252");
    if (!loc) TC_SKIP("cp1252 locale unavailable");
}
TC_CASE(isalpha_l_0xE9_true)
{
    if (!loc) TC_SKIP("no locale");
    CHECK(p_isalpha_l(0xE9, loc) != 0);
    CHECK(p_islower_l(0xE9, loc) != 0);
    CHECK(p_isupper_l(0xC9, loc) != 0);
    CHECK_EQ_I(p_isupper_l(0xE9, loc), 0);
}
TC_CASE(toupper_l_0xE9_0xC9) { if (!loc) TC_SKIP("no locale"); CHECK_EQ_I(p_toupper_l(0xE9, loc), 0xC9); }
TC_CASE(tolower_l_0xC9_0xE9) { if (!loc) TC_SKIP("no locale"); CHECK_EQ_I(p_tolower_l(0xC9, loc), 0xE9); }
TC_CASE(global_isalpha_0xE9_still_false)
{
    if (!loc) TC_SKIP("no locale");
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(isalpha(0xE9), 0);
    CHECK_EQ_I(toupper(0xE9), 0xE9);
}
TC_CASE(free_locale) { if (!loc) TC_SKIP("no locale"); p_free(loc); loc = NULL; }

int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    p_isalpha_l = (cl_t)tc_crt_proc("_isalpha_l"); p_toupper_l = (cl_t)tc_crt_proc("_toupper_l");
    p_tolower_l = (cl_t)tc_crt_proc("_tolower_l"); p_isupper_l = (cl_t)tc_crt_proc("_isupper_l");
    p_islower_l = (cl_t)tc_crt_proc("_islower_l");
    TC_RUN(create_or_skip); TC_RUN(isalpha_l_0xE9_true); TC_RUN(toupper_l_0xE9_0xC9);
    TC_RUN(tolower_l_0xC9_0xE9); TC_RUN(global_isalpha_0xE9_still_false); TC_RUN(free_locale);
    return tc_end();
}
