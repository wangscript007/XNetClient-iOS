//
//  CameraHelp.h
//  
//
//  Created by Zhuang Chuan Xian. on 11-6-28.
//  Copyright 2011  . All rights reserved.
//
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "H264HwEncoderImpl.h"
#include "VideoCaptureEncod.h"
#include "uuDefine.h"
class VideoCaptureEncod;

@protocol AVCamCapturePhotoDelegate;

@interface CameraHelp : NSObject
<AVCaptureVideoDataOutputSampleBufferDelegate,AVCaptureAudioDataOutputSampleBufferDelegate,H264HwEncoderImplDelegate>
{
 UIView* mPreview; 
@private
    int mFps;
	BOOL mFrontCamera;
	bool mStarted;
    
    bool isoutPutBuffer;
    H264HwEncoderImpl *h264Encoder;
    NSMutableData *  SpsPpsdata;
    VideoCaptureEncod *m_VideoCaptureEncod;
       bool isinith264Encoder;
}

@property (nonatomic,retain)AVCaptureSession* mCaptureSession;
@property (nonatomic,assign)AVCaptureDevice *mCaptureDevice;
@property (nonatomic,assign)AVCaptureDeviceInput *videoInput;
@property (nonatomic,retain)AVCaptureVideoDataOutput*avCaptureVideoDataOutput;
@property (nonatomic,assign)int mWidth;
@property (nonatomic,assign)int mHeight;
@property (nonatomic,assign)int mresType;
@property (nonatomic,assign)bool isoutPutBuffer;
@property (nonatomic,assign)bool mStarted;
@property (nonatomic,assign) id<VideoChatDelegate> videoChatDelegate;
@property (nonatomic,assign)bool  bvideoChatSnapshot;
@property (nonatomic, strong) NSLock * cameraLock;

//单例模式
+ (CameraHelp*)shareCameraHelp;
+ (void)closeCamera;
//设置前置摄像头
- (BOOL)setFrontCamera;
//设置后置摄像头
- (BOOL)setBackCamera;
//开始前设置捕获参数
//开始前设置捕获参数
-(void)prepareVideoCapture:(int) resType  andFps: (int) fps andFrontCamera:(BOOL) bfront andPreview:(UIView*) view;
- (void)setVideoDataOutputBuffer:(VideoCaptureEncod*)videoCaptureEncod;
//开始捕获
- (void)startVideoCapture;
//停止捕获
- (void)stopVideoCapture;

- (void)setPreview: (void*)preview;
- (void)reSetCamera;
- (void)setEncodebitrate:(long)bitrate;

-(void)videoChatSnapshot:(id<VideoChatDelegate>) delegate;

- (UIImage *) imageFromSampleBuffer:(CMSampleBufferRef) sampleBuffer;
- (void)initEncodeWith:(int)width hight:(int)hight;
@end

@protocol AVCamCapturePhotoDelegate <NSObject>
@optional
- (void) AVCamCapturePhotoImage:(UIImage *)image;
@end
