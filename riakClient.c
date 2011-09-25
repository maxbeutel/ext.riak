#include <php.h>

#include <ext/spl/php_spl.h>
#include <ext/json/php_json.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include "riak_shared.h"
#include "riak_curl.h"
#include "riakClient.h"
#include "riakBucket.h"


RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_empty, 0)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakClient_construct, 0, 0, 0)
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


int riak_client_assign_new_client_id(zval *object TSRMLS_DC) {
    int result; 
    
    char *object_hash;
    char *client_id;
    
    object_hash = emalloc(33);
    php_spl_object_hash(object, object_hash TSRMLS_CC);
    
    if (asprintf(&client_id, "php_%s", object_hash) > 0) {
        zend_update_property_stringl(riak_ce_riakClient, object, RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, client_id, strlen(client_id) TSRMLS_CC);
        free(client_id);
        
        result = SUCCESS;
    } else {
        RIAK_MALLOC_WARNING();
        result = FAILURE;
    }
    
    efree(object_hash);
    
    return result;
}

int riak_client_base_address(zval *object, int add_prefix, char **base_address TSRMLS_DC) {    
    int result;
    
    char *host;
    long port;
    char *prefix;  
    
    host = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, object, RIAK_CLIENT_HOST, RIAK_CLIENT_HOST_LEN, 0 TSRMLS_CC));
    port = Z_LVAL_P(zend_read_property(riak_ce_riakClient, object, RIAK_CLIENT_PORT, RIAK_CLIENT_PORT_LEN, 0 TSRMLS_CC));
    prefix = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, object, RIAK_CLIENT_PREFIX, RIAK_CLIENT_PREFIX_LEN, 0 TSRMLS_CC));
    
    result = SUCCESS;
    
    if (add_prefix) {
        if (asprintf(base_address, "%s:%ld/%s", host, port, prefix) < 0) {
            RIAK_MALLOC_WARNING();
            result = FAILURE;
        }
    } else {
        if (asprintf(base_address, "%s:%ld", host, port) < 0) {
            RIAK_MALLOC_WARNING();
            result = FAILURE;
        }
    }
        
    return result;
}




PHP_METHOD(riakClient, __construct) {
    char *host;
    int host_len;
    
    long port;
    
    char *prefix;
    int prefix_len;
    
    char *mapred_prefix;
    int mapred_prefix_len;
        
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
    
    riak_client_assign_new_client_id(getThis() TSRMLS_CC);
}

PHP_METHOD(riakClient, getR) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakClient, RIAK_CLIENT_R, RIAK_CLIENT_R_LEN);
}

PHP_METHOD(riakClient, setR) {
    long r;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &r) == FAILURE) {
        return;
    }
    
    zend_update_property_long(riak_ce_riakClient, getThis(), RIAK_CLIENT_R, RIAK_CLIENT_R_LEN, r TSRMLS_CC);
    
    RIAK_RETURN_SELF(); 
}


PHP_METHOD(riakClient, getW) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakClient, RIAK_CLIENT_W, RIAK_CLIENT_W_LEN);
}

PHP_METHOD(riakClient, setW) {
    long w;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &w) == FAILURE) {
        return;
    }
    
    zend_update_property_long(riak_ce_riakClient, getThis(), RIAK_CLIENT_W, RIAK_CLIENT_W_LEN, w TSRMLS_CC);
    
    RIAK_RETURN_SELF(); 
}


PHP_METHOD(riakClient, getDW) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakClient, RIAK_CLIENT_DW, RIAK_CLIENT_DW_LEN);
}

PHP_METHOD(riakClient, setDW) {
    long dw;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &dw) == FAILURE) {
        return;
    }
    
    zend_update_property_long(riak_ce_riakClient, getThis(), RIAK_CLIENT_DW, RIAK_CLIENT_DW_LEN, dw TSRMLS_CC);
    
    RIAK_RETURN_SELF(); 
}

PHP_METHOD(riakClient, getClientId) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakClient, RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN);
}

PHP_METHOD(riakClient, isAlive) {
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;
    riakCurlResponse response;
    
    char *status_ok = "OK";
    
    char *client_id;
    
    char *base_address = NULL;
    char *ping_url = NULL;
    char *client_id_header = NULL;
    
    int comparision_res;
        
    /* build ping url */
    if (riak_client_base_address(getThis(), 0, &base_address TSRMLS_CC) == FAILURE) {
        goto cleanup;
    }
    
    if (asprintf(&ping_url, "%s/ping", base_address) < 0) {
        RIAK_MALLOC_WARNING();
        goto cleanup;
    }
    
    /* build client id header */
    client_id = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC));
    
    if (asprintf(&client_id_header, "X-Riak-ClientId: %s", client_id) < 0) {
        RIAK_MALLOC_WARNING();
        goto cleanup;
    }
    
    curl = curl_easy_init();
    
    if (curl) {
        /* exec request */
        riak_curl_response_init(&response);
        
        headers = curl_slist_append(headers, client_id_header);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_URL, ping_url);        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, riak_curl_writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        res = curl_easy_perform(curl);
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        
        comparision_res = strcmp(status_ok, response.response_body);
        
        efree(response.response_body);  
    } else {
        RIAK_CURL_WARNING();
        goto cleanup;
    }
    
    
    cleanup:
    
    if (base_address) {
        free(base_address);
    }
    
    if (ping_url) {
        free(ping_url);
    }
    
    if (client_id_header) {
        free(client_id_header);
    }    
    
    
    if (comparision_res == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    } 
}

PHP_METHOD(riakClient, bucket) {
    zval *name;
    int name_len;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &name) == FAILURE) {
        return;
    }
    
    object_init_ex(return_value, riak_ce_riakBucket);
    CALL_METHOD2(riakBucket, __construct, return_value, return_value, getThis(), name);
}

PHP_METHOD(riakClient, buckets) {
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;
    riakCurlResponse response;
    
    char *base_address = NULL;
    char *bucket_list_url = NULL;
    char *client_id_header = NULL;
    
    char *client_id; 


    /* build buckets url */
    if (riak_client_base_address(getThis(), 1, &base_address TSRMLS_CC) == FAILURE) {
        goto cleanup;
    }

    if (asprintf(&bucket_list_url, "%s/?buckets=true", base_address) < 0) {
        RIAK_MALLOC_WARNING();
        goto cleanup;
    }
         
    /* build client id header */
    client_id = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, getThis(), RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC));
    
    if (asprintf(&client_id_header, "X-Riak-ClientId: %s", client_id) < 0) {
        RIAK_MALLOC_WARNING();
        goto cleanup;
    }
    
    curl = curl_easy_init();
    
    if (curl) {        
        /* exec request */
        riak_curl_response_init(&response);
        
        headers = curl_slist_append(headers, client_id_header);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_URL, bucket_list_url);        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, riak_curl_writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        res = curl_easy_perform(curl);
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
                
        array_init(return_value);
        
        if (response.len > 0) {
            /* decode json string */
            zval *buckets;
            MAKE_STD_ZVAL(buckets);
            php_json_decode(buckets, response.response_body, response.len, 1, 20 TSRMLS_CC);
            
            /* search "buckets" key */
            zval **bucket_names_array;
            
            HashTable *buckets_hash = NULL;
            buckets_hash = Z_ARRVAL_P(buckets);
 
            /* iterate over bucket names, create riakBucket instances */
            if (zend_hash_find(buckets_hash, "buckets", sizeof("buckets"), (void**) &bucket_names_array) == SUCCESS) {
                zval *arr, **data, *bucket_name;
                HashTable *arr_hash;
                HashPosition pointer;
    
                arr_hash = Z_ARRVAL_PP(bucket_names_array);
    
                for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(arr_hash, &pointer)) {
                    if (Z_TYPE_PP(data) == IS_STRING) {
                        zval *bucket_instance;
                        MAKE_STD_ZVAL(bucket_instance);
                        
                        bucket_name = *data;
                        
                        object_init_ex(bucket_instance, riak_ce_riakBucket);
                        CALL_METHOD2(riakBucket, __construct, bucket_instance, bucket_instance, getThis(), bucket_name);
                        
                        add_next_index_zval(return_value, bucket_instance);
                    }
                }
            }

            zval_ptr_dtor(&buckets);
        } 
        
        efree(response.response_body);
    } else {
        RIAK_CURL_WARNING();
        goto cleanup;
    }
             
    
    cleanup:
    
    if (base_address) {
        free(base_address);
    }
    
    if (bucket_list_url) {
        free(bucket_list_url);
    }
    
    if (client_id_header) {
        free(client_id_header);
    }
}

PHP_METHOD(riakClient, add) {
    zend_error(E_WARNING, "Map/Reduce functionality not yet implemented");
    RETURN_NULL();
}

PHP_METHOD(riakClient, search) {
    zend_error(E_WARNING, "Map/Reduce functionality not yet implemented");
    RETURN_NULL();
}

PHP_METHOD(riakClient, link) {
    zend_error(E_WARNING, "Map/Reduce functionality not yet implemented");
    RETURN_NULL();
}

PHP_METHOD(riakClient, map) {
    zend_error(E_WARNING, "Map/Reduce functionality not yet implemented");
    RETURN_NULL();
}

PHP_METHOD(riakClient, reduce) {
    zend_error(E_WARNING, "Map/Reduce functionality not yet implemented");
    RETURN_NULL();
}


















