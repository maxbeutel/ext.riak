<?php

class RiakObjectTest extends PHPUnit_Framework_TestCase
{
    public function testEmptySiblings()
    {
        $c = new riakClient();
        $b = $c->bucket('__extRiak-bucket_2');
        $o = $b->getObject('key_1');
        
        $this->assertFalse($o->hasSiblings());
        $this->assertSame(0, $o->getSiblingCount());
    }
}
