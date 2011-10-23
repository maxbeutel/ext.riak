<?php

        $client = new riakClient();
        $bucket = $client->bucket('__extRiak-bucket_1');
        $object = $bucket->getObject('key_1');

print_r($object);

