within ExternalMedia.Media.ExternalTwoPhaseMixture;
function setState_psx "Return thermodynamic state record from p, s and x"
  extends Modelica.Icons.Function;
  input SI.AbsolutePressure p "pressure";
  input SI.SpecificEntropy s "specific Entropy";
  input FixedPhase phase = 0 "2 for two-phase, 1 for one-phase, 0 if not known";
  input SI.MoleFraction X[nComp] "molar fractions of the species";
  input Integer uniqueID = 0 "unique ID number";
  output ThermodynamicState state;
  output SI.MoleFraction x[nComp] "liquid phase composition";
  output SI.MoleFraction y[nComp] "vapor phase composition";
external "C" TwoPhaseMixture_setState_psx_(p, s, phase, nComp, X, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T,
   x, y, mediumName, libraryName, substanceName) 
   annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
end setState_psx;
