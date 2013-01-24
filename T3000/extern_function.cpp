#include "stdafx.h"
#include "extern_function.h"
#include "T3000.h"

#include "global_variable_extern.h"


#if 0
bool binary_search_crc(int a)
{//use this for binary search ,inspect
	static int for_binary_search_crc=0;//retry times 10;when return value is -2
	if(a==-2)
	{
		if(for_binary_search_crc<10)
			for_binary_search_crc++;
		else
		{
			for_binary_search_crc=0;
			return true;/////////////////more ten time
		}
	}
	else
		for_binary_search_crc=0;
	return false;
}
bool can_be_writed_hex_file(int product_model,int hex_file_product_model)
{
	//product model
	// T3-8IO-------------20
	// T3-32I-------------22
	// T3-8i/60-----------23
	// Flexdriver---------25
	//Tstat5A-------------2
	//Tstat5B-------------1
	//Tstat5B2------------3
	//Tstat5C-------------4
	//Tstat5D-------------12
	//Solar---------------30
	//hex_file_product_model parameter is the hex_file_register 0x100 (256)
	//	if (product_model==18||product_model==17)
	{
		return true;
	}
	if(hex_file_product_model==255)//////////////old version hex file,before 2005.11.15
		return true;
	if(product_model<=TSTAT_PRODUCT_MODEL && hex_file_product_model<=TSTAT_PRODUCT_MODEL)
		return true;
	if(product_model==LED_PRODUCT_MODEL && hex_file_product_model==LED_PRODUCT_MODEL)
		return true;
	if(product_model==NET_WORK_CONT_PRODUCT_MODEL && hex_file_product_model==NET_WORK_CONT_PRODUCT_MODEL)
		return true;
	if(product_model==T3_8IOA_PRODUCT_MODEL && hex_file_product_model==T3_8IOA_PRODUCT_MODEL)
		return true;
	if(product_model==T3_8IOD_PRODUCT_MODEL && hex_file_product_model==T3_8IOD_PRODUCT_MODEL)
		return true;
	if(product_model==T3_32I_PRODUCT_MODEL && hex_file_product_model==T3_32I_PRODUCT_MODEL)
		return true;
	if(product_model==T3_8I_16O_PRODUCT_MODEL && hex_file_product_model==T3_8I_16O_PRODUCT_MODEL)
		return true;
	if(product_model==SOLAR_PRODUCT_MODEL && hex_file_product_model==SOLAR_PRODUCT_MODEL)
		return true;
	if(product_model==ZIGBEE_PRODUCT_MODEL && hex_file_product_model==ZIGBEE_PRODUCT_MODEL)
		return true;	
	return false;
}
CString get_product_name_by_product_model(int product_model)
{
	CString return_str;
	if(product_model>0 && product_model<=TSTAT_PRODUCT_MODEL)
		product_model=TSTAT_PRODUCT_MODEL;
	switch(product_model)
	{
	case 19:return_str="Tstat";break;
	case 20:return_str="T3-8IO";break;
	case 22:return_str="T3-32I";break;
	case 23:return_str="T3-8i/60";break;
	case 25:return_str="Flexdriver";break;
	case 30:return_str="Solar";break;
	case ZIGBEE_PRODUCT_MODEL:return_str="ZigBee";break;
	default:return_str="Unknown";break;
	}
	return return_str;
}
//down is for binary search thread
int product_size()//get the total product size 
{//get the total product size 
	return (m_binary_search_product_background_thread.size());
}
bool found_same_net_work_controller_by_mac(int id)
{//because net cable no baudrate,so inspect here
	if(read_one(id,7)!=NET_WORK_CONT_PRODUCT_MODEL)//it is not a net work controller
		return false;
	unsigned short get_mac_address[6]={0};
	Read_Multi(id,get_mac_address,100,6);
	for(int j=0;j<m_mac_address_list.size();j++)
	{
		if(get_mac_address[0]==m_mac_address_list.at(j).mac1 && 
			get_mac_address[1]==m_mac_address_list.at(j).mac2 && 
			get_mac_address[2]==m_mac_address_list.at(j).mac3 && 
			get_mac_address[3]==m_mac_address_list.at(j).mac4 &&  
			get_mac_address[4]==m_mac_address_list.at(j).mac5 && 
			get_mac_address[5]==m_mac_address_list.at(j).mac6)
		return true;//////////////////only here return true
	}
	return false;
}
void keep_back_mac_address(int id)
{
	unsigned short get_mac_address[6]={0};
	Read_Multi(id,get_mac_address,100,6);
	mac_address_list temp;
	temp.the_id=id;
	temp.mac1=get_mac_address[0];
	temp.mac2=get_mac_address[1];
	temp.mac3=get_mac_address[2];
	temp.mac4=get_mac_address[3];
	temp.mac5=get_mac_address[4];
	temp.mac6=get_mac_address[5];
	m_mac_address_list.push_back(temp);
}
void binarySearchforview(BYTE devLo, BYTE devHi)
{
//	had_scaned=true;////////////////////////////////////////////////had scan
	if(net_work_is_exist_or_not==true)
		Sleep(200);	
	else
		Sleep(10);//
	int a=CheckTstatOnline(devLo,devHi);

	TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);
	if(binary_search_crc(a))
		return ;
	char c_array_temp[5]={'0'};
	CString temp="";
	if(a>0)
	{
		int ntempID=0;
		BOOL bFindSameID=false;
		int nPos=-1;

		binary_search_result temp;
		temp.baudrate=m_baudrate2;
		unsigned short SerialNum[9];
		memset(SerialNum,0,sizeof(SerialNum));
		int nRet=0;
		nRet=Read_Multi(a,&SerialNum[0],0,9,2);
		if(nRet>0)
		{
			//serial=SerialNum[0]&0x00ff+(SerialNum[0]&0xff00>>8)*256+(SerialNum[1]&0x00ff)*65536
			//	+(SerialNum[1]&0xff00>>8)*16777216;
			temp.id=a;
			temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
			temp.product_class_id=SerialNum[7];
			temp.hardware_version=SerialNum[8];

				float tstat_version2;
			tstat_version2=SerialNum[4];//tstat version			
			if(tstat_version2 >=240 && tstat_version2 <250)
				tstat_version2 /=10;
			else 
			{
				tstat_version2 = SerialNum[5]*256+SerialNum[4];	
				tstat_version2 /=10;
			}//tstat_version
			
			temp.software_version=tstat_version2;
				if(read_one(temp.id,185)==0)
						temp.baudrate=9600;
					else
					{
							temp.baudrate=19200;
					}
					temp.nEPsize=read_one(temp.id,326);
				if(temp.serialnumber>0)
				m_binary_search_product_background_thread.push_back(temp);
		}
		else
			return;

	}
	switch(a)
	{
	case -2:
		//crc error
		if(devLo!=devHi)
		{
			binarySearchforview(devLo,(devLo+devHi)/2);
			binarySearchforview((devLo+devHi)/2+1,devHi);
		}
		else
			binarySearchforview(devLo,devHi);
		break;
	case -3:
		//more than 2 Tstat is connect
		if(devLo!=devHi)
		{
			binarySearchforview(devLo,(devLo+devHi)/2);
			binarySearchforview((devLo+devHi)/2+1,devHi);
		}
		else
		{//Two Tstat have the same ID,fewness
			do
			{
				Sleep(20);//////////////////////////////////for running is better
				char c_temp_arr[100]={'\0'};
				if(Read_One(devLo,10)!=-2)//one times
				{
					CString str_temp;
					for(int j=254;j>=1;j--)
						if(j!=devLo)
						{							
							if(!found_same_net_work_controller_by_mac(a))
							{
								bool find=false;//false==no find;true==find
								for(int w=0;w<m_binary_search_product_background_thread.size();w++)
									if(j==m_binary_search_product_background_thread.at(w).id)
									{
										find=true;
										break;
									}
								if(find==false)
								{
									//************************change the Id
//									Sleep(20);//////////////////////////////////for running is better
									if(Write_One(devLo,10,j)>0)//sometimes write failure ,so inspect,important
										if(j<devLo)
										{
											/*
											binary_search_result temp;
											temp.baudrate=m_baudrate2;
											temp.id=j;
											temp.product_class_id=read_one(j,7);
											get_serialnumber(temp.serialnumber,j);
											temp.hardware_version=read_one(j,8);
											m_binary_search_product_background_thread.push_back(temp);
										
											*/

												binary_search_result temp;
												temp.baudrate=m_baudrate2;
												unsigned short SerialNum[9];
												memset(SerialNum,0,sizeof(SerialNum));
												int nRet=0;
												temp.id=j;
												nRet=Read_Multi(j,&SerialNum[0],0,9,2);
												if(nRet>0)
												{
													//serial=SerialNum[0]&0x00ff+(SerialNum[0]&0xff00>>8)*256+(SerialNum[1]&0x00ff)*65536
													//	+(SerialNum[1]&0xff00>>8)*16777216;
												
													temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
													temp.product_class_id=SerialNum[7];
													temp.hardware_version=SerialNum[8];

														float tstat_version2;
													tstat_version2=SerialNum[4];//tstat version			
													if(tstat_version2 >=240 && tstat_version2 <250)
														tstat_version2 /=10;
													else 
													{
														tstat_version2 = SerialNum[5]*256+SerialNum[4];	
														tstat_version2 /=10;
													}//tstat_version
													
													temp.software_version=tstat_version2;
															if(read_one(temp.id,185)==0)
														temp.baudrate=9600;
													else
													{
															temp.baudrate=19200;
													}

													temp.nEPsize=read_one(temp.id,326);
											
													if(temp.serialnumber>=0)
													{
															
															m_binary_search_product_background_thread.push_back(temp);
													}
													
												}

										}
									binarySearchforview(devLo,devHi);
									return;
								}
							}
							else
							{
								return;
							}
						}
				}
			}while(1);
		}
		break;
	case -4:break;
		//no connection 
	case -5:break;
		//the input error
	}
}

void change_com_port_and_net_work_controller_baudrate(int new_baudrate)
{
	TRACE("%d\n",new_baudrate);
	static int now_baudrate=0;
	if(now_baudrate!=new_baudrate)
		now_baudrate=new_baudrate;
	else
		return;
	showing_text=T3000VIEW_SCAN_CHANGE_NET_WORK_BAUDRATE;//for showing 
	int baudrate=19200;
	if(new_baudrate!=19200)
		baudrate=9600;
	bool found=false;
	for(int i=0;i<m_binary_search_product_background_thread.size();i++)
	{
		if(NET_WORK_CONT_PRODUCT_MODEL==m_binary_search_product_background_thread.at(i).product_class_id)
		{
			found=true;

			//Change_BaudRate(m_binary_search_product_background_thread.at(i).baudrate);/////////change to same baudrate,so next step ,read,write
			if(baudrate==9600)
				write_one(m_binary_search_product_background_thread.at(i).id,128,0);
			else
				write_one(m_binary_search_product_background_thread.at(i).id,128,1);
			write_one(m_binary_search_product_background_thread.at(i).id,131,1);/////////save nemo
		}
	}
	Change_BaudRate(baudrate);
	if(found==true)
		Sleep(4000);//////////it 's better for next step
	else
		now_baudrate=0;///////////for no net work controller
	showing_text="";//for showing 
}
void turn_all_network_controller_state(int new_state)
{
	return;
	/*
	if(new_state==0xa1)
	{
	//	write_one(255,16,0xa1,1);
	//	return;
	}
	if(net_work_is_exist_or_not==false)
		return;//no net work controller, so return
	int j=0;
	for(int i=0;i<m_binary_search_product_background_thread.size();i++)
		if(NET_WORK_CONT_PRODUCT_MODEL==m_binary_search_product_background_thread.at(i).product_class_id)
		{
			do{
				j++;
				if(write_one(m_binary_search_product_background_thread.at(i).id,16,0xa2,1)>0)
					break;				
			}while(j<10);
		}
	//	TRACE("%d-------------------\n",j);
	*/

}
void scan_first_time()
{
	int netcontrols;
	
		m_baudrate2=19200;
	Change_BaudRate(19200);
	binarySearchforview();//***************************************scan
	


	/*
	netcontrols=m_binary_search_networkcontroller_background_thread.size();
	if(netcontrols>0)
	{
		int xx=m_binary_search_product_background_thread.size();
		for(int i=0;i<m_binary_search_product_background_thread.size();i++)
		{
			if(m_binary_search_product_background_thread.at(i).baudrate !=default_baudrate &&m_binary_search_product_background_thread.at(i).product_class_id!=NET_WORK_CONT_PRODUCT_MODEL)
			{
				change_com_port_and_net_work_controller_baudrate(m_binary_search_product_background_thread.at(i).baudrate);
				switch(default_baudrate)
				{
				case 9600:write_one(m_binary_search_product_background_thread.at(i).id ,128,0,1);break;
				case 19200:write_one(m_binary_search_product_background_thread.at(i).id ,128,1,1);break;
				}
				m_binary_search_product_background_thread.at(i).baudrate=default_baudrate;
			}
			write_one(m_binary_search_product_background_thread.at(i).id,131,1);
		}
	}
	*/

	if (gCommunicationType==0)
	{
		m_baudrate2=9600;
	Change_BaudRate(9600);
	binarySearchforview();

	}

}
void scan_second_time()
{

	m_baudrate2=19200;
//	turn_all_network_controller_state(0xa1);//turn to 0xa1 state
	change_com_port_and_net_work_controller_baudrate(19200);
//	Change_BaudRate(19200);
//	TRACE("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	turn_all_network_controller_state(0xa2);//turn to 0xa2 state
	binarySearchforview();//***************************************scan
	m_baudrate2=9600;
//	turn_all_network_controller_state(0xa1);//turn to 0xa1 state
	change_com_port_and_net_work_controller_baudrate(9600);
	turn_all_network_controller_state(0xa2);//turn to 0xa2 state
	binarySearchforview();
}
bool found_net_work()
{//return value ;true == found a net work controller;false == no existed net work controller
	for(int i=0;i<m_binary_search_product_background_thread.size();i++)
		if(NET_WORK_CONT_PRODUCT_MODEL==m_binary_search_product_background_thread.at(i).product_class_id)
		{
			net_work_is_exist_or_not=true;
			return true;
		}
	net_work_is_exist_or_not=false;
	return false;
}
void keep_back_net_work()
{//keep back the net work,and turn off ,write a2
	int i=0;//for next step 
	vector <binary_search_result> ::iterator iter;//existed picture
	iter=m_binary_search_product_background_thread.begin();
	for(i=0;i<m_binary_search_product_background_thread.size();i++,iter++)
	if(NET_WORK_CONT_PRODUCT_MODEL!=m_binary_search_product_background_thread.at(i).product_class_id)
		m_binary_search_product_background_thread.erase(iter);//////////////delete
	else
	{
		Change_BaudRate(m_binary_search_product_background_thread.at(i).baudrate);
		write_one(m_binary_search_product_background_thread.at(i).id,16,0xa2);//turn to a2 mode
	}
}
void background_binarysearch()
{
	m_binary_search_product_background_thread.clear();////////^0^

	for(int k=0;k<m_binary_search_networkcontroller_background_thread.size();k++)
	{
		m_binary_search_product_background_thread.push_back(m_binary_search_networkcontroller_background_thread.at(k));
	}
	m_mac_address_list.clear();////////^0^
	net_work_is_exist_or_not=false;////////^
	scan_first_time();

	/*
	if(found_net_work())
	{
		keep_back_net_work();//keep back net work and turn net work to a2 mode
		scan_second_time();
	}
	*/

	/*
	int xx=m_binary_search_product_background_thread.size();
	for(int i=0;i<m_binary_search_product_background_thread.size();i++)
	{
		if(m_binary_search_product_background_thread.at(i).baudrate !=default_baudrate &&m_binary_search_product_background_thread.at(i).product_class_id!=NET_WORK_CONT_PRODUCT_MODEL)
		{
		//	turn_all_network_controller_state(0xa1);//turn to 0xa1 state
			change_com_port_and_net_work_controller_baudrate(m_binary_search_product_background_thread.at(i).baudrate);
		//	turn_all_network_controller_state(0xa2);//turn to 0xa2 state
			switch(default_baudrate)
			{
			case 9600:write_one(m_binary_search_product_background_thread.at(i).id ,185,0,1);break;
			case 19200:write_one(m_binary_search_product_background_thread.at(i).id ,185,1,1);break;
			}
			m_binary_search_product_background_thread.at(i).baudrate=default_baudrate;
		}
	}
	*/

	//m_binary_search_networkcontroller_background_thread
	//如果Tstate的ID和networkcontroller 的ID相同，则修改Tstate的ID.
	/*
	for(int i=0;i<m_binary_search_product_background_thread.size();i++)
	{
		for(int k=0;k<m_binary_search_networkcontroller_background_thread.size();k++)
		{
			int ntempID=find_Address_towrite();
			if(m_binary_search_product_background_thread.at(i).id==m_binary_search_networkcontroller_background_thread.at(i).id)
			{
				write_one(m_binary_search_product_background_thread.at(i).id,6,ntempID);
				m_binary_search_product_background_thread.at(i).id=ntempID;
			}
		}
	}
	*/
	
}
int get_hex_file_data(unsigned char *p,CString hex_file_path)
{
	CFile m_hex_file;//the hex file
	for(long ltemp=0;ltemp<65535;ltemp++)
		p[ltemp]=255;
	unsigned int the_max_register_number=0;//the max register number to write from hex file
	if(m_hex_file.Open(hex_file_path,CFile::modeRead | CFile::shareDenyNone))
	{
		CArchive ar(&m_hex_file,CArchive::load);
		CString m_get_data_from_file;
///////////*****************inspect the file*********************
		unsigned int ii=0;
		while(NULL!=ar.ReadString(m_get_data_from_file))//循环读取文件，直到文件结束
		{//get a line from the file,check "crc" for total file
			ii++;//the line number that the wrong hex file;
			char a[256]={'a'};
			TS_UC get_hex[128]={0};//get hex data,it is get from the line char
								//the number is (i-1)
			int ddd=m_get_data_from_file.GetLength();
			for(int i=1;i<ddd;i++)//get a line
				a[i-1]=m_get_data_from_file.GetAt(i);
			if(strlen(a)%2==0)
				turn_hex_file_line_to_unsigned_char(a);//turn every char to int 
			else
			{
//				char p_c_temp[74]={'\0'};
//				sprintf(p_c_temp,"Error: the hex file had error at %d line!",ii);
//				AfxMessageBox(p_c_temp);
				return -2;
			}
			turn_int_to_unsigned_char(a,i-1,get_hex);//turn to hex 
			if(get_hex[3]==1)//for to seektobegin() function,because to end of the file
				break;

			TS_UC uctemp2=0;
			for(int j=0;j<i/2;j++)
				uctemp2+=get_hex[j];
			if(uctemp2%256!=0)
			{
//				char p_c_temp[74]={'\0'};
//				sprintf(p_c_temp,"Error: the hex file had error at %d line!",ii);
//				AfxMessageBox(p_c_temp);				
				return -2;
			}
			if(get_hex[1]==0 && get_hex[2]==0)
				get_hex[4]=255;//for the 0000 register to write 255
			unsigned int ltemp;
			ltemp=get_hex[1]*256+get_hex[2];
			for(j=0;j<get_hex[0];j++)
				p[ltemp+j]=get_hex[4+j];//get the data
			if(the_max_register_number<(ltemp+get_hex[0]))
				the_max_register_number=ltemp+get_hex[0]-1;
		}//end while
		//flash
		ar.Close();//close the CArchive
		m_hex_file.Close();//close the file		
//*******************up is to get the register value from the file***************
		return the_max_register_number;
	}
	else
		return -3;//open hex file failure
}
void background_binarysearch_TCP()
{
	m_binary_search_product_background_thread.clear();////////^0^

	for(int j=0;j<m_binary_search_networkcontroller_background_thread.size();j++)
	{
		m_binary_search_product_background_thread.push_back(m_binary_search_networkcontroller_background_thread.at(j));
	}
	m_mac_address_list.clear();////////^0^
	net_work_is_exist_or_not=false;////////^
	scan_first_time_TCP();



	if(found_net_work())
	{
		//keep_back_net_work_TCP();//keep back net work and turn net work to a2 mode
		//scan_second_time_TCP();
	}
}
void scan_first_time_TCP()
{
	//turn_all_network_controller_state(0xa1);//turn to 0xa1 state
	binarySearchforview();//***************************************scan
}
void scan_second_time_TCP()
{
	//m_baudrate2=19200;
	//turn_all_network_controller_state(0xa1);//turn to 0xa1 state
//	change_com_port_and_net_work_controller_baudrate(19200);
	turn_all_network_controller_state(0xa2);//turn to 0xa2 state

	binarySearchforview();//***************************************scan
	//m_baudrate2=9600;
	//turn_all_network_controller_state(0xa1);//turn to 0xa1 state
//	change_com_port_and_net_work_controller_baudrate(9600);
	//turn_all_network_controller_state(0xa2);//turn to 0xa2 state
	//binarySearchforview();
}
void keep_back_net_work_TCP()
{
	//keep back the net work,and turn off ,write a2
	int i=0;//for next step 
	vector <binary_search_result> ::iterator iter;//existed picture
	iter=m_binary_search_product_background_thread.begin();
	for(i=0;i<m_binary_search_product_background_thread.size();i++,iter++)
		if(NET_WORK_CONT_PRODUCT_MODEL!=m_binary_search_product_background_thread.at(i).product_class_id)
			m_binary_search_product_background_thread.erase(iter);//////////////delete
		else
		{
			//Change_BaudRate(m_binary_search_product_background_thread.at(i).baudrate);
	//		write_one(m_binary_search_product_background_thread.at(i).id,16,0xa2);//turn to a2 mode
		}
}
BOOL ScanOnlyOneProduct()
{
	return FALSE;
	int nProductID=Read_One(255,MB_Product_Model); //global scan to 255, read register 6 (model number) to see if there is any nodes on the net
	//TBD: add a register .h file for all modbus registeers
	if (nProductID<0||nProductID>254)
	{
		return FALSE;
	}
	else
	{
		int n=read_one(nProductID,188);
		if (n!=0||n!=1)
		{
		//	return FALSE;
		}
		binary_search_result temp;
		if (n==1)
		{
			temp.baudrate=19200;
		}
		if (n==0)
		{
			temp.baudrate=9600;
		}

		for(int j=0;j<m_binary_search_product_background_thread.size();j++)
		{
			if(nProductID==m_binary_search_product_background_thread.at(j).id)
			{
				return TRUE;//The product has been in datebase.
			}
		}

		//////add to datebase
		///....

		temp.id=nProductID;
		temp.product_class_id=read_one(nProductID,7);
//		get_serialnumber(temp.serialnumber,temp.id );
		temp.hardware_version=read_one(temp.id ,8);
		m_binary_search_product_background_thread.push_back(temp);
		if(read_one(nProductID,7)==NET_WORK_CONT_PRODUCT_MODEL)//net work controller
			keep_back_mac_address(nProductID);
	}
	return TRUE;
}


//change back the product if it has been changed when scane.
void RechangeProductID()
{
	return;//改函数是为了把改变了的产品ID 改变回来，存在一定的不确定性，所以去掉。
	tree_product ProductItemInDb;
	binary_search_result scanItem;
	int nProductInDB;
	int nScanItem;
	nProductInDB=m_product.size();
	nScanItem=m_binary_search_product_background_thread.size();
	for(int i=0;i<nProductInDB;i++)
	{
		ProductItemInDb=m_product.at(i);
		for(int k=0;k<nScanItem;k++)
		{
			scanItem=m_binary_search_product_background_thread.at(k);
			if (ProductItemInDb.serial_number==scanItem.serialnumber)
			{
				if (ProductItemInDb.product_id!=scanItem.id)
				{//此时的ProductItemInDb.product_id已经被改变,不再是数据库中的那个ID.
					//change the id;
					//find a temp Id;
					int nTempID=0;
					nTempID=find_Address_towrite();
					if (nTempID>0)
					{	
						//
						if (read_one(ProductItemInDb.product_id,6)>0)
						{
							//如果数据库中的的那个产品已经扫描到了,说明另一个Tstate已经占用了原来的产品号.
							/*
							if(write_one(ProductItemInDb.product_id,6,scanItem.id))
							{
							//m_product.at(i).product_id=scanItem.id;
							m_binary_search_product_background_thread.at(k).id=scanItem.id;
							}
							if(write_one(scanItem.id,6,nTempID))
							{
							m_binary_search_product_background_thread.at(k).id=nTempID;
							}
							*/
							int npos=findPos_prodctid_inList(ProductItemInDb.product_id);
							if (npos>=0)
							{
								if(write_one(ProductItemInDb.product_id,6,nTempID))
								{
									//m_product.at(i).product_id=scanItem.id;
									m_binary_search_product_background_thread.at(npos).id=nTempID;
								}

								if(write_one(scanItem.id,6,ProductItemInDb.product_id))
								{
									m_binary_search_product_background_thread.at(k).id=ProductItemInDb.product_id;
								}
							}
						}
						else
						{
							//数据库中的的那个产品的ProductID已经改变,原来的ID所代表的那个Tstate已经不存在了.
							if(write_one(scanItem.id,6,ProductItemInDb.product_id))
							{
								m_binary_search_product_background_thread.at(k).id=ProductItemInDb.product_id;
							}
						}
						/*
						BOOL bFind=FALSE;
						for(int j=0;j<nScanItem;j++)
						{
						if ()
						{
						}
						}
						*/
					}
				}
			}

		}
	}
}
int findPos_prodctid_inList(int nID)
{
	//return -1;
	int nsize=m_binary_search_product_background_thread.size();
	if (nsize<=0)
	{
		return -1;
	}
	for (int i=0;i<nsize;i++)
	{
		if (nID==m_binary_search_product_background_thread.at(i).id)
		{
			return i;
		}
	}
	return -1;
}
int find_Address_towrite()
{
	/*
	if (m_binary_search_product_background_thread.size()<=0)
	{
		return 254;
	}
	bool find=TRUE;
	for(int j=254;j>=1;j--)
	{
		for(int i=0;i<m_binary_search_product_background_thread.size();i++)
		{
			if(j==m_binary_search_product_background_thread.at(i).id)
			{//find same id,because the baudrate is different
				find=true;
				break;
			}
			else
			{
				find=FALSE;
				continue;
			}
		}
		if (find)
		{
			continue;
		}
		if (!find)
		{
			return j;
		}
	}
	return -1;
	*/

	if (m_product.size()<=0)
	{
		return 1;
	}
	bool find=TRUE;
	for(int j=1;j<=254;j++)
	{

		for(int i=0;i<m_product.size();i++)
		{
			if(j==m_product.at(i).product_id)
			{//find same id,because the baudrate is different
				find=true;
				break;
			}
			else
			{
				find=FALSE;
				continue;
			}
		}
		if (find)
		{
			continue;
		}
		if (!find)
		{
			return j;
		}
	}
	return -1;
}
void binarySearchforview_networkcontroller(BYTE devLo, BYTE devHi)
{
	//	had_scaned=true;////////////////////////////////////////////////had scan
	if(net_work_is_exist_or_not==true)
		Sleep(200);	    
	else
		Sleep(10);//
	int a=NetController_CheckTstatOnline(devLo,devHi);

	TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);
	if(binary_search_crc(a))
		return ;
	char c_array_temp[5]={'0'};
	CString temp="";
	if(a>0)
	{

			///////****************************************************************8888888
			//			TRACE("L:%d   H:%d\n",devLo,devHi);
			//			TRACE("%d\n",a);
		/*
#if 1
			binary_search_result temp;
			temp.baudrate=m_baudrate2;
			temp.id=a;
			temp.product_class_id=read_one(a,7);
			get_serialnumber(temp.serialnumber,temp.id );
			temp.hardware_version=read_one(temp.id ,8);
			m_binary_search_networkcontroller_background_thread.push_back(temp);
			if(read_one(a,7)==NET_WORK_CONT_PRODUCT_MODEL)//net work controller
				keep_back_mac_address(a);
#endif
				*/
			binary_search_result temp;
			temp.baudrate=m_baudrate2;
			unsigned short SerialNum[9];
			memset(SerialNum,0,sizeof(SerialNum));
			int nRet=0;
			temp.id=a;
			nRet=Read_Multi(temp.id,&SerialNum[0],0,9,3);
			if(nRet>0)
			{
				//serial=SerialNum[0]&0x00ff+(SerialNum[0]&0xff00>>8)*256+(SerialNum[1]&0x00ff)*65536
				//	+(SerialNum[1]&0xff00>>8)*16777216;

				if(SerialNum[0]==255&&SerialNum[1]==255&&SerialNum[2]==255&&SerialNum[3]==255)
				{
					srand((unsigned)time(NULL)); 
					SerialNum[0]=rand()%255; 
					SerialNum[1]=rand()%255; 
					SerialNum[2]=rand()%255; 
					SerialNum[3]=rand()%255; 

					write_one(temp.id,0,SerialNum[0]);
					write_one(temp.id,1,SerialNum[1]);
					write_one(temp.id,2,SerialNum[2]);
					write_one(temp.id,3,SerialNum[3]);
				}

				temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
				temp.product_class_id=SerialNum[7];
				temp.hardware_version=SerialNum[8];
			float tstat_version2;
			tstat_version2=SerialNum[4];//tstat version			
			if(tstat_version2 >=240 && tstat_version2 <250)
				tstat_version2 /=10;
			else 
			{
				tstat_version2 = SerialNum[5]*256+SerialNum[4];	
				tstat_version2 /=10;
			}//tstat_version
			
			temp.software_version=tstat_version2;
					if(read_one(temp.id,185)==0)
						temp.baudrate=9600;
					else
					{
							temp.baudrate=19200;
					}
			temp.nEPsize=read_one(temp.id,326);
			if(temp.serialnumber>0)
			m_binary_search_networkcontroller_background_thread.push_back(temp);
			}

	}
	switch(a)
	{
	case -2:
		//crc error
		if(devLo!=devHi)
		{
			binarySearchforview_networkcontroller(devLo,(devLo+devHi)/2);
			binarySearchforview_networkcontroller((devLo+devHi)/2+1,devHi);
		}
		else
			binarySearchforview_networkcontroller(devLo,devHi);
		break;
	case -3:
		//more than 2 Tstat is connect
		if(devLo!=devHi)
		{
			binarySearchforview_networkcontroller(devLo,(devLo+devHi)/2);
			binarySearchforview_networkcontroller((devLo+devHi)/2+1,devHi);
		}
		else
		{//Two Tstat have the same ID,fewness
			do
			{
				Sleep(20);//////////////////////////////////for running is better
				char c_temp_arr[100]={'\0'};
				if(Read_One(devLo,10)!=-2)//one times
				{
					CString str_temp;
					for(int j=254;j>=1;j--)
						if(j!=devLo)
						{							
							if(!found_same_net_work_controller_by_mac(a))
							{
								bool find=false;//false==no find;true==find
								for(int w=0;w<m_binary_search_product_background_thread.size();w++)
									if(j==m_binary_search_product_background_thread.at(w).id)
									{
										find=true;
										break;
									}
									if(find==false)
									{
										//************************change the Id
										//									Sleep(20);//////////////////////////////////for running is better
										if(Write_One(devLo,10,j)>0)//sometimes write failure ,so inspect,important
											if(j<devLo)
											{
												/*
												#if 1
												binary_search_result temp;
												temp.baudrate=m_baudrate2;
												temp.id=j;
												temp.product_class_id=read_one(j,7);
												get_serialnumber(temp.serialnumber,j);
												temp.hardware_version=read_one(j,8);
												m_binary_search_networkcontroller_background_thread.push_back(temp);
												if(read_one(j,7)==NET_WORK_CONT_PRODUCT_MODEL)//net work controller
													keep_back_mac_address(j);
												#endif
												*/

												binary_search_result temp;
												temp.baudrate=m_baudrate2;
												unsigned short SerialNum[9];
												memset(SerialNum,0,sizeof(SerialNum));
												int nRet=0;
												temp.id=j;
												nRet=Read_Multi(temp.id,&SerialNum[0],0,9,3);
												if(nRet>0)
												{
													//serial=SerialNum[0]&0x00ff+(SerialNum[0]&0xff00>>8)*256+(SerialNum[1]&0x00ff)*65536
													//	+(SerialNum[1]&0xff00>>8)*16777216;

														if(SerialNum[0]==255&&SerialNum[1]==255&&SerialNum[2]==255&&SerialNum[3]==255)
														{
															srand((unsigned)time(NULL)); 
															SerialNum[0]=rand()%255; 
															SerialNum[1]=rand()%255; 
															SerialNum[2]=rand()%255; 
															SerialNum[3]=rand()%255; 
															write_one(temp.id,0,SerialNum[0]);
															write_one(temp.id,1,SerialNum[1]);
															write_one(temp.id,2,SerialNum[2]);
															write_one(temp.id,3,SerialNum[3]);

														}

													temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
													temp.product_class_id=SerialNum[7];
													temp.hardware_version=SerialNum[8];
														float tstat_version2;
													tstat_version2=SerialNum[4];//tstat version			
													if(tstat_version2 >=240 && tstat_version2 <250)
														tstat_version2 /=10;
													else 
													{
														tstat_version2 = SerialNum[5]*256+SerialNum[4];	
														tstat_version2 /=10;
													}//tstat_version
													
													temp.software_version=tstat_version2;
															if(read_one(temp.id,185)==0)
													temp.baudrate=9600;
												else
												{
														temp.baudrate=19200;
												}
													temp.nEPsize=read_one(temp.id,326);
														
													if(temp.serialnumber>0)
													
													m_binary_search_networkcontroller_background_thread.push_back(temp);
												}



											}
											binarySearchforview_networkcontroller(devLo,devHi);
											return;
									}
							}
							else
							{
								return;
							}
						}
				}
			}while(1);
		}
		break;
	case -4:break;
		//no connection 
	case -5:break;
		//the input error
	}
}
void background_binarysearch_netcontroller()
{
	m_binary_search_networkcontroller_background_thread.clear();
	m_mac_address_list.clear();////////^0^
	net_work_is_exist_or_not=false;////////^

	m_baudrate2=19200;
	Change_BaudRate(19200);
	binarySearchforview_networkcontroller();//***************************************scan
	/*
	int xx=m_binary_search_networkcontroller_background_thread.size();
	for(int i=0;i<m_binary_search_networkcontroller_background_thread.size();i++)
	{
		if(m_binary_search_networkcontroller_background_thread.at(i).baudrate !=default_baudrate &&m_binary_search_networkcontroller_background_thread.at(i).product_class_id==NET_WORK_CONT_PRODUCT_MODEL)
		{

			switch(default_baudrate)
			{
			case 9600:write_one(m_binary_search_networkcontroller_background_thread.at(i).id ,128,0,2);break;
			case 19200:write_one(m_binary_search_networkcontroller_background_thread.at(i).id ,128,1,2);break;

			}

			Sleep(6*1000);
			write_one(m_binary_search_networkcontroller_background_thread.at(i).id,131,2);
			m_binary_search_networkcontroller_background_thread.at(i).baudrate=default_baudrate;
			SetPaneString("Please Wait,network controller is restarting. ");
		}
	}
	*/
	
	if (gCommunicationType==0)
	{
	m_baudrate2=9600;
	Change_BaudRate(9600);
	binarySearchforview_networkcontroller();
	}

}
void background_binarysearch_netcontroller_TCP()
{

	m_binary_search_networkcontroller_background_thread.clear();
	m_mac_address_list.clear();////////^0^
	net_work_is_exist_or_not=false;////////^


	binarySearchforview_networkcontroller();

/*

	int xx=m_binary_search_networkcontroller_background_thread.size();
	for(int i=0;i<m_binary_search_networkcontroller_background_thread.size();i++)
	{
		if(m_binary_search_networkcontroller_background_thread.at(i).baudrate !=default_baudrate &&m_binary_search_networkcontroller_background_thread.at(i).product_class_id==NET_WORK_CONT_PRODUCT_MODEL)
		{

			switch(default_baudrate)
			{
			case 9600:write_one(m_binary_search_networkcontroller_background_thread.at(i).id ,185,0,1);break;
			case 19200:write_one(m_binary_search_networkcontroller_background_thread.at(i).id ,185,1,1);break;
			}
			m_binary_search_networkcontroller_background_thread.at(i).baudrate=default_baudrate;
		}
	}
	Change_BaudRate(default_baudrate);
	m_baudrate2=19200;
	Change_BaudRate(19200);
	binarySearchforview_networkcontroller();//***************************************scan
*/

}
#endif
