#include <php.h>

#include <ext/json/php_json.h>

#include "ext/standard/php_smart_str.h"

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include "riak_curl.h"
#include "riak_shared.h"


PHPAPI void riak_curl_response_init(riakCurlResponse *s) {
    s->len = 0;
    s->response_body = emalloc(s->len + 1);
    s->response_body[0] = '\0';
}

PHPAPI size_t riak_curl_writefunc(void *ptr, size_t size, size_t nmemb, riakCurlResponse *s) {
    size_t new_len = s->len + size * nmemb;
    s->response_body = erealloc(s->response_body, new_len + 1);
    
    memcpy(s->response_body + s->len, ptr, size*nmemb);
    s->response_body[new_len] = '\0';
    s->len = new_len;
    
    return size * nmemb;
}

PHPAPI int riak_curl_fetch_response(char *client_id, char *request_url, char **response_body TSRMLS_DC) {
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;
    riakCurlResponse response;   
    
    char *client_id_header = NULL;
      
    int result;
    
    
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
        curl_easy_setopt(curl, CURLOPT_URL, request_url);        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, riak_curl_writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        res = curl_easy_perform(curl);
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        
        asprintf(response_body, "%s", response.response_body);

        php_printf("JSON response: %s\n", response.response_body);
        
        efree(response.response_body);  
        
        result = SUCCESS;
    } else {
        RIAK_CURL_WARNING();
        result = FAILURE;
        goto cleanup;
    }
    
    
    cleanup:
        
    if (client_id_header) {
        free(client_id_header);
    }
    
    return result;
}

PHPAPI int riak_curl_fetch_text_response(char *client_id, char *request_url, char **text_response TSRMLS_DC) {
    return riak_curl_fetch_response(client_id, request_url, text_response TSRMLS_CC);
}

PHPAPI int riak_curl_fetch_json_response(char *client_id, char *request_url, zval **json_response TSRMLS_DC) {
    char *response = NULL;
    
    zval *tmp = NULL;
    
    int result;
    
    if (riak_curl_fetch_response(client_id, request_url, &response TSRMLS_CC) == SUCCESS) {
        tmp = *json_response;
        
        php_json_decode(tmp, response, strlen(response), 1, 20 TSRMLS_CC);
        
        result = SUCCESS;
    } else {
        result = FAILURE;
    }
    
    
    if (response) {
        free(response);
    }
    
    return result;
}

PHPAPI int riak_curl_send_put_json_request(char *client_id, char *request_url, zval *data TSRMLS_DC) {
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;

    char *client_id_header = NULL;
    
    smart_str buf = {0};
    char *json_struct;
    
    int result;
    
    long http_code = 0;
    
    
    if (asprintf(&client_id_header, "X-Riak-ClientId: %s", client_id) < 0) {
        RIAK_MALLOC_WARNING();
        result = FAILURE;
        goto cleanup;
    }
    
    php_json_encode(&buf, data, 0 TSRMLS_CC);
    
    json_struct = strndup(buf.c, buf.len);
    php_printf("json encoded: |%s|\n", json_struct);

    curl = curl_easy_init();
    
    if (curl) {
        headers = curl_slist_append(headers, client_id_header);
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_URL, request_url);  
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_struct);
   
        res = curl_easy_perform(curl);

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
      
        if (CURLE_OK != res || http_code != 204) {
            php_printf("Error: %s\n", curl_easy_strerror(res));
            result = FAILURE;
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        
        result = SUCCESS;
    } else {
        RIAK_CURL_WARNING();
        result = FAILURE;
        goto cleanup;
    }

    
    cleanup:
        
    if (client_id_header) {
        free(client_id_header);
    }    
        
    smart_str_free(&buf);
    
    return result;
}












