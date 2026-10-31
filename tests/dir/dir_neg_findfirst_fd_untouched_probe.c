/* TC
 * id:    dir_neg_findfirst_fd_untouched_probe
 * area:  dir
 * kind:  negative
 * funcs: _findclose
 * procs: _findfirst64i32 _findfirst64 _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _findfirst (silent on the result struct after failure)
 * note:  Probes that the caller's result struct keeps its prior contents (name "SENT", size 0x1234) after a failed _findfirst
 *        (no match, and NULL filespec with the invalid parameter handler installed).
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <string.h>

typedef intptr_t (__cdecl *ff3264_t)(const char *, struct _finddata64i32_t *);
typedef intptr_t (__cdecl *ff64_t)(const char *, struct __finddata64_t *);
static const char *volatile v_nomatch = "tc_dir_neg_fdu_nomatch.zz";
static const char *volatile v_null;

TC_CASE(nomatch_64i32)
{
    struct _finddata64i32_t fd; intptr_t h; ff3264_t f = (ff3264_t)tc_crt_proc("_findfirst64i32");
    if (!f) TC_SKIP("not exported");
    memset(&fd, 0, sizeof fd); strcpy(fd.name, "SENT"); fd.size = 0x1234;
    h = f(v_nomatch, &fd);
    CHECK_EQ_I(h, -1);
    CHECK_STR(fd.name, "SENT"); CHECK_EQ_I(fd.size, 0x1234);
    if (h != -1) _findclose(h);
}
TC_CASE(nomatch_64)
{
    struct __finddata64_t fd; intptr_t h; ff64_t f = (ff64_t)tc_crt_proc("_findfirst64");
    if (!f) TC_SKIP("not exported");
    memset(&fd, 0, sizeof fd); strcpy(fd.name, "SENT"); fd.size = 0x1234;
    h = f(v_nomatch, &fd);
    CHECK_EQ_I(h, -1);
    CHECK_STR(fd.name, "SENT"); CHECK_EQ_I(fd.size, 0x1234);
    if (h != -1) _findclose(h);
}
TC_CASE(null_spec)
{
    struct _finddata64i32_t fd; intptr_t h; ff3264_t f = (ff3264_t)tc_crt_proc("_findfirst64i32");
    if (!f) TC_SKIP("not exported");
    memset(&fd, 0, sizeof fd); strcpy(fd.name, "SENT"); fd.size = 0x1234;
    tc_iph_install(); tc_iph_count = 0;
    h = f(v_null, &fd);
    CHECK_EQ_I(h, -1);
    CHECK_STR(fd.name, "SENT"); CHECK_EQ_I(fd.size, 0x1234);
    if (h != -1) _findclose(h);
}

int main(void)
{
    tc_begin();
    TC_RUN(nomatch_64i32); TC_RUN(nomatch_64); TC_RUN(null_spec);
    return tc_end();
}
