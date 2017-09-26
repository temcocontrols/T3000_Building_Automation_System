#pragma once
class CT3ControllerProgram
{
public:
	CT3ControllerProgram();
	virtual ~CT3ControllerProgram();

	int GetProgramData(uint32_t deviceid, uint8_t start_instance, uint8_t end_instance, uint8_t npackgae);
	int GetProgramData_Blocking(uint32_t deviceid, uint8_t start_instance, uint8_t end_instance, uint8_t npackgae);
	/*int GetPrivateData(uint32_t deviceid, uint8_t command, uint8_t start_instance, uint8_t end_instance, int16_t entitysize);
	int GetPrivateData_Blocking(uint32_t deviceid, uint8_t command, uint8_t start_instance, uint8_t end_instance, int16_t entitysize);
*/
	int SaveProgramData(uint32_t deviceid, uint8_t n_command, uint8_t start_instance, uint8_t end_instance, uint8_t npackage);
	 
	bool Judge_FullLabel_Exsit(LPCTSTR m_new_fulllabel);
	bool Judge_Label_Exsit(LPCTSTR m_new_fulllabel);

	void SplitStringToArray(CStringArray &saArray, CString sSource, CString sToken);
	void Inial_Product_map();
	void Initial_Product();
};

