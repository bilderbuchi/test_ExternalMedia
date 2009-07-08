within ExternalMedia.Media.ExternalTwoPhaseMixture;
function temperature_phx "Return temperature from p,h and x"
  extends Modelica.Icons.Function;
  input SI.AbsolutePressure p "Pressure";
  input Common.MolarEnthalpy h "Molar enthalpy";
  input SI.MoleFraction X[nComp] "molar fractions of the species";
  input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
  input Integer uniqueID = 0 "Unique ID";
  output SI.Temperature t "Temperature";
  annotation(derivative(noDerivative = X, noDerivative = phase, noDerivative = uniqueID) = molarDensity_ph_der,
             Inline = false);
algorithm
  t := temperature(
    setState_phx(
    p,
    h,
    phase,
    X,
    uniqueID),
    X);
end temperature_phx;
