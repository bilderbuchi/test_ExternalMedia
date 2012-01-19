within ExternalMedia.Media;
package ExternalTwoPhaseMedium
  "External two phase medium package - modify libraryName to set the external library"

  extends Modelica.Media.Interfaces.PartialTwoPhaseMedium(
    mediumName = "ExternalMedium",
    singleState = false,
    onePhase = false,
    smoothModel = false,
    fluidConstants = {externalFluidConstants});
  import ExternalMedia.Common.InputChoice;

  constant String libraryName = "UnusableExternalMedium"
    "Name of the external fluid property computation library";
  final constant String substanceName = substanceNames[1]
    "Only one substance can be specified";

  constant FluidConstants externalFluidConstants = FluidConstants(
    iupacName=  "unknown",
    casRegistryNumber=  "unknown",
    chemicalFormula=  "unknown",
    structureFormula=  "unknown",
    molarMass=  getMolarMass(),
    criticalTemperature=  getCriticalTemperature(),
    criticalPressure=  getCriticalPressure(),
    criticalMolarVolume=  getCriticalMolarVolume(),
    acentricFactor=  0,
    triplePointTemperature=  280.0,
    triplePointPressure=  500.0,
    meltingPoint=  280,
    normalBoilingPoint=  380.0,
    dipoleMoment=  2.0);

  constant InputChoice inputChoice=InputChoice.ph
    "Default choice of input variables for property computations";

  redeclare replaceable record extends ThermodynamicState
    Density d "density";
    SpecificEnthalpy h "specific enthalpy";
    AbsolutePressure p "pressure";
    SpecificEntropy s "specific entropy";
    Temperature T "temperature";
    Integer phase "phase flag: 2 for two-phase, 1 for one-phase";
    Modelica.SIunits.CubicExpansionCoefficient beta
      "isobaric expansion coefficient";
    Modelica.SIunits.SpecificHeatCapacityAtConstantPressure cp
      "specific heat capacity cp";
    Modelica.SIunits.SpecificHeatCapacityAtConstantVolume cv
      "specific heat capacity cv";
    Modelica.SIunits.DerDensityByPressure dd_dp_h
      "derivative of density wrt pressure at constant enthalpy";
    Modelica.SIunits.DerDensityByEnthalpy dd_dh_p
      "derivative of density wrt enthalpy at constant pressure";
    Modelica.SIunits.Compressibility kappa "compressibility";
    Real dT_dp_h "derivative of temperature wrt pressure at constant enthalpy";
    Real dT_dh_p "derivative of temperature wrt enthalpy at constant pressure";
    Modelica.SIunits.AbsolutePressure ps "saturation pressure";
    Modelica.SIunits.Temperature Ts "saturation temperature";
    Modelica.SIunits.Density dl "density at bubble line (for pressure ps)";
    Modelica.SIunits.Density dv "density at dew line (for pressure ps)";
    Modelica.SIunits.SpecificEnthalpy hl
      "specific enthalpy at bubble line (for pressure ps)";
    Modelica.SIunits.SpecificEnthalpy hv
      "specific enthalpy at dew line (for pressure ps)";
    Modelica.SIunits.SpecificEntropy sl
      "specific entropy at bubble line (for pressure ps)";
    Modelica.SIunits.SpecificEntropy sv
      "specific entropy at dew line (for pressure ps)";
    Modelica.SIunits.DynamicViscosity eta "dynamic viscosity";
    Modelica.SIunits.ThermalConductivity lambda "thermal conductivity";
    Modelica.SIunits.PrandtlNumber Pr "prandtl number";
    Modelica.SIunits.SurfaceTension sigma "surface tension";
    Modelica.SIunits.VelocityOfSound a "velocity of sound";
    Real d_Ts_dp "derivative of Ts wrt pressure";
    Modelica.SIunits.DerDensityByPressure d_dl_dp
      "derivative of dls wrt pressure";
    Modelica.SIunits.DerDensityByPressure d_dv_dp
      "derivative of dvs wrt pressure";
    Modelica.SIunits.DerEnthalpyByPressure d_hl_dp
      "derivative of hls wrt pressure";
    Modelica.SIunits.DerEnthalpyByPressure d_hv_dp
      "derivative of hvs wrt pressure";
  end ThermodynamicState;

  redeclare record extends SaturationProperties
    Modelica.SIunits.AbsolutePressure psat "saturation pressure";
    Modelica.SIunits.Temperature Tsat "saturation temperature";
  end SaturationProperties;

  redeclare model extends BaseProperties(
    p(stateSelect = if preferredMediumStates and
                       (basePropertiesInputChoice == InputChoice.ph or
                        basePropertiesInputChoice == InputChoice.pT or
                        basePropertiesInputChoice == InputChoice.ps) then
                            StateSelect.prefer else StateSelect.default),
    T(stateSelect = if preferredMediumStates and
                       (basePropertiesInputChoice == InputChoice.pT or
                       basePropertiesInputChoice == InputChoice.dT) then
                         StateSelect.prefer else StateSelect.default),
    h(stateSelect = if preferredMediumStates and
                       basePropertiesInputChoice == InputChoice.ph then
                         StateSelect.prefer else StateSelect.default),
    d(stateSelect = if preferredMediumStates and
                       basePropertiesInputChoice == InputChoice.dT then
                         StateSelect.prefer else StateSelect.default))
    import ExternalMedia.Common.InputChoice;
    parameter InputChoice basePropertiesInputChoice=inputChoice
      "Choice of input variables for property computations";
    FixedPhase phaseInput
      "Phase input for property computation functions, 2 for two-phase, 1 for one-phase, 0 if not known";
    Integer phaseOutput
      "Phase output for medium, 2 for two-phase, 1 for one-phase";
    Integer uniqueID(final start=0) "Unique ID of BaseProperty object";
    SpecificEntropy s(
      stateSelect = if basePropertiesInputChoice == InputChoice.ps then
                       StateSelect.prefer else StateSelect.default)
      "Specific entropy";
    SaturationProperties sat "saturation property record";
  algorithm
    when (initial()) then
      uniqueID := createMedium(uniqueID);
      assert(uniqueID>0, "Error in external medium library");
    end when;
  equation
    MM = externalFluidConstants.molarMass;
    R = Modelica.Constants.R/MM;
    if (onePhase or (basePropertiesInputChoice == InputChoice.pT)) then
      phaseInput = 1 "Force one-phase property computation";
    else
      phaseInput = 0 "Unknown phase";
    end if;
    if (basePropertiesInputChoice == InputChoice.ph) then
      // Compute the state record (including the unique ID)
      state = setState_ph(p, h, phaseInput, uniqueID);
      // Compute the remaining variables.
      // It is not possible to use the standard functions like
      // d = density(state), because differentiation for index
      // reduction and change of state variables would not be supported
      // density_ph(), which has an appropriate derivative annotation,
      // is used instead. The implementation of density_ph() uses
      // setState with the same inputs, so there's no actual overhead
      d = density_ph(p, h, phaseInput, uniqueID);
      s = specificEntropy_ph(p, h, phaseInput, uniqueID);
      T = temperature_ph(p, h, phaseInput, uniqueID);
    elseif (basePropertiesInputChoice == InputChoice.dT) then
      state = setState_dT(d, T, phaseInput, uniqueID);
      h = specificEnthalpy(state);
      p = pressure(state);
      s = specificEntropy(state);
    elseif (basePropertiesInputChoice == InputChoice.pT) then
      state = setState_pT(p, T, phaseInput, uniqueID);
      d = density(state);
      h = specificEnthalpy(state);
      s = specificEntropy(state);
    elseif (basePropertiesInputChoice == InputChoice.ps) then
      state = setState_ps(p, s, phaseInput, uniqueID);
      d = density(state);
      h = specificEnthalpy(state);
      T = temperature(state);
    end if;
    // Compute the internal energy
    u = h - p/d;
    // Compute the saturation properties record
    sat = setSat_p_state(state);
    // Event generation for phase boundary crossing
    if smoothModel then
      // No event generation
      phaseOutput = state.phase;
    else
      // Event generation at phase boundary crossing
      if basePropertiesInputChoice == InputChoice.ph then
        phaseOutput = if ((h > bubbleEnthalpy(sat) and h < dewEnthalpy(sat)) and
                           p < fluidConstants[1].criticalPressure) then 2 else 1;
      elseif basePropertiesInputChoice == InputChoice.dT then
        phaseOutput = if  ((d < bubbleDensity(sat) and d > dewDensity(sat)) and
                            T < fluidConstants[1].criticalTemperature) then 2 else 1;
      elseif basePropertiesInputChoice == InputChoice.ps then
        phaseOutput = if ((s > bubbleEntropy(sat) and s < dewEntropy(sat)) and
                           p < fluidConstants[1].criticalPressure) then 2 else 1;
      else
        // basePropertiesInputChoice == pT
        phaseOutput = 1;
      end if;
    end if;
  end BaseProperties;

  redeclare function molarMass "Return the molar mass of the medium"
      input ThermodynamicState state;
      output MolarMass MM "Mixture molar mass";
  algorithm
    MM := fluidConstants[1].molarMass;
  end molarMass;

  redeclare function extends setState_phX
  algorithm
    // The composition is an empty vector
    state :=setState_ph(p, h, phase);
  end setState_phX;

  redeclare function extends setState_pTX
  algorithm
    // The composition is an empty vector
    state :=setState_pT(p, T, phase);
  end setState_pTX;

  redeclare function extends setState_dTX
  algorithm
    // The composition is an empty vector
    state :=setState_dT(d, T, phase);
  end setState_dTX;

  redeclare function extends setState_psX
  algorithm
    // The composition is an empty vector
    state :=setState_ps(p, s, phase);
  end setState_psX;

  redeclare replaceable function density_ph "Return density from p and h"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output Density d "Density";
  algorithm
    d := density(setState_ph(p, h, phase));
    annotation(derivative(noDerivative = phase, noDerivative = uniqueID) = density_ph_der,
               Inline = false);
  end density_ph;

  redeclare replaceable function density_pT "Return density from p and T"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input Temperature T "Temperature";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output Density d "Density";
  algorithm
    d := density(setState_pT(p, T, phase));
    annotation(derivative(noDerivative = phase, noDerivative = uniqueID) = density_pT_der,
               Inline = false);
  end density_pT;

  replaceable partial function density_pT_der "Total derivative of density_pT"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input Temperature T "Temperature";
    input FixedPhase phase "2 for two-phase, 1 for one-phase, 0 if not known";
    input Real p_der;
    input Real T_der;
    output Real d_der;
  end density_pT_der;

  redeclare replaceable function density_ps "Return density from p and s"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output Density d "Density";
  algorithm
    d := density(setState_ps(p, s, phase));
    annotation(derivative(noDerivative = phase, noDerivative = uniqueID) = density_ps_der,
               Inline = false);
  end density_ps;

  replaceable partial function density_ps_der "Total derivative of density_ps"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase "2 for two-phase, 1 for one-phase, 0 if not known";
    input Real p_der;
    input Real h_der;
    output Real d_der;
  end density_ps_der;

  redeclare replaceable function temperature_ph
    "Return temperature from p and h"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output Temperature T "Temperature";
  algorithm
    T := temperature(setState_ph(p, h, phase));
    annotation(derivative(noDerivative = phase, noDerivative = uniqueID) = temperature_ph_der,
               Inline = false);
  end temperature_ph;

  redeclare replaceable function temperature_ps
    "Return temperature from p and s"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output Temperature T "Temperature";
  algorithm
    T := temperature(setState_ps(p, s, phase));
  end temperature_ps;

  replaceable function specificEntropy_ph
    "Return specific entropy from p and h"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output SpecificEntropy s "specific entropy";
  algorithm
    s := specificEntropy(setState_ph(p, h, phase));
  end specificEntropy_ph;

  redeclare replaceable function pressure_dT "Return pressure from d and T"
    extends Modelica.Icons.Function;
    input Density d "Density";
    input Temperature T "Temperature";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output AbsolutePressure p "Pressure";
  algorithm
    p := pressure(setState_dT(d, T, phase));
  end pressure_dT;

  redeclare replaceable function specificEnthalpy_pT
    "Return specific enthalpy from p and T"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input Temperature T "Temperature";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output SpecificEnthalpy h "specific enthalpy";
  algorithm
    h := specificEnthalpy(setState_pT(p, T, phase));
  end specificEnthalpy_pT;

/*
  redeclare replaceable function specificEntropy_pT 
    "Return specific entropy from p and T" 
    extends Modelica.Icons.Function;
    input Pressure p "Pressure";
    input Temperature T "Temperature";
    input FixedPhase phase "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID "Unique ID";
    output SpecificEntropy s "specific entropy";
  algorithm 
    s := specificEntropy(setState_pT(p, T, phase, uniqueID));
  end specificEntropy_ph;
*/

  redeclare replaceable function specificEnthalpy_dT
    "Return specific enthalpy from d and T"
    extends Modelica.Icons.Function;
    input Density d "Density";
    input Temperature T "Temperature";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output SpecificEnthalpy h "specific enthalpy";
  algorithm
    h := specificEnthalpy(setState_dT(d, T, phase));
  end specificEnthalpy_dT;

/*
  redeclare replaceable function specificEntropy_dT 
    "Return specific entropy from d and T" 
    extends Modelica.Icons.Function;
    input Density d "Density";
    input Temperature T "Temperature";
    input FixedPhase phase "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID "Unique ID";
    output SpecificEntropy s "specific entropy";
  algorithm 
    s := specificEntropy(setState_dt(d, T, phase, uniqueID));
  end specificEntropy_ph;
*/

  redeclare replaceable function specificEnthalpy_ps
    "Return specific enthalpy from p and s"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output SpecificEnthalpy h "specific enthalpy";
  algorithm
    h := specificEnthalpy(setState_ps(p,s, phase));
  end specificEnthalpy_ps;

  replaceable partial function getMolarMass
    output MolarMass MM "molar mass";
    external "C" MM=  TwoPhaseMedium_getMolarMass_(mediumName, libraryName, substanceName)
      annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getMolarMass;

  replaceable partial function getCriticalTemperature
    output Temperature Tc "Critical temperature";
    external "C" Tc=  TwoPhaseMedium_getCriticalTemperature_(mediumName, libraryName, substanceName)
      annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getCriticalTemperature;

  replaceable partial function getCriticalPressure
    output AbsolutePressure pc "Critical temperature";
    external "C" pc=  TwoPhaseMedium_getCriticalPressure_(mediumName, libraryName, substanceName)
      annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getCriticalPressure;

  replaceable partial function getCriticalMolarVolume
    output MolarVolume vc "Critical molar volume";
    external "C" vc=  TwoPhaseMedium_getCriticalMolarVolume_(mediumName, libraryName, substanceName)
      annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getCriticalMolarVolume;

  redeclare replaceable function extends setState_ph
    "Return thermodynamic state record from p and h"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "pressure";
    input SpecificEnthalpy h "specific enthalpy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  external "C" TwoPhaseMedium_setState_ph_(p, h, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_ph;

  redeclare replaceable function extends setState_pT
    "Return thermodynamic state record from p and T"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "pressure";
    input Temperature T "temperature";
    output ThermodynamicState state;
  external "C" TwoPhaseMedium_setState_pT_(p, T, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_pT;

  redeclare replaceable function extends setState_dT
    "Return thermodynamic state record from d and T"
    extends Modelica.Icons.Function;
    input Density d "density";
    input Temperature T "temperature";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  external "C" TwoPhaseMedium_setState_dT_(d, T, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_dT;

  redeclare replaceable function extends setState_ps
    "Return thermodynamic state record from p and s"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "pressure";
    input SpecificEntropy s "specific entropy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  external "C" TwoPhaseMedium_setState_ps_(p, s, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_ps;

  redeclare replaceable function extends setDewState
    "set the thermodynamic state on the dew line"
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation point";
    input FixedPhase phase =  1 "phase: default is one phase";
    output ThermodynamicState state "complete thermodynamic state info";
  external "C" TwoPhaseMedium_setDewState_(sat.uniqueID, phase, state.uniqueID, state.phase, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setDewState;

  redeclare replaceable function extends setBubbleState
    "set the thermodynamic state on the bubble line"
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation point";
    input FixedPhase phase =  1 "phase: default is one phase";
    output ThermodynamicState state "complete thermodynamic state info";
  external "C" TwoPhaseMedium_setBubbleState_(sat.uniqueID, phase, state.uniqueID, state.phase, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setBubbleState;

  redeclare replaceable function extends density "Return density from state"
  algorithm
    d := state.d;
  end density;

  redeclare replaceable function extends pressure "Return pressure from state"
  algorithm
    p := state.p;
  end pressure;

  redeclare replaceable function extends specificEnthalpy
    "Return specific enthalpy from state"
  algorithm
    h := state.h;
  end specificEnthalpy;

  redeclare replaceable function extends specificEntropy
    "Return specific entropy from state"
  algorithm
    s := state.s;
  end specificEntropy;

  redeclare replaceable function extends temperature
    "Return temperature from state"
  algorithm
    T := state.T;
  end temperature;

  redeclare replaceable function extends isentropicEnthalpy
  external "C" h_is=  TwoPhaseMedium_isentropicEnthalpy_(p_downstream, refState.uniqueID, inputChoice, refState.d, refState.h, refState.p, refState.s, refState.T, refState.phase,
   mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end isentropicEnthalpy;

  redeclare replaceable function extends isobaricExpansionCoefficient
    "Return isobaric expansion coefficient from state"
  algorithm
    beta := state.beta;
  end isobaricExpansionCoefficient;

  redeclare replaceable function extends isothermalCompressibility
    "Return isothermal compressibility from state"
  algorithm
    kappa := state.kappa;
  end isothermalCompressibility;

  redeclare replaceable function extends specificHeatCapacityCp
    "Return specific heat capacity cp from state"
  algorithm
    cp := state.cp;
  end specificHeatCapacityCp;

  redeclare replaceable function extends specificHeatCapacityCv
    "Return specific heat capacity cv from state"
  algorithm
    cv := state.cv;
  end specificHeatCapacityCv;

  redeclare replaceable function extends density_derp_h
    "Return derivative of density wrt pressure at constant enthalpy from state"
  algorithm
    ddph := state.dd_dp_h;
  end density_derp_h;

  redeclare replaceable function extends density_derh_p
    "Return derivative of density wrt enthalpy at constant pressure from state"
  algorithm
    ddhp := state.dd_dh_p;
  end density_derh_p;

  redeclare replaceable function extends thermalConductivity
    "Return thermal conductivity from state"
  algorithm
    lambda := state.lambda;
  end thermalConductivity;

  redeclare replaceable function extends dynamicViscosity
    "Return dynamic viscosity from state"
  algorithm
    eta := state.eta;
  end dynamicViscosity;

  redeclare replaceable function extends velocityOfSound
    "Return velocity of sound from state"
  algorithm
    a := state.a;
  end velocityOfSound;

  replaceable function density_ph_der "Total derivative of density_ph"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase "2 for two-phase, 1 for one-phase, 0 if not known";
    input Real p_der;
    input Real h_der;
    output Real d_der;
  external "C" d_der=  TwoPhaseMedium_density_ph_der_(uniqueID, p_der, h_der, p, h, phase, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end density_ph_der;

  replaceable function temperature_ph_der "Total derivative of density_ph"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase "2 for two-phase, 1 for one-phase, 0 if not known";
    input Real p_der;
    input Real h_der;
    output Real T_der;
  external "C" T_der=  TwoPhaseMedium_temperature_ph_der_(uniqueID, p_der, h_der, p, h, phase, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end temperature_ph_der;

  redeclare replaceable function setSat_p "Return saturation properties from p"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "pressure";
    output SaturationProperties sat "saturation property record";
  external "C" TwoPhaseMedium_setSat_p_(p, sat.psat, sat.Tsat, sat, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setSat_p;

  redeclare replaceable function setSat_T "Return saturation properties from p"
    extends Modelica.Icons.Function;
    input Temperature T "temperature";
    output SaturationProperties sat "saturation property record";
  external "C" TwoPhaseMedium_setSat_T_(T, sat.psat, sat.Tsat, sat, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setSat_T;

  redeclare replaceable function extends saturationPressure
  external "C" p=  TwoPhaseMedium_saturationPressure_(T, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end saturationPressure;

  redeclare replaceable function extends saturationTemperature
  external "C" T=  TwoPhaseMedium_saturationTemperature_(p, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end saturationTemperature;

  redeclare replaceable function extends saturationTemperature_derp
  external "C" dTp=  TwoPhaseMedium_saturationTemperature_derp_(p, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end saturationTemperature_derp;

  redeclare replaceable function saturationTemperature_derp_sat
    "Returns derivative of saturation temperature w.r.t.. pressure"
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation property record";
    output Real dTp "derivative of saturation temperature w.r.t. pressure";
  external "C" dTp=  TwoPhaseMedium_saturationTemperature_derp_sat_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end saturationTemperature_derp_sat;

  redeclare replaceable function extends bubbleDensity
  external "C" dl=  TwoPhaseMedium_bubbleDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end bubbleDensity;

  redeclare replaceable function extends dewDensity
  external "C" dv=  TwoPhaseMedium_dewDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dewDensity;

  redeclare replaceable function extends bubbleEnthalpy
  external "C" hl=  TwoPhaseMedium_bubbleEnthalpy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end bubbleEnthalpy;

  redeclare replaceable function extends dewEnthalpy
  external "C" hv=  TwoPhaseMedium_dewEnthalpy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dewEnthalpy;

  redeclare replaceable function extends bubbleEntropy
  external "C" sl=  TwoPhaseMedium_bubbleEntropy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end bubbleEntropy;

  redeclare replaceable function extends dewEntropy
  external "C" sv=  TwoPhaseMedium_dewDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dewEntropy;

  redeclare replaceable function extends dBubbleDensity_dPressure
  external "C" ddldp=  TwoPhaseMedium_dBubbleDensity_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dBubbleDensity_dPressure;

  redeclare replaceable function extends dDewDensity_dPressure
  external "C" ddvdp=  TwoPhaseMedium_dDewDensity_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dDewDensity_dPressure;

  redeclare replaceable function extends dBubbleEnthalpy_dPressure
  external "C" dhldp=  TwoPhaseMedium_dBubbleEnthalpy_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dBubbleEnthalpy_dPressure;

  redeclare replaceable function extends dDewEnthalpy_dPressure
  external "C" dhvdp=  TwoPhaseMedium_dDewEnthalpy_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dDewEnthalpy_dPressure;

  redeclare replaceable function extends surfaceTension
  external "C" sigma=  TwoPhaseMedium_surfaceTension_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end surfaceTension;

end ExternalTwoPhaseMedium;
