within ExternalMedia.Media.ExternalTwoPhaseMixture;
function specificEnthalpy
  extends Modelica.Icons.Function;
  input ThermodynamicState state "state record";
  input SI.MoleFraction X[nComp] "molar fractions of the species";
  output Common.MolarEnthalpy h "molar Enthalpy";
external "C" h = TwoPhaseMixture_specificEnthalpy_(
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
end specificEnthalpy;
