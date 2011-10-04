#!/bin/bash


# insert some data
curl -v -X PUT -d '{"bar":"baz"}' -H "Content-Type: application/json" http://127.0.0.1:8098/riak/__extRiak-bucket_1/key_1
curl -v -X PUT -d '{"bar":"baz"}' -H "Content-Type: application/json" http://127.0.0.1:8098/riak/__extRiak-bucket_2/key_1
curl -v -X PUT -d '{"bar":"baz"}' -H "Content-Type: application/json" http://127.0.0.1:8098/riak/__extRiak-bucket_3/key_1


