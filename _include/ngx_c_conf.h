
// 防卫式声明
#ifndef __NGX_CONF_H__
#define __NGX_CONF_H__

#include <vector>

// global variables
#include "ngx_global.h"

// Singleton Class，专门用于读取配置文件中的各项配置信息并保存在类内成员数组中
class CConfig
{
private:
	// 将构造函数设为私有
	CConfig();
	// 禁用拷贝构造和重载赋值运算符函数
	CConfig(const CConfig &temp) = delete;
	CConfig &operator=(const CConfig &temp) = delete;

public:
	~CConfig();

private:
	// 指向全局唯一的类对象的指针
	static CConfig *m_instance;

public:
	static CConfig *GetInstance() //可以使用局部静态变量，C++11保证线程安全
	{
		if (m_instance == NULL)
		{
			// 考虑到会在主线程中就实例化出对象，因此无需加锁
			// 锁
			if (m_instance == NULL)
			{
				m_instance = new CConfig();
				static CDestructor cd;
			}
			// 放锁
		}

		return m_instance;
	}

	// 类中定义类，专门用于释放类对象指向的堆区内存
	class CDestructor
	{
	public:
		~CDestructor()
		{
			if (CConfig::m_instance)
			{
				delete CConfig::m_instance;
				CConfig::m_instance = NULL;
			}
		}
	};


public:
	// 加载配置文件信息到类内成员数组中
	bool Load(const std::string pconfName);
	// 获取字符串类配置信息
	std::string GetString(const std::string item_name);
	// 获取数值类配置信息
	int GetIntDefault(const std::string item_name,const int def);

public:
	// 根据定义的结构体形式读取配置文件信息并存储到列表当中
	std::vector<CConfItem*> m_ConfigItemList; 
};

#endif