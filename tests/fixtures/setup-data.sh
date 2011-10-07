#!/bin/bash


# clear data
if [ "$1" = "clear" ]
then 
    SILENT=""
    
    if [ "$2" = "silent" ]
    then 
        SILENT=" > /dev/null 2>&1"
    fi    
   
    echo "clearing dummy data..."
    
    eval "curl -X DELETE http://127.0.0.1:8098/riak/__extRiak-bucket_1/key_1"$SILENT
    eval "curl -X DELETE http://127.0.0.1:8098/riak/__extRiak-bucket_2/key_1"$SILENT
    eval "curl -X DELETE http://127.0.0.1:8098/riak/__extRiak-bucket_3/key_1"$SILENT
fi


# insert data
if [ "$1" = "insert" ]
then 
    echo "inserting dummy data..."
    
    curl -X PUT -d '{"bar":"baz"}' -H "Content-Type: application/json" http://127.0.0.1:8098/riak/__extRiak-bucket_1/key_1
    curl -X PUT -d '{"bar":"baz"}' -H "Content-Type: application/json" http://127.0.0.1:8098/riak/__extRiak-bucket_2/key_1
    curl -X PUT -d '{"bar":"baz"}' -H "Content-Type: application/json" http://127.0.0.1:8098/riak/__extRiak-bucket_3/key_1
fi






