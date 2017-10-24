#include "BaseVector.h"
#include "BaseDebug.h"
#include "BaseMemory.h"
#include "BaseSize.h"
#include "BaseType.h"
#include <string.h>
#include <vector>


pBitSideNode BitSideNodeNew()
{
    pBitSideNode pBS = (pBitSideNode)DwmMalloc(sizeof(eBitSideNode));

    if (wmNULL == pBS)
        return wmNULL;

    memset(pBS, 0x00, sizeof(pBS));
    pBS->nCnt = 0;
    pBS->eBitSideHead = wmNULL;
    pBS->mutexLock = WMCreateLock(PTRVECT_LOCK_NAME);
    return pBS;
}

wmVOID BitSideNodeDelete(pBitSideNode pVector)
{
    DWMDEBUG("");
    if (wmNULL == pVector)
    {
        DWMERROR("wmNULL == pVector");
        return;
    }
    WMLock(pVector->mutexLock);

    if (pVector->eBitSideHead != wmNULL)
    {
        pBitSide ptail = pVector->eBitSideHead;

        while (wmNULL != ptail)
        {
            pBitSide ptemp = ptail;
            ptail = ptail->next;
            if(ptemp->pData != wmNULL)
            {
               DwmFree(ptemp->pData);
            }
            DwmFree(ptemp);
        }
    }
    
    WMUnLock(pVector->mutexLock);
    WMDestroyLock(pVector->mutexLock);
    DwmFree(pVector);
}

wmVOID AddBitSideHead(pBitSideNode eBSL, wmULONG llbitSide, wmULONG uLenS ,const wmVOID* pvData)
{
    if (wmNULL == eBSL)
    {
        DWMERROR("wmNULL == eBSL");
        return;
    }

    pBitSide pBST = (pBitSide)DwmMalloc(sizeof(eBitSide));
    if(wmNULL == pBST)
    {
        DWMERROR("wmNULL == pBST");
        return;
    }
       

    pBST->pData = (wmVOID*)DwmMalloc(uLenS);
    if(wmNULL == pBST->pData)
    {
        DWMERROR("wmNULL == pBST->pData");
        return;
    }
    DwmMemset(pBST->pData,0x0,sizeof(pBST->pData));
    DwmMemcpy(pBST->pData,pvData,0,uLenS);
    pBST->bitSide = llbitSide;
    pBST->uLen = uLenS;
    pBST->next = wmNULL;

    WMLock(eBSL->mutexLock);

    if(wmNULL == eBSL->eBitSideHead)
    {
        eBSL->eBitSideHead = pBST;
    }
    else
    {
        unsigned long i = 0;
        pBitSide pTemp = wmNULL;
        pBitSide ptail = eBSL->eBitSideHead;

        while (wmNULL != ptail->next)
        {
            ptail = ptail->next;
            i++;
        }

        pTemp = ptail->next;
        ptail->next = pBST;
        pBST->next = pTemp;
    }
    eBSL->nCnt++;
    WMUnLock(eBSL->mutexLock);
}


wmVOID InsertBitSideHead(pBitSideNode eBSL, wmULONG  llbitSide, wmULONG uLenS ,const wmVOID* pvData, wmULONG bst)
{
    DWMDEBUG("");
    if (wmNULL == eBSL)
    {
        DWMERROR("wmNULL == eBSL");
        return;
    }
    
    pBitSide pBST = (pBitSide)DwmMalloc(sizeof(eBitSide));
    if(wmNULL == pBST)
    {
        DWMERROR("wmNULL == pBST");
        return;
    }

    pBST->pData = (wmVOID*)DwmMalloc(sizeof(pvData));
    if(wmNULL == pBST->pData)
    {
        DWMERROR("wmNULL == pBST->pData");
        return;
    }
    DwmMemset(pBST->pData,0x0,sizeof(pBST->pData));
    DwmMemcpy(pBST->pData,pvData,0,sizeof(pBST->pData));
    
    pBST->bitSide = llbitSide;
    pBST->uLen = uLenS;
    pBST->next = wmNULL;

    WMLock(eBSL->mutexLock);

    if(wmNULL == eBSL->eBitSideHead)
    {
        eBSL->eBitSideHead = pBST;
    }
    else
    {
        unsigned long interi = 0;
        pBitSide pTemp = wmNULL;
        pBitSide ptail = eBSL->eBitSideHead;

        while (wmNULL != ptail->next && interi < bst)
        {
            ptail = ptail->next;
            interi++;
        }

        pTemp = ptail->next;
        ptail->next = pBST;
        pBST->next = pTemp;
    }
    eBSL->nCnt++;
    WMUnLock(eBSL->mutexLock);
}


pBitSide BitSideNodeRemove(pBitSideNode pVector)
{
    if (wmNULL == pVector ||wmNULL == pVector->eBitSideHead  )
        return wmNULL;
    else
    {
        WMLock(pVector->mutexLock);
        pBitSide pNode = pVector->eBitSideHead;
        pVector->eBitSideHead = pVector->eBitSideHead->next;
        pVector->nCnt--;
        WMUnLock(pVector->mutexLock);
        return pNode;
    }
}


pBitSide BitSideNodeGetFromFlag(pBitSideNode pVector,wmULONG llbitSide)
{
    DWMDEBUG("");
    if (wmNULL == pVector ||wmNULL == pVector->eBitSideHead)
    {
        DWMERROR("wmNULL == pVector");
        return wmNULL;
    }
    WMLock(pVector->mutexLock);

    if (pVector->eBitSideHead != wmNULL)
    {
        pBitSide pNode = pVector->eBitSideHead;

        while (wmNULL != pNode)
        {
            if(llbitSide == pNode->bitSide)
            {
                WMUnLock(pVector->mutexLock);
                return pNode;
            }
            pNode = pNode->next;
        }
    }
    
    WMUnLock(pVector->mutexLock);
    return wmNULL;
}

wmULONG BitSideNodeGetCount(pBitSideNode pVector)
{
    if (wmNULL == pVector)
        return 0;
    return pVector->nCnt;
}

void BitSideDelete(pBitSide pBSide)
{
    if(wmNULL != pBSide)
    {
        if(wmNULL != pBSide->pData)
        {
            DwmFree(pBSide->pData);
        }
        DwmFree(pBSide);
    }
}

