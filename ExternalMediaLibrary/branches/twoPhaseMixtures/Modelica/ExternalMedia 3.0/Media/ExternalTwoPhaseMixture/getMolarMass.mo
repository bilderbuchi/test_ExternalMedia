within ExternalMedia.Media.ExternalTwoPhaseMixture;
function getMolarMass
  extends Modelica.Icons.Function;
  input SI.MoleFraction X[nComp] "molar fractions of the species";
  output SI.MolarMass MM "Molar Mass";
external "C" MM = TwoPhaseMixture_getMolarMass_(
    nComp,
    X,
    mediumName,
    libraryName,
    substanceName) 
  annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
end getMolarMass;
