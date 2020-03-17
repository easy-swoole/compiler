<?php


namespace EasySwoole\Compiler;


class License
{
    protected $ip = [];
    protected $mac = [];
    protected $expire = 0;

    /**
     * @return array
     */
    public function getIp(): array
    {
        return $this->ip;
    }

    /**
     * @param array $ip
     */
    public function setIp(array $ip): void
    {
        $this->ip = $ip;
    }

    /**
     * @return array
     */
    public function getMac(): array
    {
        return $this->mac;
    }

    /**
     * @param array $mac
     */
    public function setMac(array $mac): void
    {
        $this->mac = $mac;
    }

    /**
     * @return int
     */
    public function getExpire(): int
    {
        return $this->expire;
    }

    /**
     * @param int $expire
     */
    public function setExpire(int $expire): void
    {
        $this->expire = $expire;
    }
}