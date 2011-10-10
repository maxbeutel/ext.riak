#include <php.h>

#include "riak_shared.h"
#include "riakLink.h"
#include "riakObject.h"



RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_empty, 0)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakLink_construct, 0, 0, 2)
	ZEND_ARG_INFO(0, client)
	ZEND_ARG_INFO(0, bucket)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakLink_getObject, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, r)
ZEND_END_ARG_INFO()



static function_entry riakLink_methods[] = {
    PHP_ME(riakLink, __construct, arginfo_riakLink_construct, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakLink, getObject, arginfo_riakLink_getObject, ZEND_ACC_PUBLIC)
    
    PHP_ME(riakLink, getTag, NULL, ZEND_ACC_PUBLIC)
        
    {NULL, NULL, NULL}
};


void riak_init_riakLink(TSRMLS_D) {
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "riakLink", riakLink_methods);
    riak_ce_riakLink = zend_register_internal_class(&ce TSRMLS_CC);
        
    zend_declare_property_null(riak_ce_riakLink, RIAK_LINK_CLIENT, RIAK_LINK_CLIENT_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakLink, RIAK_LINK_BUCKET, RIAK_LINK_BUCKET_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakLink, RIAK_LINK_KEY, RIAK_LINK_KEY_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakLink, RIAK_LINK_TAG, RIAK_LINK_TAG_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
}


PHP_METHOD(riakLink, __construct) {
    zval *client;
    
    zval *bucket;
    
    char *key;
    int key_len;
    
    char *tag;
    int tag_len;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "oos|s", &client, &bucket, &key, &key_len, &tag, &tag_len) == FAILURE) {
        return;
    }
    
    zend_update_property(riak_ce_riakLink, getThis(), RIAK_LINK_CLIENT, RIAK_LINK_CLIENT_LEN, client TSRMLS_CC);
    zend_update_property(riak_ce_riakLink, getThis(), RIAK_LINK_BUCKET, RIAK_LINK_BUCKET_LEN, client TSRMLS_CC);
    zend_update_property_stringl(riak_ce_riakLink, getThis(), RIAK_LINK_KEY, RIAK_LINK_KEY_LEN, key, key_len TSRMLS_CC);
    zend_update_property_stringl(riak_ce_riakLink, getThis(), RIAK_LINK_TAG, RIAK_LINK_TAG_LEN, tag, tag_len TSRMLS_CC);
}

PHP_METHOD(riakLink, getObject) {
    zval *key;

    zval *r;

    zval *bucket_instance;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &key, &r) == FAILURE) {
        return;
    }
    
    object_init_ex(return_value, riak_ce_riakObject);
    
    if (Z_TYPE_P(r) == IS_NULL) {
        CALL_METHOD1(riakObject, getObject, return_value, return_value, key);
    } else {
        CALL_METHOD2(riakObject, getObject, return_value, return_value, key, r);
    }
}

PHP_METHOD(riakLink, getTag) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakLink, RIAK_LINK_TAG, RIAK_LINK_TAG_LEN);
}