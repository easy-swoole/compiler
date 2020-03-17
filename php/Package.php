<?php


namespace EasySwoole\Compiler;


class Package
{
    /**
     * @var License
     */
    protected $license;
    /**
     * @var string
     */
    protected $code = '';

    /**
     * @return License
     */
    public function getLicense(): ?License
    {
        return $this->license;
    }

    /**
     * @param License $license
     */
    public function setLicense(?License $license): void
    {
        $this->license = $license;
    }

    /**
     * @return string
     */
    public function getCode(): string
    {
        return $this->code;
    }

    /**
     * @param string $code
     */
    public function setCode(string $code): void
    {
        $this->code = $code;
    }
}