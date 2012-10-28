/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : CameraControlDlg.cpp                                            *
*                                                                             *
*   Description: This is the Sample code to show the usage of EDSDK.          *
*                                                                             *
*                                                                             *
*******************************************************************************
*                                                                             *
*   Written and developed by Camera Design Dept.53                            *
*   Copyright Canon Inc. 2006-2008 All Rights Reserved                        *
*                                                                             *
*******************************************************************************/

#include "stdafx.h"
#include <map>

#include "CameraControl.h"
#include "CameraControlDlg.h"
#include ".\CameraControldlg.h"

#include "EDSDK.h"
#include "EDSDKTypes.h"


#include "common\xml.h"
#include "common\mvector.h"
#include "common\allxml.h"
#include "common\common_funcs.h"
#include <shlobj.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define WM_USER_DOWNLOAD_COMPLETE		WM_APP+1
#define WM_USER_PROGRESS_REPORT			WM_APP+2

CString PhotoSavePath;
bool evfAFOff=1;
// CCameraControlDlg 


CCameraControlDlg::CCameraControlDlg(CWnd* pParent )
	: CDialog(CCameraControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCameraControlDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_BUTTON1, _btnTakePicture);
  DDX_Control(pDX, IDC_PROGRESS1, _progress);
  //DDX_Control(pDX, IDC_EDIT1, _edit);
  DDX_Control(pDX, IDC_PICT, _pictureBox);
  DDX_Control(pDX, IDC_COMBO1, _comboAEMode);
  DDX_Control(pDX, IDC_COMBO2, _comboTv);
  DDX_Control(pDX, IDC_COMBO3, _comboAv);
  DDX_Control(pDX, IDC_COMBO4, _comboIso);
  DDX_Control(pDX, IDC_COMBO5, _comboMeteringMode);
  DDX_Control(pDX, IDC_COMBO6, _comboExposureComp);
  DDX_Control(pDX, IDC_COMBO7, _comboImageQuality);
  DDX_Control(pDX, IDC_BUTTON2, _btnStartEVF);
  DDX_Control(pDX, IDC_BUTTON3, _btnEndEVF);
  DDX_Control(pDX, IDC_BUTTON4, _btnFocusNear3);
  DDX_Control(pDX, IDC_BUTTON5, _btnFocusNear2);
  DDX_Control(pDX, IDC_BUTTON6, _btnFocusNear1);
  DDX_Control(pDX, IDC_BUTTON7, _btnFocusFar1);
  DDX_Control(pDX, IDC_BUTTON8, _btnFocusFar2);
  DDX_Control(pDX, IDC_BUTTON9, _btnFocusFar3);
  DDX_Control(pDX, IDC_BUTTON10, _btnFocusUp);
  DDX_Control(pDX, IDC_BUTTON11, _btnFocusLeft);
  DDX_Control(pDX, IDC_BUTTON12, _btnFocusRight);
  DDX_Control(pDX, IDC_BUTTON13, _btnFocusDown);
  DDX_Control(pDX, IDC_BUTTON14, _btnZoomFit);
  DDX_Control(pDX, IDC_BUTTON15, _btnZoomZoom);
  DDX_Control(pDX, IDC_BUTTON16, _btnPressingHalfway);
  DDX_Control(pDX, IDC_BUTTON17, _btnPressingCompletely);
  DDX_Control(pDX, IDC_BUTTON18, _btnPressingOff);
  DDX_Control(pDX, IDC_BUTTON21, _btnEvfAfON);
  DDX_Control(pDX, IDC_BUTTON22, _btnEvfAfOFF);
  DDX_Control(pDX, IDC_COMBO8, _comboEvfAFMode);
  DDX_Control(pDX, IDC_CHECK1, Lifeview);
  DDX_Control(pDX, IDC_COMBO10, drivebox);
  DDX_Control(pDX, IDC_EDIT2, settings_file);
  DDX_Control(pDX, IDC_EDIT3, filedir);
  DDX_Control(pDX, IDC_EDIT4, projectdir);
  DDX_Control(pDX, IDC_BUTTON26, shootbtn);
  DDX_Control(pDX, IDC_EDIT1, prefix);
  DDX_Control(pDX, IDC_EDIT5, current_page);
  DDX_Control(pDX, IDC_EDIT6, number_signs);
  //	DDX_Control(pDX, IDC_EDIT7, state);
  DDX_Control(pDX, IDC_EDIT7, _edit);
}

BEGIN_MESSAGE_MAP(CCameraControlDlg, CDialog)
	ON_MESSAGE(WM_USER_DOWNLOAD_COMPLETE, OnDownloadComplete)
	ON_MESSAGE(WM_USER_PROGRESS_REPORT, OnProgressReport)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK1, &CCameraControlDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON19, &CCameraControlDlg::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON23, &CCameraControlDlg::OnBnClickedButton23)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON24, &CCameraControlDlg::OnBnClickedButton24)
//	ON_BN_CLICKED(IDC_BUTTON1, &CCameraControlDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON26, &CCameraControlDlg::OnBnClickedButton26)
	//ON_BN_CLICKED(IDC_BUTTON27, &CCameraControlDlg::OnBnClickedButton27)
//  ON_BN_CLICKED(IDC_BUTTON22, &CCameraControlDlg::OnBnClickedButton22)
ON_WM_SIZE()
//ON_BN_CLICKED(IDC_BUTTON21, &CCameraControlDlg::OnBnClickedButton21)
ON_BN_CLICKED(IDC_BUTTON27, &CCameraControlDlg::OnBnClickedButton27)
END_MESSAGE_MAP()


// CCameraControlDlg message handlers

void SetDropDownHeight(CComboBox* pMyComboBox, int itemsToShow)
{
  //Get rectangles
  CRect rctComboBox, rctDropDown;
  pMyComboBox->GetClientRect(&rctComboBox); //Combo rect
  pMyComboBox->GetDroppedControlRect(&rctDropDown); //DropDownList rect

  int itemHeight = pMyComboBox->GetItemHeight(-1); //Get Item height

  pMyComboBox->GetParent()->ScreenToClient(&rctDropDown); //Converts coordinates
  rctDropDown.bottom = rctDropDown.top + rctComboBox.Height() + itemHeight*itemsToShow; //Set height
  pMyComboBox->MoveWindow(&rctDropDown); //enable changes
}

BOOL CCameraControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	setupListener(_controller);
	setupObserver(getCameraModel());

	//Execute controller
	_controller->run();

	// A set value of the camera is acquired. 
	// The value to which the camera can be set is acquired.

	//load drive combo box
	//Sleep(5000);
	TCHAR szPath[MAX_PATH];
	strcpy_s(szPath, MAX_PATH, _T("?"));

	this->DlgDirListComboBox(szPath,IDC_COMBO10,0,DDL_DRIVES);
	SetDropDownHeight(&drivebox,drivebox.GetCount());
	//drivebox.SetCurSel(0);

	this->InitProgramSett();
	//this->InitPhotoSett();


	return TRUE;   // return TRUE  unless you set the focus to a control
}

void CCameraControlDlg::setupListener(ActionListener* listener)
{
	addActionListener(listener);
	
	_comboAEMode.setActionCommand("set_AEMode");
	_comboAEMode.addActionListener(listener);
	_comboTv.setActionCommand("set_Tv");
	_comboTv.addActionListener(listener);
	_comboAv.setActionCommand("set_Av");
	_comboAv.addActionListener(listener);
	_comboIso.setActionCommand("set_ISOSpeed");
	_comboIso.addActionListener(listener);
	_comboMeteringMode.addActionListener(listener);
	_comboExposureComp.setActionCommand("set_ExposureCompensation");
	_comboExposureComp.addActionListener(listener);
	_comboImageQuality.setActionCommand("set_ImageQuality");
	_comboImageQuality.addActionListener(listener);
	_comboEvfAFMode.setActionCommand("set_EvfAFMode");
	_comboEvfAFMode.addActionListener(listener);

	_btnTakePicture.setActionCommand("TakePicture");
	_btnTakePicture.addActionListener(listener);
	_btnStartEVF.setActionCommand("startEVF");
	_btnStartEVF.addActionListener(listener);
	_btnEndEVF.setActionCommand("endEVF");
	_btnEndEVF.addActionListener(listener);
	_btnFocusNear3.setActionCommand("focus_Near3");
	_btnFocusNear3.addActionListener(listener);
	_btnFocusNear2.setActionCommand("focus_Near2");
	_btnFocusNear2.addActionListener(listener);
	_btnFocusNear1.setActionCommand("focus_Near1");
	_btnFocusNear1.addActionListener(listener);
	_btnFocusFar3.setActionCommand("focus_Far3");
	_btnFocusFar3.addActionListener(listener);
	_btnFocusFar2.setActionCommand("focus_Far2");
	_btnFocusFar2.addActionListener(listener);
	_btnFocusFar1.setActionCommand("focus_Far1");
	_btnFocusFar1.addActionListener(listener);
	_btnFocusUp.setActionCommand("focus_Up");
	_btnFocusUp.addActionListener(listener);
	_btnFocusLeft.setActionCommand("focus_Left");
	_btnFocusLeft.addActionListener(listener);
	_btnFocusRight.setActionCommand("focus_Right");
	_btnFocusRight.addActionListener(listener);
	_btnFocusDown.setActionCommand("focus_Down");
	_btnFocusDown.addActionListener(listener);
	_btnZoomFit.setActionCommand("zoom_Fit");
	_btnZoomFit.addActionListener(listener);
	_btnZoomZoom.setActionCommand("zoom_Zoom");
	_btnZoomZoom.addActionListener(listener);
	_btnPressingHalfway.setActionCommand("pressingHalfway");
	_btnPressingHalfway.addActionListener(listener);
	_btnPressingCompletely.setActionCommand("pressingCompletely");
	_btnPressingCompletely.addActionListener(listener);
	_btnPressingOff.setActionCommand("pressingOff");
	_btnPressingOff.addActionListener(listener);
	_btnEvfAfON.setActionCommand("evfAFOn");
	_btnEvfAfON.addActionListener(listener);
	_btnEvfAfOFF.setActionCommand("evfAFOff");
	_btnEvfAfOFF.addActionListener(listener);

	_pictureBox.setActionCommand("downloadEVF");
	_pictureBox.addActionListener(listener);
}


void CCameraControlDlg::setupObserver(Observable* ob)
{
	ob->addObserver(static_cast<Observer*>(&_comboAEMode));
	ob->addObserver(static_cast<Observer*>(&_comboTv));
	ob->addObserver(static_cast<Observer*>(&_comboAv));
	ob->addObserver(static_cast<Observer*>(&_comboIso));
	ob->addObserver(static_cast<Observer*>(&_comboMeteringMode));
	ob->addObserver(static_cast<Observer*>(&_comboExposureComp));
	ob->addObserver(static_cast<Observer*>(&_comboImageQuality));
	ob->addObserver(static_cast<Observer*>(&_pictureBox));
	ob->addObserver(static_cast<Observer*>(&_comboEvfAFMode));
	ob->addObserver(static_cast<Observer*>(&_btnZoomZoom));
}

void CCameraControlDlg::OnClose()
{
	// TODO : The control notification handler code is added here or Predetermined processing is called. 
	fireEvent("closing");
	Lifeview.EnableWindow(FALSE);
	_btnTakePicture.EnableWindow(FALSE);	
	_progress.EnableWindow(FALSE);
	//_edit.EnableWindow(FALSE);
	_comboAEMode.EnableWindow(FALSE);
	_comboTv.EnableWindow(FALSE);
	_comboAv.EnableWindow(FALSE);
	_comboIso.EnableWindow(FALSE);	
	_comboImageQuality.EnableWindow(FALSE);	
	_btnPressingHalfway.EnableWindow(FALSE);	
	_btnPressingCompletely.EnableWindow(FALSE);	
	_btnPressingOff.EnableWindow(FALSE);	
	_comboEvfAFMode.EnableWindow(FALSE);	
	_btnEvfAfON.EnableWindow(FALSE);
	_btnEvfAfOFF.EnableWindow(FALSE);
	__super::OnClose();
}

void CCameraControlDlg::CheckFocusEnabled(Observable* from, CameraEvent *e)
{



  if(e->getArg()==NULL)
  {
    return;
  }
  EdsPropertyID propertyID=*(EdsPropertyID*)e->getArg();
  CameraModel* _model=getCameraModel();
  
	if(propertyID == kEdsPropID_Unknown)
  {
    return;
  }


		EdsDataType	dataType = kEdsDataType_Unknown;
		EdsUInt32   dataSize = 0;

    
		EdsError	err = EdsGetPropertySize(_model->getCameraObject(),
        propertyID,0,&dataType,&dataSize );
      
		if(err == EDS_ERR_OK)
		{
      

			if(dataType == kEdsDataType_FocusInfo)
      {
        
				EdsFocusInfo focusInfo;
				//Acquisition of the property
				err = EdsGetPropertyData( _model->getCameraObject(),
		propertyID,0,dataSize,&focusInfo );

				//Acquired property value is set
				if(err == EDS_ERR_OK)
				{
	        for(EdsUInt32 i = 0; i < focusInfo.pointNumber; i++)
	        {
		        if(focusInfo.focusPoint[i].valid==1)
            {
			        // Selecte Just Focus Pen
			        if(focusInfo.focusPoint[i].justFocus&1)
              {
                //focused=1;
                if(evfAFOff)
                  return;
                evfAFOff=1;
	              fireEvent("evfAFOff");
                shootbtn.EnableWindow(1);
                return;
              }
            }
          }
        }
      }
      else
      {
        //MessageBox(NULL,"not focused",MB_OK);
      }
    }
    
  
    return;
}


void CCameraControlDlg::update(Observable* from, CameraEvent *e)
{
  this->CheckFocusEnabled(from,e);
	std::string event = e->getEvent();
  
	if(event == "DownloadComplete")
	{
		//The update processing can be executed from another thread. 
		::PostMessage(this->m_hWnd, WM_USER_DOWNLOAD_COMPLETE, NULL, NULL);
	}

	//Progress of download of image
	if(event == "ProgressReport")
	{
		EdsInt32 percent = *static_cast<EdsInt32 *>(e->getArg());
		
		//The update processing can be executed from another thread. 
		::PostMessage(this->m_hWnd, WM_USER_PROGRESS_REPORT, percent, NULL);
	}

	//shutdown event
	if(event == "shutDown")
	{
		::PostMessage(this->m_hWnd, WM_CLOSE, 0, NULL);
	}
}


LRESULT CCameraControlDlg::OnDownloadComplete(WPARAM wParam, LPARAM lParam)
{
	//End of download of image
	_progress.SetPos(0);

	
	//increment curent page
	CString temp2;
	current_page.GetWindowTextA(temp2);
	int p=atoi(temp2.GetBuffer());
	p++;
	char buffer[10];
	_itoa_s(p,buffer,10,10);
	current_page.SetWindowTextA(buffer);
  shootbtn.EnableWindow(1);
	return 0;
}

LRESULT CCameraControlDlg::OnProgressReport(WPARAM wParam, LPARAM lParam)
{
	_progress.SetPos((int)wParam);
	return 0;
}

void CCameraControlDlg::OnBnClickedCheck1()
{
	if(Lifeview.GetCheck())
		//_btnStartEVF.OnClicked();
	fireEvent("startEVF");
	//_btnStartEVF.addActionListener(listener);
		//fireEvent("startEVF");
	else
		//_btnEndEVF.OnClicked();
	fireEvent("endEVF");
	// TODO: Add your control notification handler code here
}

void CCameraControlDlg::OnBnClickedButton19()
{
	
	CFileDialog dlg(TRUE/*Open=TRUE Save=False*/,NULL/*Filename Extension*/,""/*Initial Filename*/,OFN_ENABLESIZING|OFN_EXPLORER|OFN_FILEMUSTEXIST/*Flags*/,"XML(*.xml)|*.xml||"/*Filetype Filter*/,this/*parent Window*/);
	if (dlg.DoModal() == IDOK)
	{
		CString path=dlg.GetPathName();
		settings_file.SetWindowText(path.GetBuffer());
		this->InitPhotoSett();
	}
}

void CCameraControlDlg::OnBnClickedButton23()
{
	
	CFileDialog dlg(FALSE/*Open=TRUE Save=False*/,NULL/*Filename Extension*/,"settings.xml"/*Initial Filename*/,OFN_ENABLESIZING|OFN_EXPLORER|OFN_FILEMUSTEXIST/*Flags*/,"XML(*.xml)|*.xml||"/*Filetype Filter*/,this/*parent Window*/);
	if (dlg.DoModal() == IDOK)
	{
		CString path=dlg.GetPathName();
		if(strcmp(path.Right(4).GetBuffer(),".xml"))
			path.Append(".xml");
		settings_file.SetWindowText(path.GetBuffer());
		this->SavePhotoSett(path.GetBuffer());
}
}



void CCameraControlDlg::InitPhotoSett()
{
	CString path;
	settings_file.GetWindowTextA(path);
	char* f=file_get_contents(path.GetBuffer(),0);
	if(f==NULL)
	{
		MessageBox("���� �������� �� ������, ������������ ��������� �� ���������!");
		return;
	}
	
		
	AllXml* xmlvec=new(AllXml);
	xmlvec->Parse(f,"xml");
	delete(f);
	xml* child=xmlvec->GetItemOnlyChild(xmlvec->MainElement,"xml",1);
	child=xmlvec->GetItemOnlyChild(child,"settings",1);
	
	//Loading to select lists
  this->SetComboSett(&_comboAEMode,xmlvec->GetItemOnlyChild(child,"_comboAEMode",0)->subtext);
  this->SetComboSett(&_comboTv,xmlvec->GetItemOnlyChild(child,"_comboTv",0)->subtext);
  this->SetComboSett(&_comboAv,xmlvec->GetItemOnlyChild(child,"_comboAv",0)->subtext);
  this->SetComboSett(&_comboIso,xmlvec->GetItemOnlyChild(child,"_comboIso",0)->subtext);
  this->SetComboSett(&_comboMeteringMode,xmlvec->GetItemOnlyChild(child,"_comboMeteringMode",0)->subtext);
  this->SetComboSett(&_comboExposureComp,xmlvec->GetItemOnlyChild(child,"_comboExposureComp",0)->subtext);
  this->SetComboSett(&_comboEvfAFMode,xmlvec->GetItemOnlyChild(child,"_comboEvfAFMode",0)->subtext);
  this->SetComboSett(&_comboImageQuality,xmlvec->GetItemOnlyChild(child,"_comboImageQuality",0)->subtext);

	Lifeview.SetCheck(atoi(xmlvec->GetItemOnlyChild(child,"Lifeview",0)->subtext));

//set drive if mapped
  this->SetComboSett(&drivebox,xmlvec->GetItemOnlyChild(child,"drivebox",0)->subtext);

	//set text values
	filedir.SetWindowTextA(xmlvec->GetItemOnlyChild(child,"filedir",0)->subtext);
	prefix.SetWindowTextA(xmlvec->GetItemOnlyChild(child,"prefix",0)->subtext);
	current_page.SetWindowTextA(xmlvec->GetItemOnlyChild(child,"current_page",0)->subtext);
	number_signs.SetWindowTextA(xmlvec->GetItemOnlyChild(child,"number_signs",0)->subtext);

	
	//Settings loaded. Now - update program settings
	CString SettText="<xml><program><DefaultSettFile>";
	SettText.Append(path);
	SettText.Append("</DefaultSettFile></program></xml>");
	file_put_contents("program_sett.xml",SettText.GetBuffer());

	//delete temporary objects
	delete(xmlvec);

	//update system events
	this->OnBnClickedCheck1();//enable or disable lifeview, according to options loaded
	
	
/*	_comboAEMode.OnSelChange();
	_comboTv.OnSelChange();
	_comboAv.OnSelChange();
	_comboIso.OnSelChange();
	_comboMeteringMode.OnSelChange();
	_comboExposureComp.OnSelChange();
	_comboImageQuality.OnSelChange();
	_comboEvfAFMode.OnSelChange();*/

	//MessageBox("settings ok");

}
void CCameraControlDlg::InitProgramSett()
{
	char* z=GetApplicationDir();
	CString AppPath=z;
	delete(z);
	CString SettPath;
	SettPath=AppPath;
	SettPath.Append("program_sett.xml");
	char *f=file_get_contents(SettPath.GetBuffer(),0);
	if(f==NULL)//default program settings not found, making file...
	{
		CString sett="<xml><program><DefaultSettFile>";
		sett.Append(AppPath);
		sett.Append("default.xml");
		sett.Append("</DefaultSettFile></program></xml>");
		file_put_contents(SettPath.GetBuffer(),sett.GetBuffer());
		int l=sett.GetLength()+1;
		f=new char[l];
		strcpy_s(f,l,sett.GetBuffer());
		//SaveSett(SettPath.GetBuffer());
	}
	AllXml* xmlvec=new(AllXml);
	xmlvec->Parse(f,"xml");
	delete(f);
	xml* child=xmlvec->GetItemOnlyChild(xmlvec->MainElement,"xml",1);
	child=xmlvec->GetItemOnlyChild(child,"program",1);
	child=xmlvec->GetItemOnlyChild(child,"DefaultSettFile",1);
	settings_file.SetWindowTextA(child->subtext);
	//this->SaveSett(child->subtext);
	delete(xmlvec);
}

void CCameraControlDlg::SetComboSett(CComboBox* combo,char* val)
{
  CString tmp;
  for(int i=0;i<combo->GetCount();i++)
  {
    combo->GetLBText(i,tmp);
    if(!strcmp(tmp.GetBuffer(),val))
      combo->SetCurSel(i);
  }
}


void CCameraControlDlg::GetComboSett(CComboBox* combo,char*name,CString* SettText)
{
  CString temp;
  combo->GetWindowText(temp);
  SettText->Append("<");
  SettText->Append(name);
  SettText->Append(">");
  SettText->Append(temp);
  SettText->Append("</");
  SettText->Append(name);
  SettText->Append(">");
}

void CCameraControlDlg::SavePhotoSett(char* path)
{
	//update photo settings
	CString SettText;
	CString temp;
	SettText="<xml><settings>";
  
  GetComboSett(&_comboAEMode,"_comboAEMode",&SettText);
  GetComboSett(&_comboTv,"_comboTv",&SettText);
  GetComboSett(&_comboAv,"_comboAv",&SettText);
  GetComboSett(&_comboIso,"_comboIso",&SettText);
  GetComboSett(&_comboMeteringMode,"_comboMeteringMode",&SettText);
  GetComboSett(&_comboExposureComp,"_comboExposureComp",&SettText);
  GetComboSett(&_comboImageQuality,"_comboImageQuality",&SettText);
  GetComboSett(&_comboEvfAFMode,"_comboEvfAFMode",&SettText);
  if(Lifeview.GetCheck())
    SettText.Append("<Lifeview>1</Lifeview>");
  else
    SettText.Append("<Lifeview>0</Lifeview>");

  GetComboSett(&drivebox,"drivebox",&SettText);
	
	SettText.Append("<filedir>");
	filedir.GetWindowTextA(temp);
	SettText.Append(temp);
	SettText.Append("</filedir>");
	
	SettText.Append("<prefix>");
	prefix.GetWindowTextA(temp);
	SettText.Append(temp);
	SettText.Append("</prefix>");
	
	SettText.Append("<current_page>");
	current_page.GetWindowTextA(temp);
	SettText.Append(temp);
	SettText.Append("</current_page>");
	
	SettText.Append("<number_signs>");
	number_signs.GetWindowTextA(temp);
	SettText.Append(temp);
	SettText.Append("</number_signs>");

	SettText.Append("</settings></xml>");
	file_put_contents(path,SettText.GetBuffer());

	//Settings saved. Now - update program settings
	
	SettText="<xml><program><DefaultSettFile>";
	SettText.Append(path);
	SettText.Append("</DefaultSettFile></program></xml>");
	char* p=GetApplicationDir();
	CString z=p;
	z.Append("program_sett.xml");
	delete(p);
	file_put_contents(z.GetBuffer(),SettText.GetBuffer());
}


void CCameraControlDlg::OnBnClickedButton24()
{
	if(drivebox.GetCurSel()==-1)
	{
		MessageBox("����������, �������� ������� ���� ��� ������","������",MB_OK);
		return;
	}
	LPITEMIDLIST pidlRoot = NULL;
	char* d=GetFullSavePath(0);
	PCWSTR temp;
	CString tmp;

	tmp.Append(d);
	delete(d);
	#ifdef _UNICODE
		temp = LPCTSTR(tmp);
	#else
		USES_CONVERSION;
		temp = A2CW(LPCTSTR(tmp));
	#endif
	HRESULT hR= SHParseDisplayName(temp,0, &pidlRoot, 0, 0);


	TCHAR path[_MAX_PATH];
	BROWSEINFO bi={NULL,pidlRoot,path,_T("����������, �������� ����� ��� ���������� ����������"),BIF_USENEWUI+BIF_SHAREABLE,NULL,
   NULL};


    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
    if ( pidl != 0 )//if folder selected
    {
	   if ( SHGetPathFromIDList ( pidl, path ) )
     {
       tmp=path;
       tmp=tmp.Right(tmp.GetLength()-3);
		   filedir.SetWindowTextA(tmp);

     }
	   else
		   MessageBox("�� ���������� �������� ���� � ����������","������!",MB_OK);

        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
    }
}

//void CCameraControlDlg::OnBnClickedButton1()
//{
//	
//}

char* CCameraControlDlg::GetFullSavePath(int mode)
{//0 - only get drive+project dir
 //1 - getdrive+project+file dir
//2 - full path, including prefix
	//1 by default
	CString p,temp;
	drivebox.GetWindowTextA(p);
	p=p.GetAt(2);
	p.Append(":\\");
	projectdir.GetWindowTextA(temp);
	if(temp.GetLength())
	{
		p.Append(temp);
		p.Append("\\");
	}
	if(mode)
	{
		filedir.GetWindowTextA(temp);
		if(temp.GetLength())
		{
			p.Append(temp);
			p.Append("\\");
		}
	}
	if(mode==2)
	{
		prefix.GetWindowTextA(temp);
		if(temp.GetLength())
		{
			p.Append(temp);
			//p.Append("_");
		}
		current_page.GetWindowTextA(temp);
		if(temp.GetLength())
		{
			CString temp2;
			number_signs.GetWindowTextA(temp2);
			int nulls=atoi(temp2.GetBuffer());
			while(temp.GetLength()<nulls)
				temp.Insert(0,'0');
			p.Append(temp);
		}

	}
	int n=p.GetLength()+1;
	char * res=new char[n];
	strcpy_s(res,n,p.GetBuffer());
	return res;
}

void CCameraControlDlg::OnBnClickedButton26()
{
	
	CString test;
  CString temp3;
	if(drivebox.GetCurSel()==-1)
	{
		MessageBox("����������, �������� ������� ���� ��� ������","������",MB_OK);
		return;
	}
	
	char* d=GetFullSavePath();
  CString dir=d;
	delete[] d;
  if(!DirectoryExists(dir.GetBuffer()))
	{
		CString msg;
		msg.Append("���������� '");
		msg.Append(dir.GetBuffer());
		msg.Append("' �� ����������. �������?");
		int n=MessageBox(msg,"��������������",MB_OKCANCEL);
		if(n==IDCANCEL)
			return;
		else
		{
		int r=SHCreateDirectoryEx(NULL,dir.GetBuffer(),NULL);
		if(r==ERROR_SUCCESS)
			MessageBox("���������� ������� ������","���������",MB_OK);
		else
		{
			MessageBox("���������� ������� �� �������. ��������� ����������� ����� � ����� �� ������","���������",MB_OK);
			return;
		}
		}
	}
	d=GetFullSavePath(2);
	PhotoSavePath=d;

  test.Append(d);
	delete[] d;
  _comboImageQuality.GetWindowText(temp3);
	if(temp3.Find("Jpeg")!=-1)
		test.Append(".JPG");
	else if(temp3.Find("RAW")!=-1)
		test.Append(".CR2");
	else if(temp3.Find("Raw")!=-1)
		test.Append(".CR2");
	if(FileExists(test.GetBuffer()))
	{
		int n=MessageBox("����� ���� ��� ����������. �� �������, ��� ������ ��� ������������?","��������������",MB_OKCANCEL);
		if(n==IDCANCEL)
			return;
	}

//	_btnTakePicture.OnClicked();
  shootbtn.EnableWindow(0);
	fireEvent("TakePicture");
}



void CCameraControlDlg::OnSize(UINT nType, int cx, int cy)
{
  /*
    HRGN Region;
    POINT Points[4];

    //Define the points accross down
    Points[0].x=100;
    Points[1].x= 200;
    Points[2].x=0;
    Points[3].x=305;
    Points[0].y=30;
    Points[1].y=200;
    Points[2].y= 200;
    Points[3].y=30;

    //Define the region
    Region = CreatePolygonRgn(Points, 4, ALTERNATE);


  _pictureBox.SetWindowRgn(Region,1);*/
  __super::OnSize(nType, cx, cy);
}

//void CCameraControlDlg::OnBnClickedButton21()
//{
//  // TODO: Add your control notification handler code here
//}

void CCameraControlDlg::OnBnClickedButton27()
{
  shootbtn.EnableWindow(0);
  fireEvent("evfAFOn");
  evfAFOff=0;
  // TODO: Add your control notification handler code here
}