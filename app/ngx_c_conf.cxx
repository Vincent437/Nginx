// CConfig 类的函数实现

// 系统头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fstream>

// 自定义头文件

// 相关函数声明
#include "ngx_func.h"
// 读取配置类的头文件
#include "ngx_c_conf.h"

// 静态成员变量赋值，初始为空
CConfig *CConfig::m_instance = NULL;

// 构造函数
CConfig::CConfig() {}

// 析构函数
CConfig::~CConfig()
{
    // 清空数组
    m_ConfigItemList.clear();

    return;
}

/**
 *  @brief 加载配置文件
 *  @param[in] pconfName 配置文件的文件描述符
 *  @return 加载成功为 1，失败为 0
 */
bool CConfig::Load(const std::string conf_name)
{
    // 初始化文件和读取内容
    std::ifstream infile(conf_name);
    std::string line;

    // 定义空格、换行符、回车符、Tab用于后续处理
    const std::string WHITESPACE = " \n\r\t";

    // 读取失败则返回 0
    if(!infile.is_open()){return false;}

    // 文件打开成功，开始处理

    // 循环读取每一行文件
    while(getline(infile,line)){
        // 内容为空，直接读取下一行
        if(line[0]==0) continue;
        // ignore comments
        if(line[0]==';' || line[0]==' ' || line[0]=='[' || line[0]=='#' || line[0]=='\t' || line[0]=='\n') continue;
        
        size_t endPos = line.find_last_not_of(WHITESPACE);
        if (endPos != std::string::npos) {
            line.erase(endPos + 1); // 截取到最后一个非空白字符
        } else {
            line.clear(); // 全是空白字符，直接清空
        }

        // 开始处理读取到的每一行配置信息

        size_t equal_pos = line.find('='); // 查找 '=' 所在位置
        if(equal_pos!=std::string::npos){
            // 创建临时指针保存信息
            LPCConfItem *p_confitem = new LPCConfItem();

            // 去除读取配置信息时可能多余读取的左侧或右侧空格
            p_confitem->ItemName = trim(line.substr(0,equal_pos));
            p_confitem->ItemContent = trim(line.substr(equal_pos+1));
            
            // std::cout<<p_confitem->ItemName<<' '<<p_confitem->ItemContent<<std::endl;
            // 将读取成功后的配置信息保存到类内成员数组当中，需要注意，内存最后需要释放
            m_ConfigItemList.push_back(*p_confitem);
        } //end if
    } //end while(getline(infile,line))

    // 关闭打开的文件
    infile.close();

    return true;
}

/***************************************************************
 *  @brief     读取字符串类型的配置项目
 *  @param     item_name    待查找的配置项目名称
 *  @return    查找结果，查到返回字符串，否则返回空
 **************************************************************/
std::string CConfig::GetString(const std::string item_name)
{
	std::vector<LPCConfItem>::iterator pos;	
	for(pos = m_ConfigItemList.begin(); pos != m_ConfigItemList.end(); ++pos)
	{	
		if(pos->ItemName==item_name)
			return (pos)->ItemContent;
	}//end for
	return NULL;
}

/***************************************************************
 *  @brief     查找数值类型的配置项目
 *  @param     item_name     待查找的配置项目名称
 *  @param     def     默认值
 *  @return    返回值
 *  @note      查找结果，查到返回字符串，否则返回给定的默认值
 **************************************************************/
int CConfig::GetIntDefault(const std::string item_name,const int def)
{
	std::vector<LPCConfItem>::iterator pos;	
	for(pos = m_ConfigItemList.begin(); pos !=m_ConfigItemList.end(); ++pos)
	{	
		if(pos->ItemName==item_name)
			return std::stoi(pos->ItemContent);
	}//end for
	return def;
}