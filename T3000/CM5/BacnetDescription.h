#pragma once
#include "..\global_function.h"
#include <algorithm>
class BacnetDescription
{
	public:
	BacnetDescription(void);

	~BacnetDescription(void);

	void GetAllPanels();

	HANDLE hwait_readdes_thread;
	static DWORD WINAPI  Read_BacnetPanelLable_Thread(LPVOID lpVoid);
	vector <Bacnet_RemotePoint_Info> m_bacnet_panel;
};

