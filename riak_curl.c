#include <php.h>

#include <ext/json/php_json.h>

#include "ext/standard/php_smart_str.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include "riak_curl.h"
#include "riak_shared.h"


PHPAPI riakCurlRequestHeader* riak_curl_create_request_header() {
    size_t block_size;
    block_size = 2;
    
    riakCurlRequestHeader* request_header = malloc(sizeof(riakCurlRequestHeader));
    
    if (request_header != NULL) {
        request_header->num = 0;
        request_header->size = block_size;
        request_header->block_size = block_size;
        request_header->str = malloc(sizeof(char*)*block_size);
        
        if (request_header->str == NULL) {
            free(request_header);
            return NULL;    
        }
    }
    
    return request_header;
}

PHPAPI void riak_curl_delete_request_header(riakCurlRequestHeader* request_header) {
    char **iter;
    
    for (iter = riak_curl_add_request_header_start(request_header); iter != riak_curl_add_request_header_end(request_header); ++iter) {
        free(*iter);
    }
    
    free(request_header->str);
    free(request_header);
}

PHPAPI int riak_curl_add_request_header_str(riakCurlRequestHeader* request_header, char* str, int str_len) {
    size_t num = request_header->num;
    
    if (num >= request_header->size) {
        size_t new_size = request_header->size + request_header->block_size;
        void* new_request_header = realloc(request_header->str, sizeof(char*)*new_size); 
        
        if (new_request_header == NULL) {
            return FAILURE;
        } else {    
            request_header->size = new_size;
            request_header->str = (char**)new_request_header;
        }
    }
    

    request_header->str[num] = strndup(str, str_len);
    ++request_header->num;

    return SUCCESS;
}

PHPAPI char** riak_curl_add_request_header_start(riakCurlRequestHeader* request_header) {
    return request_header->str;
}

PHPAPI char** riak_curl_add_request_header_end(riakCurlRequestHeader* request_header) {
    return &request_header->str[request_header->num];
}




typedef struct {
    char *str;
    size_t len;
} riakCurlResponse;

PHPAPI void riak_curl_response_init(riakCurlResponse *s) {
    s->len = 0;
    s->str = emalloc(s->len + 1);
    s->str[0] = '\0';
}

PHPAPI size_t riak_curl_writefunc(void *ptr, size_t size, size_t nmemb, riakCurlResponse *s) {
    size_t new_len = s->len + size * nmemb;
    s->str = erealloc(s->str, new_len + 1);
    
    memcpy(s->str + s->len, ptr, size*nmemb);
    s->str[new_len] = '\0';
    s->len = new_len;
    
    return size * nmemb;
}


PHPAPI void riak_curl_data_to_json_str(zval *data, char **json_struct TSRMLS_DC) {
    smart_str buf = {0};

    php_json_encode(&buf, data, 0 TSRMLS_CC);
    
    *json_struct = strndup(buf.c, buf.len);
    php_printf("json encoded: |%s|\n", *json_struct);    
    
    smart_str_free(&buf);
}



PHPAPI int riak_curl_fetch_response(char *client_id, char *request_url, char **response_body, riakCurlRequestHeader *response_header TSRMLS_DC) {
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;
    riakCurlResponse response_body_content;   
    riakCurlResponse response_header_content;  
        
    char *client_id_header = NULL;
    
    char *response_header_copy = NULL;
    char *header_line = NULL;
    
    char *last;
    
    int result;
    
    
    if (asprintf(&client_id_header, "X-Riak-ClientId: %s", client_id) < 0) {
        RIAK_MALLOC_WARNING();
        result = FAILURE;
        goto cleanup;
    }
    
    curl = curl_easy_init();
    
    if (curl) {
        riak_curl_response_init(&response_body_content);
        
        if (response_header) {
            riak_curl_response_init(&response_header_content);
        }
        
        
        headers = curl_slist_append(headers, client_id_header);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_URL, request_url);        
        
        /* store response headers if needed */
        if (response_header) {
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, riak_curl_writefunc); 
            curl_easy_setopt(curl, CURLOPT_WRITEHEADER, &response_header_content);
        }

        /* store response body */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, riak_curl_writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body_content);
        

        res = curl_easy_perform(curl);
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (asprintf(response_body, "%s", response_body_content.str) < 0) {
            RIAK_MALLOC_WARNING();
            result = FAILURE;
            goto cleanup;
        }

        /* parse headers in array struct */
        if (response_header) {
            if (asprintf(&response_header_copy, "%s", response_header_content.str) < 0) {
                RIAK_MALLOC_WARNING();
                result = FAILURE;
                goto cleanup;
            }
            
            for (header_line = strtok_r(response_header_copy, "\r\n", &last); header_line; header_line = strtok_r(NULL, "\r\n", &last)) {                
                riak_curl_add_request_header_str(response_header, header_line, strlen(header_line));
            }
        }
        

        /* output some debug info */
        if (response_header) {
            php_printf("Response header: %s\n", response_header_content.str);
        }
        
        php_printf("JSON response: %s\n", response_body_content.str);
              
        
        efree(response_body_content.str);  
        
        if (response_header) {
            efree(response_header_content.str);
        }
        
        result = SUCCESS;
    } else {
        RIAK_CURL_WARNING();
        result = FAILURE;
        goto cleanup;
    }
    
    
    cleanup:
        
    if (response_header_copy) {
        free(response_header_copy);
    }
                
    if (client_id_header) {
        free(client_id_header);
    }
    
    return result;
}

PHPAPI int riak_curl_fetch_json_response(char *client_id, char *request_url, zval **json_response, riakCurlRequestHeader *response_headers TSRMLS_DC) {
    char *response = NULL;
    
    zval *tmp = NULL;
    
    int result;
    
    if (riak_curl_fetch_response(client_id, request_url, &response, response_headers TSRMLS_CC) == SUCCESS) {
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

PHPAPI int riak_curl_send_write_request(char *method, char *client_id, char *request_url, char *data, riakCurlRequestHeader* request_header TSRMLS_DC) {
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;
    char *client_id_header = NULL;
    
    int result;
    
    long http_code = 0;
    
    
    if (asprintf(&client_id_header, "X-Riak-ClientId: %s", client_id) < 0) {
        RIAK_MALLOC_WARNING();
        result = FAILURE;
        goto cleanup;
    }
    

    curl = curl_easy_init();
    
    if (curl) {
        headers = curl_slist_append(headers, client_id_header);

        if (request_header) {
            char** iter;
            
            for (iter = riak_curl_add_request_header_start(request_header); iter != riak_curl_add_request_header_end(request_header); ++iter) {
                php_printf("Additional header: %s\n", *iter);
            }
        }
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_URL, request_url);  
        
        if (strcmp(method, "PUT") == 0) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        }
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
   
        res = curl_easy_perform(curl);

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
      
        if (CURLE_OK != res || http_code != 204) {
            php_printf("Error: %s\n", curl_easy_strerror(res));
            result = FAILURE;
        } else {
            result = SUCCESS;
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
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

PHPAPI int riak_curl_send_post_json_request(char *client_id, char *request_url, zval *data, riakCurlRequestHeader* request_header TSRMLS_DC) {
    char *json_struct;
    
    riak_curl_data_to_json_str(data, &json_struct TSRMLS_CC);
    
    if (request_header) {
        riak_curl_add_request_header_str(request_header, RIAK_CURL_REQUESTHEADER_CONTENTTYPE_JSON, RIAK_CURL_REQUESTHEADER_CONTENTTYPE_JSON_LEN);
    }
    
    return riak_curl_send_write_request("POST", client_id, request_url, json_struct, request_header TSRMLS_CC);
}

PHPAPI int riak_curl_send_put_json_request(char *client_id, char *request_url, zval *data, riakCurlRequestHeader* request_header TSRMLS_DC) {
    char *json_struct;
    
    riak_curl_data_to_json_str(data, &json_struct TSRMLS_CC);
    
    if (request_header) {
        riak_curl_add_request_header_str(request_header, RIAK_CURL_REQUESTHEADER_CONTENTTYPE_JSON, RIAK_CURL_REQUESTHEADER_CONTENTTYPE_JSON_LEN);
    }
    
    return riak_curl_send_write_request("PUT", client_id, request_url, json_struct, request_header TSRMLS_CC);
}

PHPAPI int riak_curl_send_post_request(char *client_id, char *request_url, char *data, riakCurlRequestHeader* request_header TSRMLS_DC) {
    return riak_curl_send_write_request("POST", client_id, request_url, data, request_header TSRMLS_CC);
}

PHPAPI int riak_curl_send_put_request(char *client_id, char *request_url, char *data, riakCurlRequestHeader* request_header TSRMLS_DC) {
    return riak_curl_send_write_request("PUT", client_id, request_url, data, request_header TSRMLS_CC);
}













