#ifndef   __PRODUCTMODEL_H__

#define __PRODUCTMODEL_H__

#define PM_TSTAT5B                    1
#define PM_TSTAT5A                    2
#define PM_TSTAT5B2                   3
#define PM_TSTAT5C                    4
#define PM_TSTAT6                     6
#define PM_TSTAT7                     7
#define PM_TSTAT5i                    8
#define PM_TSTAT8                     9
#define PM_TSTAT10                    10    //��Wifi ��TSTAT ӵ��T3 program �Ĺ��� - TSTAT with Wifi has the function of T3 program
#define PM_TSTAT5D                    12
#define PM_AirQuality                 13
#define PM_HUMTEMPSENSOR              14
#define PM_TSTATRUNAR                 15
#define PM_TSTAT5E                    16
#define PM_TSTAT5F                    17
#define PM_TSTAT5G                    18
#define PM_TSTAT5H                    19
#define PM_T38I13O                    20
#define PM_T3IOA                      21
#define PM_T332AI                     22
#define PM_T38AI16O                   23
#define PM_ZIGBEE                     24
#define PM_FLEXDRIVER                 25
#define PM_T3PT10                     26
#define PM_T3PERFORMANCE              27
#define PM_T34AO                      28
#define PM_T36CT                      29
#define PM_SOLAR                      30
#define PM_FWMTRANSDUCER              31
#define PM_CO2_NET                    32
#define PM_CO2_RS485                  33
#define PM_CO2_NODE                   34
#define PM_MINIPANEL                  35
#define PM_CS_SM_AC                   36
#define PM_CS_SM_DC                   37
#define PM_CS_RSM_AC                  38
#define PM_CS_RSM_DC                  39
#define PM_PRESSURE                   40
#define PM_PM5E                       41
#define PM_HUM_R                      42
#define PM_T322AI                     43
#define PM_T38AI8AO6DO                44
#define PM_PRESSURE_SENSOR            45
#define PM_T3PT12                     46
#define PM_T322AIVG                   47
#define PM_T38IOVG                    48
#define PM_T3PTVG                     49
#define PM_CM5                        50
#define PM_PM5E_ARM                   51   //2019 03 18 ���Ͽ�5964оƬ��ֲ���� - Ported from the old 5964 chip
#define STM32_PM25                    52
#define PM_T332AI_ARM                 53   //20240115 STM32   32 AI
#define PM_TSTAT9                     59   //2019 06 20 
#define PM_MULTI_SENSOR               60   //��ͬ�� 62 ���ֳ�ʽ�� �๦�ܴ�����; - Ported from the old 5964 chip
#define PM_TSTAT_AQ                   62   //TSTAT AQ û�и��ֿ��� ��������İ汾; - Ported from the old 5964 chip
#define PM_ZIGBEE_REPEATER            63   //zigbee���м��� - Ported from the old 5964 chip
#define PM_TSTAT6_HUM_Chamber         64
#define PM_AIRLAB_ESP32               65   //��Airlab ����һģһ��  ��wifi��оƬ - Ported from the old Airlab chip
#define PM_BEENY                      70
#define PM_WATER_SENSOR               71
#define PM_T3_LC                      72
#define PM_PWMETER			          73
#define PM_MINIPANEL_ARM              74
#define PM_WEATHER_STATION            75
#define PM_ESP32_T38AI8AO6DO          87
//��Ʒ���� 88   �������µ� ESP32�汾�� T3 BB LB TB Nano - Product number is 88, used for the latest ESP32 version of T3 BB LB TB Nano
#define PM_ESP32_T3_SERIES            88
#define PM_ESP32_T322AI               89
#define PWM_TEMPERATURE_TRANSDUCER    90  //Lijun transudcer �°���; - Lijun transudcer new board
#define PM_TSTAT8_WIFI                91
#define PM_TSTAT8_OCC                 92
#define PM_TSTAT7_ARM                 93
#define PM_TSTAT8_220V                94
#define PM_T36CTA                     95
#define PM_AFS                        96  //Air Floor Sensor
#define PM_FAN_MODULE                 97  //Lijun  ESP32 new board fan  ��ʱû���� - Lijun ESP32 new board fan, no interface for now
#define PM_NC                         100
#define PM_TSTAT8_PROGRAM             101  //TSTAT8 �Ŀɱ�̰汾; - Programmable version of TSTAT8
#define PWM_TRANSDUCER                104
#define PM_LightingController         120
#define PM_BTU_METER                  121
#define PM_BOATMONITOR                122
#define PM_MINI_TOP                   200
#define PM_LABBATS                    201
#define PM_TESTER_JIG                 202
#define STM32_CO2_NET                 210      //CO2NET
#define STM32_CO2_RS485               211      //CO2RS485
#define STM32_HUM_NET                 212      //HUMNET
#define STM32_HUM_RS485               213      //HUMRS485
#define STM32_PRESSURE_NET            214      //PSNET
#define STM32_PRESSURE_RS485         215      //PSRS485
#define STM32_CO2_NODE                216      //CO2NODE


#define PM_THIRD_PARTY_DEVICE         254      //�������豸 - Third-party device
#endif
