#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(), 
 fStepLimit(NULL),
 fCheckOverlaps(true)
{

}

DetectorConstruction::~DetectorConstruction()
{
  delete fStepLimit;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* Air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

  G4Material* Tungsten = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");

  G4Material* Water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");

  G4Element* W = nist->FindOrBuildElement("W");
  G4Element* Cd = nist->FindOrBuildElement("Cd");
  G4Element* Te = nist->FindOrBuildElement("Te");
  G4Material* CdTe = new G4Material("CdTe", 5.85*g/cm3, 2);
  CdTe->AddElement(Cd, 1);
  CdTe->AddElement(Te, 1);

  // Definitions of Solids, Logical Volumes, Physical Volumes

  // World
  G4Box* worldS
    = new G4Box("world", 0.5*m, 0.5*m, 0.5*m);                                   
  
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,   //its solid
                 Air,      //its material
                 "World"); //its name
  
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps 

  // Mask
  G4Box* maskS 
    = new G4Box("mask", 5*cm, 5*cm, 0.5*mm);

  G4LogicalVolume* maskLV
    = new G4LogicalVolume(
                 maskS,   //its solid
                 Tungsten,      //its material
                 "Mask"); //its name

   G4Tubs* holeS 
    = new G4Tubs("hole", 0, 0.15*mm, 0.5*mm, 0, 360*deg);

  G4LogicalVolume* holeLV
    = new G4LogicalVolume(
                 holeS,   //its solid
                 Air,      //its material
                 "hole"); //its name

  // drill holes 
    int n = 31;
    int c = 0;
    for(auto i=0; i< n; i++)
    for(auto j=0; j<n; j++)
      {
        int a = 0;
        if(i==0) a = 0;
        if(j==0 && i !=0) a=1;

        int ci = -1;
        int cj = -1;
        int k = 1;
        for (k=1; k<31; k++)
          {
            int k2 = k*k;
            if(k2%31 == i) ci = 1;
            if(k2%31 == j) cj = 1;
          }

        if(ci*cj == 1) a = 1;

	if(a == 1)
	{
          G4VPhysicalVolume* holePV
                 = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector((i-n/2)*0.36*mm, (j-n/2)*0.36*mm, 0.), // at (0,0,0)
                 holeLV,         // its logical volume
                 "hole",         // its name
                 maskLV,               // its mother  volume
                 false,           // no boolean operations
                 c++,               // copy number
                 fCheckOverlaps); // checking overlaps 

	}
      }


  G4VPhysicalVolume* maskPV
    = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(0., 0., (216.-44.2)*mm), // at (0,0,0)
                 maskLV,         // its logical volume
                 "Mask",         // its name
                 worldLV,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps 

  // Detector
 
  G4Box* detS 
    = new G4Box("Det", 7.05*mm, 7.05*mm, 1.*mm);

  G4LogicalVolume* detLV = new G4LogicalVolume(detS, CdTe, "Det");

  G4VPhysicalVolume* detPV = new G4PVPlacement(
                 0, G4ThreeVector(0., 0., 216.*mm), detLV, "Det",
                 worldLV, false, 0, fCheckOverlaps);  

  return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors
}
