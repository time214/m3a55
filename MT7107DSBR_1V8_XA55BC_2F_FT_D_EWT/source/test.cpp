#include "stdafx.h"
#include "math.h"

#define SITENUM 2
#define sitedut 2
BYTE sitestatus[sitedut];

int i=0,j=0,t=0;
double k=0.0f,v=0.0f,vx=0.0f,checkadd=0.0f,check[4]={0.0f};
double adresult[100]={0},result[2]={0};
double v11[2]={0},v22[2]={0},v33[2]={0},v44[2]={0},v1[2]={0},v2[2]={0},v3[2]={0},v4[2]={0};

PVI10A vin(0);
PVI10A vout(1);
DVI400 en(2);
DVI400 check_site(0);
DVI400 kelven(1);
//DVI400 vout_m(3);
CBIT128 cbit;

#define K1 1, 33
#define K2 2, 34 //kelvin
//#define K3 3, 35

//multisite settings should be included here
DUT_API void HardWareCfg()
{
	/*For example: four channels dvi to config two sites*/
	StsSetModuleToSite(MD_PVI10A, SITE_1, 0, 1, -1);
	StsSetModuleToSite(MD_PVI10A, SITE_2, 2, 3, -1);
	StsSetModuleToSite(MD_DVI400, SITE_1, 0, 1, 2, 3, -1);
	StsSetModuleToSite(MD_DVI400, SITE_2, 4, 5, 6, 7, -1);
	//StsSetModuleToSite(MD_DVI400, SITE_2, 8, 9, 10, 11, -1);
	//StsSetModuleToSite(MD_OVI40, SITE_1, 0, -1);
	//StsSetModuleToSite(MD_OVI40, SITE_2, 1, -1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initialize function will be called before all the test functions.
DUT_API void InitBeforeTestFlow()
{
	cbit.Init();
	vin.Init();
	vout.Init();
	en.Init();
	check_site.Init();
    kelven.Init();
	//vout_m.Init();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initializefunction will be called after all the test functions.
DUT_API void InitAfterTestFlow()
{
	cbit.Init();
	vin.Init();
	vout.Init();
    en.Init();
	check_site.Init();
    kelven.Init();
	//vout_m.Init();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//Fail site hardware set function will be called after failed params, it can be called for serveral times. 
DUT_API void SetupFailSite(const unsigned char*byFailSite)
{			
}

//DUT_API int SetQAStartFuncNum()
//{
//   return 13;
//}

DUT_API int Contact(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *site_check = StsGetParam(funcindex,"site_check");
    CParam *KELVIN = StsGetParam(funcindex,"KELVIN");
	//}}AFX_STS_PARAM_PROTOTYPES

	check_site.Set(FI, 1.0e-3f, DVI400_10V, DVI400_4MA, RELAY_ON);	
	delay_ms(3);
	check_site.MeasureVI(MV, 10, 20);
	for(i=0;i<SITENUM;i++)
	{
		adresult[i]=check_site.GetMeasResult(i);
		site_check->SetTestResult(i, 0, adresult[i]/(1+i));
	}	
	check_site.Set(FI, 0.0f, DVI400_5V, DVI400_40MA, RELAY_ON);
	delay_ms(1);
	check_site.Set(FI, 0.0f, DVI400_5V, DVI400_40MA, RELAY_OFF);
	delay_ms(1);

	cbit.SetOn(K2,-1);
	delay_ms(1);
	kelven.Set(FI, 100.0e-6f, DVI400_1V, DVI400_40UA, RELAY_ON);	
	delay_ms(1);
	kelven.MeasureVI(MV, 10, 20);
	for(i=0;i<SITENUM;i++)
	{ 
		adresult[i] = kelven.GetMeasResult(i);
		KELVIN->SetTestResult(i, 0, adresult[i]);
	}
	kelven.Set(FI, 0.0f, DVI400_1V, DVI400_40UA, RELAY_ON);
	kelven.Set(FI, 0.0f, DVI400_1V, DVI400_40UA, RELAY_OFF);
	cbit.SetOn(-1);
    delay_ms(1);
    return 0;
}

DUT_API int OS(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Vin_os = StsGetParam(funcindex,"Vin_os");
	CParam *En_os = StsGetParam(funcindex,"En_os");
    CParam *Vout_os = StsGetParam(funcindex,"Vout_os");
    //}}AFX_STS_PARAM_PROTOTYPES
	
	vout.Set(FI, 0.0f, PVI10A_2V, PVI10A_10MA,RELAY_OFF);
	vin.Set(FV, 0.0f, PVI10A_2V, PVI10A_10MA, RELAY_ON);
	vout.Set(FV, 0.0f, PVI10A_2V, PVI10A_10MA,RELAY_ON);
	en.Set(FV, 0.0f, DVI400_2V, DVI400_4MA, RELAY_ON);
	///////////////////////// Vin_os /////////////////////////
	vin.Set(FI, -1.0e-3, PVI10A_2V, PVI10A_10MA, RELAY_ON);
	delay_us(200);

	vin.MeasureVI(MV, 10, 20);
	for(i=0;i<SITENUM;i++)
	{
		adresult[i]=vin.GetMeasResult(i);
		Vin_os->SetTestResult(i, 0, adresult[i]);
	}	

	vin.Set(FI, 0.0e-3, PVI10A_2V, PVI10A_10MA, RELAY_ON);
	vin.Set(FV, 0.0f, PVI10A_2V, PVI10A_10MA, RELAY_ON);
	///////////////////////// En_os /////////////////////////
	en.Set(FI, -1.0e-3, DVI400_2V, DVI400_4MA, RELAY_ON);
	delay_us(200);

	en.MeasureVI(MV, 10, 20);
	for(i=0;i<SITENUM;i++)
	{
		adresult[i]=en.GetMeasResult(i);
		En_os->SetTestResult(i, 0, adresult[i]);
	}	

	en.Set(FI, 0.0e-3, DVI400_2V, DVI400_4MA, RELAY_ON);
	en.Set(FV, 0.0e-3, DVI400_2V, DVI400_4MA, RELAY_ON);
	///////////////////////// Vout_os /////////////////////////
	vout.Set(FI, -1.0e-3, PVI10A_2V, PVI10A_10MA, RELAY_ON);
	delay_us(200);

	vout.MeasureVI(MV, 10, 20);
	for(i=0;i<SITENUM;i++)
	{
		adresult[i]=vout.GetMeasResult(i);
		Vout_os->SetTestResult(i, 0, adresult[i]);
	}	
	
	vout.Set(FI, 0.0e-3, PVI10A_2V, PVI10A_10MA, RELAY_ON);
	vout.Set(FI, 0.0e-3, PVI10A_2V, PVI10A_10MA, RELAY_OFF);
    return 0;
}

DUT_API int Trim(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Trim = StsGetParam(funcindex,"Trim");
    //}}AFX_STS_PARAM_PROTOTYPES

	double adresult[16]={0};

	vin.Set(FV, 5.0, PVI10A_10V, PVI10A_1A, RELAY_ON);
	en.Set(FV, 2.0, DVI400_10V, DVI400_40MA, RELAY_ON);
	delay_ms(10);
	vout.Set(FV, 6.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
    delay_ms(3);
	vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_OFF);
    delay_ms(3);
	en.Set(FV, 0.0, DVI400_10V, DVI400_40MA, RELAY_ON);

	vin.Set(FV, 5.5, PVI10A_10V, PVI10A_1A, RELAY_ON);
	delay_ms(1);
    ///////////////////////// bit1 /////////////////////////
	for(t=0;t<1;t=t+1)	
	{
	   	vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	    delay_us(150); 
	    vout.Set(FV, 5.5, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	    delay_us(150);
	}
	vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_us(150); 
	vout.Set(FV, 5.5, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_ms(3);
    vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_ms(5);
	///////////////////////// bit2 /////////////////////////
	for(t=0;t<2;t=t+1)	
	{
	   	vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	    delay_us(150); 
	    vout.Set(FV, 5.5, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	    delay_us(150);
	}
	vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_us(150); 
	vout.Set(FV, 5.5, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_ms(3);
    vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_ms(5);
	/*
	///////////////////////// bit3 /////////////////////////
	for(t=0;t<3;t=t+1)	
	{
	   	vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	    delay_us(150); 
	    vout.Set(FV, 5.5, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	    delay_us(150);
	}
	vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_us(150); 
	vout.Set(FV, 5.5, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_ms(3);
    vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_ms(5);
	*/
    for (i=0;i<SITENUM;i++)
	{
	 		Trim->SetTestResult(i, 0, 1.8);
	}
	vin.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	return 0;
}

DUT_API int Trim_1(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Trim_1 = StsGetParam(funcindex,"Trim_1");
    //}}AFX_STS_PARAM_PROTOTYPES

	double adresult[16]={0};
    vin.Set(FV, 0.0, PVI10A_10V, PVI10A_1A, RELAY_ON);
	en.Set(FV, 0.0, DVI400_10V, DVI400_40MA, RELAY_ON);
	vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	cbit.SetOn(-1);
	delay_ms(1);	

	vin.Set(FV, 5.0, PVI10A_10V, PVI10A_1A, RELAY_ON);
	en.Set(FV, 2.0, DVI400_10V, DVI400_40MA, RELAY_ON);
	delay_ms(10);
	vout.Set(FV, 6.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
    delay_ms(3);
	vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_OFF);
    delay_ms(3);
	en.Set(FV, 0.0, DVI400_10V, DVI400_40MA, RELAY_ON);

	vin.Set(FV, 5.5, PVI10A_10V, PVI10A_1A, RELAY_ON);
	delay_ms(1);
    ///////////////////////// bit4 /////////////////////////
	for(t=0;t<4;t=t+1)	
	{
	   	vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	    delay_us(150); 
	    vout.Set(FV, 5.5, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	    delay_us(150);
	}
	vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_us(150); 
	vout.Set(FV, 5.5, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_ms(3);
    vout.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	delay_ms(5);
	
    for (i=0;i<SITENUM;i++)
	{
	 		Trim_1->SetTestResult(i, 0, 4);
	}
	vin.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	return 0;
}


DUT_API int Iq(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Iq = StsGetParam(funcindex,"Iq");
	CParam *Ien = StsGetParam(funcindex,"Ien");
	CParam *En_PD_R = StsGetParam(funcindex,"En_PD_R");
	CParam *Ishdn = StsGetParam(funcindex,"Ishdn");
    //}}AFX_STS_PARAM_PROTOTYPES

	double adresult[16]={0};

	vout.Set(FI, 0.0f, PVI10A_5V, PVI10A_10MA, RELAY_OFF);
    ////////////////////////// IQ //////////////////////////
	vin.Set(FV, 5.0f, PVI10A_10V, PVI10A_10MA, RELAY_ON);
	en.Set(FV, 2.0f, DVI400_5V, DVI400_40UA, RELAY_ON);
    vin.Set(FV, 5.0f, PVI10A_10V, PVI10A_1MA, RELAY_ON);
	delay_ms(20);

	vin.MeasureVI(MI, 10, 20);	
	for(i=0;i<SITENUM;i++)
	{
		adresult[i]=vin.GetMeasResult(i);
		Iq->SetTestResult(i, 0, (adresult[i]*1e6)-1.0f);
	}
    ////////////////////////// Ien //////////////////////////
	en.Set(FV, 2.0, DVI400_5V, DVI400_40UA, RELAY_ON);
    delay_ms(2);
	en.Set(FV, 2.0, DVI400_5V, DVI400_4UA, RELAY_ON);
    delay_ms(2);
	en.MeasureVI(MI, 10, 20);

	for(i=0;i<SITENUM;i++)
	{
		adresult[i]=en.GetMeasResult(i);
		Ien->SetTestResult(i, 0, adresult[i]*1e6);
		En_PD_R->SetTestResult(i, 0, -(((5.0-2.0)/adresult[i])/1e6));
	}
    ////////////////////////// Isd //////////////////////////
	en.Set(FV, 0.0, DVI400_2V, DVI400_400UA, RELAY_ON);
	vin.Set(FV, 5.5, PVI10A_10V, PVI10A_1MA, RELAY_ON);
	//vin.Set(FV, 5.5, PVI10A_10V, PVI10A_100UA, RELAY_ON);
	delay_ms(10);
    
	vin.MeasureVI(MI, 10, 20);	
    for(i=0;i<SITENUM;i++)
    {
		adresult[i]=vin.GetMeasResult(i);
		Ishdn->SetTestResult(i, 0, adresult[i]*1e6);
	}

	en.Set(FV, 0.0, DVI400_5V, DVI400_400UA, RELAY_ON);
	vin.Set(FV, 0.0, PVI10A_10V, PVI10A_100UA, RELAY_ON);
	return 0;
}

DUT_API int Vout(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Vout1 = StsGetParam(funcindex,"Vout1");
    CParam *Vout2 = StsGetParam(funcindex,"Vout2");
    CParam *Vout3 = StsGetParam(funcindex,"Vout3");
    CParam *LineR = StsGetParam(funcindex,"LineR");
    CParam *LoadR = StsGetParam(funcindex,"LoadR");
    //}}AFX_STS_PARAM_PROTOTYPES
    
	double adresult[16]={0}; 

	cbit.SetOn(K1,-1);
	delay_ms(1);	
	//vout_m.Set(FI, 0.0e-6, DVI400_5V, DVI400_40UA, RELAY_ON);
	///////////////////////// Vout1 /////////////////////////
	vin.Set(FV, 2.6f, PVI10A_10V, PVI10A_1A,RELAY_ON);
	en.Set(FV, 1.2f, DVI400_5V, DVI400_400UA, RELAY_ON);
	vout.Set(FI, -10.0e-3, PVI10A_5V, PVI10A_1A,RELAY_ON);
	delay_ms(5);

	vout.MeasureVI(MV, 10, 20);
	for (i=0;i<SITENUM;i++)
	{
		v1[i]=vout.GetMeasResult(i);
		Vout1->SetTestResult(i, 0, v1[i]);
	}	
	/////////////////////////// 150mA /////////////////////////
	vin.Set(FV, 2.8f, PVI10A_10V, PVI10A_1A,RELAY_ON);
    delay_ms(1);
	for (vx=-10.0e-3f; vx>=-150.0e-3f; vx=vx-1.0e-3f)
	{
		vout.Set(FI, vx, PVI10A_5V, PVI10A_1A,RELAY_ON);	
	}

	vout.MeasureVI(MV, 10, 20);
	for (i=0;i<SITENUM;i++)
	{
		v4[i]=vout.GetMeasResult(i);		
	}
	/////////////////////////// Vout2 /////////////////////////
	//vin.Set(FV, 3.1f, PVI10A_10V, PVI10A_1A,RELAY_ON);
	//vout.Set(FI, -250.0e-3, PVI10A_5V, PVI10A_1A,RELAY_ON);
	//delay_ms(10);
	for (vx=-150.0e-3f; vx>=-300.0e-3f; vx=vx-1.0e-3f)
	{
		vout.Set(FI, vx, PVI10A_5V, PVI10A_1A,RELAY_ON);	
	}
    
	vout.MeasureVI(MV, 10, 20);
	for (i=0;i<SITENUM;i++)
	{
		v2[i]=vout.GetMeasResult(i)-0.004f;		
		Vout2->SetTestResult(i, 0, v2[i]);
	}
	vout.Set(FI, 0.0e-3, PVI10A_5V, PVI10A_1A,RELAY_ON);
	///////////////////////// Vout3 /////////////////////////
	vin.Set(FV, 5.5f, PVI10A_10V, PVI10A_1A,RELAY_ON);
	vout.Set(FI,-10.0e-3, PVI10A_5V, PVI10A_1A,RELAY_ON);
	delay_ms(10);
	
	vout.MeasureVI(MV, 10, 20);
	for (i=0;i<SITENUM;i++)
	{
		v3[i]=vout.GetMeasResult(i);
		Vout3->SetTestResult(i, 0, v3[i]);
	}	
	///////////////////////// LineR & LoadR ////////////////////////
	for (i=0;i<SITENUM;i++)
	{
		LineR->SetTestResult(i, 0, (v3[i]-v1[i])*1e3);
		LoadR->SetTestResult(i, 0, (((v1[i]-v2[i])/v4[i])*100.0f)/300.0f);
	}

	vout.Set(FV, 0.0, PVI10A_5V, PVI10A_1A,RELAY_ON);
	en.Set(FV, 0.0, DVI400_10V, DVI400_400UA, RELAY_ON);
	vin.Set(FV, 0.0, PVI10A_10V, PVI10A_1A,RELAY_ON);
    cbit.SetOn(-1);
	return 0;
}

DUT_API int Ven(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Ven_h = StsGetParam(funcindex,"Ven_h");
    CParam *Ven_l = StsGetParam(funcindex,"Ven_l");
    //}}AFX_STS_PARAM_PROTOTYPES
	
	for(i=0; i<100; i++)  
	{
		adresult[i]=0.0f;
	}

	StsGetSiteStatus(sitestatus,SITENUM);

	cbit.SetOn(K1,-1);
	delay_ms(1);
    vin.Set(FV, 5.5f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
	en.Set(FV, 0.9f, DVI400_5V, DVI400_4MA, RELAY_ON);
//	vout.Set(FI, -1.0e-3f, PVI10A_5V, PVI10A_100MA,RELAY_ON);
	delay_ms(1);

	//////////// Ven_h ////////////
	for(i=0; i<SITENUM; i++)
	{
		check[i]=1.0f;
		if(sitestatus[i])
			check[i]=0.0f;
		//vmax=0.0f;
	}
	for(k=0.9f; k<1.4f; k=k+0.01f )  
	{
		checkadd=0.0f;
		for(i=0; i<SITENUM; i++ )  
		{
			BEGIN_SINGLE_SITE(i);
				if(check[i]<0.5f)	 en.Set(FV, k, DVI400_5V, DVI400_400UA, RELAY_ON);
				else                   vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
			END_SINGLE_SITE();
		}
		delay_us(200);
		vin.MeasureVI(MI,10,20);
		for(i=0; i<SITENUM; i++ )  
		{ 
			adresult[i+4]=vin.GetMeasResult(i);
			if(adresult[i+4]>0.0001f && check[i]<0.5f)
			{
				adresult[i+8]=k;
				check[i]=1.0f;
			}
			checkadd=checkadd+check[i];
		}
		//vmax=k;
		if(checkadd>3.5f)
		break;
	}
	for(i=0; i<SITENUM; i++ )  
	{ 
		Ven_h->SetTestResult(i, 0, adresult[i+8]);
	}

	//////////// Ven_l ////////////
	vout.Set(FI, 0.0e-3f, PVI10A_5V, PVI10A_100MA,RELAY_ON);
    vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
	vin.Set(FV, 5.5f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
	en.Set(FV, 1.4f, DVI400_5V, DVI400_4MA, RELAY_ON);
	vout.Set(FI, -1.0e-3f, PVI10A_5V, PVI10A_100MA,RELAY_ON);
	delay_ms(1);

	for(i=0; i<SITENUM; i++)
	{
		check[i]=1.0f;
		if(sitestatus[i])
			check[i]=0.0f;
		//vmin=0.0f;
	}
	for(k=1.4f; k>0.85f; k=k-0.01f )  
	{
		checkadd=0.0f;
		for(i=0; i<SITENUM; i++ )  
		{
			BEGIN_SINGLE_SITE(i);
				if(check[i]<0.5f)	  en.Set(FV, k, DVI400_5V, DVI400_400UA, RELAY_ON);
				else                    vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
			END_SINGLE_SITE();
		}
		delay_us(200);
		vin.MeasureVI(MI,10,20);
		for(i=0; i<SITENUM; i++ )  
		{ 
			adresult[i+12]=vin.GetMeasResult(i);
			if(adresult[i+12]<0.0001f && check[i]<0.5f)
			{
				adresult[i+16]=k;
				check[i]=1.0f;
			}
			checkadd=checkadd+check[i];
		}
		//vmin=k;
		if(checkadd>3.5f)
		break;
	}
	for(i=0; i<SITENUM; i++ )  
	{ 
		Ven_l->SetTestResult(i, 0, adresult[i+16]);
	}

	vout.Set(FI, 0.0f, PVI10A_5V, PVI10A_100MA,RELAY_ON);
	en.Set(FV, 0.0f, DVI400_5V, DVI400_40UA, RELAY_ON);
	vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
	cbit.SetOn(-1);
	return 0;
}

DUT_API int Dropout(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Dropout = StsGetParam(funcindex,"Dropout");
    CParam *Drop_vout = StsGetParam(funcindex,"Drop_vout");
    //}}AFX_STS_PARAM_PROTOTYPES
	
	double adresult[16]={0};
	double vx = 0;
	int flag[16]={0};  
	double result[16]={0};	
	double C=3.3;
		
	for(i=0;i<SITENUM;i++)
	{
			if((v1[i]>(C*(1-0.02))) && (v1[i]<(C*(1+0.02))))
				flag[i]=0;
			else
				flag[i]=1;
	}
	
	cbit.SetOn(K1,-1);
	delay_ms(1);
	//vout_m.Set(FI, 0.0, DVI400_5V, DVI400_40UA, RELAY_ON);
	vin.Set(FV, 3.9f, PVI10A_10V, PVI10A_1A, RELAY_ON);
	en.Set(FV, 2.0f, DVI400_5V, DVI400_4MA, RELAY_ON);
    delay_ms(1);
	vout.Set(FI,-200.0e-3, PVI10A_5V, PVI10A_1A,RELAY_ON);	
   
	for ( i=0; i<SITENUM; i++)
	{
		for (vx=3.9f; vx>(3.3f*0.9f); vx=vx-0.01f)
	    {
			vin.Set(FV, vx, PVI10A_10V, PVI10A_1A, RELAY_ON);
			delay_us(100);
			vout.MeasureVI(MV,10,10);
			adresult[i]=vout.GetMeasResult(i);
			if ((flag[i] == 0) && (adresult[i] < v1[i]*0.98)) //adresult[]<vout*95%则不扫描，根据扫描标志状态决定是否进行扫描判断
            {
					result[i] = vx;
					flag[i]=1;				//满足条件将扫描标志关闭
			}
			if (flag[i] == 1)// && (flag[1] == 1))
			break;				
	    }
	}

    for ( i=0; i<SITENUM; i++)
	{	
	    Dropout->SetTestResult(i, 0, (result[i]-adresult[i])*1e3);
		Drop_vout->SetTestResult(i, 0, adresult[i]);
	}

	vout.Set(FI, 0.0f, PVI10A_5V, PVI10A_1A, RELAY_ON);
	en.Set(FV, 0.0f, DVI400_5V, DVI400_4MA, RELAY_ON);
	vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_1A, RELAY_ON);
	cbit.SetOn(-1);
    return 0;
}

DUT_API int ICL(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *ICL = StsGetParam(funcindex,"ICL");
    //}}AFX_STS_PARAM_PROTOTYPES

	double adresult[16]={0};	
	int flag[16]={0};  

	StsGetSiteStatus(sitestatus,SITENUM);
	
	cbit.SetOn(K1,-1);
	delay_ms(1);
	vin.Set(FV, 5.5f, PVI10A_10V, PVI10A_1A,RELAY_ON);
	en.Set(FV, 2.0f, DVI400_5V, DVI400_400UA, RELAY_ON);
    delay_ms(1);
	vout.Set(FI,-250.0e-3, PVI10A_5V, PVI10A_1A,RELAY_ON);
    delay_ms(1);
		
	for(i=0;i<SITENUM;i++)
	{
	    BEGIN_SINGLE_SITE(i);
	    for (vx=-250.0e-3;vx>-700.0e-3;vx=vx-10.0e-3)
	    {
			vout.Set(FI, vx, PVI10A_5V, PVI10A_1A,RELAY_ON);
			delay_us(500);
			vout.MeasureVI(MV,10,10);
			//for ( i=0; i<SITENUM; i++)
			//{
			adresult[i]=vout.GetMeasResult(i);				
			if ((flag[i] == 0) && (adresult[i] < 0.5f)) //adresult[]<vout*95%则不扫描，根据扫描标志状态决定是否进行扫描判断
            {
					result[i] = vx;
					flag[i]=1;				//满足条件将扫描标志关闭
					vout.Set(FI, 0.0, PVI10A_5V, PVI10A_1A,RELAY_ON);
	                en.Set(FV, 0.0, DVI400_5V, DVI400_40UA, RELAY_ON);
	                vin.Set(FV, 0.0, PVI10A_10V, PVI10A_1A, RELAY_ON);
					cbit.SetOn(-1);
			}
			//}
			 if (flag[i] == 1)// && (flag[1] == 1))
			 break;				
	    }    	
	//}
    
	//for(i=0;i<SITENUM;i++)
	//{
	     ICL->SetTestResult(i, 0, -(result[i]*1e3));
		 END_SINGLE_SITE();
	}	

	vout.Set(FI, 0.0, PVI10A_5V, PVI10A_1A,RELAY_ON);
	en.Set(FV, 0.0, DVI400_5V, DVI400_40UA, RELAY_ON);
	vin.Set(FV, 0.0, PVI10A_10V, PVI10A_1A, RELAY_ON);
	cbit.SetOn(-1);
	vout.Set(FI, 0.0, PVI10A_5V, PVI10A_1A,RELAY_OFF);
	vin.Set(FV, 0.0, PVI10A_10V, PVI10A_1A, RELAY_OFF);
	return 0;
}

DUT_API int UVLO(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *UVLO_R = StsGetParam(funcindex,"UVLO_R");
    CParam *UVLO_F = StsGetParam(funcindex,"UVLO_F");
	CParam *UVLO_HYS = StsGetParam(funcindex,"UVLO_HYS");       
	//double E=Ven_h->GetConditionCurSelDouble(short(0));

	for(i=0; i<100; i++)  
	{
		adresult[i]=0.0f;
	}

	StsGetSiteStatus(sitestatus,SITENUM);

	cbit.SetOn(K1,-1);
	delay_ms(1);
	vin.Set(FV, 1.8f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
	en.Set(FV, 2.0f, DVI400_5V, DVI400_40UA, RELAY_ON);
//	vout.Set(FI,-1.0e-3, PVI10A_5V, PVI10A_100MA,RELAY_ON);
	delay_ms(1);
	//////////// UVLO_R ////////////
	for(i=0; i<SITENUM; i++)
	{
		check[i]=1.0f;
		if(sitestatus[i])
			check[i]=0.0f;
		//vmax=0.0f;
	}
	for(k=1.8f; k<2.5f; k=k+0.01f )  
	{
		checkadd=0.0f;
		for(i=0; i<SITENUM; i++ )  
		{
			BEGIN_SINGLE_SITE(i);
				if(check[i]<0.5f)	 vin.Set(FV, k, PVI10A_10V, PVI10A_100MA,RELAY_ON);
				else                   vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
			END_SINGLE_SITE();
		}
		delay_us(400);
		vin.MeasureVI(MI,10,20);
		for(i=0; i<SITENUM; i++ )  
		{ 
			adresult[i+4]=vin.GetMeasResult(i);
			if(adresult[i+4]>0.0009f && check[i]<0.5f)
			{
				adresult[i+8]=k;
				check[i]=1.0f;
			}
			checkadd=checkadd+check[i];
		}
		//vmax=k;
		if(checkadd>3.5f)
		break;
	}
	for(i=0; i<SITENUM; i++ )  
	{ 
		UVLO_R->SetTestResult(i, 0, adresult[i+8]-0.01f);
	}

	//////////// UVLO_F ////////////
	vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
	en.Set(FV, 0.0f, DVI400_5V, DVI400_40UA, RELAY_ON);
	vout.Set(FI, 0.0e-3, PVI10A_5V, PVI10A_100MA,RELAY_ON);
	vin.Set(FV, 2.3f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
	en.Set(FV, 2.0f, DVI400_5V, DVI400_40UA, RELAY_ON);
	vout.Set(FI,-1.0e-3, PVI10A_5V, PVI10A_100MA,RELAY_ON);
	delay_ms(1);

	for(i=0; i<SITENUM; i++)
	{
		check[i]=1.0f;
		if(sitestatus[i])
			check[i]=0.0f;
		//vmin=0.0f;
	}
	for (k=2.3f; k>1.7f; k= k-0.01f)
	{
		checkadd=0.0f;
		for(i=0; i<SITENUM; i++ )  
		{
			BEGIN_SINGLE_SITE(i);
				if(check[i]<0.5f)	  vin.Set(FV, k, PVI10A_10V, PVI10A_100MA,RELAY_ON);
				else                    vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
			END_SINGLE_SITE();
		}
		delay_us(100);
		vin.MeasureVI(MI,10,20);
		for(i=0; i<SITENUM; i++ )  
		{ 
			adresult[i+12]=vin.GetMeasResult(i);
			if(adresult[i+12]<0.00005f && check[i]<0.5f)
			{
				adresult[i+16]=k;
				check[i]=1.0f;
			}
			checkadd=checkadd+check[i];
		}
		//vmin=k;
		if(checkadd>3.5f)
		break;
	}

	for(i=0; i<SITENUM; i++ )  
	{ 
		UVLO_F->SetTestResult(i, 0, adresult[i+16]);
	    UVLO_HYS->SetTestResult(i, 0, ((adresult[i+8]-0.01f)-adresult[i+16])*1e3);
	}

	vout.Set(FI, 0.0f, PVI10A_5V, PVI10A_100MA,RELAY_ON);
	en.Set(FV, 0.0f, DVI400_5V, DVI400_40UA, RELAY_ON);
	vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
	cbit.SetOn(-1);
	return 0;
} 

DUT_API int OVP(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *OVP_R = StsGetParam(funcindex,"OVP_R");
    CParam *OVP_F = StsGetParam(funcindex,"OVP_F");
	CParam *OVP_HYS = StsGetParam(funcindex,"OVP_HYS");
    
	for(i=0; i<100; i++)  
	{
		adresult[i]=0.0f;
	}
	StsGetSiteStatus(sitestatus,SITENUM);
	cbit.SetOn(K1,-1);
	delay_ms(1);
	vin.Set(FV, 6.0f, PVI10A_10V, PVI10A_1A,RELAY_ON);
	en.Set(FV, 2.0f, DVI400_5V, DVI400_400UA, RELAY_ON);
	vout.Set(FI, -1.0e-3, PVI10A_5V, PVI10A_100MA,RELAY_ON);
	delay_ms(1);
	//////////// OVP_R ////////////
	for(i=0; i<SITENUM; i++)
	{
		check[i]=1.0f;
		if(sitestatus[i])
			check[i]=0.0f;
		//vmax=0.0f;
	}
	for (k=6.0f; k<6.8f; k=k+0.01f)
	{
		checkadd=0.0f;
		for(i=0; i<SITENUM; i++ )  
		{
			BEGIN_SINGLE_SITE(i);
				if(check[i]<0.5f)	 vin.Set(FV, k, PVI10A_10V, PVI10A_1A,RELAY_ON);
				else                   vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_1A,RELAY_ON);
			END_SINGLE_SITE();
		}
		delay_us(200);
		vin.MeasureVI(MI,10,20);
		for(i=0; i<SITENUM; i++ )  
		{ 
			adresult[i+4]=vin.GetMeasResult(i);
            if(adresult[i+4]<50.0e-6f && check[i]<0.5f)
			{
				adresult[i+8]=k;
				check[i]=1.0f;
			}
			checkadd=checkadd+check[i];
		}
		//vmax=k;
		if(checkadd>3.5f)
		break;
	}
	for(i=0; i<SITENUM; i++ )  
	{ 
		OVP_R->SetTestResult(i, 0, adresult[i+8]-0.02f);
	}

	//////////// OVP_F ////////////
	vout.Set(FI, 0.0e-3, PVI10A_5V, PVI10A_100MA,RELAY_ON);
	en.Set(FV, 0.0f, DVI400_5V, DVI400_400UA, RELAY_ON);
	vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_1A,RELAY_ON);

	vin.Set(FV, 6.5f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
	en.Set(FV, 2.0f, DVI400_5V, DVI400_400UA, RELAY_ON);
	vout.Set(FI, -0.1e-3, PVI10A_5V, PVI10A_100MA,RELAY_ON);
	delay_ms(1);

	for(i=0; i<SITENUM; i++)
	{
		check[i]=1.0f;
		if(sitestatus[i])
			check[i]=0.0f;
		//vmin=0.0f;
	}
	for (k=6.5f; k>5.5f; k=k-0.01f)
	{
		checkadd=0.0f;
		for(i=0; i<SITENUM; i++ )  
		{
			BEGIN_SINGLE_SITE(i);
				if(check[i]<0.5f)	  vin.Set(FV, k, PVI10A_10V, PVI10A_100MA,RELAY_ON);
				else                    vin.Set(FV, 0.0f, PVI10A_10V, PVI10A_100MA,RELAY_ON);
			END_SINGLE_SITE();
		}
		delay_us(200);
		vin.MeasureVI(MI,10,20);
		for(i=0; i<SITENUM; i++ )  
		{ 
			adresult[i+12]=vin.GetMeasResult(i);
            if(adresult[i+12]>90.0e-6f && check[i]<0.5f)
			{
				adresult[i+16]=k;
				check[i]=1.0f;
			}
			checkadd=checkadd+check[i];
		}
		//vmin=k;
		if(checkadd>3.5f)
		break;
	}

	for(i=0; i<SITENUM; i++ )  
	{ 
		OVP_F->SetTestResult(i, 0, adresult[i+16]);
	    OVP_HYS->SetTestResult(i, 0, ((adresult[i+8]-0.02f)-adresult[i+16])*1e3);
	}

	vout.Set(FI, 0.0, PVI10A_5V, PVI10A_100MA,RELAY_ON);
	en.Set(FV, 0.0, DVI400_5V, DVI400_400UA, RELAY_ON);
	vin.Set(FV, 0.0, PVI10A_10V, PVI10A_100MA,RELAY_ON);
	cbit.SetOn(-1);
	return 0;
}

DUT_API int Rdischarge(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Rdischarge = StsGetParam(funcindex,"Rdischarge");
	//}}AFX_STS_PARAM_PROTOTYPES

	double adresult[16]={0};
	
	vin.Set(FV, 5.0f, PVI10A_10V, PVI10A_100MA, RELAY_ON);
	en.Set(FV, 0.0f, DVI400_5V, DVI400_400UA, RELAY_ON);
	vout.Set(FV, 1.0f, PVI10A_5V, PVI10A_10MA, RELAY_ON);
    delay_ms(1);
	
	vout.MeasureVI(MI, 10, 20);	
	for(i=0;i<SITENUM;i++)
	{
		adresult[i]=vout.GetMeasResult(i);
		Rdischarge->SetTestResult(i, 0, 1/adresult[i]);
	}
    
	vin.Set(FV, 0.0, PVI10A_10V, PVI10A_100UA, RELAY_ON);
    vout.Set(FV, 0.0f, PVI10A_5V, PVI10A_10MA, RELAY_ON);
	return 0;
}