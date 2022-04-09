/*
 * =====================================================================================
 *
 *       Filename:  public.c
 *
 *    Description:  定义一些公有的变量和函数，主要供drcom认证使用（修改拷贝自fsn_server）
 *
 *        Version:  0.18
 *        Created:
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:
 *        Company:
 *
 * =====================================================================================
 */

#include "public.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char EAP_TYPE_ID_SALT[9]  = {0x00, 0x44, 0x61, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff};
char EAP_TYPE_MD5_SALT[9] = {0x00, 0x44, 0x61, 0x2a, 0x00, 0xff, 0xff, 0xff, 0xff};

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  print_mac
 *  Description:  以一定格式打印MAC地址
 *        Input:  *src: 待打印的字符串的指针
 *       Output:  无
 * =====================================================================================
 */
void print_mac(char *src)
{
    char mac[32] = "";
    sprintf(mac, "%02x%02x%02x%02x%02x%02x",
                        (unsigned char)src[0],
                        (unsigned char)src[1],
                        (unsigned char)src[2],
                        (unsigned char)src[3],
                        (unsigned char)src[4],
                        (unsigned char)src[5]);

    printf("%s\n", mac);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  print_hex_drcom
 *  Description:  打印十六进制字节流
 *        Input:  *hex: 待打印的内容的指针; len: 打印长度
 *       Output:  无
 * =====================================================================================
 */
#if DRCOM_DEBUG_ON > 0
void print_hex_drcom(char *hex, int len)
{
    printf("print_hex_drcom\n");
    for (int i = 0; i < len; ++i) {
        if (i&&i%16==0){ printf("\n");}
        printf("%.2x ",*((uint8*)hex + i));
    }
    printf("\n\n");

}
#endif

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  checkCPULittleEndian
 *  Description:  检测CPU是否是小端字节序
 *        Input:  无
 *       Output:  是则返回1
 * =====================================================================================
 */
inline int checkCPULittleEndian()
{
    union
    {
        unsigned int a;
        unsigned char b;
    } c;
    c.a = 1;
    return (c.b == 1);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  big2little_32
 *  Description:  大端字节序 转 小端字节序
 *        Input:  A: 待转换的内容
 *       Output:  转换后的内容
 * =====================================================================================
 */
inline uint32_t big2little_32(uint32_t A)
{
    return ((((uint32_t)(A) & 0xff000000) >>24) |
        (((uint32_t)(A) & 0x00ff0000) >> 8) |
        (((uint32_t)(A) & 0x0000ff00) << 8) |
        (((uint32_t)(A) & 0x000000ff) << 24));
}


