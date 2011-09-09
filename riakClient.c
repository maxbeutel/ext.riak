#include <php.h>

#include <ext/spl/php_spl.h>
#include <ext/json/php_json.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include "riak_shared.h"
#include "riakClient.h"
#include "riakBucket.h"


RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_empty, 0)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakClient_construct, 0)
	ZEND_ARG_INFO(0, host)
	ZEND_ARG_INFO(0, port)
	ZEND_ARG_INFO(0, prefix)
	ZEND_ARG_INFO(0, mapred_prefix)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakClient_setR, 0)
	ZEND_ARG_INFO(0, r)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakClient_setW, 0)
	ZEND_ARG_INFO(0, w)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakClient_setDW, 0)
	ZEND_ARG_INFO(0, dw)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakClient_bucket, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()





static function_entry riakClient_methods[] = {
    PHP_ME(riakClient, __construct, arginfo_riakClient_construct, ZEND_ACC_PUBLIC)

    PHP_ME(riakClient, getR, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakClient, setR, arginfo_riakClient_setR, ZEND_ACC_PUBLIC)

    PHP_ME(riakClient, getW, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakClient, setW, arginfo_riakClient_setW, ZEND_ACC_PUBLIC)

    PHP_ME(riakClient, getDW, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakClient, setDW, arginfo_riakClient_setDW, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakClient, getClientId, NULL, ZEND_ACC_PUBLIC)

    PHP_ME(riakClient, isAlive, NULL, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakClient, bucket, arginfo_riakClient_bucket, ZEND_ACC_PUBLIC)
    PHP_ME(riakClient, buckets, NULL, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakClient, add, NULL, ZEND_ACC_PUBLIC) /* TODO: add arginfo */
    
    PHP_ME(riakClient, search, NULL, ZEND_ACC_PUBLIC) /* TODO: add arginfo */
    
    PHP_ME(riakClient, link, NULL, ZEND_ACC_PUBLIC) /* TODO: add arginfo */
    
    PHP_ME(riakClient, map, NULL, ZEND_ACC_PUBLIC) /* TODO: add arginfo */
    PHP_ME(riakClient, reduce, NULL, ZEND_ACC_PUBLIC) /* TODO: add arginfo */
    
    {NULL, NULL, NULL}
};



void riak_init_riakClient(TSRMLS_D) {
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "riakClient", riakClient_methods);
    riak_ce_riakClient = zend_register_internal_class(&ce TSRMLS_CC);
    
    zend_declare_property_string(riak_ce_riakClient, RIAK_CLIENT_HOST, RIAK_CLIENT_HOST_LEN, "127.0.0.1", ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_long(riak_ce_riakClient, RIAK_CLIENT_PORT, RIAK_CLIENT_PORT_LEN, 8098, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_string(riak_ce_riakClient, RIAK_CLIENT_PREFIX, RIAK_CLIENT_PREFIX_LEN, "riak", ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_string(riak_ce_riakClient, RIAK_CLIENT_MAPRED_PREFIX, RIAK_CLIENT_MAPRED_PREFIX_LEN, "mapred", ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_string(riak_ce_riakClient, RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, "", ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_long(riak_ce_riakClient, RIAK_CLIENT_R, RIAK_CLIENT_R_LEN, 2, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_long(riak_ce_riakClient, RIAK_CLIENT_W, RIAK_CLIENT_W_LEN, 2, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_long(riak_ce_riakClient, RIAK_CLIENT_DW, RIAK_CLIENT_DW_LEN, 2, ZEND_ACC_PROTECTED TSRMLS_CC);
}

PHP_METHOD(riakClient, __construct) {
    char *host;
    int host_len;
    
    long port;
    
    char *prefix;
    int prefix_len;
    
    char *mapred_prefix;
    int mapred_prefix_len;
    
    char *object_hash;
    char *client_id;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|slss", &host, &host_len, &port, &prefix, &prefix_len, &mapred_prefix, &mapred_prefix_len) == FAILURE) {
        return;
    }
    
    if (host_len > 0) {
        zend_update_property_stringl(riak_ce_riakClient, getThis(), RIAK_CLIENT_HOST, RIAK_CLIENT_HOST_LEN, host, host_len TSRMLS_CC);
    }
   
    if (port > 0) {
        zend_update_property_long(riak_ce_riakClient, getThis(), RIAK_CLIENT_PORT, RIAK_CLIENT_PORT_LEN, port TSRMLS_CC);
    }
    
    if (prefix_len > 0) {
        zend_update_property_stringl(riak_ce_riakClient, getThis(), RIAK_CLIENT_PREFIX, RIAK_CLIENT_PREFIX_LEN, prefix, prefix_len TSRMLS_CC);
    }
    
    if (mapred_prefix_len > 0) {
        zend_update_property_stringl(riak_ce_riakClient, getThis(), RIAK_CLIENT_MAPRED_PREFIX, RIAK_CLIENT_MAPRED_PREFIX_LEN, mapred_prefix, mapred_prefix_len TSRMLS_CC);
    }
    
    /* TODO: put this in separate function */
    zval *object = getThis();
    
    object_hash = emalloc(33);
    php_spl_object_hash(object, object_hash TSRMLS_CC);
    
    asprintf(&client_id, "php_%s", object_hash);
    
    zend_update_property_stringl(riak_ce_riakClient, getThis(), RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, client_id, strlen(client_id) TSRMLS_CC);
    
    efree(object_hash);
    free(client_id);
}


PHP_METHOD(riakClient, getR) {
    zval *data;
    
    data = zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_R, RIAK_CLIENT_R_LEN, 0 TSRMLS_CC);

    *return_value = *data;

    zval_copy_ctor(return_value);
}

PHP_METHOD(riakClient, setR) {
    long r;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &r) == FAILURE) {
        return;
    }
    
    zval *object = getThis();
    
    zend_update_property_long(riak_ce_riakClient, getThis(), RIAK_CLIENT_R, RIAK_CLIENT_R_LEN, r TSRMLS_CC);
    
    RETURN_ZVAL(object, 1, 0); 
}


PHP_METHOD(riakClient, getW) {
    zval *data;

    data = zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_W, RIAK_CLIENT_W_LEN, 0 TSRMLS_CC);

    *return_value = *data;

    zval_copy_ctor(return_value);
}

PHP_METHOD(riakClient, setW) {
    long w;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &w) == FAILURE) {
        return;
    }
    
    zval *object = getThis();
    
    zend_update_property_long(riak_ce_riakClient, getThis(), RIAK_CLIENT_W, RIAK_CLIENT_W_LEN, w TSRMLS_CC);
    
    RETURN_ZVAL(object, 1, 0); 
}


PHP_METHOD(riakClient, getDW) {
    zval *data;

    data = zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_DW, RIAK_CLIENT_DW_LEN, 0 TSRMLS_CC);

    *return_value = *data;

    zval_copy_ctor(return_value);
}

PHP_METHOD(riakClient, setDW) {
    long dw;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &dw) == FAILURE) {
        return;
    }
    
    zval *object = getThis();
    
    zend_update_property_long(riak_ce_riakClient, getThis(), RIAK_CLIENT_DW, RIAK_CLIENT_DW_LEN, dw TSRMLS_CC);
    
    RETURN_ZVAL(object, 1, 0); 
}

PHP_METHOD(riakClient, getClientId) {
    zval *data;

    data = zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC);

    *return_value = *data;

    zval_copy_ctor(return_value);
}




struct riak_curl_response {
    char *response_body;
    size_t len;
};

void riak_curl_response_init(struct riak_curl_response *s) {
    s->len = 0;
    s->response_body = emalloc(s->len + 1);
    s->response_body[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct riak_curl_response *s) {
    size_t new_len = s->len + size * nmemb;
    s->response_body = erealloc(s->response_body, new_len + 1);
    
    memcpy(s->response_body + s->len, ptr, size*nmemb);
    s->response_body[new_len] = '\0';
    s->len = new_len;
    
    return size * nmemb;
}




PHP_METHOD(riakClient, isAlive) {
    /* TODO: put curl stuff in helper functions */
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;
    struct riak_curl_response response;
    
    char *status_ok = "OK";
    char *ping_url;
    char *client_id_header;
    
    char *host;
    char *client_id;
    long port;
    
    int comparision_res;
    
    curl = curl_easy_init();
    
    if (curl) {
        /* build ping request url */   
        host = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_HOST, RIAK_CLIENT_HOST_LEN, 0 TSRMLS_CC));
        port = Z_LVAL_P(zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_PORT, RIAK_CLIENT_PORT_LEN, 0 TSRMLS_CC));
        
        asprintf(&ping_url, "%s:%ld/ping", host, port);
        
        /* build client id header */
        client_id = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC));
        
        asprintf(&client_id_header, "X-Riak-ClientId: %s", client_id);
        
        /* exec request */
        riak_curl_response_init(&response);
        
        headers = curl_slist_append(headers, client_id_header);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_URL, ping_url);        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        res = curl_easy_perform(curl);
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        
        
        comparision_res = strcmp(status_ok, response.response_body);
        
        
        /* cleanup */
        efree(response.response_body);
        free(ping_url);
        free(client_id_header);

        if (comparision_res == 0) {
            RETURN_TRUE;
        } else {
            RETURN_FALSE;
        }   
    }
    
    zend_error(E_WARNING, "Could not initialize request");
}

PHP_METHOD(riakClient, bucket) {
    zval *name;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &name) == FAILURE) {
        return;
    }
    
    object_init_ex(return_value, riak_ce_riakBucket);
    CALL_METHOD2(riakBucket, __construct, return_value, return_value, getThis(), name);
}

PHP_METHOD(riakClient, buckets) {
    /* TODO: put curl stuff in helper functions */
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;
    struct riak_curl_response response;
    
    char *bucket_list_url;
    char *client_id_header;
    
    char *host;
    char *client_id;
    long port;
    char *prefix;
    
    zval *json;
    
    curl = curl_easy_init();
    
    if (curl) {
        /* build ping request url */   
        host = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_HOST, RIAK_CLIENT_HOST_LEN, 0 TSRMLS_CC));
        port = Z_LVAL_P(zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_PORT, RIAK_CLIENT_PORT_LEN, 0 TSRMLS_CC));
        prefix = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_PREFIX, RIAK_CLIENT_PREFIX_LEN, 0 TSRMLS_CC));
        
        asprintf(&bucket_list_url, "%s:%ld/%s?buckets=true", host, port, prefix);
        
        /* build client id header */
        client_id = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC));
        
        /* @TODO add error handling to all asprintf calls */
        asprintf(&client_id_header, "X-Riak-ClientId: %s", client_id);
        
        /* exec request */
        riak_curl_response_init(&response);
        
        headers = curl_slist_append(headers, client_id_header);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_URL, bucket_list_url);        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        res = curl_easy_perform(curl);
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        
        php_printf("RESPONSE: %s", response.response_body);
        
        
        if (response.len > 0) {
            MAKE_STD_ZVAL(json);
            array_init(return_value);
            
            php_json_decode(json, response.response_body, response.len, 1, 2 TSRMLS_CC);
            
            
            
            
            
            
HashTable *hindex = Z_ARRVAL_P(json);
HashPosition pointer;
zval **data;
 
for(zend_hash_internal_pointer_reset_ex(hindex, &pointer);
    zend_hash_get_current_data_ex(hindex, (void**)&data, &pointer) == SUCCESS;
    zend_hash_move_forward_ex(hindex, &pointer)) {
 
  char *key;
  uint key_len, key_type;
  ulong index;
 
  key_type = zend_hash_get_current_key_ex(hindex, &key, &key_len, &index, 0, &pointer);
 
  switch (key_type) {
  case HASH_KEY_IS_STRING:
    // associative array keys
    php_printf("key: %s\n", key);
    break;
  case HASH_KEY_IS_LONG:
    // numeric indexes
    php_printf("index: %d\n", index);
    break;
  default:
    php_printf("error\n");
  }
}
            
            
            
            
            
            
            
            
            
            
            
            
            efree(json);
        } else {
            array_init(return_value);
        }
        
        free(bucket_list_url);
        free(client_id_header);
        efree(response.response_body);
        
        return;
    }
    
    zend_error(E_WARNING, "Could not initialize request");
}

PHP_METHOD(riakClient, add) {

}

PHP_METHOD(riakClient, search) {

}

PHP_METHOD(riakClient, link) {

}

PHP_METHOD(riakClient, map) {

}

PHP_METHOD(riakClient, reduce) {

}


















