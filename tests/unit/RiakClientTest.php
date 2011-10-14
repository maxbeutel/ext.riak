<?php

class RiakClientTest extends PHPUnit_Framework_TestCase
{
    public function testDefaultValues()
    {
        $client = new riakClient();
        
        $this->assertSame(2, $client->getR());
        $this->assertSame(2, $client->getW());
        $this->assertSame(2, $client->getDW());
    }
    
    public function testSimpleSetter()
    {
        $client = new riakClient();
        
        $this->assertSame(2, $client->getR());
        $client->setR(5);
        $this->assertSame(5, $client->getR());
        
        $this->assertSame(2, $client->getW());
        $client->setW(5);
        $this->assertSame(5, $client->getW());
        
        $this->assertSame(2, $client->getDW());
        $client->setDW(5);
        $this->assertSame(5, $client->getDW());
    }
    
    public function testIsAlive()
    {
        $client = new riakClient();
        $this->assertTrue($client->isAlive());
    }
    
    public function testClientIdDiffersPerInstance()
    {
        $client1 = new riakClient();
        $client2 = new riakClient();
        
        $this->assertNotSame($client1->getClientId(), $client2->getClientId());
    }
    
    public function testBuckets()
    {
        $client = new riakClient();
        
        $buckets = $client->buckets();
        $this->assertSame(3, count($buckets));   
    }
    
    public function testBucket()
    {
        $client = new riakClient();
        $bucket = $client->bucket('Foo');
        $this->assertInstanceOf('riakBucket', $bucket);
    }
}
