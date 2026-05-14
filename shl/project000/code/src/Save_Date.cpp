#include "zf_common_headfile.h"
#include <vector>
//#include "Save_Date.h"

DataInfomationType DataInfo;


//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//  @brief      需要调参部分
//  @explain    1、增加需要存进flash的数据只需在其对应类型的数组里添加其地址
//              (若为顺序添加，则不会影响在其之前的数据值，若为插入添加，会影响之前数据，需先抄录之前数据）
//              如果不在这里声明要修改的变量，则flash里的数据修改后无法保存，重启或者复位后还是原来的数据

std::vector<void*> Data_uint8 ={
    //第二页
    &DataInfo.forwardLook,


    //出库速度
    &DataInfo.goSpeed,

    // &DataInfo.ad_straightSpeed, &DataInfo.ad_curSpeed, &DataInfo.ad_traditionSpeed,

    // //坡道速度
    // &DataInfo.rampSpeed,
    // //圆环尺寸
    // &Circle.Circle_size1, &Circle.Circle_size2, &Circle.Circle_size3, &Circle.Circle_size4,

    //第六页
    &DataInfo.door,  &DataInfo.OBST, &DataInfo.GOtime,&DataInfo.image_curSpeed1,

    //避障新旧版函数切换
    //&RoadFlag.Obs_N_O_cha,
};
std::vector<void*> Data_int8    = {nullptr};
std::vector<void*> Data_uint16  = {};
std::vector<void*> Data_int16   = {nullptr};
std::vector<void*> Data_uint32  = {nullptr};
std::vector<void*> Data_int32   = {nullptr};
std::vector<void*> Data_float   ={
    //一级PD
   
//    &DataInfo.image_dirTLP,  &DataInfo.image_dirTD,
//    &DataInfo.ad_dirEP,  &DataInfo.ad_dirED,
    //二级PD
    &DataInfo.image_dirTLP, &DataInfo.image_dirTRP, 
//    &DataInfo.ad_dirTP,    &DataInfo.ad_dirTD,

    //十字补线参数
    &DataInfo.crossline,
    //坡道参数
    &DataInfo.rampP, &DataInfo.rampline,
    //避障
    &DataInfo.obstacleP,
    &DataInfo.obsdis1L,
    &DataInfo.obsdis2L,
    &DataInfo.obsdis3L,
    &DataInfo.obsdis1R,
    &DataInfo.obsdis2R,
    &DataInfo.obsdis3R,

    //避障速度
    &DataInfo.obstacleSpeed,
    //急弯
    &DataInfo.CurveP,

    //斑马线速度
    &DataInfo.zebraSpeed,

    //斑马线1后的定距
//    &DataInfo.zebragoL,&DataInfo.zebragoR,

    //圆环速度
    &Circle.Speed_R50, &Circle.Speed_R60, &Circle.Speed_R80,
    //圆环参数
    &Circle.Circle_R50In, &Circle.Circle_R50Out,
    &Circle.Circle_R60In, &Circle.Circle_R60Out,
    &Circle.Circle_R80In, &Circle.Circle_R80Out,
    &Circle.Circle_R100In, &Circle.Circle_R100Out,

    &Circle.CircleR50_LP, &Circle.CircleR50_LD,
    &Circle.CircleR50_RP, &Circle.CircleR50_RD,
    &Circle.CircleR60_LP, &Circle.CircleR60_RP,
    &Circle.CircleR60_D,
    &Circle.CircleR80_EP, &Circle.CircleR80_ED,//R80和R100用同一套PD
    &Circle.CircleR80_P, &Circle.CircleR80_D,

    &DataInfo.image_straightSpeed, &DataInfo.image_curSpeed,
    //速度环，转向环pid
    &CarInfo.S_LP , &CarInfo.S_LI ,&CarInfo.S_LD,
    &CarInfo.S_RP , &CarInfo.S_RI ,&CarInfo.S_RD,

    &DataInfo.image_dirEP1,  &DataInfo.image_dirED1,    //摄像头一级
    &DataInfo.image_dirTP2,  &DataInfo.image_dirTD2,    //摄像头二级

    &DataInfo.image_traditionSpeed,         //基础赛道的速度
//    &Circle.CircleR100_EP, &Circle.CircleR100_ED,//R80和R100用同一套PD
//    &Circle.CircleR100_P, &Circle.CircleR100_D,
    //第八页
    &DataInfo.image_bigP,   &DataInfo.image_bigD,


};


//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取Flash数据函数
//  @explain    1、此函数用于备份Flash数据
//              2、刷Flash需将Read_All_Data()前的‘1’改为‘0’，再手动确认保存数据
//-------------------------------------------------------------------------------------------------------------------

void FlashData_Init(void)
{


    /** 第二页 **/
    DataInfo.forwardLook = 18;//21  19

    /** 第四页 **/
    // DataInfo.image_dirEP = 0.70f;//0.7
    // DataInfo.image_dirED = 0.0f;//0
    // DataInfo.image_dirTLP = 6.20f;//2.0 3 6 4.75 3.6
    // DataInfo.image_dirTRP = 6.6f;//2.0 3 5 3.5 3.3
    // DataInfo.image_dirTD = 0.1f;

  
    DataInfo.image_dirTLP = 2.20f;//2.0 3 6 4.75 3.6
    DataInfo.image_dirTRP = 2.6f;//2.0 3 5 3.5 3.3

    DataInfo.zebraDis = 1.0f;
    DataInfo.zebraSpeed=60.0f;

    DataInfo.rampP = 3.00f;//3
    DataInfo.rampSpeed = 170;//55 60 //80
    DataInfo.rampline = 1.95;

    DataInfo.crossline = 32;//22 30

    DataInfo.goSpeed = 50;//50 70

    DataInfo.zebraSpeed = 50.0;
//    DataInfo.zebragoL = 0.20f;
//    DataInfo.zebragoR = 0.20f;

    DataInfo.image_straightSpeed = 50;//190//95
    
    DataInfo.image_curSpeed = 50;//175 180//87

    DataInfo.traP=0.05f;
    DataInfo.CirPR50=0.0f;
    DataInfo.CurP=0.01f;
    DataInfo.CirPR60=0.0f;

    DataInfo.obstacleSpeed = 60.0;//75 80 100 150 155 180
    DataInfo.obstacleP = 6.80f; //4 5 4 7
    DataInfo.obsdis1L = 0.50f;
    DataInfo.obsdis2L = 0.80f;
    DataInfo.obsdis3L = 1.05f;
    DataInfo.obsdis1R = 0.50f;
    DataInfo.obsdis2R = 0.73f;
    DataInfo.obsdis3R = 1.00f;

    DataInfo.CurveP = 0.70f;

    DataInfo.door = 0;

    DataInfo.OBST = 1;  //避障方向 1:左    2:右
    DataInfo.GOtime = 1;

    CarInfo.S_LP = 4;
    CarInfo.S_LI = 0.15;
    CarInfo.S_LD = 0;
    
    CarInfo.S_RP = 4;
    CarInfo.S_RI = 0.15;
    CarInfo.S_RD = 0;

    DataInfo.image_traditionSpeed = 100; //150 185 95  //70//95

    DataInfo.image_dirEP1 = 0.70f;//0.7                 //摄像头一级PD
    DataInfo.image_dirED1 = 0.2f;//0
   
    DataInfo.image_dirTP2 = 0.1f;                        //摄像头二级PD
    DataInfo.image_dirTD2 = 0.1f;

    DataInfo.image_bigP = 0.8f;                         //偏差大时的PD
    DataInfo.image_bigD = 0.2f; 

    ImageCircle_init();
//    Circle_Data();
    //第一次使用时，将以下注释去掉，以此来建立相应的数据文件，完成后注释掉，重烧一次代码即可
    // Save_All_Data();

    Read_All_Data();

}
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

//------------------------------- ------------------------------------------------------------------------------------
//  @brief      对单个扇区写数据函数
//  @param      page_num      扇区号
//-------------------------------------------------------------------------------------------------------------------
static void Write_Data(uint8 page_num){

    uint8 j;
    char buffer[1024] = {0}; // 临时存储数组，200 字节
    char* ptr = buffer;
    switch(page_num){

        case 0: {
            // 构建完整的输出字符串
            for (j = 0; j < Data_uint8.size(); ++j) {
                uint8* data_ptr = static_cast<uint8*>(Data_uint8[j]);           
                if (data_ptr == nullptr) {
                    continue;
                }
                // 追加数据到 buffer
                ptr += snprintf(ptr, sizeof(buffer) - (ptr - buffer), "%u\n", *data_ptr);
                // 检查 buffer 是否溢出
                if (static_cast<long unsigned int>(ptr - buffer) >= sizeof(buffer)) {
                    break;
                }
            }
            // 一次性写入文件
            file_write_string(FILENAME_UINT_8, buffer);
            //清空buffer
            memset(buffer, 0, sizeof(buffer));
            break;
        }
        case 1: {
            // 构建完整的输出字符串
            for (j = 0; j < Data_int8.size(); ++j) {
                int8* data_ptr = static_cast<int8*>(Data_int8[j]);           
                if (data_ptr == nullptr) {
                    continue;
                }
                // 追加数据到 buffer
                ptr += snprintf(ptr, sizeof(buffer) - (ptr - buffer), "%d\n", *data_ptr);
                // 检查 buffer 是否溢出
                if (static_cast<long unsigned int>(ptr - buffer) >= sizeof(buffer)) {
                    break;
                }
            }
            // 一次性写入文件
            file_write_string(FILENAME_INT8, buffer);
            //清空buffer
            memset(buffer, 0, sizeof(buffer));
            break;
        }
        case 2: {
            // 构建完整的输出字符串
            for (j = 0; j < Data_uint16.size(); ++j) {
                uint16* data_ptr = static_cast<uint16*>(Data_uint16[j]);           
                if (data_ptr == nullptr) {
                    continue;
                }
                // 追加数据到 buffer
                ptr += snprintf(ptr, sizeof(buffer) - (ptr - buffer), "%u\n", *data_ptr);
                // 检查 buffer 是否溢出
                if (static_cast<long unsigned int>(ptr - buffer) >= sizeof(buffer)) {
                    break;
                }
            }
            // 一次性写入文件
            file_write_string(FILENAME_UINT16, buffer);
            //清空buffer
            memset(buffer, 0, sizeof(buffer));
            break;
        }
        case 3: {
            // 构建完整的输出字符串
            for (j = 0; j < Data_int16.size(); ++j) {
                int16* data_ptr = static_cast<int16*>(Data_int16[j]);           
                if (data_ptr == nullptr) {
                    continue;
                }
                // 追加数据到 buffer
                ptr += snprintf(ptr, sizeof(buffer) - (ptr - buffer), "%d\n", *data_ptr);
                // 检查 buffer 是否溢出
                if (static_cast<long unsigned int>(ptr - buffer) >= sizeof(buffer)) {
                    break;
                }
            }
            // 一次性写入文件
            file_write_string(FILENAME_INT16, buffer);
            //清空buffer
            memset(buffer, 0, sizeof(buffer));
            break;
        }
        case 4: {
            // 构建完整的输出字符串
            for (j = 0; j < Data_uint32.size(); ++j) {
                uint32* data_ptr = static_cast<uint32*>(Data_uint32[j]);           
                if (data_ptr == nullptr) {
                    continue;
                }
                // 追加数据到 buffer
                ptr += snprintf(ptr, sizeof(buffer) - (ptr - buffer), "%u\n", *data_ptr);
                // 检查 buffer 是否溢出
                if (static_cast<long unsigned int>(ptr - buffer) >= sizeof(buffer)) {
                    break;
                }
            }
            // 一次性写入文件
            file_write_string(FILENAME_UINT32, buffer);
            //清空buffer
            memset(buffer, 0, sizeof(buffer));
            break;
        }
        case 5: {
            // 构建完整的输出字符串
            for (j = 0; j < Data_int32.size(); ++j) {
                int32* data_ptr = static_cast<int32*>(Data_int32[j]);           
                if (data_ptr == nullptr) {
                    continue;
                }
                // 追加数据到 buffer
                ptr += snprintf(ptr, sizeof(buffer) - (ptr - buffer), "%d\n", *data_ptr);
                // 检查 buffer 是否溢出
                if (static_cast<long unsigned int>(ptr - buffer) >= sizeof(buffer)) {
                    break;
                }
            }
            // 一次性写入文件
            file_write_string(FILENAME_INT32, buffer);
            //清空buffer
            memset(buffer, 0, sizeof(buffer));
            break;
        }
        case 6: {
            // 构建完整的输出字符串
            for (j = 0; j < Data_float.size(); ++j) {
                float* data_ptr = static_cast<float*>(Data_float[j]);           
                if (data_ptr == nullptr) {
                    continue;
                }
                // 追加数据到 buffer
                ptr += snprintf(ptr, sizeof(buffer) - (ptr - buffer), "%f\n", *data_ptr);
                // 检查 buffer 是否溢出
                if (static_cast<long unsigned int>(ptr - buffer) >= sizeof(buffer)) {
                    break;
                }
            }
            // 一次性写入文件
            file_write_string(FILENAME_FLOAT, buffer);
            //清空buffer
            memset(buffer, 0, sizeof(buffer));
            break;
        }
        default:    break;
    }

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      对单个扇区读数据函数
//  @param      page_num      扇区号
//-------------------------------------------------------------------------------------------------------------------
static void Read_Data(uint8 page_num)
{
    
    char buffer[1024] = {0}; //  临时存储数组，200 字节
    uint8 i;
   
        switch(page_num)
        {
            case 0: { 
                for (i =0; i < Data_uint8.size() ; ++i){                                    //根据数组长度迭代读取数值                  
                        uint8* data_ptr = static_cast<uint8*>(Data_uint8[i]);               
                                if (data_ptr == nullptr) {                                  //判断该页内容是否为空，为空则跳出循环
                                    continue;
                                }       
                                file_read_string(FILENAME_UINT_8, buffer, i);                                                                         
                                *data_ptr=atoi(buffer);
                                memset(buffer, 0, sizeof(buffer));
                    }
                break;
            }
            case 1: { 
                for (i =0; i < Data_int8.size() ; ++i){                                     //根据数组长度迭代读取数值              
                        int8* data_ptr = static_cast<int8*>(Data_int8[i]);                     
                                if (data_ptr == nullptr) {                                  //判断该页内容是否为空，为空则跳出循环
                                    continue;
                                }       
                                file_read_string(FILENAME_INT8, buffer, i);                                                                         
                                *data_ptr=atoi(buffer);
                                memset(buffer, 0, sizeof(buffer));
                    }
                break;
            }
            case 2: { 
                for (i =0; i < Data_uint16.size() ; ++i){                                     //根据数组长度迭代读取数值
                        uint16* data_ptr = static_cast<uint16*>(Data_uint16[i]);              
                                if (data_ptr == nullptr) {                                  //判断该页内容是否为空，为空则跳出循环
                                    continue;
                                }       
                                file_read_string(FILENAME_UINT16, buffer, i);                                                                         
                                *data_ptr=atoi(buffer);
                                memset(buffer, 0, sizeof(buffer));
                    }
                break;
            }
            case 3: { 
                for (i =0; i < Data_int16.size() ; ++i){                                     //根据数组长度迭代读取数值
                        int16* data_ptr = static_cast<int16*>(Data_int16[i]);              
                                if (data_ptr == nullptr) {                                  //判断该页内容是否为空，为空则跳出循环
                                    continue;
                                }       
                                file_read_string(FILENAME_INT16, buffer, i);                                                                         
                                *data_ptr=atoi(buffer);
                                memset(buffer, 0, sizeof(buffer));
                    }
                break;
            }
            case 4: { 
                for (i =0; i < Data_uint32.size() ; ++i){                                     //根据数组长度迭代读取数值
                        uint32* data_ptr = static_cast<uint32*>(Data_uint32[i]);              
                                if (data_ptr == nullptr) {                                  //判断该页内容是否为空，为空则跳出循环
                                    continue;
                                }       
                                file_read_string(FILENAME_UINT32, buffer, i);                                                                         
                                *data_ptr=strtoul(buffer, nullptr, 10);;
                                memset(buffer, 0, sizeof(buffer));
                    }
                break;
            }
            case 5: { 
                for (i =0; i < Data_int32.size() ; ++i){                                     //根据数组长度迭代读取数值
                        int32* data_ptr = static_cast<int32*>(Data_int32[i]);              
                                if (data_ptr == nullptr) {                                  //判断该页内容是否为空，为空则跳出循环
                                    continue;
                                }       
                                file_read_string(FILENAME_INT32, buffer, i);                                                                         
                                *data_ptr=strtol(buffer, nullptr, 10);;
                                memset(buffer, 0, sizeof(buffer));
                    }
                break;
            }  
            case 6: { 
                for (i =0; i < Data_float.size() ; ++i){                                     //根据数组长度迭代读取数值
                        float* data_ptr = static_cast<float*>(Data_float[i]);              
                                if (data_ptr == nullptr) {                                  //判断该页内容是否为空，为空则跳出循环
                                    continue;
                                }       
                                file_read_string(FILENAME_FLOAT, buffer, i);                                                                         
                                *data_ptr=atof(buffer);
                                memset(buffer, 0, sizeof(buffer));
                    }
                break;
            }    
        }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      写入全部数据函数
//-------------------------------------------------------------------------------------------------------------------
void Save_All_Data(void)
{
    uint8 i;

    for(i = 0; i < Max_Sector; i++)
    {
        Write_Data(i);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取全部数据函数
//-------------------------------------------------------------------------------------------------------------------
void Read_All_Data(void)
{
    uint8 i;

    for(i = 0; i < Max_Sector; i++)    //Max_Sector最大页数
    {
        Read_Data(i);
    }
}
