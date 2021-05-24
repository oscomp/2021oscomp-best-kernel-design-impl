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
** 文   件   名: hoitFsTreeUtil.c
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 03 月 28 日
**
** 描        述: JFFS2-Like 红黑树数据结构
*********************************************************************************************************/

#include "hoitFsTreeUtil.h"

#define RB_LEFT_CHILD(pRbn)          (pRbn->pRbnLeft)
#define RB_RIGHT_CHILD(pRbn)         (pRbn->pRbnRight)
#define RB_PARENT(pRbn)              (pRbn->pRbnParent)
#define RB_GRAND(pRbn)               RB_PARENT(RB_PARENT(pRbn))
#define RB_UNCLE_RIGHT(pRbn)         (RB_RIGHT_CHILD(RB_GRAND(pRbn)))
#define RB_UNCLE_LEFT(pRbn)          (RB_LEFT_CHILD(RB_GRAND(pRbn)))

#define RB_IS_LEFT_CHILD(pRbn)       pRbn == RB_LEFT_CHILD(RB_PARENT(pRbn))
#define RB_IS_RIGHT_CHILD(pRbn)      pRbn == RB_RIGHT_CHILD(RB_PARENT(pRbn))
/*********************************************************************************************************
** 函数名称: __hoitRbMinimum
** 功能描述: 寻找某个红黑子树中最小的子节点
** 输　入  : pRbTree          红黑树
**           pRbnRoot          红黑子树树根             
** 输　出  : 返回最小子节点
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_RB_NODE __hoitRbMinimum(PHOIT_RB_TREE pRbTree, PHOIT_RB_NODE pRbnRoot){
    PHOIT_RB_NODE pRbnTraverse;
    pRbnTraverse = pRbnRoot;
    while (pRbnTraverse->pRbnLeft != pRbTree->pRbnGuard)
    {
        pRbnTraverse = RB_LEFT_CHILD(pRbnTraverse);   
    }
    return pRbnTraverse;
}
/*********************************************************************************************************
** 函数名称: __hoitRbMaximum
** 功能描述: 寻找某个红黑子树中最大的子节点
** 输　入  : pRbTree          红黑树
**           pRbnRoot          红黑子树树根             
** 输　出  : 返回最大子节点
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_RB_NODE __hoitRbMaximum(PHOIT_RB_TREE pRbTree, PHOIT_RB_NODE pRbnRoot){
    PHOIT_RB_NODE pRbnTraverse;
    pRbnTraverse = pRbnRoot;
    while (pRbnTraverse->pRbnRight != pRbTree->pRbnGuard)
    {
        pRbnTraverse = RB_RIGHT_CHILD(pRbnTraverse);   
    }
    return pRbnTraverse;
}
/*********************************************************************************************************
** 函数名称: __hoitRbConquer
** 功能描述: 用征服者节点及其子树替换目标节点
** 输　入  : pRbTree          红黑树
**           pRbnTarget       目标节点      
**           pRbnConqueror    征服者节点        
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoitRbConquer(PHOIT_RB_TREE pRbTree, PHOIT_RB_NODE pRbnTarget, PHOIT_RB_NODE pRbnConqueror){
    if(RB_PARENT(pRbnTarget) == pRbTree->pRbnGuard){
        pRbTree->pRbnRoot = pRbnConqueror;
    }
    else if (RB_IS_LEFT_CHILD(pRbnTarget))
    {
        pRbnTarget->pRbnParent->pRbnLeft = pRbnConqueror;
    }
    else pRbnTarget->pRbnParent->pRbnRight = pRbnConqueror;

    pRbnConqueror->pRbnParent = pRbnTarget->pRbnParent;
}
/*********************************************************************************************************
** 函数名称: __hoitFsLeftRotate
** 功能描述: 红黑树左旋
** 输　入  : pRbTree          红黑树
**           pRbn              红黑树节点
** 输　出  : 成功返回True，失败返回False
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoitRbLeftRotate(PHOIT_RB_TREE pRbTree, PHOIT_RB_NODE pRbn){
    PHOIT_RB_NODE           pRbnRight;
    
    pRbnRight = pRbn->pRbnRight;
    
    if(pRbnRight == pRbTree->pRbnGuard){
        printf("left rotate: target's right child can't be null\n");
        return FALSE;
    }
    
    pRbn->pRbnRight = RB_LEFT_CHILD(pRbnRight);         /* 移动当前节点右孩子的左孩子至该节点的右节点之下 */
    
    if(pRbnRight->pRbnLeft != pRbTree->pRbnGuard){      /* 设置当前节点右孩子的左孩子节点的父亲节点： 如果该节点是空，则不需设置，否则设置为当前节点*/
        pRbnRight->pRbnLeft->pRbnParent = pRbn;
    }

    pRbnRight->pRbnParent = RB_PARENT(pRbn);            /* 设置当前节点右孩子的父节点 */
    
    if(pRbn->pRbnParent == pRbTree->pRbnGuard){         /* 如果当前节点的父亲为空，则当前节点的右节点为红黑树的根 */
        pRbTree->pRbnRoot = pRbnRight;
    }
    else if(pRbn == pRbn->pRbnParent->pRbnLeft){        /* 当前节点是左孩子 */
        pRbn->pRbnParent->pRbnLeft = pRbnRight;
    }
    else pRbn->pRbnParent->pRbnRight = pRbnRight;       /* 当前节点是右孩子 */
    
    pRbnRight->pRbnLeft = pRbn;
    pRbn->pRbnParent = pRbnRight;

    return TRUE;
}
/*********************************************************************************************************
** 函数名称: __hoitFsRightRotate
** 功能描述: 红黑树右旋
** 输　入  : pRbTree          红黑树
**           pRbn              红黑树节点
** 输　出  : 成功返回True，失败返回False
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoitRbRightRotate(PHOIT_RB_TREE pRbTree, PHOIT_RB_NODE pRbn){
    PHOIT_RB_NODE           pRbnLeft;
    
    pRbnLeft = pRbn->pRbnLeft;
    
    if(pRbnLeft == pRbTree->pRbnGuard){
        printf("right rotate: target's left child can't be null\n");
        return FALSE;
    }
    
    pRbn->pRbnLeft = RB_RIGHT_CHILD(pRbnLeft);              
    
    if(pRbnLeft->pRbnRight != pRbTree->pRbnGuard){      
        pRbnLeft->pRbnRight->pRbnParent = pRbn;
    }

    pRbnLeft->pRbnParent = RB_PARENT(pRbn);           
    
    if(pRbn->pRbnParent == pRbTree->pRbnGuard){         
        pRbTree->pRbnRoot = pRbnLeft;
    }
    else if(pRbn == pRbn->pRbnParent->pRbnLeft){        /* 当前节点是左孩子 */
        pRbn->pRbnParent->pRbnLeft = pRbnLeft;
    }
    else pRbn->pRbnParent->pRbnRight = pRbnLeft;       /* 当前节点是右孩子 */
    
    pRbnLeft->pRbnRight = pRbn;
    pRbn->pRbnParent = pRbnLeft;

    return TRUE;
}

/*********************************************************************************************************
** 函数名称: __hoitRbInsertFixUp
** 功能描述: 重绘节点颜色
** 输　入  : pRbTree          红黑树
**           pRbn              待插入红黑树节点
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoitRbInsertFixUp(PHOIT_RB_TREE pRbTree, PHOIT_RB_NODE pRbn){
    while (RB_PARENT(pRbn)->uiColor == RB_RED)
    {
        if(RB_IS_LEFT_CHILD(RB_PARENT(pRbn))){
            /* 父亲是左孩子, 叔叔节点为右侧, 且是红色 
                       PP(B)
                      /   \
                   P(R)    uncle(R)         
                   /
                pRbn(R)
                        
                        或

                        PP(B)
                      /   \
                   P(R)    uncle(R)         
                      \
                    pRbn(R)
            */
            if(RB_UNCLE_RIGHT(pRbn)->uiColor == RB_RED){
                RB_PARENT(pRbn)->uiColor = RB_BLACK;
                RB_UNCLE_RIGHT(pRbn)->uiColor = RB_BLACK;
                RB_GRAND(pRbn)->uiColor = RB_RED;
                pRbn = RB_GRAND(pRbn);
            }
            /* 该节点是右孩子，叔叔是黑色
                      PP(B)
                      /   \
                   P(R)    uncle(B)
                      \
                    pRbn(R)
            */
            else if(RB_IS_RIGHT_CHILD(pRbn)){
                pRbn = RB_PARENT(pRbn);
                __hoitRbLeftRotate(pRbTree, pRbn);
            }
            /* 该节点的叔叔是黑色的，且自己是左孩子
                      PP(B)
                      /   \
                   P(R)    uncle(B)
                   /
                pRbn(R)
            */
            if(RB_IS_LEFT_CHILD(pRbn) && RB_UNCLE_RIGHT(pRbn)->uiColor == RB_BLACK){
                RB_PARENT(pRbn)->uiColor = RB_BLACK;
                RB_GRAND(pRbn)->uiColor = RB_RED;
                __hoitRbRightRotate(pRbTree, RB_GRAND(pRbn));
            }
        }
        else {
            /* 父亲是右孩子, 叔叔节点为左侧, 且是红色 */
            if(RB_UNCLE_LEFT(pRbn)->uiColor == RB_RED){
                RB_PARENT(pRbn)->uiColor = RB_BLACK;
                RB_UNCLE_LEFT(pRbn)->uiColor = RB_BLACK;
                RB_GRAND(pRbn)->uiColor = RB_RED;
                pRbn = RB_GRAND(pRbn);
            }
            /* 该节点是右孩子 */
            else if(RB_IS_LEFT_CHILD(pRbn)){
                pRbn = RB_PARENT(pRbn);
                __hoitRbRightRotate(pRbTree, pRbn);
            }

            if(RB_IS_RIGHT_CHILD(pRbn) && RB_UNCLE_LEFT(pRbn)->uiColor == RB_BLACK){
                RB_PARENT(pRbn)->uiColor = RB_BLACK;
                RB_GRAND(pRbn)->uiColor = RB_RED;
                __hoitRbLeftRotate(pRbTree, RB_GRAND(pRbn));
            }
        }
    }
    pRbTree->pRbnRoot->uiColor = RB_BLACK;
}

/*********************************************************************************************************
** 函数名称: __hoitRbDeleteFixUp
** 功能描述: 重绘节点颜色
** 输　入  : pRbTree          红黑树
**           pRbn             待更新的红黑树节点
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoitRbDeleteFixUp(PHOIT_RB_TREE pRbTree, PHOIT_RB_NODE pRbn){
    PHOIT_RB_NODE           pRbnBrother;

    while (pRbn != pRbTree->pRbnRoot && pRbn->uiColor == RB_BLACK)              /* 当待更新节点没有过更新到根节点，且待更新节点为黑色，参考算法导论第三版 P 201 */
    {
        if(RB_IS_LEFT_CHILD(pRbn)){                                             /* 考虑待更新节点是左孩子 */
            pRbnBrother = RB_RIGHT_CHILD(RB_PARENT(pRbn));
            if(pRbnBrother->uiColor == RB_RED){                                 /* 情况1：待更新节点的兄弟节点为红色，将其转为黑色，变成情况2、3、4 */
                pRbnBrother->uiColor = RB_BLACK;
                pRbnBrother->pRbnParent->uiColor = RB_RED;
                __hoitRbLeftRotate(pRbTree, RB_PARENT(pRbn));
                pRbnBrother = RB_RIGHT_CHILD(RB_PARENT(pRbn));
            }
            if(RB_LEFT_CHILD(pRbnBrother)->uiColor == RB_BLACK 
               && RB_RIGHT_CHILD(pRbnBrother)->uiColor == RB_BLACK){            /* 情况2：待更新节点的兄弟节点的儿子节点均为黑色 */
                pRbnBrother->uiColor = RB_RED;
                pRbn = RB_PARENT(pRbn);
            }
            else if (RB_RIGHT_CHILD(pRbnBrother)->uiColor == RB_BLACK)          /* 情况3：待更新节点的兄弟节点的右孩子是黑色 */
            {
                pRbnBrother->pRbnLeft->uiColor = RB_BLACK;
                pRbnBrother->uiColor = RB_RED;
                __hoitRbRightRotate(pRbTree, pRbnBrother);
                pRbnBrother = RB_RIGHT_CHILD(RB_PARENT(pRbn));

                pRbnBrother->uiColor = RB_PARENT(pRbn)->uiColor;                /* 情况4：待更新节点的兄弟节点的右孩子是红色 */
                pRbn->pRbnParent->uiColor = RB_BLACK;
                RB_RIGHT_CHILD(pRbnBrother)->uiColor = RB_BLACK;
                __hoitRbLeftRotate(pRbTree, RB_PARENT(pRbn));
                pRbn = pRbTree->pRbnRoot;
            }
            else {
                pRbnBrother->uiColor = RB_PARENT(pRbn)->uiColor;                /* 情况4：待更新节点的兄弟节点的右孩子是红色 */
                pRbn->pRbnParent->uiColor = RB_BLACK;
                RB_RIGHT_CHILD(pRbnBrother)->uiColor = RB_BLACK;
                __hoitRbLeftRotate(pRbTree, RB_PARENT(pRbn));
                pRbn = pRbTree->pRbnRoot;
            }
        }
        else
        {
            pRbnBrother = RB_LEFT_CHILD(RB_PARENT(pRbn));
            if(pRbnBrother->uiColor == RB_RED){
                pRbnBrother->uiColor = RB_BLACK;
                pRbnBrother->pRbnParent->uiColor = RB_RED;
                __hoitRbRightRotate(pRbTree, RB_PARENT(pRbn));
                pRbnBrother = RB_LEFT_CHILD(RB_PARENT(pRbn));
            }
            if(RB_LEFT_CHILD(pRbnBrother)->uiColor == RB_BLACK 
               && RB_RIGHT_CHILD(pRbnBrother)->uiColor == RB_BLACK){
                pRbnBrother->uiColor = RB_RED;
                pRbn = RB_PARENT(pRbn);
            }
            else if (RB_LEFT_CHILD(pRbnBrother)->uiColor == RB_BLACK)
            {
                pRbnBrother->pRbnRight->uiColor = RB_BLACK;
                pRbnBrother->uiColor = RB_RED;
                __hoitRbLeftRotate(pRbTree, pRbnBrother);
                pRbnBrother = RB_LEFT_CHILD(RB_PARENT(pRbn));

                pRbnBrother->uiColor = RB_PARENT(pRbn)->uiColor;
                pRbn->pRbnParent->uiColor = RB_BLACK;
                RB_LEFT_CHILD(pRbnBrother)->uiColor = RB_BLACK;
                __hoitRbRightRotate(pRbTree, RB_PARENT(pRbn));
                pRbn = pRbTree->pRbnRoot;
            }
            else {
                pRbnBrother->uiColor = RB_PARENT(pRbn)->uiColor;
                pRbn->pRbnParent->uiColor = RB_BLACK;
                RB_LEFT_CHILD(pRbnBrother)->uiColor = RB_BLACK;
                __hoitRbRightRotate(pRbTree, RB_PARENT(pRbn));
                pRbn = pRbTree->pRbnRoot;
            }
        }
        
    }
    pRbn->uiColor = RB_BLACK;
}
/*********************************************************************************************************
** 函数名称: __hoitRbTraverse
** 功能描述: 中序遍历红黑树
** 输　入  : pRbTree          红黑树
**           pRbnRoot          树根
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoitRbTraverse(PHOIT_RB_TREE pRbTree, PHOIT_RB_NODE pRbnRoot){
    if(pRbnRoot == pRbTree->pRbnGuard){
        return;
    }
    else
    {
        __hoitRbTraverse(pRbTree, pRbnRoot->pRbnLeft);
        printf("%d: %s left: %s right: %s\n", pRbnRoot->iKey, 
                                              pRbnRoot->uiColor == RB_RED ? "RED" : "BLACK", 
                                              RB_LEFT_CHILD(pRbnRoot)->uiColor == RB_RED ? "RED" : "BLACK", 
                                              RB_RIGHT_CHILD(pRbnRoot)->uiColor == RB_RED ? "RED" : "BLACK");
        __hoitRbTraverse(pRbTree, pRbnRoot->pRbnRight);   
    }
}


/*********************************************************************************************************
** 函数名称: hoitRbInsertNode
** 功能描述: 插入一个红黑树节点
** 输　入  : pRbTree          红黑树
**           pRbn               待插入节点              
** 输　出  : 返回插入的节点
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_RB_NODE hoitRbInsertNode(PHOIT_RB_TREE pRbTree, PHOIT_RB_NODE pRbn){
    PHOIT_RB_NODE       pRbnTrailing;
    PHOIT_RB_NODE       pRbnTraverse;

    pRbnTrailing = pRbTree->pRbnGuard;
    pRbnTraverse = pRbTree->pRbnRoot;
    
    while (pRbnTraverse != pRbTree->pRbnGuard)
    {
        pRbnTrailing = pRbnTraverse;
        if(pRbn->iKey < pRbnTraverse->iKey){
            pRbnTraverse = pRbnTraverse->pRbnLeft;
        }
        else pRbnTraverse = pRbnTraverse->pRbnRight;
    }

    pRbn->pRbnParent = pRbnTrailing;

    if(pRbnTrailing == pRbTree->pRbnGuard){
        pRbTree->pRbnRoot = pRbn;
    }
    else if(pRbn->iKey < pRbnTrailing->iKey){
        pRbnTrailing->pRbnLeft = pRbn;
    }
    else pRbnTrailing->pRbnRight = pRbn;

    pRbn->pRbnLeft = pRbTree->pRbnGuard;
    pRbn->pRbnRight = pRbTree->pRbnGuard;
    pRbn->uiColor = RB_RED;
    __hoitRbInsertFixUp(pRbTree, pRbn);

    return pRbn;
}


/*********************************************************************************************************
** 函数名称: hoitRbSearchNode
** 功能描述: 根据键值查找红黑树节点
** 输　入  : pRbTree          红黑树
**           iKey              键值              
** 输　出  : 成功返回节点指针，否则返回LW_NULL
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_RB_NODE hoitRbSearchNode(PHOIT_RB_TREE pRbTree, INT32 iKey){
    PHOIT_RB_NODE       pRbnTraverse;

    pRbnTraverse = pRbTree->pRbnRoot;

    while (pRbnTraverse != pRbTree->pRbnGuard)
    {
        if(pRbnTraverse->iKey == iKey){
            return pRbnTraverse;
        }
        else if(pRbnTraverse->iKey <= iKey){
            pRbnTraverse = RB_RIGHT_CHILD(pRbnTraverse);
        }
        else{
            pRbnTraverse = RB_LEFT_CHILD(pRbnTraverse);
        }
    }
    return pRbTree->pRbnGuard;
}

/*********************************************************************************************************
** 函数名称: hoitRbDeleteNode
** 功能描述: 删除一个红黑树节点，注意，我们并不会释放内存，这是为了后面着想
** 输　入  : pRbTree          红黑树
**           pRbn             待删除节点              
** 输　出  : 成功返回True，失败返回False
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL hoitRbDeleteNode(PHOIT_RB_TREE pRbTree, PHOIT_RB_NODE pRbn){
    PHOIT_RB_NODE           pRbnTraverse;
    PHOIT_RB_NODE           pRbnConqueror;
    UINT32                  uiTraverseOriginColor;

    pRbnTraverse = pRbn;                            /* 记录待删除节点颜色 */
    uiTraverseOriginColor = pRbnTraverse->uiColor;
    /* 待删除节点的右孩子为空，则直接移动 (同样适于LC是NIL的情况)
              |                             |
            pRbn            =>             LC
            /  \                          /  \
           LC   NIL
    */
    if(RB_RIGHT_CHILD(pRbn) == pRbTree->pRbnGuard){
        pRbnConqueror = RB_LEFT_CHILD(pRbn);
        __hoitRbConquer(pRbTree, pRbn, pRbnConqueror);
    }
    /* 待删除节点的左孩子为空，则直接移动 (同样适于RC是NIL的情况)
              |                             |
            pRbn            =>             RC
            /  \                          /  \
           NIL RC  
    */
    else if (RB_LEFT_CHILD(pRbn) == pRbTree->pRbnGuard)
    {
        pRbnConqueror = RB_RIGHT_CHILD(pRbn);
        __hoitRbConquer(pRbTree, pRbn, pRbnConqueror);
    }
    else
    {
        pRbnTraverse = __hoitRbMinimum(pRbTree, RB_RIGHT_CHILD(pRbn));        /* 寻找后继元素 */
        uiTraverseOriginColor = pRbnTraverse->uiColor;                        /* 删除pRbn节点，相当于把pRbn的值与后继节点交换，然后删除后继节点，所以这里更换删除节点的颜色*/
        pRbnConqueror = RB_RIGHT_CHILD(pRbnTraverse);                             
        
        if(RB_PARENT(pRbnTraverse) == pRbn){                                  /* https://www.zhihu.com/question/38296405 */
            pRbnConqueror->pRbnParent = pRbnTraverse;                           /* pRbnConqueror可能为pRbTree.Guard */
        }
        else
        {
            /* 待删除节点的左孩子为空，则直接移动 (同样适于LC是NIL的情况)
                      |                             |                               |
                    pRbn            =>             pRbn  S          =>              S
                    /  \                          /  \  /                         /  \
                   LC  RC                        LC  RC                          LC  RC
                     ... \                         ... \                           ... \
                     /                             /                                /
                    S                             SRC                             SRC
                  /  \                           /  \                            /  \
                NIL  SRC
            */
            __hoitRbConquer(pRbTree, pRbnTraverse, pRbnConqueror);
            pRbnTraverse->pRbnRight = RB_RIGHT_CHILD(pRbn);
            pRbnTraverse->pRbnRight->pRbnParent = pRbnTraverse;
        }
        
        __hoitRbConquer(pRbTree, pRbn, pRbnTraverse);
        pRbnTraverse->pRbnLeft = pRbn->pRbnLeft;
        pRbnTraverse->pRbnLeft->pRbnParent = pRbnTraverse;
        pRbnTraverse->uiColor = pRbn->uiColor;                              /* 因为交换，所以把S的颜色换一下 */
    }
    
    if(uiTraverseOriginColor == RB_BLACK){          /* 删除红节点不会影响树的平衡，删除黑节点要影响，这里需要调整 */
        __hoitRbDeleteFixUp(pRbTree, pRbnConqueror);
    }

    return LW_TRUE;
}

/*********************************************************************************************************
** 函数名称: hoitInitRbTree
** 功能描述: 红黑树初始化
** 输　入  : NONE
** 输　出  : 成功返回True，失败返回False
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_RB_TREE hoitRbInitTree(){
    PHOIT_RB_TREE           pRbTree; 
    PHOIT_RB_NODE           pRbNullNode;

    pRbTree = (PHOIT_RB_TREE)lib_malloc(sizeof(HOIT_RB_TREE));
    pRbTree->pRbnGuard      = (PHOIT_RB_NODE)lib_malloc(sizeof(HOIT_RB_NODE));
    pRbNullNode             = (PHOIT_RB_NODE)lib_malloc(sizeof(HOIT_RB_NODE));      /* 2021-05-15 添加 */
    pRbNullNode->uiColor    = PX_ERROR;                                         

    if(pRbTree < 0 || pRbTree->pRbnGuard < 0){
        printf("init red/black tree fail: memory low\n");
        return LW_NULL;
    }
    pRbTree->pRbnGuard->uiColor     = RB_BLACK;
    pRbTree->pRbnGuard->pRbnLeft    = pRbNullNode;
    pRbTree->pRbnGuard->pRbnRight   = pRbNullNode;
    pRbTree->pRbnRoot               = pRbTree->pRbnGuard;
    return pRbTree;
}

#ifdef RB_TEST
VOID hoitRbTreeTest(){
    INT i;
    PHOIT_RB_TREE pRbTree;
    PHOIT_RB_NODE pRbn;
    //INT testArray[10] = {8,11,14,15,1,2,4,5,7, 0};
    INT testArray[10] = {0, 1,2,3, 4,5,7,11,14,15};
    
    printf("\n [Red Black Tree Test Start] \n");
    pRbTree = hoitRbInitTree();
    
    for (i = 0; i < 10; i++)
    {
        if(testArray[i] == 5){
            printf("\n");
        }
        pRbn = newHoitRbNode(testArray[i]);
        hoitRbInsertNode(pRbTree, pRbn);
    }
    pRbn = hoitRbSearchNode(pRbTree, 7);
    
    printf("[test traverse] \n");
    __hoitRbTraverse(pRbTree, pRbTree->pRbnRoot);
    printf("[test delete 7] \n");
    
    hoitRbDeleteNode(pRbTree, pRbn);
    lib_free(pRbn);
    
    __hoitRbTraverse(pRbTree, pRbTree->pRbnRoot);
    
}
#endif // DEBUG
