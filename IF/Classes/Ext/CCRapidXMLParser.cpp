//
//  CCRapidXMLParser.cpp
//  clash of kings
//
//  Created by zhaohf on 14-5-9.
//
//

#include "cocos2d.h"
#include "CCRapidXMLParser.h"

USING_NS_CC;

CCRapidXMLParser::CCRapidXMLParser()
:m_isJumpHeadData(false),m_pDictGroup(NULL),m_pDictItem(NULL)
{
    m_pDictItem = new CCDictionary();
    m_pDictGroup = new CCDictionary();
}

CCRapidXMLParser::~CCRapidXMLParser()
{
    CC_SAFE_RELEASE_NULL(this->m_pDictItem);
    CC_SAFE_RELEASE_NULL(this->m_pDictGroup);
}

CCRapidXMLParser* CCRapidXMLParser::parseWithFile(const char *xmlFileName,const char* localFileName)
{
    CCRapidXMLParser *pXMLParser = new CCRapidXMLParser();
    if(!pXMLParser->initWithFile(xmlFileName) ) {
        if(!pXMLParser->initWithFile(localFileName) )
        {
            CC_SAFE_RELEASE_NULL(pXMLParser);
        }
    }
    return pXMLParser;
}

bool CCRapidXMLParser::initWithFile(const char *xmlFileName)
{
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(xmlFileName);
    CCLog("XMLPath====>%s",fullPath.c_str());
    ssize_t size = 0;
    char* pBuffer = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "r", &size);
    if (pBuffer != NULL && size > 0)
    {
        xml_document<> doc;
        std::string content(pBuffer, size);
        CC_SAFE_DELETE_ARRAY(pBuffer);
        doc.parse<0>(&content[0]);
        xml_node<>* node = doc.first_node()->first_node();
        while (node)
        {
            std::string nodeName = node->name();
            if (nodeName == "Group")
            {
                startElement(node);
                node = node->next_sibling();
            }
        }
    }
    return true;
}

//  elementNode处理
void CCRapidXMLParser::startElement(xml_node<>* dictNode)
{
    //  属性值处理
    std::string elementName = dictNode->name();
    //  如果是分组节点，创建组数据字典；
    std::string strGroupID= "";
    CCDictionary * pDictGroupItem = CCDictionary::create();
    if(strcmp(elementName.c_str(),"Group") == 0)
    {
        xml_attribute<> * attribute = dictNode->first_attribute();
        std::string value = attribute->value();
        m_groupName = value;
        m_pDictGroup->setObject(pDictGroupItem, value.c_str());
//        CCLOG("crate Group %s",value.c_str());
    }
    
    xml_node<>* node = dictNode->first_node();
    //  处理节点属性值；
    if(node)
    {
        
        while (node)
        {
            xml_attribute<> * attribute = node->first_attribute();
            CCDictionary * pDictTmp = CCDictionary::create();
            while (attribute) {
                std::string key = attribute->name();
                std::string value = attribute->value();
                
                pDictTmp->setObject(CCString::create(value), key);
                attribute = attribute->next_attribute();
//                CCLOG("-----key:%s, value:%s",key.c_str(),value.c_str());
                
            }
            
            //  按照id建立索引表
            CCString* pID = (CCString*)pDictTmp->objectForKey("id");
            if (!pID)
            {
                node = node->next_sibling();
                continue;
            }
            m_pDictItem->setObject(pDictTmp , pID->getCString());
            //  按照组id建立索引表
            pDictGroupItem->setObject(pDictTmp , pID->getCString());
            
            node = node->next_sibling();
            
        }

    }
    
}

CCDictionary * CCRapidXMLParser::getObjectByKey(std::string key)
{
    //  按照id建立索引表
    auto ret = (CCDictionary *)m_pDictItem->objectForKey(key);
    if (ret == NULL) {
        ret = CCDictionary::create();
    }
    return ret;
}

CCDictionary * CCRapidXMLParser::getGroupByKey(std::string key)
{
    //  按照GroupID建立索引表
    auto ret = (CCDictionary *)m_pDictGroup->objectForKey(key);
    if (ret == NULL) {
        ret = CCDictionary::create();
    }
    return ret;
}
