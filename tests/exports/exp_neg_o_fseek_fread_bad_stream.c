/* TC
 * id:    exp_neg_o_fseek_fread_bad_stream
 * area:  exports
 * funcs:
 * procs: _o_fseek _o_fread _o__fseeki64 _o_ftell _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fseek, _fseeki64, fread (EINVAL / invalid parameter; size or count 0 returns 0)
 * note:  PROBE: _o_ stream aliases validate whence/offset like fseek/fread. Uses fopen/fclose/remove from the CRT as plain helpers.
 *        The _o_ aliases are undocumented; expectations mirror the documented behaviour of the underlying function.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

typedef int (__cdecl *fseek_t)(FILE *, long, int);
typedef int (__cdecl *fseeki64_t)(FILE *, long long, int);
typedef size_t (__cdecl *fread_t)(void *, size_t, size_t, FILE *);
typedef long (__cdecl *ftell_t)(FILE *);
static fseek_t p_fseek; static fseeki64_t p_fseeki64; static fread_t p_fread; static ftell_t p_ftell;
static volatile int bad_whence = 99; static volatile long long neg1 = -1; static volatile size_t zero = 0;
static const char fname[] = "tc_exp_neg_o_fseek_fread_bad_stream.tmp";

static FILE *mk(void)
{
    FILE *fp = fopen(fname, "wb");
    if (!fp) return NULL;
    fwrite("0123456789", 1, 10, fp); fclose(fp);
    return fopen(fname, "rb");
}
TC_CASE(fseek_bad_whence)
{
    FILE *fp; int r;
    if (!p_fseek) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    fp = mk(); CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; errno = 0; r = p_fseek(fp, 0, bad_whence);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); remove(fname);
}
TC_CASE(fseeki64_negative)
{
    FILE *fp; int r;
    if (!p_fseeki64) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    fp = mk(); CHECK(fp != NULL); if (!fp) return;
    errno = 0; r = p_fseeki64(fp, neg1, SEEK_SET);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);
    fclose(fp); remove(fname);
}
TC_CASE(fseek_before_start_keeps_position)
{
    FILE *fp; int r;
    if (!p_fseek || !p_ftell) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    fp = mk(); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(p_fseek(fp, 4, SEEK_SET), 0);
    errno = 0; r = p_fseek(fp, -5, SEEK_CUR);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);
    fclose(fp); remove(fname);
}
TC_CASE(fseek_before_start_keeps_position_probe)  /* isolated guess: failed fseek leaves position unchanged (not stated by C11) */
{
    FILE *fp; long pos;
    if (!p_fseek || !p_ftell) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    fp = mk(); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(p_fseek(fp, 4, SEEK_SET), 0);
    p_fseek(fp, -5, SEEK_CUR);
    pos = p_ftell(fp); CHECK_EQ_I(pos, 4);
    fclose(fp); remove(fname);
}
TC_CASE(fread_zero)
{
    FILE *fp; char b[8]; size_t r;
    if (!p_fread) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    fp = mk(); CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; errno = 0;
    r = p_fread(b, zero, 5, fp); CHECK_EQ_U(r, 0);
    r = p_fread(b, 5, zero, fp); CHECK_EQ_U(r, 0);
    CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    fclose(fp); remove(fname);
}
int main(void)
{
    tc_begin();
    p_fseek = (fseek_t)tc_crt_proc("_o_fseek"); p_fseeki64 = (fseeki64_t)tc_crt_proc("_o__fseeki64");
    p_fread = (fread_t)tc_crt_proc("_o_fread"); p_ftell = (ftell_t)tc_crt_proc("_o_ftell");
    TC_RUN(fseek_bad_whence); TC_RUN(fseeki64_negative);
    TC_RUN(fseek_before_start_keeps_position); TC_RUN(fseek_before_start_keeps_position_probe); TC_RUN(fread_zero);
    return tc_end();
}
