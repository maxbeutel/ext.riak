#include <php.h>

#include <ext/spl/php_spl.h>
#include <ext/json/php_json.h>

#include "ext/standard/php_smart_str.h"

#include "riak_shared.h"
#include "riakObject.h"
#include "riakClient.h"
#include "riakBucket.h"



RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_empty, 0)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_construct, 0, 0, 2)
	ZEND_ARG_INFO(0, client)
	ZEND_ARG_INFO(0, bucket)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_setData, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_setContentType, 0, 0, 1)
	ZEND_ARG_INFO(0, contentType)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_addLink, 0, 0, 1)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_removeLink, 0, 0, 1)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_store, 0, 0, 0)
	ZEND_ARG_INFO(0, w)
	ZEND_ARG_INFO(0, dw)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_riakObject_reload, 0, 0, 0)
	ZEND_ARG_INFO(0, r)
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
    PHP_ME(riakObject, getLinks, NULL, ZEND_ACC_PUBLIC)   
    
    PHP_ME(riakObject, store, arginfo_riakObject_store, ZEND_ACC_PUBLIC)
    PHP_ME(riakObject, reload, arginfo_riakObject_reload, ZEND_ACC_PUBLIC)
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
    zend_declare_property_bool(riak_ce_riakObject, RIAK_OBJECT_JSONIZE, RIAK_OBJECT_JSONIZE_LEN, 0, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_HEADERS, RIAK_OBJECT_HEADERS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_LINKS, RIAK_OBJECT_LINKS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC); 
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_SIBLINGS, RIAK_OBJECT_SIBLINGS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_bool(riak_ce_riakObject, RIAK_OBJECT_EXISTS, RIAK_OBJECT_EXISTS_LEN, 0, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_DATA, RIAK_OBJECT_DATA_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
}


int riak_object_get_sibling_count(zval *object_instance TSRMLS_DC) {
    int siblings_count;
    
    zval *siblings;
    HashTable *siblings_hash;
    
    
    siblings = zend_read_property(riak_ce_riakObject, object_instance, RIAK_OBJECT_SIBLINGS, RIAK_OBJECT_SIBLINGS_LEN, 0 TSRMLS_CC);
    
    if (Z_TYPE_P(siblings) == IS_ARRAY) {
        siblings_hash = Z_ARRVAL_P(siblings);
        
        siblings_count = zend_hash_num_elements(siblings_hash);
    } else {
        siblings_count = 0;
    }
    
    return siblings_count;
}


PHP_METHOD(riakObject, __construct) {
    zval *client;
    zval *bucket;

    char *key;
    int key_len;

    zval *headers, *links;
    
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
}

PHP_METHOD(riakObject, exists) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakObject, RIAK_OBJECT_EXISTS, RIAK_OBJECT_EXISTS_LEN);
}

PHP_METHOD(riakObject, getContentType) {
    zval *headers;
    
    headers = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_HEADERS, RIAK_OBJECT_HEADERS_LEN, 0 TSRMLS_CC);
    
    HashTable *headers_hash = NULL;
    headers_hash = Z_ARRVAL_P(headers);
    
    zval **content_type;

    if (zend_hash_find(headers_hash, "content-type", sizeof("content-type"), (void**) &content_type) == SUCCESS) {
        *return_value = **content_type;
        zval_copy_ctor(return_value); 
        
        zval_ptr_dtor(content_type);
    }
}

PHP_METHOD(riakObject, setContentType) {
    char *content_type;
    int content_type_len;
    
    zval *headers;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &content_type, &content_type_len) == FAILURE) {
        return;
    }
    
    headers = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_HEADERS, RIAK_OBJECT_HEADERS_LEN, 0 TSRMLS_CC);
    add_assoc_stringl(headers, "content-type", content_type, content_type_len, 1);
    
    if (strcmp(content_type, "text/json") == 0) {
        zend_update_property_bool(riak_ce_riakObject, getThis(), RIAK_OBJECT_JSONIZE, RIAK_OBJECT_JSONIZE_LEN, 1 TSRMLS_CC);
    }
    
    RIAK_RETURN_SELF();
}

PHP_METHOD(riakObject, addLink) {
}

PHP_METHOD(riakObject, removeLink) {
}

PHP_METHOD(riakObject, getLinks) {
}

PHP_METHOD(riakObject, store) {
    zval *data;
    zval *jsonize;
    
    zval *json_data;
    
    zend_bool encode_as_json;

    data = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_DATA, RIAK_OBJECT_DATA_LEN, 0 TSRMLS_CC);
    jsonize = zend_read_property(riak_ce_riakObject, getThis(), RIAK_OBJECT_JSONIZE, RIAK_OBJECT_JSONIZE_LEN, 0 TSRMLS_CC);
    
    encode_as_json = Z_BVAL_P(jsonize);
    
    /* encode as json before storing */
    if (encode_as_json) {
        smart_str buf = {0};
        
        php_json_encode(&buf, data, 0 TSRMLS_CC);
        
        MAKE_STD_ZVAL(json_data);
        ZVAL_STRINGL(json_data, buf.c, buf.len, 1);
        
        smart_str_free(&buf);
                
        zval_ptr_dtor(&json_data);
    }
    
    
    
    RIAK_RETURN_SELF();
}

PHP_METHOD(riakObject, reload) {
    long r;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &r) == FAILURE) {
        return;
    }
    
    php_printf("Reloading object with: r=%ld\n", r);
}

PHP_METHOD(riakObject, delete) {
}

PHP_METHOD(riakObject, clear) {
}

PHP_METHOD(riakObject, vclock) {
}

PHP_METHOD(riakObject, populate) {
}

PHP_METHOD(riakObject, populateLinks) {
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
}

PHP_METHOD(riakObject, getSiblings) {
}

PHP_METHOD(riakObject, add) {
}

PHP_METHOD(riakObject, link) {
}

PHP_METHOD(riakObject, reduce) {
}

PHP_METHOD(riakObject, map) {
}







