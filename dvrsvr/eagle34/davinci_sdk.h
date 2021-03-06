#ifndef __HIK_DAV_SDK_INTERFACE_H__
#define __HIK_DAV_SDK_INTERFACE_H__


#ifdef __cplusplus
extern "C" {
#endif

//��������
#define ErrorCodeSuccess		0x0000

#define ErrorCodeDSPUninit		0x0001
#define ErrorCodeDSPNotReady		0x0002
#define ErrorCodeDSPLoadFail		0x0004
#define ErrorCodeDSPSyncObjectFail	0x0005
#define ErrorCodeDSPCountOverflow	0x0009
#define ErrorCodeEncodeChannelOverflow	0x000a
#define ErrorCodeDecodeChannelOverflow	0x000b
#define ErrorCodeBoardOverflow		0x000c
#define ErrorCodeDSPHexBlockLenOverflow	0x000d
#define ErrorCodeDSPProgramCheckoutFail	0x000e
#define ErrorCodeDSPInitRecheckFail	0x000f
#define ErrorCodeDSPBusy		0x0010
#define ErrorCodeDSPCOMFail		0x0011


#define ErrorCodeNoCardInstalled	0x0102
#define ErrorCodeIoctlFail		0x0103
#define ErrorCodeMemLocateFail		0x0104
#define ErrorCodeDuplicateSN		0x0105
#define ErrorCodeCreateThreadFail	0x0106
#define ErrorCodeDSPCmdInvalid		0x0107
#define ErrorCodeHOSTCmdInvalid		0x0108
#define ErrorCodeDSPRespondCmdFail	0x0109
#define ErrorCodeOrderFail		0x010a
#define ErrorCodeKernelFail		0x010b
#define ErrorCodeStreamBufUnderflow	0x010c

#define ErrorCodeChannelOutofRange	0x0200
#define ErrorCodeInvalidEncodeChannel	0x0201
#define ErrorCodeInvalidArgument	0x0202
#define ErrorCodeNotSupport		0x0203
#define ErrorCodeMmapFail		0x0204

#define ErrorCodeFileInvalid			0x0301
#define ErrorCodeOpenFileFail			0x0302
#define ErrorCodeFileSizeZero			0x0303
#define ErrorCodeBadFileFormat			0x0304
#define ErrorCodeBadFileHeader			0x0305
#define ErrorCodeParaOver			0x0306
#define ErrorCodeCreateFileFail			0x0307
#define ErrorCodeNoSpare			0x0308
#define ErrorCodeInvalidDevice			0x0309
#define ErrorCodeInsufficientMem		0x030a
#define ErrorCodeCardSerialError		0x030b
#define ErrorCodeInvalidDecodeChannel		0x030c
#define ErrorCodeOutOfMemory			0x030d
#define ErrorCodeSemCreateFail			0x030e


//SDK�汾��
#define MAJ_VER(version) ( (version&0xff00)>>8 )
#define MIN_VER(version) ( (version&&0x00ff) )

//�����Ͷ���
#define STREAM_TYPE_VIDOE 	1	//��Ƶ��
#define STREAM_TYPE_AUDIO 	2	//��Ƶ��
#define STREAM_TYPE_AVSYN 	3	//����Ƶͬ����

#define DECODE_FILE_MODE		1			/*�ļ��ط�ģʽ*/

//��������ģʽ
#define MODE_VBR 0 	//������
#define MODE_CBR 1	//�㶨����

#define SDK_MAX_MENU_DISP_NUM 3 //�˵�������ʾ��������

/////////////////////////////////////////////////////////////////////////////
//���ݻص�֡���Ͷ���
 #define     FRAME_TYPE_VIDEO_I_PS           0x1        /*PS��װ��������ƵI֡*/
 #define	FRAME_TYPE_VIDEO_P_PS        0x2        /*PS��װ��ͨ����ƵP֡*/
 #define	FRAME_TYPE_VIDEO_BP_PS      0x3        /*PS��װ��ͨ����ƵBP��BBP֡*/

 #define	FRAME_TYPE_VIDEO_I_TS         0x4        /*TS��װ��������ƵI֡*/
 #define	FRAME_TYPE_VIDEO_P_TS        0x5        /*TS��װ��������ƵP֡*/
 #define	FRAME_TYPE_VIDEO_BP_TS      0x6        /*TS��װ��������ƵBP��BBP֡*/
 #define	FRAME_TYPE_VIDEO_I_RTP       0x7        /*RTP��װ��������ƵI֡*/
 #define	FRAME_TYPE_VIDEO_P_RTP      0x8        /*RTP��װ��������ƵP֡*/
 #define	FRAME_TYPE_VIDEO_BP_RTP    0x9       /*RTP��װ��������ƵBP��BBP֡*/

 #define	FRAME_TYPE_VIDEO_I_HIK         0xa        /*HIK��װ��������ƵI֡*/
 #define	FRAME_TYPE_VIDEO_P_HIK        0xb        /*HIK��װ��������ƵP֡*/
 #define	FRAME_TYPE_VIDEO_BP_HIK      0xc        /*HIK��װ��������ƵBP��BBP֡*/
#define    FRAME_TYPE_VIDEO_I_ORG          0xd        /*�޷�װ��������ƵI֡*/
#define	FRAME_TYPE_VIDEO_P_ORG          0xe        /*�޷�װ��������ƵP֡*/
#define	FRAME_TYPE_VIDEO_BP_ORG         0xf        /*�޷�װ��������ƵBP��BBP֡*/

 #define	FRAME_TYPE_VIDEO_SUB_I_PS  		0x11        /*PS��װ��������ƵI֡*/
 #define	FRAME_TYPE_VIDEO_SUB_P_PS  	0x12        /*PS��װ��������ƵP֡*/
 #define	FRAME_TYPE_VIDEO_SUB_BP_PS  	0x13        /*PS��װ��������ƵBP��BBP֡*/

 #define	FRAME_TYPE_VIDEO_SUB_I_TS       0x14        /*TS��װ��������ƵI֡*/
 #define	FRAME_TYPE_VIDEO_SUB_P_TS      0x15        /*TS��װ��������ƵP֡*/
 #define	FRAME_TYPE_VIDEO_SUB_BP_TS    0x16        /*TS��װ��������ƵBP��BBP֡*/

 #define	FRAME_TYPE_VIDEO_SUB_I_RTP  	0x17       /*RTP��װ��������ƵI֡*/
 #define	FRAME_TYPE_VIDEO_SUB_P_RTP  	0x18       /*RTP��װ��������ƵP֡*/
 #define	FRAME_TYPE_VIDEO_SUB_BP_RTP     0x19       /*RTP��װ��������ƵBP��BBP֡*/

 #define	FRAME_TYPE_VIDEO_SUB_I_HIK      0x1a       /*HIK��װ��������ƵI֡*/
 #define	FRAME_TYPE_VIDEO_SUB_P_HIK     0x1b      /*HIK��װ��������ƵP֡*/
 #define	FRAME_TYPE_VIDEO_SUB_BP_HIK     0x1c       /*HIK��װ��������ƵBP��BBP֡*/

#define    FRAME_TYPE_VIDEO_SUB_I_ORG      0x1d       /*�޷�װ��������ƵI֡*/
#define	FRAME_TYPE_VIDEO_SUB_P_ORG      0x1e       /*�޷�װ��������ƵP֡*/
#define	FRAME_TYPE_VIDEO_SUB_BP_ORG     0x1f       /*�޷�װ��������ƵBP��BBP֡*/
	/* ��Ƶ���� */

 #define	FRAME_TYPE_AUDIO_PS             0x21       /*PS��װ��������Ƶ֡*/
 #define	FRAME_TYPE_AUDIO_TS             0x22       /*TS��װ��������Ƶ֡*/
 #define	FRAME_TYPE_AUDIO_RTP           0x23       /*RTP��װ��������Ƶ֡*/
 #define	FRAME_TYPE_AUDIO_HIK           0x24       /*HIK��װ��������Ƶ֡*/

 #define	FRAME_TYPE_AUDIO_SUB_PS        0x28       /*PS��װ��������Ƶ֡*/
 #define	FRAME_TYPE_AUDIO_SUB_TS        0x29       /*TS��װ��������Ƶ֡*/
 #define	FRAME_TYPE_AUDIO_SUB_RTP      0x2a       /*RTP��װ��������Ƶ֡*/
 #define	FRAME_TYPE_AUDIO_SUB_HIK      0x2b       /*HIK��װ��������Ƶ֡*/

	/* ���������� */
 #define	FRAME_TYPE_MD_RESULT  			0x31       /*�ƶ���������*/
 #define	FRAME_TYPE_YUV_IMG          	0x32       /*ץͼ����YUV*/
 #define	FRAME_TYPE_JPEG_IMG         	0x33       /*ץͼ����JPEG*/
 #define	FRAME_TYPE_IMG_STREAM  			0x34       /*ԭʼͼ����*/
 #define	FRAME_TYPE_RAW_AUDIO  			0x35       /*ԭʼ��Ƶ��*/

 #define	STREAM_MPEG4_HEADER					0x38		/*mpeg4����ͷ*/
 #define	STREAM_MPEG4_SUB_HEADER				0x39		/*mpeg4����ͷ*/
 #define	STREAM_H264_HEADER					0x3a		/*h264sps����ͷ rtp��װʱ��*/
 #define	STREAM_H264_SUB_HEADER				0x3b		/*h264sps����ͷ rtp��װʱ��*/
 #define	FRAME_TYPE_HEADER				0x3c		/*������װ�ļ�ͷ*/
 #define	FRAME_TYPE_SUB_HEADER			0x3d		/*������װ�ļ�ͷ*/

//bill adds start
 #define FRAME_TYPE_AUDIO                       0x40

//bill adds end
//RS485���ݶ���
//baudrate 
#define BAUD50 		50
#define BAUD75 		75
#define BAUD110 	110
#define BAUD150		150
#define BAUD300		300
#define BAUD600		600
#define BAUD1200	1200
#define BAUD2400	2400
#define BAUD4800	4800
#define BAUD9600	9600
#define BAUD19200	19200
#define BAUD38400	38400
#define BAUD57600	57600
#define BAUD76800	76800
#define BAUD115200	115200

//data bits defines
#define DATAB5     	0
#define DATAB6     	1
#define DATAB7     	2
#define DATAB8     	3

//stop bits defines
#define STOPB0		0
#define STOPB1		1

//parity defines 
#define NOPARITY	0
#define ODDPARITY	1
#define EVENPARITY	2

//flow control defines 
//��֧������
/* flow control defines */
#define	NOCTRL		0
#define SOFTCTRL	1		/* xon/xoff flow control */
#define HARDCTRL	2		/* RTS/CTS flow control */
typedef enum{
#define DAVINCI_SDK_DVS 0X0
	//HC
	DS_SDK_SDK_6101HC = DAVINCI_SDK_DVS + 1,
	DS_SDK_6102HC = DAVINCI_SDK_DVS + 2,
	DS_SDK_6103HC = DAVINCI_SDK_DVS + 3,
	DS_SDK_6104HC = DAVINCI_SDK_DVS + 4,
	DS_SDK_6105HC = DAVINCI_SDK_DVS + 5,
	DS_SDK_6106HC = DAVINCI_SDK_DVS + 6,
	DS_SDK_6107HC = DAVINCI_SDK_DVS + 7,
	DS_SDK_6108HC = DAVINCI_SDK_DVS + 8,
	DS_SDK_6109HC = DAVINCI_SDK_DVS + 9,
	DS_SDK_6110HC = DAVINCI_SDK_DVS + 10,
	DS_SDK_6111HC = DAVINCI_SDK_DVS + 11,
	DS_SDK_6112HC = DAVINCI_SDK_DVS + 12,
	DS_SDK_6113HC = DAVINCI_SDK_DVS + 13,
	DS_SDK_6114HC = DAVINCI_SDK_DVS + 14,
	DS_SDK_6115HC = DAVINCI_SDK_DVS + 15,
	DS_SDK_6116HC = DAVINCI_SDK_DVS + 16,

	//HT
	DS_SDK_6101HT = DAVINCI_SDK_DVS + 17,
	DS_SDK_6102HT = DAVINCI_SDK_DVS + 18,
	DS_SDK_6103HT = DAVINCI_SDK_DVS + 19,
	DS_SDK_6104HT = DAVINCI_SDK_DVS + 20,
	DS_SDK_6105HT = DAVINCI_SDK_DVS + 21,
	DS_SDK_6106HT = DAVINCI_SDK_DVS + 22,
	DS_SDK_6107HT = DAVINCI_SDK_DVS + 23,
	DS_SDK_6108HT = DAVINCI_SDK_DVS + 24,
	DS_SDK_6109HT = DAVINCI_SDK_DVS + 25,
	DS_SDK_6110HT = DAVINCI_SDK_DVS + 26,
	DS_SDK_6111HT = DAVINCI_SDK_DVS + 27,
	DS_SDK_6112HT = DAVINCI_SDK_DVS + 28,
	DS_SDK_6113HT = DAVINCI_SDK_DVS + 29,
	DS_SDK_6114HT = DAVINCI_SDK_DVS + 30,
	DS_SDK_6115HT = DAVINCI_SDK_DVS + 31,
	DS_SDK_6116HT = DAVINCI_SDK_DVS + 32,

	//HF
	DS_SDK_6101HF = DAVINCI_SDK_DVS + 33,
	DS_SDK_6102HF = DAVINCI_SDK_DVS + 34,
	DS_SDK_6103HF = DAVINCI_SDK_DVS + 35,
	DS_SDK_6104HF = DAVINCI_SDK_DVS + 36,

#define DAVINCI_SDK_DVS_A 0X40
	//HC 
	DS_SDK_6101HC_A = DAVINCI_SDK_DVS_A + 1,
	DS_SDK_6102HC_A = DAVINCI_SDK_DVS_A + 2,
	DS_SDK_6103HC_A = DAVINCI_SDK_DVS_A + 3,
	DS_SDK_6104HC_A = DAVINCI_SDK_DVS_A + 4,

	//HT
	DS_SDK_6101HT_A = DAVINCI_SDK_DVS_A + 17,
	DS_SDK_6102HT_A = DAVINCI_SDK_DVS_A + 18,
	DS_SDK_6103HT_A = DAVINCI_SDK_DVS_A + 19,
	DS_SDK_6104HT_A = DAVINCI_SDK_DVS_A + 20,

	//HF
	DS_SDK_6101HF_A = DAVINCI_SDK_DVS_A + 33,
	DS_SDK_6102HF_A = DAVINCI_SDK_DVS_A + 34,
	DS_SDK_6103HF_A = DAVINCI_SDK_DVS_A + 35,
	DS_SDK_6104HF_A = DAVINCI_SDK_DVS_A + 36,


#define DAVINCI_SDK_DVR 0x100
	//HS
	DS_SDK_7101HS = DAVINCI_SDK_DVR + 1,
	DS_SDK_7102HS = DAVINCI_SDK_DVR + 2,
	DS_SDK_7103HS = DAVINCI_SDK_DVR + 3,
	DS_SDK_7104HS = DAVINCI_SDK_DVR + 4,
	DS_SDK_7105HS = DAVINCI_SDK_DVR + 5,
	DS_SDK_7106HS = DAVINCI_SDK_DVR + 6,
	DS_SDK_7107HS = DAVINCI_SDK_DVR + 7,
	DS_SDK_7108HS = DAVINCI_SDK_DVR + 8,
	DS_SDK_7109HS = DAVINCI_SDK_DVR + 9,
	DS_SDK_7110HS = DAVINCI_SDK_DVR + 10,
	DS_SDK_7111HS = DAVINCI_SDK_DVR + 11,
	DS_SDK_7112HS = DAVINCI_SDK_DVR + 12,
	DS_SDK_7113HS = DAVINCI_SDK_DVR + 13,
	DS_SDK_7114HS = DAVINCI_SDK_DVR + 14,
	DS_SDK_7115HS = DAVINCI_SDK_DVR + 15,
	DS_SDK_7116HS = DAVINCI_SDK_DVR + 16,

	//HC
	DS_SDK_7101HC = DAVINCI_SDK_DVR + 17,
	DS_SDK_7102HC = DAVINCI_SDK_DVR + 18,
	DS_SDK_7103HC = DAVINCI_SDK_DVR + 19,
	DS_SDK_7104HC = DAVINCI_SDK_DVR + 20,
	DS_SDK_7105HC = DAVINCI_SDK_DVR + 21,
	DS_SDK_7106HC = DAVINCI_SDK_DVR + 22,
	DS_SDK_7107HC = DAVINCI_SDK_DVR + 23,
	DS_SDK_7108HC = DAVINCI_SDK_DVR + 24,
	DS_SDK_7109HC = DAVINCI_SDK_DVR + 25,
	DS_SDK_7110HC = DAVINCI_SDK_DVR + 26,
	DS_SDK_7111HC = DAVINCI_SDK_DVR + 27,
	DS_SDK_7112HC = DAVINCI_SDK_DVR + 28,
	DS_SDK_7113HC = DAVINCI_SDK_DVR + 29,
	DS_SDK_7114HC = DAVINCI_SDK_DVR + 30,
	DS_SDK_7115HC = DAVINCI_SDK_DVR + 31,
	DS_SDK_7116HC = DAVINCI_SDK_DVR + 32,

	//HF

#define DAVINCI_SDK_IPMODULE 0x200
	DS_SDK_6101HF_IP = DAVINCI_SDK_IPMODULE+ 1, 	//6446
	DS_SDK_6101H_IP  = DAVINCI_SDK_IPMODULE+ 2 	//6437


/*-------------------------------------------------------*/

}DEVICE_TYPE;


//***********************************
//���ݽṹ����
//***********************************

struct version_info 
{
	unsigned short version;
	unsigned short build;
	unsigned int reserved;
};

struct board_info
{
	unsigned int board_type;
	unsigned char production_date[8];
	unsigned char sn[48];
	unsigned int flash_size;
	unsigned int mem_size;
	unsigned int dsp_count;
	unsigned int enc_chan;
	unsigned int dec_chan;
	unsigned int alarm_in;
	unsigned int alarm_out;
	unsigned int disk_num;
	unsigned char mac[6];
	unsigned char reserved[34];
};

struct frame_statistics
{
	unsigned int video_frames;	/*��������Ƶ֡��*/
	unsigned int audio_frames;	/*��������Ƶ֡��*/
	unsigned int frames_lost;	/*��������Ƶ֡��ʧ���*/
	unsigned int queue_overflow;	/*��Ƶ���붪ʧ֡��*/
	unsigned int cur_bps;		/*����*/
};

struct dec_statistics
{
      unsigned int dec_video_num;  /*��������Ƶ֡��*/
      unsigned int dec_passedV_num;/*������������Ƶ֡��*/
      unsigned int dec_passedA_num;/*������������Ƶ֡��*/
      unsigned int dec_audio_num;  /*��������Ƶ֡��*/
      unsigned int dec_sec;/*��������*/
};

struct disk_status
{
};

struct RECT 
{
	int left;
	int top;
	int right;
	int bottom;
};

struct RECT_EX
{
       int x,y,w,h;
};
struct SYSTEMTIME
{
	unsigned short year;
	unsigned short month;
	unsigned short dayofweek;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
	unsigned short milliseconds;
};

enum video_standard
{
   STANDARD_NONE = 0,
   STANDARD_NTSC,
   STANDARD_PAL 
};               

enum picture_format {
	ENC_QCIF =2,
	ENC_CIF = 1,
	ENC_2CIF =4,
	ENC_DCIF =0,
	ENC_4CIF =3,
	ENC_D1 = 0xf
};

struct MENU_COPY_PARAM
{
	int       index;/*��������ѡ��*/
	int       bFullScreen;/*�Ƿ�ȫ����ʾ*/
	unsigned int x,y,w,h;/*��ȫ����ʾʱָ����ʾ����λ��,�������㣬���͸�*/
	int       res[2];
};

/****************************�˵�ALPHA*****************************/
#define SDK_ALPHA_MENU_BACK11				0		/*�˵������Ա�Ϊ1:1*/
#define SDK_ALPHA_MENU_BACK31				1		/*�˵������Ա�Ϊ3:1*/
#define SDK_ALPHA_MENU_BACK13				2		/*�˵������Ա�Ϊ1:3*/
#define SDK_ALPHA_MENU_BACK_NONE				7		/*��͸��*/
struct DISP_PARAM
{
	int		bEnable;	/*�˵�����*/
	int		menuAlpha;/*�˵��뱳��ͼ���Աȶ�*/
	unsigned int x;		    /*��ȫ����ʾʱָ����ʾ����λ��*/
	unsigned int y;
	unsigned int w;
	unsigned int h;
};
struct SDK_MENU_DISP_PARAM
{
	int	index;/*��������ѡ��*/
	int	bFilter;/*�Ƿ��˲�*/
	int	bFullScreen;/*�Ƿ�ȫ����ʾ*/
	struct DISP_PARAM FullScreenDispParam;/*ȫ����ʾʱָ����ʾ���ڲ���*/
	struct DISP_PARAM DispParam[SDK_MAX_MENU_DISP_NUM]; /*��ȫ����ʾʱָ����ʾ���ڲ���*/
	int		res[2];
};

struct SDK_MENU_REFRESH_PARAM
{
	int	index;/*��������ѡ��*/
	int	bFullScreenRefresh;/*�Ƿ�ȫ��ˢ��*/
	int	x;/*��ȫ��ˢ��ʱָ��ˢ������*/
	int	y;
	int	w;
	int	h;
};

struct SDK_MENU_BUF_PARAM
{
	int		index;/*��ȡ��������ѡ������Ӧ�ò㴫����SDK��*/
	unsigned char * pMenuY;/*�⼸��������SDK������*/
	unsigned char * pMenuU;
	unsigned char * pMenuV;
	unsigned int	MenuBufW;/*�˵�����������Ϊ1024*/
	unsigned int	MenuBufH;/*�������߶�Ϊ768*/
};


struct SDK_POS
{
	unsigned int	x;
	unsigned int	y;
};

struct SDK_MOUSE_POS
{
	unsigned int index;/*��������ѡ��*/
	struct SDK_POS pos;
};

struct LOGO_DISP_PARAM
{
   unsigned int	bTranslucent;	/*�Ƿ���͸��*/
   unsigned int x,y;
   unsigned int w;/*������256����Ϊ16��������*/
   unsigned int h;/*������128����Ϊ8��������*/
   unsigned int flash;/*��˸���ơ�����flash��(2<<8)|1 ��ʾLogo��ʾ2�룬ֹͣ1��*/
   unsigned int res[2];
};


typedef struct
{
    int channel;
    int size;
    int frameType;
    unsigned int standardTime;/* TS/PS��׼ʱ��,��λΪ90kHzʱ��,ȡ����32λ,��45kHzʱ�� */
    struct SYSTEMTIME absTime;/*����ʱ��*/
    void * pBuf;
}CALLBACK_DATA;

//**************************
// API����
//**************************

//��ȡSDK�汾��Ϣ
int GetSDKVersion(struct version_info* info);
//ϵͳ��ʼ��
int InitSystem();
int FiniSystem();

//��ȡ����������Ϣ
int GetBoardInfo(struct board_info* info);

//���Ź�
int StartWatchdog(unsigned int timer);
int StopWatchdog();

//�л�FLASH��HDISK
int EnableATA(void);
int EnableFLASH(void);

//��ʼ��485�豸
int InitRS485(int fd,int baudrate, int data, int stop, int parity, int flowcontrol);

//������������
int GetAlarmIn(unsigned int* alarm_in_status);
int SetAlarmOut(unsigned int alarm_out_status);
int GetAlarmOut(unsigned int * out_status);


int SetOSDDisplayMode(int channel, int brightness, int translucent, int param, int line_count,unsigned short**format);

/*
@osd_type
#define OSD_TYPE_ENGYYMMDD			0		 XXXX-XX-XX ������ 
#define OSD_TYPE_ENGMMDDYY			1		 XX-XX-XXXX ������ 
#define OSD_TYPE_CHNYYMMDD			2		 XXXX��XX��XX�� 
#define OSD_TYPE_CHNMMDDYY			3		 XX��XX��XXXX�� 
#define OSD_TYPE_ENGDDMMYY			4		 XX-XX-XXXX ������ 
#define OSD_TYPE_CHNDDMMYY			5		 XX��XX��XXXX�� 

@osd_attr:
0 ����ʾOSD
1 ͸������˸
2 ͸����������˸
3 ��͸������˸
4 ��͸����������˸
*/
int EnableOSD(int channel, int enable);
int SetDSPDateTime(int channel, struct SYSTEMTIME* now);

/*
 * ��Ļ�ڵ�
 * ���εĿ���Ϊ16����,�߶�Ϊ8����
*/
int SetupMask(int channel, struct RECT* rc, int count);
int StopMask(int channel);

//�ƶ�����
int SetMotionDetection(int channel, int grade, int fast_fps, int low_fps, struct RECT* rc, int count);
int EnalbeMotionDetection(int channel, int enable);
int GetMotionDetectionMask(int channel, unsigned int* mask);

/**
@int channel ͨ���1-n
@int quality ͼ����� 0 ���� ��1�Ϻã�2һ��
@int mode    ͼ��ģʽ 0 cif��1 qcif��2 d1
@unsigned char* image
@unsigned int* size
*/
int GetJPEGImage(int channel, int quality,int mode, unsigned char* image,unsigned int* size);
//int GetOriginalImage(int channel, unsigned char* image, unsigned int size);

int SetVideoParam(int channel, int brightness,int contrast, int saturation, int hue);
int GetVideoParam(int channel, enum video_standard* standard, int* brightness, int*contrast, int*saturation,int* hue);

int SetStreamType(int channel,int chan_type, int type);
int GetStreamType(int channel,int chan_type,int*type);


/*
*@ ���ϵ��12��30
*/
int SetDefaultQuant(int channle, int chan_type,int quality);

//���ñ���֡�ṹ,֡��
int SetIBPMode(int channel,  int chan_type,int key_frame_inter, int b_frames, int p_frames,int frame_rate);

//�������������������ʺ���������ģʽ
int SetBitrateControl(int channel,int chan_type, unsigned int max_bps,int brc );


//���ñ����ķֱ��ʸ�ʽ
int SetEncoderPictureFormat(int channel, int chan_type,enum picture_format format);

//�𶯱�����
int StartCodec(int channel, int chan_type);
int StopCodec(int channel, int chan_type);

//ǿ��I֡
int CaptureIFrame(int channle, int chan_type);

//��ȡͳ����Ϣ
int GetFramesStatistics(int channel,int chan_types,struct frame_statistics* statistics);

/*
@signal 1 have signal;0 no siganl 
*/
int GetVideoSignal(int channel, int * signal);

int SetMaskAlarm(int channel, int enable, struct RECT* rc);

/*
@mask 1 mask ;0 no mask 
*/
int GetVideoMask(int channel, int* mask);

typedef void (*STREAM_DATA_CALLBACK)(CALLBACK_DATA CallBackData,void* context);
int RegisterStreamDataCallback(STREAM_DATA_CALLBACK func,void* context);

//�����Խ�
int OpenVoiceChannel();
int CloseVoiceChannel();
int ReadVoiceData (void* buf, int size);
int WriteVoiceData(void* buf, int size);


//������
void Beep(unsigned int mode);

int IsDVS_A(void);
int GetDeviceType(unsigned int * device_type);

#define MAIN_CHANNEL 1 
#define SUB_CHANNEL  0 


int SetOutputFormat(int format);

int SetDisplayParams(int index, int screen);

int ClrDisplayParams(int index,int channel);

int SetDisplayOrder(int index, unsigned char* order);

int SetPreviewScreen(int index, int channel, int mode);

int SetPreviewAudio(int index, int channel, int mode);

int SetDecodeScreen(int index, int channel, int mode);

int SetDecodeAudio(int index, int channel, int mode);

int StartDecode(int index, int channel, int mode, void * file_header);

int StopDecode(int handle);

int SetDecodeSpeed(int handle, int speed);

int SetDecodeVolume(int handle, int volume);

int DecodeNextFrame(int handle);

int InputAvData(int handle, void* buf, int size);

int SetMenuDispParam(struct SDK_MENU_DISP_PARAM * pMenuParam);

int	RefreshMenuDisp(struct SDK_MENU_REFRESH_PARAM * pRefreshParam);

int	GetMenuBufParam(struct SDK_MENU_BUF_PARAM * pMenuBufParam);

int DisplayMouseEnable(int VoIndex,int bEnable);

int InitMouse(unsigned char * pMouseData);

int SetMousePos(struct SDK_MOUSE_POS MousePos);


#ifdef __cplusplus
}
#endif

#endif

