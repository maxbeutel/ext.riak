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



static function_entry riakObject_methods[] = {
    PHP_ME(riakObject, __construct, arginfo_riakObject_construct, ZEND_ACC_PUBLIC)
    
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
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_HEADERS, RIAK_OBJECT_HEADERS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC); /* @TODO init array */
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_LINKS, RIAK_OBJECT_LINKS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC); /* @TODO init array */
    zend_declare_property_null(riak_ce_riakObject, RIAK_OBJECT_SIBLINGS, RIAK_OBJECT_SIBLINGS_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_bool(riak_ce_riakObject, RIAK_OBJECT_EXISTS, RIAK_OBJECT_EXISTS_LEN, 0, ZEND_ACC_PROTECTED TSRMLS_CC);
}

PHP_METHOD(riakObject, __construct) {
}


