within ExternalMedia.Media.ExternalTwoPhaseMixture;
function thermalConductivity
  extends Modelica.Icons.Function;
  input ThermodynamicState state "state record";
  input SI.MoleFraction X[nComp] "molar fractions of the species";
  output SI.ThermalConductivity lambda "thermal Conductivity";
external "C" lambda = TwoPhaseMixture_thermalConductivity_(
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
end thermalConductivity;
