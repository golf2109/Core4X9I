/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.22                          *
*        Compiled Jul  4 2013, 15:16:01                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "header.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern GUI_CONST_STORAGE GUI_BITMAP bmbmp;
extern GUI_CONST_STORAGE GUI_BITMAP bmjpg;
extern GUI_CONST_STORAGE GUI_BITMAP bmpdf;
extern GUI_CONST_STORAGE GUI_BITMAP bmgif;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_ICON_ADD (GUI_ID_USER + 0x2C)
#define ID_ICON_DEL  (GUI_ID_USER + 0x2D)
#define ID_TREE_0     (GUI_ID_USER + 0x2E)
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x0E)
#define ID_SCROLLBAR_0     (GUI_ID_USER + 0x08)
#define ID_EDIT_0     (GUI_ID_USER + 0x09)
#define ID_BUTTON_EXIT     (GUI_ID_USER + 0x0A)
#define file 1
#define directory  0
uint8_t FlagIconDisable;
static TREEVIEW_Handle hTree;
TREEVIEW_ITEM_Handle hNode,hNode_Last;
EDIT_Handle hEdit;

void _readDIR(char *pPath);

BUTTON_Handle hButton;
extern void _drawJPG(char *Path,char *fn);
extern void _drawBMP(char *Path,char *fn);
extern void _drawGIF(char *Path,char *fn);

extern int _cbButtonEXIT(const WIDGET_ITEM_DRAW_INFO *pDrawItemInfo);

WM_HWIN hWin_SD;
extern WM_HWIN hWin_menu;
extern WM_HWIN BUTTON_KEY(WM_HWIN hWin);
extern ICONVIEW_Handle hALARMA,hALARMB,hIcon_EXIT,hIcon_BRIGHT;
 
extern RTC_DateTypeDef			RTC_Date;
char *pPath;
int FRAMEWIN_OFFSET_X;
int FRAMEWIN_OFFSET_Y;
TREEVIEW_ITEM_INFO pInfo;
//GUI_WIDGET_CREATE_INFO_struct {
//  GUI_WIDGET_CREATE_FUNC* pfCreateIndirect;
//  const char * pName;                    /* Text ... Not used on all widgets */
//  I16 Id;                                /* ID ... should be unique in a dialog */
//  I16 x0, y0, xSize, ySize;              /* Define position and size */
//  U16 Flags;                             /* Widget specific create flags (opt.) */
//  I32 Para;                              /* Widget specific parameter (opt.) */
//  U32 NumExtraBytes;                     /* Number of extra bytes usable with <WIDGET>_SetUserData & <WIDGET>_GetUserData */
//};

static const GUI_WIDGET_CREATE_INFO _aSD[] = {
  { FRAMEWIN_CreateIndirect, "SDCARD", ID_FRAMEWIN_0, 110, 0, 360, 272, 0, 0x0, 0 },
  { TREEVIEW_CreateIndirect, NULL, ID_TREE_0, 35, 0, 315, 220, 0, 0x0,1},
	{ EDIT_CreateIndirect, NULL, ID_EDIT_0, 0,225,350,20,0,256,0},
	{ ICONVIEW_CreateIndirect,NULL,ID_ICON_ADD,1,30,32,32,WM_CF_SHOW|WM_CF_HASTRANS,(30<<16)|30,0},
	{ ICONVIEW_CreateIndirect,NULL,ID_ICON_DEL,1,75,32,32,WM_CF_SHOW|WM_CF_HASTRANS,(30<<16)|30,0},
};

#if 1
WCHAR ff_convert (WCHAR wch, UINT dir)
{
	if (wch < 0x80) {
		/* ASCII Char */
		return wch;
	}

	/* I don't support unicode it is too big! */
	return 0;
}

WCHAR ff_wtoupper (WCHAR wch)
{
	if (wch < 0x80) {
		/* ASCII Char */
		if (wch >= 'a' && wch <= 'z') {
			wch &= ~0x20;
		}
		return wch;
	}

	/* I don't support unicode it is too big! */
	return 0;
}
#endif

/*********************************************************************
*
*       _readDIR
*/
void _readDIR(char *pPath){
	uint8_t mask,i;
	char *fn,*p;
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
    finfo.lfname = lfn;
    finfo.lfsize = sizeof lfn;
#endif	
	folders=0;files=0;
	mask=directory;
		for(i=0;i<2;i++)
		{ 
			fresult=f_opendir(&dir, pPath);
			for(;;)
			 {
				fresult=f_readdir(&dir,&finfo);
				if((fresult!=FR_OK)||(finfo.fname[0]==0)) 
					break;
				if(finfo.fname[0]=='.')										
					continue;
#if _USE_LFN
            fn = *finfo.lfname ? finfo.lfname : finfo.fname;
#else
            fn = finfo.fname;
#endif				
				
					
				if((finfo.fattrib&AM_DIR)&&(!mask)) 
					{
					hNode=TREEVIEW_InsertItem(hTree,TREEVIEW_ITEM_TYPE_NODE,hNode,TREEVIEW_INSERT_BELOW,fn/*finfo.fname*/);
					folders++;
					}
				else if((!(finfo.fattrib&AM_DIR))&&mask) 
					{	
					hNode=TREEVIEW_InsertItem(hTree,TREEVIEW_ITEM_TYPE_LEAF,hNode,TREEVIEW_INSERT_BELOW,fn/*finfo.fname*/);
					p=strchr(fn,'\0');
					p-=3;
					switch(*p){	
					case 'b':
						TREEVIEW_ITEM_SetImage(hNode, TREEVIEW_BI_LEAF,&bmbmp);	
					break;
					case 'j':
						TREEVIEW_ITEM_SetImage(hNode, TREEVIEW_BI_LEAF,&bmjpg);
					break;
					case 'p':
						TREEVIEW_ITEM_SetImage(hNode, TREEVIEW_BI_LEAF,&bmpdf);		
					break;
					case 'g':
						TREEVIEW_ITEM_SetImage(hNode, TREEVIEW_BI_LEAF,&bmgif);		
					break;
						}
					files++;
					}
				}
			mask=file;
				
		}	
		
}

static void _cbEXIT(WM_MESSAGE *pMsg){
	int     NCode;
  int     Id;
		switch (pMsg->MsgId) {
			case WM_TOUCH:
				hButton=WM_GetDialogItem(pMsg->hWin, ID_BUTTON_EXIT);
				WM_ShowWindow(hButton);
			break;	
			case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) {
					case ID_BUTTON_EXIT:
						switch(NCode){
							case WM_NOTIFICATION_RELEASED:
								hButton=WM_GetDialogItem(pMsg->hWin, ID_BUTTON_EXIT);
								time_show=1;
								WM_DeleteWindow(hButton);
								GUI_SetAlpha(0);
								GUI_SetBkColor(GUI_LIGHTBLUE);
								GUI_ClearRect(1,17,58,270);
								GUI_SetColor(GUI_YELLOW);
								GUI_DrawRect(0,16,59,271);
								GUI_SetBkColor(GUI_DARKBLUE);
								GUI_ClearRect(0,0,470,15);
								RTC_GetDate(RTC_Format_BIN, &RTC_Date);
								GUI_DispDecAt(RTC_Date.RTC_Date,5,0,2);
								GUI_DispString(".");
								GUI_DispDec(RTC_Date.RTC_Month,2);
								GUI_DispString(".20");
								GUI_DispDec(RTC_Date.RTC_Year,2);
								WM_ShowWindow(PROGBAR_MEM);
								WM_ShowWindow(hIcon_EXIT);
								WM_ShowWindow(hIcon_BRIGHT);
								WM_ShowWindow(hALARMA);
								WM_ShowWindow(hALARMB);
								WM_ShowWindow(hWin_menu);
							break;	
						}
					break;
				}
			break;
			default:
				WM_DefaultProc(pMsg);
			break;	
		}	
}

/*********************************************************************
*
*       _cbSD
*/
static void _cbSD(WM_MESSAGE * pMsg) {
 char Buff[40];
	ICONVIEW_Handle hIcon;
	WM_HWIN hWin;
	int     NCode;
  int     Id,i,m;
	switch (pMsg->MsgId) {
		case WM_DELETE:
			Path[0]='0';
			Path[1]=':';
			Path[2]='\0';
			hNode_Last=0;
			hNode=0;
			FRAMEWIN_OFFSET_X+=20;
			FRAMEWIN_OFFSET_Y-=20;
			hWin_SD=0;
			countSD_files_window--;                  // Decrement count window SD_files
		break;	
		case WM_INIT_DIALOG:
		hWin = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hWin, 20);
    FRAMEWIN_SetFont(hWin,GUI_FONT_16_1);
		FRAMEWIN_SetMoveable(hWin, 1);
		FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT,0);
		WM_MoveWindow(hWin,FRAMEWIN_OFFSET_X,FRAMEWIN_OFFSET_Y);		
		FRAMEWIN_OFFSET_X-=20;
		FRAMEWIN_OFFSET_Y+=20;
		
		
		hIcon = WM_GetDialogItem(pMsg->hWin, ID_ICON_ADD);
		ICONVIEW_SetFrame(hIcon,GUI_COORD_X,1);
		ICONVIEW_SetFrame(hIcon,GUI_COORD_Y,1);
		ICONVIEW_AddBitmapItem(hIcon,(const GUI_BITMAP*)(add_folder+1152),0);
	
		hIcon= WM_GetDialogItem(pMsg->hWin, ID_ICON_DEL);
		ICONVIEW_SetFrame(hIcon,GUI_COORD_X,1);
		ICONVIEW_SetFrame(hIcon,GUI_COORD_Y,1);	
		ICONVIEW_AddBitmapItem(hIcon,(const GUI_BITMAP*)(del_folder+1152),0);			
		
		
		hTree = WM_GetDialogItem(pMsg->hWin, ID_TREE_0);
		TREEVIEW_SetAutoScrollV(hTree,1);
		TREEVIEW_SetSelMode(hTree,TREEVIEW_SELMODE_ROW);
	
		Path[0]='0';
		Path[1]=':';
		Path[2]='\0';
		_readDIR(Path);
		hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
		EDIT_SetText(hEdit, Path);
		level_DIR[0]=0;
		TREEVIEW_SetUserData(hTree,level_DIR,1);
		break;	
		case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
			case ID_ICON_ADD:
				switch(NCode) {
					case WM_NOTIFICATION_RELEASED:
						hIcon = WM_GetDialogItem(pMsg->hWin, ID_ICON_ADD);
						WM_DisableWindow(hIcon);
						hIcon = WM_GetDialogItem(pMsg->hWin, ID_ICON_DEL);
						WM_DisableWindow(hIcon);
						BUTTON_KEY(pMsg->hWin);
					break;	
				}
			break;
			case ID_ICON_DEL:
				switch(NCode) {
					case WM_NOTIFICATION_RELEASED:
						hTree = WM_GetDialogItem(pMsg->hWin, ID_TREE_0);
						hEdit=WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
						hNode=TREEVIEW_GetSel(hTree);
						hIcon= WM_GetDialogItem(pMsg->hWin, ID_ICON_DEL);
						Buff[0]='/';
						TREEVIEW_ITEM_GetText(hNode,(unsigned char*)&Buff[1],40);
						strcat(Path,Buff);
						fresult=f_unlink(Path);
						EDIT_GetText(hEdit,Path,256);
						
						if(fresult==FR_OK)
						{
							TREEVIEW_ITEM_Delete(hNode);
							WM_SetFocus(hTree);
						}
						else if(fresult==FR_DENIED)
							GUI_MessageBox("DIRECTORY IS NOT EMPTY! ", "ERROR", 0);
						else if(fresult==FR_INVALID_NAME)
							GUI_MessageBox("INVALID DIRECTORY NAME! ", "ERROR", 0);
						break;	
				}
			break;	 
			case ID_TREE_0: // Notifications sent by 'Button'
      switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
					hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
					EDIT_GetText(hEdit, Path, 256);
				break;	
				case WM_NOTIFICATION_RELEASED:
				hTree = WM_GetDialogItem(pMsg->hWin, ID_TREE_0);
				hNode=TREEVIEW_GetSel(hTree);
				TREEVIEW_ITEM_GetInfo(hNode,&pInfo);
				hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
				if(hNode==hNode_Last)
						{							
							hNode_Last=0;
							TREEVIEW_ITEM_GetText(hNode,(unsigned char*)Buff,40);
							if(pInfo.IsNode==1)		 /* For open directory */
							{	
								//TREEVIEW_ITEM_GetText(hNode,(unsigned char*)Buff,40);
								if((Buff[0]=='.')&&(Buff[1]=='.'))
								{
									pPath=strchr(Path,'\0');
									pPath-=2;
									while(*pPath!='/')
										pPath--;
									*pPath='\0';
									EDIT_SetText(hEdit, Path);
									while(hNode)
									{	hNode=TREEVIEW_GetSel(hTree);	
										TREEVIEW_ITEM_Delete(hNode);
									}
									
									TREEVIEW_GetUserData(hTree,level_DIR,1);
									level_DIR[0]--;
									TREEVIEW_SetUserData(hTree,level_DIR,1);
									if(level_DIR[0]!=0)
											hNode=TREEVIEW_InsertItem(hTree,TREEVIEW_ITEM_TYPE_NODE,hNode,TREEVIEW_INSERT_ABOVE,"..");
									_readDIR(Path);
									
								}
								else
								{
									m=strlen(Buff);
									pPath=strchr(Path,'\0');
									*pPath++='/';
									for(i=0;i<m;i++)
										*pPath++=Buff[i];
									*pPath='\0';		
									hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
									EDIT_SetText(hEdit, Path);
									while(hNode)
									{
									hNode=TREEVIEW_GetSel(hTree);	
									TREEVIEW_ITEM_Delete(hNode);
									}
									hNode=TREEVIEW_InsertItem(hTree,TREEVIEW_ITEM_TYPE_NODE,hNode,TREEVIEW_INSERT_ABOVE,"..");
									TREEVIEW_GetUserData(hTree,level_DIR,1);
									level_DIR[0]++;
									TREEVIEW_SetUserData(hTree,level_DIR,1);
									_readDIR(Path);
								}
																
							}
							else     /* For open file*/
							{
								strcat(Path,"/");
								strcat(Path,Buff);
								pPath=strchr(Buff,'\0');
								pPath-=3;
								GUI_SetFont(&GUI_Font8x16);	
								if((*pPath=='j')||(*pPath=='J'))	
								{
									time_show=0;
									WM_HideWindow(hWin_menu);
									WM_HideWindow(hIcon_EXIT);
									WM_HideWindow(hIcon_BRIGHT);
									WM_HideWindow(hALARMA);
									WM_HideWindow(hALARMB);
									WM_HideWindow(PROGBAR_MEM);
									GUI_SetBkColor(GUI_BLACK);
									GUI_SetColor(GUI_RED);
									GUI_Clear();
									_drawJPG(Path,Buff);
									hButton=BUTTON_CreateEx(5,5,30,30,WM_HBKWIN,/*WM_CF_SHOW|*/WM_CF_HASTRANS,0,ID_BUTTON_EXIT);
									BUTTON_SetSkin(hButton, _cbButtonEXIT);
									WM_SetCallback(WM_HBKWIN, _cbEXIT);
								}
								else if((*pPath=='b')||(*pPath=='B'))
								{
									time_show=0;
									WM_HideWindow(hWin_menu);
									WM_HideWindow(hIcon_EXIT);
									WM_HideWindow(hIcon_BRIGHT);
									WM_HideWindow(hALARMA);
									WM_HideWindow(hALARMB);
									WM_HideWindow(PROGBAR_MEM);
									GUI_SetBkColor(GUI_BLACK);
									GUI_SetColor(GUI_RED);
									GUI_Clear();
									_drawBMP(Path,Buff);
									hButton=BUTTON_CreateEx(5,5,30,30,WM_HBKWIN,/*WM_CF_SHOW|*/WM_CF_HASTRANS,0,ID_BUTTON_EXIT);
									BUTTON_SetSkin(hButton, _cbButtonEXIT);
									WM_SetCallback(WM_HBKWIN, _cbEXIT);
								}
								else if((*pPath=='g')||(*pPath=='G'))
									{
										time_show=0;
									WM_HideWindow(hWin_menu);
									WM_HideWindow(hIcon_EXIT);
									WM_HideWindow(hIcon_BRIGHT);
									WM_HideWindow(hALARMA);
									WM_HideWindow(hALARMB);
									WM_HideWindow(PROGBAR_MEM);
									GUI_SetBkColor(GUI_BLACK);
									GUI_SetColor(GUI_RED);
									GUI_Clear();
									_drawGIF(Path,Buff);
									hButton=BUTTON_CreateEx(5,5,30,30,WM_HBKWIN,/*WM_CF_SHOW|*/WM_CF_HASTRANS,0,ID_BUTTON_EXIT);
									BUTTON_SetSkin(hButton, _cbButtonEXIT);
									WM_SetCallback(WM_HBKWIN, _cbEXIT);
									}
								else
									__nop(); 
							}			
						}
					else
						hNode_Last=hNode;
				break;
			}
    break;
    }
  break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       CreateMENU
*/

WM_HWIN SD_Files(void) {
	
		hWin_SD= GUI_CreateDialogBox(_aSD, GUI_COUNTOF(_aSD), _cbSD, hWin_menu, 0, 0);
	
	return hWin_SD;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/