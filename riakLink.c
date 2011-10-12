#include <php.h>

#include "riak_shared.h"
#include "riakBucket.h"
#include "riakClient.h"
#include "riakLink.h"
#include "riakObject.h"



RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_empty, 0)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakLink_construct, 0, 0, 2)	
	ZEND_ARG_OBJ_INFO(0, client, riakClient, 0)
	ZEND_ARG_OBJ_INFO(0, bucket, riakBucket, 0)
	ZEND_ARG_OBJ_INFO(0, object, riakObject, 0)
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
    zend_declare_property_null(riak_ce_riakLink, RIAK_LINK_OBJECT, RIAK_LINK_OBJECT_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakLink, RIAK_LINK_TAG, RIAK_LINK_TAG_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(riak_ce_riakLink, RIAK_LINK_REQUESTHEADER_STR, RIAK_LINK_REQUESTHEADER_STR_LEN, ZEND_ACC_PROTECTED TSRMLS_CC);    
}

PHPAPI int riak_link_instances_equal(zval *link_instance_a, zval *link_instance_b TSRMLS_DC) {
    zval *client_instance_a;
    zval *bucket_instance_a;
    zval *object_instance_a;
    
    char *client_id_a;
    char *bucket_name_a;
    char *key_a;
    char *tag_a;
    
    zval *client_instance_b;
    zval *bucket_instance_b;
    zval *object_instance_b;
    
    char *client_id_b;
    char *bucket_name_b;
    char *key_b;
    char *tag_b;
    
    
    client_instance_a = zend_read_property(riak_ce_riakLink, link_instance_a, RIAK_LINK_CLIENT, RIAK_LINK_CLIENT_LEN, 0 TSRMLS_CC);
    bucket_instance_a = zend_read_property(riak_ce_riakLink, link_instance_a, RIAK_LINK_BUCKET, RIAK_LINK_BUCKET_LEN, 0 TSRMLS_CC);
    object_instance_a = zend_read_property(riak_ce_riakLink, link_instance_a, RIAK_LINK_OBJECT, RIAK_LINK_OBJECT_LEN, 0 TSRMLS_CC);
    
    client_instance_b = zend_read_property(riak_ce_riakLink, link_instance_b, RIAK_LINK_CLIENT, RIAK_LINK_CLIENT_LEN, 0 TSRMLS_CC);
    bucket_instance_b = zend_read_property(riak_ce_riakLink, link_instance_b, RIAK_LINK_BUCKET, RIAK_LINK_BUCKET_LEN, 0 TSRMLS_CC);
    object_instance_b = zend_read_property(riak_ce_riakLink, link_instance_b, RIAK_LINK_OBJECT, RIAK_LINK_OBJECT_LEN, 0 TSRMLS_CC);
    

    client_id_a = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, client_instance_a, RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC));
    bucket_name_a = Z_STRVAL_P(zend_read_property(riak_ce_riakBucket, bucket_instance_a, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, 0 TSRMLS_CC));
    key_a = Z_STRVAL_P(zend_read_property(riak_ce_riakObject, object_instance_a, RIAK_OBJECT_KEY, RIAK_OBJECT_KEY_LEN, 0 TSRMLS_CC));
    tag_a = Z_STRVAL_P(zend_read_property(riak_ce_riakLink, link_instance_a, RIAK_LINK_TAG, RIAK_LINK_TAG_LEN, 0 TSRMLS_CC));
    
    client_id_b = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, client_instance_b, RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC));
    bucket_name_b = Z_STRVAL_P(zend_read_property(riak_ce_riakBucket, bucket_instance_b, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, 0 TSRMLS_CC));
    key_b = Z_STRVAL_P(zend_read_property(riak_ce_riakObject, object_instance_b, RIAK_OBJECT_KEY, RIAK_OBJECT_KEY_LEN, 0 TSRMLS_CC));
    tag_b = Z_STRVAL_P(zend_read_property(riak_ce_riakLink, link_instance_b, RIAK_LINK_TAG, RIAK_LINK_TAG_LEN, 0 TSRMLS_CC));
    
    if (strcmp(client_id_a, client_id_b) != 0) {
        return 0;
    }
    
    if (strcmp(bucket_name_a, bucket_name_b) != 0) {
        return 0;
    }
    
    if (strcmp(key_a, key_b) != 0) {
        return 0;
    }
    
    if (strcmp(tag_a, tag_b) != 0) {
        return 0;
    }
    
    return 1;
}

PHPAPI int riak_link_create_request_header_str(char **str, zval* client_instance, zval* bucket_instance, zval* object_instance, char* tag, int tag_len TSRMLS_DC) {
    char *prefix;
    int prefix_len;
    
    char *bucket_name;
    int bucket_name_len;
    
    char *key;
    int key_len;    
    
    char *prefix_encoded;
    int prefix_encoded_len;
    
    char *bucket_name_encoded;
    int bucket_name_encoded_len;
        
    char *key_encoded;
    int key_encoded_len;
    
    char *tag_encoded;
    int tag_encoded_len;
    
    int result;
    
    
    prefix = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, client_instance, RIAK_CLIENT_PREFIX, RIAK_CLIENT_PREFIX_LEN, 0 TSRMLS_CC));
    prefix_len = Z_STRLEN_P(zend_read_property(riak_ce_riakClient, client_instance, RIAK_CLIENT_PREFIX, RIAK_CLIENT_PREFIX_LEN, 0 TSRMLS_CC));
    
    bucket_name = Z_STRVAL_P(zend_read_property(riak_ce_riakBucket, bucket_instance, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, 0 TSRMLS_CC));
    bucket_name_len = Z_STRLEN_P(zend_read_property(riak_ce_riakBucket, bucket_instance, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, 0 TSRMLS_CC));
    
    key = Z_STRVAL_P(zend_read_property(riak_ce_riakObject, object_instance, RIAK_OBJECT_KEY, RIAK_OBJECT_KEY_LEN, 0 TSRMLS_CC));
    key_len = Z_STRLEN_P(zend_read_property(riak_ce_riakObject, object_instance, RIAK_OBJECT_KEY, RIAK_OBJECT_KEY_LEN, 0 TSRMLS_CC));    
    
    /* urlencode values */
    prefix_encoded = estrndup(prefix, prefix_len);
    prefix_encoded_len = php_url_encode(prefix_encoded, prefix_len);
    
    bucket_name_encoded = estrndup(bucket_name, bucket_name_len);
    bucket_name_encoded_len = php_url_encode(bucket_name_encoded, bucket_name_len);
    
    key_encoded = estrndup(key, key_len);
    key_encoded_len = php_url_encode(key_encoded, key_len);    
    
    if (tag_len > 0) {
        tag_encoded = estrndup(tag, tag_len);
        tag_encoded_len = php_url_encode(tag_encoded, tag_len);  
    }
    
    /* build link header string */
    if (asprintf(str, "Link: </%s/%s/%s>; riaktag=\"%s\"", prefix_encoded, bucket_name_encoded, key_encoded, tag_encoded) < 0) {
        result = FAILURE;
        RIAK_MALLOC_WARNING();
    } else {
        result = SUCCESS;
    }
    
    return result;
}


PHP_METHOD(riakLink, __construct) {
    zval *client_instance;
    
    zval *bucket_instance;
    
    zval *object_instance;
    
    char *tag;
    int tag_len;
    
    char *request_header_str = NULL;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ooo|s", &client_instance, &bucket_instance, &object_instance, &tag, &tag_len) == FAILURE) {
        return;
    }
    
    zend_update_property(riak_ce_riakLink, getThis(), RIAK_LINK_CLIENT, RIAK_LINK_CLIENT_LEN, client_instance TSRMLS_CC);
    zend_update_property(riak_ce_riakLink, getThis(), RIAK_LINK_BUCKET, RIAK_LINK_BUCKET_LEN, bucket_instance TSRMLS_CC);
    zend_update_property(riak_ce_riakLink, getThis(), RIAK_LINK_OBJECT, RIAK_LINK_OBJECT_LEN, object_instance TSRMLS_CC);
    zend_update_property_stringl(riak_ce_riakLink, getThis(), RIAK_LINK_TAG, RIAK_LINK_TAG_LEN, tag, tag_len TSRMLS_CC);
    
    if (riak_link_create_request_header_str(&request_header_str, client_instance, bucket_instance, object_instance, tag, tag_len TSRMLS_CC) == SUCCESS) {
        zend_update_property_stringl(riak_ce_riakLink, getThis(), RIAK_LINK_REQUESTHEADER_STR, RIAK_LINK_REQUESTHEADER_STR_LEN, request_header_str, strlen(request_header_str) TSRMLS_CC);
    }
    
    
    if (request_header_str) {
        free(request_header_str);
    }
}

PHP_METHOD(riakLink, getObject) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakLink, RIAK_LINK_OBJECT, RIAK_LINK_OBJECT_LEN);
}

PHP_METHOD(riakLink, getTag) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakLink, RIAK_LINK_TAG, RIAK_LINK_TAG_LEN);
}