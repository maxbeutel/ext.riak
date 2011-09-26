#ifndef RIAK_CURL_H
#define RIAK_CURL_H

typedef struct riak_curl_response {
    char *response_body;
    size_t len;
} riakCurlResponse;

void riak_curl_response_init(riakCurlResponse *s);

size_t riak_curl_writefunc(void *ptr, size_t size, size_t nmemb, riakCurlResponse *s);

int riak_curl_fetch_json_response(char *client_id, char *request_url, zval **json_response TSRMLS_DC);

int riak_curl_fetch_text_response(char *client_id, char *request_url, char **text_response TSRMLS_DC);

#endif
