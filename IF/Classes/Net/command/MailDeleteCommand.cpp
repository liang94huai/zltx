//
//  MailDeleteCommand.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-12.
//
//

#include "MailDeleteCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"

bool MailDeleteCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DELETE_COMMAND) != 0)
        return false;
 //   CCDictionary* params = _dict(dict->objectForKey("params"));
 //   CCArray* array = dynamic_cast<CCArray*>(params->objectForKey("result"));
//    if(array!=NULL){
 //       int num = array->count();
//        for (int i=0; i<num; i++) {
//            auto dic = _dict(array->objectAtIndex(i));
//            MailController::getInstance()->addMail(dic);
//        }
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SAVE_LIST_CHANGE);
 //   }
    
    return true;
}
bool MailDialogReadCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DIALOG_READ_COMMAND) != 0)
        return false;
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        return true;
    }
//        CCDictElement * pElement;
//        CCDICT_FOREACH(params, pElement)
//    
//        {
//    
//            const char * key = pElement->getStrKey();
//    
//            CCString * value = (CCString *)pElement->getObject();
//    
//            CCLog(key);
//    
//            CCLog(value ->getCString());
//            
//        }
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(MailController::getInstance()->getIsNewMailListEnable())
    {
        if(m_types=="" && m_type>=0)
            ChatServiceCocos2dx::setMutiMailStatusByType(m_type,1);
        else if(m_types!="")
        {
            vector<std::string> vector;
            CCCommonUtils::splitString(m_types, ",", vector);
            CCLOGFUNCF("vector.size() :%d",vector.size());
            bool hasMessageType = false;
            for (int i = 0; i<vector.size(); i++)
            {
                CCLOGFUNCF("vector.at(i) :%s",vector.at(i).c_str());
                int type = atoi(vector.at(i).c_str());
                if(type>=0)
                {
                    if((type == 0 || type == 1 || type ==20))
                    {
                        if(!hasMessageType)
                        {
                            hasMessageType = true;
                            ChatServiceCocos2dx::setMutiMailStatusByType(type,1);
                        }
                    }
                    else
                    {
                        ChatServiceCocos2dx::setMutiMailStatusByType(type,1);
                    }
                }
            }
        }
    }
    
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    int readStatus =params ->valueForKey("success") ->intValue();
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
        if  (readStatus == 1){
            if (m_types == "" && m_type > 0){
                ChatServiceCocos2dx::settingUnReadStatusToreadWith_mailType(m_type,true);
            }else if (m_types != "" ){
                vector<std::string> vector;
                CCCommonUtils::splitString(m_types, ",", vector);
                CCLOGFUNCF("vector.size() :%d",vector.size());
                for (int i = 0; i<vector.size(); i++)
                {
                    CCLOGFUNCF("vector.at(i) :%s",vector.at(i).c_str());
                    int type = atoi(vector.at(i).c_str());
                    if(type>=0){
                         ChatServiceCocos2dx::settingUnReadStatusToreadWith_mailType(type,true);
                    }
                        
                }

            }else if (m_type == -1){
                ChatServiceCocos2dx::settingUnReadStatusToReadWIth_ChatUserUid( cs_fromUser,true);
            }
        }

    }
#endif
    return true;
}

bool MailDialogSaveCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DIALOG_SAVE_COMMAND) != 0)
        return false;
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        return true;
    }
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(MailController::getInstance()->getIsNewMailListEnable())
    {
        if(m_flag==0)
            ChatServiceCocos2dx::setMutiMailStatusByType(m_type,2,true);
        else
            ChatServiceCocos2dx::setMutiMailStatusByType(m_type,2);
    }
#endif
    return true;
}
bool MailDialogDeleteCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DIALOG_DELETE_COMMAND) != 0)
        return false;
    if (MailController::getInstance()->getIsNewMailListEnable()) {
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::deleteMail(m_uid ,CHANNEL_TYPE_OFFICIAL,m_type);
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        ChatServiceCocos2dx::deleteMail(m_uid ,CHANNEL_TYPE_OFFICIAL,m_type);
#endif
    }
    
    return true;
}