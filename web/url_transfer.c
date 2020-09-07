#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "url_transfer.h"

Custom_buffer *urlt_transfer_data(const char *url) {
	static Custom_buffer custom_buffer; // There's no need for Custom_buffer *custom_buffer = malloc(sizeof *custom_buffer).
	custom_buffer.data = malloc(1); // Will grow as needed.
	if (NULL == custom_buffer.data) { // No memory available.
		fprintf(stderr, "\n%s\n", strerror(errno));
		return NULL;
	}
	custom_buffer.size = 0;
	curl_global_init(CURL_GLOBAL_ALL);
	CURL *easy_handle = curl_easy_init();
	if (NULL == easy_handle) {
		fprintf(stderr, "\nError: curl_easy_init returned NULL.\n");
		free(custom_buffer.data);
		return NULL;
	}
	curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1);
	curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(easy_handle, CURLOPT_URL, url);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, urlt_write_data);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &custom_buffer);
	CURLcode ret_code = curl_easy_perform(easy_handle);
	curl_easy_cleanup(easy_handle);
	curl_global_cleanup();
	if (CURLE_OK != ret_code) {
		fprintf(stderr, "\n%s\n", curl_easy_strerror(ret_code));
		free(custom_buffer.data);
		return NULL;
	}
	return &custom_buffer;
}

size_t urlt_write_data(void *curl_buffer, size_t elem_size, size_t elem_count, void *custom_buffer) {
	size_t curl_buffer_size = elem_size * elem_count;
	Custom_buffer *aux = (Custom_buffer *)custom_buffer;
	aux->data = realloc(aux->data, aux->size + curl_buffer_size + 1);
	if (NULL == aux->data) { // No memory available.
		fprintf(stderr, "\n%s\n", strerror(errno));
		return 1;
	}
	memcpy(&(aux->data[aux->size]), curl_buffer, curl_buffer_size);
	aux->size += curl_buffer_size;
	aux->data[aux->size] = 0;
	return curl_buffer_size;
}
