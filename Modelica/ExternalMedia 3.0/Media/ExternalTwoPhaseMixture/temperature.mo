within ExternalMedia.Media.ExternalTwoPhaseMixture;
function temperature

    extends Modelica.Icons.Function;
    input ThermodynamicState state "state record";
    input Real X[nComp] "molar fractions of the species";
   output SI.Temperature t "Temperature";
external "C" t = TwoPhaseMixture_temperature_(
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
end temperature;
