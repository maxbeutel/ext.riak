<?php

class RiakObjectTest extends PHPUnit_Framework_TestCase
{
    public function testEmptySiblings()
    {
        $client = new riakClient();
        $bucket = $client->bucket('__extRiak-bucket_2');
        $object = $bucket->getObject('key_1');
        
        $this->assertInstanceOf('riakObject', $object);
        $this->assertFalse($object->hasSiblings());
        $this->assertSame(0, $object->getSiblingCount());
    }
    
    public function testNewObject()
    {
        $client = new riakClient();
        $bucket = $client->bucket('__extRiak-bucket_2');
        $object = $bucket->newObject('__new_key');
        
        $this->assertInstanceOf('riakObject', $object);
        $this->assertNull($object->getData());
        $this->assertEquals('text/json', $object->getContentType());
    }
    
    public function testStore()
    {
        $client = new riakClient();
        $bucket = $client->bucket('__extRiak-bucket_2');
        $object = $bucket->newObject('__new_key', array(1, 2, 3));
        
        $object->store();
    }
}
