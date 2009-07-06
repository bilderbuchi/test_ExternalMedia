within ExternalMedia.Media.ExternalTwoPhaseMixture;
function bubbleTemperature "Return bubble Temperature"

  extends Modelica.Icons.Function;
  input SI.AbsolutePressure p "pressure";
  input SI.MoleFraction X[nComp] "molar fractions of the species";
  output SI.Temperature Tb;
 external "C" Tb = TwoPhaseMixture_bubbleTemperature_(p, nComp, X, mediumName, libraryName, substanceName) 
   annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
end bubbleTemperature;
