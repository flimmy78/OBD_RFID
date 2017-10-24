/**************************************************************************************
 * json.h
 **************************************************************************************/

#ifndef _JSON_H__
#define _JSON_H__

#include "baseDef.h"

enum {
    JS_ARRAY = 0,
    JS_OBJECT,
    JS_STRING,
    JS_DIGITAL,
    JS_NULL,
    JS_BOOL,
    JS_MAX
};

class Json {
public:
    Json();
    Json(const wmCHAR* pStr);
    Json(const wmUSHORT* pStr);
    Json(const wmCHAR* pStr, const wmINT & len);
    Json(Json& json);
    ~Json();
    const wmCHAR* c_str();
    const wmINT toInt();
	const wmLONG toLong();
    const wmFLOAT toFloat();
    const wmDOUBLE toDouble();
    const wmCHAR* toStr();
    const wmBOOL toBool();
    wmINT setStr(const wmCHAR* pVal, const wmBOOL bEncode = wmFALSE);
    wmINT setStr(const wmINT val);
    wmINT setStr(const wmUINT val);
    wmINT setStr(const wmLONG val);
    wmINT setStr(const wmFLOAT val);
    wmINT setStr(const wmDOUBLE val);
    wmINT setStr(const wmBOOL val);
    Json &addProperty(const wmCHAR* pKey, const wmCHAR* pVal, const wmBOOL bEncode = wmFALSE);
    Json &addProperty(const wmCHAR* pKey, const wmINT val);
	Json &addProperty(const wmCHAR* pKey, const wmULONG val);
    Json &addProperty(const wmCHAR* pKey, const wmBOOL val);
    Json &addProperty(const wmCHAR* pKey, Json& json);
    Json &delProperty(const wmCHAR* pKey);
    Json &addVal(const wmCHAR* pVal, const wmBOOL bEncode = wmFALSE);
    Json &addVal(const wmINT val);
    Json &addVal(const wmUINT val);
    Json &addVal(const wmLONG val);
    Json &addVal(const wmFLOAT val);
    Json &addVal(const wmDOUBLE val);
    Json &addVal(const wmBOOL val);
    Json &addVal(Json& json);
    Json &addVal(const wmUSHORT* pVal, const wmBOOL bEncode = wmFALSE);
    Json &addValNoRepeat(const wmCHAR* pVal, const wmBOOL bEncode = wmFALSE);
    Json &addValNoRepeat(const wmUSHORT* pVal, const wmBOOL bEncode = wmFALSE);
    Json &delVal(const wmINT & index);
    Json &operator [](const wmCHAR* pKey);
    Json &operator [](const wmINT & index);
    Json &operator =(const wmCHAR* pVal);
    Json &operator =(const wmUSHORT* pVal);
    Json &operator =(const wmINT val);
    Json &operator =(const wmUINT val);
    Json &operator =(const wmLONG val);
    Json &operator =(const wmFLOAT val);
    Json &operator =(const wmDOUBLE val);
    Json &operator =(const wmBOOL val);
    Json &operator =(Json& json);
    wmBOOL operator ==(Json& json);
    wmBOOL operator ==(const wmCHAR* pStr);
    wmINT length;
    wmVOID init(const wmCHAR* pStr, const wmBOOL bCheck = wmTRUE);
    wmVOID init(const wmCHAR* pStr, const wmINT & len, const wmBOOL bCheck = wmTRUE);
    wmBOOL isKeyExist(const wmCHAR* pKey);
    wmBOOL isStrExist(const wmCHAR* pStr);
    wmBOOL isStrExist(const wmUSHORT* pStr);
    wmINT getType();
    /**
     * @brief check string if need encode
     * @param pSrc the source string which need to check
     * @return -1:no need encode, > 0:need encode buf size
     */
    static wmINT encodeCheck(const wmCHAR* pSrc);
    /**
     * @brief encode a string to JS_STRING
     * @param pSrc the source string buf
     * @param pObj the obj string buf
     * @param size the buf size of pObj
     * @param bCheck if need encodeCheck inside 
     * @return jsERROR:encode failed, jsOK:encode success
     */
    static wmINT encode(const wmCHAR* pSrc, wmCHAR* pObj, const wmINT & size, const wmBOOL bCheck = wmTRUE);
    /**
     * @brief decode a JS_STRING string to string
     * @param pSrc the source string buf
     * @param pObj the obj string buf
     * @param size the buf size of pObj
     * @return jsERROR:decode failed, jsOK:decode success
     */
    static wmINT decode(const wmCHAR* pSrc, wmCHAR* pObj, const wmINT & size);

    static wmVOID trimEnd0(wmCHAR* pVal);
    static wmINT js_strncpy(wmCHAR* pObj, const wmCHAR* pSrc, wmUINT n);
    static wmINT js_wstrnlen(const wmUSHORT* pSrc, wmUINT n);
    static wmINT js_wstrnlen(const wmCHAR* pSrc, wmUINT n);
    static wmBOOL Utf8ToUtf16(wmUSHORT* pObj, const wmCHAR* pSrc, wmUINT n);
    static wmBOOL Utf16ToUtf8(wmCHAR* pObj, const wmUSHORT* pSrc, wmUINT n);
    
private:
    static wmVOID freePtr(Json* &p);
    static wmVOID freePtr(wmCHAR* &p);
    static wmVOID freePtr(wmCHAR** &p);
    static wmVOID freePtr(wmUSHORT* &p);
    static wmVOID freePtr(Json** &p);
    wmINT getKeyIndex(const wmCHAR* pKey);
    wmINT getObjIndex(Json& obj);
    wmINT getEndChPos(wmCHAR* pStr);
    wmVOID parse();
    wmVOID InitData();
    wmVOID FreeData();
    wmVOID parseArray();
    wmVOID parseObject();
    wmVOID parseValue();
    wmVOID init();
    wmINT getStrLen();
    wmVOID setVal(const wmCHAR* pVal);
    wmVOID addElement(const wmCHAR* pKey, const wmCHAR* pVal);
    wmVOID addElement(const wmCHAR* pVal);
    wmVOID trim();
    static const wmCHAR* encodeChar(const wmCHAR & ch);
    static Json s_NullJson;
    wmCHAR* m_pStr;
    wmCHAR** m_ppKey;
    Json** m_ppVal;
    wmINT m_type;
    wmVOID* m_pArray;
    wmCHAR* m_pBuf;
    wmCHAR* m_pValue;
    wmCHAR* m_pDecode;
    wmINT m_length;
};

#endif

