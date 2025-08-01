#ifndef __APP_IPCAM_SENSORS_H__
#define __APP_IPCAM_SENSORS_H__

#ifdef __cplusplus
extern "C" {
#endif

// Enabled sensors
#define SNS0_OV_OV5647
#define SNS0_SMS_SC530AI_2L



extern ISP_SNS_OBJ_S stSnsOv5647_Obj;
extern ISP_SNS_OBJ_S stSnsSC530AI_2L_Obj;
extern ISP_SNS_OBJ_S stSnsSC531AI_2L_Obj;


typedef enum _SENSOR_TYPE_E {
    SENSOR_NONE,
    SENSOR_GCORE_GC1054,
    SENSOR_GCORE_GC2053,
    SENSOR_GCORE_GC2053_1L,
    SENSOR_GCORE_GC2053_SLAVE,
    SENSOR_GCORE_GC2093,
    SENSOR_GCORE_GC2093_SLAVE,
    SENSOR_GCORE_GC4653,
    SENSOR_GCORE_GC4023,
    SENSOR_GCORE_GC4653_SLAVE,
    SENSOR_NEXTCHIP_N5,
    SENSOR_NEXTCHIP_N6,
    SENSOR_OV_OS08A20,
    SENSOR_OV_OS08A20_SLAVE,
    SENSOR_OV_OV5647,
    SENSOR_PICO_384,
    SENSOR_PICO_640,
    SENSOR_PIXELPLUS_PR2020,
    SENSOR_PIXELPLUS_PR2100,
    SENSOR_SMS_SC1346_1L,
    SENSOR_SMS_SC1346_1L_60,
    SENSOR_SMS_SC200AI,
    SENSOR_SMS_SC2331_1L,
    SENSOR_SMS_SC2335,
    SENSOR_SMS_SC2336,
    SENSOR_SMS_SC2336P,
    SENSOR_SMS_SC3335,
    SENSOR_SMS_SC3335_SLAVE,
    SENSOR_SMS_SC3336,
    SENSOR_SMS_SC401AI,
    SENSOR_SMS_SC4210,
    SENSOR_SMS_SC8238,
    SENSOR_SMS_SC530AI_2L,
    SENSOR_SMS_SC531AI_2L,
    SENSOR_SMS_SC5336_2L,
    SENSOR_SMS_SC4336P,
    SENSOR_SOI_F23,
    SENSOR_SOI_F35,
    SENSOR_SOI_F35_SLAVE,
    SENSOR_SOI_H65,
    SENSOR_SOI_K06,
    SENSOR_SOI_Q03P,
    SENSOR_SONY_IMX290_2L,
    SENSOR_SONY_IMX307,
    SENSOR_SONY_IMX307_2L,
    SENSOR_SONY_IMX307_SLAVE,
    SENSOR_SONY_IMX307_SUBLVDS,
    SENSOR_SONY_IMX327,
    SENSOR_SONY_IMX327_2L,
    SENSOR_SONY_IMX327_SLAVE,
    SENSOR_SONY_IMX327_SUBLVDS,
    SENSOR_SONY_IMX334,
    SENSOR_SONY_IMX335,
    SENSOR_SONY_IMX347,
    SENSOR_SONY_IMX385,
    SENSOR_VIVO_MCS369,
    SENSOR_VIVO_MCS369Q,
    SENSOR_VIVO_MM308M2,
    SENSOR_IMGDS_MIS2008,
    SENSOR_IMGDS_MIS2008_1L,
    SENSOR_BUTT
} SENSOR_TYPE_E;

ISP_SNS_OBJ_S* app_ipcam_SnsObj_Get(SENSOR_TYPE_E enSnsType);
CVI_S32 app_ipcam_Vi_DevAttr_Get(SENSOR_TYPE_E enSnsType, VI_DEV_ATTR_S* pstViDevAttr);
CVI_S32 app_ipcam_Vi_PipeAttr_Get(SENSOR_TYPE_E enSnsType, VI_PIPE_ATTR_S* pstViPipeAttr);
CVI_S32 app_ipcam_Vi_ChnAttr_Get(SENSOR_TYPE_E enSnsType, VI_CHN_ATTR_S* pstViChnAttr);
CVI_S32 app_ipcam_Isp_InitAttr_Get(SENSOR_TYPE_E enSnsType, WDR_MODE_E enWDRMode, ISP_INIT_ATTR_S* pstIspInitAttr);
CVI_S32 app_ipcam_Isp_PubAttr_Get(SENSOR_TYPE_E enSnsType, ISP_PUB_ATTR_S* pstIspPubAttr);

#ifdef __cplusplus
}
#endif

#endif
