#pragma once



#define FLEXGRID_CELL_COLOR						13421772
#define FLEXGRID_CELL_GRAY_COLOR				13421772
#define NET_WORK_SLEEP_BETWEEN_WRITE_READ			Sleep(50);
#define NET_WORK_SET_TIMER							SetTimer(1,60000,NULL);
#define NET_WORK_KILL_TIMER							KillTimer(1);
#define FLEXGRID_CELL_WHITE_COLOR				0

#define EXIT_BUTTON_GET_FOCUS					m_exit_button.SetFocus();
// WeeklyRout_InsertDia 对话框
// Weekly_Routines 对话框
#define GRID_DOWN_MOVE								if(lRow==m_FlexGrid.get_Rows()-1) \
													{ \
														if(m_FlexGrid.get_RowIsVisible(1)) \
														{ \
															on_select() ; \
															return ; \
														} \
														int top_row=0; \
														top_row = m_FlexGrid.get_TopRow(); \
														m_FlexGrid.put_TopRow(top_row+1); \
													} \
													else if(!m_FlexGrid.get_RowIsVisible(lRow+1)) \
													{ \
														int top_row=0; \
														top_row = m_FlexGrid.get_TopRow(); \
														m_FlexGrid.put_TopRow(top_row+1); \
													}
#define GRID_UP_MOVE								if(!m_FlexGrid.get_RowIsVisible(lRow)) \
													{ \
														int top_row=0; \
														top_row = m_FlexGrid.get_TopRow(); \
														m_FlexGrid.put_TopRow(top_row-1); \
													}
#define MESSAGE_DIRECTION_CONTROL					if(pMsg->wParam == VK_RETURN ) \
													{ \
														EXIT_BUTTON_GET_FOCUS \
														return true; \
													} \
													else if(pMsg->wParam== VK_TAB) \
													{ \
														EXIT_BUTTON_GET_FOCUS \
														if(::GetKeyState(VK_SHIFT)<0) \
															shift_tab_move(); \
														else \
															tab_move(); \
														return true; \
													} \
													else if(pMsg->wParam== VK_LEFT) \
													{ \
														if(::GetKeyState(VK_SHIFT)<0) \
														{ \
															EXIT_BUTTON_GET_FOCUS \
															left_move(); \
															return true; \
														} \
														else \
														{ \
															if(::GetFocus()==m_Change.GetSafeHwnd() ) \
																return CDialog::PreTranslateMessage(pMsg); \
															else \
															{ \
																EXIT_BUTTON_GET_FOCUS \
																left_move(); \
																return true; \
															} \
														} \
													} \
													else if(pMsg->wParam== VK_RIGHT) \
													{ \
													 	if(::GetKeyState(VK_SHIFT)<0) \
														{ \
															EXIT_BUTTON_GET_FOCUS \
															right_move(); \
															return true; \
														} \
														else \
														{ \
															if(::GetFocus()==m_Change.GetSafeHwnd() ) \
																return CDialog::PreTranslateMessage(pMsg); \
															else \
															{ \
																EXIT_BUTTON_GET_FOCUS \
																right_move(); \
																return true; \
															} \
														} \
													} \
													else if(pMsg->wParam== VK_UP) \
													{ \
														if(::GetKeyState(VK_SHIFT)<0) \
														{ \
															EXIT_BUTTON_GET_FOCUS \
															up_move(); \
															return true; \
														} \
														else \
														{ \
															if(::GetFocus()==m_combo_col_row.GetSafeHwnd()) \
																return CDialog::PreTranslateMessage(pMsg); \
															else \
															{ \
																EXIT_BUTTON_GET_FOCUS \
																up_move(); \
																return true; \
															} \
														} \
													} \
													else if(pMsg->wParam== VK_DOWN) \
													{ \
														if(::GetKeyState(VK_SHIFT)<0) \
														{ \
															EXIT_BUTTON_GET_FOCUS \
															down_move(); \
															return true; \
														} \
														else \
														{ \
															if(::GetFocus()==m_combo_col_row.GetSafeHwnd()) \
																return CDialog::PreTranslateMessage(pMsg); \
															else \
															{ \
																EXIT_BUTTON_GET_FOCUS \
																down_move(); \
																return true; \
															} \
														} \
													} 
