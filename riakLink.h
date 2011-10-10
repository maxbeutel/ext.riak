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


PHP_METHOD(riakLink, __construct);

PHP_METHOD(riakLink, getObject);

PHP_METHOD(riakLink, getTag);

#endif