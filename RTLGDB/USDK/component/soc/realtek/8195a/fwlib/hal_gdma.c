/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */


#include "rtl8195a.h"
#include "hal_gdma.h"

#ifdef CONFIG_GDMA_EN

#define MAX_GDMA_INDX       1
#define MAX_GDMA_CHNL       6

static uint8_t HalGdmaReg[MAX_GDMA_INDX+1];

const HAL_GDMA_CHNL GDMA_Chnl_Option[] = { 
        {0,0,GDMA0_CHANNEL0_IRQ,0},
        {1,0,GDMA1_CHANNEL0_IRQ,0},
        {0,1,GDMA0_CHANNEL1_IRQ,0},
        {1,1,GDMA1_CHANNEL1_IRQ,0},
        {0,2,GDMA0_CHANNEL2_IRQ,0},
        {1,2,GDMA1_CHANNEL2_IRQ,0},
        {0,3,GDMA0_CHANNEL3_IRQ,0},
        {1,3,GDMA1_CHANNEL3_IRQ,0},
        {0,4,GDMA0_CHANNEL4_IRQ,0},
        {1,4,GDMA1_CHANNEL4_IRQ,0},
        {0,5,GDMA0_CHANNEL5_IRQ,0},
        {1,5,GDMA1_CHANNEL5_IRQ,0},
        
        {0xff,0,0,0}    // end
};

const HAL_GDMA_CHNL GDMA_Multi_Block_Chnl_Option[] = { 
        {0,4,GDMA0_CHANNEL4_IRQ,0},
        {1,4,GDMA1_CHANNEL4_IRQ,0},
        {0,5,GDMA0_CHANNEL5_IRQ,0},
        {1,5,GDMA1_CHANNEL5_IRQ,0},
        
        {0xff,0,0,0}    // end
};


const uint16_t HalGdmaChnlEn[6] = {
    GdmaCh0, GdmaCh1, GdmaCh2, GdmaCh3,
    GdmaCh4, GdmaCh5
};


    
void HalGdmaOpInit(
    IN  void *Data
)
{
    PHAL_GDMA_OP pHalGdmaOp = (PHAL_GDMA_OP) Data;

    pHalGdmaOp->HalGdmaOnOff = HalGdmaOnOffRtl8195a;
    pHalGdmaOp->HalGdamChInit = HalGdamChInitRtl8195a;
    pHalGdmaOp->HalGdmaChDis = HalGdmaChDisRtl8195a;
    pHalGdmaOp->HalGdmaChEn = HalGdmaChEnRtl8195a;
    pHalGdmaOp->HalGdmaChSeting = HalGdmaChSetingRtl8195a;
#ifndef CONFIG_CHIP_E_CUT
    pHalGdmaOp->HalGdmaChBlockSeting = HalGdmaChBlockSetingRtl8195a_Patch;
#else
    pHalGdmaOp->HalGdmaChBlockSeting = HalGdmaChBlockSetingRtl8195a_V04;
#endif
    pHalGdmaOp->HalGdmaChIsrEnAndDis = HalGdmaChIsrEnAndDisRtl8195a;
    pHalGdmaOp->HalGdmaChIsrClean = HalGdmaChIsrCleanRtl8195a;
    pHalGdmaOp->HalGdmaChCleanAutoSrc = HalGdmaChCleanAutoSrcRtl8195a;
    pHalGdmaOp->HalGdmaChCleanAutoDst = HalGdmaChCleanAutoDstRtl8195a;
}

void HalGdmaOn(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
    pHalGdmaAdapter->GdmaOnOff = ON;
    HalGdmaOnOffRtl8195a((void*)pHalGdmaAdapter);
}

void HalGdmaOff(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
    pHalGdmaAdapter->GdmaOnOff = OFF;
    HalGdmaOnOffRtl8195a((void*)pHalGdmaAdapter);
}

BOOL HalGdmaChInit(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
    return (HalGdamChInitRtl8195a((void*)pHalGdmaAdapter));
}

void HalGdmaChDis(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
    HalGdmaChDisRtl8195a((void*)pHalGdmaAdapter);
}

void HalGdmaChEn(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
    HalGdmaChEnRtl8195a((void*)pHalGdmaAdapter);
}

BOOL HalGdmaChSeting(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
    return (HalGdmaChSetingRtl8195a((void*)pHalGdmaAdapter));
}

BOOL HalGdmaChBlockSeting(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
#ifndef CONFIG_CHIP_E_CUT
    return (HalGdmaChBlockSetingRtl8195a_Patch((void*)pHalGdmaAdapter));
#else
    return (HalGdmaChBlockSetingRtl8195a_V04((void*)pHalGdmaAdapter));
#endif
}

void HalGdmaChIsrEn(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
    pHalGdmaAdapter->IsrCtrl = ENABLE;
    HalGdmaChIsrEnAndDisRtl8195a((void*)pHalGdmaAdapter);
}

void HalGdmaChIsrDis(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
    pHalGdmaAdapter->IsrCtrl = DISABLE;
    HalGdmaChIsrEnAndDisRtl8195a((void*)pHalGdmaAdapter);
}

uint8_t HalGdmaChIsrClean(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
    return (HalGdmaChIsrCleanRtl8195a((void*)pHalGdmaAdapter));
}

void HalGdmaChCleanAutoSrc(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
    HalGdmaChCleanAutoSrcRtl8195a((void*)pHalGdmaAdapter);
}

void HalGdmaChCleanAutoDst(PHAL_GDMA_ADAPTER pHalGdmaAdapter)
{
    HalGdmaChCleanAutoDstRtl8195a((void*)pHalGdmaAdapter);
}

HAL_Status HalGdmaChnlRegister (uint8_t GdmaIdx, uint8_t ChnlNum)
{
    uint32_t mask;

    if ((GdmaIdx > MAX_GDMA_INDX) || (ChnlNum > MAX_GDMA_CHNL)) {
        // Invalid GDMA Index or Channel Number
        return HAL_ERR_PARA;
    }

    mask = 1 << ChnlNum;

    if ((HalGdmaReg[GdmaIdx] & mask) != 0) {
        return HAL_BUSY;
    }
    else {
#if 1
        if (HalGdmaReg[GdmaIdx] == 0) {
            if (GdmaIdx == 0) {
                ACTCK_GDMA0_CCTRL(ON);
                GDMA0_FCTRL(ON);
            }
            else {
                ACTCK_GDMA1_CCTRL(ON);
                GDMA1_FCTRL(ON);
            }
        }
#endif    
        HalGdmaReg[GdmaIdx] |= mask;
        return HAL_OK;
    }
}

void HalGdmaChnlUnRegister (uint8_t GdmaIdx, uint8_t ChnlNum)
{
    uint32_t mask;

    if ((GdmaIdx > MAX_GDMA_INDX) || (ChnlNum > MAX_GDMA_CHNL)) {
        // Invalid GDMA Index or Channel Number
        return;
    }
    
    mask = 1 << ChnlNum;

    HalGdmaReg[GdmaIdx] &= ~mask;
#if 1
    if (HalGdmaReg[GdmaIdx] == 0) {
        if (GdmaIdx == 0) {
            ACTCK_GDMA0_CCTRL(OFF);
            GDMA0_FCTRL(OFF);
        }
        else {
            ACTCK_GDMA1_CCTRL(OFF);
            GDMA1_FCTRL(OFF);
        }
    }
#endif    
}

PHAL_GDMA_CHNL HalGdmaChnlAlloc (HAL_GDMA_CHNL *pChnlOption)
{
    HAL_GDMA_CHNL *pgdma_chnl;

    pgdma_chnl = pChnlOption;
    if (pChnlOption == NULL) {
        // Use default GDMA Channel Option table
        pgdma_chnl = (HAL_GDMA_CHNL*)&GDMA_Chnl_Option[0];
    }
    else{
        pgdma_chnl = (HAL_GDMA_CHNL*) pgdma_chnl;
    }

    while (pgdma_chnl->GdmaIndx <= MAX_GDMA_INDX) {
        if (HalGdmaChnlRegister(pgdma_chnl->GdmaIndx, pgdma_chnl->GdmaChnl) == HAL_OK) {
            // This GDMA Channel is available
            break;
        }
        pgdma_chnl += 1;
    }

    if (pgdma_chnl->GdmaIndx > MAX_GDMA_INDX) {
        pgdma_chnl = NULL;
    }
    
    return pgdma_chnl;
}

void HalGdmaChnlFree (HAL_GDMA_CHNL *pChnl)
{
    IRQ_HANDLE IrqHandle;

    IrqHandle.IrqNum = pChnl->IrqNum;
    InterruptDis(&IrqHandle);
    InterruptUnRegister(&IrqHandle);
    HalGdmaChnlUnRegister(pChnl->GdmaIndx, pChnl->GdmaChnl);
}

void HalGdmaMemIrqHandler(void *pData)
{
    PHAL_GDMA_OBJ pHalGdmaObj=(PHAL_GDMA_OBJ)pData;
    PHAL_GDMA_ADAPTER pHalGdmaAdapter;
    PIRQ_HANDLE pGdmaIrqHandle;
    
    pHalGdmaAdapter = &(pHalGdmaObj->HalGdmaAdapter);
    pGdmaIrqHandle = &(pHalGdmaObj->GdmaIrqHandle);
    // Clean Auto Reload Bit
    HalGdmaChCleanAutoDst((void*)pHalGdmaAdapter);

    // Clear Pending ISR
    HalGdmaChIsrClean((void*)pHalGdmaAdapter);

    HalGdmaChDis((void*)(pHalGdmaAdapter));
    pHalGdmaObj->Busy = 0;

    if (pGdmaIrqHandle->IrqFun != NULL) {
        pGdmaIrqHandle->IrqFun((void*)pGdmaIrqHandle->Data);
    }
}

BOOL HalGdmaMemCpyAggrInit(PHAL_GDMA_OBJ pHalGdmaObj)
{
    HAL_GDMA_CHNL *pgdma_chnl;
    PHAL_GDMA_ADAPTER pHalGdmaAdapter;
    PIRQ_HANDLE pGdmaIrqHandle;
    IRQ_HANDLE IrqHandle;

    pgdma_chnl = HalGdmaChnlAlloc((PHAL_GDMA_CHNL) &GDMA_Multi_Block_Chnl_Option[0]);    // get a whatever GDMA channel
    if (NULL == pgdma_chnl) {        
        DBG_GDMA_ERR("%s: Cannot allocate a GDMA Channel\n", __FUNCTION__);
        return _FALSE;
    }

    pHalGdmaAdapter = &(pHalGdmaObj->HalGdmaAdapter);
    pGdmaIrqHandle = &(pHalGdmaObj->GdmaIrqHandle);
    
    DBG_GDMA_INFO("%s: Use GDMA%d CH%d\n", __FUNCTION__, pgdma_chnl->GdmaIndx, pgdma_chnl->GdmaChnl);

    _memset((void *)pHalGdmaAdapter, 0, sizeof(HAL_GDMA_ADAPTER));

    pHalGdmaAdapter->GdmaCtl.TtFc = TTFCMemToMem;
    pHalGdmaAdapter->GdmaCtl.Done = 1;
    pHalGdmaAdapter->MuliBlockCunt = 0;
    pHalGdmaAdapter->MaxMuliBlock = 1;
    pHalGdmaAdapter->ChNum = pgdma_chnl->GdmaChnl;
    pHalGdmaAdapter->GdmaIndex = pgdma_chnl->GdmaIndx;
    pHalGdmaAdapter->ChEn = 0x0101 << pgdma_chnl->GdmaChnl;
    pHalGdmaAdapter->GdmaIsrType = (TransferType|ErrType);
    pHalGdmaAdapter->IsrCtrl = ENABLE;
    pHalGdmaAdapter->GdmaOnOff = ON;
    pHalGdmaAdapter->GdmaCtl.IntEn      = 1;
    pHalGdmaAdapter->Rsvd4to7 = 1;
    pHalGdmaAdapter->Llpctrl = 1;
    pGdmaIrqHandle->IrqNum = pgdma_chnl->IrqNum;
    pGdmaIrqHandle->Priority = 10;

    IrqHandle.IrqFun = (IRQ_FUN) HalGdmaMemIrqHandler;
    IrqHandle.Data = (uint32_t) pHalGdmaObj;
    IrqHandle.IrqNum = pGdmaIrqHandle->IrqNum;
    IrqHandle.Priority = pGdmaIrqHandle->Priority;

    InterruptRegister(&IrqHandle);
    InterruptEn(&IrqHandle);
    pHalGdmaObj->Busy = 0;
    
    return _TRUE;
}


void HalGdmaMultiBlockSetting(PHAL_GDMA_OBJ pHalGdmaObj, PHAL_GDMA_BLOCK pHalGdmaBlock)
{
    PHAL_GDMA_ADAPTER pHalGdmaAdapter;
    uint8_t BlockNumber;
    uint8_t BlockIndex;
    uint8_t FourBytesAlign;

    BlockNumber = pHalGdmaObj->BlockNum;
    pHalGdmaAdapter = &(pHalGdmaObj->HalGdmaAdapter);

    pHalGdmaAdapter->GdmaCtl.LlpSrcEn = 1;
    pHalGdmaAdapter->GdmaCtl.LlpDstEn = 1;

    if(((pHalGdmaBlock[0].SrcAddr & 0x03) == 0) &&((pHalGdmaBlock[0].DstAddr & 0x03) == 0) 
        && ((pHalGdmaBlock[0].BlockLength & 0X03) == 0)){   
        pHalGdmaAdapter->GdmaCtl.SrcMsize   = MsizeEight;
        pHalGdmaAdapter->GdmaCtl.SrcTrWidth = TrWidthFourBytes;
        pHalGdmaAdapter->GdmaCtl.DestMsize = MsizeEight;
        pHalGdmaAdapter->GdmaCtl.DstTrWidth = TrWidthFourBytes; 
        FourBytesAlign = 1;
    }
    else{
        pHalGdmaAdapter->GdmaCtl.SrcMsize   = MsizeEight;
        pHalGdmaAdapter->GdmaCtl.SrcTrWidth = TrWidthOneByte;
        pHalGdmaAdapter->GdmaCtl.DestMsize = MsizeEight;
        pHalGdmaAdapter->GdmaCtl.DstTrWidth = TrWidthOneByte;
        FourBytesAlign = 0;
    }
    
    for(BlockIndex = 0; BlockIndex < BlockNumber; BlockIndex++){
       
        pHalGdmaObj->GdmaChLli[BlockIndex].Sarx = pHalGdmaBlock[BlockIndex].SrcAddr;
        pHalGdmaObj->GdmaChLli[BlockIndex].Darx = pHalGdmaBlock[BlockIndex].DstAddr;
        pHalGdmaObj->BlockSizeList[BlockIndex].pNextBlockSiz = &pHalGdmaObj->BlockSizeList[BlockIndex + 1];

        if(FourBytesAlign){
            pHalGdmaObj->BlockSizeList[BlockIndex].BlockSize = pHalGdmaBlock[BlockIndex].BlockLength >> 2;
        }
        else{
            pHalGdmaObj->BlockSizeList[BlockIndex].BlockSize = pHalGdmaBlock[BlockIndex].BlockLength;
        }
        
        pHalGdmaObj->Lli[BlockIndex].pLliEle = (GDMA_CH_LLI_ELE*) &pHalGdmaObj->GdmaChLli[BlockIndex];
        pHalGdmaObj->Lli[BlockIndex].pNextLli = &pHalGdmaObj->Lli[BlockIndex + 1];


        if(BlockIndex == BlockNumber - 1){
            pHalGdmaObj->BlockSizeList[BlockIndex].pNextBlockSiz = NULL;
            pHalGdmaObj->Lli[BlockIndex].pNextLli = NULL;
        }
        //DBG_GDMA_INFO("Lli[%d].pLiEle = %x\r\n", BlockIndex,Lli[BlockIndex].pLliEle);
        //DBG_GDMA_INFO("Lli[%d].pNextLli = %x\r\n", BlockIndex,Lli[BlockIndex].pNextLli);
    }

    pHalGdmaAdapter->pBlockSizeList = (struct BLOCK_SIZE_LIST*) &pHalGdmaObj->BlockSizeList;
    pHalGdmaAdapter->pLlix = (struct GDMA_CH_LLI*) &pHalGdmaObj->Lli;
    //DBG_GDMA_INFO("pHalGdmaAdapter->pBlockSizeList = %x\r\n", pHalGdmaAdapter->pBlockSizeList);
    //DBG_GDMA_INFO("pHalGdmaAdapter->pLlix  = %x\r\n", pHalGdmaAdapter->pLlix );
}

void HalGdmaLLPMemAlign(PHAL_GDMA_OBJ pHalGdmaObj, PHAL_GDMA_BLOCK pHalGdmaBlock)
{
    PHAL_GDMA_ADAPTER pHalGdmaAdapter;
    PGDMA_CH_LLI_ELE pLliEle;
    struct GDMA_CH_LLI *pGdmaChLli;
    struct BLOCK_SIZE_LIST *pGdmaChBkLi;
    uint32_t CtlxLow;
    uint32_t CtlxUp;
    uint8_t BlockNumber;
    uint8_t BlockIndex;
    
    pHalGdmaAdapter = &(pHalGdmaObj->HalGdmaAdapter);
    BlockNumber = pHalGdmaObj->BlockNum;
    
    pLliEle = pHalGdmaAdapter->pLlix->pLliEle;
    pGdmaChLli = pHalGdmaAdapter->pLlix->pNextLli;
    pGdmaChBkLi = pHalGdmaAdapter->pBlockSizeList;
    
    //4 Move to the second block to configure Memory Alginment setting
    pLliEle->Llpx = (uint32_t) pGdmaChLli->pLliEle;
    pGdmaChBkLi = pGdmaChBkLi ->pNextBlockSiz;
    
    for(BlockIndex = 1; BlockIndex < BlockNumber; BlockIndex++){
        pLliEle = pGdmaChLli->pLliEle;
        CtlxLow = pLliEle->CtlxLow;
        CtlxLow &= (BIT_INVC_CTLX_LO_DST_TR_WIDTH & BIT_INVC_CTLX_LO_SRC_TR_WIDTH);
        CtlxUp = pLliEle->CtlxUp;    
        CtlxUp &= (BIT_INVC_CTLX_UP_BLOCK_BS);

        if(((pHalGdmaBlock[BlockIndex].SrcAddr & 0x03) == 0) &&((pHalGdmaBlock[BlockIndex].DstAddr & 0x03) == 0) 
            && ((pHalGdmaBlock[BlockIndex].BlockLength & 0X03) == 0)){
            pHalGdmaAdapter->GdmaCtl.SrcTrWidth = TrWidthFourBytes;
            pHalGdmaAdapter->GdmaCtl.DstTrWidth = TrWidthFourBytes;
            pGdmaChBkLi->BlockSize = pHalGdmaBlock[BlockIndex].BlockLength>> 2;

        }
        else{
            pHalGdmaAdapter->GdmaCtl.SrcTrWidth = TrWidthOneByte;
            pHalGdmaAdapter->GdmaCtl.DstTrWidth = TrWidthOneByte;
            pGdmaChBkLi->BlockSize = pHalGdmaBlock[BlockIndex].BlockLength;
        }

        CtlxLow |= (BIT_CTLX_LO_DST_TR_WIDTH(pHalGdmaAdapter->GdmaCtl.DstTrWidth) |
              BIT_CTLX_LO_SRC_TR_WIDTH(pHalGdmaAdapter->GdmaCtl.SrcTrWidth));
        CtlxUp |= BIT_CTLX_UP_BLOCK_BS(pGdmaChBkLi->BlockSize);  

        pGdmaChLli = pGdmaChLli->pNextLli;
        pGdmaChBkLi = pGdmaChBkLi->pNextBlockSiz;    
        pLliEle->CtlxLow = CtlxLow;
        pLliEle->CtlxUp = CtlxUp;
        pLliEle->Llpx = (uint32_t)(pGdmaChLli->pLliEle);

    }
}

void HalGdmaMemAggr(PHAL_GDMA_OBJ pHalGdmaObj, PHAL_GDMA_BLOCK pHalGdmaBlock)
{
    PHAL_GDMA_ADAPTER pHalGdmaAdapter;
    
    uint8_t BlockNumber;

    BlockNumber = pHalGdmaObj->BlockNum;
    pHalGdmaAdapter = &(pHalGdmaObj->HalGdmaAdapter);

    if (pHalGdmaObj->Busy) {
        DBG_GDMA_ERR("%s: ==> GDMA is Busy\r\n", __FUNCTION__);
        return;
    }
    pHalGdmaObj->Busy = 1;

    pHalGdmaAdapter->MaxMuliBlock = BlockNumber;
    pHalGdmaAdapter->ChSar = pHalGdmaBlock[0].SrcAddr;
    pHalGdmaAdapter->ChDar = pHalGdmaBlock[0].DstAddr;
    
    HalGdmaMultiBlockSetting(pHalGdmaObj, pHalGdmaBlock);    
    HalGdmaOn((pHalGdmaAdapter));
    HalGdmaChIsrEn((pHalGdmaAdapter));
    HalGdmaChBlockSeting((pHalGdmaAdapter));
    HalGdmaLLPMemAlign(pHalGdmaObj, pHalGdmaBlock);
    HalGdmaChEn((pHalGdmaAdapter));

}



BOOL HalGdmaMemCpyInit(PHAL_GDMA_OBJ pHalGdmaObj)
{
    HAL_GDMA_CHNL *pgdma_chnl;
    PHAL_GDMA_ADAPTER pHalGdmaAdapter;
    PIRQ_HANDLE pGdmaIrqHandle;
    IRQ_HANDLE IrqHandle;

    pgdma_chnl = HalGdmaChnlAlloc(NULL);    // get a whatever GDMA channel
    if (NULL == pgdma_chnl) {        
        DBG_GDMA_ERR("%s: Cannot allocate a GDMA Channel\n", __FUNCTION__);
        return _FALSE;
    }

    pHalGdmaAdapter = &(pHalGdmaObj->HalGdmaAdapter);
    pGdmaIrqHandle = &(pHalGdmaObj->GdmaIrqHandle);
    
    DBG_GDMA_INFO("%s: Use GDMA%d CH%d\n", __FUNCTION__, pgdma_chnl->GdmaIndx, pgdma_chnl->GdmaChnl);
#if 0
    if (pgdma_chnl->GdmaIndx == 0) {
        ACTCK_GDMA0_CCTRL(ON);
        GDMA0_FCTRL(ON);
    }
    else if (pgdma_chnl->GdmaIndx == 1) {
        ACTCK_GDMA1_CCTRL(ON);
        GDMA1_FCTRL(ON);
    }
#endif    
    _memset((void *)pHalGdmaAdapter, 0, sizeof(HAL_GDMA_ADAPTER));

//    pHalGdmaAdapter->GdmaCtl.TtFc = TTFCMemToMem;
    pHalGdmaAdapter->GdmaCtl.Done = 1;
//    pHalGdmaAdapter->MuliBlockCunt = 0;
//    pHalGdmaAdapter->MaxMuliBlock = 1;
    pHalGdmaAdapter->ChNum = pgdma_chnl->GdmaChnl;
    pHalGdmaAdapter->GdmaIndex = pgdma_chnl->GdmaIndx;
    pHalGdmaAdapter->ChEn = 0x0101 << pgdma_chnl->GdmaChnl;
    pHalGdmaAdapter->GdmaIsrType = (TransferType|ErrType);
    pHalGdmaAdapter->IsrCtrl = ENABLE;
    pHalGdmaAdapter->GdmaOnOff = ON;

    pHalGdmaAdapter->GdmaCtl.IntEn      = 1;
//    pHalGdmaAdapter->GdmaCtl.SrcMsize   = MsizeEight;
//    pHalGdmaAdapter->GdmaCtl.DestMsize  = MsizeEight;
//    pHalGdmaAdapter->GdmaCtl.SrcTrWidth = TrWidthFourBytes;
//    pHalGdmaAdapter->GdmaCtl.DstTrWidth = TrWidthFourBytes;
//    pHalGdmaAdapter->GdmaCtl.Dinc = IncType;
//    pHalGdmaAdapter->GdmaCtl.Sinc = IncType;

    pGdmaIrqHandle->IrqNum = pgdma_chnl->IrqNum;
    pGdmaIrqHandle->Priority = 10;

    IrqHandle.IrqFun = (IRQ_FUN) HalGdmaMemIrqHandler;
    IrqHandle.Data = (uint32_t) pHalGdmaObj;
    IrqHandle.IrqNum = pGdmaIrqHandle->IrqNum;
    IrqHandle.Priority = pGdmaIrqHandle->Priority;

    InterruptRegister(&IrqHandle);
    InterruptEn(&IrqHandle);
    pHalGdmaObj->Busy = 0;
    
    return _TRUE;
}

void HalGdmaMemCpyDeInit(PHAL_GDMA_OBJ pHalGdmaObj)
{
    HAL_GDMA_CHNL GdmaChnl;
    PHAL_GDMA_ADAPTER pHalGdmaAdapter;
    PIRQ_HANDLE pGdmaIrqHandle;

    pHalGdmaAdapter = &(pHalGdmaObj->HalGdmaAdapter);
    pGdmaIrqHandle = &(pHalGdmaObj->GdmaIrqHandle);

    GdmaChnl.GdmaIndx = pHalGdmaAdapter->GdmaIndex;
    GdmaChnl.GdmaChnl = pHalGdmaAdapter->ChNum;
    GdmaChnl.IrqNum = pGdmaIrqHandle->IrqNum;
    HalGdmaChnlFree(&GdmaChnl);
}

// If multi-task using the same GDMA Object, then it needs a mutex to protect this procedure
void* HalGdmaMemCpy(PHAL_GDMA_OBJ pHalGdmaObj, void* pDest, void* pSrc, uint32_t len)
{
    PHAL_GDMA_ADAPTER pHalGdmaAdapter;

    if (pHalGdmaObj->Busy) {
        DBG_GDMA_ERR("%s: ==> GDMA is Busy\r\n", __FUNCTION__);
        return 0;
    }
    pHalGdmaObj->Busy = 1;
    pHalGdmaAdapter = &(pHalGdmaObj->HalGdmaAdapter);

    DBG_GDMA_INFO("%s: ==> Src=0x%x Dst=0x%x Len=%d\r\n", __FUNCTION__, pSrc, pDest, len);
    if ((((uint32_t)pSrc & 0x03)==0) &&
        (((uint32_t)pDest & 0x03)==0) &&        
        ((len & 0x03)== 0)) {
        // 4-bytes aligned, move 4 bytes each transfer
        pHalGdmaAdapter->GdmaCtl.SrcMsize   = MsizeEight;
        pHalGdmaAdapter->GdmaCtl.SrcTrWidth = TrWidthFourBytes;
        pHalGdmaAdapter->GdmaCtl.DestMsize = MsizeEight;
        pHalGdmaAdapter->GdmaCtl.DstTrWidth = TrWidthFourBytes;
        pHalGdmaAdapter->GdmaCtl.BlockSize = len >> 2;
    }
    else {
        pHalGdmaAdapter->GdmaCtl.SrcMsize   = MsizeEight;
        pHalGdmaAdapter->GdmaCtl.SrcTrWidth = TrWidthOneByte;
        pHalGdmaAdapter->GdmaCtl.DestMsize = MsizeEight;
        pHalGdmaAdapter->GdmaCtl.DstTrWidth = TrWidthOneByte;
        pHalGdmaAdapter->GdmaCtl.BlockSize = len;
    }

    pHalGdmaAdapter->ChSar = (uint32_t)pSrc;
    pHalGdmaAdapter->ChDar = (uint32_t)pDest;                
    pHalGdmaAdapter->PacketLen = len;

    HalGdmaOn((pHalGdmaAdapter));
    HalGdmaChIsrEn((pHalGdmaAdapter));
    HalGdmaChSeting((pHalGdmaAdapter));
    HalGdmaChEn((pHalGdmaAdapter));

    return (pDest);
}

#endif // CONFIG_GDMA_EN
