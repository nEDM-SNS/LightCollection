#include "nEDMMaterials.hh"

#include "G4SystemOfUnits.hh"

nEDMMaterials* nEDMMaterials::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEDMMaterials::nEDMMaterials()
{
    m_NistMan = G4NistManager::Instance();
    
    m_NistMan->SetVerbose(2);
    
    CreateMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEDMMaterials::~nEDMMaterials()
{
    delete    m_PMMA;
    delete    m_WLSPMMA;
    delete    m_Pethylene;
    delete    m_FPethylene;
    delete    m_Polystyrene;
    delete    m_TPB_outer;
    delete    m_TPB_inner;
    delete    m_Silicone;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEDMMaterials* nEDMMaterials::GetInstance()
{
    if (fInstance == 0)
    {
        fInstance = new nEDMMaterials();
    }
    return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* nEDMMaterials::GetMaterial(const G4String material)
{
    G4Material* mat =  m_NistMan->FindOrBuildMaterial(material);
    
    if (!mat) mat = G4Material::GetMaterial(material);
    if (!mat) {G4cout << material << "Not Found, Please Retry"<< G4endl;}
    
    return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEDMMaterials::CreateMaterials()
{
    G4double density, temperature;

    std::vector<G4String> elements;
    
    std::vector<G4int> natoms;
    std::vector<G4double> fractionMass;
    
    // Materials Definitions
    // =====================
    
    //--------------------------------------------------
    // Vacuum
    //--------------------------------------------------
    
    m_NistMan->FindOrBuildMaterial("G4_Galactic");
    
    //--------------------------------------------------
    // Air
    //--------------------------------------------------
    
    m_Air = m_NistMan->FindOrBuildMaterial("G4_AIR");
    
    //--------------------------------------------------
    // PMMA
    //--------------------------------------------------
    
    elements.push_back("C");     natoms.push_back(5);
    elements.push_back("H");     natoms.push_back(8);
    elements.push_back("O");     natoms.push_back(2);
    
    density = 1.190*g/cm3;
    
    m_PMMA = m_NistMan->
    ConstructNewMaterial("PMMA", elements, natoms, density);
    
    elements.clear();
    natoms.clear();
    
    //--------------------------------------------------
    // WLSfiber PMMA - PMMA w/ different MPT
    //--------------------------------------------------
    
    m_WLSPMMA = new   G4Material("WLSPMMA",density,m_PMMA);
    
    //--------------------------------------------------
    // Cladding (polyethylene)
    //--------------------------------------------------
    
    elements.push_back("C");     natoms.push_back(2);
    elements.push_back("H");     natoms.push_back(4);
    
    density = 1.200*g/cm3;
    
    m_Pethylene = m_NistMan->
    ConstructNewMaterial("Pethylene", elements, natoms, density);
    
    elements.clear();
    natoms.clear();
    
    //--------------------------------------------------
    // Double Cladding (fluorinated polyethylene)
    //--------------------------------------------------
    
    elements.push_back("C");     natoms.push_back(2);
    elements.push_back("H");     natoms.push_back(4);
    
    density = 1.400*g/cm3;
    
    m_FPethylene = m_NistMan->
    ConstructNewMaterial("FPethylene", elements, natoms, density);
    
    elements.clear();
    natoms.clear();
    
    //--------------------------------------------------
    // Polystyrene
    //--------------------------------------------------
    
    elements.push_back("C");     natoms.push_back(8);
    elements.push_back("H");     natoms.push_back(8);
    
    density = 1.050*g/cm3;
    
    m_Polystyrene = m_NistMan->
    ConstructNewMaterial("Polystyrene", elements, natoms, density);
    
    elements.clear();
    natoms.clear();
    
    //--------------------------------------------------
    // Tetraphenyl butadiene (TPB) (inner and outer)
    // Inner and outer have different Index of Refraction
    //--------------------------------------------------
    
    elements.push_back("C");     natoms.push_back(28);
    elements.push_back("H");     natoms.push_back(22);
    
    density = 1.079*g/cm3;
    
    m_TPB_outer = m_NistMan->
    ConstructNewMaterial("TPB_outer", elements, natoms, density);
    
    elements.clear();
    natoms.clear();

    m_TPB_inner = new G4Material("TPB_inner",density,m_TPB_outer);
    
    
    
    /*
    Just use TPB for now. Is the following ever needed?
    //--------------------------------------------------
    // TPB doped PS matrix - 2% TPB and 98% polystyrene by weight.
    //--------------------------------------------------
    
    // Assuming same density as PS?
    density = 1.050*g/cm3;
    
    fTPBDopedPS =
    new G4Material("TPBDopedPS", density, ncomponents=2);
    
    fTPBDopedPS->AddMaterial(TPB,         fractionmass = 2*perCent);
    fTPBDopedPS->AddMaterial(fPolystyrene, fractionmass = 98*perCent);
    */
    
    //--------------------------------------------------
    // Silicone (Template for Optical Grease)
    //--------------------------------------------------
    
    elements.push_back("C");     natoms.push_back(2);
    elements.push_back("H");     natoms.push_back(6);
    
    density = 1.060*g/cm3;
    
    m_Silicone = m_NistMan->
    ConstructNewMaterial("Silicone", elements, natoms, density);
    
    elements.clear();
    natoms.clear();
    
    //--------------------------------------------------
    // Aluminium
    //--------------------------------------------------
    
    m_NistMan->FindOrBuildMaterial("G4_Al");
    
    //
    // Liquid Helium
    //
    
    // Superfluid Helium: Density from Tito (07/13/10)
    elements.push_back("He");   natoms.push_back(1);

    density=0.145*g/cm3;
    
    m_SuperfluidHelium = m_NistMan->ConstructNewMaterial("SuperfluidHelium", elements, natoms, density, true, kStateLiquid, temperature=0.4*kelvin);
    
    elements.clear();
    natoms.clear();
    
    
    //
    // ------------ Generate & Add Material Properties Table ------------
    //84 elements in total 
    G4double photonEnergy[] =
	   {2.*eV, 2.03*eV, 2.06*eV, 2.091*eV, 2.121*eV,
	    2.15*eV, 2.181*eV, 2.21*eV, 2.24*eV, 2.27*eV, 
	    2.3*eV, 2.33*eV, 2.36*eV, 2.391*eV, 2.421*eV, 
	    2.45*eV, 2.481*eV, 2.51*eV, 2.541*eV, 2.571*eV, 
	    2.6*eV, 2.631*eV, 2.66*eV, 2.69*eV, 2.721*eV,
	    2.731*eV, 2.75*eV, 2.757*eV, 2.766*eV, 2.78*eV, 
	    2.796*eV, 2.811*eV, 2.84*eV, 2.87*eV, 2.884*eV, 
	    2.901*eV, 2.93*eV, 2.96*eV, 2.99*eV, 3.021*eV, 
	    3.05*eV, 3.081*eV, 3.11*eV, 3.14*eV, 3.171*eV,
	    3.2*eV, 3.23*eV, 3.261*eV, 3.29*eV, 3.321*eV, 
	    3.35*eV, 3.381*eV, 3.41*eV, 3.441*eV, 3.47*eV,
	    3.521*eV, 3.582*eV, 3.641*eV, 3.7*eV, 3.76*eV, 
	    3.82*eV, 3.881*eV, 3.94*eV, 4*eV, 4.06*eV, 
	    4.121*eV, 4.181*eV, 4.241*eV, 4.301*eV, 4.36*eV,
	    4.42*eV, 4.48*eV, 4.54*eV, 4.601*eV, 4.66*eV,
	    4.72*eV, 4.78*eV, 4.84*eV, 4.901*eV, 4.96*eV,
	    5.02*eV, 5.08*eV, 5.141*eV, 5.201*eV};

   

    const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);
    const G4int nEntriesAcr = sizeof(photonEnergy)/sizeof(G4double);
  /* Wavelength conversion (nm)
{620.0, 611.0, 602.0, 593.0, 585.0,
 577.0, 568.0, 561.0, 553.0, 546.0, 
 539.0, 532.0, 525.0, 519.0, 512.0, 
 506.0, 500.0, 494.0, 488.0, 482.0,
 477.0, 471.0, 466.0, 461.0, 456.0, 
 454.0, 451.0, 450.0, 448.0, 446.0,
 443.0, 441.0, 437.0, 432.0, 430.0,
 427.0, 423.0, 419.0, 415.0, 410.0,
 406.0, 402.0, 399.0, 395.0, 391.0,
 387.0, 384.0, 380.0, 377.0, 373.0,
 370.0, 367.0, 364.0, 360.0, 357.0,
 352.0, 346.0, 341.0, 335.0, 330.0,
 325.0, 319.0, 315.0, 310.0, 305.0,
 301.0, 297.0, 292.0, 288.0,284.0, 
 280.0, 277.0, 273.0, 269.0,266.0, 
 263.0, 259.0, 256.0, 253.0,250.0, 
 247.0, 244.0, 241.0, 238.0} */
    
    
    //--------------------------------------------------
    // Air
    //--------------------------------------------------
    
    G4double refractiveIndex[] =
     {1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
    
    assert(sizeof(refractiveIndex) == sizeof(photonEnergy));
    
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
    
    m_Air->SetMaterialPropertiesTable(mpt);
    
    //--------------------------------------------------
    // SuperFluid Helium
    //--------------------------------------------------

    // Set the SHe optical properties the same as air
    
    m_SuperfluidHelium->SetMaterialPropertiesTable(mpt);
    
    //--------------------------------------------------
    //  Regular PMMA properties - used everywhere but fiber
    //--------------------------------------------------
    
      G4double refractiveIndexPMMA[] =
    {1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
     1.60, 1.60, 1.60, 1.60, 1.60, 1.60};
    
    assert(sizeof(refractiveIndexPMMA) == sizeof(photonEnergy));

    G4double absPMMA[] =
     {10.*m, 10.*m, 10.*m, 10.*m, 10.*m, 
      10.*m,10.*m, 10.*m, 10.*m, 10.*m,
      10.*m, 10.*m,10.*m, 10.*m, 10.*m, 
      10.*m, 10.*m, 10.*m, 5.*m, 5.*m, 
      5.*m, 5.*m, 5.*m, 5.*m, 5.*m, 
      5.*m, 5.*m, 5.*m, 5.*m, 5.*m, 
      5.*m, 5.*m, 5.*m, 1.*m, 1.*m,
      1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 
      1.*m, 1.*m, 1.*m, 50.*cm, 50.*cm,
      50.*cm, 50.*cm, 50.*cm, 50.*cm, 20.*cm, 
      20.*cm, 20.*cm, 20.*cm, 20.*cm, 20.*cm,
      20.*cm, 20.*cm, 20.*cm, 20.*cm, 20.*cm,
      20.*cm, 20.*cm,20.*cm,  20.*cm,10.*cm,
      7.5*cm, 4.*cm, 1.*cm, 6.*mm,3.*mm, 
      2.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*nm,
      1.*nm, 1.*nm, 1.*nm, 1.*nm,1.*nm, 
      1.*nm, 1.*nm, 1.*nm, 1.*nm };
	
	/* Wavelength (nm)

{620.0, 611.0, 602.0, 593.0, 585.0,
 577.0, 568.0, 561.0, 553.0, 546.0, 
 539.0, 532.0, 525.0, 519.0, 512.0, 
 506.0, 500.0, 494.0, 488.0, 482.0,
 477.0, 471.0, 466.0, 461.0, 456.0, 
 454.0, 451.0, 450.0, 448.0, 446.0,
 443.0, 441.0, 437.0, 432.0, 430.0,
 427.0, 423.0, 419.0, 415.0, 410.0,
 406.0, 402.0, 399.0, 395.0, 391.0,
 387.0, 384.0, 380.0, 377.0, 373.0,
 370.0, 367.0, 364.0, 360.0, 357.0,
 352.0, 346.0, 341.0, 335.0, 330.0,
 325.0, 319.0, 315.0, 310.0, 305.0,
 301.0, 297.0, 292.0, 288.0,284.0, 
 280.0, 277.0, 273.0, 269.0,266.0, 
 263.0, 259.0, 256.0, 253.0,250.0, 
 247.0, 244.0, 241.0, 238.0} 
	
	*/


	
	
    assert(sizeof(absPMMA) == sizeof(photonEnergy));

    // Add entries into properties table
    G4MaterialPropertiesTable* mptPMMA = new G4MaterialPropertiesTable();
    mptPMMA->
    AddProperty("RINDEX",acrylicPhotonEnergy,refractiveIndexPMMA,nEntriesAcr);
    mptPMMA->AddProperty("ABSLENGTH",acrylicPhotonEnergy,absPMMA,nEntriesAcr);

    m_PMMA->SetMaterialPropertiesTable(mptPMMA);

    //--------------------------------------------------
    //  PMMA for WLSfibers properties
    //--------------------------------------------------
    
    G4double refractiveIndexWLSfiber[] =
      {   1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
        1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
        1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
        1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
        1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
        1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
        1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
        1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
        1.60, 1.60, 1.60, 1.60};
    assert(sizeof(refractiveIndexWLSfiber) == sizeof(photonEnergy));

    G4double absfiber[] =
    {   7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
        7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
	7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
	7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
	7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
    	7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
    	7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
    	7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
        7.5*m, 7.5*m,7.5*m, 7.5*m };

    assert(sizeof(absfiber) == sizeof(photonEnergy));
  

    G4double absWLSfiber[] =
        { 1.0*km,1.0*km,1.0*km,1.0*km,1.0*km,
	 1.0*km,1.0*km,1.0*km,1.0*km,1.0*km,
	 1.0*km,1.0*km,100.0*m,100.0*m,100.0*m,
	 20.0*m,20.0*m,20.0*m,20.0*m,55.0757*mm,
	 2.04762*mm,4.03093*mm,1.63563*mm,0.807274*mm,0.584756*mm,
	 0.561603*mm,0.606365*mm,0.602746*mm,0.701749*mm,0.745194*mm,
	 0.765499*mm,0.711484*mm,0.491352*mm,0.34043*mm,0.354659*mm,
	 0.443855*mm,0.657921*mm,0.886918*mm,1.08736*mm,1.17534*mm,
	 1.34467*mm,1.55013*mm,1.84965*mm,2.31813*mm,2.92241*mm,
	 3.77047*mm,4.84304*mm,6.52701*mm,8.51557*mm,10.3782*mm,
	 13.9593*mm,15.7403*mm,18.6949*mm,20.2011*mm,22.9623*mm,
	 28.0367*mm,34.6965*mm,41.7367*mm,41.7367*mm,41.7367*mm,
	 41.7367*mm,41.7367*mm,41.7367*mm,105.096*mm,105.096*mm,
	 20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
	 20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
	 20.0*m,20.0*m,20.0*m,1.0*km,1.0*km,
	 1.0*km,1.0*km,1.0*km,1.0*km
	};
/* Wavelength (nm)

{620.0, 611.0, 602.0, 593.0, 585.0,
 577.0, 568.0, 561.0, 553.0, 546.0, 
 539.0, 532.0, 525.0, 519.0, 512.0, 
 506.0, 500.0, 494.0, 488.0, 482.0,
 477.0, 471.0, 466.0, 461.0, 456.0, 
 454.0, 451.0, 450.0, 448.0, 446.0,
 443.0, 441.0, 437.0, 432.0, 430.0,
 427.0, 423.0, 419.0, 415.0, 410.0,
 406.0, 402.0, 399.0, 395.0, 391.0,
 387.0, 384.0, 380.0, 377.0, 373.0,
 370.0, 367.0, 364.0, 360.0, 357.0,
 352.0, 346.0, 341.0, 335.0, 330.0,
 325.0, 319.0, 315.0, 310.0, 305.0,
 301.0, 297.0, 292.0, 288.0,284.0, 
 280.0, 277.0, 273.0, 269.0,266.0, 
 263.0, 259.0, 256.0, 253.0,250.0, 
 247.0, 244.0, 241.0, 238.0} 
	
	*/

    assert(sizeof(absWLSfiber) == sizeof(photonEnergy));
    
    G4double emissionFib[] =
    {0.05, 0.06, 0.13, 0.19, 0.28, 0.37, 0.53, 0.64, 1.00, 1.40,     // 10 elements per line
        1.90, 2.20, 2.30, 2.53, 3.10, 4.30, 5.00, 4.60, 3.00, 1.00,
        0.14, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
        0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
        0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
        0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
        0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
        0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
        0.00, 0.00, 0.00, 0.00};
    
    assert(sizeof(emissionFib) == sizeof(photonEnergy));
    
    
    // Add entries into properties table
    G4MaterialPropertiesTable* mptWLSfiber = new G4MaterialPropertiesTable();
    mptWLSfiber->
    AddProperty("RINDEX",photonEnergy,refractiveIndexWLSfiber,nEntries);
    mptWLSfiber->AddProperty("ABSLENGTH",photonEnergy,absfiber,nEntries);
    mptWLSfiber->AddProperty("WLSABSLENGTH",photonEnergy,absWLSfiber,nEntries);
    mptWLSfiber->AddProperty("WLSCOMPONENT",photonEnergy,emissionFib,nEntries);
    mptWLSfiber->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);
    

    m_WLSPMMA->SetMaterialPropertiesTable(mptWLSfiber);
    
    //--------------------------------------------------
    //  Polyethylene
    //--------------------------------------------------
    
    G4double refractiveIndexClad1[] =
    { 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
        1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
        1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
        1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
        1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
        1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
        1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
        1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
        1.49, 1.49, 1.49, 1.49};
    
    assert(sizeof(refractiveIndexClad1) == sizeof(photonEnergy));
    
    G4double absClad[] =
     {7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
        7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
	7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
	7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
	7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
        7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
	7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
	7.5*m, 7.5*m,7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m, 7.5*m,
        7.5*m, 7.5*m, 7.5*m, 7.5*m};
    
    assert(sizeof(absClad) == sizeof(photonEnergy));
    
    // Add entries into properties table
    G4MaterialPropertiesTable* mptClad1 = new G4MaterialPropertiesTable();
    mptClad1->AddProperty("RINDEX",photonEnergy,refractiveIndexClad1,nEntries);
    mptClad1->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);
    
    m_Pethylene->SetMaterialPropertiesTable(mptClad1);
    
    //--------------------------------------------------
    // Fluorinated Polyethylene
    //--------------------------------------------------
    
    G4double refractiveIndexClad2[] =
     { 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
        1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
        1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
        1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
        1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
        1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
        1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
        1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
        1.42, 1.42, 1.42, 1.42};
    
    assert(sizeof(refractiveIndexClad2) == sizeof(photonEnergy));
    
    // Add entries into properties table
    G4MaterialPropertiesTable* mptClad2 = new G4MaterialPropertiesTable();
    mptClad2->AddProperty("RINDEX",photonEnergy,refractiveIndexClad2,nEntries);
    mptClad2->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);
    
    m_FPethylene->SetMaterialPropertiesTable(mptClad2);
    
    //--------------------------------------------------
    // Silicone
    //--------------------------------------------------
    
    G4double refractiveIndexSilicone[] =
    { 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
        1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
        1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
        1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
        1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
        1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
        1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
        1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
        1.46, 1.46, 1.46, 1.46};
    
    assert(sizeof(refractiveIndexSilicone) == sizeof(photonEnergy));
    
    // Add entries into properties table
    G4MaterialPropertiesTable* mptSilicone = new G4MaterialPropertiesTable();
    mptSilicone->
    AddProperty("RINDEX",photonEnergy,refractiveIndexSilicone,nEntries);
    mptSilicone->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);
    
    m_Silicone->SetMaterialPropertiesTable(mptSilicone);
    
    //--------------------------------------------------
    //  Polystyrene
    //--------------------------------------------------
    
    G4double refractiveIndexPS[] =
    { 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
        1.50, 1.50, 1.50, 1.50};
    
    assert(sizeof(refractiveIndexPS) == sizeof(photonEnergy));
    
    G4double absPS[] =
     {2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
        2.*cm,2.*cm,2.*cm,2.*cm};
    
    assert(sizeof(absPS) == sizeof(photonEnergy));
    
    G4double scintilFast[] =
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
       0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0};
    
    assert(sizeof(scintilFast) == sizeof(photonEnergy));
    
    // Add entries into properties table
    G4MaterialPropertiesTable* mptPolystyrene = new G4MaterialPropertiesTable();
    mptPolystyrene->AddProperty("RINDEX",photonEnergy,refractiveIndexPS,nEntries);
    mptPolystyrene->AddProperty("ABSLENGTH",photonEnergy,absPS,nEntries);
    mptPolystyrene->
    AddProperty("FASTCOMPONENT",photonEnergy, scintilFast,nEntries);
    mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD",10./keV);
    mptPolystyrene->AddConstProperty("RESOLUTIONSCALE",1.0);
    mptPolystyrene->AddConstProperty("FASTTIMECONSTANT", 10.*ns);
    
    m_Polystyrene->SetMaterialPropertiesTable(mptPolystyrene);
    
    // Set the Birks Constant for the Polystyrene scintillator
    
    m_Polystyrene->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
    
    //--------------------------------------------------
    // Tetraphenyl butadiene (TPB) (inner and outer)
    // Inner and outer have different Index of Refraction
    //--------------------------------------------------

    
    G4double absTPB[] =
    {1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km,
        1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 0.3*mm, 0.12*mm,
        0.25*mm, 10.*m,10.*m,10.*m,10.*m,10.*m,10.*m,10.*m,10.*m,10.*m,10.*m, 
	 0.004*mm,  0.004*mm, 0.004*mm,0.004*mm,  0.004*mm, 0.004*mm, 0.004*mm, 0.004*mm, 0.004*mm, 0.004*mm, 
	  0.004*mm,  0.004*mm, 0.004*mm,0.004*mm,  0.004*mm, 0.004*mm, 0.004*mm, 0.004*mm, 0.004*mm, 0.004*mm, 
	  0.004*mm,  0.004*mm, 0.004*mm,0.004*mm, 0.004*mm, 0.004*mm, 0.004*mm,0.004*mm, 0.004*mm, 0.004*mm, 0.004*mm, 
	 10.*m, 10.*m, 10.*m,10.*m, 10.*m, 10.*m, 10.*m, 10.*m, 10.*m,10.*m, 
	 10.*m, 10.*m, 10.*m, 10.*m, 0.018*mm, 0.018*mm, 0.018*mm, 10.*m, 10.*m, 10.*m, 
	 10.*m, 10.*m,10.*m, 10.*m}
    /* Wavelength (nm)

{620.0, 611.0, 602.0, 593.0, 585.0,
 577.0, 568.0, 561.0, 553.0, 546.0, 
 539.0, 532.0, 525.0, 519.0, 512.0, 
 506.0, 500.0, 494.0, 488.0, 482.0,
 477.0, 471.0, 466.0, 461.0, 456.0, 
 454.0, 451.0, 450.0, 448.0, 446.0,
 443.0, 441.0, 437.0, 432.0, 430.0,
 427.0, 423.0, 419.0, 415.0, 410.0,
 406.0, 402.0, 399.0, 395.0, 391.0,
 387.0, 384.0, 380.0, 377.0, 373.0,
 370.0, 367.0, 364.0, 360.0, 357.0,
 352.0, 346.0, 341.0, 335.0, 330.0,
 325.0, 319.0, 315.0, 310.0, 305.0,
 301.0, 297.0, 292.0, 288.0,284.0, 
 280.0, 277.0, 273.0, 269.0,266.0, 
 263.0, 259.0, 256.0, 253.0,250.0, 
 247.0, 244.0, 241.0, 238.0} 
	
	*/
    assert(sizeof(absTPB) == sizeof(PhotonEnergy));
    
    G4double absWLSTPB[] =
     {  1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km,1.*km, 1.*km,
        1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km,1.*km,1.*km,
        1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km,1.*km, 1.*km,
        1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km, 1.*km,1.*km,1.*km,
         1.*km,1.*km,1.*km, 0.045*mm, 0.039*mm, 0.025*mm,0.019*mm, 0.013*mm, 0.01*mm,0.01*mm,
         0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,
         10.*m, 10.*m, 10.*m, 10.*m, 0.027*mm, 0.027*mm, 0.027*mm, 10.*m, 10.*m,10.*m,
         10.*m, 10.*m, 10.*m, .049*mm, .049*mm, .049*mm, .049*mm, .049*mm, .049*mm,.049*mm, 
         .049*mm, .049*mm, 10.*nm, 10.*nm
        
        };
    
    assert(sizeof(absWLSTPB) == sizeof(photonEnergy));
    
    G4double emissionTPB[] =
    {0.05, 0.06, 0.08, 0.13, 0.15
     0.19, 0.23,0.28, 0.34 ,0.37,
     0.44, 0.53, 0.59 ,0.64, 0.8
     1.00, 1.18, 1.40, 1.58 ,1.90, 
     2.05, 2.20, 2.25,2.30, 2.37,
     2.45, 2.53,3.67,3.81,3.99
     3.10, 3.17, 3.25, 4.30,4.55,
     4.8,  5.00, 4.8, 4.6, 3.7
     3, 2.2, 1, 0.35 ,0.14 ,
     0.07,0.05, 0.0, 0.0, 0.0, 
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0};

    
    assert(sizeof(emissionTPB) == sizeof(photonEnergy));


    G4MaterialPropertiesTable* mptTPB_inner = new G4MaterialPropertiesTable();
    mptTPB_inner->AddProperty("RINDEX", photonEnergy, refractiveIndexPMMA, nEntries);
    mptTPB_inner->AddProperty("ABSLENGTH", acrylicPhotonEnergy, absTPB, nEntriesAcr);
    mptTPB_inner->AddProperty("WLSABSLENGTH", acrylicPhotonEnergy, absWLSTPB, nEntriesAcr);
    mptTPB_inner->AddProperty("WLSCOMPONENT", acrylicPhotonEnergy, emissionTPB, nEntriesAcr);
    mptTPB_inner->AddConstProperty("WLSMEANNUMBERPHOTONS", 0.35);

    mptTPB_inner->AddConstProperty("WLSTIMECONSTANT", 0.01*ns);
    m_TPB_inner->SetMaterialPropertiesTable(mptTPB_inner);
    
    
    
    G4MaterialPropertiesTable* mptTPB_outer = new G4MaterialPropertiesTable();
    mptTPB_outer->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
    mptTPB_outer->AddProperty("ABSLENGTH", acrylicPhotonEnergy, absTPB, nEntriesAcr);
    mptTPB_outer->AddProperty("WLSABSLENGTH", acrylicPhotonEnergy, absWLSTPB, nEntriesAcr);
    mptTPB_outer->AddProperty("WLSCOMPONENT", acrylicPhotonEnergy, emissionTPB, nEntriesAcr);
    mptTPB_outer->AddConstProperty("WLSMEANNUMBERPHOTONS", 0.35);
    mptTPB_outer->AddConstProperty("WLSTIMECONSTANT", 0.01*ns);
    m_TPB_outer->SetMaterialPropertiesTable(mptTPB_outer);

/* Not used anymore
    //--------------------------------------------------
    // TPB doped PS matrix - 2% TPB and 98% polystyrene by weight.
    //--------------------------------------------------
    
    // Add entries into properties table (same as PS)
    G4MaterialPropertiesTable* mptTPBDopedPS = new G4MaterialPropertiesTable();
    mptTPBDopedPS->AddProperty("RINDEX", photonEnergy, refractiveIndexPS, nEntries);
//    mptTPBDopedPS->AddProperty("RINDEX", photonEnergy, refractiveIndexPMMA, nEntries);
    mptTPBDopedPS->AddProperty("ABSLENGTH",photonEnergy,absPS,nEntries);
//    mptTPBDopedPS->AddProperty("ABSLENGTH",photonEnergy,absPMMA,nEntries);
    
    // This property triggers the TPB process
    mptTPBDopedPS->AddConstProperty("TPBCONVERSIONEFFICIENCY", 0.33);
    
    fTPBDopedPS->SetMaterialPropertiesTable(mptTPBDopedPS);

    // Define TPB optical properties
    const G4int nTPBEntries = 25;
    const G4int nSiO2Entries = 20;
  
    G4double SiO2Energy[nSiO2Entries] =
    {0.602*eV, 0.689*eV, 1.03*eV,  1.926*eV,
        2.583*eV, 2.845*eV, 2.857*eV, 3.124*eV,
        3.457*eV, 3.643*eV, 3.812*eV, 4.086*eV,
        4.511*eV, 4.953*eV, 5.474*eV, 6.262*eV,
        7.000*eV, 8.300*eV, 10.00*eV, 12.60*eV };
    
    G4double SiO2RefractiveIndex[nSiO2Entries] =
    { 1.520,    1.5241,   1.5323,   1.5423,
        1.550118, 1.553790, 1.55396, 1.55813,
        1.563915, 1.56747, 1.570915, 1.57955,
        1.58752, 1.60032, 1.61818, 1.65087,
        1.698, 1.877, 2.581, 1.390 };

    G4double TPBEnergy[nTPBEntries] =
    {0.602*eV, 0.689*eV, 1.03*eV,  1.926*eV, 2.138*eV, 2.25*eV, 2.38*eV, 2.48*eV,
        2.583*eV, 2.845*eV, 2.857*eV, 2.95*eV, 3.124*eV,
        3.457*eV, 3.643*eV, 3.812*eV, 4.086*eV,
        4.511*eV, 4.953*eV, 5.474*eV, 6.262*eV,
        7.000*eV, 8.300*eV, 10.00*eV, 12.60*eV };
    
    G4double TPBAbsorption[nSiO2Entries] =
    { 100000*m, 100000*m, 100000*m, 100000*m,
        100000*m, 100000*m, 100000*m, 100000*m,
        100000*m, 100000*m, 100000*m, 100000*m,
        100000*m, 100000*m, 100000*m, 0.001*nm,
        0.001*nm, 0.001*nm, 0.001*nm, 0.001*nm };
    
    G4double TPBEmission[nTPBEntries] =
    { 0, 0, 0, 0, 0.0005, 0.0015, 0.003, 0.005,
        0.007, 0.011, 0.011, 0.006, 0.002,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0 };
    
    G4MaterialPropertiesTable* fTPBMat = new G4MaterialPropertiesTable();
    fTPBMat->AddProperty("RINDEX", SiO2Energy, SiO2RefractiveIndex, nSiO2Entries);
    fTPBMat->AddProperty("WLSABSLENGTH", SiO2Energy, TPBAbsorption, nSiO2Entries);
    fTPBMat->AddProperty("WLSCOMPONENT", TPBEnergy, TPBEmission, nTPBEntries);
    fTPBMat->AddConstProperty("WLSTIMECONSTANT", 0.01*ns);
    TPB->SetMaterialPropertiesTable( fTPBMat );
*/

}
