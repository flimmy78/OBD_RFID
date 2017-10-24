#include "json.h"
#include "baseDef.h"

#define JS_NULL_SIZE 5
#define JS_TRUE_SIZE 5
#define JS_FALSE_SIZE 6
#define JS_INT_SIZE 10
#define JS_UINT_SIZE 20
#define JS_LINT_SIZE 20
#define JS_FLOAT_SIZE 25
#define JS_DOUBLE_SIZE 30
#define JS_BOOL_SIZE 6
#define JS_UTF16_STR_MAX_SIZE 256
#define JS_MAX_STR_LEN (512*4096 - 1)
#define JS_FORMAT_CHECK_SWITCH 1
#define JS_NODE_MOD_NUM 256
#define JS_DIV(i) ((i)>>8) 
#define JS_MOD(i) ((i)&0xFF) 
#define JS_UTF162UTF8_SIZE(size) (3*(size))


#define JS_EMPTY

#define JS_PTR_CHECK(ptr, ret) \
    if(!(ptr)) \
    { \
        return ret; \
    } 
    
#define JS_MEM_ALLOC_CHECK(ptr, ret) \
    if(!(ptr)) \
    { \
        return ret; \
    } 



#define JS_MEMSET memset

typedef wmVOID jsNodeDataDelCbk(wmVOID* &pData);

#define GetNode(node, i) node[JS_DIV(i)][JS_MOD(i)]
template <class T> class jsNodeList {
public:
    jsNodeList(){
        pCbk = wmNULL;
        length = 0;
        JS_MEMSET(nodeArry, 0, sizeof(nodeArry));
    }
    ~jsNodeList(){
        clear();
        for(wmINT i = 0; i < JS_NODE_MOD_NUM; i++){
            if(nodeArry[i]){
                wmDELETE nodeArry[i];
            }
            else{
                break;
            }
        }
        pCbk = wmNULL;
        length = 0;
    }
    wmINT add(T* pData){
        if(!nodeArry[JS_DIV(length)]){
            nodeArry[JS_DIV(length)] = wmNEW T*[JS_NODE_MOD_NUM];
        }
        GetNode(nodeArry, length) = pData;
        length++;
        return length;
    }
    wmINT del(const wmINT & index){
        if(pCbk){
            pCbk((wmVOID* &)(getRef(index)));
        }
        if(index < length - 1){
            for(wmINT i = index; i < length - 1; i++){
                GetNode(nodeArry, i) = GetNode(nodeArry, i + 1);
            }
        }
        length--;
        return length;
    }
    T* get(const wmINT & index){
        return GetNode(nodeArry, index);
    }
    T* &getRef(const wmINT & index){
        return GetNode(nodeArry, index);
    }
    wmVOID clear(){
        for(wmINT i = length - 1; i >= 0; i--){
            del(i);
        }
    }
    wmINT getCnt(){
        return length;
    }
    wmVOID setJsNodeDelCbk(jsNodeDataDelCbk* cbk){
        pCbk = cbk;
    }
private:
    wmINT length;
    T** nodeArry[JS_NODE_MOD_NUM];
    jsNodeDataDelCbk* pCbk;
};

wmVOID JsonDelCbk(wmVOID* &pData){
    wmDELETE (Json*)pData;
    pData = wmNULL;
}

#define addJsNode(pList, pData) ((jsNodeList<Json>*)pList)->add(pData)
#define getJsNodeByIdx(pList, i) ((jsNodeList<Json>*)pList)->get(i)
#define getJsNodeCnt(pList) ((jsNodeList<Json>*)pList)->getCnt()
#define setJsNodeDelCbk(pList, cbk) ((jsNodeList<Json>*)pList)->setJsNodeDelCbk(cbk)
#define delJsNodeByIdx(pList, i) ((jsNodeList<Json>*)pList)->del(i)
#define clearJsNode(pList) ((jsNodeList<Json>*)pList)->clear()
#define JS_NODELIST_NEW(pList, ret) { \
            if(!pList){ \
                pList = wmNEW jsNodeList<Json>; \
                JS_MEM_ALLOC_CHECK(pList, ret) \
                setJsNodeDelCbk(pList, JsonDelCbk); \
            } \
        }

#define JS_NODELIST_DELETE(pList) { \
            wmDELETE (jsNodeList<Json>*)pList; \
            pList = wmNULL; \
        }

wmINT Json::js_strncpy(wmCHAR* pObj, const wmCHAR* pSrc, wmUINT n)
{
    //DWMDEBUG("");
    JS_PTR_CHECK(pObj, wmFalse)
    JS_PTR_CHECK(pSrc, wmFalse)
    while(n && (*pObj++ = *pSrc++) != '\0')
    {
        n--;
    }
    *(pObj - 1) = '\0';
    return wmTrue;
}

wmINT Json::js_wstrnlen(const wmUSHORT* pSrc, wmUINT n)
{
    //DWMDEBUG("");
    JS_PTR_CHECK(pSrc, 0)
    wmINT len = 0;
    while(n && pSrc[len++] != 0){
        n--;
    }
    len--;
    return len;
}

wmINT Json::js_wstrnlen(const wmCHAR* pSrc, wmUINT n)
{
    //DWMDEBUG("");
    JS_PTR_CHECK(pSrc, 0)
    wmUSHORT* pObj = wmNEW wmUSHORT[n];
    Utf8ToUtf16(pObj, pSrc, n);
    wmINT len = js_wstrnlen(pObj, n);
    freePtr(pObj);
    return len;
}

wmBOOL Json::Utf8ToUtf16(wmUSHORT* pObj, const wmCHAR* pSrc, wmUINT n)
{
    //DWMDEBUG("");
    JS_PTR_CHECK(pObj, wmFALSE)
    JS_PTR_CHECK(pSrc, wmFALSE)
    const wmCHAR* pTmp8 = pSrc;
    wmUSHORT* pTmp16 = pObj;
    while(*pTmp8 != '\0' &&  --n){
        if(*pTmp8 >= 0xE0 && *pTmp8 <=0xEF){
            *pTmp16 = 0;
            *pTmp16 |= ((*pTmp8++ & 0xEF) << 12);
            *pTmp16 |= ((*pTmp8++ & 0x3F) << 6);
            *pTmp16 |= (*pTmp8++ & 0x3F);
        }
        else if(*pTmp8 >= 0xC0 && *pTmp8 <=0xDF){
            *pTmp16 = 0;
            *pTmp16 |= ((*pTmp8++ & 0x1F) << 6);
            *pTmp16 |= (*pTmp8++ & 0x3F);
        }
        else if(*pTmp8 >= 0x00 && *pTmp8 <=0x7F){
            *pTmp16 = *pTmp8++;
        }
        else{
            return wmFALSE;
        }
        pTmp16++;
    }
    *pTmp16 = 0;
    return wmTRUE;
}

wmBOOL Json::Utf16ToUtf8(wmCHAR* pObj, const wmUSHORT* pSrc, wmUINT n)
{
    //DWMDEBUG("");
    JS_PTR_CHECK(pObj, wmFALSE)
    JS_PTR_CHECK(pSrc, wmFALSE)
    wmCHAR* pTmp8 = pObj;
    const wmUSHORT* pTmp16 = pSrc;
    while(*pTmp16 != 0 &&  --n){
        if(*pTmp16 <= 0x007F){
            *pTmp8++ = (wmCHAR)*pTmp16;
        }
        else if(*pTmp16 <= 0x07FF){
            *pTmp8++ = (*pTmp16 >> 6) | 0xC0;
            *pTmp8++ = (*pTmp16 & 0x3F) | 0x80;
        }
        else if(*pTmp16 <= 0xFFFF){
            *pTmp8++ = (*pTmp16 >> 12) |0xE0;
            *pTmp8++ = ((*pTmp16 >> 6) & 0x3F) | 0x80;
            *pTmp8++ = (*pTmp16 & 0x3F) | 0x80;
        }
        else{
            return wmFALSE;
        }
        pTmp16++;
    }
    *pTmp8 = 0;
    return wmTRUE;
}


Json Json::s_NullJson;
    
Json::Json(){
    //DWMDEBUG("");
    InitData();
    init(wmNull, wmFALSE);
}

Json::Json(const wmCHAR* pStr){
    //DWMDEBUG("");
    InitData();
    init(pStr);
}

Json::Json(const wmUSHORT* pStr){
    //DWMDEBUG("");
    InitData();
    wmINT size = js_wstrnlen(pStr, JS_UTF16_STR_MAX_SIZE) + 1;
    wmCHAR* pBuf = wmNEW wmCHAR[JS_UTF162UTF8_SIZE(size)];
    if(wmNULL == pBuf)
    {
        //DWMERROR("alloc memery failed");
        return;
    }
    Utf16ToUtf8(pBuf, pStr, size);
    init(pBuf);
    freePtr(pBuf);
}

Json::Json(const wmCHAR* pStr, const wmINT & len){
    //DWMDEBUG("");
    InitData();
    init(pStr, len);
}

Json::Json(Json& json){
    //DWMDEBUG("");
    InitData();
    init(json.c_str(), wmFALSE);
}

wmVOID Json::InitData(){
    //DWMDEBUG("");
    length = 0;
    m_length = 0;
    m_type = JS_MAX;
    m_pStr = wmNULL;
    m_ppKey = wmNULL;
    m_ppVal = wmNULL;
    m_pArray= wmNULL;
    m_pValue = wmNULL;
    m_pBuf = wmNULL;
    m_pDecode = wmNULL;
}

wmVOID Json::FreeData(){
    //DWMDEBUG("");
    freePtr(m_pStr);
    switch(m_type){
        case JS_OBJECT:
            for(wmINT i = 0; i < m_length; i++){
                freePtr(m_ppKey[i]);
                freePtr(m_ppVal[i]);
            }
            freePtr(m_ppKey);
            freePtr(m_ppVal);
            break;
        case JS_ARRAY:
            if(m_pArray){
                clearJsNode(m_pArray);
                JS_NODELIST_DELETE(m_pArray)
            }
            break;
        case JS_STRING:
            freePtr(m_pDecode);
            freePtr(m_pValue);
            break;
        case JS_DIGITAL:
        case JS_NULL:
        case JS_BOOL:
            freePtr(m_pValue);
            break;
        default:
            freePtr(m_pValue);
            break;
    }
    freePtr(m_pBuf);
    m_type = JS_MAX;
    length = 0;
    m_length = 0;
}

Json::~Json(){
    //DWMDEBUG("");
    FreeData();
}

wmVOID Json::trim(){
#if JS_FORMAT_CHECK_SWITCH
    //DWMDEBUG("");
    JS_PTR_CHECK(m_pStr, JS_EMPTY)
    wmINT len = strnlen(m_pStr, JS_MAX_STR_LEN);
    wmCHAR * pStr = wmNEW wmCHAR[len+1];
    JS_MEM_ALLOC_CHECK(pStr, JS_EMPTY)
    wmBOOL bDoubleQuoteFlag = wmFALSE;
    wmBOOL bEscapeFlag = wmFALSE;
    for(wmINT i = 0,j=0; i <= len; i++){
        switch(m_pStr[i]){
            case '\n':
            case '\t':
            case '\r':
            case ' ':
                if(bDoubleQuoteFlag){
                    pStr[j] = m_pStr[i];
                    j++;
                }
                break;
            case '\"':
                if(bDoubleQuoteFlag && (!bEscapeFlag)){
                    bDoubleQuoteFlag = wmFALSE;
                }
                else if((!bDoubleQuoteFlag) && (!bEscapeFlag)){
                    bDoubleQuoteFlag = wmTRUE;
                }
                else{
                    //do nothing
                }
                bEscapeFlag = wmFALSE;
                pStr[j] = m_pStr[i];
                j++;
                break;
            case '\\':
                if(bEscapeFlag){
                    bEscapeFlag = wmFALSE;
                }
                else{
                    bEscapeFlag = wmTRUE;
                }
                pStr[j] = m_pStr[i];
                j++;
                break;
            case '\0':
                pStr[j] = m_pStr[i];
                j++;
                break;
            default:
                bEscapeFlag = wmFALSE;
                pStr[j] = m_pStr[i];
                j++;
                break;         
        }
    }
    freePtr(m_pStr);
    m_pStr = pStr;
#endif
}

wmVOID Json::init(const wmCHAR* pStr, const wmBOOL bCheck){
    //DWMDEBUG("");
    FreeData();
    const wmCHAR* pTmpStr = wmNULL;
    if(pStr != wmNULL){
        pTmpStr = pStr;
    }else{
        pTmpStr = wmNull;
    }
    wmUINT size = strnlen(pTmpStr, JS_MAX_STR_LEN) + 1;
    m_pStr = wmNEW wmCHAR[size];
    JS_MEM_ALLOC_CHECK(m_pStr, JS_EMPTY)
    js_strncpy(m_pStr, pTmpStr, size);
    if(bCheck){
#if JS_FORMAT_CHECK_SWITCH
        trim();
#endif
    }
    parse();
    freePtr(m_pStr);
}

wmVOID Json::init(const wmCHAR* pStr, const wmINT & len, const wmBOOL bCheck){
    //DWMDEBUG("");
    FreeData();
    const wmCHAR* pTmpStr = wmNULL;
    if(pStr != wmNULL){
        pTmpStr = pStr;
    }else{
        pTmpStr = wmNull;
    }
    if(len < 0){
        return;
    }
    m_pStr = wmNEW wmCHAR[len+1];
    JS_MEM_ALLOC_CHECK(m_pStr, JS_EMPTY)
    js_strncpy(m_pStr, pTmpStr, len + 1);
    if(bCheck){
#if JS_FORMAT_CHECK_SWITCH
        trim();
#endif
    }
    parse();
    freePtr(m_pStr);
}

wmVOID Json::parse(){
    //DWMDEBUG("");
    wmINT len = strnlen(m_pStr, JS_MAX_STR_LEN);
    if('[' == m_pStr[0] && ']' == m_pStr[len -1]){
        m_type = JS_ARRAY; 
        JS_NODELIST_NEW(m_pArray, JS_EMPTY)
        if(2 == len){
            length = 0;
            return;
        }
        else{
            parseArray();
        }
    }
    else if('{' == m_pStr[0] && '}' == m_pStr[len -1]){
        m_type = JS_OBJECT; 
        if(2 == len){
            length = 0;
            m_length = 0;
            return;
        }
        parseObject();
    }
    else{
        parseValue();
    }
}

wmVOID Json::parseArray(){
    //DWMDEBUG("");
    wmINT i = 0;
    wmINT cnt = 0;
    wmCHAR* pStr = m_pStr;
    wmINT len = strnlen(pStr, JS_MAX_STR_LEN);
    i++;
    if('\"' == pStr[i] || 
        '[' ==  pStr[i] ||
        '{' == pStr[i] ||
        'n' == pStr[i] ||
        't' == pStr[i] ||
        'f' == pStr[i] ||
        '-' == pStr[i] ||
        (pStr[i] >= '0' && pStr[i] <= '9')){
        while(i < len){
            i += getEndChPos(pStr + i);
            i = i+2;
            cnt++;
        }
        length = cnt;
        i = 1;
        wmINT pos = 0;
        Json* pJson = wmNULL;
        for(wmINT j = 0; j < cnt; j++ ){
            pos = getEndChPos(pStr + i);
            pJson = wmNEW Json();
            JS_MEM_ALLOC_CHECK(pJson, JS_EMPTY)
            pJson->init(pStr + i, pos + 1, wmFALSE);
            addJsNode(m_pArray, pJson);
            i = i + pos + 2;
        }
    }
    else{
        return;
    }
}

wmVOID Json::parseObject(){
    //DWMDEBUG("");
    wmINT i = 1;
    wmINT cnt = 0;
    wmCHAR* pStr = m_pStr;
    wmINT len = strnlen(pStr, JS_MAX_STR_LEN);
    if('\"' == pStr[i]){
        wmINT pos = 0;
        while(i < len){
            i += getEndChPos(pStr + i);
            if(pStr[i+1] != ':'){
                DWMDEBUG("pStr[i+1] = [%s]",pStr+i);
                return;
            }
            else{
                i = i + 2;
                i += getEndChPos(pStr + i);
                i = i+2;
                cnt++;
            }
        }
        length = cnt;
        m_length = cnt;
        i = 1;
        m_ppKey = wmNEW wmCHAR* [cnt];
        JS_MEM_ALLOC_CHECK(m_ppKey, JS_EMPTY)
        m_ppVal = wmNEW Json* [cnt];
        JS_MEM_ALLOC_CHECK(m_ppVal, JS_EMPTY)
        for(wmINT j = 0; j < cnt; j++ ){
            pos = getEndChPos(pStr + i);
            m_ppKey[j] = wmNEW wmCHAR[pos];
            JS_MEM_ALLOC_CHECK(m_ppKey[j], JS_EMPTY)
            js_strncpy(m_ppKey[j], pStr + i + 1, pos);
            i = i + pos + 2;
            pos = getEndChPos(pStr + i);
            m_ppVal[j] = wmNEW Json();
            JS_MEM_ALLOC_CHECK(m_ppVal[j], JS_EMPTY)
            m_ppVal[j]->init(pStr + i, pos + 1, wmFALSE); 
            i = i + pos + 2;
        }
    }
    else if('[' == pStr[i]  ||
               '{' == pStr[i] ||
               '-' == pStr[i] ||
               (pStr[i] >= '0' && pStr[i] <= '9')){
        DWMWORNG("wrong start char");
    }
    else{
        DWMWORNG("unknown start char");
        return;
    }
}

wmVOID Json::parseValue(){
    //DWMDEBUG("");
    wmINT i = 0;
    wmCHAR* pStr = m_pStr;
    wmINT len = strnlen(pStr, JS_MAX_STR_LEN);
    if('\"' == pStr[0]){
        wmINT pos = getEndChPos(pStr + i);
        if(pos + i + 1 != len){
            return;
        }
        else{
            m_type = JS_STRING;
            m_pValue = wmNEW wmCHAR[pos];
            JS_MEM_ALLOC_CHECK(m_pValue, JS_EMPTY)
            js_strncpy(m_pValue, m_pStr+i + 1, pos);
        }
    }
    else if((pStr[0] >= '0' && pStr[0] <= '9')||
                 '-' == pStr[0]){
        wmINT pos = getEndChPos(pStr + i);
        if(pos + i + 1 != len){
            return;
        }
        else{
            m_type = JS_DIGITAL;
            m_pValue = wmNEW wmCHAR[pos+2];
            JS_MEM_ALLOC_CHECK(m_pValue, JS_EMPTY)
            js_strncpy(m_pValue, m_pStr+i, pos+2);
            trimEnd0(m_pValue);
        }
    }
    else if('n' == pStr[0]){
        if(i + JS_NULL_SIZE - 1 != len){
            return;
        }
        else{
            m_type = JS_NULL;
            m_pValue = wmNEW wmCHAR[JS_NULL_SIZE];
            JS_MEM_ALLOC_CHECK(m_pValue, JS_EMPTY)
            js_strncpy(m_pValue, m_pStr+i, JS_NULL_SIZE);
        }
    }
    else if('t' == pStr[0]){
        if(i + JS_TRUE_SIZE -1 != len){
            return;
        }
        else{
            m_type = JS_BOOL;
            m_pValue = wmNEW wmCHAR[JS_TRUE_SIZE];
            JS_MEM_ALLOC_CHECK(m_pValue, JS_EMPTY)
            js_strncpy(m_pValue, m_pStr+i, JS_TRUE_SIZE);
        }
    }
    else if('f' == pStr[0]){
        if(i + JS_FALSE_SIZE - 1 != len){
            return;
        }
        else{
            m_type = JS_BOOL;
            m_pValue = wmNEW wmCHAR[JS_FALSE_SIZE];
            JS_MEM_ALLOC_CHECK(m_pValue, JS_EMPTY)
            js_strncpy(m_pValue, m_pStr+i, JS_FALSE_SIZE);
        }
    }
    else{
        DWMWORNG("unknown start char code[%d]", pStr[0]);
        return;
    }

}

wmINT Json::getEndChPos(wmCHAR* pStr)
{
    //DWMDEBUG("");
    if(wmNULL == pStr){
        DWMWORNG("Err point pStr = %p\n",pStr);
        return -1;
    }
    wmINT len = strnlen(pStr, JS_MAX_STR_LEN);
    wmCHAR start = pStr[0];
    wmCHAR end = '\0';
    wmINT i = 1;
    wmINT pos = 0;
    if('[' == start){
        end = ']';
        while(i <= len){
            if(pStr[i] == end && '\\' != pStr[i - 1]){
                return i;
            }
            else if((('{' == pStr[i] ||
                       '[' == pStr[i] ||
                       '\"' == pStr[i]) && '\\'  != pStr[i - 1])){
                pos = getEndChPos(pStr + i);
                if(0 == pos){
                    return 0;
                }
                else{
                    i = i + pos + 1;
                }
            }
            else if('-' == pStr[i] ||
                       (pStr[i] >= '0' && pStr[i] <= '9')){
                pos = getEndChPos(pStr + i);
                i = i + pos + 1;
            }
            else{
                i++;
            }
        }
    }
    else if('{' == start){
        end = '}';
        while(i <= len){
            if(pStr[i] == end && '\\' != pStr[i - 1]){
                return i;
            }
            else if((('{' == pStr[i] ||
                       '[' == pStr[i] ||
                       '\"' == pStr[i]) && '\\'  != pStr[i - 1])||
                       '-' == pStr[i] ||
                       (pStr[i] >= '0' && pStr[i] <= '9')){
                pos = getEndChPos(pStr + i);
                if(-1 == pos){
                    return 0;
                }
                else{
                    i = i + pos + 1;
                }
            }
            else{
                i++;
            }
        }
    }
    else if('\"' == start){
        end = '\"';
        wmBOOL bEscapeFlag = wmFALSE;
        while(i <= len){
            if(pStr[i] == end &&
              (('\\' != pStr[i-1]) ||
               ('\\' == pStr[i-1] && (!bEscapeFlag)))
              ){
                return i;
            }
            else if('\\' == pStr[i]){
                if(bEscapeFlag){
                    bEscapeFlag = wmFALSE;
                }
                else{
                    bEscapeFlag = wmTRUE;
                }
            }
            else{
                bEscapeFlag = wmFALSE;
            }
            i++;
        }
        return 0;
    }
    else if((start>= '0' && start <= '9')||
                '-' == start){
        while(i <= len){
            if((pStr[i]< '0' || pStr[i]> '9') && pStr[i] != '.'){
                return i - 1;
            }
            i++;
        }
        return 0;
    }
    else if('n' == start){
        if(strnlen(pStr, JS_MAX_STR_LEN) < JS_NULL_SIZE - 1){
            return 0;
        }
        else if('u' == pStr[1] && 
            'l' == pStr[2] && 
            'l' == pStr[3]){
            return JS_NULL_SIZE - 2;
        }
        else{
            return 0;
        }
    }
    else if(start == 't'){
        if(strnlen(pStr, JS_MAX_STR_LEN) < JS_TRUE_SIZE - 1){
            return 0;
        }
        else if('r' == pStr[1] && 
            'u' == pStr[2] && 
            'e' == pStr[3]){
            return JS_TRUE_SIZE - 2;
        }
        else{
            return 0;
        }
    }
    else if('f' == start){
        if(strnlen(pStr, JS_MAX_STR_LEN) < JS_FALSE_SIZE - 1){
            return 0;
        }
        else if('a' == pStr[1] && 
            'l' == pStr[2] && 
            's' == pStr[3] &&
            'e' == pStr[4]){
            return JS_FALSE_SIZE - 2;
        }
        else{
            return 0;
        }
    }
    else{
        DWMWORNG("Err start code = %c\n",start);
        return 0;
    }
    return 0;
}

wmINT Json::getType(){
    //DWMDEBUG("");
    return m_type; 
}

Json &Json::operator [](const wmCHAR* pKey){
    //DWMDEBUG("");
    if(m_type != JS_OBJECT){
        DWMWORNG("This is not an object, return null Json object!!!\n");
        s_NullJson = (wmCHAR*)wmNULL;
        return s_NullJson;
    }
    for(wmINT i = 0; i < m_length; i++){
        if(strcmp(m_ppKey[i], pKey) == 0){
            return *m_ppVal[i];
        }
    }
    DWMWORNG("Cannot find the key of \"%s\", return it self!!!\n", pKey);
    return *this;
}

Json &Json::operator [](const wmINT & index){
    //DWMDEBUG("");
    if(m_type != JS_ARRAY){
        DWMWORNG("This is not an array, return null Json array!!!\n");
        s_NullJson = (wmCHAR*)wmNULL;
        return s_NullJson;
    }
    if(index >= getJsNodeCnt(m_pArray) || index < 0){
        DWMWORNG("The index=%d is not right, please check it, return itself!!!\n",index);
        return *this;
    }
    return *(getJsNodeByIdx(m_pArray, index));
}

wmVOID Json::setVal(const wmCHAR* pVal)
{
    //DWMDEBUG("");
    FreeData();
    if(wmNULL == pVal){
        m_pValue = wmNEW wmCHAR[JS_NULL_SIZE];
        JS_MEM_ALLOC_CHECK(m_pValue, JS_EMPTY)
        js_strncpy(m_pValue, wmNull, JS_NULL_SIZE);
    }
    else{
        wmINT size = strnlen(pVal, JS_MAX_STR_LEN) + 1;
        m_pValue = wmNEW wmCHAR[size];
        JS_MEM_ALLOC_CHECK(m_pValue, JS_EMPTY)
        js_strncpy(m_pValue, pVal, size);
    }
}

wmINT Json::setStr(const wmCHAR* pVal, const wmBOOL bEncode)
{
    //DWMDEBUG("");
    if(pVal){
        wmINT size = -1;
        if(bEncode){
            size  = encodeCheck(pVal);
        }
        if(size > 0){
            wmCHAR* pBuf = wmNEW wmCHAR[size];
            JS_MEM_ALLOC_CHECK(pBuf, wmFalse)
            wmINT ret = encode(pVal, pBuf, size, wmFALSE);
            if(wmTrue == ret){
                setVal(pBuf);
                freePtr(pBuf);
            }
            else{
                //DWMERROR("encode pVal[%s] failed\n", pVal);
                freePtr(pBuf);
                return ret;
            }
        }
        else{
            setVal(pVal);
        }
        m_type = JS_STRING;
    }
    else{
        setVal(pVal);
        m_type = JS_NULL;
    }
    return wmTrue;
}

wmINT Json::setStr(const wmINT val)
{
    //DWMDEBUG("");
    *this = val;
    m_type = JS_STRING;
    return wmTrue;
}

wmINT Json::setStr(const wmUINT val)
{
    //DWMDEBUG("");
    *this = val;
    m_type = JS_STRING;
    return wmTrue;
}

wmINT Json::setStr(const wmLONG val)
{
    //DWMDEBUG("");
    *this = val;
    m_type = JS_STRING;
    return wmTrue;
}

wmINT Json::setStr(const wmFLOAT val)
{
    //DWMDEBUG("");
    *this = val;
    m_type = JS_STRING;
    return wmTrue;
}

wmINT Json::setStr(const wmDOUBLE val)
{
    //DWMDEBUG("");
    *this = val;
    m_type = JS_STRING;
    return wmTrue;
}

wmINT Json::setStr(const wmBOOL val)
{
    //DWMDEBUG("");
    *this = val;
    m_type = JS_STRING;
    return wmTrue;
}
    
Json &Json::operator =(const wmCHAR* pVal)
{
    //DWMDEBUG("");
    setVal(pVal);
    if(pVal){
        m_type = JS_STRING;
    }
    else{
        m_type = JS_NULL;
    }
    return *this;
}


Json &Json::operator =(const wmUSHORT* pVal)
{
    //DWMDEBUG("");
    wmINT size = js_wstrnlen(pVal, JS_UTF16_STR_MAX_SIZE) + 1;
    wmCHAR* pBuf = wmNEW wmCHAR[JS_UTF162UTF8_SIZE(size)];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    Utf16ToUtf8(pBuf, pVal, size);
    setVal(pBuf);
    if(pBuf){
        m_type = JS_STRING;
    }
    else{
        m_type = JS_NULL;
    }
    freePtr(pBuf);    
    return *this;
}

Json &Json::operator =(const wmINT val){
    //DWMDEBUG("");
    wmCHAR* pBuf = wmNEW wmCHAR[JS_INT_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_INT_SIZE, "%d", val);
    pBuf[JS_INT_SIZE - 1] = '\0';
    setVal(pBuf);
    freePtr(pBuf);
    m_type = JS_DIGITAL;
    return *this;
}

Json &Json::operator =(const wmUINT  val){
    //DWMDEBUG("");
    wmCHAR* pBuf = wmNEW wmCHAR[JS_UINT_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_UINT_SIZE, "%d", val);
    pBuf[JS_UINT_SIZE - 1] = '\0';
    setVal(pBuf);
    freePtr(pBuf);
    m_type = JS_DIGITAL;
    return *this;
}

Json &Json::operator =(const wmLONG val){
    //DWMDEBUG("");
    wmCHAR* pBuf = wmNEW wmCHAR[JS_UTF16_STR_MAX_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_UTF16_STR_MAX_SIZE, "%ld", val);
    pBuf[JS_UTF16_STR_MAX_SIZE - 1] = '\0';
    setVal(pBuf);
    freePtr(pBuf);
    m_type = JS_DIGITAL;
    return *this;
}

Json &Json::operator =(const wmFLOAT val){
    //DWMDEBUG("");
    wmCHAR* pBuf = wmNEW wmCHAR[JS_FLOAT_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_FLOAT_SIZE, "%f", val);
    pBuf[JS_FLOAT_SIZE - 1] = '\0';
    trimEnd0(pBuf);
    setVal(pBuf);
    freePtr(pBuf);
    m_type = JS_DIGITAL;
    return *this;
}

Json &Json::operator =(const wmDOUBLE val){
    //DWMDEBUG("");
    wmCHAR* pBuf = wmNEW wmCHAR[JS_DOUBLE_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_DOUBLE_SIZE, "%lf", val);
    pBuf[JS_DOUBLE_SIZE - 1] = '\0';
    trimEnd0(pBuf);
    setVal(pBuf);
    freePtr(pBuf);
    m_type = JS_DIGITAL;
    return *this;
}

Json &Json::operator =(const wmBOOL val)
{
    //DWMDEBUG("");
    const wmCHAR *pStr = "false";
    if(val){
        pStr = "true";
    }
    wmCHAR* pBuf = wmNEW wmCHAR[JS_BOOL_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_BOOL_SIZE, "%s", pStr);
    pBuf[JS_BOOL_SIZE - 1] = '\0';
    setVal(pBuf);
    freePtr(pBuf);
    m_type = JS_BOOL;
    return *this;
}

Json &Json::operator =(Json& json)
{
    //DWMDEBUG("");
    FreeData();
    init(json.c_str(), wmFALSE);
    return *this;
}

wmBOOL Json::operator ==(Json& json)
{
    //DWMDEBUG("");
    if(strcmp(this->c_str(), json.c_str()) == 0){
        return wmTRUE;
    }else{
        return wmFALSE;
    }
}


wmBOOL Json::operator ==(const wmCHAR* pStr)
{
    //DWMDEBUG("");
    Json json(pStr);
    if(strcmp(this->c_str(), json.c_str()) == 0){
        return wmTRUE;
    }else{
        return wmFALSE;
    }
}

const wmCHAR* Json::c_str()
{
    //DWMDEBUG("");
    wmINT size = getStrLen() + 1;
    if(1 == size)
    {
        return "";
    }
    freePtr(m_pBuf);
    m_pBuf = wmNEW wmCHAR[size];
    JS_MEM_ALLOC_CHECK(m_pBuf, wmNULL)
    wmCHAR* ptr = m_pBuf;
    switch(m_type){
        case JS_OBJECT:
        {
            ptr += DwmSnprintf(ptr, size, "{");
            for(wmINT i = 0; i<m_length; i++){
                if(0 == i){
                    ptr += DwmSnprintf(ptr, size + (m_pBuf - ptr), "\"%s\":%s", m_ppKey[i], m_ppVal[i]->c_str());
                }else{
                    ptr += DwmSnprintf(ptr, size + (m_pBuf - ptr), ",\"%s\":%s", m_ppKey[i], m_ppVal[i]->c_str());
                }
            }
            ptr += DwmSnprintf(ptr, size + (m_pBuf - ptr), "}");
            break;
        }
        case JS_ARRAY:
        {
            wmINT cnt = getJsNodeCnt(m_pArray);
            ptr += DwmSnprintf(ptr, size, "[");
            for(wmINT i = 0; i < cnt; i++){
                if(0 == i){
                    ptr += DwmSnprintf(ptr, size + (m_pBuf - ptr), "%s", getJsNodeByIdx(m_pArray, i)->c_str());
                }else{
                    ptr += DwmSnprintf(ptr, size + (m_pBuf - ptr), ",%s", getJsNodeByIdx(m_pArray, i)->c_str());
                }
            }
            ptr += DwmSnprintf(ptr, size + (m_pBuf - ptr), "]");
            break;
        }
        case JS_DIGITAL:
        {
            js_strncpy(m_pBuf, m_pValue, size);
            break;
        }
        case JS_STRING:
        {
            DwmSnprintf(m_pBuf, size, "\"%s\"", m_pValue);
            break;
        }
        case JS_NULL:
        {
            DwmSnprintf(m_pBuf, size, "%s", m_pValue);
            break;
        }
        case JS_BOOL:
        {
            DwmSnprintf(m_pBuf, size, "%s", m_pValue);
            break;
        }
        case JS_MAX:
        {
            m_pBuf[0] = '\0';
            break;
        }
        default:
            break;
    }
    m_pBuf[size - 1] = '\0';
    return m_pBuf;
}


wmINT Json::getStrLen()
{
    //DWMDEBUG("");
    wmINT len = 0;
    switch(m_type){
        case JS_OBJECT:
        {
            for(wmINT i = 0; i<m_length; i++){
                if(i > 0){
                    len++;
                }
                len = len + 3 + strnlen(m_ppKey[i], JS_MAX_STR_LEN) + m_ppVal[i]->getStrLen();
            }
            len = len + 2;
            break;
        }
        case JS_ARRAY:
        {
            wmINT cnt = getJsNodeCnt(m_pArray);
            for(wmINT i = 0; i < cnt; i++){
                if(i > 0){
                    len++;
                }
                len = len + getJsNodeByIdx(m_pArray, i)->getStrLen();
            }
            len = len + 2;
            break;
        }
        case JS_DIGITAL:
            len  = strnlen(m_pValue, JS_MAX_STR_LEN);
            break;
        case JS_STRING:
            len  = strnlen(m_pValue, JS_MAX_STR_LEN) + 2;
            break;
        case JS_NULL:
            len  = strnlen(m_pValue, JS_MAX_STR_LEN);
            break;
        case JS_BOOL:
            len  = strnlen(m_pValue, JS_MAX_STR_LEN);
            break;
        default:
            break;
    }
    return len;
}

const wmCHAR* Json::toStr()
{
    //DWMDEBUG("");
    if(JS_STRING == m_type){
        wmINT size = strnlen(m_pValue, JS_MAX_STR_LEN) + 1;
        freePtr(m_pDecode);
        m_pDecode = wmNEW wmCHAR[size];
        JS_MEM_ALLOC_CHECK(m_pDecode, wmNULL)
        wmINT ret = decode(m_pValue, m_pDecode, size);
        if(wmTrue == ret){
            return m_pDecode;
        }
        else{
            //DWMERROR("decode m_pValue[%s] failed\n", m_pValue);
            return wmNULL;
        }
    }
    else
    {
        DWMWORNG("This is not a string\n");
        return m_pValue;
    }
}

const wmINT Json::toInt()
{
    wmINT ret = 0;
    if(wmNULL == m_pValue)
        return ret;
    ret = atoi(m_pValue);
    return ret;
}

const wmLONG Json::toLong()
{
    wmLONG ret = 0;
    if(wmNULL == m_pValue)
        return ret;
    ret = atol(m_pValue);
    return ret;
}

const wmFLOAT Json::toFloat()
{
    wmFLOAT ret = 0;
    if(wmNULL == m_pValue)
        return ret;
    ret = (wmFLOAT)atof(m_pValue);
    return ret;
}

const wmDOUBLE Json::toDouble()
{
    wmDOUBLE ret = 0;
    if(wmNULL == m_pValue)
        return ret;
    ret = atof(m_pValue);
    return ret;
}

const wmBOOL Json::toBool()
{
    if(wmNULL == m_pValue)
        return wmFALSE;
   if('t' == m_pValue[0]){
        return wmTRUE;
    }
    else{
        return wmFALSE;
    }
    return wmFALSE;
}

wmVOID Json::addElement(const wmCHAR* pKey, const wmCHAR* pVal){
    if(m_type != JS_OBJECT &&
        m_type != JS_MAX && 
        m_type != JS_NULL){
        return;
    }
    if(wmNULL == pKey){
        return;
    }
    if(JS_NULL == m_type){
        freePtr(m_pValue);
        length = 1;
        m_length = 1;
        wmCHAR** iKey = wmNEW wmCHAR* [m_length];
        JS_MEM_ALLOC_CHECK(iKey, JS_EMPTY)
        Json** iVal = wmNEW Json* [m_length];
        JS_MEM_ALLOC_CHECK(iVal, JS_EMPTY)
        wmINT size = strnlen(pKey, JS_MAX_STR_LEN) + 1;
        iKey[0] = wmNEW wmCHAR[size];
        JS_MEM_ALLOC_CHECK(iKey[0], JS_EMPTY)
        js_strncpy(iKey[0], pKey, size);
        wmCHAR* pBuf = wmNULL;
        if(pVal){
            size = strnlen(pVal, JS_MAX_STR_LEN) + 1;
            pBuf = wmNEW wmCHAR[size];
            JS_MEM_ALLOC_CHECK(pBuf, JS_EMPTY)
            DwmSnprintf(pBuf, size, "%s", pVal);
            pBuf[size - 1] = '\0';
            iVal[0] = wmNEW Json(pBuf);
            JS_MEM_ALLOC_CHECK(iVal[0], JS_EMPTY)
        }
        else{
            iVal[0] = wmNEW Json();
            JS_MEM_ALLOC_CHECK(iVal[0], JS_EMPTY)
        }
        m_ppKey = iKey;
        m_ppVal = iVal;
        m_type = JS_OBJECT;
        freePtr(pBuf);
    }
    else if(isKeyExist(pKey)){
        DWMWORNG("Key=\"%s\" already exist\n",pKey);
        (*this)[pKey].init(pVal, wmFALSE);
    }
    else{
        wmCHAR** iKey = wmNEW wmCHAR* [m_length + 1];
        JS_MEM_ALLOC_CHECK(iKey, JS_EMPTY)
        Json** iVal = wmNEW Json* [m_length + 1];
        JS_MEM_ALLOC_CHECK(iVal, JS_EMPTY)
        for(wmINT i = 0; i < m_length; i++ ){
            iKey[i] = m_ppKey[i];
            iVal[i] = m_ppVal[i];
        }
        freePtr(m_ppKey);
        freePtr(m_ppVal);
        wmINT size = strnlen(pKey, JS_MAX_STR_LEN) + 1;
        iKey[m_length] = wmNEW wmCHAR[size];
        JS_MEM_ALLOC_CHECK(iKey[m_length], JS_EMPTY)
        js_strncpy(iKey[m_length], pKey, size);
        wmCHAR* pBuf = wmNULL;
        if(pVal){
            size = strnlen(pVal, JS_MAX_STR_LEN) + 1;
            pBuf= wmNEW wmCHAR[size];
            JS_MEM_ALLOC_CHECK(pBuf, JS_EMPTY)
            DwmSnprintf(pBuf, size, "%s", pVal);
            pBuf[size - 1] = '\0';
            iVal[m_length] = wmNEW Json(pBuf);
            JS_MEM_ALLOC_CHECK(iVal[m_length], JS_EMPTY)
        }
        else{
            iVal[m_length] = wmNEW Json();
            JS_MEM_ALLOC_CHECK(iVal[m_length], JS_EMPTY)
        }
        m_ppKey = iKey;
        m_ppVal = iVal;
        m_length++;
        length = m_length;
        m_type = JS_OBJECT;
        freePtr(pBuf);
    }

}

Json &Json::addProperty(const wmCHAR* pKey, const wmCHAR* pVal, const wmBOOL bEncode)
{
    if(wmNULL == pKey)
    {
        return *this;
    }
    wmCHAR* pBuf = wmNULL;
    if(pVal){
        wmINT size = -1;
        if(bEncode){
            size = encodeCheck(pVal);
        }
        if(-1 == size){
            size = strnlen(pVal, JS_MAX_STR_LEN) + 3;
            pBuf = wmNEW wmCHAR[size];
            JS_MEM_ALLOC_CHECK(pBuf, *this)
            DwmSnprintf(pBuf, size, "\"%s\"", pVal);
            pBuf[size - 1] = '\0';
        }
        else{
            wmCHAR* pTmpBuf = wmNEW wmCHAR[size];
            JS_MEM_ALLOC_CHECK(pTmpBuf, *this)
            wmINT ret = encode(pVal, pTmpBuf, size, wmFALSE);
            if(wmFalse == ret){
                freePtr(pTmpBuf);
                //DWMERROR("encode pVal[%s] failed\n", pVal);
                return *this;
            }
            pBuf = wmNEW wmCHAR[size + 2];
            JS_MEM_ALLOC_CHECK(pBuf, *this)
            DwmSnprintf(pBuf, size + 2, "\"%s\"", pTmpBuf);
            pBuf[size + 1] = '\0';
            freePtr(pTmpBuf);
        }
    }
    addElement(pKey, pBuf);
    freePtr(pBuf);
    return *this;
}

Json &Json::addProperty(const wmCHAR* pKey, const wmINT val)
{
    if(wmNULL == pKey)
    {
        return *this;
    }
    wmCHAR* pBuf = wmNEW wmCHAR[JS_INT_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_INT_SIZE, "%d", val);
    pBuf[JS_INT_SIZE - 1] = '\0';
    addElement(pKey, pBuf);
    freePtr(pBuf);
    return *this;
}

Json &Json::addProperty(const wmCHAR* pKey, const wmULONG val)
{
    //DWMDEBUG("");
    if(wmNULL == pKey)
    {
        //DWMERROR("pKey is JS_NULL_PTR\n");
        return *this;
    }
    wmCHAR* pBuf = wmNEW wmCHAR[JS_UTF16_STR_MAX_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_UTF16_STR_MAX_SIZE, "%ld", val);
    pBuf[JS_UTF16_STR_MAX_SIZE - 1] = '\0';
    addElement(pKey, pBuf);
    freePtr(pBuf);
    return *this;
}

Json &Json::addProperty(const wmCHAR* pKey, const wmBOOL val)
{
    //DWMDEBUG("");
    if(wmNULL == pKey)
    {
        //DWMERROR("pKey is JS_NULL_PTR\n");
        return *this;
    }
    const wmCHAR *pStr = "false";
    if(val){
        pStr = "true";
    }
    wmCHAR* pBuf = wmNEW wmCHAR[JS_BOOL_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_BOOL_SIZE, "%s", pStr);
    pBuf[JS_BOOL_SIZE - 1] = '\0';
    addElement(pKey, pBuf);
    freePtr(pBuf);
    return *this;
}

Json &Json::addProperty(const wmCHAR* pKey, Json& json)
{
    //DWMDEBUG("");
    if(wmNULL == pKey)
    {
        //DWMERROR("pKey is JS_NULL_PTR\n");
        return *this;
    }
    addElement(pKey, json.c_str());
    return *this;
}

Json &Json::delProperty(const wmCHAR* pKey)
{
    //DWMDEBUG("");
    if(wmNULL == pKey)
    {
        //DWMERROR("pKey is JS_NULL_PTR\n");
        return *this;
    }
    wmINT index = getKeyIndex(pKey);
    if(index < 0){
        DWMWORNG("object didn't have the property %s\n", pKey);
    }
    else{
        for(wmINT i = index; i < m_length; i++){
            if(i == index){
                if(m_ppKey){
                    freePtr(m_ppKey[i]);
                }
                if(m_ppVal){
                    freePtr(m_ppVal[i]);
                }
            }
            else{
                m_ppKey[i - 1] = m_ppKey[i];
                m_ppVal[i - 1] = m_ppVal[i];
            }
        }
        m_length--;
        length = m_length;
        m_ppKey[m_length] = wmNULL;
        m_ppVal[m_length] = wmNULL;
        if(0 == m_length){
            init(wmNull, wmFALSE);
        }
    }
    return *this;
}

wmVOID Json::addElement(const wmCHAR* pVal){
    //DWMDEBUG("");
    if(JS_NULL == m_type){
        freePtr(m_pValue);
        length = 1;
        JS_NODELIST_NEW(m_pArray, JS_EMPTY)
        Json* pJson = wmNEW Json();
        JS_MEM_ALLOC_CHECK(pJson, JS_EMPTY)
        pJson->init(pVal, wmFALSE);
        addJsNode(m_pArray, pJson);
        m_type = JS_ARRAY;
    }
    else if(JS_ARRAY == m_type){
        Json* pJson = wmNEW Json();
        JS_MEM_ALLOC_CHECK(pJson, JS_EMPTY)
        pJson->init(pVal, wmFALSE);
        length = addJsNode(m_pArray, pJson);
    }
    else{
        DWMWORNG("This is not a type of array!!!\n");
    }
}

Json &Json::addVal(const wmCHAR* pVal, const wmBOOL bEncode)
{
    //DWMDEBUG("");
    wmCHAR* pBuf = wmNULL;
    if(pVal){
        wmINT size = -1;
        if(bEncode) {
            size = encodeCheck(pVal);
        }
        if(size > 0){
            wmCHAR* pTmpBuf = wmNEW wmCHAR[size];
            JS_MEM_ALLOC_CHECK(pTmpBuf, *this)
            wmINT ret = encode(pVal, pTmpBuf, size, wmFALSE);
            if(wmFalse == ret){
                freePtr(pTmpBuf);
                //DWMERROR("encode pVal[%s] failed\n", pVal);
                return *this;
            }
            pBuf = wmNEW wmCHAR[size + 2];
            JS_MEM_ALLOC_CHECK(pBuf, *this)
            DwmSnprintf(pBuf, size + 2, "\"%s\"", pTmpBuf);
            pBuf[size + 1] = '\0';
            freePtr(pTmpBuf);
        }
        else{
            size = strnlen(pVal, JS_MAX_STR_LEN) + 3;
            pBuf = wmNEW wmCHAR[size];
            JS_MEM_ALLOC_CHECK(pBuf, *this)
            DwmSnprintf(pBuf, size, "\"%s\"", pVal);
            pBuf[size - 1] = '\0';
        }
    }
    else{
        pBuf = wmNEW wmCHAR[JS_NULL_SIZE];
        JS_MEM_ALLOC_CHECK(pBuf, *this)
        js_strncpy(pBuf, wmNull, JS_NULL_SIZE);
    }
    addElement(pBuf);
    freePtr(pBuf);
    return *this;
}

Json &Json::addVal(const wmINT val)
{
    //DWMDEBUG("");
    wmCHAR* pBuf = wmNEW wmCHAR[JS_INT_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_INT_SIZE, "%d", val);
    pBuf[JS_INT_SIZE - 1] = '\0';
    addElement(pBuf);
    freePtr(pBuf);
    return *this;
}

Json &Json::addVal(const wmUINT val)
{
    //DWMDEBUG("");
    wmCHAR* pBuf = wmNEW wmCHAR[JS_UINT_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_UINT_SIZE, "%d", val);
    pBuf[JS_UINT_SIZE - 1] = '\0';
    addElement(pBuf);
    freePtr(pBuf);
    return *this;
}

Json &Json::addVal(const wmLONG val){
    //DWMDEBUG("");
    wmCHAR* pBuf = wmNEW wmCHAR[JS_LINT_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_LINT_SIZE, "%ld", val);
    pBuf[JS_LINT_SIZE - 1] = '\0';
    addElement(pBuf);
    freePtr(pBuf);
    return *this;
}

Json &Json::addVal(const wmFLOAT val){
    //DWMDEBUG("");
    wmCHAR* pBuf = wmNEW wmCHAR[JS_FLOAT_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_FLOAT_SIZE, "%f", val);
    pBuf[JS_FLOAT_SIZE - 1] = '\0';
    trimEnd0(pBuf);
    addElement(pBuf);
    freePtr(pBuf);
    return *this;
}

Json &Json::addVal(const wmDOUBLE val){
    //DWMDEBUG("");
    wmCHAR* pBuf = wmNEW wmCHAR[JS_DOUBLE_SIZE];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    DwmSnprintf(pBuf, JS_DOUBLE_SIZE, "%lf", val);
    pBuf[JS_DOUBLE_SIZE - 1] = '\0';
    trimEnd0(pBuf);
    addElement(pBuf);
    freePtr(pBuf);
    return *this;
}

Json &Json::addVal(const wmBOOL val){
    //DWMDEBUG("");
    const wmCHAR* pBuf = "false";
    if(val){
        pBuf = "true";
    }
    addElement(pBuf);
    return *this;
}

Json &Json::addVal(Json& json){
    //DWMDEBUG("");
    const wmCHAR* pBuf = json.c_str();
    addElement(pBuf);
    return *this;
}

Json &Json::addVal(const wmUSHORT* pVal, const wmBOOL bEncode){
    //DWMDEBUG("");
    wmINT size = js_wstrnlen(pVal, JS_UTF16_STR_MAX_SIZE) + 1;
    wmCHAR* pBuf = wmNEW wmCHAR[JS_UTF162UTF8_SIZE(size)];
    JS_MEM_ALLOC_CHECK(pBuf, *this)
    Utf16ToUtf8(pBuf, pVal, size);
    addVal(pBuf, bEncode);
    freePtr(pBuf);
    return *this;
}

Json &Json::addValNoRepeat(const wmCHAR* pVal, const wmBOOL bEncode){
    //DWMDEBUG("");
    if(!isStrExist(pVal)){
        return addVal(pVal, bEncode);
    }
    return *this;
}

Json &Json::addValNoRepeat(const wmUSHORT* pVal, const wmBOOL bEncode){
    //DWMDEBUG("");
    if(!isStrExist(pVal)){
        return addVal(pVal, bEncode);
    }
    return *this;
}

Json &Json::delVal(const wmINT &index){
    //DWMDEBUG("");
    if(m_type != JS_ARRAY){
        DWMWORNG("The type of this object is not an array!!!\n");
    }
    else if(getJsNodeCnt(m_pArray) <= index || index < 0){
        DWMWORNG("The index is you want to wmDELETE is ERR!!!\n");
    }
    else{
        length = delJsNodeByIdx(m_pArray, index);
        if(0 == length){
            init(wmNull, wmFALSE);
        }
    }
    return *this;
}
    
wmBOOL Json::isKeyExist(const wmCHAR* pKey){
    //DWMDEBUG("");
    if(wmNULL == pKey)
    {
        //DWMERROR("pKey is JS_NULL_PTR\n");
        return wmFALSE;
    }
    if(getKeyIndex(pKey) >= 0){
        return wmTRUE;
    }
    else{
        return wmFALSE;
    }
}

wmINT Json::getKeyIndex(const wmCHAR* pKey)
{
    //DWMDEBUG("");
    if(wmNULL == pKey)
    {
        //DWMERROR("pKey is JS_NULL_PTR\n");
        return -1;
    }
    if(m_type != JS_OBJECT){
        return -1;
    }
    for(wmINT i = 0; i<m_length; i++){
        if(strcmp(m_ppKey[i], pKey) == 0){
            return i;
        }
    }
    return -1;
}

wmBOOL Json::isStrExist(const wmCHAR* pStr){
    //DWMDEBUG("");
    if(JS_ARRAY == m_type){
        Json obj;
        obj = pStr;
        if(getObjIndex(obj) >= 0){
            return wmTRUE;
        }
    }
    return wmFALSE;
}

wmBOOL Json::isStrExist(const wmUSHORT* pStr){
    //DWMDEBUG("");
    if(JS_ARRAY == m_type){
        Json obj;
        obj = pStr;
        if(getObjIndex(obj) >= 0){
            return wmTRUE;
        }
    }
    return wmFALSE;
}


wmINT Json::getObjIndex(Json& obj)
{
    //DWMDEBUG("");
    if(m_type != JS_ARRAY){
        return -1;
    }
    wmINT cnt = getJsNodeCnt(m_pArray);
    for(wmINT i = 0; i < cnt; i++){
        if(strcmp(getJsNodeByIdx(m_pArray, i)->c_str(), obj.c_str()) == 0){
            return i;
        }
    }
    return -1;
}

wmVOID Json::freePtr(Json* &p){
    //DWMDEBUG("");
    if(p){
        wmDELETE p;
        p = wmNULL;
    }
}

wmVOID Json::freePtr(wmCHAR* &p){
    //DWMDEBUG("");
    if(p){
        wmDELETE []p;
        p = wmNULL;
    }
}

wmVOID Json::freePtr(wmCHAR** &p){
    //DWMDEBUG("");
    if(p){
        wmDELETE p;
        p = wmNULL;
    }
}

wmVOID Json::freePtr(wmUSHORT* &p){
    //DWMDEBUG("");
    if(p){
        wmDELETE []p;
        p = wmNULL;
    }
}

wmVOID Json::freePtr(Json** &p){
    //DWMDEBUG("");
    if(p){
        wmDELETE p;
        p = wmNULL;
    }
}

const wmCHAR* Json::encodeChar(const wmCHAR &ch)
{
    //DWMDEBUG("");
    switch(ch){
        case '"':
            return "\\\"";
        case '\\':
            return "\\\\";
        case '\n':
            return "\\n";
        case '\t':
            return "\\t";
        case '\r':
            return "\\r";
        default:
            return wmNULL;
    }
}

wmINT Json::encodeCheck(const wmCHAR* pSrc)
{
    //DWMDEBUG("");
    wmINT len = strnlen(pSrc, JS_MAX_STR_LEN);
    wmINT size = 0;
    for(wmINT i = 0; i <= len; i++){
        switch(pSrc[i]){
            case '"':
            case '\\':
            case '\n':
            case '\t':
            case '\r':
                size += 2;
                break;
            default:
                size += 1;
                break;
        }
    }
    if(size == len + 1){
        return -1;
    }
    else{
        return size;
    }
}

wmINT Json::encode(const wmCHAR* pSrc, wmCHAR* pObj, const wmINT & size, const wmBOOL bCheck)
{
    //DWMDEBUG("");
    if(bCheck) {
        wmINT eSize = encodeCheck(pSrc);
        if(eSize > size){
            //DWMERROR("the param size[%d] is less than need eSize[%d]\n", size, eSize);
            return wmFalse;
        }
    }
    wmINT len = strnlen(pSrc, JS_MAX_STR_LEN);
    if((len + 1) > size){
        //DWMERROR("the param size[%d] is less than the length of pSrc[%s]\n", size, pSrc);
        return wmFalse;
    }
    else{
        wmINT j = 0;
        for(wmINT i = 0; i <= len; i++){
            const wmCHAR* pChar = encodeChar(pSrc[i]);
            if(pChar){
                if(j + 1 > size - 1){
                    //DWMERROR("the param size[%d] is less than need size\n", size);
                    return wmFalse;
                }
                pObj[j] = pChar[0];
                pObj[j + 1] = pChar[1];
                j += 2;
            }
            else{
                if(j > size - 1){
                    //DWMERROR("the param size[%d] is less than need size\n", size);
                    return wmFalse;
                }
                pObj[j] = pSrc[i];
                j++;
            }
        }
    }
    return wmTrue;
}

wmINT Json::decode(const wmCHAR* pSrc, wmCHAR* pObj, const wmINT & size)
{
    //DWMDEBUG("");
    wmINT len = strnlen(pSrc, JS_MAX_STR_LEN);
    wmINT j = 0;
    for(wmINT i = 0; i <= len; i++){
        if(j > size - 1){
            //DWMERROR("size[%d] is not enough for decode pSrc[%s]\n", size, pSrc);
            return wmFalse;
        }
        switch(pSrc[i]){
            case '\\':
                switch(pSrc[i+1]){
                    case '\0':
                        //DWMERROR("pSrc[%s] is not a string which was encoded\n", pSrc);
                        return wmFalse;
                    case 'n':
                        pObj[j] = '\n';
                        break;
                    case 't':
                        pObj[j] = '\t';
                        break;
                    case 'r':
                        pObj[j] = '\r';
                        break;
                    case '"':
                        pObj[j] = '"';
                        break;
                    case '\\':
                        pObj[j] = '\\';
                        break;
                }
                i++;
                break;
            default :
                pObj[j] = pSrc[i];
                break;
        }
        j++;
    }
    return wmTrue;
}

wmVOID Json::trimEnd0(wmCHAR* pVal)
{
    //DWMDEBUG("");
    wmINT len = strnlen(pVal, JS_DOUBLE_SIZE);
    wmINT pointPostion = 0;
    for(wmINT i = 0; i < len; i++){
        if('.' == pVal[i]){
            pointPostion = i;
        }
    }
    if(0 == pointPostion){
        return;
    }
    for(wmINT i = len - 1; i > pointPostion; i--){
        if('0' == pVal[i]){
            pVal[i] = '\0';
            if(i == pointPostion + 1){
                pVal[pointPostion] = '\0';
            }
        }
        else{
            break;
        }
    }
}

