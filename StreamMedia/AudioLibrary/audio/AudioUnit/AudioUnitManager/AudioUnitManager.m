//
//  AudioUnitManager.m
//  AUGraphApp
//
//  Created by ShengQiangLiu on 16/4/15.
//  Copyright © 2016年 www.mob.com. All rights reserved.
//

#import "AudioUnitManager.h"
#import "AUNodeWrapper.h"
#import "AudioStreamBasicDescriptionWrapper.h"
#include <pthread.h>
 


// 单声道或立体声声音的结构体，传递给渲染函数，当它需要更多音频来播放的时候，它被一个混频器单元输入总线调用。
typedef struct
{
    BOOL isStereo; // 如果数据在 audioDataRight,设置为 true
    
    UInt64 frameCount; // 音频数据中的帧总数
    UInt32 sampleNumber; // 下一个播放的采用音频
    SInt32 *audioDataLeft; // 从音频文件读取的完整的左（或单声道）的音频数据
    SInt32 *audioDataRight; // 从音频文件读取的完整的右声道的音频数据
    
} SoundStruct, *SoundStructPtr;

static AudioUnitManager* g_AudioUnitManager= nil;

@interface AudioUnitManager ()
{
    AUGraph _graph;
   // 输出单元
    AudioUnit _remoteIOUnit;
    // 多通道混合单元
    AudioUnit _mixerUnit;
    int  busCount;
    AUNode iONode;
    AUNode mixerNode;
    
    
}
// 立体声格式
@property (nonatomic, assign) AudioStreamBasicDescription stereoStreamFormat;
// 单声道格式
@property (nonatomic, assign) AudioStreamBasicDescription monoStreamFormat;
// 音频处理链的采样率
@property (nonatomic, assign) Float64 graphSampleRate;
@end

@implementation AudioUnitManager

+ (AudioUnitManager*)shareHandle
{
    if(!g_AudioUnitManager)
        g_AudioUnitManager = [[AudioUnitManager alloc] initAUGraph];
    return g_AudioUnitManager;
}

+(void)closeHandle
{
    if(g_AudioUnitManager)
    {
  
        
        [g_AudioUnitManager dealloc];
        g_AudioUnitManager = nil;
    }
}

- (void)dealloc
{
        if (_graph)
        {
            AUGraphStop(_graph);
            AUGraphIsRunning(_graph, &_playing);
            AUGraphClose(_graph);
            DisposeAUGraph(_graph);
            AUGraphUninitialize(_graph);
    
        }
    [super dealloc];
}

- (void)setupDecoder
{
    _decoder = [[CSIOpusDecoder decoderWithSampleRate:self.graphSampleRate channels:1 frameDuration:0.02]retain];
}

- (void)setaudiodata:(unsigned char *)data  length:(int)length audioid:(long)audioid
{
    @autoreleasepool {
       (void)pthread_mutex_lock(&m_pMutex); 
        NSData *dedata = [self.decoder decode:(const unsigned char*)data len:length];
         Audio_MAP_data::iterator iter = audio_MAP_data.find(audioid);
        if(iter != audio_MAP_data.end())
        {
          NSMutableData    *preaudiodata=(*iter).second;
            if(preaudiodata&&dedata)
            {
                [ preaudiodata appendData:dedata];
            }
       
        }
        else
        {
            NSMutableData    * preaudiodata = [[NSMutableData alloc]init];
            [ preaudiodata appendData:dedata];
            audio_MAP_data[audioid] = preaudiodata;
        }
        pthread_mutex_unlock(&m_pMutex);
        usleep(50);
    }
}

- (NSData*)getaudiodata:(int)nframes audioid:(long)audioid
{
    (void)pthread_mutex_lock(&m_pMutex);
    NSData *subFileData = nil;
    
    if(audio_MAP_data.size()==0)
    {
           pthread_mutex_unlock(&m_pMutex);
        return nil;
    }
    Audio_MAP_data::iterator iter = audio_MAP_data.find(audioid);
    if(iter == audio_MAP_data.end())
    {
        pthread_mutex_unlock(&m_pMutex);
        return nil;
    }
    
    NSMutableData    *preaudiodata=(*iter).second;
    
    if (preaudiodata&&[ preaudiodata length]>nframes*2)
    {
        NSRange range = NSMakeRange(0,nframes*2);
        subFileData =  [ preaudiodata  subdataWithRange:range];
        [preaudiodata replaceBytesInRange:range withBytes:NULL length:0];
    }
    pthread_mutex_unlock(&m_pMutex);
    return subFileData;
}

- (instancetype)initAUGraph
{
    if (self = [super init])
    {
        _playing = NO;
        busCount = 2;
        
      self.graphSampleRate = 16000;
      [self setupDecoder];
      [self setupStereoStreamFormat];
     [self setupMonoStreamFormat];
        
//        AVAudioSession* session = [AVAudioSession sharedInstance];
//        NSError *error = nil;
//
//        [session setCategory:AVAudioSessionCategoryPlayAndRecord withOptions:AVAudioSessionCategoryOptionDefaultToSpeaker error:&error];
 
//        [self prepareAUGraph];
        

    }
    return self;
}

//
//
- (void)prepareAUGraph
{
  // 创建一个音频处理图
    CheckError(NewAUGraph(&_graph), "创建一个音频处理图错误");
    // 打开音频处理图
    CheckError(AUGraphOpen(_graph), "打开音频处理图错误");


    // 输出单元
    [AUNodeWrapper setupRemoteIOUnitWithGraph:_graph
                                       ioNode:&iONode
                                       ioUnit:&_remoteIOUnit];

    int val = 0;
   int len = sizeof(UInt32);
 CheckError(AudioUnitSetProperty(_remoteIOUnit, kAUVoiceIOProperty_BypassVoiceProcessing, kAudioUnitScope_Input, 0, &val, len),"kAUVoiceIOProperty_BypassVoiceProcessing错误");


    // 混频器单元
    [AUNodeWrapper setupMultiChannelMixerWithGraph:_graph
                                         mixerNode:&mixerNode
                                         mixerUnit:&_mixerUnit];


    // 连接mixer的输出到I/O单元的输入
    CheckError(AUGraphConnectNodeInput(_graph, mixerNode, 0, iONode, 0), "连接mixer的输出到I/O单元的输入错误");


    CheckError(AudioUnitSetProperty(_mixerUnit,
                                    kAudioUnitProperty_ElementCount,
                                    kAudioUnitScope_Input, 0,
                                    &busCount, sizeof(busCount)), "多通道混合单元设置 bus 错误");


//         设置 kAudioUnitProperty_MaximumFramesPerSlice， 防止屏幕锁住时停止
        UInt32 maximumFramesPerSlice = 4096;
        CheckError(AudioUnitSetProperty(_mixerUnit,
                                        kAudioUnitProperty_MaximumFramesPerSlice,
                                        kAudioUnitScope_Global, 0,
                                        &maximumFramesPerSlice,
                                        sizeof(maximumFramesPerSlice)),
    "kAudioUnitProperty_MaximumFramesPerSlice 设置错误");
    // 将渲染函数附加到每个输入总线上
    for (UInt16 busNumber = 0; busNumber < busCount; busNumber++)
    {
        //
        AURenderCallbackStruct inputCallbackStruct;
        inputCallbackStruct.inputProc = &inputRenderCallback;
        inputCallbackStruct.inputProcRefCon = self;

        // 给指定节点的指定输入设置回调
        CheckError(AUGraphSetNodeInputCallback(_graph, mixerNode, busNumber, &inputCallbackStruct), "AUGraphSetNodeInputCallback 错误");
    }

    for (UInt16 busNumber = 0; busNumber < busCount; busNumber++)
    {

        //    // 给混频器单元输入总线（guitarBus）设置立体声流格式
        CheckError(AudioUnitSetProperty(_mixerUnit,
                                        kAudioUnitProperty_StreamFormat,
                                        kAudioUnitScope_Input,
                                        busNumber,
                                        &_stereoStreamFormat,
                                        sizeof(_stereoStreamFormat)), "设置流格式错误");


    }


//


    // 设置混频器输出采样率格式
    CheckError(AudioUnitSetProperty(_mixerUnit,
                                    kAudioUnitProperty_SampleRate,
                                    kAudioUnitScope_Output, 0,
                                    &_graphSampleRate,
                                    sizeof(_graphSampleRate)), "设置混合器输出采样率格式错误");




    AUGraphInitialize(_graph);

}



static OSStatus inputRenderCallback
(
             void                        *inRefCon,
             AudioUnitRenderActionFlags  *ioActionFlags,
             const AudioTimeStamp        *inTimeStamp,
             UInt32                      inBusNumber,
             UInt32                      inNumberFrames,
             AudioBufferList             *ioData
 )
{
    
    AudioUnitManager *audioUnitTool=(AudioUnitManager*)inRefCon;
    NSData *buff=[audioUnitTool getaudiodata:inNumberFrames audioid:inBusNumber];
    
  
    if (buff)
    {
        memcpy(ioData->mBuffers[0].mData,
               [buff bytes],
               [buff length] );
             ioData->mBuffers[0].mDataByteSize = [buff length] ;
        ioData->mBuffers[0].mNumberChannels = 1;
    }
    else
    {
        for (UInt32 i=0; i < ioData->mNumberBuffers; i++)
        {
            memset(ioData->mBuffers[i].mData, 0, ioData->mBuffers[i].mDataByteSize);
        }
    }
  
    return noErr;
}


- (void)setupStereoStreamFormat
{
  // ASBD 使用前记得初始化为 0
    self.stereoStreamFormat = (AudioStreamBasicDescription){0};
    _stereoStreamFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
    _stereoStreamFormat.mBitsPerChannel = sizeof(short) * 8;
    _stereoStreamFormat.mFormatID = kAudioFormatLinearPCM;
    _stereoStreamFormat.mSampleRate = 16000;
    _stereoStreamFormat.mChannelsPerFrame = 1 ;
    _stereoStreamFormat.mBytesPerFrame = 1 * sizeof(short);
    _stereoStreamFormat.mBytesPerPacket = 1 * sizeof(short);
    _stereoStreamFormat.mFramesPerPacket = 1;
}


- (void)setupMonoStreamFormat
{
     self.monoStreamFormat = (AudioStreamBasicDescription) {0};
  _monoStreamFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
    _monoStreamFormat.mBitsPerChannel = sizeof(short) * 8;
    _monoStreamFormat.mFormatID = kAudioFormatLinearPCM;
    _monoStreamFormat.mSampleRate = 16000;
    _monoStreamFormat.mChannelsPerFrame = 1 ;
    _monoStreamFormat.mBytesPerFrame = 1 * sizeof(short);
    _monoStreamFormat.mBytesPerPacket = 1 * sizeof(short);
    _monoStreamFormat.mFramesPerPacket = 1;
}

- (void)start:(int)audioCount
{
    busCount = audioCount;
 
    [self prepareAUGraph];
    
 if (_graph)
    {
        AUGraphStart(_graph);
        AUGraphIsRunning(_graph, &_playing);
    }
}

- (void)stop
{
    if (_graph)
    {
        AUGraphStop(_graph);
        AUGraphIsRunning(_graph, &_playing);
        
        AUGraphDisconnectNodeInput(_graph, iONode,0);
        AUGraphRemoveNode(_graph, iONode);
         AUGraphUninitialize(_graph);
        AUGraphClose(_graph);
        DisposeAUGraph(_graph);
       
        _graph = NULL;
   
        
//            AudioUnitUninitialize(_remoteIOUnit);
//            AudioUnitUninitialize(_mixerUnit);
    }


           (void)pthread_mutex_lock(&m_pMutex); 
    std::map<unsigned long,  NSMutableData*>::iterator it=audio_MAP_data.begin();
  
    while (it!=audio_MAP_data.end())
    {
        NSMutableData    *preaudiodata=(*it).second;
        if(preaudiodata)
        {
            [preaudiodata resetBytesInRange:NSMakeRange(0, [preaudiodata length])];
            
            [preaudiodata setLength:0];
        }
        it++;
    }
         pthread_mutex_unlock(&m_pMutex);
    
  
    
}
 

static void CheckError(OSStatus error, const char *operation)
{
    if (error == noErr) return;
    char errorString[20];
    // See if it appears to be a 4-char-code
    *(UInt32 *)(errorString + 1) = CFSwapInt32HostToBig(error);
    if (isprint(errorString[1]) && isprint(errorString[2]) && isprint(errorString[3]) && isprint(errorString[4]))
    {
        errorString[0] = errorString[5] = '\'';
        errorString[6] = '\0';
    } else
        // No, format it as an integer
        sprintf(errorString, "%d", (int)error);
    fprintf(stderr, "Error: %s (%s)\n", operation, errorString);
    exit(1);
}


@end
