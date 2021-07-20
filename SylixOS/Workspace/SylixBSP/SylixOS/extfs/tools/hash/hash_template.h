/*
 * list_template.h
 *
 *  Created on: Jun 2, 2021
 *      Author: Administrator
 */

#ifndef SYLIXOS_EXTFS_TOOLS_HASH_HASH_TEMPLATE_H_
#define SYLIXOS_EXTFS_TOOLS_HASH_HASH_TEMPLATE_H_

#include "SylixOS.h"
UINT32 hash(PUCHAR pucString, UINT32 uiMaxSize) {
    UINT32 uiHash = 5381;
    UCHAR uChar;
    UINT i = 0;
    while ((uChar = pucString[i++]) && i < uiMaxSize) {
        uiHash = (uiHash * 33) ^ uChar;
    }
    return uiHash;
}
#endif /* SYLIXOS_EXTFS_TOOLS_HASH_HASH_TEMPLATE_H_ */