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
        $link = new riakLink($this->clientMock, $this->bucketMock, "LINK_TO_KEY", "LINK_TAG");
    }
    
    public function testCreatingWithoutTag()
    {
        $link = new riakLink($this->clientMock, $this->bucketMock, "LINK_TO_KEY");
    }
    
    public function testGetTag()
    {
        
    }
}
