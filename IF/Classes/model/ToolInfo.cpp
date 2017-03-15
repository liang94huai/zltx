//
//  ToolInfo.cpp
//  IF
//
//  Created by fubin on 14-2-27.
//
//

#include "ToolInfo.h"
#include "CCCommonUtils.h"
#include "ToolController.h"

ToolInfo::ToolInfo(CCDictionary* dict)
{//<ItemSpec id="200330" name="104126" icon="ico200330" type="3" type2="3" lv="1" page="3" use="1" order_num="1" price="40"/>
    if (dict->objectForKey("id")) {
         this->itemId = dict->valueForKey("id")->intValue();
    } else {
        this->itemId = 0;
    }
    if (dict->objectForKey("name")) {
        this->name = dict->valueForKey("name")->getCString();
    } else {
        this->name = "";
    }
    if (dict->objectForKey("icon")) {
        this->icon = dict->valueForKey("icon")->getCString();
    } else {
        this->icon = "";
    }
    if (dict->objectForKey("description")) {
        this->des = dict->valueForKey("description")->getCString();
    } else {
        this->des = "";
    }
    if (dict->objectForKey("page")) {
        this->page = dict->valueForKey("page")->intValue();
    } else {
        this->page = 0;
    }
    if (dict->objectForKey("use")) {
        this->use = dict->valueForKey("use")->intValue();
    } else {
        this->use = 0;
    }
    if (dict->objectForKey("order_num")) {
        this->orderNum = dict->valueForKey("order_num")->intValue();
    } else {
        this->orderNum = 0;
    }
    if (dict->objectForKey("color")) {
        this->color = dict->valueForKey("color")->intValue();
    } else {
        this->color = 0;
    }
    if (dict->objectForKey("type")) {
        this->type = dict->valueForKey("type")->intValue();
    } else {
        this->type = 0;
    }
    if (dict->objectForKey("type2")) {
        this->type2 = dict->valueForKey("type2")->intValue();
    } else {
        this->type2 = 0;
    }
    if (dict->objectForKey("price")) {
        this->price = dict->valueForKey("price")->intValue();
    } else {
        this->price = 0;
    }
    if (dict->objectForKey("price_all")) {
        this->price_all = dict->valueForKey("price_all")->intValue();
    } else {
        this->price_all = 0;
    }
    if (dict->objectForKey("onsale")) {
        this->onsale = dict->valueForKey("onsale")->intValue();
    } else {
        this->onsale = 0;
    }
    if (dict->objectForKey("lv")) {
        this->limitLv = dict->valueForKey("lv")->intValue();
    } else {
        this->limitLv = 0;
    }
    if (dict->objectForKey("sales")) {
        this->sales = dict->valueForKey("sales")->getCString();
    } else {
        this->sales = "";
    }
    if (dict->objectForKey("lv_limit")) {
        this->limitUseLv = dict->valueForKey("lv_limit")->intValue();
    } else {
        this->limitUseLv = 0;
    }
    if (dict->objectForKey("useall")) {
        this->useall = dict->valueForKey("useall")->intValue();
    } else {
        this->useall = 0;
    }
    /*
    this->itemId = dict->valueForKey("id")->intValue();
    this->name = dict->valueForKey("name")->getCString();
    this->icon = dict->valueForKey("icon")->getCString();
    this->des = dict->valueForKey("description")->getCString();
    this->page = dict->valueForKey("page")->intValue();
    this->use = dict->valueForKey("use")->intValue();
    this->orderNum = dict->valueForKey("order_num")->intValue();
    this->color = dict->valueForKey("color")->intValue();
    this->type = dict->valueForKey("type")->intValue();//5:礼包
    this->type2 = dict->valueForKey("type2")->intValue();
    this->price = dict->valueForKey("price")->intValue();
    this->price_all = dict->valueForKey("price_all")->intValue();
    this->onsale = dict->valueForKey("onsale")->intValue();
    this->limitLv = dict->valueForKey("lv")->intValue();
    this->sales = dict->valueForKey("sales")->getCString();
    this->limitUseLv = dict->valueForKey("lv_limit")->intValue();
    this->useall = dict->valueForKey("useall")->intValue();
     */
 
    if (dict->objectForKey("price_hot") ==nullptr) {
        this->price_hot = 0;
    }
    else {
        this->price_hot = dict->valueForKey("price_hot")->intValue();
    }
    
    if (dict->objectForKey("para1") ==nullptr) {
        this->para1 = "";
    }
    else {
        this->para1 = dict->valueForKey("para1")->getCString();
    }
    
    if (dict->objectForKey("para2") ==nullptr) {
        this->para2 = "";
    }
    else {
        this->para2 = dict->valueForKey("para2")->getCString();
    }
    
    if (dict->objectForKey("para3") ==nullptr) {
        this->para3 = "";
    }
    else {
        this->para3 = dict->valueForKey("para3")->getCString();
    }
    
    if (dict->objectForKey("para4") ==nullptr) {
        this->para4 = "";
    }
    else {
        this->para4 = dict->valueForKey("para4")->getCString();
    }
    
    
    if(dict->objectForKey("is_show")){
        this->is_show = dict->valueForKey("is_show")->intValue();
    }else{
        this->is_show = 0;
    }
    
    if(dict->objectForKey("is_gift")){
        if(dict->valueForKey("is_gift")->intValue() == 1){
            this->is_gift = false;
        }else{
            this->is_gift = true;
        }
    }else{
        this->is_gift = true;
    }
    
    if(dict->objectForKey("pages")){
        this->pages = dict->valueForKey("pages")->intValue();
    }else{
        this->pages = 0;
    }
    if(dict->objectForKey("store_order")){
        this->store_order = dict->valueForKey("store_order")->intValue();
    }
    if(dict->objectForKey("alliance_order")){
        this->alliance_order = dict->valueForKey("alliance_order")->intValue();
    }
    if(dict->objectForKey("version")){
        this->version = dict->valueForKey("version")->getCString();
    }
    if(dict->objectForKey("pagehot")){
        this->pagehot = dict->valueForKey("pagehot")->intValue();
    }else{
        this->pagehot = 0;
    }
    
    crt_order = -1;
    crt_cd = -1;
    crt_gold = -1;
    if (dict->objectForKey("make")) {
        string makeMsg = dict->valueForKey("make")->getCString();
        vector<string> makeVec;
        CCCommonUtils::splitString(makeMsg, "|", makeVec);
        if (makeVec.size() >= 3) {
            crt_order = atoi(makeVec[0].c_str());
            crt_cd = atoi(makeVec[1].c_str());
            crt_gold = atoi(makeVec[2].c_str());
        }
    }
    
    this->cnt = 0;
    this->allianceCnt = 0;
    this->buyType = 0;
    this->uuid = "";
    ishot = false;
    isShowHot = false;
    if(dict->objectForKey("store_tab")){
        this->hotPara = dict->valueForKey("store_tab")->floatValue();
    }else{
        this->hotPara = 0.0;
    }
    
    string strParas = dict->valueForKey("para")->getCString();
    para = strParas;
    if (strParas != "") {
        vector<string> vecParas;
        CCCommonUtils::splitString(strParas, ";", vecParas);
        for (int i=0; i<vecParas.size(); i++) {
            string value = vecParas[i];
            this->paras[i+1] = value;
        }
    }
    
    if (this->type==18) {//能购买的装备
        if(dict->objectForKey("para")){
            this->equipId = dict->valueForKey("para")->getCString();
        }else{
            this->equipId = this->itemId;
        }
    }
    //龙晶加个
    if (this->type==9) {
        if (dict->valueForKey("price_crystal")->intValue()) {
            LongJing_price = dict->valueForKey("price_crystal")->intValue();
        }
        //对应的装备id
        if(dict->objectForKey("para")){
            this->equipId = dict->valueForKey("para")->getCString();
        }else{
            this->equipId = this->itemId;
        }
        
        this->des = "";
        string show1 = CCCommonUtils::getPropById(this->equipId, "show1");
        string num1 = CCCommonUtils::getPropById(this->equipId, "num1");
        string para1 = CCCommonUtils::getPropById(this->equipId, "para1");
        string dialog_initial = CCCommonUtils::getPropById(this->equipId, "dialog_initial");
        string num_initial = CCCommonUtils::getPropById(this->equipId, "num_initial");
        string ran_initial = CCCommonUtils::getPropById(this->equipId, "ran_initial");
        
        vector<string> sVec;
        CCCommonUtils::splitString(show1, "|", sVec);
        vector<string> nVec;
        CCCommonUtils::splitString(num1, "|", nVec);
        vector<string> pVec;
        CCCommonUtils::splitString(para1, "|", pVec);
        vector<string> extSVec;
        CCCommonUtils::splitString(dialog_initial, "|", extSVec);
        vector<string> extNVec;
        CCCommonUtils::splitString(num_initial, "|", extNVec);
        vector<string> extPVec;
        CCCommonUtils::splitString(ran_initial, "|", extPVec);
        
        if (sVec.size()==1 && nVec.size()==1 && pVec.size()==1)
        {
            string format1 = "%";
            string pm1 = "+";
            vector<string> tmpVec1;
            CCCommonUtils::splitString(pVec[0], "@", tmpVec1);
            if (tmpVec1.size()>0) {
                format1 = CCCommonUtils::getEffFormat(tmpVec1[0]);
                pm1 = CCCommonUtils::getEffPM(tmpVec1[0]);
            }
            string desNum = pm1+nVec[0]+format1;
            this->des=this->des+_lang(sVec[0].c_str())+" "+desNum+"\n";
            
            if (extSVec.size()==1 && extNVec.size()==1 && extPVec.size()==1) {
                string fEx1 = "%";
                string exPm1 = "+";
                vector<string> tpVec1;
                CCCommonUtils::splitString(extPVec[0], "@", tpVec1);
                if (tpVec1.size()>0) {
                    fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                    exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
                }
                string exDesNum = exPm1+extNVec[0]+fEx1;
                this->des=this->des+_lang(extSVec[0].c_str())+" "+exDesNum+"\n";
            }
        }
        else if (sVec.size()==2 && nVec.size()==2 && pVec.size()==2)
        {
            string format1 = "%";
            string pm1 = "+";
            vector<string> tmpVec1;
            CCCommonUtils::splitString(pVec[0], "@", tmpVec1);
            if (tmpVec1.size()>0) {
                format1 = CCCommonUtils::getEffFormat(tmpVec1[0]);
                pm1 = CCCommonUtils::getEffPM(tmpVec1[0]);
            }
            
            string format2 = "%";
            string pm2 = "+";
            vector<string> tmpVec2;
            CCCommonUtils::splitString(pVec[1], "@", tmpVec2);
            if (tmpVec2.size()>0) {
                format2 = CCCommonUtils::getEffFormat(tmpVec2[0]);
                pm2 = CCCommonUtils::getEffPM(tmpVec2[0]);
            }
            
            this->des=this->des+_lang(sVec[0].c_str())+" "+pm1+nVec[0]+format1+"\n";
            this->des=this->des+_lang(sVec[1].c_str())+" "+pm2+nVec[1]+format2+"\n";
            
            if (extSVec.size()>=1 && extNVec.size()>=1 && extPVec.size()>=1) {
                string fEx1 = "%";
                string exPm1 = "+";
                vector<string> tpVec1;
                CCCommonUtils::splitString(extPVec[0], "@", tpVec1);
                if (tpVec1.size()>0) {
                    fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                    exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
                }
                this->des=this->des+_lang(extSVec[0].c_str())+" "+exPm1+extNVec[0]+fEx1+"\n";
            }
            if (extSVec.size()>=2 && extNVec.size()>=2 && extPVec.size()>=2) {
                string fEx1 = "%";
                string exPm1 = "+";
                vector<string> tpVec1;
                CCCommonUtils::splitString(extPVec[1], "@", tpVec1);
                if (tpVec1.size()>0) {
                    fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                    exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
                }
                this->des=this->des+_lang(extSVec[1].c_str())+" "+exPm1+extNVec[1]+fEx1+"\n";
            }
        }
        else if (sVec.size()==3 && nVec.size()==3 && pVec.size()==3)
        {
            string format1 = "%";
            string pm1 = "+";
            vector<string> tmpVec1;
            CCCommonUtils::splitString(pVec[0], "@", tmpVec1);
            if (tmpVec1.size()>0) {
                format1 = CCCommonUtils::getEffFormat(tmpVec1[0]);
                pm1 = CCCommonUtils::getEffPM(tmpVec1[0]);
            }
            string format2 = "%";
            string pm2 = "+";
            vector<string> tmpVec2;
            CCCommonUtils::splitString(pVec[1], "@", tmpVec2);
            if (tmpVec2.size()>0) {
                format2 = CCCommonUtils::getEffFormat(tmpVec2[0]);
                pm2 = CCCommonUtils::getEffPM(tmpVec2[0]);
            }
            string format3 = "%";
            string pm3 = "+";
            vector<string> tmpVec3;
            CCCommonUtils::splitString(pVec[2], "@", tmpVec3);
            if (tmpVec3.size()>0) {
                format3 = CCCommonUtils::getEffFormat(tmpVec3[0]);
                pm3 = CCCommonUtils::getEffPM(tmpVec3[0]);
            }
            
            this->des=this->des+_lang(sVec[0].c_str())+" "+pm1+nVec[0]+format1+"\n";
            this->des=this->des+_lang(sVec[1].c_str())+" "+pm2+nVec[1]+format2+"\n";
            this->des=this->des+_lang(sVec[2].c_str())+" "+pm3+nVec[2]+format3+"\n";
            
            if (extSVec.size()>=1 && extNVec.size()>=1 && extPVec.size()>=1) {
                string fEx1 = "%";
                string exPm1 = "+";
                vector<string> tpVec1;
                CCCommonUtils::splitString(extPVec[0], "@", tpVec1);
                if (tpVec1.size()>0) {
                    fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                    exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
                }
                this->des=this->des+_lang(extSVec[0].c_str())+" "+exPm1+extNVec[0]+fEx1+"\n";
            }
            if (extSVec.size()>=2 && extNVec.size()>=2 && extPVec.size()>=2) {
                string fEx1 = "%";
                string exPm1 = "+";
                vector<string> tpVec1;
                CCCommonUtils::splitString(extPVec[1], "@", tpVec1);
                if (tpVec1.size()>0) {
                    fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                    exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
                }
                this->des=this->des+_lang(extSVec[1].c_str())+" "+exPm1+extNVec[1]+fEx1+"\n";
            }
        }
        
    }
}

string ToolInfo::getActId()
{
    string tPara = para;
    vector<string> vec;
    CCCommonUtils::splitString(para, ";", vec);
    if (vec.size() == 2 && vec[0] == "a") {
        return vec[1];
    } else {
        return "";
    }
}

CCDictionary* ToolInfo::toDic()
{
    auto dic = CCDictionary::create();
    dic->setObject(CCString::create(CC_ITOA(itemId)), "itemId");
    dic->setObject(CCString::create(CC_ITOA(getCNT())), "cnt");
    dic->setObject(CCString::create(name), "name");
    dic->setObject(CCString::create(des), "des");
    return dic;
}

void ToolInfo::SetInfoFromServer(CCDictionary* dict,bool isinit)
{
    if(isinit){
        this->cnt = dict->valueForKey("count")->intValue();
    }else{
        setCNT(dict->valueForKey("count")->intValue());
    }
    this->uuid = dict->valueForKey("uuid")->getCString();
    if (dict->objectForKey("para1")) {
        this->para1 = dict->valueForKey("para1")->getCString();
    }
    if (dict->objectForKey("para2")) {
        this->para2 = dict->valueForKey("para2")->getCString();
    }
    if (dict->objectForKey("para3")) {
        this->para3 = dict->valueForKey("para3")->getCString();
    }

    for (int i=1; ; i++) {
        string key = "para";
        key = key+CC_ITOA(i);
        if (dict->objectForKey(key)) {
            this->paras[i] = dict->valueForKey(key)->getCString();
        }
        else{
            break;
        }
    }
}
int ToolInfo::getCNT(){
    return cnt;
}
void ToolInfo::setCNT(int var){
    if(var>cnt){
        ToolController::getInstance()->addToNewGoodsList(this->itemId);
    }
    cnt = MAX(var,0);
    if (cnt == 0) {
        ToolController::getInstance()->removeFromNewGoodsList(this->itemId);
    }
}
string ToolInfo::getName() const{
    if(type == 3 && !para.empty()){
        if(para.find_first_not_of("1234567890")==string::npos){
            return _lang_1(name, CC_CMDITOA(atoi(para.c_str())).c_str());
        }
    }
    return _lang(name);
}

ToolRewardInfo::ToolRewardInfo(CCDictionary* dict)
{
    this->resId = dict->valueForKey("type")->intValue();
    this->itemId=0;
    if(this->resId == R_GOODS) {
        auto toolDict = _dict(dict->objectForKey("value"));
        if( toolDict ){
            this->itemId = toolDict->valueForKey("id")->intValue();
            this->num = toolDict->valueForKey("num")->intValue();
        }
    } else {
        if( dict->objectForKey("value") )
            this->num = dict->valueForKey("value")->intValue();
    }
}
MerchantToolInfo::MerchantToolInfo(CCDictionary* dict){
    SetInfoFromServer(dict);
}
void MerchantToolInfo::SetInfoFromServer(CCDictionary* dict){
    itemId = dict->valueForKey("itemId")->intValue();
    this->itemNum = dict->valueForKey("itemNum")->intValue();
    this->price = dict->valueForKey("price")->intValue();
    this->priceType = dict->valueForKey("priceType")->intValue();
    this->num = dict->valueForKey("num")->intValue();
    this->color = dict->valueForKey("color")->intValue();
    if(dict->objectForKey("type")){
        this->type = (MerchantToolType)dict->valueForKey("type")->intValue();
    }else{
        this->type = MerchantTool_GOODS;
    }
    if(dict->objectForKey("price_hot")){
        this->priceHot = dict->valueForKey("price_hot")->intValue();
    }else{
        this->priceHot = 0;
    }
}