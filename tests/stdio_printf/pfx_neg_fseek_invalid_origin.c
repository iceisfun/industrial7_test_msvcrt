/* TC
 * id:    pfx_neg_fseek_invalid_origin
 * area:  stdio_printf
 * kind:  negative
 * funcs: fseek ftell fopen fclose fputs
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fseek (invalid origin -> EINVAL, invalid parameter handler; negative resulting position fails)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile path = "tc_pfx_neg_fseek_origin_a.tmp";
static volatile int bad_origin = 99;

TC_CASE(origin_99)
{
    FILE *f = fopen(path, "wb+"); int r;
    CHECK(f != NULL); if (!f) return;
    fputs("0123456789", f);
    fseek(f, 4, SEEK_SET);
    tc_iph_count = 0; errno = 0;
    r = fseek(f, 0, bad_origin);
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(ftell(f), 4);
    fclose(f);
}
TC_CASE(negative_set)
{
    FILE *f = fopen(path, "rb"); int r;
    CHECK(f != NULL); if (!f) return;
    fseek(f, 3, SEEK_SET);
    errno = 0;
    r = fseek(f, -1, SEEK_SET);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(ftell(f), 3);
    fclose(f);
}
TC_CASE(seek_cur_still_works)
{
    FILE *f = fopen(path, "rb"); int r;
    CHECK(f != NULL); if (!f) return;
    fseek(f, 2, SEEK_SET);
    r = fseek(f, 3, SEEK_CUR);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(ftell(f), 5);
    fclose(f);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(origin_99); TC_RUN(negative_set); TC_RUN(seek_cur_still_works);
    DeleteFileA(path);
    return tc_end();
}
