within ExternalMedia.Media;
package CoolPropMedium "Medium package accessing the CoolProp solver"
  extends BaseClasses.ExternalTwoPhaseMedium(final libraryName="CoolProp");
  import ExternalMedia.Common.InputChoice;

  redeclare replaceable model extends BaseProperties
  equation
    if (basePropertiesInputChoice == InputChoice.hs) then
      state = setState_hs(
          h,
          s,
          phaseInput);
      d = density(state);
      p = pressure(state);
      T = temperature(state);
    end if;
  end BaseProperties;

  replaceable function setState_hs
    "Return thermodynamic state record from h and s"
    extends Modelica.Icons.Function;
    input SpecificEnthalpy h "specific enthalpy";
    input SpecificEntropy s "specific entropy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  external"C" TwoPhaseMedium_setState_hs_(
        h,
        s,
        phase,
        state,
        mediumName,
        libraryName,
        substanceName)
    annotation(Include="#include <ExternalMediaLib.h>", Library="ExternalMediaLib");
  end setState_hs;

  replaceable function setState_hsX
    extends Modelica.Icons.Function;
    input SpecificEnthalpy h "specific enthalpy";
    input SpecificEntropy s "specific entropy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  algorithm
    // The composition is an empty vector
    state := setState_hs(
        h,
        s,
        phase);
  end setState_hsX;

  function density_hs "returns density for given h and s"
    extends Modelica.Icons.Function;
    input SpecificEnthalpy h "Enthalpy";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    output Density d "density";
  algorithm
    d := density_hs_state(
        h=h,
        s=s,
        state=setState_hs(
          h=h,
          s=s,
          phase=phase));
    annotation (Inline=true);
  end density_hs;

  function density_hs_state "returns density for given h and s"
    extends Modelica.Icons.Function;
    input SpecificEnthalpy h "Enthalpy";
    input SpecificEntropy s "Specific entropy";
    input ThermodynamicState state;
    output Density d "density";
  algorithm
    d := density(state);
    annotation (Inline=false, LateInline=true);
  end density_hs_state;

  replaceable function pressure_hs "Return pressure from h and s"
    extends Modelica.Icons.Function;
    input SpecificEnthalpy h "Specific enthalpy";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    output AbsolutePressure p "Pressure";
  algorithm
    p := pressure_hs_state(
        h=h,
        s=s,
        state=setState_hs(
          h=h,
          s=s,
          phase=phase));
    annotation (Inline=true, inverse(h=specificEnthalpy_ps(
              p=p,
              s=s,
              phase=phase), s=specificEntropy_ph(
              p=p,
              h=h,
              phase=phase)));
  end pressure_hs;

  function pressure_hs_state "returns pressure for given h and s"
    extends Modelica.Icons.Function;
    input SpecificEnthalpy h "Enthalpy";
    input SpecificEntropy s "Specific entropy";
    input ThermodynamicState state;
    output AbsolutePressure p "Pressure";
  algorithm
    p := pressure(state);
    annotation (Inline=false, LateInline=true);
  end pressure_hs_state;

  replaceable function temperature_hs "Return temperature from h and s"
    extends Modelica.Icons.Function;
    input SpecificEnthalpy h "Specific enthalpy";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    output Temperature T "Temperature";
  algorithm
    T := temperature_hs_state(
        h=h,
        s=s,
        state=setState_hs(
          h=h,
          s=s,
          phase=phase));
    annotation (Inline=true);
  end temperature_hs;

  function temperature_hs_state "returns temperature for given h and s"
    extends Modelica.Icons.Function;
    input SpecificEnthalpy h "Enthalpy";
    input SpecificEntropy s "Specific entropy";
    input ThermodynamicState state;
    output Temperature T "Temperature";
  algorithm
    T := temperature(state);
    annotation (Inline=false, LateInline=true);
  end temperature_hs_state;

  redeclare replaceable function extends isentropicExponent
    "Return isentropic exponent"
    extends Modelica.Icons.Function;
    input ThermodynamicState state "thermodynamic state record";
    output IsentropicExponent gamma "Isentropic exponent";
  algorithm
    gamma := density(state)/pressure(state)*velocityOfSound(state)*
      velocityOfSound(state);
  end isentropicExponent;

  redeclare replaceable function extends specificInternalEnergy
    "Returns specific internal energy"
    extends Modelica.Icons.Function;
    input ThermodynamicState state "thermodynamic state record";
    output SpecificInternalEnergy u "specific internal energy";
  algorithm
    u := specificEnthalpy(state) - pressure(state)/density(state);
  end specificInternalEnergy;

  redeclare replaceable function isentropicEnthalpy
    input AbsolutePressure p_downstream "downstream pressure";
    input ThermodynamicState refState "reference state for entropy";
    output SpecificEnthalpy h_is "Isentropic enthalpy";
  protected
    SpecificEntropy s_ideal;
    ThermodynamicState state_ideal;
  algorithm
    s_ideal := specificEntropy(refState);
    state_ideal := setState_psX(p_downstream, s_ideal);
    h_is := specificEnthalpy(state_ideal);
  end isentropicEnthalpy;

  redeclare replaceable function setBubbleState
    "Set the thermodynamic state on the bubble line"
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation point";
    input FixedPhase phase=0 "phase flag";
    output ThermodynamicState state "complete thermodynamic state info";
    // Standard definition
  external"C" TwoPhaseMedium_setBubbleState_C_impl(
        sat,
        phase,
        state,
        mediumName,
        libraryName,
        substanceName)
      annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
    annotation (Inline=true);
  end setBubbleState;

  redeclare replaceable function setDewState
    "Set the thermodynamic state on the dew line"
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation point";
    input FixedPhase phase=0 "phase flag";
    output ThermodynamicState state "complete thermodynamic state info";
    // Standard definition
  external"C" TwoPhaseMedium_setDewState_C_impl(
        sat,
        phase,
        state,
        mediumName,
        libraryName,
        substanceName)
      annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
    annotation (Inline=true);
  end setDewState;

  redeclare function bubbleEntropy "Return bubble point specific entropy"
    input SaturationProperties sat "saturation property record";
    output SI.SpecificEntropy sl "boiling curve specific entropy";
  algorithm
    sl := specificEntropy(setBubbleState(sat));
  end bubbleEntropy;

  redeclare function dewEntropy "Return dew point specific entropy"
    input SaturationProperties sat "saturation property record";
    output SI.SpecificEntropy sv "dew curve specific entropy";
  algorithm
    sv := specificEntropy(setDewState(sat));
  end dewEntropy;

  redeclare function surfaceTension
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation property record";
    output SurfaceTension sigma "Surface tension sigma in the two phase region";
  algorithm
    assert(false, "The CoolProp solver does not provide surface tension");
  end surfaceTension;
end CoolPropMedium;
