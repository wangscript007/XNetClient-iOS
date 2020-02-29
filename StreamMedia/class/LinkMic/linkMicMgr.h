//
//  AiYouLinkMicManage.h
//  LinkMicTalk
//
//  Created by zhangxinming on 16/7/21.
//  Copyright © 2016年 zhangxinming. All rights reserved.
//

#ifndef AiYouLinkMicManage_h
#define AiYouLinkMicManage_h
#import <Foundation/Foundation.h>
#include "uuDefine.h"

@protocol LinkMicNetWorkDelegate <NSObject>

@required

 - (void) IConnectStatusCallback:(CONNECT_NET_STATUS) cs;

@optional

- (void) INetReceiveUserList:(CLIENTUSERINFOLIST_MAP&) UserInfoList;

- (void)INetReceiveUserLogin:(unsigned long )uPeerUserID  strName:(std::string) strName useraudioID:(unsigned long) useraudioID;

- (void)INetReceiveUserLogOut:(unsigned long )uPeerUserID;

- (void)INetReceiveData:(unsigned long) uPeerUserID   strName:(std::string) strName  strData:(std::string )strData nLen:(unsigned long )nLen;

- (void)INetBroadcastData:(unsigned long) uPeerUserID pData:( const char*) pData nLen:(unsigned long )nLen;
- (void) INetAudioStatus:(unsigned long )uPeerUserID isRoom:(bool) isRoom AudioStatus:(AUDIO_SEND_STATUS )AudioStatus strName:(std::string) strName;

- (void) INetVideoCall:(unsigned long) uPeerUserID videocall:(bool) videocall  strName:(std::string) strName ;

@end


#import <Foundation/Foundation.h>

#ifdef __cplusplus
extern "C" {
#endif
 
    void  setLinkMicNetWorkDelegate( id<LinkMicNetWorkDelegate> delegate);
    
    void initLinkMicManage(const char* unityObject,const char* UnityFunSucces,const char* UnityFunFail);
   
 void loginLinkMicServer(const char* szServerHostIp,const char* szUserName,const char* szRoomId,bool isencrypt, bool isAnchor,bool ishaveVideo);
 
    void  logoutLinkMicServer();
    void  openLinkMicVideoWtihName(const char *name);
    void  closeLinkMicVideoWtihName(const char *name);
    
#ifdef __cplusplus
}
#endif

#endif /* AiYouLinkMicManage_h */
