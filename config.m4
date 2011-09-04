dnl lines starting with "dnl" are comments

PHP_ARG_ENABLE(riak, whether to enable Riak extension, [ --enable-riak Enable Riak extension])

if test "$PHP_RIAK" != "no"; then 
    paths="/usr /usr/local /sw"
        
    for path in $paths; do
    if test -x "$path/bin/curl-config"; then
    CURL_CONFIG=$path/bin/curl-config
    break
    fi
    done
        
    test -z "$CURL_CONFIG" && AC_MSG_ERROR(Cannot find libcurl)
    
    RIAK_LIBS=$($CURL_CONFIG --libs)    
    RIAK_INCS=$($CURL_CONFIG --cflags)
    
    PHP_EVAL_LIBLINE($RIAK_LIBS, RIAK_SHARED_LIBADD)
    PHP_EVAL_INCLINE($RIAK_INCS)

    PHP_SUBST(RIAK_SHARED_LIBADD)
    PHP_NEW_EXTENSION(riak, php_riak.c riakClient.c riakBucket.c, $ext_shared)
fi


