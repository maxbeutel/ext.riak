
#ifndef RIAK_OBJECT_H
#define RIAK_OBJECT_H

#define RIAK_OBJECT_CLIENT "client"
#define RIAK_OBJECT_CLIENT_LEN strlen(RIAK_OBJECT_CLIENT)

#define RIAK_OBJECT_BUCKET "bucket"
#define RIAK_OBJECT_BUCKET_LEN strlen(RIAK_OBJECT_BUCKET)

#define RIAK_OBJECT_KEY "key"
#define RIAK_OBJECT_KEY_LEN strlen(RIAK_OBJECT_KEY)

#define RIAK_OBJECT_JSONIZE "jsonize"
#define RIAK_OBJECT_JSONIZE_LEN strlen(RIAK_OBJECT_JSONIZE)

#define RIAK_OBJECT_HEADERS "headers"
#define RIAK_OBJECT_HEADERS_LEN strlen(RIAK_OBJECT_HEADERS)

#define RIAK_OBJECT_LINKS "links"
#define RIAK_OBJECT_LINKS_LEN strlen(RIAK_OBJECT_LINKS)

#define RIAK_OBJECT_SIBLINGS "siblings"
#define RIAK_OBJECT_SIBLINGS_LEN strlen(RIAK_OBJECT_SIBLINGS)

#define RIAK_OBJECT_EXISTS "exists"
#define RIAK_OBJECT_EXISTS_LEN strlen(RIAK_OBJECT_EXISTS)

zend_class_entry *riak_ce_riakObject;

PHP_METHOD(riakObject, __construct);

#endif
