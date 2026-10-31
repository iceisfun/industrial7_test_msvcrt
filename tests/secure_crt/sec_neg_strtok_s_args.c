/* TC
 * id:    sec_neg_strtok_s_args
 * area:  secure_crt
 * funcs: strtok_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtok_s, wcstok_s (invalid delimiter or context => NULL + EINVAL)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static volatile const char *v_delim;

TC_CASE(null_delim_returns_null_EINVAL) { char s[] = "a b"; char *ctx = NULL; v_delim = NULL; RESET();
    CHECK(strtok_s(s, (const char *)v_delim, &ctx) == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_context_pointer_returns_null_EINVAL) { char s[] = "a b"; char **volatile pc = NULL; RESET();
    CHECK(strtok_s(s, " ", (char **)pc) == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(handler_counts) { char s[] = "a b"; char *ctx = NULL; char **volatile pc = NULL; v_delim = NULL; RESET();
    strtok_s(s, (const char *)v_delim, &ctx); CHECK_EQ_I(tc_iph_count, 1);
    strtok_s(s, " ", (char **)pc); CHECK_EQ_I(tc_iph_count, 2); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(null_delim_returns_null_EINVAL); TC_RUN(null_context_pointer_returns_null_EINVAL); TC_RUN(handler_counts); return tc_end(); }
