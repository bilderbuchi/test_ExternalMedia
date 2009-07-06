within ExternalMedia.Media;
package ExternalTwoPhaseMixture 
  import SI = Modelica.SIunits;
  import ExternalMedia.Common.InputChoice;


  replaceable record ThermodynamicState
  "Minimal variable set that is available as input argument to every medium function"
    extends Modelica.Icons.Record;

      Integer uniqueID "unique ID number";
      Common.MolarDensity d "molar density";
      Common.MolarEnthalpy h "molar enthalpy";
      SI.AbsolutePressure p "pressure";
      SI.MolarEntropy s "molar entropy";
      SI.Temperature T "temperature";
      FixedPhase phase(min=0, max=2);
  end ThermodynamicState;
  constant String mediumName = "CB" "Name of the medium";
  constant String libraryName = "FluidProp.RefProp"
  "Name of the external fluid property computation library";
  constant Real nComp = 2 "number of species";
  constant String substanceName = "pentane-hexane";


annotation (Documentation(info="<html>
All functions arguments and return values are in molar units.
</html>"));
end ExternalTwoPhaseMixture;
