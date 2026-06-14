#include "cross.h"

#ifdef RUSM_PLAT_WINDOWS
char pathtonasm[MAX_PATH] = {0};
char *linker = NULL;
char pathtolinker[MAX_PATH] = {0};
#else
const char *pathtonasm = "/usr/bin/nasm";
const char *linker = "/usr/bin/ld";
#endif

#ifdef RUSM_PLAT_WINDOWS
void init_paths(void)
{
    char exe_path[MAX_PATH];

    DWORD len = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    if (len > 0)
    {
        char *last_slash = strrchr(exe_path, '\\');
        if (last_slash) *last_slash = '\0';

        snprintf(pathtonasm, sizeof(pathtonasm), "%s\\nasm.exe", exe_path);
        snprintf(pathtolinker, sizeof(pathtolinker), "%s\\lld-link.exe", exe_path);
        linker = pathtolinker;
    }

    if (pathtonasm[0] == '\0')
    {
        strncpy(pathtonasm, "nasm.exe", sizeof(pathtonasm) - 1);
        linker = "lld-link.exe";
    }
}
#endif

static void get_system_temp_dir(char *out_path, size_t maxlen)
{
#ifdef RUSM_PLAT_WINDOWS
    DWORD len = GetTempPathA((DWORD)maxlen, out_path);
    if (len == 0 || len > maxlen)
    {
        CreateDirectoryA("C:\\temp", NULL);
        snprintf(out_path, maxlen, "C:\\temp");
    }
#else
    const char *tmp = getenv("TMPDIR");
    if (!tmp) tmp = getenv("TMP");
    if (!tmp) tmp = getenv("TEMP");
    if (!tmp) tmp = "/tmp";
    snprintf(out_path, maxlen, "%s", tmp);
    size_t len = strlen(out_path);
    if (len > 0 && out_path[len-1] != '/') {
        strncat(out_path, "/", maxlen - len - 1);
    }
#endif
}

void get_temp_path(const char *input, char *output)
{
    char path_copy[256];
    strncpy(path_copy, input, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';
    
    char *filename = strrchr(path_copy, '/');
    char *filename_win = strrchr(path_copy, '\\');
    if (filename_win > filename) filename = filename_win;
    
    if (filename) filename++;
    else filename = path_copy;

    char *dot = strrchr(filename, '.');
    if (dot) *dot = '\0';

    char temp_dir[MAX_PATH];
    get_system_temp_dir(temp_dir, sizeof(temp_dir));
    
    size_t len = strlen(temp_dir);
    if (len > 0 && (temp_dir[len-1] == '/' || temp_dir[len-1] == '\\')) {
        temp_dir[len-1] = '\0';
    }

 
#ifdef RUSM_PLAT_WINDOWS
    snprintf(output, 256, "%s\\%s.asm", temp_dir, filename);
#else
    snprintf(output, 256, "%s/%s.asm", temp_dir, filename);
#endif
}

static const char* get_file_extension(const char* filename) 
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return NULL;
    
    const char *last_slash = strrchr(filename, '/');
    const char *last_backslash = strrchr(filename, '\\');
    const char *last_sep = (last_backslash > last_slash) ? last_backslash : last_slash;
    
    if (last_sep && dot < last_sep) return NULL;
    
    return dot;
}

FileType check_file_type(const char *filename)
{
    const char *ext = get_file_extension(filename);

    if (!ext) {
#ifdef RUSM_PLAT_WINDOWS
        return FILE_TYPE_UNKNOWN;
#else
        return FILE_TYPE_EXECUTABLE;
#endif
    }

    if (strcasecmp(ext, ".o") == 0 || strcasecmp(ext, ".obj") == 0) {
        return FILE_TYPE_OBJECT;
    }

    if (strcasecmp(ext, ".exe") == 0 || strcasecmp(ext, ".bin") == 0) {
        return FILE_TYPE_EXECUTABLE;
    }

    return FILE_TYPE_UNKNOWN;
}