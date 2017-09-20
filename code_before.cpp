#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "string.h"
#include <cstring> 
#include <cstdlib>
#include <cmath>



/*Double_t fitSqrt(Double_t *x, Double_t *par){
    return par[0]+par[1]*sqrt(x[0]);
}    */
//===========================================Using Function================================================
//=========================================================================================================
Double_t fitSqrt(double *x, double *par) {
   double s;
   s = sqrt( par[0]*par[0] + par[1]*par[1]*x[0]/2.9 );
   return s;
}

void Save_Param_P1(TString Name,double param)
{
    ofstream fout;  //create stream
    fout.open("el_noise_paramP1.txt", ios::in); //tell where file
    fout.seekp(0, ios::end); //move to end string
    
    fout << Name<<"| el noise (parametr p1) =" << param << endl; //write string
    fout.close(); // close file
}

double paramP0 (TString NameCell)
{
    ifstream fin("el_noise_mc_August_2016.txt");
 
    double value;
    std::string str;
    std::string buf;
    if (fin)
    {
        TString word;
        while (fin >> word)
        {
            //i=i+1;
            int k=strcmp(word,NameCell);
            //cout<<k<<" "<<endl;
            if (k==0)
            {
                
                getline(fin,str);
                
                int i=0;
                int k;
                while (i<strlen(str.c_str()))
                {
                    if (str[i]=='	')  k=i+1;
                    i=i+1;
                }
        
                int j=0;
                int s;
                while (j<strlen(str.c_str()))
                {
                    if (str[j]!=' ' && str[j+1]==' ' && j>k-1)  s=i;
                    j=j+1;
                }
                //cout<<s<<" "<<endl;
                
                str.erase(str.begin()+s+1, str.begin()+strlen(str.c_str()) );
                str.erase(str.begin(), str.begin()+k );
                
                buf=str;
                //cout<<buf<<""<<endl;
                //cout<<"---------------------"<<endl;
                number = atof( buf.c_str() );
                cout<<number<<""<<endl;
            }
            
            fin.ignore(255, '\n'); 
        }
    }
    fin.close();
    cout << number << endl;
    return number;
}

//=================================================MAIN=================================================================
//The main body of the program
//======================================================================================================================


void codeNomUpgrade(TString NameCell,
                    int sect,
                    int side,
                    int samp,
                    int tow ,
                    bool spesh,
                    bool isBS)
{

//=================================================Reading==============================================================
//Reading root file to after calculation
    cout << "Hello" << endl;
	TString NameFile;
	TString NameHistogram;
    Upgrade=0;
    
    
    TString Temp_SP;
    TString Temp_BSG;

    
    
    //Flag check to Bootstrap Generator, indepedant variable = NameHistogram
    if(isBS == true)
    {
        Temp_BSG="BS_Errors_RMS";
    }
    else
    {
        Temp_BSG="averageIntPerXing_VS_cellEnergy";
    }
    //Flag check to Special Cells
    if(spesh == false)
    {
        Temp_SP="";
    }
    else
    {
        if((NameCell=="MBE+1") || (NameCell=="MBE-1"))
        {
            Temp_SP="/Special/More";
        }
        else
        {
            Temp_SP="/Special";
        }
    }
    
    
    // independant variables per flag = Upgrade, isBS, spesh
    TString Folder_SRC=TString::Format("20161109_mc16_valid_A3TUNE%s",Temp_SP.Data());
    
    NameFile = TString::Format("%s/MC_Sect%d_Side%d_Samp%d_Tow%d.root",Folder_SRC.Data(),sect,side,samp,tow);
    
    NameHistogram= TString::Format("cells/%s",Temp_BSG.Data());
    
    TString Run=TString::Format("runNoise");
    
    //Flag-independent variables
    TFile *SourceFile= TFile::Open(NameFile,"read");
    TH2D *HistBS = (TH2D*)SourceFile->Get(NameHistogram);
    
    

    
    
//correct ==== done !
    
//=================================================Projection==============================================================
//Need to crate histogram with projection every bin(mu) = Energy per bunch crossing

  	
    TString ProjectionHist="Projection";
	TString NoiseTable="Table";
  	TString FolderProjection= Form("%s%s",ProjectionHist.Data(),NameCell.Data());
  	
	system(TString::Format("mkdir -p %s/%s",Run.Data(),NoiseTable.Data()));
  	system(TString::Format("mkdir -p %s/%s/%s",Run.Data(),ProjectionHist.Data(),FolderProjection.Data()));
    
    
	//decloration to create RMS Graph
  	vector<float> x;
	vector<float> y;
	vector<float> ex;
  	vector<float> ey;
    
  	int NbinsMu=51;
  	for(int i=1; i<NbinsMu;i++ )
	{
        TCanvas *histCanvas=new TCanvas (TString::Format("Projection for bin%d",i),TString::Format("Projection for bin%d",i));
        TH1F *ProjBS= (TH1F*)HistBS->ProjectionY(Form("ProjBS_%d",i),i,i);
        ProjBS->GetXaxis()->SetRangeUser(-300,300);
        
        if (0 == ProjBS->GetRMS()) continue;
        x.push_back(i-0.5);
        ex.push_back(0.5);
        
        if (isBS==0)
		{
			y.push_back(ProjBS->GetRMS());
            ey.push_back(ProjBS->GetRMSError());
			//projBS->SetAxisRange(-5000,5000,"X");
			ProjBS->Fit("gaus");
			gPad-> SetLogy();
		}
		else
		{
			y.push_back(ProjBS->GetMean());
			//cout<<projBS->GetMean()<<""<<endl;
            ey.push_back(ProjBS->GetRMS());
			//projBS->SetAxisRange(0,1000,"X");
			ProjBS->Fit("gaus");
		}
        
        ProjBS->Draw();

        TString ProjSave=Form("%s/%s/%s/projBS_%d.png",Run.Data(),ProjectionHist.Data(),FolderProjection.Data(),i);
        cout << ProjSave << endl;
        histCanvas->SaveAs(ProjSave);
        //delete cc;
   	}

    //maybe correct = done!

//=================================================Canvas==============================================================
//Creating canvas with projection with atlas style
    
    
	TCanvas *CanvasBS=new TCanvas ("Hist","Hist",1440,1080);
   	CanvasBS->Divide(2,1);
   	Double_t xStart1 = 0.01, xStart2 = 0.65, xStart3 = 0.99;
   	CanvasBS->cd(1)->SetPad(xStart1,.01,xStart2,.99);
   	CanvasBS->cd(2)->SetPad(xStart2,.01,xStart3,.99);
   	CanvasBS->cd(1)->SetRightMargin(0.01);
   	CanvasBS->cd(2)->SetLeftMargin(0.01);
   
   	CanvasBS->cd(1);
   	TGraphErrors *Graph_RMS = new TGraphErrors(x.size(),&(x[0]),&(y[0]),&(ex[0]),&(ey[0]));
   	Graph_RMS->SetMarkerStyle(24);
   	Graph_RMS->SetMarkerColor(620);
   	Graph_RMS->SetTitle(";<#mu>;Noise [MeV]");
   	Graph_RMS->Draw("ap");

	double Number=paramP0(NameCell);
	//cout<<numb<<" "<<endl;
	int mumin=1;
   	int mumax=51;
   	TF1 *func_to_aprocsimation = new TF1("fitsqrt", fitSqrt, mumin,mumax,2);
	
    //func_to_aprocsimation->SetParLimits(0,0,1000);
    paramP0 = func_to_aprocsimation->SetParameter();
    func_to_aprocsimation->SetParameter(0,50);
    Graph_RMS->Fit("fitsqrt","R","",0,50);
	
    /*else
	{
		cout<<"It isn't fix electron noise"<<endl;
		func_to_aprocsimation->SetParLimits(1,0,1000);
        func_to_aprocsimation->SetParameter(0,20);
		func_to_aprocsimation->SetParameter(1,20);
		Graph_RMS->Fit("fitsqrt","R","",1,51);
	} */

//=====================================================================================================================

   	TF1 *func_oder = Graph_RMS->GetFunction("fitsqrt");
   	float p0=func_to_aprocsimation->GetParameter(0);
   	float p1=func_to_aprocsimation->GetParameter(1);
   	cout<<p0<<"  " << p1 << "  "<<endl;
    Save_Param_P1(NameCell,p1);
    
   	//TString param;
   	//param = TString::Format("p0=%0.3f   p1=%0.3f",p0,p1);
   	//TText *t=new TText(1.0,5.5,param );
   	//t->SetTextSize(0.03); 
   	//t->Draw(); 

	TFile *New_File = new TFile (TString::Format("%s/%s/newfile%s.root",Run.Data(),NoiseTable.Data(),NameCell.Data()),"recreate");
   	Graph_RMS->Write();
   	func_to_aprocsimation->Write();
   	New_File->Close();

	
	/*  TLegend *leg = new TLegend(0.15,0.8,0.3,0.9);
   	leg->SetFillColor(0);
   	leg->SetFillStyle(0);
   	leg->SetBorderSize(0);
   	leg->SetTextFont(72);
   	leg->SetTextSize(0.04);
   	// leg->AddEntry(fitSqrt,"Cell %s",NameCell,"le"); 
   	// leg->AddEntry(fitSqrt,"et. noise #mu=1","l");
   	leg->Draw(""); */


   
   	CanvasBS->cd(2);
   	float leg_height2 = 0.13;   
   	float leg_width2 = 0.1;
   	float leg_xoffset2 = 0.7;
   	float leg_yoffset2 = 0.9;
   	TLegend *leg_name = new TLegend(0.15,0.5,0.3,0.90);
   	leg_name->SetFillColor(0);
   	leg_name->SetFillStyle(0);
   	leg_name->SetBorderSize(0);
   	leg_name->SetTextFont(72);
   	leg_name->SetTextSize(0.06);
   	leg_name->AddEntry("","ATLAS Internal","");
   	leg_name->AddEntry("","Tile Calorimeter","");
   	leg_name->AddEntry("","MC15 13 TeV [COF]","");
   	//leg_name->AddEntry("","#eta=0.45   Right side","");
  	leg_name->AddEntry("","","");
   	leg_name->AddEntry("","BootStrap","");
   	leg_name->AddEntry("",(TString::Format("%s",NameCell.Data()) ),"");
   	cout<<p0<<" "<<endl;
   	leg_name->AddEntry("", (TString::Format("f=#sqrt{%0.1f^{2}+ %0.1f^{2}*#mu/2.9}",p0,p1)) ,"");
   	leg_name->AddEntry("", (TString::Format("#chi^{2}=%0.1f  ndf=%d",func_to_aprocsimation->GetChisquare(),func_to_aprocsimation->GetNDF()) ),"");
   	leg_name->AddEntry("","____________________","");
   	leg_name->Draw("");


	/* canv->cd(1);
   	TLine *line= new TLine(0,27,2,27);
   	line->SetLineColor(2);
   	line->SetLineWidth(4);
   	line->SetLineStyle(1);
   	line->Draw();

   	TLine *line2= new TLine(2,27,45,27);
   	line2->SetLineColor(2);
   	line2->SetLineWidth(2);
   	line2->SetLineStyle(2);
   	line2->Draw("same");      */
  
   	CanvasBS->SaveAs(TString::Format("%s/RMSvsMu_forCell_%s.png",Run.Data(),NameCell.Data()));
}
