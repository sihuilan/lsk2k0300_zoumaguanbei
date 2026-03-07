#ifndef CODE_INC_SEEKFREE_HELP_H_
#define CODE_INC_SEEKFREE_HELP_H_

// 另外一端的IP地址
#define SERVER_IP "192.168.33.220"
// 端口号
#define PORT 8086

//0：不包含边界信息  
//1：包含三条边线信息，边线信息只包含横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点
//2：包含三条边线信息，边界信息只含有纵轴坐标，横轴坐标由图像宽度得到，意味着每个边界在一列中只会有一个点，一般来说很少有这样的使用需求
//3：包含三条边线信息，边界信息含有横纵轴坐标，意味着你可以指定每个点的横纵坐标，边线的数量也可以大于或者小于图像的高度，通常来说边线数量大于图像的高度，一般是搜线算法能找出回弯的情况
//4：没有图像信息，仅包含三条边线信息，边线信息只包含横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点，这样的方式可以极大的降低传输的数据量
#define INCLUDE_BOUNDARY_TYPE   1

// 边界的点数量远大于图像高度，便于保存回弯的情况
#define BOUNDARY_NUM            (CROP_H * 4 / 2)

void wave_oscilloscope(void);
void transfer_image(void);

#endif