#ifndef __DRI_BKP_H__
#define __DRI_BKP_H__

#include "Com_Util.h"

/**
 * @brief 初始化 BKP 备份寄存器
 *
 * @note 写入备份寄存器的数据,在有备用电源的情况下,数据掉电不丢失
 */
void Dri_BKP_Init(void);

#endif /* __DRI_BKP_H__ */

// BKP（backup register，备份寄存器）:
// 是42个16位的寄存器，可用来存储84个字节的用户应用程序数据。
// 它们处在备份域里，当VDD电源被切断时，可以由VBAT继续供电，做到“掉电不丢失”。
// 备份寄存器的内容在系统复位时会被清除，但是在掉电情况下，备份寄存器的内容不会丢失。
