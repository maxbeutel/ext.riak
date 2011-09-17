#include <php.h>

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
    zend_declare_property_bool(riak_ce_riakObject, RIAK_OBJECT_JSONIZE, RIAK_OBJECT_JSONIZE_LEN, 1, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_HEADERS, RIAK_OBJECT_HEADERS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_LINKS, RIAK_OBJECT_LINKS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC); 
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_SIBLINGS, RIAK_OBJECT_SIBLINGS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_bool(riak_ce_riakObject, RIAK_OBJECT_EXISTS, RIAK_OBJECT_EXISTS_LEN, 0, ZEND_ACC_PROTECTED TSRMLS_CC);
}

PHP_METHOD(riakObject, __construct) {
}

PHP_METHOD(riakObject, getBucket) {
}

PHP_METHOD(riakObject, getKey) {
}

PHP_METHOD(riakObject, getData) {
}

PHP_METHOD(riakObject, setData) {
}

PHP_METHOD(riakObject, status) {
}

PHP_METHOD(riakObject, exists) {
}

PHP_METHOD(riakObject, getContentType) {
}

PHP_METHOD(riakObject, setContentType) {
}

PHP_METHOD(riakObject, addLink) {
}

PHP_METHOD(riakObject, removeLink) {
}

PHP_METHOD(riakObject, getLinks) {
}

PHP_METHOD(riakObject, store) {
}

PHP_METHOD(riakObject, reload) {
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
}

PHP_METHOD(riakObject, getSiblingCount) {
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






