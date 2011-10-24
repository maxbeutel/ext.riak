#include <php.h>

#include <ext/spl/php_spl.h>
#include <ext/json/php_json.h>

#include "ext/standard/php_smart_str.h"

#include "riak_shared.h"
#include "riak_curl.h"
#include "riakObject.h"
#include "riakClient.h"
#include "riakBucket.h"
#include "riakLink.h"



RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_empty, 0)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_construct, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, client, riakClient, 0)
    ZEND_ARG_OBJ_INFO(0, bucket, riakBucket, 0)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_setData, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_setContentType, 0, 0, 1)
	ZEND_ARG_INFO(0, contentType)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_addLink, 0, 0, 1)
    ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_removeLink, 0, 0, 0)
	ZEND_ARG_OBJ_INFO(0, link, riakLink, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_removeLinkByTag, 0, 0, 1)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_store, 0, 0, 0)
	ZEND_ARG_INFO(0, w)
	ZEND_ARG_INFO(0, dw)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_delete, 0, 0, 0)
	ZEND_ARG_INFO(0, dw)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_populate, 0, 0, 2)
	ZEND_ARG_INFO(0, response)
	ZEND_ARG_INFO(0, expectedStatuses)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_getSibling, 0, 0, 1)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, r)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_getSiblings, 0, 0, 0)
	ZEND_ARG_INFO(0, r)
ZEND_END_ARG_INFO()





static function_entry riakObject_methods[] = {
    PHP_ME(riakObject, __construct, arginfo_riakObject_construct, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakObject, getBucket, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, getKey, NULL, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakObject, getData, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, setData, arginfo_riakObject_setData, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakObject, status, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, exists, NULL, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakObject, getContentType, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, setContentType, arginfo_riakObject_setContentType, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakObject, addLink, arginfo_riakObject_addLink, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, removeLink, arginfo_riakObject_removeLink, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, removeLinkByTag, arginfo_riakObject_removeLinkByTag, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, getLinks, NULL, ZEND_ACC_PUBLIC)   
    
    PHP_ME(riakObject, store, arginfo_riakObject_store, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, delete, arginfo_riakObject_delete, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakObject, clear, NULL, ZEND_ACC_PROTECTED)
    PHP_ME(riakObject, vclock, NULL, ZEND_ACC_PROTECTED)
    
    PHP_ME(riakObject, populate, arginfo_riakObject_populate, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, populateLinks, NULL, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakObject, hasSiblings, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, getSiblingCount, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, getSibling, arginfo_riakObject_getSibling, ZEND_ACC_PUBLIC)  
    PHP_ME(riakObject, getSiblings, arginfo_riakObject_getSiblings, ZEND_ACC_PUBLIC)
        
    PHP_ME(riakObject, add, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, link, NULL, ZEND_ACC_PUBLIC)   
    
    PHP_ME(riakObject, reduce, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, map, NULL, ZEND_ACC_PUBLIC) 
    
    {NULL, NULL, NULL}
};



void riak_init_riakObject(TSRMLS_D) {
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "riakObject", riakObject_methods);
    riak_ce_riakObject = zend_register_internal_class(&ce TSRMLS_CC);
        
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_CLIENT, RIAK_OBJECT_CLIENT_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_BUCKET, RIAK_OBJECT_BUCKET_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_KEY, RIAK_OBJECT_KEY_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_HEADERS, RIAK_OBJECT_HEADERS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_LINKS, RIAK_OBJECT_LINKS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC); 
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_SIBLINGS, RIAK_OBJECT_SIBLINGS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_bool(riak_ce_riakObject, RIAK_OBJECT_EXISTS, RIAK_OBJECT_EXISTS_LEN, 0, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_bool(riak_ce_riakObject, RIAK_OBJECT_DELETED, RIAK_OBJECT_DELETED_LEN, 0, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_DATA, RIAK_OBJECT_DATA_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
}


PHPAPI int riak_object_get_sibling_count(zval *object_instance TSRMLS_DC) {
    int siblings_count;
    
    zval *siblings;
    HashTable *siblings_hash;
    
    
    siblings = zend_read_property(riak_ce_riakObject, object_instance, RIAK_OBJECT_SIBLINGS, RIAK_OBJECT_SIBLINGS_LEN, 0 TSRMLS_CC);
    siblings_hash = Z_ARRVAL_P(siblings);
    siblings_count = zend_hash_num_elements(siblings_hash);
    
    return siblings_count;
}

PHPAPI int riak_object_get_header(zval *object_instance, char *key, int key_size, zval **return_value TSRMLS_DC) {
    zval *headers;
    
    int result;
    
    headers = zend_read_property(riak_ce_riakObject, object_instance, RIAK_OBJECT_HEADERS, RIAK_OBJECT_HEADERS_LEN, 0 TSRMLS_CC);
    
    HashTable *headers_hash = NULL;
    headers_hash = Z_ARRVAL_P(headers);
    
    zval **value;

    if (zend_hash_find(headers_hash, key, key_size, (void**) &value) == SUCCESS) {
        **return_value = **value;
        zval_copy_ctor(*return_value);
        
        result = SUCCESS;
                    
        zval_ptr_dtor(value);
    } else {
        result = FAILURE;
    }
    
    return result;
}

PHPAPI void riak_object_set_header(zval *object_instance, char *key, char *value, int value_len TSRMLS_DC) {
    zval *headers;
    
    headers = zend_read_property(riak_ce_riakObject, object_instance, RIAK_OBJECT_HEADERS, RIAK_OBJECT_HEADERS_LEN, 0 TSRMLS_CC);
    add_assoc_stringl(headers, key, value, value_len, 1);
}


PHPAPI void riak_object_add_link_as_request_header(riakCurlRequestHeader *request_header, zval* link_instance TSRMLS_DC) {
    char *header_str;
    int header_str_len;
    
    header_str = Z_STRVAL_P(zend_read_property(riak_ce_riakLink, link_instance, RIAK_LINK_REQUESTHEADER_STR, RIAK_LINK_REQUESTHEADER_STR_LEN, 0 TSRMLS_CC));
    header_str_len = Z_STRLEN_P(zend_read_property(riak_ce_riakLink, link_instance, RIAK_LINK_REQUESTHEADER_STR, RIAK_LINK_REQUESTHEADER_STR_LEN, 0 TSRMLS_CC));
    
    riak_curl_add_request_header_str(request_header, header_str, header_str_len);    
}





PHPAPI int riak_object_set_header_from_raw_string(zval *object_instance, char *header_str, int header_str_len, int header_str_prefix_len, char *key TSRMLS_DC) {
    int result;
    
    char *to = NULL;
    to = strndup(header_str + header_str_prefix_len, header_str_len);   
    
    if (to) {
        php_printf("value from raw string: |%s|\n", to);
        
        riak_object_set_header(object_instance, key, to, strlen(to) TSRMLS_CC);
        
        /* found location header -> exchange key */
        if (strcmp(key, RIAK_OBJECT_HEADER_LOCATION) == 0) {
            zend_update_property_stringl(riak_ce_riakObject, object_instance, RIAK_OBJECT_KEY, RIAK_OBJECT_KEY_LEN, to, strlen(to) TSRMLS_CC);
        }
        
        result = SUCCESS;
    } else {
        result = FAILURE;
    }
    
    
    if (to) {
        free(to);
    }
    
    return result;
}

PHPAPI int riak_object_fetch_initialized_object(zval *client_instance, zval *bucket_instance, zval *key, long r, zval **return_value TSRMLS_DC) {
    int result;
    
    char *bucket_name;
    
    char *base_address = NULL;
    char *object_url = NULL;
    
    char *client_id = NULL;
    
    zval *object_data;
    MAKE_STD_ZVAL(object_data);
    
    riakCurlRequestHeader *request_header;
    request_header = riak_curl_create_request_header();
    
    zval *links;
    
    
    /* build object url */
    if (riak_client_base_address(client_instance, 1, &base_address TSRMLS_CC) == FAILURE) {
        goto cleanup;
    }
    
    
    bucket_name = Z_STRVAL_P(zend_read_property(riak_ce_riakBucket, bucket_instance, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, 0 TSRMLS_CC));
    r = riak_bucket_local_or_client_setting(client_instance, bucket_instance, r, RIAK_CLIENT_R, RIAK_CLIENT_R_LEN TSRMLS_CC);
    
    if (asprintf(&object_url, "%s/%s/%s?r=%ld", base_address, bucket_name, Z_STRVAL_P(key), r) < 0) {
        RIAK_MALLOC_WARNING();
        result = FAILURE;
        goto cleanup;
    }
    
    
    php_printf("Object url: %s\n", object_url);
 
    client_id = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, client_instance, RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC));
   
 
    if (riak_curl_fetch_json_response(client_id, object_url, &object_data, request_header TSRMLS_CC) == SUCCESS && Z_TYPE_P(object_data) != IS_NULL) {
   
        object_init_ex(*return_value, riak_ce_riakObject); 
        CALL_METHOD3(riakObject, __construct, *return_value, *return_value, client_instance, bucket_instance, key);
        
        zend_update_property(riak_ce_riakObject, *return_value, RIAK_OBJECT_DATA, RIAK_OBJECT_DATA_LEN, object_data TSRMLS_CC);
        
        
        if (request_header) {
            char** iter;
            
            links = zend_read_property(riak_ce_riakObject, *return_value, RIAK_OBJECT_LINKS, RIAK_OBJECT_LINKS_LEN, 0 TSRMLS_CC);
            
            for (iter = riak_curl_add_request_header_start(request_header); iter != riak_curl_add_request_header_end(request_header); ++iter) {
                if (strncmp(*iter, "Link:", strlen("Link:")) == 0) { 
                    zval *link_instance;
                    MAKE_STD_ZVAL(link_instance);
                    object_init_ex(link_instance, riak_ce_riakLink); 
                    
                    if (riak_link_create_link_instance_from_raw_string(client_instance, *iter, strlen(*iter), &link_instance TSRMLS_CC) == SUCCESS) {
                        add_next_index_zval(links, link_instance);
                    } else {
                        zval_ptr_dtor(&link_instance);
                    }  
                }
                
                else if (strncmp(*iter, "Content-Type: ", strlen("Content-Type: ")) == 0) {
                    riak_object_set_header_from_raw_string(*return_value, *iter, strlen(*iter), strlen("Content-Type: "), RIAK_OBJECT_HEADER_CONTENTTYPE TSRMLS_CC);
                }
                
                else if (strncmp(*iter, "X-Riak-Vclock: ", strlen("X-Riak-Vclock: ")) == 0) {
                    riak_object_set_header_from_raw_string(*return_value, *iter, strlen(*iter), strlen("X-Riak-Vclock: "), RIAK_OBJECT_HEADER_VCLOCK TSRMLS_CC);
                } 
                
                else if (strncmp(*iter, "Location: ", strlen("Location: ")) == 0) {
                    riak_object_set_header_from_raw_string(*return_value, *iter, strlen(*iter), strlen("Location: "), RIAK_OBJECT_HEADER_LOCATION TSRMLS_CC);
                }
            }
        }
        
        /* @TODO handle siblings/300 multiple choice response status */
        
       
        result = SUCCESS;
    } else {
        result = FAILURE;
    }
    

    
    
    cleanup:
    
    if (base_address) {
        free(base_address);
    }
    
    if (object_url) {
        free(object_url);
    }    
        
    zval_ptr_dtor(&object_data);
    
    if (request_header) {
        riak_curl_delete_request_header(request_header);
    }
    
    return result;
}


PHP_METHOD(riakObject, __construct) {
    zval *client;
    zval *bucket;

    char *key;
    int key_len;

    zval *headers, *links, *siblings;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "oos", &client, &bucket, &key, &key_len) == FAILURE) {
        return;
    }
    
    zend_update_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_CLIENT, RIAK_OBJECT_CLIENT_LEN, client TSRMLS_CC);
    zend_update_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_BUCKET, RIAK_OBJECT_BUCKET_LEN, bucket TSRMLS_CC);
    zend_update_property_stringl(riak_ce_riakObject, getThis(), RIAK_OBJECT_KEY, RIAK_OBJECT_KEY_LEN, key, key_len TSRMLS_CC);
    
    MAKE_STD_ZVAL(headers);
    array_init(headers);
    
    zend_update_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_HEADERS, RIAK_OBJECT_HEADERS_LEN, headers TSRMLS_CC);
    
    zval_ptr_dtor(&headers);
    
    MAKE_STD_ZVAL(links);
    array_init(links);
    
    zend_update_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_LINKS, RIAK_OBJECT_LINKS_LEN, links TSRMLS_CC);
    
    zval_ptr_dtor(&links);
    
    MAKE_STD_ZVAL(siblings);
    array_init(siblings);
    
    zend_update_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_SIBLINGS, RIAK_OBJECT_SIBLINGS_LEN, links TSRMLS_CC);
    
    zval_ptr_dtor(&siblings);    
}

PHP_METHOD(riakObject, getBucket) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakObject, RIAK_OBJECT_BUCKET, RIAK_OBJECT_BUCKET_LEN);
}

PHP_METHOD(riakObject, getKey) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakObject, RIAK_OBJECT_KEY, RIAK_OBJECT_KEY_LEN);
}

PHP_METHOD(riakObject, getData) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakObject, RIAK_OBJECT_DATA, RIAK_OBJECT_DATA_LEN);
}

PHP_METHOD(riakObject, setData) {
    zval *data;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &data) == FAILURE) {
        return;
    }
    
    zend_update_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_DATA, RIAK_OBJECT_DATA_LEN, data TSRMLS_CC);
    
    RIAK_RETURN_SELF();
}

PHP_METHOD(riakObject, status) {
    if (riak_object_get_header(getThis(), RIAK_OBJECT_HEADER_HTTPSTATUS, RIAK_OBJECT_HEADER_HTTPSTATUS_SIZE, &return_value TSRMLS_CC) == FAILURE) {
        RETURN_NULL();
    }
}

PHP_METHOD(riakObject, exists) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakObject, RIAK_OBJECT_EXISTS, RIAK_OBJECT_EXISTS_LEN);
}

PHP_METHOD(riakObject, getContentType) {
    if (riak_object_get_header(getThis(), RIAK_OBJECT_HEADER_CONTENTTYPE, RIAK_OBJECT_HEADER_CONTENTTYPE_SIZE, &return_value TSRMLS_CC) == FAILURE) {
        RETURN_NULL();
    }
}

PHP_METHOD(riakObject, setContentType) {
    char *content_type;
    int content_type_len;
    
    zval *headers;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &content_type, &content_type_len) == FAILURE) {
        return;
    }
    
    riak_object_set_header(getThis(), RIAK_OBJECT_HEADER_CONTENTTYPE, content_type, content_type_len TSRMLS_CC);

    RIAK_RETURN_SELF();
}

PHP_METHOD(riakObject, addLink) {    
    char *key;
    int key_len;
    
    char *tag = NULL;
    int tag_len;
    
    zval *client_instance;
    zval *bucket_instance;
    
    zval *link_instance;
    
    zval *links;
    
    zval **data;
    zval *current_link;
    HashTable *links_hash;
    HashPosition pointer;    
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &key, &key_len, &tag, &tag_len) == FAILURE) {
        return;
    }
    

    client_instance = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_CLIENT, RIAK_OBJECT_CLIENT_LEN, 0 TSRMLS_CC);
    bucket_instance = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_BUCKET, RIAK_OBJECT_BUCKET_LEN, 0 TSRMLS_CC);
        
    MAKE_STD_ZVAL(link_instance);
    object_init_ex(link_instance, riak_ce_riakLink);
   
    riak_link__constructor(link_instance, client_instance, bucket_instance, key, key_len, tag, tag_len TSRMLS_CC);
    
    /* remove existing equal link if exists */
    links = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_LINKS, RIAK_OBJECT_LINKS_LEN, 0 TSRMLS_CC);
    links_hash = Z_ARRVAL_P(links);
    
    for (zend_hash_internal_pointer_reset_ex(links_hash, &pointer); zend_hash_get_current_data_ex(links_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(links_hash, &pointer)) {
        long index;
        
        /* fugly, but i dont need key and key_len... */
        char *_;
        int __;
        
        zend_hash_get_current_key_ex(links_hash, &_, &__, &index, 0, &pointer);
        
        if (Z_TYPE_PP(data) == IS_OBJECT) {
            current_link = *data;
            
            if (riak_link_instances_equal(current_link, link_instance TSRMLS_CC)) {
                if (zend_hash_index_del(links_hash, index) == FAILURE) {
                    php_printf("Removing existing link did not work\n");
                }
            }
        }
    } /* holy arrow code, batman! */
    
    add_next_index_zval(links, link_instance);
    
    RIAK_RETURN_SELF();
}

PHP_METHOD(riakObject, removeLink) {
    zval *arg;
    /* can be a link instance or an array of link instances */
    
    
}

PHP_METHOD(riakObject, removeLinkByTag) {
    zval *arg;
    /* can be a tag or an array of tags */
    
}

PHP_METHOD(riakObject, getLinks) {
    
}

PHP_METHOD(riakObject, store) {
    long w = 0;
    long dw = 0;
    
    zval *client_instance;
    zval *bucket_instance;
    
    zval *data;
    
    zval *content_type_header;
    char *content_type;
    char *content_type_request_header = NULL;
    
    zval *vclock_header;
    char *vclock;
    char *vclock_request_header = NULL;
    
    zval *links;
    HashTable *links_hash;
    
    char *base_address = NULL;
    char *bucket_name;
    char *key;
    
    char *object_url = NULL;

    char *client_id;
    
    riakCurlRequestHeader *request_header = NULL;

    int result;
    
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ll", &w, &dw) == FAILURE) {
        return;
    }
    
    
    MAKE_STD_ZVAL(content_type_header);
    MAKE_STD_ZVAL(vclock_header);
    
    client_instance = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_CLIENT, RIAK_OBJECT_CLIENT_LEN, 0 TSRMLS_CC);
    bucket_instance = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_BUCKET, RIAK_OBJECT_BUCKET_LEN, 0 TSRMLS_CC);
    

    /* get content type header */
    riak_object_get_header(getThis(), RIAK_OBJECT_HEADER_CONTENTTYPE, RIAK_OBJECT_HEADER_CONTENTTYPE_SIZE, &content_type_header TSRMLS_CC);
    content_type = Z_STRVAL_P(content_type_header);
    
    
    /* read data */
    data = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_DATA, RIAK_OBJECT_DATA_LEN, 0 TSRMLS_CC);
    
    /* construct object url */
    if (riak_client_base_address(client_instance, 1, &base_address TSRMLS_CC) == FAILURE) {
        goto cleanup;
    }    
    
    bucket_name = Z_STRVAL_P(zend_read_property(riak_ce_riakBucket, bucket_instance, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, 0 TSRMLS_CC));
    key = Z_STRVAL_P(zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_KEY, RIAK_OBJECT_KEY_LEN, 0 TSRMLS_CC));
    w = riak_bucket_local_or_client_setting(client_instance, bucket_instance, w, RIAK_CLIENT_W, RIAK_CLIENT_W_LEN TSRMLS_CC);
    dw = riak_bucket_local_or_client_setting(client_instance, bucket_instance, dw, RIAK_CLIENT_DW, RIAK_CLIENT_DW_LEN TSRMLS_CC);
    
    if (asprintf(&object_url, "%s/%s/%s?w=%ld&dw=%ld", base_address, bucket_name, key, w, dw) < 0) {
        RIAK_MALLOC_WARNING();
        goto cleanup;
    }
        
    /* add additional headers */
    request_header = riak_curl_create_request_header();
    
    riak_curl_add_request_header_str(request_header, "Accept: text/plain, */*; q=0.5", strlen("Accept: text/plain, */*; q=0.5"));
    
    if (riak_object_get_header(getThis(), RIAK_OBJECT_HEADER_VCLOCK, RIAK_OBJECT_HEADER_VCLOCK_SIZE, &vclock_header TSRMLS_CC) == SUCCESS) {
        vclock = Z_STRVAL_P(vclock_header);
        
        if (asprintf(&vclock_request_header, "X-Riak-Vclock: %s", vclock) == FAILURE) {
            RIAK_MALLOC_WARNING();
            goto cleanup; 
        }
        
        riak_curl_add_request_header_str(request_header, vclock_request_header, strlen(vclock_request_header));
    }
    
    links = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_LINKS, RIAK_OBJECT_LINKS_LEN, 0 TSRMLS_CC);
    links_hash = Z_ARRVAL_P(links);
    
    if (zend_hash_num_elements(links_hash) > 0) {
        HashPosition pointer;
        zval **links_data;
        zval *link_instance;
        
        for (zend_hash_internal_pointer_reset_ex(links_hash, &pointer); zend_hash_get_current_data_ex(links_hash, (void**) &links_data, &pointer) == SUCCESS; zend_hash_move_forward_ex(links_hash, &pointer)) {
            if (Z_TYPE_PP(links_data) == IS_OBJECT) {
                link_instance = *links_data;
                riak_object_add_link_as_request_header(request_header, link_instance TSRMLS_CC);
            } 
        }
    }


    /* send request */
    client_id = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, client_instance, RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC));
    
    
    /* json content type header is added within specialized send_json_request functions, 
       if data is not json, add custom content type before sending request
    */
    if (strcmp(content_type, RIAK_OBJECT_JSON_CONTENTTYPE) == 0) {
        php_printf("Storing object as JSON\n");
        
        if (zend_is_true(zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_EXISTS, RIAK_OBJECT_EXISTS_LEN, 0 TSRMLS_CC))) {
            if (riak_curl_send_put_json_request(client_id, object_url, data, request_header TSRMLS_CC) == FAILURE) {
                zend_error(E_WARNING, "Could not update existing object");
            } 
        } else {
            if (riak_curl_send_post_json_request(client_id, object_url, data, request_header TSRMLS_CC) == FAILURE) {
                zend_error(E_WARNING, "Could not store object");
            } 
        }
    } else {
        php_printf("Storing object as %s\n", content_type);
        
        if (strlen(content_type) > 0) {
            if (asprintf(&content_type_request_header, "Content-Type: %s", content_type) == FAILURE) {
                RIAK_MALLOC_WARNING();
                goto cleanup; 
            }
            
            php_printf("Adding custom header: %s\n", content_type_request_header);
        
            riak_curl_add_request_header_str(request_header, content_type_request_header, strlen(content_type_request_header));
        }
        
        if (zend_is_true(zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_EXISTS, RIAK_OBJECT_EXISTS_LEN, 0 TSRMLS_CC))) {
            if (riak_curl_send_put_request(client_id, object_url, Z_STRVAL_P(data), request_header TSRMLS_CC) == FAILURE) {
                zend_error(E_WARNING, "Could not update existing object");
            }  
        } else {
            if (riak_curl_send_post_request(client_id, object_url, Z_STRVAL_P(data), request_header TSRMLS_CC) == FAILURE) {
                zend_error(E_WARNING, "Could not store object");
            } 
        }
    }

    
    cleanup:
    
    if (base_address) {
        free(base_address);
    }    
    
    if (object_url) {
        free(object_url);
    }
    
    if (request_header) {
        riak_curl_delete_request_header(request_header);
    }
    
    if (content_type_request_header) {
        free(content_type_request_header);
    }
    
    if (vclock_request_header) {
        free(vclock_request_header);
    }
        
    zval_ptr_dtor(&content_type_header);
    
    RIAK_RETURN_SELF();
}

PHP_METHOD(riakObject, delete) {
    long dw = 0;
    
    zval *client_instance;
    zval *bucket_instance;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &dw) == FAILURE) {
        return;
    }
    
    client_instance = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_CLIENT, RIAK_OBJECT_CLIENT_LEN, 0 TSRMLS_CC);
    bucket_instance = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_BUCKET, RIAK_OBJECT_BUCKET_LEN, 0 TSRMLS_CC);
    
    dw = riak_bucket_local_or_client_setting(client_instance, bucket_instance, dw, RIAK_CLIENT_DW, RIAK_CLIENT_DW_LEN TSRMLS_CC);
    
    /* @TODO exec request */
    /* @TODO freeze object as it is deleted */
}

PHP_METHOD(riakObject, clear) {
    /* do we actually need this? */
}

PHP_METHOD(riakObject, vclock) {
    /* do we actually need this? */
}

PHP_METHOD(riakObject, populate) {
    /* do we actually need this? */
}

PHP_METHOD(riakObject, populateLinks) {
    /* do we actually need this? */
}

PHP_METHOD(riakObject, hasSiblings) {
    int sibling_count;
    
    sibling_count = riak_object_get_sibling_count(getThis() TSRMLS_CC);
    
    if (sibling_count > 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_METHOD(riakObject, getSiblingCount) {    
    int sibling_count;
    
    sibling_count = riak_object_get_sibling_count(getThis() TSRMLS_CC);
    
    RETURN_LONG(sibling_count);
}

PHP_METHOD(riakObject, getSibling) {
    long i;
    long r = 0;
    
    zval *client_instance;
    zval *bucket_instance;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "i|l", &r) == FAILURE) {
        return;
    }
    
    client_instance = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_CLIENT, RIAK_OBJECT_CLIENT_LEN, 0 TSRMLS_CC);
    bucket_instance = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_BUCKET, RIAK_OBJECT_BUCKET_LEN, 0 TSRMLS_CC);
    
    r = riak_bucket_local_or_client_setting(client_instance, bucket_instance, r, RIAK_CLIENT_R, RIAK_CLIENT_R_LEN TSRMLS_CC);
    
  
    
    /* @TODO exec request */
}

PHP_METHOD(riakObject, getSiblings) {
    long r;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &r) == FAILURE) {
        return;
    }    
}

PHP_METHOD(riakObject, add) {
}

PHP_METHOD(riakObject, link) {
}

PHP_METHOD(riakObject, reduce) {
}

PHP_METHOD(riakObject, map) {
}







