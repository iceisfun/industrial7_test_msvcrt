/* TC
 * id:    gdr_neg_stat32_missing
 * area:  dir
 * kind:  negative
 * funcs: _stat32 _stat32i64 _wstat32 _wstat32i64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _stat: ENOENT file name or path not found; returns -1
 * note:  not a parameter error: the invalid-parameter handler must not run. Empty-string path is covered elsewhere.
 */
#include "tc.h"
#include <errno.h>
#include <sys/stat.h>

static const char *volatile v_missing = "tc_gdr_no_such_file_xyz.tmp";
static const char *volatile v_noparent = "tc_gdr_no_such_dir_xyz\\sub\\file.tmp";
static const wchar_t *volatile v_wmissing = L"tc_gdr_no_such_file_xyz.tmp";
static const wchar_t *volatile v_wnoparent = L"tc_gdr_no_such_dir_xyz\\sub\\file.tmp";

#define BEGIN tc_iph_install(); tc_iph_count = 0; errno = 0
TC_CASE(stat32)       { struct _stat32 st;    BEGIN; CHECK_EQ_I(_stat32(v_missing, &st), -1); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(stat32i64)    { struct _stat32i64 st; BEGIN; CHECK_EQ_I(_stat32i64(v_missing, &st), -1); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(wstat32)      { struct _stat32 st;    BEGIN; CHECK_EQ_I(_wstat32(v_wmissing, &st), -1); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(wstat32i64)   { struct _stat32i64 st; BEGIN; CHECK_EQ_I(_wstat32i64(v_wmissing, &st), -1); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(missing_parent)
{
    struct _stat32 a; struct _stat32i64 b;
    BEGIN; CHECK_EQ_I(_stat32(v_noparent, &a), -1); CHECK_EQ_I(errno, ENOENT);
    errno = 0; CHECK_EQ_I(_stat32i64(v_noparent, &b), -1); CHECK_EQ_I(errno, ENOENT);
    errno = 0; CHECK_EQ_I(_wstat32(v_wnoparent, &a), -1); CHECK_EQ_I(errno, ENOENT);
    errno = 0; CHECK_EQ_I(_wstat32i64(v_wnoparent, &b), -1); CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(stat32); TC_RUN(stat32i64); TC_RUN(wstat32); TC_RUN(wstat32i64); TC_RUN(missing_parent); return tc_end(); }
