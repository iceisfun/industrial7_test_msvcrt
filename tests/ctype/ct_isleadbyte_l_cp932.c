/* TC
 * id:    ct_isleadbyte_l_cp932
 * area:  ctype
 * funcs: 
 * procs: _create_locale _free_locale _isleadbyte_l ___mb_cur_max_l_func
 * crt:   >=100
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs isleadbyte, _create_locale, MB_CUR_MAX
 * note:  Skipped if the Japanese locale cannot be created.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef int (__cdecl *il_t)(int, void *);
typedef int (__cdecl *mb_t)(void *);
static create_t p_create; static free_t p_free; static il_t p_isleadbyte_l; static mb_t p_mbmax;
static void *loc;

TC_CASE(create_or_skip)
{
    CHECK(p_create && p_free && p_isleadbyte_l && p_mbmax);
    if (!p_create) TC_SKIP("no _create_locale");
    loc = p_create(LC_ALL, "Japanese_Japan.932");
    if (!loc) TC_SKIP("cp932 locale unavailable");
}
TC_CASE(lead_true_0x81_0xE0)
{
    if (!loc) TC_SKIP("no locale");
    CHECK(p_isleadbyte_l(0x81, loc) != 0);
    CHECK(p_isleadbyte_l(0x9F, loc) != 0);
    CHECK(p_isleadbyte_l(0xE0, loc) != 0);
    CHECK(p_isleadbyte_l(0xFC, loc) != 0);
}
TC_CASE(trail_only_false_0x40_0xA0)
{
    if (!loc) TC_SKIP("no locale");
    CHECK_EQ_I(p_isleadbyte_l(0x40, loc), 0);
    CHECK_EQ_I(p_isleadbyte_l(0xA0, loc), 0);
    CHECK_EQ_I(p_isleadbyte_l(0xB1, loc), 0);
    CHECK_EQ_I(p_isleadbyte_l('A', loc), 0);
}
TC_CASE(mb_cur_max_l_is_2) { if (!loc) TC_SKIP("no locale"); CHECK_EQ_I(p_mbmax(loc), 2); }
TC_CASE(global_isleadbyte_still_false)
{
    if (!loc) TC_SKIP("no locale");
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(isleadbyte(0x81), 0);
    p_free(loc); loc = NULL;
}
int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    p_isleadbyte_l = (il_t)tc_crt_proc("_isleadbyte_l"); p_mbmax = (mb_t)tc_crt_proc("___mb_cur_max_l_func");
    TC_RUN(create_or_skip); TC_RUN(lead_true_0x81_0xE0); TC_RUN(trail_only_false_0x40_0xA0);
    TC_RUN(mb_cur_max_l_is_2); TC_RUN(global_isleadbyte_still_false);
    return tc_end();
}
