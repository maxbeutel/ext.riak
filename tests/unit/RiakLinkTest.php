<?php

class RiakLinkTest extends PHPUnit_Framework_TestCase
{
    protected $clientMock;
    protected $bucketMock;
    
    public function setUp()
    {
        $this->clientMock = $this->getMock('riakClient', array(), array(), '', false, false);
        $this->bucketMock = $this->getMock('riakBucket', array(), array(), '', false, false);
    }
    
    public function testCreatingWithTag()
    {
        $link = new riakLink($this->clientMock, $this->bucketMock, 'LINK_TO_KEY', 'LINK_TAG');
    }
    
    public function testCreatingWithoutTag()
    {
        $link = new riakLink($this->clientMock, $this->bucketMock, 'LINK_TO_KEY');
    }
    
    public function testGetTag()
    {
        $link = new riakLink($this->clientMock, $this->bucketMock, 'LINK_TO_KEY');
        $this->assertNull($link->getTag());
        
        $link = new riakLink($this->clientMock, $this->bucketMock, 'LINK_TO_KEY', 'LINK_TAG');
        $this->assertEquals('LINK_TAG', $link->getTag());
    }
    
    public function testGetObject()
    {
        $client = new riakClient();
        $bucket = new riakBucket($client, '__extRiak-bucket_1');
        
        
        $link = new riakLink($client, $bucket, 'key_1');
        $object = $link->getObject();
        $this->assertInstanceOf('riakObject', $object);
    }
}
