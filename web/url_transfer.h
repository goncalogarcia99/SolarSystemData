#ifndef URL_TRANSFER_H
#define URL_TRANSFER_H

#include <curl/curl.h>

typedef struct Custom_buffer {
	char *data;
	size_t size;
} Custom_buffer;

Custom_buffer *urlt_transfer_data(const char *url);
size_t urlt_write_data(void *curl_buffer, size_t elem_size, size_t elem_count, void *custom_buffer);

#endif
