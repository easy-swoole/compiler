//
//  pkcs7.c
//  test
//
//  Created by 张一丰 on 2020/3/20.
//  Copyright © 2020 张一丰. All rights reserved.
//

#include "pkcs7.h"



/**
 * 加密明文如果不是16byte整倍数，则根据PKCS7标准填充至16byte整倍数
 * 参数 p:明文的字符串数组。
 * 参数 plen:明文长度
 * 返回padding后的明文长度
 */
int PKCS7Padding(char *p, int plen)
{
    int plen_after_Padding = 0, remain = 0, padding_num = 0;
    char padding_value = NULL;
    
    if(plen % 16 == 0)
    {
        plen_after_Padding = plen;
        return plen;
    }
    
    if(plen == 0)
    {
        return plen;
    }
    
    if( plen % 16 != 0)
    {
        remain = plen % 16;
        switch (remain)
        {
            case 1:
                padding_num = 15;
                padding_value = 0x0F;
                break;
                
            case 2:
                padding_num = 14;
                padding_value = 0x0E;
                break;
                
            case 3:
                padding_num = 13;
                padding_value = 0x0D;
                break;
                
            case 4:
                padding_num = 12;
                padding_value = 0x0C;
                break;
                
            case 5:
                padding_num = 11;
                padding_value = 0x0B;
                break;
                
            case 6:
                padding_num = 10;
                padding_value = 0x0A;
                break;
                
            case 7:
                padding_num = 9;
                padding_value = 0x09;
                break;
                
            case 8:
                padding_num = 8;
                padding_value = 0x08;
                break;
                
            case 9:
                padding_num = 7;
                padding_value = 0x07;
                break;
                
            case 10:
                padding_num = 6;
                padding_value = 0x06;
                break;
                
            case 11:
                padding_num = 5;
                padding_value = 0x5;
                break;
                
            case 12:
                padding_num = 4;
                padding_value = 0x04;
                break;
                
            case 13:
                padding_num = 3;
                padding_value = 0x03;
                break;
                
            case 14:
                padding_num = 2;
                padding_value = 0x02;
                break;
                
            case 15:
                padding_num = 1;
                padding_value = 0x01;
                break;
        }
    }
    plen_after_Padding = plen + padding_num;
    int i = plen;
    for(; i < plen_after_Padding; i++)
    {
        p[i] = padding_value;
    }
    
    p[plen_after_Padding] = '\0';
    
    
    
    return plen_after_Padding;
}


/**
 * 将解密后的明文填充数据去掉，还原数据
 * int PKCS7Cutting(char *p, int plen)
 * 参数 p:解密后明文的字符串数组。
 * 参数 plen:解密后明文长度
 * 返回cutting后的明文长度
 */
int PKCS7Cutting(char *p, int plen)
{
    int plen_after_cutting;
    
    if(p[plen-1] == 0x01)
    {
        plen_after_cutting = plen -1;
    }
    else if(p[plen-1] == 0x02)
    {
        if(p[plen-2] == 0x02)
            plen_after_cutting = plen -2;
    }
    else if(p[plen-1] == 0x03)
    {
        if((p[plen-2] == 0x03) && (p[plen-3] == 0x03))
            plen_after_cutting = plen -3;
    }
    else if(p[plen-1] == 0x04)
    {
        if((p[plen-2] == 0x04) && (p[plen-3] == 0x04) && (p[plen-4] == 0x04))
            plen_after_cutting = plen -4;
    }
    else if(p[plen-1] == 0x05)
    {
        if((p[plen-2] == 0x05) && (p[plen-3] == 0x05) && (p[plen-4] == 0x05) && (p[plen-5] == 0x05))
            plen_after_cutting = plen -5;
    }
    else if(p[plen-1] == 0x06)
    {
        if((p[plen-2] == 0x06) && (p[plen-3] == 0x06) && (p[plen-4] == 0x06) && (p[plen-5] == 0x06) && (p[plen-6] == 0x06))
            plen_after_cutting = plen -6;
    }
    else if(p[plen-1] == 0x07)
    {
        if((p[plen-2] == 0x07) && (p[plen-3] == 0x07) && (p[plen-4] == 0x07) && (p[plen-5] == 0x07) && (p[plen-6] == 0x07) && (p[plen-7] == 0x07))
            plen_after_cutting = plen -7;
    }
    else if(p[plen-1] == 0x08)
    {
        if((p[plen-2] == 0x08) && (p[plen-3] == 0x08) && (p[plen-4] == 0x08) && (p[plen-5] == 0x08) && (p[plen-6] == 0x08) && (p[plen-7] == 0x08) && (p[plen-8] == 0x08))
            plen_after_cutting = plen -8;
    }
    else if(p[plen-1] == 0x09)
    {
        if((p[plen-2] == 0x09) && (p[plen-3] == 0x09) && (p[plen-4] == 0x09) && (p[plen-5] == 0x09) && (p[plen-6] == 0x09) && (p[plen-7] == 0x09) && (p[plen-8] == 0x09) && (p[plen-9] == 0x09))
            plen_after_cutting = plen -9;
    }
    else if(p[plen-1] == 0x0A)
    {
        if((p[plen-2] == 0x0A) && (p[plen-3] == 0x0A) && (p[plen-4] == 0x0A) && (p[plen-5] == 0x0A) && (p[plen-6] == 0x0A) && (p[plen-7] == 0x0A) && (p[plen-8] == 0x0A) && (p[plen-9] == 0x0A) && (p[plen-10] == 0x0A))
            plen_after_cutting = plen -10;
    }
    else if(p[plen-1] == 0x0B)
    {
        if((p[plen-2] == 0x0B) && (p[plen-3] == 0x0B) && (p[plen-4] == 0x0B) && (p[plen-5] == 0x0B) && (p[plen-6] == 0x0B) && (p[plen-7] == 0x0B) && (p[plen-8] == 0x0B) && (p[plen-9] == 0x0B) && (p[plen-10] == 0x0B) && (p[plen-11] == 0x0B))
            plen_after_cutting = plen -11;
    }
    else if(p[plen-1] == 0x0C)
    {
        if((p[plen-2] == 0x0C) && (p[plen-3] == 0x0C) && (p[plen-4] == 0x0C) && (p[plen-5] == 0x0C) && (p[plen-6] == 0x0C) && (p[plen-7] == 0x0C) && (p[plen-8] == 0x0C) && (p[plen-9] == 0x0C) && (p[plen-10] == 0x0C) && (p[plen-11] == 0x0C) && (p[plen-12] == 0x0C))
            plen_after_cutting = plen -12;
    }
    else if(p[plen-1] == 0x0D)
    {
        if((p[plen-2] == 0x0D) && (p[plen-3] == 0x0D) && (p[plen-4] == 0x0D) && (p[plen-5] == 0x0D) && (p[plen-6] == 0x0D) && (p[plen-7] == 0x0D) && (p[plen-8] == 0x0D) && (p[plen-9] == 0x0D) && (p[plen-10] == 0x0D) && (p[plen-11] == 0x0D) && (p[plen-12] == 0x0D) && (p[plen-13] == 0x0D))
            plen_after_cutting = plen -13;
    }
    else if(p[plen-1] == 0x0E)
    {
        if((p[plen-2] == 0x0E) && (p[plen-3] == 0x0E) && (p[plen-4] == 0x0E) && (p[plen-5] == 0x0E) && (p[plen-6] == 0x0E) && (p[plen-7] == 0x0E) && (p[plen-8] == 0x0E) && (p[plen-9] == 0x0E) && (p[plen-10] == 0x0E) && (p[plen-11] == 0x0E) && (p[plen-12] == 0x0E) && (p[plen-13] == 0x0E) && (p[plen-14] == 0x0E))
            plen_after_cutting = plen -14;
    }
    else if(p[plen-1] == 0x0F)
    {
        if((p[plen-2] == 0x0F) && (p[plen-3] == 0x0F) && (p[plen-4] == 0x0F) && (p[plen-5] == 0x0F) && (p[plen-6] == 0x0F) && (p[plen-7] == 0x0F) && (p[plen-8] == 0x0F) && (p[plen-9] == 0x0F) && (p[plen-10] == 0x0F) && (p[plen-11] == 0x0F) && (p[plen-12] == 0x0F) && (p[plen-13] == 0x0F) && (p[plen-14] == 0x0F) && (p[plen-15] == 0x0F))
            plen_after_cutting = plen -15;
    }
    else
    {
        plen_after_cutting = plen;
    }
    
    return plen_after_cutting;
    
}
