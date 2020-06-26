//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SystemOfUnits.hh"
#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"

#include "TimepixDetector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  

   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 20 * cm;
  G4double world_sizeZ  = 3 * m;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  //     
  // Radiator
  //
  
  G4double radiator_sizeXY = 5 * cm;
  G4double radiator_sizeZ  = 50 * micrometer;
  G4double radiator_gapZ = 2.97 * mm;
  G4double radiator_posXY = 0 * mm;
  G4double radiator_posZ = -115 * cm;
  G4int radiator_count = 30;
  G4Material* radiator_mat = nist->FindOrBuildMaterial("G4_MYLAR");
  G4Material* radiator_env_mat = nist->FindOrBuildMaterial("G4_AIR");
  /*
  G4Box* solidRadiatorEnv =    
    new G4Box("Radiator_Envelope",                       //its name
       0.5*radiator_sizeXY, 0.5*radiator_sizeXY, 0.5*(radiator_sizeZ+radiator_gapZ)*radiator_count);
  
  G4LogicalVolume* logicRadiatorEnv =                         
    new G4LogicalVolume(solidRadiatorEnv,          //its solid
                        radiator_mat,           //its material
                        "Radiator_Envelope");            //its name
  
  G4VPhysicalVolume* physRadiatorEnv = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(radiator_posXY, radiator_posXY, radiator_posZ),       //at (0,0,0)
                      logicRadiatorEnv,            //its logical volume
                      "Radiator_Envelope",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);   
    
  
  G4Box* solidRadiator =    
    new G4Box("Radiator",                       //its name
       0.5*radiator_sizeXY, 0.5*radiator_sizeXY, 0.5*radiator_sizeZ);     //its size
      
  G4LogicalVolume* logicRadiator =                         
    new G4LogicalVolume(solidRadiator,          //its solid
                        radiator_mat,           //its material
                        "Radiator");            //its name
                                   
  G4VPhysicalVolume* physRadiator = 
    new G4PVReplica("Radiator",                   
                      logicRadiator,
                      logicRadiatorEnv,        
                      kZAxis,               
                      radiator_count,                     
                      radiator_gapZ);
                      
  */
  
  G4Box* solidRadiator =    
    new G4Box("Radiator",                       //its name
       0.5*radiator_sizeXY, 0.5*radiator_sizeXY, 0.5*radiator_sizeZ);     //its size
      
  G4LogicalVolume* logicRadiator =                         
    new G4LogicalVolume(solidRadiator,          //its solid
                        radiator_mat,           //its material
                        "Radiator");            //its name
  
  G4PVPlacement* physRadiator[90];  
  for (int i = 0; i < radiator_count; ++i) {
    physRadiator[i] = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(radiator_posXY, radiator_posXY, radiator_posZ + i * (radiator_gapZ + radiator_sizeZ)),       //at (0,0,0)
                      logicRadiator,            //its logical volume
                      "Radiator",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      i,                     //copy number
                      checkOverlaps);   
  }                   

  
  //
  //Drift tube
  //
  G4double tube_radius = 3 * cm;
  G4double tube_sizeZ  = 208 * cm;
  G4double tube_thickness = 16 * micrometer;
  G4double tube_posXY = 0 * mm;
  G4double tube_posZ = 0 * cm;
  G4Material* tube_wall_mat = nist->FindOrBuildMaterial("G4_MYLAR");
  G4Material* tube_gas_mat = nist->FindOrBuildMaterial("G4_He");  

  G4Tubs* solidTubeWall =    
    new G4Tubs("Tube_Wall",
               0,
               tube_radius + tube_thickness,
               tube_thickness + 0.5 * tube_sizeZ,
               0,
               2 * M_PI * rad);
  
  G4LogicalVolume* logicTubeWall =                         
    new G4LogicalVolume(solidTubeWall,          //its solid
                        tube_wall_mat,           //its material
                        "Tube_Wall");            //its name
  
  G4VPhysicalVolume* physTubeWall = 
    new G4PVPlacement(0,                   
                      G4ThreeVector(tube_posXY, tube_posXY, tube_posZ),     
                      logicTubeWall,            
                      "Tube_Wall",               
                      logicWorld,                
                      false,           
                      0,                    
                      checkOverlaps);  

  G4Tubs* solidTubeGas =    
    new G4Tubs("Tube_Gas",
               0,
               tube_radius,
               0.5 * tube_sizeZ,
               0,
               2 * M_PI * rad);
  
  G4LogicalVolume* logicTubeGas =                         
    new G4LogicalVolume(solidTubeGas,          //its solid
                        tube_gas_mat,           //its material
                        "Tube_Gas");            //its name
  
  G4VPhysicalVolume* physTubeGas = 
    new G4PVPlacement(0,                   
                      G4ThreeVector(),     
                      logicTubeGas,            
                      "Tube_Wall",               
                      logicTubeWall,                
                      false,           
                      0,                    
                      checkOverlaps);     
  // 
  //Timepix0
  //
   
  //sensor  
  G4double tpx0_sensor_sizeXY = 14.08 * mm;
  G4double tpx0_sensor_sizeZ  = 500 * micrometer;
  G4double tpx0_sensor_posXY = 0 * mm;
  G4double tpx0_sensor_posZ = 1050 * mm;
  G4Material* tpx_sensor_mat = nist->FindOrBuildMaterial("G4_Si");
  
  G4Box* solidTpx0Sensor =    
    new G4Box("Tpx0_Sensor",                       //its name
       0.5*tpx0_sensor_sizeXY, 0.5*tpx0_sensor_sizeXY, 0.5*tpx0_sensor_sizeZ);     //its size
      
  G4LogicalVolume* logicTpx0Sensor =                         
    new G4LogicalVolume(solidTpx0Sensor,          //its solid
                        tpx_sensor_mat,           //its material
                        "Tpx0_Sensor");            //its name
                                   
  G4VPhysicalVolume* physTpx0Sensor = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(tpx0_sensor_posXY, tpx0_sensor_posXY, tpx0_sensor_posZ),       //at (0,0,0)
                      logicTpx0Sensor,            //its logical volume
                      "Tpx0_Sensor",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking  
    
  //chip
  G4double tpx0_chip_sizeXY = 14 * mm;
  G4double tpx0_chip_sizeZ  = 700 * micrometer;
  G4double tpx0_chip_posXY = 0 * mm;
  G4double tpx0_chip_posZ = 1050.3 * mm;
  G4Material* tpx_chip_mat = nist->FindOrBuildMaterial("G4_Si");  
  
  G4Box* solidTpx0Chip =    
    new G4Box("Tpx0_Chip",                       //its name
       0.5*tpx0_chip_sizeXY, 0.5*tpx0_chip_sizeXY, 0.5*tpx0_chip_sizeZ);     //its size
      
  G4LogicalVolume* logicTpx0Chip =                         
    new G4LogicalVolume(solidTpx0Chip,          //its solid
                        tpx_chip_mat,           //its material
                        "Tpx0_Chip");            //its name
                                   
  G4VPhysicalVolume* physTpx0Chip = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(tpx0_chip_posXY, tpx0_chip_posXY, tpx0_chip_posZ),       //at (0,0,0)
                      logicTpx0Chip,            //its logical volume
                      "Tpx0_Chip",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking    
  //pcb
  G4double tpx0_pcb_sizeXY = 30 * mm;
  G4double tpx0_pcb_sizeZ  = 1600 * micrometer;
  G4double tpx0_pcb_posXY = 0 * mm;
  G4double tpx0_pcb_posZ = 1051 * mm;
  G4Material* tpx_pcb_mat = nist->FindOrBuildMaterial("G4_BAKELITE");  
  
  G4Box* solidTpx0Pcb =    
    new G4Box("Tpx0_Pcb",                       //its name
       0.5*tpx0_pcb_sizeXY, 0.5*tpx0_pcb_sizeXY, 0.5*tpx0_pcb_sizeZ);     //its size
      
  G4LogicalVolume* logicTpx0Pcb =                         
    new G4LogicalVolume(solidTpx0Pcb,          //its solid
                        tpx_pcb_mat,           //its material
                        "Tpx0_Pcb");            //its name
                                   
  G4VPhysicalVolume* physTpx0Pcb = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(tpx0_pcb_posXY, tpx0_pcb_posXY, tpx0_pcb_posZ),       //at (0,0,0)
                      logicTpx0Pcb,            //its logical volume
                      "Tpx0_Pcb",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking   

    
  //G4VSensitiveDetector setup

    TimepixDetector* timepix = new TimepixDetector("/Timepix0");                    
    G4SDManager* SDM = G4SDManager::GetSDMpointer(); 
    SDM -> AddNewDetector(timepix);
    logicTpx0Sensor -> SetSensitiveDetector(timepix);
   
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
