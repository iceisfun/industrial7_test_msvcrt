/* TC
 * id:    dir_probe_makepath_slash_style
 * area:  dir
 * funcs: _makepath
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _makepath, _wmakepath
 * note:  Probes (0) a dir already ending in a forward slash gets no extra separator ("a/b/" + "f" + ".e" -> "a/b/f.e"; moved
 *        here from dir_makepath_basic because the docs do not spell out that / counts as a trailing slash), (1) which separator _makepath appends when dir does not end in a slash and contains only forward
 *        slashes (expected: always a backslash), (2) multi-letter drive string (expected: only the first character
 *        is used, followed by ':'). Neither is spelled out in the documentation.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_drive, *volatile v_dir, *volatile v_fname, *volatile v_ext;

TC_CASE(forward_slash_dir)
{
    char b[260]; v_drive = NULL; v_dir = "a/b"; v_fname = "f"; v_ext = NULL;
    _makepath(b, v_drive, v_dir, v_fname, v_ext);
    CHECK_STR(b, "a/b\\f");
}
TC_CASE(fwd_trailing_slash)
{
    char b[260]; v_drive = NULL; v_dir = "a/b/"; v_fname = "f"; v_ext = ".e";
    _makepath(b, v_drive, v_dir, v_fname, v_ext);
    CHECK_STR(b, "a/b/f.e");
}
TC_CASE(backslash_dir)
{
    char b[260]; v_drive = NULL; v_dir = "a\\b"; v_fname = "f"; v_ext = NULL;
    _makepath(b, v_drive, v_dir, v_fname, v_ext);
    CHECK_STR(b, "a\\b\\f");
}
TC_CASE(multi_letter_drive)
{
    char b[260]; v_drive = "CD"; v_dir = NULL; v_fname = "f"; v_ext = NULL;
    _makepath(b, v_drive, v_dir, v_fname, v_ext);
    CHECK_STR(b, "C:f");
}
int main(void) { tc_begin(); TC_RUN(fwd_trailing_slash); TC_RUN(forward_slash_dir); TC_RUN(backslash_dir); TC_RUN(multi_letter_drive); return tc_end(); }
