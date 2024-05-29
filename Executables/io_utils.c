#include "io_utils.h"

#define STRBUF_MAX_CAPACITY_INCREMENT 65536
#define STRBUF_MIN_CAPACITY_INCREMENT 256

StringBuffer* strbuf_new(size_t initial_capacity)
{
	StringBuffer* buffer = malloc(sizeof(StringBuffer));
	buffer->size = 0;
	buffer->capacity = initial_capacity;
	if (buffer->capacity == 0)
		buffer->data = NULL;
	else
		buffer->data = malloc(sizeof(char) * buffer->capacity);
	return buffer;
}

void strbuf_delete(StringBuffer* buffer)
{
	if (buffer->data != NULL)
		free(buffer->data);
	free(buffer);
}

const char* strbuf_string(const StringBuffer* buffer) { return buffer->data; }
size_t strbuf_size(const StringBuffer* buffer) { return buffer->size; }
size_t strbuf_capacity(const StringBuffer* buffer) { return buffer->capacity; }
bool strbuf_empty(const StringBuffer* buffer) { return buffer->size == 0; }

void strbuf_clear(StringBuffer* buffer)
{
	buffer->size = 0;
}

void strbuf_ensure_capacity(StringBuffer* buffer, size_t capacity)
{
	if (capacity == 0)
	{
		if (buffer->data != NULL)
			free(buffer->data);
		buffer->data = NULL;
		buffer->capacity = 0;
	}
	else if(capacity > buffer->capacity)
	{
		char* old_data = buffer->data;
		size_t old_capacity = buffer->capacity;
		if (buffer->capacity < STRBUF_MIN_CAPACITY_INCREMENT)
			buffer->capacity = STRBUF_MIN_CAPACITY_INCREMENT * 2;
		else while (capacity > buffer->capacity)
			buffer->capacity += min(buffer->capacity, STRBUF_MAX_CAPACITY_INCREMENT);

		buffer->data = malloc(sizeof(char) * buffer->capacity);
		if (old_data != NULL)
		{
			if(old_capacity > 0)
				memcpy(buffer->data, old_data, old_capacity);
			free(old_data);
		}
	}
}

void strbuf_append(StringBuffer* buffer, const char* data, size_t data_length)
{
	if (data_length < 1)
		return;

	strbuf_ensure_capacity(buffer, buffer->size + data_length);
	memcpy(buffer->data + buffer->size, data, data_length);
	buffer->size += data_length;
}

void strbuf_append_string(StringBuffer* buffer, const char* data)
{
	strbuf_append(buffer, data, strlen(data));
}



StringBuffer* read_all_text_in_file(const char* filename)
{
	StringBuffer* str_buffer = strbuf_new(1024);

	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		print_error("Cannot open '%s' file.", filename);
		return str_buffer;
	}

	char buffer[1024];
	while (fgets(buffer, 1024, file) != NULL)
	{
		strbuf_append_string(str_buffer, buffer);
	}

	fclose(file);

	return str_buffer;
}
