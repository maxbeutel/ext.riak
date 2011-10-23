#ifndef RIAK_LINK_H
#define RIAK_LINK_H

zend_class_entry *riak_ce_riakLink;


#define RIAK_LINK_BUCKET "bucket"
#define RIAK_LINK_BUCKET_LEN strlen(RIAK_LINK_BUCKET)

#define RIAK_LINK_KEY "key"
#define RIAK_LINK_KEY_LEN strlen(RIAK_LINK_KEY)

#define RIAK_LINK_TAG "tag"
#define RIAK_LINK_TAG_LEN strlen(RIAK_LINK_TAG)

#define RIAK_LINK_CLIENT "client"
#define RIAK_LINK_CLIENT_LEN strlen(RIAK_LINK_CLIENT)

#define RIAK_LINK_REQUESTHEADER_STR "requestHeader"
#define RIAK_LINK_REQUESTHEADER_STR_LEN strlen(RIAK_LINK_REQUESTHEADER_STR)


PHP_METHOD(riakLink, __construct);

PHP_METHOD(riakLink, getObject);

PHP_METHOD(riakLink, getTag);

PHPAPI void riak_link__constructor(zval *link_instance, zval *client_instance, zval *bucket_instance, char *key, int key_len, char *tag, int tag_len TSRMLS_DC);

PHPAPI int riak_link_instances_equal(zval *link_instance_a, zval *link_instance_b TSRMLS_DC);

PHPAPI int riak_link_create_link_instance_from_raw_string(zval *client_instance, char *header_str, int header_str_len, zval **link_instance TSRMLS_DC);

#endif
