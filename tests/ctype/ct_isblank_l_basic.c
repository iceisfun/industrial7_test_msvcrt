/* TC
 * id:    ct_isblank_l_basic
 * area:  ctype
 * funcs: 
 * procs: _isblank_l _iswblank_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs isblank, _isblank_l, _iswblank_l, iswblank (VS2013+)
 */
#include "tc.h"
#include <locale.h>

static int (__cdecl *p_b)(int, void *);
static int (__cdecl *p_wb)(unsigned short, void *);
static void *(__cdecl *p_create)(int, const char *);
static void (__cdecl *p_free)(void *);
static void *loc;

TC_CASE(space_tab_true)
{
    loc = p_create ? p_create(LC_ALL, "C") : NULL;
    CHECK(loc != NULL); CHECK(p_b != NULL); if (!loc || !p_b) return;
    CHECK(p_b(' ', loc)); CHECK(p_b('\t', loc));
}
TC_CASE(other_false)
{
    if (!loc || !p_b) TC_SKIP("missing");
    CHECK(!p_b('a', loc)); CHECK(!p_b('\n', loc)); CHECK(!p_b('0', loc)); CHECK(!p_b('\0', loc)); CHECK(!p_b('\r', loc));
}
TC_CASE(wide_space_tab_true)
{
    CHECK(p_wb != NULL); if (!loc || !p_wb) return;
    CHECK(p_wb(L' ', loc)); CHECK(p_wb(L'\t', loc)); CHECK(!p_wb(L'a', loc)); CHECK(!p_wb(L'\n', loc));
    if (p_free) p_free(loc);
}
int main(void)
{
    tc_begin();
    p_create = (void *(__cdecl *)(int, const char *))tc_crt_proc("_create_locale");
    p_free = (void (__cdecl *)(void *))tc_crt_proc("_free_locale");
    p_b = (int (__cdecl *)(int, void *))tc_crt_proc("_isblank_l");
    p_wb = (int (__cdecl *)(unsigned short, void *))tc_crt_proc("_iswblank_l");
    TC_RUN(space_tab_true); TC_RUN(other_false); TC_RUN(wide_space_tab_true);
    return tc_end();
}
