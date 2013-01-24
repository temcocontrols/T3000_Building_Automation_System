#ifndef EXTERN_FUNCTION_H
#define EXTERN_FUNCTION_H


#if 0
bool binary_search_crc(int a);
bool can_be_writed_hex_file(int product_model,int hex_file_product_model);
CString get_product_name_by_product_model(int product_model);
int product_size();//get the total product size 
bool found_same_net_work_controller_by_mac(int id);
void keep_back_mac_address(int id);
void binarySearchforview(BYTE devLo=1, BYTE devHi=254);
void change_com_port_and_net_work_controller_baudrate(int new_baudrate);
void turn_all_network_controller_state(int new_state);
void scan_first_time();
void scan_second_time();
bool found_net_work();
void keep_back_net_work();
void background_binarysearch();
int get_hex_file_data(unsigned char *p,CString hex_file_path);

void background_binarysearch_TCP();
void scan_first_time_TCP();
void scan_second_time_TCP();
void keep_back_net_work_TCP();
BOOL ScanOnlyOneProduct();

int find_Address_towrite();
void RechangeProductID();

int findPos_prodctid_inList(int nID);

void background_binarysearch_netcontroller();
void binarySearchforview_networkcontroller(BYTE devLo=1, BYTE devHi=254);
void background_binarysearch_netcontroller_TCP();

#endif

#endif