#ifndef IO_UTILS_H
#define IO_UTILS_H

#include "commons.h"

typedef struct {
	char* data;
	size_t size;
	size_t capacity;
} StringBuffer;

StringBuffer* strbuf_new(size_t initial_capacity);
void strbuf_delete(StringBuffer* buffer);

const char* strbuf_string(const StringBuffer* buffer);
size_t strbuf_size(const StringBuffer* buffer);
size_t strbuf_capacity(const StringBuffer* buffer);
bool strbuf_empty(const StringBuffer* buffer);

void strbuf_clear(StringBuffer* buffer);
void strbuf_ensure_capacity(StringBuffer* buffer, size_t capacity);

void strbuf_append(StringBuffer* buffer, const char* data, size_t data_length);
void strbuf_append_string(StringBuffer* buffer, const char* data);


StringBuffer* read_all_text_in_file(const char* filename);


#endif //IO_UTILS_H
