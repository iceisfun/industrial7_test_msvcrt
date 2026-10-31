/* TC
 * id:    gct_iswpunct_l_iswxdigit_l_ascii
 * area:  ctype
 * funcs: 
 * procs: _iswpunct_l _iswxdigit_l _create_locale _free_locale
 * crt:   >=100
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs iswpunct, _iswpunct_l, iswxdigit, _iswxdigit_l (VS2005+)
 * note:  msvcr90/80 import libs lack these; msvcrt.dll has no _iswpunct_l per MS docs, so crt starts at 100. Only zero/nonzero is asserted.
 */
#include "tc.h"
typedef void *loc_t;
TC_DECL_PROC(loc_t, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (loc_t));
TC_DECL_PROC(int, _iswpunct_l, (unsigned short, loc_t));
TC_DECL_PROC(int, _iswxdigit_l, (unsigned short, loc_t));
static loc_t loc;

static int is_punct_ascii(int c) { return (c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126); }
static int is_xdigit_ascii(int c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

TC_CASE(loaded) { CHECK(tc_p__iswpunct_l && tc_p__iswxdigit_l && tc_p__create_locale && tc_p__free_locale); CHECK(loc != NULL); }
TC_CASE(punct_true)
{
    int c, n = 0;
    if (!loc || !tc_p__iswpunct_l) TC_SKIP("unavailable");
    for (c = 0; c < 128; c++) if (is_punct_ascii(c)) { n++; if (!tc_p__iswpunct_l((unsigned short)c, loc)) { CHECK_EQ_I(c, -1); } }
    CHECK_EQ_I(n, 32);
}
TC_CASE(punct_false)
{
    int c;
    if (!loc || !tc_p__iswpunct_l) TC_SKIP("unavailable");
    for (c = 0; c < 128; c++) if (!is_punct_ascii(c) && tc_p__iswpunct_l((unsigned short)c, loc)) { CHECK_EQ_I(c, -1); }
    CHECK(!tc_p__iswpunct_l('a', loc)); CHECK(!tc_p__iswpunct_l('5', loc)); CHECK(!tc_p__iswpunct_l(' ', loc));
    CHECK(!tc_p__iswpunct_l(0, loc)); CHECK(!tc_p__iswpunct_l(31, loc)); CHECK(!tc_p__iswpunct_l(127, loc));
}
TC_CASE(xdigit_exact)
{
    int c;
    if (!loc || !tc_p__iswxdigit_l) TC_SKIP("unavailable");
    for (c = 0; c < 128; c++) { int r = tc_p__iswxdigit_l((unsigned short)c, loc) != 0; if (r != is_xdigit_ascii(c)) { CHECK_EQ_I(c, -1); } }
}
TC_CASE(xdigit_neighbours)
{
    if (!loc || !tc_p__iswxdigit_l) TC_SKIP("unavailable");
    CHECK(!tc_p__iswxdigit_l('g', loc)); CHECK(!tc_p__iswxdigit_l('G', loc));
    CHECK(!tc_p__iswxdigit_l('/', loc)); CHECK(!tc_p__iswxdigit_l(':', loc));
    CHECK(!tc_p__iswxdigit_l('@', loc)); CHECK(!tc_p__iswxdigit_l('`', loc));
    CHECK(tc_p__iswxdigit_l('0', loc)); CHECK(tc_p__iswxdigit_l('9', loc));
    CHECK(tc_p__iswxdigit_l('a', loc)); CHECK(tc_p__iswxdigit_l('F', loc));
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale); TC_LOAD_PROC(_iswpunct_l); TC_LOAD_PROC(_iswxdigit_l);
    if (tc_p__create_locale) loc = tc_p__create_locale(0 /* LC_ALL */, "C");
    TC_RUN(loaded); TC_RUN(punct_true); TC_RUN(punct_false); TC_RUN(xdigit_exact); TC_RUN(xdigit_neighbours);
    if (loc && tc_p__free_locale) tc_p__free_locale(loc);
    return tc_end();
}
