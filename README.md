# mpxsim4

mpxsim4 - Geant4 application to simulate a radioactive source, a CA mask and a pixel detector. 
Output: ROOT tree containing the MC truth at sensor surface (x, y, kine [MeV], PDG) and total Edep in the sensor for each incoming particle.
TODO: the actual mask geometry

scripts/clsim.C - script to simulate Timepix clusters using the ROOT tree from previos step
Output: CLOG file with clusters
