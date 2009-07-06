within ExternalMedia.Media.ExternalTwoPhaseMixture;
function molarDensity_ph_der
  extends Modelica.Icons.Function;
  input SI.AbsolutePressure p "Pressure";
  input Common.MolarEnthalpy h "Molar enthalpy";
  input SI.MoleFraction X[nComp] "molar fractions of the species";
  input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
  input Integer uniqueID = 0 "Unique ID";
  input Real p_der;
  input Real h_der;
  output Real d_der;
external "C" d_der = TwoPhaseMixture_density_ph_der_(
    uniqueID,
    p_der,
    h_der,
    p,
    h,
    phase,
    nComp,
    X,
    mediumName,
    libraryName,
    substanceName) 
  annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
end molarDensity_ph_der;
