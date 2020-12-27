add_library(cjson STATIC
            cjson_array.c
            cjson_assert.c
            cjson_object.c
            cjson_reader.c
            cjson_str.c
            cjson_stringstream.c
            cjson_utils.c
            cjson_value.c
            cjson_writer.c
            cjson.c)

target_include_directories(cjson PUBLIC include)
