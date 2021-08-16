/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: hoitFsTree.c
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 03 月 28 日
**
** 描        述: JFFS2 Like Frag Tree实现
*********************************************************************************************************/
#include "hoitFsTree.h"
#include "hoitFsFDLib.h"
#include "hoitFsCache.h"
#include "driver/mtd/nor/nor.h"

#ifdef FT_TEST
PHOIT_FULL_DNODE __hoit_truncate_full_dnode(PHOIT_VOLUME pfs, PHOIT_FULL_DNODE pFDnode, UINT uiOffset, UINT uiSize){
    PHOIT_FULL_DNODE pFDNode = (PHOIT_FULL_DNODE)hoit_malloc(pfs, sizeof(HOIT_FULL_DNODE));
    pFDNode->HOITFD_length = uiSize;
    pFDNode->HOITFD_offset = uiOffset;
    return pFDNode;
}

BOOL __hoit_delete_full_dnode(PHOIT_VOLUME pfs, PHOIT_FULL_DNODE pFDnode, BOOL bDoDelete){
    if(!bDoDelete){
        hoit_free(pfs, pFDnode, sizeof(HOIT_FULL_DNODE));
    }
    return LW_TRUE;
}


#endif //FT_TEST

#define FT_GET_KEY(pFTn)            pFTn->pRbn.iKey
#define FT_LEFT_CHILD(pFTn)         (PHOIT_FRAG_TREE_NODE)pFTnRoot->pRbn.pRbnLeft
#define FT_RIGHT_CHILD(pFTn)        (PHOIT_FRAG_TREE_NODE)pFTnRoot->pRbn.pRbnRight
#define RB_GUARD(pFTTree)           (pFTTree->pRbTree->pRbnGuard)

#define MAX(a, b)                   ((a) > (b) ? (a) : (b))
#define MIN(a, b)                   ((a) < (b) ? (a) : (b))    
#define FT_GET_FIRST_NODE(pFTTree)  (__hoitFragTreeGetMinimum(pFTTree, (PHOIT_FRAG_TREE_NODE)pFTTree->pRbTree->pRbnRoot))

PHOIT_FRAG_TREE_NODE __hoitFragTreeGetMaximum(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTnRoot){
    PHOIT_RB_NODE       pRbnTraverse; 
    pRbnTraverse    =   &pFTnRoot->pRbn;
    if(pRbnTraverse != RB_GUARD(pFTTree)){    
        while (pRbnTraverse->pRbnRight != RB_GUARD(pFTTree))
        {
            pRbnTraverse = pRbnTraverse->pRbnRight;
        }
    }
    return (PHOIT_FRAG_TREE_NODE)pRbnTraverse;
}


PHOIT_FRAG_TREE_NODE __hoitFragTreeGetMinimum(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTnRoot){
    PHOIT_RB_NODE       pRbnTraverse; 
    pRbnTraverse    =   &pFTnRoot->pRbn;
    if(pRbnTraverse != RB_GUARD(pFTTree)){    
        while (pRbnTraverse->pRbnLeft != RB_GUARD(pFTTree))
        {
            pRbnTraverse = pRbnTraverse->pRbnLeft;
        }
    }
    return (PHOIT_FRAG_TREE_NODE)pRbnTraverse;
}

PHOIT_FRAG_TREE_NODE __hoitFragTreeGetSuccessor(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTnRoot){
    PHOIT_RB_NODE pRbnTraverse = RB_GUARD(pFTTree);
    PHOIT_RB_NODE pRbn;

    pRbn = &pFTnRoot->pRbn;
    if(pRbn != RB_GUARD(pFTTree)){
        if(pRbn->pRbnRight != RB_GUARD(pFTTree)){
            return __hoitFragTreeGetMinimum(pFTTree, FT_RIGHT_CHILD(pFTnRoot));
        }

        pRbnTraverse = pRbn->pRbnParent;
        while (pRbnTraverse != RB_GUARD(pFTTree) && pRbn == pRbnTraverse->pRbnRight)
        {
            pRbn = pRbnTraverse;
            pRbnTraverse = pRbnTraverse->pRbnParent;
        }
    }
    return (PHOIT_FRAG_TREE_NODE)pRbnTraverse;
}
#ifndef FT_OBSOLETE_TREE_LIST
/*********************************************************************************************************
** 函数名称: __hoitFragTreeCollectRangeHelper
** 功能描述: 搜集iKeyLow至iKeyHigh的FragTree节点
** 输　入  : pFTlistHeader          链表头
**          pFTTree               FragTree
**          pFTnRoot              根节点
**          iKeyLow               低键值
**          iKeyHigh              高键值
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoitFragTreeCollectRangeHelper(PHOIT_FRAG_TREE_LIST_HEADER pFTlistHeader,
                                      PHOIT_FRAG_TREE pFTTree, 
                                      PHOIT_FRAG_TREE_NODE pFTnRoot, 
                                      INT32 iKeyLow, 
                                      INT32 iKeyHigh){
    PHOIT_FRAG_TREE_LIST_NODE   pFTlistNode;
    PHOIT_FRAG_TREE_NODE        pFTSuccessor;
    BOOL                        bHasSuccessor = LW_TRUE;
    BOOL                        bIsInRange = LW_FALSE;


    if(&pFTnRoot->pRbn == RB_GUARD(pFTTree)){
        return;
    }
    __hoitFragTreeCollectRangeHelper(pFTlistHeader, 
                                     pFTTree, 
                                     FT_LEFT_CHILD(pFTnRoot), 
                                     iKeyLow,
                                     iKeyHigh);
    pFTSuccessor =  __hoitFragTreeGetSuccessor(pFTTree, pFTnRoot);
    if(&pFTSuccessor->pRbn == RB_GUARD(pFTTree)){
        bHasSuccessor = LW_FALSE;
    }
    /* 
        Case 1:         [key1    iKeyLow    key2 ...]             下界：key1
        Case 2:         [key1    iKeyLow]                         下界：key1
    */
    if((bHasSuccessor && (FT_GET_KEY(pFTnRoot) <= iKeyLow && FT_GET_KEY(pFTSuccessor) > iKeyLow)) ||
       (!bHasSuccessor && FT_GET_KEY(pFTnRoot) <= iKeyLow)){                                          /* iKey < iKeyLow 锟斤拷 iKey锟侥猴拷探诘锟斤拷iKey > iKeyLow*/
        bIsInRange = LW_TRUE;
        pFTlistHeader->uiLowBound = FT_GET_KEY(pFTnRoot);
    }
    /* 
        Case 3:         [iKeyLow   key1   key2 ...]               下界：key1
        --------------------------------------------------------------------
        Case 1:         [iKeyLow   key1   iKeyHigh]               下界：key1
        Case 2:         [iKeyLow   key1   iKeyHigh    key2]       上界：key2
        Case 3:         [ikey      iKeyLow   iKeyHigh]            下界：key1
    */
    else if (FT_GET_KEY(pFTnRoot) > iKeyLow && FT_GET_KEY(pFTnRoot) <= iKeyHigh){                     /* iKey > iKeyLow 锟斤拷 iKey < iKeyHigh */
        bIsInRange = LW_TRUE;
        if(pFTlistHeader->uiLowBound == INT_MIN){
            pFTlistHeader->uiLowBound = FT_GET_KEY(pFTnRoot);
        }
        if(bHasSuccessor){
            if(FT_GET_KEY(pFTSuccessor) > iKeyHigh){
                pFTlistHeader->uiHighBound =  FT_GET_KEY(pFTnRoot);
            }
        }
        else{
            pFTlistHeader->uiHighBound = FT_GET_KEY(pFTnRoot);
        }
    }

    if(bIsInRange){
        pFTlistNode = newFragTreeListNode(pFTnRoot);
        hoitFragTreeListInsertNode(pFTlistHeader, pFTlistNode);
        pFTlistHeader->uiNCnt++;
#ifdef FT_DEBUG
        printf("Collecting Node %d \n", pFTlistNode->pFTn->pRbn.iKey);
#endif // FT_DEBUG
    }
    if (FT_GET_KEY(pFTnRoot) > iKeyHigh)                                                /* 剪枝 */
    {
        return;
    }
    __hoitFragTreeCollectRangeHelper(pFTlistHeader, 
                                     pFTTree, 
                                     FT_RIGHT_CHILD(pFTnRoot), 
                                     iKeyLow,
                                     iKeyHigh);
}   
/*********************************************************************************************************
** 函数名称: hoitFragTreeCollectRange
** 功能描述: 在FragTree中搜集[i, j]节点，其中 i <= iKeyLow, j >= iKeyHigh
** 输　入  : pFTTree    FragTree
**          iKeyLow       低键值
**          iKeyHigh      高键值
** 输　出  : 搜集链表头
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_FRAG_TREE_LIST_HEADER  hoitFragTreeCollectRange(PHOIT_FRAG_TREE pFTTree, INT32 iKeyLow, INT32 iKeyHigh){
    PHOIT_FRAG_TREE_LIST_HEADER     pFTlistHeader;
    pFTlistHeader = hoitFragTreeListInit();
    __hoitFragTreeCollectRangeHelper(pFTlistHeader, 
                                     pFTTree, 
                                     (PHOIT_FRAG_TREE_NODE)pFTTree->pRbTree->pRbnRoot,
                                     iKeyLow,
                                     iKeyHigh);
    if(pFTlistHeader->uiHighBound == INT_MAX){
        pFTlistHeader->uiHighBound = pFTlistHeader->uiLowBound;
    }
#ifdef FT_DEBUG
    printf("Collecting Over\n");
#endif //FT_DEBUG
    return pFTlistHeader;
}
#endif /* not FT_OBSOLETE_TREE_LIST */
/*********************************************************************************************************
** 函数名称: __hoitFragTreeConquerNode
** 功能描述: 分四种情况征服一个节点
** 输　入  : pFTTree                 FragTree
**          pFTn                   待征服节点
**          uiConquerorLow          征服者上界
**          uiConquerorHigh         征服者下界
**          pFTnNew                 生成的新节点
**          uiCase                  返回征服的类型
**          bDoDelete               是否删除RawInfo的内容
** 输　出  : 是否能够被征服
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoitFragTreeConquerNode(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTn, 
                               UINT32 uiConquerorLow, UINT32 uiConquerorHigh, PHOIT_FRAG_TREE_NODE *pFTnNew, 
                               UINT8 * uiCase, BOOL bDoDelete) {
    PHOIT_FULL_DNODE          pFDNodeNew;
    PHOIT_FRAG_TREE_LIST_NODE pFTlistNodeNew;
    

    UINT32                    uiLeftRemainSize;
    UINT32                    uiRightRemainSize;

    UINT32                    uiCurLow;
    UINT32                    uiCurHigh;

    BOOL                      bIsOverlay;

    uiCurLow                  = pFTn->uiOfs;
    uiCurHigh                 = uiCurLow + pFTn->uiSize == 0 ? 0 : uiCurLow + pFTn->uiSize - 1;
    bIsOverlay                = MAX(uiCurLow, uiConquerorLow) <= MIN(uiCurHigh, uiConquerorHigh);
    *uiCase                   = -1;
    *pFTnNew                  = LW_NULL;



    if(bIsOverlay){                                                                     /* 检测到覆盖，政府开始 */
        /* 
            |-------|-------------|-------|
            ^       ^             ^       ^
            Cur  Conqueror       Cur    Conqueror  
        */
        if(uiCurLow < uiConquerorLow && uiCurHigh <= uiConquerorHigh){                  /* 情况1 */
            uiLeftRemainSize = uiConquerorLow - uiCurLow;
            pFTn->uiSize = uiLeftRemainSize;
            pFTn->pFDnode->HOITFD_length = uiLeftRemainSize;
            *uiCase = 1;
        }
        /* 
            |-------|-------------|-------|
            ^       ^             ^       ^
            Cur  Conqueror      Conqueror  Cur
        */
        else if (uiCurLow < uiConquerorLow && uiCurHigh > uiConquerorHigh)              /* 情况2 */
        {
            uiLeftRemainSize = uiConquerorLow - uiCurLow ;
            uiRightRemainSize = uiCurHigh - uiConquerorHigh;

            pFDNodeNew = __hoit_truncate_full_dnode(pFTTree->pfs,                       /* 截取(ConquerorHigh, CurHigh]的节点，创建新节点 */
                                                    pFTn->pFDnode,
                                                    uiConquerorHigh - uiCurLow + 1,
                                                    uiRightRemainSize);

            pFTn->uiSize = uiLeftRemainSize;                                            /* 设置被征服节点的大小 */
            pFTn->pFDnode->HOITFD_length = uiLeftRemainSize;                            /* 更新FDNode的大小 */

            *pFTnNew = newHoitFragTreeNode(pFTTree->pfs, pFDNodeNew, pFDNodeNew->HOITFD_length,       /* 生成FragTree节点 */
                                           pFDNodeNew->HOITFD_offset, pFDNodeNew->HOITFD_offset);

            hoitFragTreeInsertNode(pFTTree, *pFTnNew);                                   /* 把该节点放入FragTree中 */
            *uiCase = 2;
        }
        /* 
                |-------|-------------|-------|
                ^       ^             ^       ^
            Conqueror  Cur       Conqueror  Cur
        */
        else if (uiCurLow >= uiConquerorLow && uiCurHigh > uiConquerorHigh)             /* 情况3 */
        {
            uiRightRemainSize = uiCurHigh - uiConquerorHigh;
            pFDNodeNew = __hoit_truncate_full_dnode(pFTTree->pfs,
                                                    pFTn->pFDnode,
                                                    uiConquerorHigh - uiCurLow + 1,
                                                    uiRightRemainSize);
            *pFTnNew = newHoitFragTreeNode(pFTTree->pfs, pFDNodeNew, pFDNodeNew->HOITFD_length,       /* 根据pFDNodeNew生成新的FragTree节点 */
                                           pFDNodeNew->HOITFD_offset, pFDNodeNew->HOITFD_offset);
            hoitFragTreeInsertNode(pFTTree, *pFTnNew);
            hoitFragTreeDeleteNode(pFTTree, pFTn, bDoDelete);                            /* 删除pFTn节点，因为[CurLow, ConquerorHigh]已经被征服 */
            *uiCase = 3;
        }
        /* 
            |-------|-------------|-------|
            ^       ^             ^       ^
        Conqueror  Cur           Cur    Conqueror
        */
        else if (uiCurLow >= uiConquerorLow && uiCurHigh <= uiConquerorHigh)            /* 情况4 */
        {
            hoitFragTreeDeleteNode(pFTTree, pFTn, bDoDelete);                            /* 直接删除pFTn即可 */
            *uiCase = 4;
        }
    }
    return bIsOverlay;
}
/*********************************************************************************************************
** 函数名称: hoitInitFragTree
** 功能描述: 初始化FragTree
** 输　入  : pfs    HoitFS设备头
** 输　出  : FragTree数据结构
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_FRAG_TREE hoitInitFragTree(PHOIT_VOLUME pfs){
    PHOIT_FRAG_TREE         pFTTree;

    pFTTree = (PHOIT_FRAG_TREE)hoit_malloc(pfs, sizeof(HOIT_FRAG_TREE));
    pFTTree->pRbTree = hoitRbInitTree();
    pFTTree->uiNCnt = 0;
    pFTTree->pfs = pfs;
    pFTTree->uiMemoryBytes = 0;

    /* //! Add By PYQ 2021-08-15 我们不能修改Rb的lib_malloc为hoit_malloc */
    pfs->HOITFS_ulCurBlk += sizeof(HOIT_RB_NODE);
    pfs->HOITFS_ulCurBlk += sizeof(HOIT_RB_NODE);
    pfs->HOITFS_ulCurBlk += sizeof(HOIT_RB_TREE);
    return pFTTree;
}
/*********************************************************************************************************
** 函数名称: hoitFragTreeSearchNode
** 功能描述: 在FragTree中根据键值搜索目标节点，已弃用
** 输　入  : pFTTree    FragTree
**          iKey       键值
** 输　出  : 被插入的节点
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_FRAG_TREE_NODE hoitFragTreeSearchNode(PHOIT_FRAG_TREE pFTTree, INT32 iKey){
    PHOIT_RB_NODE           pRbn; 
    PHOIT_FRAG_TREE_NODE    pFTn;

    pRbn = hoitRbSearchNode(pFTTree->pRbTree, iKey);
    pFTn = (PHOIT_FRAG_TREE_NODE)(pRbn);  
    return pFTn;
}
/*********************************************************************************************************
** 函数名称: hoitFragTreeGetLastNode
** 功能描述: 获取FragTree的最后一个节点
** 输　入  : pFTTree    FragTree
**          iKey       键值
** 输　出  : 被插入的节点
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_FRAG_TREE_NODE hoitFragTreeGetLastNode(PHOIT_FRAG_TREE pFTTree){
    return __hoitFragTreeGetMaximum(pFTTree, (PHOIT_FRAG_TREE_NODE)pFTTree->pRbTree->pRbnRoot);
}
/*********************************************************************************************************
** 函数名称: hoitFragTreeDeleteNode
** 功能描述: 在FragTree中删除一个节点
** 输　入  : pFTTree    FragTree
**          pFTn        待删除节点
**          bDoDelete   是否删除RawInfo
** 输　出  : 删除是否成功
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL hoitFragTreeDeleteNode(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTn, BOOL bDoDelete){
    BOOL        res; 
    if(pFTTree->uiNCnt == 0)
        return LW_FALSE;
    
    /* 07-18 增加删除红黑树节点检查其是否在WriteBuffer链上的代码 */
    if (pFTn->pMergeEntry != LW_NULL) {
        pFTn->pMergeEntry->pTreeNode = LW_NULL;
    }
#ifdef FT_DEBUG
    // 测试
    printf("Delete: [%d, %d]\n", pFTn->pRbn.iKey, pFTn->uiSize + pFTn->pRbn.iKey - 1);
#endif
    res = hoitRbDeleteNode(pFTTree->pRbTree, &pFTn->pRbn);
    if(res){
        pFTTree->uiNCnt--;
        pFTTree->uiMemoryBytes -= sizeof(HOIT_FRAG_TREE_NODE);
        __hoit_delete_full_dnode(pFTTree->pfs, pFTn->pFDnode, bDoDelete); /* 删除pFDNode */
        hoit_free(pFTTree->pfs, pFTn, sizeof(HOIT_FRAG_TREE_NODE));                                                   /* 删除整个TreeNode */
    }
    return res;
}
/*********************************************************************************************************
** 函数名称: hoitFragTreeTraverse
** 功能描述: 遍历FragTree结构
** 输　入  : pFTTree    FragTree
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID hoitFragTreeTraverse(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTnRoot){
    if(&pFTnRoot->pRbn == RB_GUARD(pFTTree)){
        return;
    }
    else
    {
        hoitFragTreeTraverse(pFTTree, FT_LEFT_CHILD(pFTnRoot));
        printf("uiOfs: %d, uiSize: %d, iKey: %d \n", pFTnRoot->uiOfs, pFTnRoot->uiSize, FT_GET_KEY(pFTnRoot));
        hoitFragTreeTraverse(pFTTree, FT_RIGHT_CHILD(pFTnRoot));
    }
}
/*********************************************************************************************************
** 函数名称: __hoitFragTreeCollectRangeHelper
** 功能描述: 搜集iKeyLow至iKeyHigh的FragTree节点
** 输　入  : pFTlistHeader          链表头
**          pFTTree               FragTree
**          pFTnRoot              根节点
**          iKeyLow               低键值
**          iKeyHigh              高键值
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VIS_STATUE __hoitFragTreeInsertOverlayFixUpVisitor(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTnRoot, PVOID pUserValue, PVOID *ppReturn){
    HOIT_FRAG_TREE_OVERLAY_FIXUP_RARAM *pParam = (HOIT_FRAG_TREE_OVERLAY_FIXUP_RARAM *) pUserValue;
    PHOIT_FRAG_TREE_NODE    pFTnConqueror      = (PHOIT_FRAG_TREE_NODE)pParam->pFTn;
    BOOL                    bDoDelete          = pParam->bDoDelete;
    PHOIT_FRAG_TREE_NODE    pFTnVictim         = pFTnRoot;
    PHOIT_FRAG_TREE_NODE    pFTnNew;   
    BOOL                    bIsOverlay         = LW_FALSE;
    UINT                    uiCase             = 0;
    UINT32                  uiConquerorLow     = pFTnConqueror->uiOfs;
    UINT32                  uiConquerorHigh    = uiConquerorLow + pFTnConqueror->uiSize == 0 ?
                                              0 : uiConquerorLow + pFTnConqueror->uiSize - 1;
    VIS_STATUE              visStatue = VIS_CONTINUE;

    bIsOverlay = __hoitFragTreeConquerNode(pFTTree, pFTnVictim, uiConquerorLow, uiConquerorHigh,
                                           &pFTnNew, &uiCase, bDoDelete);
    if(bIsOverlay){
        switch (uiCase)
        {
        /*! 情况1
            1. 仅修改被征服者Cur的Size即可，__hoitFragTreeConquerNode已经帮我们做了

            |-------|-------------|-------|
            ^       ^             ^       ^
            Cur  Conqueror       Cur    Conqueror  
        */
        case 1:
            break;
        /*! 情况2
            1. 截取被征服节点Cur为pFTnNew，其偏移为ConquerorHigh，大小为CurHigh - ConquerorHigh，
                并将其插入FragTree中
            2. 修改被征服节点Cur的Size为ConquereorLow - CurLow
            3. 将pFTnNew加入检测链表的末尾，以供下一次检测
            |-------|-------------|-------|
            ^       ^             ^       ^
            Cur  Conqueror      Conqueror  Cur
        */
        case 2:
            if(pFTnNew == LW_NULL){
                printf("something wrong with [fragtree fixup] CASE 2 when fix overlay of nodes containing %d and %d \n", 
                        FT_GET_KEY(pFTnVictim), FT_GET_KEY(pFTnConqueror));
                return VIS_ERROR;
            }
            break;
        /*! 情况3
            1. 截取被征服节点Cur为pFTnNew，其偏移为ConquerorHigh，大小为CurHigh - ConquerorHigh，
                并将其插入FragTree中
            2. 从FragTree中删除被征服节点Cur
            3. 从链表中删除Cur指针
            4. 将pFTnNew插入链表中，表示即将做下一次检查
            5. Cur指向下一个节点
            6. 因为被征服节点改变，因此Conqueror应该从链表头，代表重新开始征服
                |-------|-------------|-------|
                ^       ^             ^       ^
            Conqueror  Cur       Conqueror  Cur
        */
        case 3:
            if(pFTnNew == LW_NULL){
                printf("something wrong with [fragtree fixup] CASE 3 when fix overlay of nodes containing %d and %d \n", 
                        FT_GET_KEY(pFTnVictim), FT_GET_KEY(pFTnConqueror));
                return VIS_ERROR;
            }
            break;
        /*! 情况4
            1. 删除FragTree中的Cur
            2. 从链表中删除Cur
            3. Cur指向下一个节点
            4. 因为被征服节点改变，因此Conqueror应该从链表头，代表重新开始征服
                |-------|-------------|-------|
                ^       ^             ^       ^
            Conqueror  Cur          Cur  Conqueror
        */
        case 4:
            break;
        default:
            break;
        }
    }
    else {
        if(pFTnVictim->uiOfs > uiConquerorHigh) {
            visStatue = VIS_END;
        }
    }
    return visStatue;
}
/*********************************************************************************************************
** 函数名称: hoitFragTreeTraverseVisitor
** 功能描述: 用Visitor遍历FragTree结构
** 输　入  : pFTTree    FragTree
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VIS_STATUE hoitFragTreeTraverseVisitor(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTnStart, visitorHoitFragTree visitor, PVOID pUserValue, PVOID *ppReturn){
    VIS_STATUE  visStatue = VIS_CONTINUE;
    UINT        i;
    PHOIT_FRAG_TREE_NODE    pFTnNext;
    while (pFTnStart != (PHOIT_FRAG_TREE_NODE)pFTTree->pRbTree->pRbnGuard)
    {
        pFTnNext = __hoitFragTreeGetSuccessor(pFTTree, pFTnStart);
        if(visitor) {
            visStatue = visitor(pFTTree, pFTnStart, pUserValue, ppReturn);
            if(visStatue == VIS_END){
                break;
            }
        }
        pFTnStart = pFTnNext;
    }
    
    return visStatue;
}
/*********************************************************************************************************
** 函数名称: __hoitFragTreeReadVisitor
** 功能描述: 读FragTree的内容
** 输　入  : pFTTree    FragTree
**          uiOfs       偏移
**          uiSize      大小
**          pContent    读取的内容
** 输　出  : ERROR_NONE 无错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VIS_STATUE __hoitFragTreeReadVisitor(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTnRoot, 
                                     PVOID pUserValue, PVOID *ppReturn){
    
    HOIT_FRAG_TREE_READ_PARAM *pParam        = (HOIT_FRAG_TREE_READ_PARAM *)pUserValue; 
  
    UINT                      uiCurLow       = pFTnRoot->uiOfs;
    UINT                      uiCurHigh      = uiCurLow + pFTnRoot->uiSize == 0 ?
                                               0 : uiCurLow + pFTnRoot->uiSize - 1;
    
    UINT                      uiTargetLow    = pParam->uiOfs;
    UINT                      uiTargetHigh   = uiTargetLow + pParam->uiSize == 0 ?
                                               0 : uiTargetLow + pParam->uiSize - 1;

    PHOIT_VOLUME              pfs            = pFTTree->pfs;

    UINT                      uiReadOfs      = 0;           /* 读出物理偏移 */
    UINT                      uiReadSize     = 0;           /* 读出大小 */
    UINT                      uiReadBias     = 0;           /* 相对pContent的偏移 */
    if(!ppReturn){
        return VIS_END;
    }
    uiReadOfs = pFTnRoot->pFDnode->HOITFD_raw_info->phys_addr + sizeof(HOIT_RAW_INODE);  /* 找到真实位置 */

    /* 
        [uiTargetLow, uiTargetHigh] | [uiCurLow, uiCurHigh]
     */
    if(uiCurLow > uiTargetHigh){
        return VIS_END;
    }
    /* 
        [uiCurLow, uiCurHigh] | [uiTargetLow, uiTargetHigh]  
     */
    else if(uiCurHigh < uiTargetLow){   
        return VIS_CONTINUE;
    }
    /* 
                     [uiTargetLow, uiTargetHigh]  
        [uiCurLow,                          uiCurHigh]
     */
    else if(uiCurLow <= uiTargetLow && uiCurHigh >= uiTargetHigh){
        uiReadBias = 0;
        uiReadSize = uiTargetHigh - uiTargetLow + 1;
        uiReadOfs  += (uiTargetLow - uiCurLow);
        hoitReadFromCache(pfs->HOITFS_cacheHdr, uiReadOfs, 
                          ((PCHAR)*(ppReturn)) + uiReadBias, uiReadSize);
        return VIS_END;
    }   

    /* 
                     [uiTargetLow, uiTargetHigh]  
        [uiCurLow, uiCurHigh] 
     */
    if(uiCurLow <= uiTargetLow && uiCurHigh <= uiTargetHigh){          /* 三种情况 */
        uiReadBias = 0;
        uiReadSize = uiCurHigh - uiTargetLow + 1;
        uiReadOfs  += (uiTargetLow - uiCurLow);
    }   
    /* 
        [uiTargetLow,                   uiTargetHigh]
                    [uiCurLow, uiCurHigh]  
     */
    else if(uiCurLow >= uiTargetLow && uiCurHigh <= uiTargetHigh){
        uiReadBias = uiCurLow - uiTargetLow;
        uiReadSize = uiCurHigh - uiCurLow + 1;
        uiReadOfs  += (uiCurLow - uiCurLow);
    }       
    /* 
                [uiTargetLow, uiTargetHigh]  
                                    [uiCurLow, uiCurHigh] 
     */      
    else if(uiCurLow >= uiTargetLow && uiCurHigh >= uiTargetHigh){
        uiReadBias = uiCurLow - uiTargetLow;
        uiReadSize = uiTargetHigh - uiCurLow + 1;
        uiReadOfs  += (uiCurLow - uiCurLow);
    }
    hoitReadFromCache(pfs->HOITFS_cacheHdr, uiReadOfs, 
                      ((PCHAR)*(ppReturn)) + uiReadBias, uiReadSize);
    return VIS_CONTINUE;
}
/*********************************************************************************************************
** 函数名称: hoitFragTreeRead
** 功能描述: 读FragTree的内容
** 输　入  : pFTTree    FragTree
**          uiOfs       偏移
**          uiSize      大小
**          pContent    读取的内容
** 输　出  : ERROR_NONE 无错误
** 全局变量:
** 调用模块:
*********************************************************************************************************/
error_t hoitFragTreeRead(PHOIT_FRAG_TREE pFTTree, UINT32 uiOfs, UINT32 uiSize, PCHAR pContent){
    UINT32                      iKeyLow;
    UINT32                      iKeyHigh;
    UINT32                      uiBias;
    // PHOIT_FRAG_TREE_LIST_NODE   pFTlist;
    // PHOIT_FRAG_TREE_LIST_HEADER pFTlistHeader;

    UINT32                      uiPhyOfs;
    UINT32                      uiPhySize;

    UINT32                      uiSizeRead;
    UINT32                      uiSizeRemain;

    UINT32                      uiPerOfs;
    UINT32                      uiPerSize;
    // PCHAR                       pPerContent;
    // CHAR                        cPerContent[HOIT_MAX_DATA_SIZE];
#ifdef FT_OBSOLETE_TREE_LIST
    PHOIT_FRAG_TREE_NODE        pFTnStart;
    HOIT_FRAG_TREE_READ_PARAM   param;

    // uiSizeRead          = 0;
    // uiSizeRemain        = uiSize;
    // iKeyLow             = uiOfs;
    // iKeyHigh            = uiOfs + uiSize;
    param.uiOfs         = uiOfs;
    param.uiSize        = uiSize;
    pFTnStart           = FT_GET_FIRST_NODE(pFTTree);
    hoitFragTreeTraverseVisitor(pFTTree, pFTnStart, __hoitFragTreeReadVisitor, &param, (PVOID)&pContent);
#else
    pFTlistHeader       = hoitFragTreeCollectRange(pFTTree, iKeyLow, iKeyHigh);
    uiBias              = uiOfs - pFTlistHeader->uiLowBound; 

    pFTlist             = pFTlistHeader->pFTlistHeader->pFTlistNext;
    while (pFTlist != LW_NULL)
    {
        uiPhyOfs = pFTlist->pFTn->pFDnode->HOITFD_raw_info->phys_addr + sizeof(HOIT_RAW_INODE);
        uiPhySize = pFTlist->pFTn->pFDnode->HOITFD_length;
        /* 读第一块
            |--H-|-uiBias-|
            |----|--------|-------|
                 |        |       |
                 |        |       |
             uiPhyOfs  uiPerOfs  uiPhyOfs + uiPhySize
        */
        if(uiSizeRead == 0){
            uiPerOfs = uiPhyOfs + uiBias;
            uiPerSize = uiPhySize - uiBias;
        }
        /* 读最后一块
                        |--H-|-remain-|  
                        |----|--------|-------|
                             |                |
                             |                |
                uiPhyOfs(uiPerOfs)  uiPhyOfs + uiPhySize
        */
        else if (uiSizeRead + uiPhySize >= uiSize)
        {
            uiPerOfs = uiPhyOfs;
            uiPerSize = uiSizeRemain;
        }
        else                                                                        /* 其他情况 */
        {
            uiPerOfs = uiPhyOfs;
            uiPerSize = uiPhySize;
        }

        if(uiPerSize > uiSize){
            uiPerSize = uiSize;
        }

        pPerContent = (PCHAR)hoit_malloc(pfs, uiPerSize);                                 /* 每一次读取的内容 */
        //TODO: 待实现
        hoitReadFromCache(pFTTree->pfs->HOITFS_cacheHdr, uiPerOfs, pPerContent, uiPerSize);

        lib_memcpy(pContent + uiSizeRead, pPerContent, uiPerSize);
        hoit_free(pfs, pPerContent);
        
        pFTlist = pFTlist->pFTlistNext;


        uiSizeRead += uiPerSize;
        uiSizeRemain -= uiPerSize;

        if(uiSizeRead == uiSize) {
            break;
        }
    }

    hoitFragTreeListFree(pFTlistHeader);                                             /* 释放收集链表 */
#endif
    return ERROR_NONE;
}

/*********************************************************************************************************
** 函数名称: hoitFragTreeInsertNode
** 功能描述: 初始化FragTree
** 输　入  : pFTTree    FragTree
**          pFTn       待插入节点
** 输　出  : 被插入的节点
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_FRAG_TREE_NODE hoitFragTreeInsertNode(PHOIT_FRAG_TREE pFTTree, PHOIT_FRAG_TREE_NODE pFTn){
#ifdef FT_DEBUG
    printf("Insert: [%d, %d]\n", pFTn->pRbn.iKey, pFTn->uiSize + pFTn->pRbn.iKey - 1);
#endif

#ifndef FT_OBSOLETE_TREE_LIST /* 未定义过期时，采用 Insert + OverlayFix Up做法 */
    hoitRbInsertNode(pFTTree->pRbTree, &pFTn->pRbn);
#else
    HOIT_FRAG_TREE_OVERLAY_FIXUP_RARAM param;
    param.pFTn      = pFTn;
    param.bDoDelete = LW_TRUE;
    PHOIT_FRAG_TREE_NODE pFTnStart = __hoitFragTreeGetMinimum(pFTTree, (PHOIT_FRAG_TREE_NODE)pFTTree->pRbTree->pRbnRoot);
    if(pFTTree->uiNCnt != 0){
        hoitFragTreeTraverseVisitor(pFTTree, pFTnStart, 
                                    __hoitFragTreeInsertOverlayFixUpVisitor, 
                                    (PVOID)&param, NULL);
    }
    hoitRbInsertNode(pFTTree->pRbTree, &pFTn->pRbn);
#ifdef FT_DEBUG
    printf("==================Insert: [%d, %d]==================\n", pFTn->pRbn.iKey, 
            pFTn->uiOfs + pFTn->uiSize == 0 ? 0 : pFTn->uiOfs + pFTn->uiSize - 1);
    hoitFragTreeTraverse(pFTTree, (PHOIT_FRAG_TREE_NODE)pFTTree->pRbTree->pRbnRoot);
    printf("==================Insert End      ==================\n\n");
#endif  /* FT_DEBUG */

#endif  /* FT_OBSOLETE_TREE_LIST */
    pFTTree->uiNCnt++;
    pFTTree->uiMemoryBytes += sizeof(HOIT_FRAG_TREE_NODE);
    return pFTn;
}
/*********************************************************************************************************
** 函数名称: hoitFragTreeShowMemory
** 功能描述: 查看指定FragTree结构
** 输　入  : pFTTree    FragTree
** 输　出  : 成功 LW_TRUE，否则LW_FALSE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID hoitFragTreeShowMemory(PHOIT_FRAG_TREE pFTTree){
   printf("\n============= checking fragtree statue ...  =============\n");
   printf("nodes  count: %d.\n", pFTTree->uiNCnt);
   printf("memory usage: %dB\n", pFTTree->uiMemoryBytes);
   printf("============= checking fragtree statue over =============\n");
}


/*********************************************************************************************************
** 函数名称: hoitFragTreeDeleteNode
** 功能描述: 在FragTree中删除[iKeyLow, iKeyHigh]的节点，利用ConquerNode进行，因为有些部分不需要被删除
** 输　入  : pFTTree    FragTree
**          iKeyLow    区间低值
**          iKeyHigh   区间高值
**          bDoDelete   是否删除RawInfo
** 输　出  : 删除是否成功
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL hoitFragTreeDeleteRange(PHOIT_FRAG_TREE pFTTree, INT32 iKeyLow, INT32 iKeyHigh, BOOL bDoDelete){
    HOIT_FRAG_TREE_OVERLAY_FIXUP_RARAM param;
    HOIT_FRAG_TREE_NODE         FTn;

    BOOL                        bRes = LW_TRUE;

    /* 构造征服者 */
    FTn.uiOfs      = iKeyLow < 0 ? 0 : iKeyLow;
    FTn.uiSize     = iKeyHigh + 1 - FTn.uiOfs;

#ifdef FT_OBSOLETE_TREE_LIST
    PHOIT_FRAG_TREE_NODE        pFTnStart = __hoitFragTreeGetMinimum(pFTTree, (PHOIT_FRAG_TREE_NODE)pFTTree->pRbTree->pRbnRoot);
    param.bDoDelete = bDoDelete;
    param.pFTn      = &FTn;
    if(pFTTree->uiNCnt != 0){
        hoitFragTreeTraverseVisitor(pFTTree, pFTnStart, 
                                    __hoitFragTreeInsertOverlayFixUpVisitor, 
                                    (PVOID)&param, NULL);
        return LW_TRUE;
    }
#else
    UINT                        uiCount = 0;
    UINT8                       uiCase;
    PHOIT_FRAG_TREE_NODE        pFTnNew;
    PHOIT_FRAG_TREE_LIST_HEADER pFTlistHeader;
    PHOIT_FRAG_TREE_LIST_NODE   pFTlistNode;
    UINT32                      uiConquerorLow = iKeyLow;
    UINT32                      uiConquerorHigh = iKeyHigh;
    pFTlistHeader = hoitFragTreeCollectRange(pFTTree, iKeyLow, iKeyHigh);
    pFTlistNode = pFTlistHeader->pFTlistHeader->pFTlistNext;
    while (pFTlistNode)
    {
        uiCount++;
#ifdef FT_DEBUG
        printf("count %d\n", uiCount);
#endif
        //TODO: 验证可行性
        __hoitFragTreeConquerNode(pFTTree, pFTlistNode->pFTn, uiConquerorLow, uiConquerorHigh, 
                                    &pFTnNew, &uiCase, bDoDelete);
        
        pFTlistNode = pFTlistNode->pFTlistNext;
    }
    hoitFragTreeListFree(pFTlistHeader);
    return bRes;  
#endif  
}

/*********************************************************************************************************
** 函数名称: hoitFragTreeDeleteTree
** 功能描述: 删除FragTree结构
** 输　入  : pFTTree    FragTree
** 输　出  : 删除是否成功
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL hoitFragTreeDeleteTree(PHOIT_FRAG_TREE pFTTree, BOOL bDoDelete){
    BOOL                          res;
    PHOIT_VOLUME                  pfs = pFTTree->pfs;
    res = hoitFragTreeDeleteRange(pFTTree, INT_MIN, INT_MAX, bDoDelete);
    hoit_free(pfs, pFTTree->pRbTree->pRbnGuard->pRbnLeft, sizeof(HOIT_RB_NODE));
    hoit_free(pfs, pFTTree->pRbTree->pRbnGuard, sizeof(HOIT_RB_NODE));
    hoit_free(pfs, pFTTree->pRbTree, sizeof(HOIT_RB_TREE));
    hoit_free(pfs, pFTTree, sizeof(HOIT_FRAG_TREE));
    
#ifdef FT_DEBUG
    hoitFragTreeTraverse(pFTTree, (PHOIT_FRAG_TREE_NODE)pFTTree->pRbTree->pRbnRoot);
    hoitFragTreeShowMemory(pFTTree);
#endif
    return res;
}
/*********************************************************************************************************
** 函数名称: hoitFragTreeOverlayFixUp
** 功能描述: 修复FragTree上的Overlap
** 输　入  : pFTTree    FragTree
** 输　出  : 成功 LW_TRUE，否则LW_FALSE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
//TODO: 性能降为 O(n)，参数添加最近节点
#ifdef FT_OBSOLETE_TREE_LIST
BOOL hoitFragTreeOverlayFixUp(PHOIT_FRAG_TREE pFTTree){
    /* 虚假接口 */
}
#else
BOOL hoitFragTreeOverlayFixUp(PHOIT_FRAG_TREE pFTTree){
    PHOIT_FRAG_TREE_LIST_HEADER     pFTlistHeader;
    PHOIT_FRAG_TREE_LIST_NODE       pFTlistCur;
    PHOIT_FRAG_TREE_LIST_NODE       pFTlistConqueror;
    PHOIT_FRAG_TREE_LIST_NODE       pFTlistNext;

    PHOIT_FRAG_TREE_NODE            pFTnConqueror;

    PHOIT_FRAG_TREE_NODE            pFTnNew;
    PHOIT_FRAG_TREE_NODE            pFTn;
    PHOIT_FRAG_TREE_LIST_NODE       pFTlistNodeNew;

    UINT32                          uiConquerorLow;
    UINT32                          uiConquerorHigh;
    UINT8                           uiCase;

    UINT32                          uiLeftRemainSize;
    UINT32                          uiRightRemainSize;
    UINT32                          uiRightOffset;

    BOOL                            bIsOverlay;
    UINT32                          debug_outer_count = 0;
    UINT32                          debug_inner_count = 0;

    pFTlistHeader = hoitFragTreeCollectRange(pFTTree, INT_MIN, INT_MAX);
    pFTlistCur = pFTlistHeader->pFTlistHeader->pFTlistNext;
    while (pFTlistCur != LW_NULL)                                                   /* 没走到尾巴上 */
    {
        debug_outer_count ++;
        if (debug_outer_count == 783){
//          printf("debug_outer_count:%d \n",debug_outer_count);
            debug_inner_count = debug_outer_count;
        }
        debug_inner_count = 0;
        //TODO: 验证正确性?
        pFTlistConqueror = pFTlistHeader->pFTlistHeader->pFTlistNext;               /* 征服者 */
        while (pFTlistConqueror != LW_NULL)
        {
            debug_inner_count ++;

            bIsOverlay = LW_FALSE;
            if(pFTlistCur == LW_NULL){
                break;
            }
            if(pFTlistConqueror == pFTlistCur)                                      /* 跳过 */
            {
                pFTlistConqueror = pFTlistConqueror->pFTlistNext;
                continue;
            }
            if(pFTlistConqueror->pFTn->pFDnode->HOITFD_version                      /* Conqueror的Version必须要比被征服者的Version大 */
             < pFTlistCur->pFTn->pFDnode->HOITFD_version){
                pFTlistConqueror = pFTlistConqueror->pFTlistNext;
                continue;
            }

            pFTn            = pFTlistCur->pFTn;
            pFTnConqueror   = pFTlistConqueror->pFTn;
            uiConquerorLow  = pFTnConqueror->uiOfs;
            uiConquerorHigh = uiConquerorLow + pFTnConqueror->uiSize == 0 ?
                              0 : uiConquerorLow + pFTnConqueror->uiSize - 1;
            // if(uiConquerorLow >= 472 && uiConquerorLow < 1000 && pFTn->uiOfs >= 360)
            //     printf("Conqueror: [%d, %d], Victim: [%d, %d]\n", uiConquerorLow, uiConquerorHigh, pFTn->uiOfs,
            //             pFTn->uiOfs + pFTn->uiSize == 0 ? 0 : pFTn->uiOfs + pFTn->uiSize - 1);
            // if(uiConquerorLow == 472 && uiConquerorHigh == 472 &&
            // pFTn->uiOfs == 472 &&  pFTn->uiOfs + pFTn->uiSize - 1 == 4095){
            //     printf("debug\n");
            // }
            bIsOverlay = __hoitFragTreeConquerNode(pFTTree, pFTn, uiConquerorLow, uiConquerorHigh,
                                                   &pFTnNew, &uiCase, LW_TRUE);
            if(bIsOverlay){
                switch (uiCase)
                {
                /*! 情况1
                    1. 仅修改被征服者Cur的Size即可

                    |-------|-------------|-------|
                    ^       ^             ^       ^
                    Cur  Conqueror       Cur    Conqueror  
                */
                case 1:
                    pFTlistConqueror = pFTlistConqueror->pFTlistNext;
                    break;
                /*! 情况2
                    1. 截取被征服节点Cur为pFTnNew，其偏移为ConquerorHigh，大小为CurHigh - ConquerorHigh，
                       并将其插入FragTree中
                    2. 修改被征服节点Cur的Size为ConquereorLow - CurLow
                    3. 将pFTnNew加入检测链表的末尾，以供下一次检测
                    |-------|-------------|-------|
                    ^       ^             ^       ^
                    Cur  Conqueror      Conqueror  Cur
                */
                case 2:
                    if(pFTnNew == LW_NULL){
                        printf("something wrong with [fragtree fixup] CASE 2 when fix overlay of nodes containing %d and %d \n", 
                                FT_GET_KEY(pFTn), FT_GET_KEY(pFTnConqueror));
                        return;
                    }
                    else {
                        pFTlistNodeNew = newFragTreeListNode(pFTnNew);
                        hoitFragTreeListInsertNode(pFTlistHeader, pFTlistNodeNew);                  /* 将pFTnNew插入链表末尾 */

                        pFTlistConqueror = pFTlistConqueror->pFTlistNext;
                    }
                    break;
                /*! 情况3
                    1. 截取被征服节点Cur为pFTnNew，其偏移为ConquerorHigh，大小为CurHigh - ConquerorHigh，
                       并将其插入FragTree中
                    2. 从FragTree中删除被征服节点Cur
                    3. 从链表中删除Cur指针
                    4. 将pFTnNew插入链表中，表示即将做下一次检查
                    5. Cur指向下一个节点
                    6. 因为被征服节点改变，因此Conqueror应该从链表头，代表重新开始征服
                        |-------|-------------|-------|
                        ^       ^             ^       ^
                    Conqueror  Cur       Conqueror  Cur
                */
                case 3:
                    if(pFTnNew == LW_NULL){
                        printf("something wrong with [fragtree fixup] CASE 3 when fix overlay of nodes containing %d and %d \n", 
                                FT_GET_KEY(pFTn), FT_GET_KEY(pFTnConqueror));
                        return;
                    }
                    else {
                        pFTlistNodeNew = newFragTreeListNode(pFTnNew);
                        hoitFragTreeListInsertNode(pFTlistHeader, pFTlistNodeNew);                  /* 将pFTnNew插入链表末尾? */
                        
                        pFTlistNext = pFTlistCur->pFTlistNext;
                        hoitFragTreeListDeleteNode(pFTlistHeader, pFTlistCur);                      /* 从链表中删除Cur */
                        hoit_free(pfs, pFTlistCur);

                        pFTlistCur = pFTlistNext;                                                   /* 置当前被征服节点为下一个 */
                        pFTlistConqueror = pFTlistHeader->pFTlistHeader;                            /* 征服者从头开始征服 */
                        
                        pFTlistConqueror = pFTlistConqueror->pFTlistNext;
                    }
                    break;
                /*! 情况4
                    1. 删除FragTree中的Cur
                    2. 从链表中删除Cur
                    3. Cur指向下一个节点
                    4. 因为被征服节点改变，因此Conqueror应该从链表头，代表重新开始征服
                        |-------|-------------|-------|
                        ^       ^             ^       ^
                    Conqueror  Cur          Cur  Conqueror
                */
                case 4:
                    pFTlistNext = pFTlistCur->pFTlistNext;
                    hoitFragTreeListDeleteNode(pFTlistHeader, pFTlistCur);                        /* 从链表中删除Cur */
                    hoit_free(pfs, pFTlistCur);

                    pFTlistCur = pFTlistNext;                                                     /* 置当前被征服节点为下一个 */
                    pFTlistConqueror = pFTlistHeader->pFTlistHeader;                              /* 征服者从头开始征服 */
                    
                    pFTlistConqueror = pFTlistConqueror->pFTlistNext;
                    break;
                default:
                    break;
                }
            }
            else{
                pFTlistConqueror = pFTlistConqueror->pFTlistNext;
            }
        }
        if(pFTlistCur == LW_NULL){
            break;
        }
        pFTlistCur = pFTlistCur->pFTlistNext;   
    }
    hoitFragTreeListFree(pFTlistHeader);
    return LW_TRUE;
}
#endif

#ifdef FT_TEST
VOID hoitFTTreeTest(){
    INT                                 i;
    PHOIT_FRAG_TREE                     pFTTree;
    PHOIT_FRAG_TREE_NODE                pFTn;
    PHOIT_FRAG_TREE_LIST_HEADER         pFTlistHeader;
    PHOIT_FRAG_TREE_LIST_NODE           pFTlistNode;
    BOOL                                res;
    PHOIT_VOLUME                        pfs;
    //INT testArray[10] = {8,11,14,15,1,2,4,5,7, 1};
    INT testArray[10] = {0, 1,2,3, 4,5,7,11,14,15};
    
    pfs = (PHOIT_VOLUME)hoit_malloc(pfs, sizeof(HOIT_VOLUME));
    pFTTree = hoitInitFragTree(pfs);
    
    for (i = 0; i < 10; i++)
    {
        pFTn = newHoitFragTreeNode(LW_NULL, i, 10, testArray[i]);
        hoitFragTreeInsertNode(pFTTree, pFTn);
    }

    /*!  锟斤拷锟斤拷锟斤拷锟斤拷锟剿ｏ拷锟斤拷锟斤拷顺锟斤拷锟斤拷锟斤拷锟侥伙拷锟斤拷锟斤拷锟斤拷锟斤拷 -- SOlVE */
    // INT randomArray[5] = {2,4,5,7, 8};
    // for (i = 0; i < 5; i++)
    // {
    //     printf("Delete Node %d \n", randomArray[i]);
    //     pFTn = hoitFragTreeSearchNode(pFTTree, randomArray[i]);
    //     hoitFragTreeDeleteNode(pFTTree, pFTn);
    // }
    // printf("total nodes: %d \n", pFTTree->uiNCnt);

    pFTn = hoitFragTreeSearchNode(pFTTree, 7);
    printf("pFTn - uiOfs : %d\n", pFTn->uiOfs);
    printf("pFTn - uiSize: %d\n", pFTn->uiSize);
    printf("pFTn - iKey  : %d\n", FT_GET_KEY(pFTn));
    
    printf("\n 1. [test traverse] \n");
    hoitFragTreeTraverse(pFTTree, (PHOIT_FRAG_TREE_NODE)pFTTree->pRbTree->pRbnRoot);
    printf("\n 2. [test delete 7] \n");
    hoitFragTreeDeleteNode(pFTTree, pFTn, LW_FALSE);
    hoitFragTreeTraverse(pFTTree, (PHOIT_FRAG_TREE_NODE)pFTTree->pRbTree->pRbnRoot);
    
    

    printf("\n 3. [test collect range [2, 4] ] \n");
    pFTlistHeader = hoitFragTreeCollectRange(pFTTree, INT_MIN, INT_MAX);
    pFTlistNode = pFTlistHeader->pFTlistHeader->pFTlistNext;
    while (pFTlistNode)
    {
        printf("Key: %d\n", FT_GET_KEY(pFTlistNode->pFTn));   
        pFTlistNode = pFTlistNode->pFTlistNext;
    }
    printf("range [%d, %d] \n", pFTlistHeader->uiLowBound, pFTlistHeader->uiHighBound);
    hoitFragTreeListFree(pFTlistHeader);
    
    printf("\n 4. [test delete range] \n");
    res = hoitFragTreeDeleteRange(pFTTree, 2, 8, LW_FALSE);
    hoitFragTreeTraverse(pFTTree, (PHOIT_FRAG_TREE_NODE)pFTTree->pRbTree->pRbnRoot);
    printf("total nodes: %d\n", pFTTree->uiNCnt);

    printf("\n 5. [test delete FTtree] \n");
    res = hoitFragTreeDeleteTree(pFTTree, LW_FALSE);
}
#endif // DEBUG
