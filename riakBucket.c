#include <php.h>

#include "riak_shared.h"
#include "riakClient.h"
#include "riakBucket.h"


RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_empty, 0)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_riakBucket_construct, 0)
    ZEND_ARG_OBJ_INFO(0, client, riakClient, 0)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()


static function_entry riakBucket_methods[] = {
    PHP_ME(riakBucket, __construct, arginfo_riakBucket_construct, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, getName, NULL, ZEND_ACC_PUBLIC)
    
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
}