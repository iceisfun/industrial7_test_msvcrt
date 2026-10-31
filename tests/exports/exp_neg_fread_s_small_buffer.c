/* TC
 * id:    exp_neg_fread_s_small_buffer
 * area:  exports
 * kind:  negative
 * funcs: fopen fclose remove fputs
 * procs: fread_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fread_s: bufferSize < elementSize*count -> invalid parameter handler, ERANGE, returns 0; NULL buffer -> EINVAL
 * note:  Buffer content after failure is not asserted. File tc_exp_neg_fread_s.tmp.
 */
#include "tc.h"
#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(size_t, fread_s, (void *, size_t, size_t, size_t, FILE *));
static const char *volatile fn = "tc_exp_neg_fread_s_" TC_STR(TC_CRT_VER) ".tmp";
static FILE *f;
static char buf[32];

static void mk(void) { FILE *w = fopen(fn, "wb"); if (w) { fputs("0123456789abcdef", w); fclose(w); } f = fopen(fn, "rb"); }
static void rm(void) { if (f) fclose(f); f = NULL; remove(fn); }

TC_CASE(returns_0_iph_erange)
{
    mk(); CHECK(f && tc_p_fread_s);
    if (f && tc_p_fread_s) { tc_iph_count = 0; errno = 0; CHECK_EQ_U(tc_p_fread_s(buf, 4, 1, 8, f), 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
    rm();
}
TC_CASE(null_buffer_einval)
{
    mk(); CHECK(f && tc_p_fread_s);
    if (f && tc_p_fread_s) { tc_iph_count = 0; errno = 0; CHECK_EQ_U(tc_p_fread_s(NULL, 8, 1, 4, f), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
    rm();
}
TC_CASE(zero_size_returns_0)
{
    mk(); CHECK(f && tc_p_fread_s);
    if (f && tc_p_fread_s) { CHECK_EQ_U(tc_p_fread_s(buf, sizeof buf, 0, 4, f), 0); CHECK_EQ_U(tc_p_fread_s(buf, sizeof buf, 4, 0, f), 0); }
    rm();
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(fread_s); tc_iph_install();
    TC_RUN(returns_0_iph_erange); TC_RUN(null_buffer_einval); TC_RUN(zero_size_returns_0);
    return tc_end();
}
