within ExternalMedia.Media.ExternalTwoPhaseMixture;
function specificInternalEnergy
  extends Modelica.Icons.Function;
  input ThermodynamicState state "state record";
  input SI.MoleFraction X[nComp] "molar fractions of the species";
  output SI.MolarInternalEnergy h "molar Enthalpy";
external "C" h = TwoPhaseMixture_specificInternalEnergy_(
    state.uniqueID,
    3,
    state.d,
    state.h,
    state.p,
    state.s,
    state.T,
    state.phase,
    nComp,
    X,
    mediumName,
    libraryName,
    substanceName) 
  annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
end specificInternalEnergy;
