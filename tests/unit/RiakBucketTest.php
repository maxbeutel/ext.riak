<?php

class RiakBucketTest extends PHPUnit_Framework_TestCase
{
    public function testSimpleGetter()
    {
        $c = new riakClient();
        $b = $c->bucket('__extRiak-bucket_3');
        
        $this->assertSame('__extRiak-bucket_3', $b->getName());
    }
    
    public function testGetReadWriteValuesAreTakenFromClientIfNotDefined()
    {
        $c = new riakClient();
        $b = $c->bucket('__extRiak-bucket_3');        
        
        $this->assertSame($c->getR(), $b->getR());
        $this->assertSame($c->getW(), $b->getW());
        $this->assertSame($c->getDW(), $b->getDW());
        
        $b->setR(5);
        $this->assertNotSame($c->getR(), $b->getR());
        $this->assertSame(5, $b->getR());
        
        $b->setW(5);
        $this->assertNotSame($c->getW(), $b->getW());
        $this->assertSame(5, $b->getW());
        
        $b->setDW(5);
        $this->assertNotSame($c->getDW(), $b->getDW());
        $this->assertSame(5, $b->getDW());
    }
        
    public function testGetKeys()
    {
        $c = new riakClient();
        $b = $c->bucket('__extRiak-bucket_3');
        
        $this->assertSame(array('key_1'), $b->getKeys());
    }
    
    public function testGetProperties()
    {
        $c = new riakClient();
        $b = $c->bucket('__extRiak-bucket_3');
        
        $p = $b->getProperties();
        $this->assertInternalType('array', $p);
    }
    
    public function testNewJsonObject()
    {
        $c = new riakClient();
        $b = $c->bucket('__extRiak-bucket_1');
        $o = $b->newObject('key', array());
        
        $this->assertInstanceOf('riakObject', $o);
        $this->assertSame('text/json', $o->getContentType());
        $this->assertFalse($o->exists());
    }
    
    public function testNewBinary()
    {
        $c = new riakClient();
        $b = $c->bucket('__extRiak-bucket_1');
        $o = $b->newObject('key', 'data', 'text/plain');
        
        $this->assertInstanceOf('riakObject', $o);
        $this->assertSame('text/plain', $o->getContentType());
        $this->assertFalse($o->exists());
    }    
    
    public function testGetSimpleObject()
    {
        $client = new riakClient();
        $bucket = $client->bucket('__extRiak-bucket_1');
        $object = $bucket->getObject('key_1');
        
        $this->assertInstanceOf('riakObject', $object);
    }
}
















