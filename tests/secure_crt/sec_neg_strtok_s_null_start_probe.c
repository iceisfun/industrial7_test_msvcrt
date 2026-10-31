/* TC
 * id:    sec_neg_strtok_s_null_start_probe
 * area:  secure_crt
 * funcs: strtok_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtok_s (str NULL continues from *context)
 * note:  PROBE: strtok_s(NULL, d, &ctx) with ctx==NULL (no prior state) is believed to be an invalid parameter (NULL+EINVAL); after a string is exhausted the context is believed to be non-NULL (points at terminator) so NULL str returns NULL silently.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static char *volatile v_str;

TC_CASE(null_str_null_ctx_returns_null_and_EINVAL) { char *ctx = NULL; v_str = NULL; RESET();
    CHECK(strtok_s(v_str, " ", &ctx) == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_str_after_exhausted_string_returns_null_no_error) { char s[] = "a"; char *ctx = NULL; char *t;
    t = strtok_s(s, " ", &ctx); CHECK(t != NULL); t = strtok_s(NULL, " ", &ctx); CHECK(t == NULL); RESET();
    v_str = NULL; t = strtok_s(v_str, " ", &ctx); CHECK(t == NULL); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(null_str_null_ctx_returns_null_and_EINVAL); TC_RUN(null_str_after_exhausted_string_returns_null_no_error); return tc_end(); }
