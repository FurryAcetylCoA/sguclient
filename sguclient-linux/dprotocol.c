/*
 * =====================================================================================
 *
 *       Filename:  dprotocol.c
 *
 *    Description:  主要含drcom认证的代码（修改拷贝自fsn_server）
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

#include "dprotocol.h"

//默认使用简明日志
static char *DMSG_SendU8 = "Drcom: U8↑--";
static char *DMSG_SendU8_Fail = " U38↑✖ ";
static char *DMSG_GotU8R = "↓✓ ";
static char *DMSG_SendU244 = " U244↑--";
static char *DMSG_SendU244_Fail = "✖ !!\n";
static char *DMSG_SendU38 = " U38↑";
static char *DMSG_SendU38_Fail = " =✖ !!\n";
static char *DMSG_SentU38 = "✓--";
static char *DMSG_LoginU244 = "↓✓﹣﹣ ";
static char *DMSG_SendU40_1_Fail = "U40-1↑✖ !!\n ";
static char *DMSG_GotU40_2 = "--2↓";
static char *DMSG_FinishU40 = "--4↓U40✓";
static char *DMSG_StartInterval = "   Wait 8s...";
static char *DMSG_DoneInterval = "✓ \n";
static char *DMSG_GotU38 = "↓ ♡↺✓ ";
static char *DMSG_SendU40_1 = "U40-1↑";
static char *DMSG_SendU40_3 = "--3↑";

dr_info DrInfo;
uint8 revData[RECV_BUF_LEN];
uint8 drcom_pkt_counter;
int dstatus;
int xstatus;  //802.1x状态

char dstatusMsg[256];

static int sock;


int SendU8GetChallenge();

int SendU244Login();

int SendU38HeartBeat();

int SendU40DllUpdater(uint8 type);

void U8ResponseParser();

void U244ResponseParser();

void U40ResponseParser();

void FillCheckSum(uint8 *ChallengeFromU8, uint16 Length, uint8 *CheckSum);

uint32 GetU40_3Sum(uint8 *buf);

void DecodeU244Response(uint8 *buf);

int udp_send_and_rev(uint8 *send_buf, int send_len, uint8 *recv_buf);

static void perrorAndSleep(char *str);

static void printAll(char *str);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  init_logStyle
 *  Description:  初始化debug模式的详细日志
 *  	  Input:  debug_log_style
 *  	 Output:  无
 * =====================================================================================
 */
void init_logStyle() {
    if (debug_log_style) {
        DMSG_SendU8 = "Drcom: Sending login request U8.\n";
        DMSG_SendU8_Fail = "DrCom: Sending login request U8 failed.\n";
        DMSG_GotU8R = "Drcom: Got response for start request U8.\n";
        DMSG_SendU244 = "Drcom: Sending login request U244.\n";
        DMSG_SendU244_Fail = "Drcom: Login request U244 failed.\n";
        DMSG_SendU38 = "Drcom: Sending heart beat U38.\n";
        DMSG_SendU38_Fail = "Drcom: Heart beat U38 failed.\n";
        DMSG_SentU38 = "Drcom: Sent heart beat U38.\n";
        DMSG_LoginU244 = "Drcom: Got U244 login response, U244 login success!\n";
        DMSG_SendU40_1_Fail = "Drcom: U40 phase 1 error.\n";
        DMSG_GotU40_2 = "Drcom: Got U40 response phase 2.\n";
        DMSG_FinishU40 = "Drcom: Got U40 response phase 4, U40 cycle done!\n";
        DMSG_StartInterval = "Drcom: Waiting for 8s before sending next U8.\n";
        DMSG_DoneInterval = "Drcom: 8s Done.\n";
        DMSG_GotU38 = "Drcom: Got U38 response. Keep alive cycle done!\n";
    } else return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  drcom_crc32
 *  Description:  计算drcom协议中的crc校验值 （旧版，已弃用）
 *  	  Input:  *data: 指向数据包内容的指针; data_len: 数据包的长度
 *  	 Output:  返回计算出来的校验值
 * =====================================================================================
 */
uint32_t drcom_crc32(char *data, int data_len) {
    uint32_t ret = 0;
    int i = 0;
    for (i = 0; i < data_len;) {
        ret ^= *(unsigned int *) (data + i);
        ret &= 0xFFFFFFFF;
        i += 4;
    }

    // 大端小端的坑
    if (checkCPULittleEndian() == 0) ret = big2little_32(ret);
    ret = (ret * 19680126) & 0xFFFFFFFF;
    if (checkCPULittleEndian() == 0) ret = big2little_32(ret);

    return ret;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  SendU8GetChallenge
 *  Description:  发起drcom协议的认证(发送长度为8的数据包)
 *  	  Input:  无
 *  	 Output:  成功返回0；失败返回-1
 * =====================================================================================
 */
int SendU8GetChallenge() {
    /*数据包U8，长度固定为8字节，必须在EAP结束后尽快发出
    * +------+----------+----------+-----------------+
    * | 标头  |计数器|长度 |   类型    |   零填充         |
    * +------+-----+----+----------+-----------------+
    * |  07  |  XX | 08 |  00  01  |   00   00   00  |
    * +------+-----+----+----------+-----------------+
    * 计数器:
    *   首次发送时是0,然后就从2开始往后数
    */
    const int pkt_data_len = 8;
    uint8 pkt_data[8] =
            {0x07, 0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00};
    memcpy(&pkt_data[1], &DrInfo.U8Counter, sizeof(uint8));

    int revLen =
            udp_send_and_rev(pkt_data, pkt_data_len, revData);
#if DRCOM_DEBUG_ON > 0
    print_hex_drcom(revData, revLen);
#endif
    /*数据包U8的响应，长度固定为32字节
    * +------+-----+-----+---------+-------+------------+-----------+
    * | 标头  |计数器|长度|   类型   |算法选择|      零     |    时间码  |
    * +------+-----+----+---------+------+-+------------+-----------+
    * |  07  | XX |10  | 00  02 |   8X  |  00 00 00   |XX XX XX XX |
    * +------+-------+--+--------+----+--+-------------+------------+
    * |    客户端IP   |      某种长度   |       零        |    某种版本 |
    * +--------------+----------------+--------------+-+------------+
    * | c0 a8 XX XX  | a8  ac  00  00 |  00 00 00 00 | dc 02 00 00  |
    * +--------------+----------------+--------------+--------------+
    *  计数器:
    *    原样送回
    *  时间码：
    *    小端序，且最靠近包头的一字节的最后两bit会用来决定U244校验值的产生算法（一共有3种
    *    在新版加密中不接受最后两bit均为0的情况），且整个时间码会被用来当质询值
    *  算法选择：
    *    转换成二进制后，高八位固定为 1000。低八位的最后两位据观察总与上面用来决定U244校验算法的选择位一致
    *  长度：
    *    从其他数据包来看，这里应该是保存包长度用的，但U8的响应包这里却是错的 (0x10=0d16!=0d32)
    *  如果没有特别注明，则所有数据段均为网络端序（也就是大端序）
    */
    if (revData[0] != 0x07 || revData[4] != 0x02)    // Start Response
        return -1;
    U8ResponseParser();
#if DRCOM_DEBUG_ON > 0
    print_hex_drcom(DrInfo.ChallengeTimer, 4);
#endif

    return 0;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  SendU244Login
 *  Description:  发起drcom协议的登录（发送包含用户名、主机名等信息的长度为244的数据包）
 *  	  Input:  无
 *  	 Output:  成功返回0
 * =====================================================================================
 */
int SendU244Login() {
    /*U244，长度固定为32字节，
    * +-----+-----+-----+-------+-------+-------------------+
    * |标头 |计数器| 长度 |  类型  |用户名长|     客户端MAC      |
    * +-----+-----+-----+-------+-------+-------------------+
    * |  07 | 01  | f4  | 00 03 |  0b   | XX XX XX XX XX XX |
    * +-----+-----+--+--+-------+-------+-------------------+
    * |    客户端IP   |  定值1,与版本有关  |  U8[8:12] 质询值   |
    * +--------------+------------------+-------------------+
    * | c0 a8 XX XX  |  02  22  00  31  |   XX  XX  XX  XX  |
    * +--------------+----------+-------+-------------------+
    * |        U244校验值        |          用户名            |
    * +-------------------------+---------------------------+
    * | XX XX XX XX XX XX XX XX |  XX XX XX  ...  XX XX XX  |
    * +-------------------------+-------------+-------------+
    * |        计算机名          |  客户端DNS1  | 客户端DHCP   |
    * +-------------------------+-------------+-------------+
    * | XX XX XX  ...  XX XX XX | ca 60 80 a6 | 00 00 00 00 |
    * +-------------+-----------+-------------+-+-----------+--+
    * |  客户端DNS2  | 客户端WINS1  | 客户端WINS2   | 系统版本段长度|
    * +-------------+-------------+-------------+--------------+
    * | 72 72 72 72 | 00 00 00 00 | 00 00 00 00 | 94 00 00 00  |
    * +-------------+-------------+-------------+--------------+
    * | MajorVersion| MinorVersion| BuildNumber |  PlatformId  |
    * +-------------+-------------+-------------+--------------+
    * | 06 00 00 00 | 02 00 00 00 | f0 23 00 00 | 02 00 00 00  |
    * +-------------+--+-------+--+---------+---+--------+-----+
    * |    "DrCOM"     | DrVer | DrCustomId | DrNewVerId |
    * +----------------+-------+------------+------------+
    * | 44 72 43 4f 4d |  00   |    b8 01   |   31 00    |
    * +-------------+--+-------+------+-----+-----+------+
    * |  五十四字节零 |客户端验证模块校验码|   零填充   |
    * +-------------+-----------------+-----------+
    * |  00 ... 00  | c9 14 ... f6 4b | 00 ... 00 |
    * +-------------+------------- ---+-----------+
     * U244需要在U8反回后十分钟内发出,否则会掉线。但测试发现，时间码的有效期只有一分钟
     * 计数器：
     *   固定为1，这里延续了laijingwu学长在文章[laijingwu.com/222.html]中对其的标记
     * 长度：
     *   小端序 0x00f4 = 0d244
     * 用户名长度：
     *   0x0b = 0d11
     * 定值1：
     *   小端序，最高字从26变成了31。其他位功能未知
     * U244校验值：
     *   新版从32位变成了64位，具体算法见FillU244CheckSum
     * 计算机名：
     *   也就是主机名，最长32字节
     * 客户端DNS,DHCP,WINS:
     *   统统可以置空
     * 系统版本段：
     *   结构与WIN-API中OSVERSIONINFO结构体完全一致
     *   除了其中的szCSDVersion成员被换成了DrCom自定义的内容
     *   由于DrCom客户端使用GetVersion的姿势不对，从Win8.1后获取到的永远是6.2.9200,所以本段可视为定值
     * "DrCOM"：
     *    是字符串"DrCOM"的ASCII。
     * 客户端验证模块校验码:
     *    来自Log文件中的AuthModuleFileHash段
    */
    const int pkt_data_len = 244;
    uint8 pkt_data[pkt_data_len];

    memset(pkt_data, 0, pkt_data_len);
    int data_index = 0;

    int i = 0;

    // header
    pkt_data[data_index++] = 0x07;    // Code
    pkt_data[data_index++] = 0x01;    //id
    pkt_data[data_index++] = 0xf4;    //len(244低位)
    pkt_data[data_index++] = 0x00;    //len(244高位)
    pkt_data[data_index++] = 0x03;    //step 第几步
    pkt_data[data_index++] = (strlen(user_id) & 0xff);    //uid len  用户ID长度

    // 0x0006 mac
    memcpy(pkt_data + data_index, local_mac, 6);
    data_index += 6;

    // 0x000C ip
    memcpy(pkt_data + data_index, &local_ip, 4);
    data_index += 4;

    // 0x0010 fix-options(4B)
    pkt_data[data_index++] = 0x02;
    pkt_data[data_index++] = 0x22;
    pkt_data[data_index++] = 0x00;
    pkt_data[data_index++] = 0x31;

    // 0x0014 challenge
    memcpy(pkt_data + data_index, DrInfo.ChallengeTimer, 4);
    data_index += 4;

    // 0x0018 checkSum

    FillCheckSum(DrInfo.ChallengeTimer, sizeof(DrInfo.ChallengeTimer), &pkt_data[data_index]);
    data_index += 8;

    // 0x0020  帐号 + 计算机名
    int user_id_length = strlen(user_id);
    memcpy(pkt_data + data_index, user_id, user_id_length);
    data_index += user_id_length;
    char UserNameBuffer[15];
    memset(UserNameBuffer, 0, sizeof(UserNameBuffer));
    strcat(UserNameBuffer, "LAPTOP-");
    memcpy(UserNameBuffer + sizeof("LAPTOP-"), local_mac, sizeof(local_mac));
    memcpy(pkt_data + data_index, UserNameBuffer, sizeof(UserNameBuffer));

    data_index += (32 - user_id_length);//用户名+设备名段总长为32

    //0x004B  dns 1 (202.96.128.166)
    data_index += 11;
    pkt_data[data_index++] = 0xca;
    pkt_data[data_index++] = 0x60;
    pkt_data[data_index++] = 0x80;
    pkt_data[data_index++] = 0xa6;

    //0x0050 dhcp server (全0）
    data_index += 4;

    //0x0054 dns 2 (114.114.114.114)
    pkt_data[data_index++] = 0x72;
    pkt_data[data_index++] = 0x72;
    pkt_data[data_index++] = 0x72;
    pkt_data[data_index++] = 0x72;

    //0x0058 wins server 1/2 (totally useless)
    data_index += 8;

    //0x0060  系统版本
    //pkt_data[data_index++] = 0x94;
    data_index += 3;
    pkt_data[data_index++] = 0x06;
    data_index += 3;
    pkt_data[data_index++] = 0x02;
    data_index += 3;
    pkt_data[data_index++] = 0xf0;
    pkt_data[data_index++] = 0x23;
    data_index += 2;
    pkt_data[data_index++] = 0x02;
    data_index += 3;

    //0x0073 魔法值DrCOM
    uint8 drcom_ver[] =
            {'D', 'r', 'C', 'O', 'M', 0x00, 0xb8, 0x01, 0x31, 0x00};
    memcpy(pkt_data + data_index, drcom_ver, sizeof(drcom_ver));

    data_index += 64;

    //0x00b4
    char hashcode[] = "c9145cb8eb2a837692ab3f303f1a08167f3ff64b";
    memcpy(pkt_data + data_index, hashcode, 40);


    /* //旧版U244校验码产生方式，已弃用
    unsigned int crc = drcom_crc32(pkt_data, pkt_data_len);
    #if DRCOM_DEBUG_ON > 0
    print_hex_drcom((char *) &crc, 4);
    #endif

    memcpy(pkt_data + 24, (char *) &crc, 4);
    memcpy(drcom_keepalive_info, (char *) &crc, 4);

    // 完成crc32校验，置位0
    pkt_data[28] = 0x00;
    */

#if DRCOM_DEBUG_ON > 0
    print_hex_drcom(pkt_data,pkt_data_len);
#endif

    // memset(revData, 0, RECV_BUF_LEN);
    int revLen = udp_send_and_rev(pkt_data, pkt_data_len, revData);

#if DRCOM_DEBUG_ON > 0
    print_hex_drcom(revData, revLen);
#endif
    /*数据包U244的响应
    * +------+-------+----+--------+------+----------+
    * | 标头  | 计数器 |长度|  类型  |用户名长| 加密内容长 |
    * +------+-------+----+-------+-------+----------+
    * |  07  |  01   | 30 | 00 04 |   0b  |    20    |
    * +------+-------+------------+-------+----------+
    * |    校验值1    |     未知    |      加密载荷     |
    * +--------------+------------+------------------+
    * | XX XX XX XX  | 01 00 00 00| XX XX ... XX XX  |
    * +--------------+------------+------------------+
    *  校验值1：
    *    U244校验值靠近包头的四字节，先转换为小端序，然后循环右移两次，再转为大端序。
    *    可以猜测服务端的做法是直接赋值到一个uint32里，移完了再赋值回去，没考虑大小端的事情，于是就会出现这种奇观
    *  加密载荷：
    *    这部分收到后的处理和其他部分明显不一样，推测是加密了的，解密算法见 DecodeU244Response函数
    *    解密后的数据只能识别到服务端IP与客户端IP，其他位功能未知
    *
    *  要注意这里会连回两个包，紧接着这个的就是服务端的公告了
    */

#if DRCOM_DEBUG_ON > 0
    DecodeU244Response(revData);
    print_hex_drcom(drcom_keepalive_info, 16);
#endif
    if (revData[0] != 0x07 || revData[4] != 0x04)
        return -1;
    uint8 Announcement[RECV_BUF_LEN];
    udp_send_and_rev(NULL, 0, Announcement);//todo: 有没有可能先收到公告
    return 0;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  FillCheckSum
 *  Description:  生成新版协议中U244/U38所需的的校验值
 *  	  Input:  *ChallengeFromU8:指向U8发来的质询值;
 *  	          Length:质询值的长度;
 *  	          *CheckSum:计算完成的校验值(长8个字节)会被直接填入这里
 *  	 Output:  无
 * =====================================================================================
 */
void FillCheckSum(uint8 *ChallengeFromU8, uint16 Length, uint8 *CheckSum) {

    uint8 Hash[16 + 4] = {0};//16 for md4/5 and 20 for sha1
    uint8 ChallengeFromU8Extended[32] = {0};
    uint8 type;


    memcpy(ChallengeFromU8Extended, ChallengeFromU8, Length);
    *(uint32 *) &ChallengeFromU8Extended[Length] = 20161130;//Extending Challenge Code
    Length += 4;

#if DRCOM_DEBUG_ON
    printf("Info: Challange from u8:\n");
    for (int i = 0; i < 4; ++i) {
        printf("0x%.2x ",*((uint8*)ChallengeFromU8 + i));
    }
    printf("\n\n");
#endif

    type = ChallengeFromU8[0] & 0x03;//这其实是最后两位，但是因为大小端的问题，服务器发出来的时候就跑到最前面了

    if (type == 2) {

        md4(ChallengeFromU8Extended, Length, Hash);

        *((uint8 *) CheckSum + 0) = Hash[1];
        *((uint8 *) CheckSum + 1) = Hash[2];
        *((uint8 *) CheckSum + 2) = Hash[8];
        *((uint8 *) CheckSum + 3) = Hash[9];
        *((uint8 *) CheckSum + 4) = Hash[4];
        *((uint8 *) CheckSum + 5) = Hash[5];
        *((uint8 *) CheckSum + 6) = Hash[11];
        *((uint8 *) CheckSum + 7) = Hash[12];

    } else if (type == 3) {

        sha1(ChallengeFromU8Extended, Length, Hash);

        *((uint8 *) CheckSum + 0) = Hash[2];
        *((uint8 *) CheckSum + 1) = Hash[3];
        *((uint8 *) CheckSum + 2) = Hash[9];
        *((uint8 *) CheckSum + 3) = Hash[10];
        *((uint8 *) CheckSum + 4) = Hash[5];
        *((uint8 *) CheckSum + 5) = Hash[6];
        *((uint8 *) CheckSum + 6) = Hash[15];
        *((uint8 *) CheckSum + 7) = Hash[16];

    } else if (type == 1) {

        md5(ChallengeFromU8Extended, Length, Hash);

        *((uint8 *) CheckSum + 0) = Hash[2];
        *((uint8 *) CheckSum + 1) = Hash[3];
        *((uint8 *) CheckSum + 2) = Hash[8];
        *((uint8 *) CheckSum + 3) = Hash[9];
        *((uint8 *) CheckSum + 4) = Hash[5];
        *((uint8 *) CheckSum + 5) = Hash[6];
        *((uint8 *) CheckSum + 6) = Hash[13];
        *((uint8 *) CheckSum + 7) = Hash[14];

    } else if (type == 0) {

        printf("WARNING:收到旧版U8质询值！\n");
        //尽管这不应该发生，但此处为了保持一定的兼容性，仍然保留了这两句
        //要注意的是，旧版的校验方式和整个U244的内容有关，详见drcom_crc32函数
        *((uint32 *) CheckSum + 0) = checkCPULittleEndian() == 0 ? big2little_32(20000711) : 20000711;
        *((uint32 *) CheckSum + 1) = checkCPULittleEndian() == 0 ? big2little_32(126) : 126;
        //本想绕开大小端的，但那样会打断常量

    }
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  SendU40DllUpdater
 *  Description:  发送drcom长度为40的数据包，这种包每次出现都是两个来回一组。第一/三个包由客户端发送
 *  	  Input:  type:包类型，可选1或3;
 *  	 Output:  成功返回0
 * =====================================================================================
 */
int SendU40DllUpdater(uint8 type) {
    /*数据包U40系列 (上下行均适用)
    * +------+-------+-------+-----+-----+------------+
    * | 标头  | 计数器 |  长度  | 类型 | 步骤 | MyDllVer  |
    * +------+-------+-------+-----+-----+-----------+
    * |  07  |  XX   | 28 00 | 0b  |  0X |  dc 02    |
    * +------+-+-----+-----+-+----++-----+-----------+
    * | 随机值  |   6字节零#  |    时间码    |  四字节零   |
    * +--------+-----------+-------------+-----------+
    * | 00  00 | 00 ... 00 | XX XX XX XX | 00 ... 00 |
    * +--------+----+------+------+------+-----------+
    * |    校验值*   |   客户端IP*   |     八字节零      |
    * +-------------+------+------+------+-----------+
    * | XX XX XX XX | c0 a8 XX XX | 00 00 ... 00 00  |
    * +-------------+-------------+------+-----------+
    *
    * 计数器：
    *  每个来回会加1。注意，如果收到的回包是U40-6，也会加1
    * 步骤：
    *  程序中U40-n的数字n值得就是这个步骤。奇数为客户端->服务端。偶数反之
    * 时间码：
    *  服务端发回数据包时，会更新时间码，需要记录。
    * 6字节零#：
    *  如果是U40-6，则从这里开始格式变为：四字节零(而不是六)，四字节某种长度，四字节某种校验值，四字节零，四字节某种版本，文件载荷
    * 校验值*：
    *  只存在于U40-3，其他包均为0。
    * 客户端IP*：
    *  只存在于U40-3，其他包均为0。
    */
    if (debug_log_style) {
        printf("%s\tDrcom: Sending U40 response phase %d.\n", getTime(), type);
    } else {
        switch (type) {
            case 1:
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_SendU40_1);
                break;
            case 3:
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_SendU40_3);
                break;
            default:
                printf("WTF??");
                break;
        }
    }
    const int pkt_data_len = 40;
    uint8 pkt_data[pkt_data_len];

    memset(pkt_data, 0, pkt_data_len);
    int data_index = 0;
    pkt_data[data_index++] = 0x07;    // Code
    pkt_data[data_index++] = drcom_pkt_counter;
    pkt_data[data_index++] = 0x28;    //len(40低位)
    pkt_data[data_index++] = 0x00;  //len(40高位)

    pkt_data[data_index++] = 0x0B;    // Step
    pkt_data[data_index++] = type;  // Type

    memcpy(pkt_data + data_index, DrInfo.MyDllVer, 2);
    data_index += 2;


    pkt_data[data_index++] = 0x00;    //此处为两位随机生成值，用于分辨同一组包，但置零并不会影响功能
    pkt_data[data_index++] = 0x00;


    memcpy(pkt_data + 16, DrInfo.ChallengeTimer, 4);

    if (type == 3) {//只有U40-3需要校验值
        uint32 CheckSum = GetU40_3Sum(pkt_data);
        memcpy(pkt_data + 24, &CheckSum, 4);
    }
    int revLen =
            udp_send_and_rev(pkt_data, pkt_data_len, revData);
#if DRCOM_DEBUG_ON > 0
    print_hex_drcom(revData, revLen);
#endif
    if (revData[0] != 0x07 || revData[4] != 0x0b)
        return -1;

    return 0;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  SendU38HeartBeat
 *  Description:  发起DrCom协议的心跳包U38
 *  	  Input:  无
 *  	 Output:  成功返回0
 * =====================================================================================
 */
int SendU38HeartBeat() {
    /*数据包U38系列
   * +------+-----------+----------------+
   * | 标头  |  七字节零   | U8[8:12] 质询值 |
   * +------+-----------+----------------+
   * |  ff  | 00 ... 00 |  XX XX  XX XX  |
   * +------+-----------+------+---------+---+
   * |         U38 校验值       |    "Drco"   |
   * +-------------------------+--------+----+
   * | XX XX XX XX XX XX XX XX | 44 72 63 6f |
   * +-------------+-----------+-------------+------+----------------+-------+
   * |    服务端IP  | OffsetId  |    客户端IP   | 常数 | ClientBufSerno | 随机数  |
   * +-------------+-----------+-------------+------+----------------+-------+
   * | c0 a8 7f 81 |  XX   XX  | c0 a8 XX XX |  01  |       XX       | XX XX |
   * +-------------+-----------+-------------+------+----------------+-------+
   * U38校验值:
   *  产生方式与U244的那个应该是一样的
   */

    const int pkt_data_len = 38;
    uint8 pkt_data[pkt_data_len];
    memset(pkt_data, 0, pkt_data_len);
    int data_index = 0;

    pkt_data[data_index++] = 0xff;    // Code

    data_index += 7;
    memcpy(pkt_data + data_index, DrInfo.ChallengeTimer, 4);
    data_index += 4;

    FillCheckSum(DrInfo.ChallengeTimer, 4, pkt_data + data_index);
    data_index += 8;

    char Drco[] =
            {'D', 'r', 'c', 'o'};
    memcpy(pkt_data + data_index, Drco, 4);
    data_index += 4;

    uint32 ServerIp = inet_addr(DR_SERVER_IP);
    memcpy(pkt_data + data_index, &ServerIp, sizeof(ServerIp));
    data_index += 4;

    memcpy(pkt_data + data_index, DrInfo.ServerOffsetId, 2);
    data_index += 2;

    memcpy(pkt_data + data_index, &local_ip, 4);
    data_index += 4;

    pkt_data[data_index++] = 0x01;
    memcpy(pkt_data + data_index, DrInfo.ServerClientBufSerno, 1);
    data_index += 1;

    pkt_data[data_index++] = 0x00;
    pkt_data[data_index++] = 0x00;//对包码,用于分辩同一组包

/*
    for (int i = 0; i < 38; ++i) {
        if (i && i % 7 == 0)printf("\n");
        printf("0x%.2x ", pkt_data[i]);
    }*/

    udp_send_and_rev(pkt_data, pkt_data_len, revData);
    if (revData[0] != 0x07 || revData[4] != 0x06)    // Start Response
        return -1;
    return 0;

}

/*
* ===  FUNCTION  ======================================================================
*         Name:  GetU40_3Sum
*  Description:  填充U40-3校验值
*  	  Input:  buf，指向数据包内容
*  	 Output:  无
* =====================================================================================
*/
uint32 GetU40_3Sum(uint8 *buf) {
    int16_t v7 = 0;
    uint16_t v5 = 0;
    for (int i = 0; i < 20; i++) {
        memcpy(&v7, &buf[2 * i], 2);
        v5 ^= v7;
    }
    return (uint32) (v5 * 711);
}
/*
* ===  FUNCTION  ======================================================================
*         Name:  init_udp_socket
*  Description:  初始化DrCom协议所使用的UDPsocket
*  	  Input:  无
*  	 Output:  无
* =====================================================================================
*/
void init_udp_socket() {
    struct sockaddr_in LocalAddr;
    struct sockaddr_in DrcomSerAddr;
    memset(&LocalAddr, 0, sizeof(LocalAddr));
    LocalAddr.sin_family = AF_INET;
    LocalAddr.sin_port = htons(clientPort);
    LocalAddr.sin_addr.s_addr = local_ip;


    memset(&DrcomSerAddr, 0, sizeof(DrcomSerAddr));
    DrcomSerAddr.sin_family = AF_INET;
    DrcomSerAddr.sin_port = htons(DR_PORT);
    inet_pton(AF_INET, DR_SERVER_IP, &DrcomSerAddr.sin_addr);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sock) {
        perror("Create drcom socket failed\n");
        exit(-1);
    }

    if (0 != bind(sock, (struct sockaddr *) &LocalAddr, sizeof(LocalAddr))) {
        perror("Bind drcom socket failed\n");
        exit(-1);
    }

    //UDP是无连接的，这里的connect只是在内核的网络层面上建立“这个socket“与”这个地址“的关联，这么做有以下几点好处
    //      1：使用这个socket收发包时，不再需要提供对端地址
    //      2：可以在对端不可达时捕获到相应的异步ICMP错误
    //      3：这个socket只会接收到DrCom服务器发来的数据包，而其他地址往这个端口发包将被内核直接丢弃，有助于提升稳健性
    if (0 != connect(sock, (struct sockaddr *) &DrcomSerAddr,sizeof (DrcomSerAddr))){
        perror("Connect drcom socket failed\n");
        perror("This might caused by wrong interface this socket have bound to\n");
        //已知的一种出错形式是：当bind到本地环回时，connect本地环回以外的任何地址都会失败。
        //除非用户在dev参数传入了本地环回，否则这种情况应当不会发生
        exit(-1);
    }
}



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  udp_send_and_rev
 *  Description:  发送并接收udp协议的数据包
 *  	  Input:  *send_buf: 指向待发送数据的指针; send_len: 待发送数据的长度; 这两者任一为空表示只收不发
 				  *recv_buf: 指向接收缓冲区的指针
 *  	 Output:  返回接收的长度
 * =====================================================================================
 */
int udp_send_and_rev(uint8 *send_buf, int send_len, uint8 *recv_buf) {
    int try_times = RETRY_TIME;
    ssize_t ret;
    struct timeval TimeVal;
    fd_set FdSet;

    while (try_times--) {
        if (send_len != 0 && send_buf != NULL) {
            ret = send(sock, send_buf, send_len, 0);
            if (ret == -1) {
                perror("Udp send failed. This should not happen!\n");//正常来讲，UDP发送是不会“失败”的（就算对面根本没收到）。如果这里报错，通常是socket本身的问题
                exit(-1);
            }
        }

        //接收
        FD_ZERO(&FdSet);
        FD_SET(sock, &FdSet);
        TimeVal.tv_sec = 4;
        TimeVal.tv_usec = 0;  //设置超时时间4秒，应该够了

        ret = select(sock + 1, &FdSet, NULL, NULL, &TimeVal);
        //通过select函数来在一定时限内阻塞式地监听这个socket是否准备好读（即有数据发进来了）
        //这是为了修复先前版本直接用recvfrom而引起的——任何一次丢包都会导致整个线程无止境地阻塞，直至掉线
        //Must reset TimeVal and FdSet after each use
        if (ret == -1) {
            perror("UDP socket select failed. This is very strange\n");//这里报错基本就意味着程序的编写有问题，与运行环境无关
            exit(-1);
        }

        if (FD_ISSET(sock, &FdSet)) {//收到内容
            ret = recv(sock, recv_buf, RECV_BUF_LEN, 0);
            if (ret == -1) {
                if (errno == ECONNREFUSED) { //处理收到的ICMP错误
                    //这得是之前的包发到了DrCom服务器上，但是内核发现没有程序在监听对应的UDP端口才会反回这个错误
                    perror("UDP packet was delivered. However Drcom service is not listing the correspond port\n");
                } else {
                    perror("Unknown udp error\n");
                }
            }else{
                break; //成功接收
            }
        } else {//超时
            perror("UDP timeout");
            if (try_times > 0) {
                perror("Try send it again\n");
            }
        }
    }
    return ret;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  U8ResponseParser
 *  Description:  分析U8响应包，提取信息
 *  	  Input:  无
 *  	 Output:  无
 * =====================================================================================
 */
void U8ResponseParser() {
    memcpy(DrInfo.MyDllVer, revData + 28, 4);
    memcpy(DrInfo.ChallengeTimer, revData + 8, 4);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  U244ResponseParser
 *  Description:  分析U244响应包，提取信息
 *  	  Input:  无
 *  	 Output:  无
 * =====================================================================================
 */
void U244ResponseParser() {
    DecodeU244Response(revData);
    uint8 *pBuf = &revData[revData[2] - revData[6]];//指向解密后的加密载荷的起始处
    memcpy(DrInfo.ServerOffsetId, pBuf + 8, 2);
    memcpy(DrInfo.ServerClientBufSerno, pBuf + 15, 1);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  U40ResponseParser
 *  Description:  分析U40响应包，提取信息
 *  	  Input:  无
 *  	 Output:  无
 * =====================================================================================
 */
void U40ResponseParser() {
    if (revData[5] == 0x06) { //File 类
        //这种包可能是用来更新mydll用的，但是发过来的dll不完整.当然最好不要完整发过来，那个文件看起来不小
        //正常来讲如果不主动发U40-5或发送含有错误版本的U40-1/3时是不会进入这里的
        memcpy(DrInfo.MyDllVer, revData + 28, 4);//所以这里还是更新一下MyDllVer比较好
        printf("%s\tInfo: Got dll from U40. Ignored.\n",getTime());

    } else {
        memcpy(DrInfo.ChallengeTimer, revData + 16, 2);// 只有不是File的时候revData[16:19]才是时间
    }


}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  perrorAndSleep
 *  Description:  打印错误信息并休眠
 *  	  Input:  *str: 指向待打印字符串的指针
 *  	 Output:  无
 * =====================================================================================
 */
static void perrorAndSleep(char *str) {
    printf("%s\tError Report: %s\n", getTime(),str);
    strcpy(dstatusMsg, str);
    dstatus = DOFFLINE;
    sleep(20);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  printAll
 *  Description:  打印错误信息
 *  	  Input:  *str: 指向待打印字符串的指针
 *  	 Output:  无
 * =====================================================================================
 */
static void printAll(char *str) {
    printf("%s\tError Report: drcom %s\n", getTime(),str);
    strcpy(dstatusMsg, str);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  DrComServerDaemon
 *  Description:  drcom认证主程序
 *  	  Input:  *args: 传入的参数指针(并不需要)
 *  	 Output:  无
 * =====================================================================================
 */
void *DrComServerDaemon(void *args) {

    /*允许取消进程*/
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    /*异步取消， 线程接到取消信号后，立即退出*/
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    int ret;
    strcpy(dstatusMsg, "please log on first");

    drcom_pkt_counter = 0;
    dstatus = DOFFLINE;
    needToSendDrComStart = 1;

    while (1)//todo:检查是否涵盖所有情况
    {
        sleep(2);
        if (xstatus == XOFFLINE)  //802.1x还没有上线
        {
            continue;
        }

        if (needToSendDrComStart) {

            printf("%s\t%s", getTime(), DMSG_SendU8);
            ret = SendU8GetChallenge();
            if (ret != 0) {
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_SendU8_Fail);
                return NULL;
            }
            needToSendDrComStart = 0;
            continue;
        }
        //下面开始处理收到的数据包 这是一个以收到的数据包的标志位驱动的状态机//todo:会不会一个包被重复处理多次？
        if ((revData[0] == 0x07) && (revData[4] == 0x02)) { //Response for start request U8
            if (debug_log_style)
                printf("%s\t", getTime());
            printf("%s", DMSG_GotU8R);

            if (dstatus == DOFFLINE) { //还没有发送U244
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_SendU244);
                ret = SendU244Login();
                if (ret != 0) {
                    if (debug_log_style)
                        printf("%s\t", getTime());
                    printf("%s", DMSG_SendU244_Fail);
                    continue;
                }

            } else if (dstatus == DONLINE)  //drcom协议 已经上线成功
            {
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_SendU38);
                ret = SendU38HeartBeat();
                if (ret != 0) {
                    if (debug_log_style)
                        printf("%s\t", getTime());
                    printf("%s", DMSG_SendU38_Fail);
                    continue;
                }
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_SentU38);
            }
            continue;
        }

        if ((revData[0] == 0x07) && (revData[4] == 0x04))  //U244登录成功
        {
            U244ResponseParser();
            if (debug_log_style)
                printf("%s\t", getTime());
            printf("%s", DMSG_LoginU244);
            dstatus = DONLINE;
            DrInfo.U8Counter = 2;//登录成功后是从2开始数
            ret = SendU40DllUpdater(1);
            if (ret != 0) {
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_SendU40_1_Fail);
                continue;
            }
        }

        if ((revData[0] == 0x07) && (revData[4] == 0x0b))  //U40-X
        {
            U40ResponseParser();
            if (revData[5] == 0x02) {
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_GotU40_2);
                SendU40DllUpdater(3);
            } else if (revData[5] == 0x04) {
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_FinishU40);
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_StartInterval);
                sleep(8);
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_DoneInterval);
                printf("%s\t%s", getTime(), DMSG_SendU8);
                ret = SendU8GetChallenge();
                DrInfo.U8Counter++;
                if (ret != 0) {
                    if (debug_log_style)
                        printf("%s\t", getTime());
                    printf("%s", DMSG_SendU8_Fail);
                    return NULL;
                }

            }
            drcom_pkt_counter++;
        }

        if ((revData[0] == 0x07) && (revData[4] == 0x06))  //U38-R
        {
            if (debug_log_style)
                printf("%s\t", getTime());
            //U38的回包没啥好处理的
            printf("%s", DMSG_GotU38);
            sleep(1);
            ret = SendU40DllUpdater(1);
            if (ret != 0) {
                if (debug_log_style)
                    printf("%s\t", getTime());
                printf("%s", DMSG_SendU40_1_Fail);
                continue;
            }
        }
    }
    close(sock);
    return NULL;
}

void DecodeU244Response(uint8 *buf) {

    uint8 *pBuf = &buf[buf[2] - buf[6]];
    uint16 shift;
    uint8 len = buf[6];
    uint8 tempLeft, tempRight;

    for (int i = 0; i < len; ++i) {
        shift = i & 0x7;
        tempLeft = pBuf[i] << shift;
        tempRight = pBuf[i] >> (8 - shift);
        pBuf[i] = tempRight + tempLeft;
    }
#if DRCOM_DEBUG_ON > 0
    for (int i = 0; i < buf[2]; ++i) {
        if (i && i % 7 == 0)printf("\n");
        printf("0x%.2x ", buf[i]);
    }
#endif

}
