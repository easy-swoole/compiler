<?php


namespace EasySwoole\Complier;


class License
{
    private $expire;
    private $mac = [];
    private $ip = [];
    private $pubicKey;

    /**
     * @return mixed
     */
    public function getExpire()
    {
        return $this->expire;
    }

    /**
     * @param mixed $expire
     */
    public function setExpire($expire)
    {
        $this->expire = $expire;
    }

    /**
     * @return array
     */
    public function getMac()
    {
        return $this->mac;
    }

    /**
     * @param array $mac
     */
    public function setMac($mac)
    {
        $this->mac = $mac;
    }

    /**
     * @return array
     */
    public function getIp()
    {
        return $this->ip;
    }

    /**
     * @param array $ip
     */
    public function setIp($ip)
    {
        $this->ip = $ip;
    }

    /**
     * @return mixed
     */
    public function getPubicKey()
    {
        return $this->pubicKey;
    }

    /**
     * @param mixed $pubicKey
     */
    public function setPubicKey($pubicKey)
    {
        $this->pubicKey = $pubicKey;
    }
}