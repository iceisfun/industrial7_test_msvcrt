/* TC
 * id:    gct_getdays_c_locale
 * area:  ctype
 * funcs: _Getdays
 * procs: free
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   internal helper used by C++ std::time_get (<xtime>/<yvals.h>)
 * note:  PROBE: _Getdays is an undocumented internal helper (msvcp support). Probed: result is a malloc'd (CRT free) string of the form
 *        ':Abbr:Full' x7 (leading ':', 14 colons, abbrev = first 3 chars of full name) in the C locale. Exact text is in gct_getdays_exact_string_probe.
 */
#include "tc.h"
extern char *__cdecl _Getdays(void);
TC_DECL_PROC(void, free, (void *));
static size_t slen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }

TC_CASE(non_null_colon_prefixed)
{
    char *d = _Getdays();
    CHECK(d != NULL); if (!d) return;
    CHECK_EQ_I(d[0], ':');
    CHECK(d[slen(d) - 1] != ':');
    if (tc_p_free) tc_p_free(d);
}
TC_CASE(fourteen_tokens)
{
    char *d = _Getdays(); size_t i, n, colons = 0, tok = 0, tlen = 0;
    CHECK(d != NULL); if (!d) return;
    n = slen(d);
    for (i = 0; i < n; i++) { if (d[i] == ':') { colons++; if (i > 0 && tlen == 0) tok = 999; tlen = 0; } else tlen++; }
    CHECK_EQ_U(colons, 14);
    CHECK(tok != 999);
    CHECK(tlen > 0);
    if (tc_p_free) tc_p_free(d);
}
TC_CASE(abbrev_is_prefix_of_full)
{
    char *d = _Getdays(); const char *p; int k;
    CHECK(d != NULL); if (!d) return;
    p = d;
    for (k = 0; k < 7; k++) {
        const char *ab = p + 1, *e = ab; const char *fu; size_t j = 0;
        while (*e && *e != ':') e++;
        CHECK(*e == ':'); if (*e != ':') break;
        fu = e + 1;
        while (ab + j < e && fu[j] == ab[j]) j++;
        CHECK(ab + j == e);
        CHECK(e - ab == 3);
        p = fu; while (*p && *p != ':') p++;
    }
    CHECK(*p == 0);
    if (tc_p_free) tc_p_free(d);
}
TC_CASE(distinct_blocks)
{
    char *a = _Getdays(), *b = _Getdays();
    CHECK(a != NULL && b != NULL);
    CHECK(a != b);
    if (a && b) CHECK(tc_strequ(a, b));
    CHECK(tc_p_free != NULL);
    if (tc_p_free) { tc_p_free(a); tc_p_free(b); }
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(free);
    TC_RUN(non_null_colon_prefixed); TC_RUN(fourteen_tokens); TC_RUN(abbrev_is_prefix_of_full); TC_RUN(distinct_blocks);
    return tc_end();
}
