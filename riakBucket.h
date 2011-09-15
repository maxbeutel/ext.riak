
#ifndef RIAK_BUCKET_H
#define RIAK_BUCKET_H

zend_class_entry *riak_ce_riakBucket;

#define RIAK_BUCKET_CLIENT "client"
#define RIAK_BUCKET_CLIENT_LEN strlen(RIAK_BUCKET_CLIENT)

#define RIAK_BUCKET_NAME "name"
#define RIAK_BUCKET_NAME_LEN strlen(RIAK_BUCKET_NAME)

PHP_METHOD(riakBucket, __construct);

PHP_METHOD(riakBucket, getName);

PHP_METHOD(riakBucket, getR);
PHP_METHOD(riakBucket, setR);

PHP_METHOD(riakBucket, getW);
PHP_METHOD(riakBucket, setW);

PHP_METHOD(riakBucket, getDW);
PHP_METHOD(riakBucket, setDW);

PHP_METHOD(riakBucket, newObject);
PHP_METHOD(riakBucket, newBinary);

PHP_METHOD(riakBucket, get);
PHP_METHOD(riakBucket, getBinary);

PHP_METHOD(riakBucket, setNVal);
PHP_METHOD(riakBucket, getNVal);

PHP_METHOD(riakBucket, setAllowMultiples);
PHP_METHOD(riakBucket, getAllowMultiples);

PHP_METHOD(riakBucket, setProperty);
PHP_METHOD(riakBucket, getProperty);

PHP_METHOD(riakBucket, setProperties);
PHP_METHOD(riakBucket, getProperties);

PHP_METHOD(riakBucket, getKeys);

#endif