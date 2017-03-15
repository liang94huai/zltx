//
//  ChatServicCSManagerDefs.h
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#ifndef ChatServicCSManagerDefs_h
#define ChatServicCSManagerDefs_h
/*!
 @enum
 @brief 聊天类型
 @constant CSMessageBodyType_Text 文本类型
 @constant CSMessageBodyType_Image 图片类型
 @constant CSMessageBodyType_Video 视频类型
 @constant CSMessageBodyType_Location 位置类型
 @constant CSMessageBodyType_Voice 语音类型
 @constant CSMessageBodyType_File 文件类型
 @constant CSMessageBodyType_Command 命令类型
 */
typedef NS_ENUM(NSInteger, CSMessageBodyType) {
     /** 0普通聊天消息 */
    CSMessageBodyType_Text_Normal = 0,
    /** 1普通系统消息 */
    CSMessageBodyType_Text_System = 1,
    /** 4战报分享 */
    CSMessageBodyType_Text_FightReport = 4,
    /** 5侦查战报分享 */
    CSMessageBodyType_Text_InvestigateReport = 5,
    /** 6喇叭消息 */
    CSMessageBodyType_Text_Loudspeaker = 6,
    /** 7装备分享 */
    CSMessageBodyType_Text_EquipShare = 7,
    /** 8打招呼 */
    CSMessageBodyType_Text_SayHello = 8,
    /** 9联盟集结 */
    CSMessageBodyType_Text_AllianceConciles = 9,
    /** 10轮盘分享 */
    CSMessageBodyType_Text_TurntableShare = 10,
    /** 11联盟任务 */
    CSMessageBodyType_Text_AllianceTask = 11,
    /** 12 红包 */
    CSMessageBodyType_RedPackge = 12,
    /** 100 聊天室系统消息 */
    CSMessageBodyType_Text_ChatRoomSystemMsg = 100,
    /** 200mod邮件 */
    CSMessageBodyType_Text_ModMsg = 200 ,
    /** 不能解析 */
    CSMessageBodyType_NotCanParse =300,
    
    
    CSMessageBodyType_Image,
    CSMessageBodyType_Video,
    CSMessageBodyType_Location,
    CSMessageBodyType_Voice,
    CSMessageBodyType_File,
    CSMessageBodyType_Command
};

/*!
 @enum
 @brief 聊天消息发送状态
 */
typedef NS_ENUM(NSInteger, CSMessageSendState) {
    /**  正在发送 */
    CSMessageSendState_Delivering =0,
    /** 已发送,失败*/
    CSMessageSendState_Failure =1,
    /** 已发送,成功  */
    CSMessageSendState_Delivered=2,
    /** 待发送 */
    CSMessageSendState_Pending = 3,
};

/*!
 @brief 消息回执类型
 @constant CSReceiptTypeRequest   回执请求
 @constant CSReceiptTypeResponse  回执响应
 */
typedef NS_ENUM(NSInteger, CSReceiptType){
    CSReceiptTypeRequest  = 0,
    CSReceiptTypeResponse,
};

/*!
 @brief 消息类型
 @constant CSMessageTypeChat            单聊消息
 @constant CSMessageTypeChatRoom        聊天室消息
 */
typedef NS_ENUM(NSInteger, CSMessageType){
    CSMessageTypeChat,
    CSMessageTypeChatRoom
};
#endif /* ChatServicCSManagerDefs_h */
