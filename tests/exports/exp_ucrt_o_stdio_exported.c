/* TC
 * id:    exp_ucrt_o_stdio_exported
 * area:  exports
 * funcs: 
 * procs: _o_fopen _o_fopen_s _o_fclose _o_fread _o_fwrite _o_fgets _o_fputs _o_fseek _o_ftell _o_fflush _o_fgetc _o_fputc _o__close _o__read _o__write _o__lseek _o_remove _o_rename _o_tmpfile_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   Windows 10 ucrtbase.dll export table
 * note:  Undocumented _o_ aliases (API-set forwarding targets); names enumerated from the import library.
 */
#include "tc.h"

static void check_group(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) { tc_fail_expr(__FILE__, __LINE__, names[i]); }
}

static const char *const g_stream_aliases[] = { "_o_fopen", "_o_fopen_s", "_o_fclose", "_o_fread", "_o_fwrite", "_o_fgets", "_o_fputs", "_o_fseek", "_o_ftell", "_o_fflush", "_o_fgetc", "_o_fputc" };
TC_CASE(stream_aliases) { check_group(g_stream_aliases, (int)(sizeof g_stream_aliases / sizeof g_stream_aliases[0])); }

static const char *const g_lowio_aliases[] = { "_o__close", "_o__read", "_o__write", "_o__lseek" };
TC_CASE(lowio_aliases) { check_group(g_lowio_aliases, (int)(sizeof g_lowio_aliases / sizeof g_lowio_aliases[0])); }

static const char *const g_filesystem_aliases[] = { "_o_remove", "_o_rename", "_o_tmpfile_s" };
TC_CASE(filesystem_aliases) { check_group(g_filesystem_aliases, (int)(sizeof g_filesystem_aliases / sizeof g_filesystem_aliases[0])); }

int main(void)
{
    tc_begin();
    TC_RUN(stream_aliases);
    TC_RUN(lowio_aliases);
    TC_RUN(filesystem_aliases);
    return tc_end();
}
