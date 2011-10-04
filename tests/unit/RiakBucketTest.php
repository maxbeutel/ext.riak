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
    
    public function testGetSetNVal()
    {
        $c = new riakClient();
        $b = $c->bucket('__extRiak-bucket_3');
        
        $this->assertSame(3, $b->getNVal());
        
        $b->setNVal(5);
        $this->assertSame(5, $b->getNVal());
        
        $b->setNVal(3);
    }
    
    public function testGetSetAllowMultiples()
    {
        $c = new riakClient();
        $b = $c->bucket('__extRiak-bucket_3');
        
        $this->assertFalse($b->getAllowMultiples());
        
        $b->setAllowMultiples(true);
        $this->assertTrue($b->getAllowMultiples());
        
        $b->setAllowMultiples(false);
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
    
    public function testNewObject()
    {
        $c = new riakClient();
        $b = $c->bucket('__extRiak-bucket_1');
        $o = $b->newObject('key', array());
        
        $this->assertInstanceOf('riakObject', $o);
        $this->assertSame('text/json', $o->getContentType());
    }
    
    public function testNewBinary()
    {
        $c = new riakClient();
        $b = $c->bucket('__extRiak-bucket_1');
        $o = $b->newBinary('key', 'data', 'text/plain');
        
        $this->assertInstanceOf('riakObject', $o);
        $this->assertSame('text/plain', $o->getContentType());
    }    
}
