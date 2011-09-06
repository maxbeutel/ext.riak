
#ifndef RIAK_BUCKET_H
#define RIAK_BUCKET_H

zend_class_entry *riak_ce_riakBucket;

#define RIAK_BUCKET_CLIENT "client"
#define RIAK_BUCKET_CLIENT_LEN strlen(RIAK_BUCKET_CLIENT)

#define RIAK_BUCKET_NAME "name"
#define RIAK_BUCKET_NAME_LEN strlen(RIAK_BUCKET_NAME)

PHP_METHOD(riakBucket, __construct);

PHP_METHOD(riakBucket, getName);

#endif