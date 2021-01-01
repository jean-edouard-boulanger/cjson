#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "cjson.h"


int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "usage: %s <json file path>\n", argv[1]);
        return 1;
    }
    
    const char* const path = argv[1];
    if(0 != access(path, F_OK)) {
        fprintf(stderr, "error: file '%s' does not exist\n", path);
        return 2;
    }

    const int fd = open(path, O_RDONLY);
    if(fd < 0) {
        fprintf(stderr, "error: could not open '%s'\n", path);
        return 3;
    }

    const off_t file_len = lseek(fd, 0, SEEK_END);
    char* data = mmap(0, file_len, PROT_READ, MAP_PRIVATE, fd, 0);

    CJsonAllocator* allocator = cjson_linear_allocator_new(16 * 1024 * 1024);

    CJsonValue* value = NULL;
    {
        clock_t t = clock();
        value = cjson_read(data, allocator);
        t = clock() - t;
        if(value == NULL) {
            fprintf(stderr, "error: could not parse json\n");
            return 4;
        }
        const double time_taken = ((double)t) / CLOCKS_PER_SEC;
        printf("parse_time=%fs\n", time_taken);
    }

    {
        clock_t t = clock();
        char* result = cjson_to_str(value, allocator);
        t = clock() - t;
        if(result == NULL) {
            fprintf(stderr, "error: could not format json\n");
            return 4;
        }
        const double time_taken = ((double)t) / CLOCKS_PER_SEC;
        printf("format_time=%fs\n", time_taken);
        cjson_dealloc(allocator, result);
    }

    {
        clock_t t = clock();
        cjson_value_free(value);
        t = clock() - t;
        const double time_taken = ((double)t) / CLOCKS_PER_SEC;
        printf("cleanup_time=%fs\n", time_taken);
    }

    cjson_linear_allocator_free(allocator);
    return close(fd);
}
