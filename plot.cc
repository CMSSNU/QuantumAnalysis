/*
TString hsseopath="/cms/scratch/hsseo/SKFlatOutput/v949cand2_2/FirstAnalyzer/2017/";
const int nfile=8;
TString filenames[nfile]={"FirstAnalyzer_DoubleMuon.root","FirstAnalyzer_DYJets.root","FirstAnalyzer_DYJets.root","FirstAnalyzer_WW_pythia.root","FirstAnalyzer_WZ_pythia.root","FirstAnalyzer_ZZ_pythia.root","FirstAnalyzer_WJets_MG.root","FirstAnalyzer_TTLL_powheg.root"};
int sampleindex[nfile]={0,1,2,3,3,3,4,5};
*/

/*
TString hsseopath="/data2/CAT_SKTreeOutput/JobOutPut/hsseo/LQanalyzer/data/output/CAT/ISR2016MuonAnalyzer/periodBtoH/";
#include<vector>
const int nfile=10;
TString filenames[nfile]={"ISR2016MuonAnalyzer_data_DoubleMuon_cat_v8-0-7.root","ISR2016MuonAnalyzer_DYJets_cat_v8-0-7.root","ISR2016MuonAnalyzer_DYJets_10to50_cat_v8-0-7.root","ISR2016MuonAnalyzer_DYJets_cat_v8-0-7.root","ISR2016MuonAnalyzer_DYJets_10to50_cat_v8-0-7.root","ISR2016MuonAnalyzer_SKWW_dilep_cat_v8-0-7.root","ISR2016MuonAnalyzer_SKWZ_dilep_cat_v8-0-7.root","ISR2016MuonAnalyzer_SKZZ_dilep_cat_v8-0-7.root","ISR2016MuonAnalyzer_SKWJets_dilep_cat_v8-0-7.root","ISR2016MuonAnalyzer_SKTT_powheg_dilep_cat_v8-0-7.root"};
int sampleindex[nfile]={0,1,1,2,2,3,3,3,4,5};
*/

#include<iostream>
#include<vector>
#include"TH2D.h"
#include"TFile.h"
#include"TKey.h"
#include"TString.h"
#include"THStack.h"
#include"TLegend.h"
#include"TCanvas.h"
#include"TLine.h"
using namespace std;
TString filedir="/data7/Users/salee/SKFlatOutput/v949cand2_2/SMPValidation/2017/";
const int nfile=12;
TString filenames[nfile]={"DATA/SMPValidation_DoubleMuon_B.root","DATA/SMPValidation_DoubleMuon_C.root","DATA/SMPValidation_DoubleMuon_D.root","DATA/SMPValidation_DoubleMuon_E.root","DATA/SMPValidation_DoubleMuon_F.root","SMPValidation_DYJets.root","SMPValidation_DYJets.root","SMPValidation_WW_pythia.root","SMPValidation_WZ_pythia.root","SMPValidation_ZZ_pythia.root","SMPValidation_WJets_MG.root","SMPValidation_TTLL_powheg.root"};
int sampleindex[nfile]={0,0,0,0,0,1,2,3,3,3,4,5};

const int nsample=6;
TString samplenames[nsample]={"data","#gamma*/Z#rightarrow#mu#mu","#gamma*/Z#rightarrow#tau#tau","Diboson","W","t#bar{t}"};
int colorcode[nsample]={1,2,3,4,5,6};

const int nsys=14;
TString sysname[nsys]={"RECOSF","IDSF","ISOSF","triggerSF","PUreweight","alphaS","scalevariation","noRECOSF","noIDSF","noISOSF","notriggerSF","noPUreweight","noefficiencySF","totalsys"};
int systype[nsys]={2,2,2,2,2,2,9,1,1,1,1,1,1,-((1<<7)-1)};
int sysexist_data[nsys]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int sysexist_bg[nsys]={1,1,1,1,1,0,0,1,1,1,1,1,1,0};

void PrintKeys(TList* keys){
  for(int i=0;i<keys->GetSize();i++){
    TKey* key=(TKey*)keys->At(i);
    if(strcmp(key->GetClassName(),"TDirectoryFile")==0) PrintKeys(((TDirectoryFile*)key->ReadObj())->GetListOfKeys());   
    else cout<<key->GetName()<<"\n";
  }
}
void PrintHistList(int filenum=1){
  TFile f(filedir+filenames[filenum]);
  cout<<f.GetName()<<" "<<f.GetTitle()<<endl;
  PrintKeys(f.GetListOfKeys());
}
TH1* GetHist(TString filename,TString histname){
  TFile f(filename);
  TH1* hist=(TH1*)f.Get(histname);
  if(hist){
    hist->SetDirectory(0);
    return hist;
  }else{
    cout<<"no "<<histname<<" in "<<filename<<endl;
    return NULL;
  }
}
vector<TH1*> GetHists(TString dirname,TString datahistname,TString dyhistname,TString bghistname){
  vector<TH1*> hists;
  vector<TString> histnames;
  for(int i=0;i<nfile;i++){
    if(sampleindex[i]==0) histnames.push_back(datahistname);
    else if(sampleindex[i]==1) histnames.push_back(dyhistname);
    else if(sampleindex[i]==2) histnames.push_back(bghistname(0,bghistname.Last('/')+1)+"tau_"+bghistname(bghistname.Last('/')+1,bghistname.Length()));
    else histnames.push_back(bghistname);
  }					   
  for(int i=0;i<nfile;i++){
    TH1* hist=GetHist(dirname+filenames[i],histnames[i]);
    if(hist){
      hist->SetName(samplenames[sampleindex[i]]);
      hist->SetTitle(dyhistname);
      hist->GetXaxis()->SetTitle(dyhistname);
      hist->SetLineColor(colorcode[sampleindex[i]]);
      hist->SetFillColor(colorcode[sampleindex[i]]);
      bool isexist=false;
      for(int j=0;j<(int)hists.size();j++){
	if(strcmp(hists.at(j)->GetName(),hist->GetName())==0){
	  hists.at(j)->Add(hist);
	  isexist=true;
	  continue;
	}
      }
      if(!isexist) hists.push_back(hist);
    }
  }
  return hists;
}
vector<TH1*> GetHists(TString dirname,TString histname){
  return GetHists(dirname,histname,histname,histname);
}

THStack *GetStack(vector<TH1*> hists){
  THStack* hstack=new THStack;
  for(int i=(int)hists.size()-1;i>0;i--){
    hstack->Add(hists.at(i),"HIST");
  }
  return hstack;
}
TH1* GetMCHist(const vector<TH1*>& hists){
  TH1* hist=(TH1*)hists.at(1)->Clone("mchist");
  for(int i=2;i<(int)hists.size();i++) hist->Add(hists.at(i));
  return hist;
}
TLegend* GetLegend(vector<TH1*> hists){
  TLegend* legend=new TLegend(0.67,0.4,0.89,0.88);
  for(int i=0;i<(int)hists.size();i++){
    TString option="f";
    if(i==0) option="lp";
    legend->AddEntry(hists.at(i),hists.at(i)->GetName(),option);
  }
  legend->SetBorderSize(0);
  return legend;
}
TH1* GetSysHistMax(TH1* central,vector<TH1*> variations){
  TH1* syshist=(TH1*)central->Clone("sys");
  for(int i=1;i<syshist->GetNbinsX()+1;i++) syshist->SetBinError(i,0);
  for(int i=0;i<(int)variations.size();i++){
    for(int j=0;j<syshist->GetNbinsX()+1;j++){
      double diff=fabs(syshist->GetBinContent(j)-variations.at(i)->GetBinContent(j));
      if(diff>syshist->GetBinError(j)) syshist->SetBinError(j,diff);
    }
  }
  return syshist;
}
TH1* GetSysHistMax(TH1* central,TH1* variation1,TH1* variation2=NULL,TH1* variation3=NULL,TH1* variation4=NULL,TH1* variation5=NULL,TH1* variation6=NULL,TH1* variation7=NULL,TH1* variation8=NULL,TH1* variation9=NULL){
  vector<TH1*> variations;
  if(variation1) variations.push_back(variation1);
  if(variation2) variations.push_back(variation2);
  if(variation3) variations.push_back(variation3);
  if(variation4) variations.push_back(variation4);
  if(variation5) variations.push_back(variation5);
  if(variation6) variations.push_back(variation6);
  if(variation7) variations.push_back(variation7);
  if(variation8) variations.push_back(variation8);
  if(variation9) variations.push_back(variation9);
  return GetSysHistMax(central,variations);
}    
int AddError(TH1* hist,TH1* sys){
  for(int i=1;i<hist->GetNbinsX()+1;i++){
    if(fabs(hist->GetBinContent(i)-sys->GetBinContent(i))>0.000001*hist->GetBinContent(i)){
      cout<<"[AddError] sys hist is wrong"<<endl;
      return -1;
    }
  }
  for(int i=1;i<hist->GetNbinsX()+1;i++){
    double err1=hist->GetBinError(i);
    double err2=sys->GetBinError(i);
    hist->SetBinError(i,sqrt(err1*err1+err2*err2));
  }
  return 1;
} 
TCanvas* GetCompareStack(vector<TH1*> hists,TH1* sys=NULL,int rebin=0,double xmin=0,double xmax=0,bool setlog=0){
  if(rebin){
    if(sys) sys->Rebin(rebin);
    for(int i=0;i<(int)hists.size();i++) hists.at(i)->Rebin(rebin);
  }
  if(xmin||xmax){
    if(sys) sys->GetXaxis()->SetRangeUser(xmin,xmax);
    for(int i=0;i<(int)hists.size();i++) hists.at(i)->GetXaxis()->SetRangeUser(xmin,xmax);
  }
  TCanvas* c1=new TCanvas(hists.at(1)->GetTitle(),hists.at(1)->GetTitle(),800,800);
  c1->Divide(1,2);
  c1->cd(1);
  gPad->SetPad(0,0.35,1,1);
  gPad->SetBottomMargin(0.02);
  hists.at(0)->Draw("e");
  hists.at(0)->GetXaxis()->SetLabelSize(0);
  hists.at(0)->GetXaxis()->SetTitle("");
  hists.at(0)->SetStats(0);
  hists.at(0)->SetMarkerStyle(20);
  hists.at(0)->SetMarkerSize(0.7);
  THStack* hstack=GetStack(hists);
  hstack->Draw("same");

  TH1* mchist=GetMCHist(hists);
  TH1* mchistsys=(TH1*)mchist->Clone("mchistsys");
  if(sys) AddError(mchistsys,sys);
  mchistsys->SetFillStyle(3002);
  mchistsys->SetFillColor(4);
  mchistsys->Draw("same e2");

  TLegend* legend=GetLegend(hists);
  legend->Draw();

  if(hists.at(0)->GetMaximum()<mchist->GetMaximum()) hists.at(0)->GetYaxis()->SetRangeUser(0,mchist->GetMaximum()*1.1);
  if(setlog){
    if(hists.at(0)->GetMinimum()==0) hists.at(0)->SetMinimum(1000);
    gPad->SetLogy();
  }
  hists.at(0)->Draw("same a e");

  c1->cd(2);
  gPad->SetPad(0,0,1,0.35);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.2);
  gPad->SetGridx();gPad->SetGridy();
  TH1* ratio=(TH1*)hists.at(0)->Clone("ratio");
  ratio->SetTitle("");
  ratio->SetStats(0);
  ratio->Divide(mchist);
  ratio->Draw();
  ratio->GetYaxis()->SetTitle("Data/Simulation");
  ratio->GetYaxis()->SetTitleSize(0.1);
  ratio->GetYaxis()->SetTitleOffset(0.5);
  ratio->GetYaxis()->SetRangeUser(0.8,1.2);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTitle(hists.at(0)->GetTitle());
  ratio->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetLabelSize(0.1);
  TLine *line=new TLine(xmin?xmin:ratio->GetXaxis()->GetXmin(),1,xmax?xmax:ratio->GetXaxis()->GetXmax(),1);
  line->SetLineColor(2);
  line->Draw();
  if(sys){
    TH1* ratiosys=(TH1*)sys->Clone("ratiosys");
    for(int i=1;i<ratiosys->GetNbinsX()+1;i++){
      double y=mchist->GetBinContent(i);
      if(y==0) continue;
      ratiosys->SetBinContent(i,ratiosys->GetBinContent(i)/y);
      ratiosys->SetBinError(i,ratiosys->GetBinError(i)/y);  
    }
    ratiosys->SetFillStyle(3002);
    ratiosys->SetFillColor(4);
    ratiosys->Draw("same p e2");
    TLegend* syslegend=new TLegend(0.6,0.75,0.89,0.95);
    syslegend->AddEntry(ratio,"Data/Simulation (Stat.)","lp");
    syslegend->AddEntry(ratiosys,"Syst.","f");
    syslegend->Draw();
  }
  ratio->Draw("same");

  return c1;
}
TCanvas* GetCompareStack(TString histname,int sysbit=0,int rebin=0,double xmin=0,double xmax=0,bool setlog=0){
  vector<TH1*> hists_central=GetHists(filedir,histname,histname,histname);
  TH1* central=GetMCHist(hists_central);
  vector<TH1*> syss;
  for(int i=0;i<nsys;i++){
    if((sysbit>>i)%2==1){
      cout<<"systype="<<systype[i]<<endl;
      if(systype[i]<1) continue;
      vector<TString> suffix;
      if(systype[i]==1) suffix.push_back("");
      else if(systype[i]==2){
	suffix.push_back("_up");
	suffix.push_back("_down");
      }else if(systype[i]>2){
	for(int j=0;j<systype[i];j++) suffix.push_back(Form("%d",j));
      }
      vector<TH1*> variations;
      for(int j=0;j<systype[i];j++){
	if((systype[i]==9&&j==4)||(systype[i]==9&&j==8)) continue;
	TString datahistname=histname+(sysexist_data[i]?("_"+sysname[i]+suffix[j]):"");
	TString dyhistname=histname+"_"+sysname[i]+suffix[j];
	TString bghistname=histname+(sysexist_bg[i]?("_"+sysname[i]+suffix[j]):"");
	cout<<datahistname<<" "<<dyhistname<<" "<<bghistname<<endl;
	vector<TH1*> gethists=GetHists(filedir,datahistname,dyhistname,bghistname);
	variations.push_back(GetMCHist(gethists));
	for(int k=0;k<(int)gethists.size();k++) delete gethists.at(k);
      }
      syss.push_back(GetSysHistMax(central,variations));
      for(int j=0;j<(int)variations.size();j++) delete variations.at(j);
      cout<<sysname[i]+": "<<variations.size()<<" variations"<<endl;
    }
  }
  TH1* sys_total=NULL;
  if(syss.size()>0){
    for(int i=0;i<(int)syss.size();i++){
      if(i==0) sys_total=(TH1*)syss.at(0)->Clone("sys");
      else AddError(sys_total,syss.at(i));
      delete syss.at(i);
    }
  }
  return GetCompareStack(hists_central,sys_total,rebin,xmin,xmax,setlog);
}
void SaveAll(){
  const int nhist=14;
  TString histname[nhist]={"muon2017/OS/Z/dimass","muon2017/OS/Z/dipt","muon2017/OS/Z/dirap","muon2017/OS/Z/l0pt","muon2017/OS/Z/l0eta","muon2017/OS/Z/l1pt","muon2017/OS/Z/l1eta","muon2017/OS/dimass","muon2017/OS/dipt","muon2017/OS/dirap","muon2017/OS/l0pt","muon2017/OS/l0eta","muon2017/OS/l1pt","muon2017/OS/l1eta"};
  int histrebin[nhist]={0,4,0,4,0,4,0, 4,4,0,4,0,4,0};
  double histxmin[nhist]={80,0,0,0,0,0,0, 0,0,0,0,0,0,0};
  double histxmax[nhist]={100,0,0,0,0,0,0, 0,0,0,0,0,0,0};
  for(int i=0;i<nsys;i++) 
    for(int j=0;j<nhist;j++){
      TString this_histname=histname[j];
      cout<<"mkdir -p plot/"+this_histname(0,this_histname.Last('/')+1)+sysname[i]<<endl;
      system("mkdir -p plot/"+this_histname(0,this_histname.Last('/')+1)+sysname[i]);
    }
  TCanvas* c=NULL;
  for(int i=0;i<nhist;i++){
    TString this_histname=histname[i];
    cout<<this_histname<<" "<<histrebin[i]<<" "<<histxmin[i]<<" "<<histxmax[i]<<endl;
    c=GetCompareStack(this_histname,0,histrebin[i],histxmin[i],histxmax[i]);
    c->SaveAs("plot/"+this_histname+".png");
    delete c;
    for(int j=0;j<nsys;j++){
      int sysbit=0;
      TCanvas* c;
      if(systype[j]>1) c=GetCompareStack(this_histname,1<<j,histrebin[i],histxmin[i],histxmax[i]);
      else if(systype[j]==1) c=GetCompareStack(GetHists(filedir,this_histname+(sysexist_data[j]?"_"+sysname[j]:""),this_histname+"_"+sysname[j],this_histname+(sysexist_bg[j]?"_"+sysname[j]:"")),0,histrebin[i],histxmin[i],histxmax[i]);
      else c=GetCompareStack(this_histname,-systype[j],histrebin[i],histxmin[i],histxmax[i]);
      c->SaveAs("plot/"+this_histname(0,this_histname.Last('/')+1)+sysname[j]+this_histname(this_histname.Last('/'),this_histname.Length())+".png");
      delete c;
    }      
  }
}
/*
void SaveAll_ISR2016Muon(){
  const int nhist=16;
  TString prefix="Hists/";
  TString histname[nhist]={"dimass","dipt","met","nvtx","l1pt","l2pt","l1eta","l2eta","dimass_m5","dipt_m5","met_m5","nvtx_m5","l1pt_m5","l2pt_m5","l1eta_m5","l2eta_m5"};
  int histrebin[nhist]={4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  double histxmax[nhist]={0,0,0,0,0,0,0,0,80,0,0,0,0,0,0,0};
  double histxmin[nhist]={0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0};
  for(int i=0;i<nsys;i++) system("mkdir -p plot/"+sysname[i]);
  system("mkdir -p plot/totalsys");
  TCanvas* c=NULL;
  for(int i=0;i<nhist;i++){
    c=GetCompareStack(prefix+histname[i],0,histrebin[i],histxmax[i],histxmin[i]);
    c->SaveAs("plot/"+histname[i]+".png");
    delete c;
    c=GetCompareStack(prefix+histname[i],(1<<nsys)-1,histrebin[i],histxmax[i],histxmin[i]);
    c->SaveAs("plot/totalsys/"+histname[i]+".png");
    delete c;
    for(int j=0;j<nsys;j++){
      system("mkdir -p plot/"+sysname[j]);
      TCanvas* c=GetCompareStack(prefix+histname[i],1<<j,histrebin[i],histxmax[i],histxmin[i]);
      c->SaveAs("plot/"+sysname[j]+"/"+histname[i]+".png");
      delete c;
    }      
    for(int j=0;j<nno;j++){
      system("mkdir -p plot/no"+sysname[j]);
      TCanvas* c=GetCompareStack(GetHists(filedir,prefix+histname[i]+(noexist_data[j]?("_no"+sysname[j]):""),prefix+histname[i]+"_no"+sysname[j],prefix+histname[i]+(noexist_bg[j]?("_no"+sysname[j]):"")),0,histrebin[i],histxmax[i],histxmin[i]);
      c->SaveAs("plot/no"+sysname[j]+"/"+histname[i]+"_no"+sysname[j]+".png");
      delete c;
    }      
  }
}
*/

