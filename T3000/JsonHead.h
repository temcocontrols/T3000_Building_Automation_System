#pragma once
#pragma region JsonCode

#ifndef  DISABLE_HANDLE_JSON_DATA



#include "CM5/ud_str.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>
using json = nlohmann::json;
#include <string>

#define JSON_COLOR_LENGTH 10
#define JSON_STRING_LENGTH  10




typedef struct StrJson
{
	// Controls the number of items in the structure myitems, does not store in the device
	unsigned short ncount; //控制结构体 myitems 的数量的，不存入设备


	unsigned char activeItemIndex;
	unsigned char customObjectsCount;
	unsigned char groupCount;

	unsigned short itemsCount;

	myitems* m_data_item;
	char version[JSON_COLOR_LENGTH];
	//string version;
	str_viewportTransform viewportTransform;
#if 1
	void jsonBuild(nlohmann::json& jsonData, const Str_t3_screen_Json& json_screen)
	{
		jsonData["activeItemIndex"] = json_screen.reg.activeItemIndex;
		jsonData["customObjectsCount"] = json_screen.reg.customObjectsCount;
		jsonData["groupCount"] = json_screen.reg.groupCount;
		jsonData["itemsCount"] = json_screen.reg.itemsCount;
		jsonData["version"] = json_screen.reg.version;
		jsonData["viewportTransform"] = { json_screen.reg.viewportTransform.scale,json_screen.reg.viewportTransform.x,json_screen.reg.viewportTransform.y };
		int z = 0; //temp_index;
		for (int i = 0; i < json_screen.reg.ncount; i++)
		{
			bool find_next = false;
			for (int j = z; j < BAC_GRPHIC_JSON_ITEM_COUNT; j++) //把所有80个里面，从头开始是这幅图的组件拉出来;
			{
				if (m_json_item_data.at(j).reg.json_items.item_belong_screen == screen_list_line)
				{
					z = j;
					find_next = true;
					break;
				}
			}
			if (find_next)
			{
				jsonData["items"][i]["active"] = m_json_item_data.at(z).reg.json_items.active;
				jsonData["items"][i]["group"] = m_json_item_data.at(z).reg.json_items.group;
				jsonData["items"][i]["height"] = m_json_item_data.at(z).reg.json_items.height;
				jsonData["items"][i]["id"] = m_json_item_data.at(z).reg.json_items.id;
				jsonData["items"][i]["rotate"] = m_json_item_data.at(z).reg.json_items.rotate;

				jsonData["items"][i]["scaleX"] = m_json_item_data.at(z).reg.json_items.scaleX;
				jsonData["items"][i]["scaleY"] = m_json_item_data.at(z).reg.json_items.scaleY;
				jsonData["items"][i]["type"] = m_json_item_data.at(z).reg.json_items.strtype;
				jsonData["items"][i]["title"] = m_json_item_data.at(z).reg.json_items.title;
				if (m_json_item_data.at(z).reg.json_items.ntranslate_count == 1)
					jsonData["items"][i]["translate"] = m_json_item_data.at(z).reg.json_items.translate[0];
				else if (m_json_item_data.at(z).reg.json_items.ntranslate_count == 2)
					jsonData["items"][i]["translate"] = { m_json_item_data.at(z).reg.json_items.translate[0],m_json_item_data.at(z).reg.json_items.translate[1] };
				else if (m_json_item_data.at(z).reg.json_items.ntranslate_count == 3)
					jsonData["items"][i]["translate"] = { m_json_item_data.at(z).reg.json_items.translate[0],m_json_item_data.at(z).reg.json_items.translate[1],m_json_item_data.at(z).reg.json_items.translate[2] };
				else 
					jsonData["items"][i]["translate"] = { m_json_item_data.at(z).reg.json_items.translate[0],m_json_item_data.at(z).reg.json_items.translate[1],m_json_item_data.at(z).reg.json_items.translate[2],m_json_item_data.at(z).reg.json_items.translate[3] };
			

				jsonData["items"][i]["width"] = m_json_item_data.at(z).reg.json_items.width;
				jsonData["items"][i]["zindex"] = m_json_item_data.at(z).reg.json_items.zindex;

				jsonData["items"][i]["settings"]["active"] = m_json_item_data.at(z).reg.json_items.settings.active;
				jsonData["items"][i]["settings"]["bgColor"] = m_json_item_data.at(z).reg.json_items.settings.bgColor;
				jsonData["items"][i]["settings"]["blink"] = m_json_item_data.at(z).reg.json_items.settings.blink;
				jsonData["items"][i]["settings"]["blinkInterval"] = m_json_item_data.at(z).reg.json_items.settings.blinkInterval;
				jsonData["items"][i]["settings"]["fillColor"] = m_json_item_data.at(z).reg.json_items.settings.fillColor;
				jsonData["items"][i]["settings"]["fontSize"] = m_json_item_data.at(z).reg.json_items.settings.fontSize;
				jsonData["items"][i]["settings"]["icon"] = m_json_item_data.at(z).reg.json_items.settings.icon;
				jsonData["items"][i]["settings"]["inAlarm"] = m_json_item_data.at(z).reg.json_items.settings.inAlarm;
				jsonData["items"][i]["settings"]["offColor"] = m_json_item_data.at(z).reg.json_items.settings.offColor;
				jsonData["items"][i]["settings"]["t3EntryDisplayField"] = m_json_item_data.at(z).reg.json_items.settings.t3EntryDisplayField;
				jsonData["items"][i]["settings"]["textAlign"] = m_json_item_data.at(z).reg.json_items.settings.textAlign;
				jsonData["items"][i]["settings"]["textColor"] = m_json_item_data.at(z).reg.json_items.settings.textColor;
				jsonData["items"][i]["settings"]["titleColor"] = m_json_item_data.at(z).reg.json_items.settings.titleColor;
			}
			z++;
		}
			

		//jsonData = nlohmann::json{ 
		//	{"activeItemIndex", json_screen.reg.activeItemIndex},
		//	{"customObjectsCount", json_screen.reg.customObjectsCount},
		//	{"groupCount", json_screen.reg.groupCount}
		//};
	}
#endif
	StrJson() { m_data_item = NULL; }
	// Constructor to convert JSON to struct
	StrJson(const nlohmann::json& jsonData)
	{
		strcpy_s(version, jsonData.value("version", "").c_str());
		if(jsonData["activeItemIndex"].is_null())
			activeItemIndex = 0;
		else
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

			m_data_item[i].ntranslate_count = 0;
			for (const auto& item : jsonData["items"][i]["translate"]) //Gets the number of the array
			{
				m_data_item[i].ntranslate_count++;
			}
			for (int j = 0; j < m_data_item[i].ntranslate_count; j++)
			{
				m_data_item[i].translate[j] = jsonData["items"][i]["translate"].at(j);
			}
			//if (!jsonData["items"][i]["title"].is_null()) // if "title" is missing  ,it will throw exception crash  here
			//{
			//	string mytitle = jsonData["items"][i].value("title", "");
			//}
			//strcpy_s(m_data_item[i].title, jsonData["items"][i].value("title", "").c_str());
			strcpy_s(m_data_item[i].strtype, 20, jsonData["items"][i].value("type", "").c_str());
			m_data_item[i].width = jsonData["items"][i].value("width", 0);
			m_data_item[i].zindex = jsonData["items"][i].value("zindex", 0);

			m_data_item[i].settings.active = jsonData["items"][i]["settings"].value("active", 0);
			strcpy_s(m_data_item[i].settings.bgColor, jsonData["items"][i]["settings"].value("bgColor", "").c_str());
			m_data_item[i].settings.fontSize = jsonData["items"][i]["settings"].value("fontSize", 16);

			strcpy_s(m_data_item[i].settings.fillColor, jsonData["items"][i]["settings"].value("fillColor", "").c_str());
			m_data_item[i].settings.inAlarm = jsonData["items"][i]["settings"].value("inAlarm", 0);

			strcpy_s(m_data_item[i].settings.icon, JSON_STRING_LENGTH, jsonData["items"][i]["settings"].value("icon", "").c_str());
			strcpy_s(m_data_item[i].settings.offColor, JSON_STRING_LENGTH, jsonData["items"][i]["settings"].value("offColor", "").c_str());
			strcpy_s(m_data_item[i].settings.onColor, JSON_STRING_LENGTH, jsonData["items"][i]["settings"].value("onColor", "").c_str());
			strcpy_s(m_data_item[i].settings.t3EntryDisplayField, JSON_STRING_LENGTH, jsonData["items"][i]["settings"].value("t3EntryDisplayField", "").c_str());
			strcpy_s(m_data_item[i].settings.textAlign, JSON_STRING_LENGTH, jsonData["items"][i]["settings"].value("textAlign", "").c_str());
			strcpy_s(m_data_item[i].settings.textColor, JSON_STRING_LENGTH, jsonData["items"][i]["settings"].value("textColor", "").c_str());
			strcpy_s(m_data_item[i].settings.titleColor, JSON_STRING_LENGTH, jsonData["items"][i]["settings"].value("titleColor", "").c_str());

			m_data_item[i].settings.blink = jsonData["items"][i]["settings"].value("blink", 0);
			m_data_item[i].settings.blinkInterval = jsonData["items"][i]["settings"].value("blinkInterval", 0);

			m_data_item[i].t3_Entrys.pid = jsonData["items"][i]["t3Entry"].value("pid", bac_gloab_panel);
			m_data_item[i].t3_Entrys.index = jsonData["items"][i]["t3Entry"].value("index", 0);

			char temp_type[10];
			memset(temp_type, 0, 10);
			strcpy_s(temp_type, JSON_STRING_LENGTH, jsonData["items"][i]["t3Entry"].value("type", "").c_str());
			if (strcmp(temp_type, "INPUT") == 0)
				m_data_item[i].t3_Entrys.ntype = BAC_IN;
			else if (strcmp(temp_type, "OUTPUT") == 0)
				m_data_item[i].t3_Entrys.ntype = BAC_OUT;
			else if (strcmp(temp_type, "VARIABLE") == 0)
				m_data_item[i].t3_Entrys.ntype = BAC_VAR;
			else if (strcmp(temp_type, "PROGRAM") == 0)
				m_data_item[i].t3_Entrys.ntype = BAC_PRG;
			else if (strcmp(temp_type, "SCHEDULE") == 0)
				m_data_item[i].t3_Entrys.ntype = BAC_SCH;
			else if (strcmp(temp_type, "HOLIDAY") == 0)
				m_data_item[i].t3_Entrys.ntype = BAC_HOL;
			else if (strcmp(temp_type, "GRP") == 0)
				m_data_item[i].t3_Entrys.ntype = BAC_GRP;
			else if (strcmp(temp_type, "MON") == 0)
				m_data_item[i].t3_Entrys.ntype = BAC_AMON;
			else 
				m_data_item[i].t3_Entrys.ntype = BAC_IN;
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


#endif // ! DISABLE_HANDLE_JSON_DATA

#pragma endregion