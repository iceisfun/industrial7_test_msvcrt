/* TC
 * id:    exp_stdio_file_exported
 * area:  exports
 * funcs: 
 * procs: fopen fclose freopen tmpfile setvbuf fread fwrite fgets fputs fgetc fputc ungetc fflush fseek ftell rewind fgetpos fsetpos feof ferror clearerr remove rename perror puts putchar getchar
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs per-function Requirements (stdio.h); GetProcAddress only
 */
#include "tc.h"

static void present(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = not exported */
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = unexpectedly exported */
}

static const char *const g_open_close_group[] = { "fopen", "fclose", "freopen", "tmpfile", "setvbuf", NULL };
static const char *const g_read_write_group[] = { "fread", "fwrite", "fgets", "fputs", "fgetc", "fputc", "ungetc", "fflush", NULL };
static const char *const g_positioning_group[] = { "fseek", "ftell", "rewind", "fgetpos", "fsetpos", NULL };
static const char *const g_status_group[] = { "feof", "ferror", "clearerr", NULL };
static const char *const g_filesystem_group[] = { "remove", "rename", "perror", NULL };
static const char *const g_stdout_group[] = { "puts", "putchar", "getchar", NULL };

TC_CASE(open_close_group) { present(g_open_close_group); }
TC_CASE(read_write_group) { present(g_read_write_group); }
TC_CASE(positioning_group) { present(g_positioning_group); }
TC_CASE(status_group) { present(g_status_group); }
TC_CASE(filesystem_group) { present(g_filesystem_group); }
TC_CASE(stdout_group) { present(g_stdout_group); }

int main(void)
{
    tc_begin();
    TC_RUN(open_close_group);
    TC_RUN(read_write_group);
    TC_RUN(positioning_group);
    TC_RUN(status_group);
    TC_RUN(filesystem_group);
    TC_RUN(stdout_group);
    return tc_end();
}
