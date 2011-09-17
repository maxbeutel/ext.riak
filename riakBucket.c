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
    PHP_ME(riakBucket, newObject, arginfo_riakBucket_newObject, ZEND_ACC_PUBLIC)
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
    RIAK_CALL_SIMPLE_GETTER(RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN);
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
    char *key;
    int key_len;
    
    zval *data;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &key, &key_len, &data) == FAILURE) {
        return;
    }
    
    RETURN_NULL();
}

PHP_METHOD(riakBucket, newBinary) {
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
}











