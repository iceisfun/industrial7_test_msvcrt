/* TC
 * id:    gdr_neg_stat32_null_args
 * area:  dir
 * kind:  negative
 * funcs: _stat32 _stat32i64 _wstat32 _wstat32i64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _stat: if filename or buffer is NULL the invalid parameter handler is invoked; returns -1, errno EINVAL
 */
#include "tc.h"
#include <errno.h>
#include <sys/stat.h>

static const char *volatile v_path = ".";
static const wchar_t *volatile v_wpath = L".";
static const char *volatile v_npath = NULL;
static const wchar_t *volatile v_nwpath = NULL;
static struct _stat32 *volatile v_nb32 = NULL;
static struct _stat32i64 *volatile v_nb64 = NULL;

#define BEGIN tc_iph_install(); tc_iph_count = 0; errno = 0
#define EXPECT(r) CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1)

TC_CASE(stat32)
{
    struct _stat32 st;
    BEGIN; EXPECT(_stat32(v_npath, &st));
    BEGIN; EXPECT(_stat32(v_path, v_nb32));
}
TC_CASE(stat32i64)
{
    struct _stat32i64 st;
    BEGIN; EXPECT(_stat32i64(v_npath, &st));
    BEGIN; EXPECT(_stat32i64(v_path, v_nb64));
}
TC_CASE(wstat32)
{
    struct _stat32 st;
    BEGIN; EXPECT(_wstat32(v_nwpath, &st));
    BEGIN; EXPECT(_wstat32(v_wpath, v_nb32));
}
TC_CASE(wstat32i64)
{
    struct _stat32i64 st;
    BEGIN; EXPECT(_wstat32i64(v_nwpath, &st));
    BEGIN; EXPECT(_wstat32i64(v_wpath, v_nb64));
}
int main(void) { tc_begin(); TC_RUN(stat32); TC_RUN(stat32i64); TC_RUN(wstat32); TC_RUN(wstat32i64); return tc_end(); }
