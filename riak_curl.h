#ifndef RIAK_CURL_H
#define RIAK_CURL_H

typedef struct riak_curl_response {
    char *response_body;
    size_t len;
} riakCurlResponse;

void riak_curl_response_init(riakCurlResponse *s);

size_t riak_curl_writefunc(void *ptr, size_t size, size_t nmemb, riakCurlResponse *s);

#endif
