/**
 * @file BaseVector.h
 * @brief all data
 * @author Fude
 * @version Ver 0.1 2016.02.26
 * 
 */
#ifndef __BASE_VECTOR_H__
#define __BASE_VECTOR_H__

#include "BaseDefine.h"
#include "BaseLock.h"

extern "C" {

typedef struct
{
    wmULONG nModId;
    wmULONG nIfId;
    wmULONG nRef;
} ModletMan;

typedef struct _tagBitSide {
    wmVOID* pData;
    wmULONG  bitSide;
    wmULONG uLen;
    struct _tagBitSide* next;
} eBitSide, *pBitSide;

typedef struct _tagBitSideNode {
    pBitSide eBitSideHead;
    wmULONG    nCnt;

    wmLock mutexLock;
} eBitSideNode, *pBitSideNode;

pBitSideNode BitSideNodeNew();
wmVOID BitSideNodeDelete(pBitSideNode pVector);
wmULONG BitSideNodeGetCount(pBitSideNode pVector);
pBitSide BitSideNodeGetFromFlag(pBitSideNode pVector,wmULONG llbitSide);
wmVOID AddBitSideHead(pBitSideNode eBSL, wmULONG  llbitSide,wmULONG uLenS ,const wmVOID* pvData);
wmVOID InsertBitSideHead(pBitSideNode eBSL, wmULONG  llbitSide, wmULONG uLenS ,const wmVOID* pvData, wmULONG bst);
pBitSide BitSideNodeRemove(pBitSideNode pVector);
wmVOID BitSideDelete(pBitSide pBSide);

}


#endif  /* __BASE_VECTOR_H__ */
