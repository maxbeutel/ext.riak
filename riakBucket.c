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
#include "riakObject.h"


RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_empty, 0)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakBucket_construct, 0)
    ZEND_ARG_OBJ_INFO(0, client, riakClient, 0)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakBucket_setR, 0)
	ZEND_ARG_INFO(0, r)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakBucket_setW, 0)
	ZEND_ARG_INFO(0, w)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakBucket_setDW, 0)
	ZEND_ARG_INFO(0, dw)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakBucket_newObject, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, data)
    ZEND_ARG_INFO(0, contentType)	
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakBucket_newBinary, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, data)	
	ZEND_ARG_INFO(0, contentType)		
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakBucket_get, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, r)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakBucket_getBinary, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, r)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakBucket_setNVal, 0)
	ZEND_ARG_INFO(0, nval)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakBucket_setAllowMultiples, 0)
	ZEND_ARG_INFO(0, nval)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakBucket_setProperty, 0)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)	
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakBucket_getProperty, 0)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakBucket_setProperties, 0)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()


static function_entry riakBucket_methods[] = {
    PHP_ME(riakBucket, __construct, arginfo_riakBucket_construct, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, getName, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, getR, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, setR, arginfo_riakBucket_setR, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, getW, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, setW, arginfo_riakBucket_setW, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, getDW, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, setDW, arginfo_riakBucket_setDW, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, newObject, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, newBinary, arginfo_riakBucket_newBinary, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, get, arginfo_riakBucket_get, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, getBinary, arginfo_riakBucket_getBinary, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, setNVal, arginfo_riakBucket_setNVal, ZEND_ACC_PUBLIC)    
    PHP_ME(riakBucket, getNVal, NULL, ZEND_ACC_PUBLIC)    
    PHP_ME(riakBucket, setAllowMultiples, arginfo_riakBucket_setAllowMultiples, ZEND_ACC_PUBLIC)    
    PHP_ME(riakBucket, getAllowMultiples, NULL, ZEND_ACC_PUBLIC)    
    PHP_ME(riakBucket, setProperty, arginfo_riakBucket_setProperty, ZEND_ACC_PUBLIC)    
    PHP_ME(riakBucket, getProperty, arginfo_riakBucket_getProperty, ZEND_ACC_PUBLIC)    
    PHP_ME(riakBucket, setProperties, arginfo_riakBucket_setProperties, ZEND_ACC_PUBLIC)   
    PHP_ME(riakBucket, getProperties, NULL, ZEND_ACC_PUBLIC)       
    PHP_ME(riakBucket, getKeys, NULL, ZEND_ACC_PUBLIC)     
    
    {NULL, NULL, NULL}
};


void riak_init_riakBucket(TSRMLS_D) {
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "riakBucket", riakBucket_methods);
    riak_ce_riakBucket = zend_register_internal_class(&ce TSRMLS_CC);
    
    zend_declare_property_null(riak_ce_riakBucket, RIAK_BUCKET_CLIENT, RIAK_BUCKET_CLIENT_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakBucket, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakBucket, RIAK_CLIENT_R, RIAK_CLIENT_R_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakBucket, RIAK_CLIENT_W, RIAK_CLIENT_W_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakBucket, RIAK_CLIENT_DW, RIAK_CLIENT_DW_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
}

PHP_METHOD(riakBucket, __construct) {    
    zval *client;
    
    char *name;
    int name_len;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "os", &client, &name, &name_len) == FAILURE) {
        return;
    }
    
    zend_update_property(riak_ce_riakBucket, getThis(), RIAK_BUCKET_CLIENT, RIAK_BUCKET_CLIENT_LEN, client TSRMLS_CC);
    zend_update_property_stringl(riak_ce_riakBucket, getThis(), RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, name, name_len TSRMLS_CC);
}

PHP_METHOD(riakBucket, getName) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakBucket, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN);
}

PHP_METHOD(riakBucket, getR) {
    zval *data, *client_instance;
    
    data = zend_read_property(riak_ce_riakBucket, getThis(), RIAK_CLIENT_R, RIAK_CLIENT_R_LEN, 0 TSRMLS_CC);

    if (Z_TYPE_P(data) == IS_NULL) {
        client_instance = zend_read_property(riak_ce_riakBucket, getThis(), RIAK_BUCKET_CLIENT, RIAK_BUCKET_CLIENT_LEN, 0 TSRMLS_CC);
        
        CALL_METHOD(riakClient, getR, return_value, client_instance);
    } else {
        *return_value = *data;
    
        zval_copy_ctor(return_value); 
    }
}

PHP_METHOD(riakBucket, setR) {
    long r;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &r) == FAILURE) {
        return;
    }
    
    zend_update_property_long(riak_ce_riakBucket, getThis(), RIAK_CLIENT_R, RIAK_CLIENT_R_LEN, r TSRMLS_CC);
    
    RIAK_RETURN_SELF();
}

PHP_METHOD(riakBucket, getW) {
    zval *data, *client_instance;
    
    data = zend_read_property(riak_ce_riakBucket, getThis(), RIAK_CLIENT_W, RIAK_CLIENT_W_LEN, 0 TSRMLS_CC);

    if (Z_TYPE_P(data) == IS_NULL) {
        client_instance = zend_read_property(riak_ce_riakBucket, getThis(), RIAK_BUCKET_CLIENT, RIAK_BUCKET_CLIENT_LEN, 0 TSRMLS_CC);
        
        CALL_METHOD(riakClient, getW, return_value, client_instance);
    } else {
        *return_value = *data;
    
        zval_copy_ctor(return_value); 
    }
}

PHP_METHOD(riakBucket, setW) {  
    long w;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &w) == FAILURE) {
        return;
    }
    
    zend_update_property_long(riak_ce_riakBucket, getThis(), RIAK_CLIENT_W, RIAK_CLIENT_W_LEN, w TSRMLS_CC);
    
    RIAK_RETURN_SELF();    
}

PHP_METHOD(riakBucket, getDW) {
    zval *data, *client_instance;
    
    data = zend_read_property(riak_ce_riakBucket, getThis(), RIAK_CLIENT_DW, RIAK_CLIENT_DW_LEN, 0 TSRMLS_CC);

    if (Z_TYPE_P(data) == IS_NULL) {
        client_instance = zend_read_property(riak_ce_riakBucket, getThis(), RIAK_BUCKET_CLIENT, RIAK_BUCKET_CLIENT_LEN, 0 TSRMLS_CC);
        
        CALL_METHOD(riakClient, getDW, return_value, client_instance);
    } else {
        *return_value = *data;
    
        zval_copy_ctor(return_value); 
    }
}

PHP_METHOD(riakBucket, setDW) {
    long dw;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &dw) == FAILURE) {
        return;
    }
    
    zend_update_property_long(riak_ce_riakBucket, getThis(), RIAK_CLIENT_DW, RIAK_CLIENT_DW_LEN, dw TSRMLS_CC);
    
    RIAK_RETURN_SELF(); 
}

PHP_METHOD(riakBucket, newObject) {
    zval *key;
    zval *data, *data_arg;
    zval *content_type;
    
    zval *client_instance;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|zz", &key, &data, &content_type) == FAILURE) {
        return;
    }
        
    /* set up object instance and return it */    
    client_instance = zend_read_property(riak_ce_riakBucket, getThis(), RIAK_BUCKET_CLIENT, RIAK_BUCKET_CLIENT_LEN, 0 TSRMLS_CC);
    
    object_init_ex(return_value, riak_ce_riakObject);
    CALL_METHOD3(riakObject, __construct, return_value, return_value, client_instance, getThis(), &Z_STRVAL_P(key));
    
    if (Z_TYPE_P(content_type) == IS_STRING) {
        php_printf("using user supplied ctype %s\n", Z_STRVAL_P(content_type));
        
        CALL_METHOD1(riakObject, setContentType, return_value, return_value, &Z_STRVAL_P(content_type)); 
    } else {
        MAKE_STD_ZVAL(content_type);
        ZVAL_STRING(content_type, "text/json", 1);
        
        php_printf("using default ctype text/json\n");
        
        CALL_METHOD1(riakObject, setContentType, return_value, return_value, &Z_STRVAL_P(content_type));
        
        zval_ptr_dtor(&content_type);
    }
    
    if (Z_TYPE_P(data) != IS_NULL) {
        php_printf("setting data\n");
        
        CALL_METHOD1(riakObject, setData, return_value, return_value, data);
    }

    /* dont encode data here! */
    /*
    if (use_json_encoding) {
        smart_str buf = {0};
        
        php_json_encode(&buf, data, 0 TSRMLS_CC);
        
        MAKE_STD_ZVAL(json_data);
        ZVAL_STRINGL(json_data, buf.c, buf.len, 1);
        
        smart_str_free(&buf);
        
        CALL_METHOD1(riakObject, setData, NULL, return_value, &Z_STRVAL_P(json_data));
    } else {
        CALL_METHOD1(riakObject, setData, NULL, return_value, &Z_STRVAL_P(data));
    }
    */
}

PHP_METHOD(riakBucket, newBinary) {
    zval *key;
    zval *data;
    zval *content_type;
    
    zval *json_data, *client_instance;
    
    int use_json_encoding = 0;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|zz", &key, &data, &content_type) == FAILURE) {
        return;
    }
    
    CALL_METHOD3(riakBucket, newObject, return_value, getThis(), key, data, content_type);
}

PHP_METHOD(riakBucket, get) {
}

PHP_METHOD(riakBucket, getBinary) {
}

PHP_METHOD(riakBucket, getNVal) {
}

PHP_METHOD(riakBucket, setNVal) {
}

PHP_METHOD(riakBucket, setAllowMultiples) {
}

PHP_METHOD(riakBucket, getAllowMultiples) {
}

PHP_METHOD(riakBucket, getProperty) {
}

PHP_METHOD(riakBucket, setProperty) {
}

PHP_METHOD(riakBucket, setProperties) {
}

PHP_METHOD(riakBucket, getProperties) {
}

PHP_METHOD(riakBucket, getKeys) {
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;
    riakCurlResponse response;
    
    char *base_address;
    char *bucket_keys_url;
    char *client_id_header;
    
    zval *client_id;
    zval *client_instance;
    zval *bucket_name;
    
    MAKE_STD_ZVAL(client_id);
    
    
    client_instance = zend_read_property(riak_ce_riakBucket, getThis(), RIAK_BUCKET_CLIENT, RIAK_BUCKET_CLIENT_LEN, 0 TSRMLS_CC);
    bucket_name = zend_read_property(riak_ce_riakBucket, getThis(), RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, 0 TSRMLS_CC);
    
    /* build keys url */
    if (riak_client_base_address(client_instance, 1, &base_address TSRMLS_CC) == FAILURE) {
        RIAK_MALLOC_WARNING();
        goto cleanup;
    }

    if (asprintf(&bucket_keys_url, "%s/%s?keys=true&props=false", base_address, Z_STRVAL_P(bucket_name)) < 0) {
        RIAK_MALLOC_WARNING();
        goto cleanup;
    }
        
    /* build client id header */
    
    CALL_METHOD(riakClient, getClientId, client_id, client_instance);
    
    if (asprintf(&client_id_header, "X-Riak-ClientId: %s", Z_STRVAL_P(client_id)) < 0) {
        RIAK_MALLOC_WARNING();
        goto cleanup;
    }
    
    curl = curl_easy_init();
    
    if (curl) {
        /* exec request */
        riak_curl_response_init(&response);
        
        headers = curl_slist_append(headers, client_id_header);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_URL, bucket_keys_url);        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, riak_curl_writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        res = curl_easy_perform(curl);
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        array_init(return_value);
        
        if (response.len > 0) {
            /* decode json string */
            zval *keys;
            MAKE_STD_ZVAL(keys);
            php_json_decode(keys, response.response_body, response.len, 1, 20 TSRMLS_CC);
            
            /* search "keys" key */
            zval **key_names_array;
            
            HashTable *keys_hash = NULL;
            keys_hash = Z_ARRVAL_P(keys);
 
            /* iterate over key names, add them to array */
            if (zend_hash_find(keys_hash, "keys", sizeof("keys"), (void**) &key_names_array) == SUCCESS) {
                zval *arr, **data;
                HashTable *arr_hash;
                HashPosition pointer;
    
                arr_hash = Z_ARRVAL_PP(key_names_array);
    
                for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(arr_hash, &pointer)) {
                    if (Z_TYPE_PP(data) == IS_STRING) {
                        /* @TODO maybe this could be cleaned up a bit */
                        char *key_name;
                        int key_name_len;
                        
                        char *key_name_decoded;
                        int key_name_decoded_len;
                        
                        key_name = Z_STRVAL_PP(data);
                        key_name_len = Z_STRLEN_PP(data);
                        
                        key_name_decoded = estrndup(key_name, key_name_len);
                        key_name_decoded_len = php_url_decode(key_name_decoded, key_name_len);

                        add_next_index_stringl(return_value, key_name_decoded, key_name_decoded_len, 0);
                    }
                }
            }

            zval_ptr_dtor(&keys);
        }
        
        efree(response.response_body);
    }
    
    cleanup:
                    
    if (base_address) {
        free(base_address);
    }    
        
    if (bucket_keys_url) {
        free(bucket_keys_url);
    } 
    
    if (client_id_header) {
        free(client_id_header); 
    }
    
    zval_ptr_dtor(&client_id); 
    
    /*zval_ptr_dtor(&bucket_name); */
}

