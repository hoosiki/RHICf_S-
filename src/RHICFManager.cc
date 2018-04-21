#include "RHICFManager.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <utility>
#include <sstream>
#include <iomanip>
#include <map>
#include <string>

//SINGLETON FOR STORING PARAMETER WHILE PROGRAM RUNNING

bool RHICFManager::RHICFManagerFlag = false;

RHICFManager* RHICFManager::fRHICFManager = NULL;

RHICFManager::RHICFManager() 
{
  SetTables();
  SetMap();
  ARM1X = 0.;
  ARM1Y = 0.;
  ARM1Z = 0.;
  trigger = false;
}

RHICFManager::~RHICFManager()
{
  RHICFManagerFlag = false;
}

void RHICFManager::SetARM1X(G4double x)
{
  ARM1X = x;
}

void RHICFManager::SetARM1Y(G4double y)
{
  ARM1Y = y;
}

void RHICFManager::SetARM1Z(G4double z)
{
  ARM1Z = z;
}

void RHICFManager::SetMomentumX(G4double x)
{
  MomentumX = x;
}

void RHICFManager::SetMomentumY(G4double y)
{
  MomentumY = y;
}

void RHICFManager::SetMomentumZ(G4double z)
{
  MomentumZ = z;
}

void RHICFManager::SetVerboseSW(G4bool tr)
{
  trigger = tr;
}

void RHICFManager::ShowDInfo(G4String s, G4double d)
{
  if (trigger==true) 
  {
    G4cout << s+" : " << d << G4endl;
  }
}

void RHICFManager::ShowIInfo(G4String s, G4int i)
{
  if (trigger==true) 
  {
    G4cout << s+" : " << i << G4endl;
  }
}

void RHICFManager::ShowSInfo(G4String s, G4String ss)
{
  if (trigger==true) 
  {
    G4cout << s+" : " << ss << G4endl;
  }
}

RHICFManager* RHICFManager::GetInstance()
{
  if(!fRHICFManager)
  {
    fRHICFManager = new RHICFManager();
    RHICFManagerFlag = true;
  }
  return fRHICFManager;


}

void RHICFManager::SetParticleName(G4String string)
{
  fParticleName = string;
}

void RHICFManager::SetParticleNumber(G4int num)
{
  fParticleNum = num;
}

void RHICFManager::SetTables()
{
  std::string tmpstring(getenv("ETC"));
  tmpstring = tmpstring + "/tables/GSOplateMap.root";
  fTable = new TFile(tmpstring.c_str());
  for (int i = 0; i < 2; i++) 
  {
    for (int j = 0; j < 16; j++) 
    {
      std::ostringstream ostr;
      ostr << std::setfill('0') << std::setw(2) << (j < 0 ? -j : j);
      tmpstring = "map_tow"+std::to_string(i)+"_plate"+ostr.str();
      fLightCollection[i][j] = (TGraph2D*)fTable->Get(tmpstring.c_str());
    }
  }

}

void RHICFManager::Initialize()
{
  for (auto &var : dEwithLightCollection) 
  {
      var = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  }
}

void RHICFManager::SetMap()
{
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small00_PV",std::make_pair(0,0)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small01_PV",std::make_pair(0,1)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small02_PV",std::make_pair(0,2)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small03_PV",std::make_pair(0,3)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small04_PV",std::make_pair(0,4)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small05_PV",std::make_pair(0,5)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small06_PV",std::make_pair(0,6)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small07_PV",std::make_pair(0,7)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small08_PV",std::make_pair(0,8)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small09_PV",std::make_pair(0,9)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small10_PV",std::make_pair(0,10)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small11_PV",std::make_pair(0,11)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small12_PV",std::make_pair(0,12)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small13_PV",std::make_pair(0,13)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small14_PV",std::make_pair(0,14)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-small15_PV",std::make_pair(0,15)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large00_PV",std::make_pair(1,0))); 
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large01_PV",std::make_pair(1,1))); 
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large02_PV",std::make_pair(1,2))); 
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large03_PV",std::make_pair(1,3))); 
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large04_PV",std::make_pair(1,4))); 
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large05_PV",std::make_pair(1,5))); 
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large06_PV",std::make_pair(1,6))); 
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large07_PV",std::make_pair(1,7))); 
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large08_PV",std::make_pair(1,8))); 
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large09_PV",std::make_pair(1,9))); 
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large10_PV",std::make_pair(1,10)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large11_PV",std::make_pair(1,11)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large12_PV",std::make_pair(1,12)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large13_PV",std::make_pair(1,13)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large14_PV",std::make_pair(1,14)));
    fVolumeMap.insert(std::make_pair("Vol-gso-scintillator-large15_PV",std::make_pair(1,15)));
}
