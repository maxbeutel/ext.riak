#include <php.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include "riak_curl.h"

void riak_curl_response_init(riakCurlResponse *s) {
    s->len = 0;
    s->response_body = emalloc(s->len + 1);
    s->response_body[0] = '\0';
}

size_t riak_curl_writefunc(void *ptr, size_t size, size_t nmemb, riakCurlResponse *s) {
    size_t new_len = s->len + size * nmemb;
    s->response_body = erealloc(s->response_body, new_len + 1);
    
    memcpy(s->response_body + s->len, ptr, size*nmemb);
    s->response_body[new_len] = '\0';
    s->len = new_len;
    
    return size * nmemb;
}



