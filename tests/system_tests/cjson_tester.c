#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "cjson.h"


char* get_data(const char* const path) {
    int fd = open("/Users/jboulanger/Desktop/test.json", O_RDONLY);
    off_t len = lseek(fd, 0, SEEK_END);
    return mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);
}

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

    CJsonValue* value = cjson_read(data);
    if(value == NULL) {
        fprintf(stderr, "error: could not parse json\n");
        return 4;
    }

    cjson_print(value);
    return close(fd);
}
