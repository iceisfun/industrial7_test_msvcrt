/* TC
 * id:    mb_neg_mbstok_s_errors
 * area:  mbcs
 * funcs:
 * procs: _mbstok_s _mbstok_s_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtok_s, _mbstok_s, _mbstok_s_l (NULL context / NULL delimiters / NULL str with NULL *context -> EINVAL, return NULL)
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
typedef unsigned char *(__cdecl *tok_t)(unsigned char *, const unsigned char *, unsigned char **);
typedef unsigned char *(__cdecl *tok_l_t)(unsigned char *, const unsigned char *, unsigned char **, void *);
static tok_t p_tok; static tok_l_t p_tok_l;
static unsigned char ** volatile nctx = NULL;
static const unsigned char * volatile ndelim = NULL;
static unsigned char * volatile nstr = NULL;
static const unsigned char delim[] = " ";
static const unsigned char * volatile dp = delim;
#define BEGIN do { errno = 0; tc_iph_count = 0; } while (0)
#define EXPECT do { CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); } while (0)

TC_CASE(null_context)
{
    unsigned char s[] = "a b";
    CHECK(p_tok && p_tok_l); if (!p_tok || !p_tok_l) return;
    BEGIN; CHECK(p_tok(s, dp, nctx) == NULL); EXPECT;
    BEGIN; CHECK(p_tok_l(s, dp, nctx, NULL) == NULL); EXPECT;
}
TC_CASE(null_delim)
{
    unsigned char s[] = "a b", *ctx = NULL;
    CHECK(p_tok); if (!p_tok) return;
    BEGIN; CHECK(p_tok(s, ndelim, &ctx) == NULL); EXPECT;
}
TC_CASE(null_str_with_null_ctx)
{
    unsigned char *ctx = NULL;
    CHECK(p_tok); if (!p_tok) return;
    BEGIN; CHECK(p_tok(nstr, dp, &ctx) == NULL); EXPECT;
}
TC_CASE(exhausted_returns_null_no_iph)
{
    unsigned char s[] = "a b", *ctx = NULL, *t;
    CHECK(p_tok); if (!p_tok) return;
    BEGIN;
    t = p_tok(s, dp, &ctx); CHECK(t != NULL); if (t) CHECK_STR((char *)t, "a");
    t = p_tok(NULL, dp, &ctx); CHECK(t != NULL); if (t) CHECK_STR((char *)t, "b");
    CHECK(p_tok(NULL, dp, &ctx) == NULL);
    CHECK(p_tok(NULL, dp, &ctx) == NULL);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_tok = (tok_t)tc_crt_proc("_mbstok_s"); p_tok_l = (tok_l_t)tc_crt_proc("_mbstok_s_l");
    TC_RUN(null_context); TC_RUN(null_delim); TC_RUN(null_str_with_null_ctx); TC_RUN(exhausted_returns_null_no_iph);
    return tc_end();
}
