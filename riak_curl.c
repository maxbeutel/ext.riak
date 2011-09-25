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



/* TODO make exec curl request function */
/*
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;
    riakCurlResponse response;    
    
    char *base_address = NULL;
    char *bucket_properties_url = NULL;
    
    char *client_id = NULL;
    
    int result;
    
    if (riak_client_base_address(client_instance, 1, &base_address TSRMLS_CC) == FAILURE) {
        result = FAILURE;
        goto cleanup;
    }
    
    if (asprintf(bucket_properties_url, "%s/?props=true&keys=false") < 0) {
        RIAK_MALLOC_WARNING();
        result = FAILURE;
        goto cleanup;
    }


    client_id = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, client_instance, RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC));
    
    if (asprintf(&client_id_header, "X-Riak-ClientId: %s", client_id) < 0) {
        RIAK_MALLOC_WARNING();
        result = FAILURE;
        goto cleanup;
    }
    
    curl = curl_easy_init();
    
    if (curl) {
        riak_curl_response_init(&response);
        
        headers = curl_slist_append(headers, client_id_header);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_URL, bucket_properties_url);        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, riak_curl_writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        res = curl_easy_perform(curl);
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        
        php_printf("Bucket properties: %s\n", response.response_body);
        

        efree(response.response_body);  
    } else {
        RIAK_CURL_WARNING();
        result = FAILURE;
        goto cleanup;
    }
 */
