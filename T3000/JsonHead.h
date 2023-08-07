#pragma once
#pragma region JsonCode

#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>
using json = nlohmann::json;
#include <string>

#define JSON_COLOR_LENGTH 10
#define JSON_STRING_LENGTH  20

typedef struct str_viewportTransform
{
	unsigned short scale;
	unsigned short x;
	unsigned short y;
};

typedef struct str_settings
{
	unsigned char active;
	char bgColor[JSON_COLOR_LENGTH];
	char fillColor[JSON_COLOR_LENGTH];
	unsigned char fontSize;
	unsigned char inAlarm;
	char icon[JSON_STRING_LENGTH];
	char offColor[JSON_COLOR_LENGTH];
	char onColor[JSON_COLOR_LENGTH];
	char t3EntryDisplayField[JSON_STRING_LENGTH];
	char textAlign[JSON_STRING_LENGTH];
	char textColor[JSON_COLOR_LENGTH];
	char titleColor[JSON_COLOR_LENGTH];
	unsigned char blink;
	unsigned short blinkInterval;
};

typedef struct myitems
{
	unsigned char active;
	unsigned char group;
	unsigned short height;
	unsigned char id;
	unsigned short rotate;
	unsigned short scaleX;
	unsigned short scaleY;
	str_settings settings;
	char title[JSON_STRING_LENGTH];
	int translate[4];
	char strtype[JSON_STRING_LENGTH];
	unsigned short width;
	unsigned short zindex;
};

typedef struct StrJson
{
	unsigned short ncount; //控制结构体 myitems 的数量的，不存入设备
	unsigned char ntranslate_count;// 控制结构体 translate 数量

	unsigned char activeItemIndex;
	unsigned char customObjectsCount;
	unsigned char groupCount;

	unsigned short itemsCount;

	myitems* m_data_item;
	char version[JSON_COLOR_LENGTH];
	//string version;
	str_viewportTransform viewportTransform;
	// Constructor to convert JSON to struct
	StrJson(const nlohmann::json& jsonData)
	{
		strcpy_s(version, jsonData.value("version", "").c_str());
		activeItemIndex = jsonData.value("activeItemIndex", 0);
		customObjectsCount = jsonData.value("customObjectsCount", 0);
		itemsCount = jsonData.value("itemsCount", 0);
		groupCount = jsonData.value("groupCount", 0);
		viewportTransform.scale = jsonData["viewportTransform"].value("scale", 0);
		viewportTransform.x = jsonData["viewportTransform"].value("x", 0);
		viewportTransform.y = jsonData["viewportTransform"].value("y", 0);

		ncount = 0; // 获取动态item的个数
		for (const auto& item : jsonData["items"])
		{
			ncount++;
		}
		m_data_item = new myitems[ncount];
		for (int i = 0; i < ncount; i++)
		{
			m_data_item[i].active = jsonData["items"][i].value("active", 0);// .get<bool>();
			m_data_item[i].group = jsonData["items"][i].value("group", 0);
			m_data_item[i].height = jsonData["items"][i].value("height", 0);
			m_data_item[i].id = jsonData["items"][i].value("id", 0);
			m_data_item[i].rotate = jsonData["items"][i].value("rotate", 0);
			m_data_item[i].scaleX = jsonData["items"][i].value("scaleX", 0);
			m_data_item[i].scaleY = jsonData["items"][i].value("scaleY", 0);

			ntranslate_count = 0;
			for (const auto& item : jsonData["items"][i]["translate"]) //Gets the number of the array
			{
				ntranslate_count++;
			}
			for (int j = 0; j < ntranslate_count; j++)
			{
				m_data_item[i].translate[j] = jsonData["items"][i]["translate"].at(j);
			}
			//if (!jsonData["items"][i]["title"].is_null()) // if "title" is missing  ,it will throw exception crash  here
			//{
			//	string mytitle = jsonData["items"][i].value("title", "");
			//}
			//strcpy_s(m_data_item[i].title, jsonData["items"][i].value("title", "").c_str());
			strcpy_s(m_data_item[i].strtype, jsonData["items"][i].value("type", "").c_str());
			m_data_item[i].width = jsonData["items"][i].value("width", 0);
			m_data_item[i].zindex = jsonData["items"][i].value("zindex", 0);

			m_data_item[i].settings.active = jsonData["items"][i]["settings"].value("active", 0);
			strcpy_s(m_data_item[i].settings.bgColor, jsonData["items"][i]["settings"].value("bgColor", "").c_str());
			m_data_item[i].settings.fontSize = jsonData["items"][i]["settings"].value("fontSize", 16);

			strcpy_s(m_data_item[i].settings.fillColor, jsonData["items"][i]["settings"].value("fillColor", "").c_str());
			m_data_item[i].settings.inAlarm = jsonData["items"][i]["settings"].value("inAlarm", 0);

			strcpy_s(m_data_item[i].settings.icon, jsonData["items"][i]["settings"].value("icon", "").c_str());
			strcpy_s(m_data_item[i].settings.offColor, jsonData["items"][i]["settings"].value("offColor", "").c_str());
			strcpy_s(m_data_item[i].settings.onColor, jsonData["items"][i]["settings"].value("onColor", "").c_str());
			strcpy_s(m_data_item[i].settings.t3EntryDisplayField, jsonData["items"][i]["settings"].value("t3EntryDisplayField", "").c_str());
			strcpy_s(m_data_item[i].settings.textAlign, jsonData["items"][i]["settings"].value("textAlign", "").c_str());
			strcpy_s(m_data_item[i].settings.textColor, jsonData["items"][i]["settings"].value("textColor", "").c_str());
			strcpy_s(m_data_item[i].settings.titleColor, jsonData["items"][i]["settings"].value("titleColor", "").c_str());

			m_data_item[i].settings.blink = jsonData["items"][i]["settings"].value("blink", 0);
			m_data_item[i].settings.blinkInterval = jsonData["items"][i]["settings"].value("blinkInterval", 0);

		}
		Sleep(1);
	}

	~StrJson()
	{
		if (m_data_item != NULL)
		{
			delete[] m_data_item;
		}
	}


}Str_Json;

#pragma pack(push) //保存对齐状态 
#pragma pack(1)

typedef union
{
	uint8_t all[40];
	struct
	{
		unsigned short ncount; //控制结构体 myitems 的数量的，不存入设备
		unsigned char ntranslate_count;// 控制结构体 translate 数量
		unsigned char activeItemIndex;
		unsigned char customObjectsCount;
		unsigned char groupCount;
		unsigned short itemsCount;
		char version[JSON_COLOR_LENGTH];
		str_viewportTransform viewportTransform;
	}reg;
}Str_t3_Json;


typedef union
{
	uint8_t all[40];
	struct
	{
		myitems json_items;  //size约185
	}reg;
}Str_item_Json;


#pragma pack(pop)//恢复对齐状态 


#pragma endregion