#include <php.h>

#include "riakClient.h"
#include "riakBucket.h"

zend_class_entry *riak_ce_riakBucket;

/* TODO: arginfo */

static function_entry riakBucket_methods[] = {
    PHP_ME(riakBucket, __construct, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(riakBucket, getName, NULL, ZEND_ACC_PUBLIC)
    
    {NULL, NULL, NULL}
};


void riak_init_riakBucket(TSRMLS_D) {
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "riakBucket", riakBucket_methods);
    riak_ce_riakBucket = zend_register_internal_class(&ce TSRMLS_CC);
    
    zend_declare_property_null(riak_ce_riakBucket, RIAK_BUCKET_CLIENT, RIAK_BUCKET_CLIENT_LEN, ZEND_ACC_PROTECTED TSRMLS_CC); /* TODO: shall we really pass client instance in constructor? maybe make private setter */
    zend_declare_property_null(riak_ce_riakBucket, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakBucket, RIAK_CLIENT_R, RIAK_CLIENT_R_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakBucket, RIAK_CLIENT_W, RIAK_CLIENT_W_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakBucket, RIAK_CLIENT_DW, RIAK_CLIENT_DW_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
}

PHP_METHOD(riakBucket, __construct) {
}

PHP_METHOD(riakBucket, getName) {
}