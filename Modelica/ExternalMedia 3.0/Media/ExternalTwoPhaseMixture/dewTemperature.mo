within ExternalMedia.Media.ExternalTwoPhaseMixture;
function dewTemperature "Return dew Temperature"

  extends Modelica.Icons.Function;
  input SI.AbsolutePressure p "pressure";
  input SI.MoleFraction X[nComp] "molar fractions of the species";
  output SI.Temperature Td;

 external "C" Td = TwoPhaseMixture_dewTemperature_(p, nComp, X, mediumName, libraryName, substanceName) 
   annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
end dewTemperature;
