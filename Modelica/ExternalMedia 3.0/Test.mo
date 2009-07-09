within ExternalMedia;
package Test
  package TestMedium "Test cases for TestMedium"
    model TestStatesSat
      "Test case using TestMedium, with baseProperties and state + sat records without explicit uniqueID handling"
      replaceable package Medium = Media.TestMedium;
      Medium.BaseProperties baseProperties1;
      Medium.BaseProperties baseProperties2;
      Medium.ThermodynamicState state1;
      Medium.ThermodynamicState state2;
      Medium.SaturationProperties sat1;
      Medium.SaturationProperties sat2;

      Medium.Temperature Ts;
      Medium.AbsolutePressure ps;

      ExternalMedia.Test.TestMedium.GenericModels.CompleteThermodynamicState
        completeState1(                         redeclare package Medium = Medium,
                                                state = state1);
      ExternalMedia.Test.TestMedium.GenericModels.CompleteThermodynamicState
        completeState2(                         redeclare package Medium = Medium,
                                                state = state2);
      ExternalMedia.Test.TestMedium.GenericModels.CompleteSaturationProperties
        completeSat1(                           redeclare package Medium = Medium,
                                                sat = sat1);
      ExternalMedia.Test.TestMedium.GenericModels.CompleteSaturationProperties
        completeSat2(                           redeclare package Medium = Medium,
                                                sat = sat2);
      ExternalMedia.Test.TestMedium.GenericModels.CompleteBubbleDewStates
        completeBubbleDewStates1(                      redeclare package Medium
          =                                                                       Medium,
                                                       sat = sat1);
      ExternalMedia.Test.TestMedium.GenericModels.CompleteBubbleDewStates
        completeBubbleDewStates2(                      redeclare package Medium
          =                                                                       Medium,
                                                       sat = sat1);
    equation
      baseProperties1.p = 1e5+1e5*time;
      baseProperties1.h = 1e5;
      baseProperties2.p = 1e5;
      baseProperties2.h = 1e5 + 2e5*time;

      state1 = Medium.setState_ph(1e5 + 1e5*time, 1e5);
      state2 = Medium.setState_pT(1e5, 300+ 50*time);

      sat1 = Medium.setSat_p(1e5 + 1e5*time);
      sat2 = Medium.setSat_T(300 + 50 * time);

      Ts = Medium.saturationTemperature(1e5+1e5*time);
      ps = Medium.saturationPressure(300 + 50*time);
    end TestStatesSat;

    model TestBasePropertiesExplicit
      "Test case using TestMedium and BaseProperties with explicit equations"
      replaceable package Medium = Media.TestMedium;
      ExternalMedia.Test.TestMedium.GenericModels.CompleteBaseProperties
        medium1(                 redeclare package Medium = Medium)
        "Constant pressure, varying enthalpy";
      ExternalMedia.Test.TestMedium.GenericModels.CompleteBaseProperties
        medium2(                 redeclare package Medium = Medium)
        "Varying pressure, constant enthalpy";
    equation
      medium1.baseProperties.p = 1e5+1e5*time;
      medium1.baseProperties.h = 1e5;
      medium2.baseProperties.p = 1e5;
      medium2.baseProperties.h = 1e5 + 2e5*time;
    end TestBasePropertiesExplicit;

    model TestBasePropertiesImplicit
      "Test case using TestMedium and BaseProperties with implicit equations"
      replaceable package Medium = Media.TestMedium;
      ExternalMedia.Test.TestMedium.GenericModels.CompleteBaseProperties
        medium1(
        redeclare package Medium = Medium,
        baseProperties(h(start=1e5))) "Constant pressure, varying enthalpy";
      ExternalMedia.Test.TestMedium.GenericModels.CompleteBaseProperties
        medium2(
        redeclare package Medium = Medium,
        baseProperties(h(start=1e5))) "Varying pressure, constant enthalpy";
    equation
      medium1.baseProperties.p = 1e5*time;
      medium1.baseProperties.T = 300 + 25*time;
      medium2.baseProperties.p = 1e5+1e5*time;
      medium2.baseProperties.T = 300;
    end TestBasePropertiesImplicit;

    model TestBasePropertiesDynamic
      "Test case using TestMedium and dynamic equations"
      replaceable package Medium = Media.TestMedium;
      parameter SI.Volume V = 1 "Storage Volume";
      parameter Real p_atm = 101325 "Atmospheric pressure";
      parameter SI.Temperature Tstart = 300;
      parameter Real Kv0 = 1.00801e-2 "Valve flow coefficient";
      Medium.BaseProperties medium(preferredMediumStates = true);
      SI.Mass M;
      SI.Energy U;
      SI.MassFlowRate win(start = 100);
      SI.MassFlowRate wout;
      SI.SpecificEnthalpy hin;
      SI.SpecificEnthalpy hout;
      SI.Power Q;
      Real Kv;
    equation
      // Mass & energy balance equation
      M = medium.d*V;
      U = medium.u*M;
      der(M) = win - wout;
      der(U) = win*hin - wout*hout + Q;

      // Inlet pump equations
      medium.p - p_atm = 2e5 - (1e5/100^2)*win^2;
      hin = 1e5;

      // Outlet valve equation
      wout = Kv * sqrt(medium.d*(medium.p - p_atm));
      hout = medium.h;

      // Input variables
      Kv = if time<50 then Kv0 else Kv0*1.1;
      Q = if time < 1 then 0 else 1e7;
    initial equation
      // Initial conditions

      // Fixed initial states
      // medium.p = 2e5;
      // medium.h = 1e5;

      // Steady state equations
      der(medium.p) = 0;
      der(medium.h) = 0;
      annotation (experiment(StopTime=80, Tolerance=1e-007),experimentSetupOutput(
          equdistant=false));
    end TestBasePropertiesDynamic;

    package GenericModels
      "Contains generic models to use for thorough medium model testing"

      model CompleteFluidConstants
        "Compute all available medium fluid constants"
        replaceable package Medium = 
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // Fluid constants
        Medium.Temperature Tc = Medium.fluidConstants[1].criticalTemperature;
        Medium.AbsolutePressure pc = Medium.fluidConstants[1].criticalPressure;
        Medium.MolarVolume vc = Medium.fluidConstants[1].criticalMolarVolume;
        Medium.MolarMass MM = Medium.fluidConstants[1].molarMass;
      end CompleteFluidConstants;

      model CompleteThermodynamicState
        "Compute all available two-phase medium properties from a ThermodynamicState model"
        replaceable package Medium = 
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // ThermodynamicState record
        Medium.ThermodynamicState state;

        // Medium properties
        Medium.AbsolutePressure p =                Medium.pressure(state);
        Medium.SpecificEnthalpy h =                Medium.specificEnthalpy(state);
        Medium.Temperature T =                     Medium.temperature(state);
        Medium.Density d =                         Medium.density(state);
        Medium.SpecificEntropy s =                 Medium.specificEntropy(state);
        Medium.SpecificHeatCapacity cp =           Medium.specificHeatCapacityCp(state);
        Medium.SpecificHeatCapacity cv =           Medium.specificHeatCapacityCv(state);
        Medium.IsobaricExpansionCoefficient beta = Medium.isobaricExpansionCoefficient(state);
        SI.IsothermalCompressibility kappa =       Medium.isothermalCompressibility(state);
        Medium.DerDensityByPressure d_d_dp_h =     Medium.density_derp_h(state);
        Medium.DerDensityByEnthalpy d_d_dh_p =     Medium.density_derh_p(state);
        Medium.MolarMass MM =                      Medium.molarMass(state);
      end CompleteThermodynamicState;

      model CompleteSaturationProperties
        "Compute all available saturation properties from a SaturationProperties record"
        replaceable package Medium = 
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // SaturationProperties record
        Medium.SaturationProperties sat;

        // Saturation properties
        Medium.Temperature Ts =      Medium.saturationTemperature_sat(sat);
        Medium.Density dl =          Medium.bubbleDensity(sat);
        Medium.Density dv =          Medium.dewDensity(sat);
        Medium.SpecificEnthalpy hl = Medium.bubbleEnthalpy(sat);
        Medium.SpecificEnthalpy hv = Medium.dewEnthalpy(sat);
        Real d_Ts_dp =               Medium.saturationTemperature_derp_sat(sat);
        Real d_dl_dp =               Medium.dBubbleDensity_dPressure(sat);
        Real d_dv_dp =               Medium.dDewDensity_dPressure(sat);
        Real d_hl_dp =               Medium.dBubbleEnthalpy_dPressure(sat);
        Real d_hv_dp =               Medium.dDewEnthalpy_dPressure(sat);
      end CompleteSaturationProperties;

      model CompleteBubbleDewStates
        "Compute all available properties for dewpoint and bubble point states corresponding to a sat record"
        replaceable package Medium = 
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // SaturationProperties record
        Medium.SaturationProperties sat;

        ExternalMedia.Test.TestMedium.GenericModels.CompleteThermodynamicState
          dewStateOnePhase(                         state=
              Medium.setDewState(sat, 1), redeclare package Medium = Medium);
        ExternalMedia.Test.TestMedium.GenericModels.CompleteThermodynamicState
          dewStateTwoPhase(                         state=
              Medium.setDewState(sat, 2), redeclare package Medium = Medium);
        ExternalMedia.Test.TestMedium.GenericModels.CompleteThermodynamicState
          bubbleStateOnePhase(                         state=
              Medium.setBubbleState(sat, 1), redeclare package Medium = Medium);
        ExternalMedia.Test.TestMedium.GenericModels.CompleteThermodynamicState
          bubbleStateTwoPhase(                         state=
              Medium.setBubbleState(sat, 2), redeclare package Medium = Medium);
      end CompleteBubbleDewStates;

      model CompleteBaseProperties
        "Compute all available two-phase medium properties from a BaseProperties model"
        replaceable package Medium = 
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // BaseProperties object
        Medium.BaseProperties baseProperties;

        // All the complete properties
        ExternalMedia.Test.TestMedium.GenericModels.CompleteThermodynamicState
          completeState(                         redeclare package Medium = 
                     Medium, state=baseProperties.state);
        ExternalMedia.Test.TestMedium.GenericModels.CompleteSaturationProperties
          completeSat(                           redeclare package Medium = 
                     Medium, sat=baseProperties.sat);
        ExternalMedia.Test.TestMedium.GenericModels.CompleteFluidConstants
          completeConstants(                     redeclare package Medium = 
                     Medium);
        ExternalMedia.Test.TestMedium.GenericModels.CompleteBubbleDewStates
          completeBubbleDewStates(
            redeclare package Medium = Medium, sat=baseProperties.sat);
      end CompleteBaseProperties;
    end GenericModels;
  end TestMedium;

  package FluidProp "Test cases for FluidPropMedium"

    partial package GenericModels "Generic models for FluidProp media tests"
      model CompleteFluidConstants
        "Compute all available medium fluid constants"
        replaceable package Medium = 
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // Fluid constants
        Medium.Temperature Tc = Medium.fluidConstants[1].criticalTemperature;
        Medium.AbsolutePressure pc = Medium.fluidConstants[1].criticalPressure;
        Medium.MolarVolume vc = Medium.fluidConstants[1].criticalMolarVolume;
        Medium.MolarMass MM = Medium.fluidConstants[1].molarMass;
      end CompleteFluidConstants;

      model CompleteThermodynamicState
        "Compute all available two-phase medium properties from a ThermodynamicState model"
        replaceable package Medium = 
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // ThermodynamicState record
        Medium.ThermodynamicState state;

        // Medium properties
        Medium.AbsolutePressure p =                Medium.pressure(state);
        Medium.SpecificEnthalpy h =                Medium.specificEnthalpy(state);
        Medium.Temperature T =                     Medium.temperature(state);
        Medium.Density d =                         Medium.density(state);
        Medium.SpecificEntropy s =                 Medium.specificEntropy(state);
        Medium.SpecificHeatCapacity cp =           Medium.specificHeatCapacityCp(state);
        Medium.SpecificHeatCapacity cv =           Medium.specificHeatCapacityCv(state);
      // Not yet implemented in FluidProp
        Medium.IsobaricExpansionCoefficient beta = Medium.isobaricExpansionCoefficient(state);
        SI.IsothermalCompressibility kappa =       Medium.isothermalCompressibility(state);
        Medium.DerDensityByPressure d_d_dp_h =     Medium.density_derp_h(state);
        Medium.DerDensityByEnthalpy d_d_dh_p =     Medium.density_derh_p(state);
        Medium.MolarMass MM =                      Medium.molarMass(state);
      end CompleteThermodynamicState;

      model CompleteSaturationProperties
        "Compute all available saturation properties from a SaturationProperties record"
        replaceable package Medium = 
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // SaturationProperties record
        Medium.SaturationProperties sat;

        // Saturation properties
        Medium.Temperature Ts =      Medium.saturationTemperature_sat(sat);
        Medium.Density dl =          Medium.bubbleDensity(sat);
        Medium.Density dv =          Medium.dewDensity(sat);
        Medium.SpecificEnthalpy hl = Medium.bubbleEnthalpy(sat);
        Medium.SpecificEnthalpy hv = Medium.dewEnthalpy(sat);
        Real d_Ts_dp =               Medium.saturationTemperature_derp_sat(sat);
        Real d_dl_dp =               Medium.dBubbleDensity_dPressure(sat);
        Real d_dv_dp =               Medium.dDewDensity_dPressure(sat);
        Real d_hl_dp =               Medium.dBubbleEnthalpy_dPressure(sat);
        Real d_hv_dp =               Medium.dDewEnthalpy_dPressure(sat);
      end CompleteSaturationProperties;

      model CompleteBubbleDewStates
        "Compute all available properties for dewpoint and bubble point states corresponding to a sat record"
        replaceable package Medium = 
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // SaturationProperties record
        Medium.SaturationProperties sat;

        CompleteThermodynamicState dewStateOnePhase(state=
              Medium.setDewState(sat, 1), redeclare package Medium = Medium);
        CompleteThermodynamicState dewStateTwoPhase(state=
              Medium.setDewState(sat, 2), redeclare package Medium = Medium);
        CompleteThermodynamicState bubbleStateOnePhase(state=
              Medium.setBubbleState(sat, 1), redeclare package Medium = Medium);
        CompleteThermodynamicState bubbleStateTwoPhase(state=
              Medium.setBubbleState(sat, 2), redeclare package Medium = Medium);
      end CompleteBubbleDewStates;

      model CompleteBaseProperties
        "Compute all available two-phase medium properties from a BaseProperties model"
        replaceable package Medium = 
              Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // BaseProperties object
        Medium.BaseProperties baseProperties;

        // All the complete properties
        CompleteThermodynamicState completeState(redeclare package Medium = 
                             Medium, state=baseProperties.state);
        CompleteSaturationProperties completeSat(redeclare package Medium = 
                             Medium, sat=baseProperties.sat);
        CompleteFluidConstants completeConstants(redeclare package Medium = 
                             Medium);
        CompleteBubbleDewStates completeBubbleDewStates(
            redeclare package Medium = Medium, sat=baseProperties.sat);
      end CompleteBaseProperties;

    end GenericModels;

    package IF95 "Test suite for the FluidProp-Refprop IF95 medium model"
      model TestStatesSat
        "Test case using TestMedium, with baseProperties and state + sat records without explicit uniqueID handling"
        replaceable package Medium = Media.FluidPropMedia.WaterIF95;
        Medium.BaseProperties baseProperties1;
        Medium.BaseProperties baseProperties2;
        Medium.ThermodynamicState state1;
        Medium.ThermodynamicState state2;
        Medium.SaturationProperties sat1;
        Medium.SaturationProperties sat2;

        Medium.Temperature Ts;
        Medium.AbsolutePressure ps;

        ExternalMedia.Test.FluidProp.GenericModels.CompleteThermodynamicState
          completeState1(                                    redeclare package
            Medium = 
              Medium, state=state1);
        ExternalMedia.Test.FluidProp.GenericModels.CompleteThermodynamicState
          completeState2(                                    redeclare package
            Medium = 
              Medium, state=state2);
        ExternalMedia.Test.FluidProp.GenericModels.CompleteSaturationProperties
          completeSat1(                                      redeclare package
            Medium = 
              Medium, sat=sat1);
        ExternalMedia.Test.FluidProp.GenericModels.CompleteSaturationProperties
          completeSat2(                                      redeclare package
            Medium = 
              Medium, sat=sat2);
        ExternalMedia.Test.FluidProp.GenericModels.CompleteBubbleDewStates
          completeBubbleDewStates1(                                 redeclare
            package Medium = 
                     Medium, sat=sat1);
        ExternalMedia.Test.FluidProp.GenericModels.CompleteBubbleDewStates
          completeBubbleDewStates2(                                 redeclare
            package Medium = 
                     Medium, sat=sat1);
      equation
        baseProperties1.p = 1e5+1e5*time;
        baseProperties1.h = 1e5;
        baseProperties2.p = 1e5;
        baseProperties2.h = 1e5 + 2e5*time;

        state1 = Medium.setState_ph(1e5 + 1e5*time, 1e5);
        state2 = Medium.setState_pT(1e5, 300+ 50*time);

        sat1 = Medium.setSat_p(1e5 + 1e5*time);
        sat2 = Medium.setSat_T(300 + 50 * time);

        Ts = Medium.saturationTemperature(1e5+1e5*time);
        ps = Medium.saturationPressure(300 + 50*time);
      end TestStatesSat;

      model TestBasePropertiesExplicit
        "Test case using FluidProp IF95 and explicit equations"
        replaceable package Medium = Media.FluidPropMedia.WaterIF95 
          constrainedby Modelica.Media.Interfaces.PartialTwoPhaseMedium;
        ExternalMedia.Test.FluidProp.GenericModels.CompleteBaseProperties
          medium1(                 redeclare package Medium = Medium)
          "Constant pressure, varying enthalpy";
        ExternalMedia.Test.FluidProp.GenericModels.CompleteBaseProperties
          medium2(                 redeclare package Medium = Medium)
          "Varying pressure, constant enthalpy";
      equation
        medium1.baseProperties.p = 1e5+1e5*time;
        medium1.baseProperties.h = 1e5;
        medium2.baseProperties.p = 1e5;
        medium2.baseProperties.h = 1e5 + 2e5*time;
      end TestBasePropertiesExplicit;

      model TestBasePropertiesImplicit
        "Test case using FluidProp IF95  and implicit equations"
        replaceable package Medium = Media.FluidPropMedia.WaterIF95 
          constrainedby Modelica.Media.Interfaces.PartialTwoPhaseMedium;
        ExternalMedia.Test.FluidProp.GenericModels.CompleteBaseProperties
          medium1(                 redeclare package Medium = Medium,
                                   baseProperties(h(start=1e5)))
          "Constant pressure, varying enthalpy";
        ExternalMedia.Test.FluidProp.GenericModels.CompleteBaseProperties
          medium2(                 redeclare package Medium = Medium,
                                   baseProperties(h(start=1e5)))
          "Varying pressure, constant enthalpy";
      equation
        medium1.baseProperties.p = 1e5+1e5*time;
        medium1.baseProperties.T = 300 + 25*time;
        medium2.baseProperties.p = 1e5+1e5*time;
        medium2.baseProperties.T = 300;
      end TestBasePropertiesImplicit;

    model TestBasePropertiesDynamic
        "Test case using FluidProp IF95 and dynamic equations"
      replaceable package Medium = Media.FluidPropMedia.WaterIF95 
        constrainedby Modelica.Media.Interfaces.PartialTwoPhaseMedium;
      parameter SI.Volume V = 1 "Storage Volume";
      parameter Real p_atm = 101325 "Atmospheric pressure";
      parameter SI.Temperature Tstart = 300;
      parameter Real Kv0 = 1.00801e-2 "Valve flow coefficient";
      Medium.BaseProperties medium(preferredMediumStates = true,
                                   h(start=1e5));
      SI.Mass M;
      SI.Energy U;
      SI.MassFlowRate win(start = 100);
      SI.MassFlowRate wout;
      SI.SpecificEnthalpy hin;
      SI.SpecificEnthalpy hout;
      SI.Power Q;
      Real Kv;
    //  SI.Pressure p(stateSelect = StateSelect.prefer, start = 1e5);
    //  SI.SpecificEnthalpy h(stateSelect = StateSelect.prefer, start = 2e5);
    equation
      // Mass & energy balance equation
      M = medium.d*V;
      U = medium.u*M;
      der(M) = win - wout;
      der(U) = win*hin - wout*hout + Q;

      // Preferred states
    //  p = medium.p;
    //  h = medium.h;

      // Inlet pump equations
      medium.p - p_atm = 2e5 - (1e5/100^2)*win^2;
      hin = 1e5;

      // Outlet valve equation
      wout = Kv * sqrt(medium.d*(medium.p - p_atm));
      hout = medium.h;

      // Input variables
      Kv = if time<50 then Kv0 else Kv0*1.1;
      Q = if time < 1 then 0 else 1e7;
    initial equation
      // Initial conditions

      // Fixed initial states
      // medium.p = 2e5;
      // medium.h = 1e5;

      // Steady state equations
      der(medium.p) = 0;
      der(medium.h) = 0;
      annotation (experiment(StopTime=80, Tolerance=1e-007),experimentSetupOutput(
            equdistant=false));
    end TestBasePropertiesDynamic;

      model TestBasePropertiesExplicit_ModelicaIF97
        "Test case using FluidProp IF95 and explicit equations"
        extends TestBasePropertiesExplicit(
          redeclare package Medium = Modelica.Media.Water.StandardWater);
      end TestBasePropertiesExplicit_ModelicaIF97;

      partial model CompareModelicaFluidProp
        "Comparison between Modelica IF97 and FluidProp IF95 models"
        ExternalMedia.Test.FluidProp.GenericModels.CompleteBaseProperties
          modelicaMedium(
          redeclare package Medium = Modelica.Media.Water.StandardWater)
          "Modelica IF97 model";
        ExternalMedia.Test.FluidProp.GenericModels.CompleteBaseProperties
          fluidPropMedium(
          redeclare package Medium = Media.FluidPropMedia.WaterIF95)
          "FluidProp IF95";
        parameter Modelica.SIunits.Pressure pmin;
        parameter Modelica.SIunits.Pressure pmax;
        parameter Modelica.SIunits.SpecificEnthalpy hmin;
        parameter Modelica.SIunits.SpecificEnthalpy hmax;
      equation
        modelicaMedium.baseProperties.p = pmin + (pmax-pmin)*time;
        modelicaMedium.baseProperties.h = hmin + (hmax-hmin)*time;
        fluidPropMedium.baseProperties.p = pmin + (pmax-pmin)*time;
        fluidPropMedium.baseProperties.h = hmin + (hmax-hmin)*time;
      end CompareModelicaFluidProp;

      model CompareModelicaFluidProp_liquid
        "Comparison between Modelica IF97 and FluidProp IF95 models - liquid"
        extends CompareModelicaFluidProp(
          pmin = 1e5,
          pmax = 1e5,
          hmin = 1e5,
          hmax = 4e5);

      end CompareModelicaFluidProp_liquid;

      model CompareModelicaFluidProp_twophase
        "Comparison between Modelica IF97 and FluidProp IF95 models - liquid"
        extends CompareModelicaFluidProp(
          pmin = 60e5,
          pmax = 60e5,
          hmin = 1000e3,
          hmax = 2000e3);

      end CompareModelicaFluidProp_twophase;

      model CompareModelicaFluidProp_vapour
        "Comparison between Modelica IF97 and FluidProp IF95 models - liquid"
        extends CompareModelicaFluidProp(
          pmin = 60e5,
          pmax = 60e5,
          hmin = 2800e3,
          hmax = 3200e3);

      end CompareModelicaFluidProp_vapour;
    end IF95;
  end FluidProp;

  package WrongMedium "Test cases with wrong medium models"

  model TestWrongMedium
      "Test the error reporting messages for unsupported external media"
    package Medium = Media.ExternalTwoPhaseMedium;
    Medium.BaseProperties medium;
  equation
    medium.p = 1e5;
    medium.h = 1e5;
  end TestWrongMedium;
  end WrongMedium;

  package SpeedTest
    "Test possible speed differences between BaseProperties and setState approaches"
    model TestSpeedTestMedium_BaseProperties
      replaceable package Medium = ExternalMedia.Media.TestMedium 
        constrainedby ExternalMedia.Interfaces.PartialExternalTwoPhaseMedium
        "Medium package";

      parameter Integer n=100 "Number of base property instances";

      Medium.BaseProperties[n] medium(
        each final basePropertiesInputChoice=ExternalMedia.Common.InputChoices.pT)
        "Medium base properties";

      Medium.AbsolutePressure p "Pressure";
      Medium.Temperature T "Temperature";

      annotation (experiment(StopTime=1000));
    equation
      p = 1e5;
      T = 300.0 + 20.0*sin(time);

      for i in 1:n loop
        medium[i].p = p;
        medium[i].T = T + (i-1)/100.0;
      end for;
    end TestSpeedTestMedium_BaseProperties;

    model TestSpeedTestMedium_setState
      replaceable package Medium = ExternalMedia.Media.TestMedium 
        constrainedby ExternalMedia.Interfaces.PartialExternalTwoPhaseMedium
        "Medium package";

      parameter Integer n=100 "Number of state record instances";

      Medium.ThermodynamicState[n] state "Thermodynamic state record";
      Medium.SaturationProperties[n] sat "Saturation property record";

      Medium.AbsolutePressure p "Pressure";
      Medium.Temperature T "Temperature";

      annotation (experiment(StopTime=1000));
    equation
      p = 1e5;
      T = 300.0 + 20.0*sin(time);

      for i in 1:n loop
        state[i] = Medium.setState_pT(p, T + (i-1)/100.0);
        sat[i] = Medium.setSat_T(state[i].T);
      end for;
    end TestSpeedTestMedium_setState;
  end SpeedTest;

  package BasicTestMixture
    model Test_setState
      replaceable package Medium=Media.ExternalTwoPhaseMixture;

      //inputs
      SI.AbsolutePressure p = 1e5 "pressure";
      SI.Temperature T "temperature";
      Medium.FixedPhase phase = 0
        "2 for two-phase, 1 for one-phase, 0 if not known";
      parameter Integer nComp = 2;
      Real X[nComp];// = {0.5,0.5};
      Integer uniqueID = 0 "unique ID number";

      //Testing
      SI.Temperature Tb;
      SI.Temperature Td;
      SI.MolarHeatCapacity cp;
      SI.MolarHeatCapacity cv;
      Common.MolarDensity rho;
      Common.MolarDensity rho_phx;
      SI.Pressure p_fun;
      Common.MolarEnthalpy h;
      SI.MolarInternalEnergy u;
      SI.MolarInternalEnergy u_fun;
      SI.MolarEntropy s;
      SI.Temperature t;
      SI.Temperature t_phx;
      SI.Temperature t_bubble;
      SI.Temperature t_dew;
      SI.DynamicViscosity eta;
      SI.ThermalConductivity lambda;
      SI.MolarMass MM;

      //outputs
      Medium.ThermodynamicState state_pT;
      Real x_pT[nComp];
      Real y_pT[nComp];
      Medium.ThermodynamicState state;
      Real x[nComp];
      Real y[nComp];

    equation
      T = 320;//+time*5;
      X[1] = time/50;
      X[2] = 1 - X[1];
      (state_pT,x_pT,y_pT) = Medium.setState_pTx(
        p,
        T,
        phase,
        X,
        0);
      (state,x,y) = Medium.setState_psx(
        p,
        state_pT.s,
        phase,
        X,
        0);
      Tb = Medium.bubbleTemperature(p, X);
      Td = Medium.dewTemperature(p, X);
      cp = ExternalMedia.Media.ExternalTwoPhaseMixture.specificHeatCapacityCp(
        state_pT, X);
      cv = ExternalMedia.Media.ExternalTwoPhaseMixture.specificHeatCapacityCv(
        state_pT, X);
      rho = ExternalMedia.Media.ExternalTwoPhaseMixture.molarDensity(state_pT, X);
      rho_phx = ExternalMedia.Media.ExternalTwoPhaseMixture.molarDensity_phx(
        p,
        state_pT.h,
        X);
      p_fun = ExternalMedia.Media.ExternalTwoPhaseMixture.pressure(state_pT, X);
      h = ExternalMedia.Media.ExternalTwoPhaseMixture.specificEnthalpy(state_pT, X);
      u = h - p/rho;
      u_fun = ExternalMedia.Media.ExternalTwoPhaseMixture.specificInternalEnergy(state_pT, X);
      s = ExternalMedia.Media.ExternalTwoPhaseMixture.specificEntropy(state_pT, X);
      t = ExternalMedia.Media.ExternalTwoPhaseMixture.temperature(state_pT, X);
      t_bubble = ExternalMedia.Media.ExternalTwoPhaseMixture.bubbleTemperature(p, X);
      t_dew = ExternalMedia.Media.ExternalTwoPhaseMixture.dewTemperature(p, X);
      t_phx = ExternalMedia.Media.ExternalTwoPhaseMixture.temperature_phx(
        p,
        state_pT.h,
        X);
      eta = ExternalMedia.Media.ExternalTwoPhaseMixture.dynamicViscosity(state_pT,
        X);
      lambda = ExternalMedia.Media.ExternalTwoPhaseMixture.thermalConductivity(
        state_pT, X);
      MM = ExternalMedia.Media.ExternalTwoPhaseMixture.getMolarMass(X);
      annotation (experiment(StopTime=50), experimentSetupOutput);
    end Test_setState;

    model TestDynamic "Test case using TestMedium and dynamic equations"
      replaceable package Medium = Media.TestMixture;
      parameter SI.Volume V = 1 "Storage Volume";
      parameter Real p_atm = 101325 "Atmospheric pressure";
      parameter SI.Temperature Tstart = 300;
      parameter Real Kv0 = 1.00801e-2 "Valve flow coefficient";
      SI.AmountOfSubstance M;
      SI.Energy U;
      SI.MolarFlowRate win(start = 100);
      SI.MolarFlowRate wout;
      Common.MolarEnthalpy hin;
      Common.MolarEnthalpy hout;
      SI.Power Q;
      Real Kv;
      // Medium Properties
      Medium.ThermodynamicState state;
      SI.MoleFraction X[Medium.nComp] = {0.5,0.5};
      SI.MoleFraction x[Medium.nComp];
      SI.MoleFraction y[Medium.nComp];
      Common.MolarDensity d;
      Common.MolarEnthalpy h(start = 4000);
      SI.MolarInternalEnergy u;

      //Pressure
      SI.Pressure p(start=1e5);
    equation
      // get media data
      (state,x,y) = Medium.setState_phx(p,h,0,X,0);
      d = state.d;//Medium.molarDensity_ph(state.p,state.h,X);
      u = state.h - state.p/d;

      // Mass & energy balance equation
      M = d*V;
      U = u*M;
      der(M) = win - wout;
      der(U) = win*hin - wout*hout + Q;

      // Inlet pump equations
      state.p - p_atm = 2e5 - (1e5/100^2)*win^2;
      hin = 1e5;

      // Outlet valve equation
      wout = Kv * sqrt(d*(state.p - p_atm));
      hout = state.h;

      // Input variables
      Kv = if time<50 then Kv0 else Kv0*1.1;
      Q = if time < 1 then 0 else 1e7;
    initial equation
      // Initial conditions

      p = 1.2e5;
      state.h = 1e5;
      annotation (experiment(StopTime=80, Tolerance=1e-007),experimentSetupOutput(
            equdistant=false));
    end TestDynamic;
  end BasicTestMixture;
end Test;
