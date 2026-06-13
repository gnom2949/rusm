#include "cross.h"

#ifdef RUSM_PLAT_WINDOWS
char* find_vs_linker() {
    char vs_base_path[512] = {0};
    char cmd_buffer[1024];
    
    const char* vswhere_path = "C:\\Program Files (x86)\\Microsoft Visual Studio\\Installer\\vswhere.exe";
    
    snprintf(cmd_buffer, sizeof(cmd_buffer), 
        "\"%s\" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath", vswhere_path);

    FILE* pipe = _popen(cmd_buffer, "r");
    if (!pipe) return NULL;
    
    if (fgets(vs_base_path, sizeof(vs_base_path), pipe) != NULL) {vs_base_path[strcspn(vs_base_path, "\r\n")] = 0;}
    _pclose(pipe);

    if (strlen(vs_base_path) == 0) return NULL; 

    char msvc_search_path[1024];
    snprintf(msvc_search_path, sizeof(msvc_search_path), "%s\\VC\\Tools\\MSVC\\*", vs_base_path);

    WIN32_FIND_DATAA find_data;
    HANDLE hFind = FindFirstFileA(msvc_search_path, &find_data);
    if (hFind == INVALID_HANDLE_VALUE) return NULL;

    char highest_version[256] = {0};
    do {
        if ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
            find_data.cFileName[0] != '.') {
            if (strcmp(find_data.cFileName, highest_version) > 0) strcpy(highest_version, find_data.cFileName);
        }
    } while (FindNextFileA(hFind, &find_data));
    FindClose(hFind);

    if (strlen(highest_version) == 0) return NULL;

    char* final_linker_path = (char*)malloc(2048);
    if (!final_linker_path) return NULL;

    snprintf(final_linker_path, 2048, 
        "%s\\VC\\Tools\\MSVC\\%s\\bin\\HostX64\\x64\\link.exe", 
        vs_base_path, highest_version);

    return final_linker_path;
}
#endif

static void get_system_temp_dir(char *out_path, size_t maxlen)
{
#ifdef RUSM_PLAT_WINDOWS
    GetTempPathA((DWORD)maxlen, out_path);
#else
    const char *tmp = getenv("TMPDIR");
    if (!tmp) tmp = getenv("TMP");
    if (!tmp) tmp = getenv("TEMP");
    if (!tmp) tmp = "/tmp";
    snprintf(out_path, maxlen, "%s/", tmp);
#endif
}

void get_temp_path(const char *input, char *output)
{
    char path_copy[256];
    strncpy(path_copy, input, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';
    char *filename = strrchr(path_copy, '/');

    if (filename) filename++;
    else filename = path_copy;

    char *dot = strrchr(filename, '.');
    if (dot) *dot = '\0';

    char temp_dir[256];
    get_system_temp_dir(temp_dir, sizeof(temp_dir));

    snprintf(output, 256, "%s%s.asm", temp_dir, filename);
}

static const char* get_file_extension(const char* filename) 
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename || *(dot - 1) == '/' || *(dot - 1) == '\\') return NULL; 
    return dot;
}

FileType check_file_type(const char *filename)
{
    const char *ext = get_file_extension(filename);

    if (!ext)
    {
        #ifdef RUSM_PLAT_WINDOWS
            return FILE_TYPE_UNKNOWN;
        #else
            return FILE_TYPE_EXECUTABLE;
        #endif
    }

    if (strcasecmp(ext, ".o") == 0 || strcasecmp(ext, ".obj") == 0) return FILE_TYPE_OBJECT;

    if (strcasecmp(ext, ".exe") == 0 || strcasecmp(ext, ".bin") == 0) return FILE_TYPE_EXECUTABLE;

    return FILE_TYPE_UNKNOWN;
}