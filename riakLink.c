#include <php.h>

#include <ext/standard/url.h>

#include "riak_shared.h"
#include "riakBucket.h"
#include "riakClient.h"
#include "riakLink.h"
#include "riakObject.h"



RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO(arginfo_empty, 0)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakLink_construct, 0, 0, 3)	
	ZEND_ARG_OBJ_INFO(0, client, riakClient, 0)
	ZEND_ARG_OBJ_INFO(0, bucket, riakBucket, 0)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

RIAK_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_riakLink_getObject, 0, 0, 0)
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

    client_instance_b = zend_read_property(riak_ce_riakLink, link_instance_b, RIAK_LINK_CLIENT, RIAK_LINK_CLIENT_LEN, 0 TSRMLS_CC);
    bucket_instance_b = zend_read_property(riak_ce_riakLink, link_instance_b, RIAK_LINK_BUCKET, RIAK_LINK_BUCKET_LEN, 0 TSRMLS_CC);


    client_id_a = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, client_instance_a, RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC));
    bucket_name_a = Z_STRVAL_P(zend_read_property(riak_ce_riakBucket, bucket_instance_a, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, 0 TSRMLS_CC));
    key_a = Z_STRVAL_P(zend_read_property(riak_ce_riakLink, link_instance_a, RIAK_LINK_KEY, RIAK_LINK_KEY_LEN, 0 TSRMLS_CC));
    tag_a = Z_STRVAL_P(zend_read_property(riak_ce_riakLink, link_instance_a, RIAK_LINK_TAG, RIAK_LINK_TAG_LEN, 0 TSRMLS_CC));
    
    client_id_b = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, client_instance_b, RIAK_CLIENT_CLIENT_ID, RIAK_CLIENT_CLIENT_ID_LEN, 0 TSRMLS_CC));
    bucket_name_b = Z_STRVAL_P(zend_read_property(riak_ce_riakBucket, bucket_instance_b, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, 0 TSRMLS_CC));
    key_b = Z_STRVAL_P(zend_read_property(riak_ce_riakLink, link_instance_b, RIAK_LINK_KEY, RIAK_LINK_KEY_LEN, 0 TSRMLS_CC));
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

PHPAPI int riak_link_create_request_header_str(zval* client_instance, zval* bucket_instance, char *key, int key_len, char* tag, int tag_len, char **str TSRMLS_DC) {
    char *prefix;
    int prefix_len;
    
    char *bucket_name;
    int bucket_name_len;
   
    char *prefix_encoded = NULL;
    int prefix_encoded_len;
    
    char *bucket_name_encoded = NULL;
    int bucket_name_encoded_len;
        
    char *key_encoded = NULL;
    int key_encoded_len;
    
    char *tag_encoded = NULL;
    int tag_encoded_len;
    
    int result;
    
    
    prefix = Z_STRVAL_P(zend_read_property(riak_ce_riakClient, client_instance, RIAK_CLIENT_PREFIX, RIAK_CLIENT_PREFIX_LEN, 0 TSRMLS_CC));
    prefix_len = Z_STRLEN_P(zend_read_property(riak_ce_riakClient, client_instance, RIAK_CLIENT_PREFIX, RIAK_CLIENT_PREFIX_LEN, 0 TSRMLS_CC));
    
    bucket_name = Z_STRVAL_P(zend_read_property(riak_ce_riakBucket, bucket_instance, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, 0 TSRMLS_CC));
    bucket_name_len = Z_STRLEN_P(zend_read_property(riak_ce_riakBucket, bucket_instance, RIAK_BUCKET_NAME, RIAK_BUCKET_NAME_LEN, 0 TSRMLS_CC));
    
    /* urlencode values */
    prefix_encoded = php_url_encode(prefix, prefix_len, &prefix_encoded_len);
    
    bucket_name_encoded = php_url_encode(bucket_name, bucket_name_len, &bucket_name_encoded_len);
    
    key_encoded = php_url_encode(key, key_len, &key_encoded_len);    
    
    if (tag) {
        tag_encoded = php_url_encode(tag, tag_len, &tag_encoded_len);  
    } else {
        tag_encoded = "";
    }
    
    /* build link header string */
    if (asprintf(str, "Link: </%s/%s/%s>; riaktag=\"%s\"", prefix_encoded, bucket_name_encoded, key_encoded, tag_encoded) < 0) {
        result = FAILURE;
        RIAK_MALLOC_WARNING();
    } else {
        result = SUCCESS;
    }
    
    
    if (prefix_encoded) {
        efree(prefix_encoded);
    }
    
    if (bucket_name_encoded) {
        efree(bucket_name_encoded);
    }
    
    if (key_encoded) {
        efree(key_encoded);
    }
    
    if (tag && tag_encoded) {
        efree(tag_encoded);
    }
    
    return result;
}

PHPAPI int riak_link_create_link_instance_from_raw_string(zval *client_instance, char *header_str, int header_str_len, zval **link_instance TSRMLS_DC) {
    int result;
    
    char *header_str_copy = NULL;
    
    char *tok;
    char *last;
    
    /* prefix, bucket name, key */
    char *object_address_parts[3] = { NULL, NULL, NULL };
    int object_address_parts_len = 3;
    int current_object_address_part = 0;
    
    int parse_tag = 0;
    
    /* tag name, tag value */
    char *tag_parts[2] = { NULL, NULL };
    int tag_parts_len = 2;
    int current_tag_part = 0;
    
    zval *bucket_instance;
    MAKE_STD_ZVAL(bucket_instance);
 
    
    if (asprintf(&header_str_copy, "%s", header_str) < 0) {
        RIAK_MALLOC_WARNING();
        result = FAILURE;
        goto cleanup;
    }
     
    for (tok = strtok_r(header_str_copy, "</>; =\"", &last); tok; tok = strtok_r(NULL, "</>; =\"", &last)) {
        if (strcmp(tok, "Link:") != 0) {
            if (parse_tag) {
                if (current_tag_part < object_address_parts_len) {
                    tag_parts[current_tag_part] = tok;
                    current_tag_part++;
                }

            } else {
                if (current_object_address_part < object_address_parts_len) {
                    object_address_parts[current_object_address_part] = tok;
                    current_object_address_part++;   
                }
            }
            
            /* found the delimeter between object address and tag key/value pair -> from now on fill tag_parts */
            if (!parse_tag && *last == ';') {
                parse_tag = 1;
            } 
        }
    }
    
    
    /* debug */
    /*
    if (tag_parts[0]) {
        php_printf("tag key: %s\n", tag_parts[0]);
    }
    
    if (tag_parts[1]) {
        php_printf("tag val: %s\n", tag_parts[1]);
    }
    
    
    if (object_address_parts[0]) {
        php_printf("Base address: %s\n", object_address_parts[0]);
    }
    
    if (object_address_parts[1]) {
        php_printf("Bucket name: %s\n", object_address_parts[1]);
    }
      
    if (object_address_parts[2]) {
        php_printf("Key: %s\n", object_address_parts[2]);
    }    
    */
    
    
    /* the bucket name, the key and if available the tag */
    if (object_address_parts[1] && object_address_parts[2]) {
        php_printf("Creating new LINK\n");
        
        /* create new bucket instance */
        object_init_ex(bucket_instance, riak_ce_riakBucket);
        riak_bucket__constructor(bucket_instance, client_instance, object_address_parts[1], strlen(object_address_parts[1]) TSRMLS_CC);
        
        /* create new link instance */
        riak_link__constructor(*link_instance, client_instance, bucket_instance, object_address_parts[2], strlen(object_address_parts[2]), tag_parts[1], (tag_parts[1] ? strlen(tag_parts[1]) : 0) TSRMLS_CC);

        result = SUCCESS;
    } else {
        result = FAILURE;
    }
    
    
    
    
    cleanup:
    
    if (header_str_copy) {
        free(header_str_copy);
    }        
    
    zval_ptr_dtor(&bucket_instance);
    
    return result;
}

PHPAPI void riak_link__constructor(zval *link_instance, zval *client_instance, zval *bucket_instance, char *key, int key_len, char *tag, int tag_len TSRMLS_DC) {
    char *request_header_str = NULL;
    
    zend_update_property(riak_ce_riakLink, link_instance, RIAK_LINK_CLIENT, RIAK_LINK_CLIENT_LEN, client_instance TSRMLS_CC);
    zend_update_property(riak_ce_riakLink, link_instance, RIAK_LINK_BUCKET, RIAK_LINK_BUCKET_LEN, bucket_instance TSRMLS_CC);
    zend_update_property_stringl(riak_ce_riakLink, link_instance, RIAK_LINK_KEY, RIAK_LINK_KEY_LEN, key, key_len TSRMLS_CC);

    if (tag) {
        zend_update_property_stringl(riak_ce_riakLink, link_instance, RIAK_LINK_TAG, RIAK_LINK_TAG_LEN, tag, tag_len TSRMLS_CC);
    }
    
    if (riak_link_create_request_header_str(client_instance, bucket_instance, key, key_len, tag, tag_len, &request_header_str TSRMLS_CC) == SUCCESS) {
        zend_update_property_stringl(riak_ce_riakLink, link_instance, RIAK_LINK_REQUESTHEADER_STR, RIAK_LINK_REQUESTHEADER_STR_LEN, request_header_str, strlen(request_header_str) TSRMLS_CC);
    }

    
    if (request_header_str) {
        free(request_header_str);
    }
}


PHP_METHOD(riakLink, __construct) {
    zval *client_instance;
    
    zval *bucket_instance;
    
    char *key;
    int key_len;
    
    char *tag = NULL;
    int tag_len;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "oos|s", &client_instance, &bucket_instance, &key, &key_len, &tag, &tag_len) == FAILURE) {
        return;
    }
    
    riak_link__constructor(getThis(), client_instance, bucket_instance, key, key_len, tag, tag_len TSRMLS_CC);
}

PHP_METHOD(riakLink, getObject) {
    zval *r;

    zval *bucket_instance;
    
    zval *key;

    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &r) == FAILURE) {
        return;
    }
    
    bucket_instance = zend_read_property(riak_ce_riakLink, getThis(), RIAK_LINK_BUCKET, RIAK_LINK_BUCKET_LEN, 0 TSRMLS_CC);
    key = zend_read_property(riak_ce_riakLink, getThis(), RIAK_LINK_KEY, RIAK_LINK_KEY_LEN, 0 TSRMLS_CC);
    
    if (Z_TYPE_P(r) == IS_NULL) {
        CALL_METHOD1(riakBucket, getObject, return_value, bucket_instance, key);
    } else {
        CALL_METHOD2(riakBucket, getObject, return_value, bucket_instance, key, r);
    }
}

PHP_METHOD(riakLink, getTag) {
    RIAK_CALL_SIMPLE_GETTER(riak_ce_riakLink, RIAK_LINK_TAG, RIAK_LINK_TAG_LEN);
}











