<?php

class RiakClientTest extends PHPUnit_Framework_TestCase
{
    public function testDefaultValues()
    {
        $c = new riakClient();
        
        $this->assertSame(2, $c->getR());
        $this->assertSame(2, $c->getW());
        $this->assertSame(2, $c->getDW());
    }
    
    public function testSimpleSetter()
    {
        $c = new riakClient();
        
        $this->assertSame(2, $c->getR());
        $c->setR(5);
        $this->assertSame(5, $c->getR());
        
        $this->assertSame(2, $c->getW());
        $c->setW(5);
        $this->assertSame(5, $c->getW());
        
        $this->assertSame(2, $c->getDW());
        $c->setDW(5);
        $this->assertSame(5, $c->getDW());
    }
    
    public function testIsAlive()
    {
        $c = new riakClient();
        $this->assertTrue($c->isAlive());
    }
    
    public function testClientIdDiffersPerInstance()
    {
        $c1 = new riakClient();
        $c2 = new riakClient();
        
        $this->assertNotSame($c1->getClientId(), $c2->getClientId());
    }
    
    public function testBuckets()
    {
        $c = new riakClient();
        
        $b = $c->buckets();
        $this->assertSame(3, count($b));   
    }
    
    public function testBucket()
    {
        $c = new riakClient();
        $b = $c->bucket('Foo');
        $this->assertInstanceOf('riakBucket', $b);
    }
}
