//
//  pkcs7.h
//  test
//
//  Created by 张一丰 on 2020/3/20.
//  Copyright © 2020 张一丰. All rights reserved.
//

#ifndef pkcs7_h
#define pkcs7_h

#include <stdio.h>
#define MAX_PKCS7_LEN  1024*32
int PKCS7Padding(char *p, int plen);
int PKCS7Cutting(char *p, int plen);
#endif /* pkcs7_h */
