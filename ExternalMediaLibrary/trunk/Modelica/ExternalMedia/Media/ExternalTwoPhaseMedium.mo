package ExternalTwoPhaseMedium 
  "External two phase medium package - modify libraryName to set the external library" 
  extends ExternalMedia.Interfaces.PartialExternalTwoPhaseMedium;
  redeclare replaceable function extends setState_ph 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_ph_(p, h, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T,
                            mediumName, libraryName, substanceName);
  end setState_ph;
  
  redeclare replaceable function extends setState_pT 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_pT_(p, T, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T,
                            mediumName, libraryName, substanceName);
  end setState_pT;
  
  redeclare replaceable function extends setState_dT 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_dT_(d, T, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T,
                            mediumName, libraryName, substanceName);
  end setState_dT;
  
  redeclare replaceable function extends setState_ps 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_ps_(p, s, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T,
                            mediumName, libraryName, substanceName);
  end setState_ps;
  
  redeclare replaceable function extends setSat_p 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setSat_p_(p, uniqueID, sat.psat, sat.Tsat, sat.uniqueID,
                            mediumName, libraryName, substanceName);
  end setSat_p;
  
  redeclare replaceable function extends setSat_T 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setSat_T_(T, uniqueID, sat.psat, sat.Tsat, sat.uniqueID,
                            mediumName, libraryName, substanceName);
  end setSat_T;
  
  redeclare replaceable function extends setSat_p_state 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setSat_p_state_(state.uniqueID, sat.psat, sat.Tsat, sat.uniqueID);
  end setSat_p_state;
  
  redeclare replaceable function extends setDewState 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setDewState_(sat.uniqueID, phase, state.uniqueID, state.phase,
                            mediumName, libraryName, substanceName);
  end setDewState;
  
  redeclare replaceable function extends setBubbleState 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setBubbleState_(sat.uniqueID, phase, state.uniqueID, state.phase,
                               mediumName, libraryName, substanceName);
  end setBubbleState;
  
  redeclare replaceable function extends saturationPressure 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" p=  saturationPressure_(T, mediumName, libraryName, substanceName);
  end saturationPressure;
  
  redeclare replaceable function extends saturationTemperature 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" T = 
                  saturationTemperature_(p, mediumName, libraryName, substanceName);
  end saturationTemperature;
  
  redeclare replaceable function extends saturationTemperature_derp 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dTp=  saturationTemperature_derp_(p, mediumName, libraryName, substanceName);
  end saturationTemperature_derp;
  
  redeclare replaceable function saturationTemperature_derp_sat 
    "Returns derivative of saturation temperature w.r.t.. pressure" 
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation property record";
    output Real dTp "derivative of saturation temperature w.r.t. pressure";
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dTp=  saturationTemperature_derp_sat_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName);
  end saturationTemperature_derp_sat;
  
  redeclare replaceable function extends density 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" d=  density_(state.uniqueID);
  end density;
  
  redeclare replaceable function extends pressure 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" p=  pressure_(state.uniqueID);
  end pressure;
  
  redeclare replaceable function extends specificEnthalpy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" h=  specificEnthalpy_(state.uniqueID);
  end specificEnthalpy;
  
  redeclare replaceable function extends specificEntropy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" s=  specificEntropy_(state.uniqueID);
  end specificEntropy;
  
  redeclare replaceable function extends temperature 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" T=  temperature_(state.uniqueID);
  end temperature;
  
  redeclare replaceable function extends isentropicEnthalpy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" h_is=  isentropicEnthalpy_(p_downstream, refState.uniqueID);
  end isentropicEnthalpy;
  
  redeclare replaceable function extends isobaricExpansionCoefficient 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" beta=  isobaricExpansionCoefficient_(state.uniqueID);
  end isobaricExpansionCoefficient;
  
  redeclare replaceable function extends isothermalCompressibility 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" kappa=  isothermalCompressibility_(state.uniqueID);
  end isothermalCompressibility;
  
  redeclare replaceable function extends specificHeatCapacityCp 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" cp=  specificHeatCapacityCp_(state.uniqueID);
  end specificHeatCapacityCp;
  
  redeclare replaceable function extends specificHeatCapacityCv 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" cv=  specificHeatCapacityCv_(state.uniqueID);
  end specificHeatCapacityCv;
  
  redeclare replaceable function extends density_derp_h 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
    external "C" ddph=  density_derp_h_(state.uniqueID);
  end density_derp_h;
  
  redeclare replaceable function extends density_derh_p 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
    external "C" ddhp=  density_derh_p_(state.uniqueID);
  end density_derh_p;
  
  redeclare replaceable function extends bubbleDensity 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dl=  bubbleDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName);
  end bubbleDensity;
  
  redeclare replaceable function extends dewDensity 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dv=  dewDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName);
  end dewDensity;
  
  redeclare replaceable function extends bubbleEnthalpy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" hl=  bubbleEnthalpy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName);
  end bubbleEnthalpy;
  
  redeclare replaceable function extends dewEnthalpy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" hv=  dewEnthalpy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName);
  end dewEnthalpy;
  
  redeclare replaceable function extends bubbleEntropy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" sl=  bubbleEntropy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName);
  end bubbleEntropy;
  
  redeclare replaceable function extends dewEntropy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" sv=  dewDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName);
  end dewEntropy;
  
  redeclare replaceable function extends dBubbleDensity_dPressure 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" ddldp=  dBubbleDensity_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName);
  end dBubbleDensity_dPressure;
  
  redeclare replaceable function extends dDewDensity_dPressure 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" ddvdp=  dDewDensity_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName);
  end dDewDensity_dPressure;
  
  redeclare replaceable function extends dBubbleEnthalpy_dPressure 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dhldp=  dBubbleEnthalpy_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName);
  end dBubbleEnthalpy_dPressure;
  
  redeclare replaceable function extends dDewEnthalpy_dPressure 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dhvdp=  dDewEnthalpy_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName);
  end dDewEnthalpy_dPressure;
  
  redeclare replaceable function extends density_ph_der 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" d_der=  density_ph_der_(uniqueID, p_der, h_der);
  end density_ph_der;
  
  redeclare replaceable function extends temperature_ph_der 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" T_der=  temperature_ph_der_(uniqueID, p_der, h_der);
  end temperature_ph_der;
  
  redeclare replaceable function extends createMedium 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" uniqueID=  createMedium_(mediumName, libraryName, substanceName, oldUniqueID);
  end createMedium;
  
  redeclare replaceable function extends getMolarMass 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" MM=  getMolarMass_(mediumName, libraryName, substanceName);
  end getMolarMass;
  
  redeclare replaceable function extends getCriticalTemperature 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" Tc=  getCriticalTemperature_(mediumName, libraryName, substanceName);
  end getCriticalTemperature;
  
  redeclare replaceable function extends getCriticalPressure 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" pc=  getCriticalPressure_(mediumName, libraryName, substanceName);
  end getCriticalPressure;
  
  redeclare replaceable function extends getCriticalMolarVolume 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" vc=  getCriticalMolarVolume_(mediumName, libraryName, substanceName);
  end getCriticalMolarVolume;
  
  redeclare replaceable function extends thermalConductivity 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" lambda=  thermalConductivity_(state.uniqueID);
  end thermalConductivity;
  
  redeclare replaceable function extends dynamicViscosity 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" eta=  dynamicViscosity_(state.uniqueID);
  end dynamicViscosity;
  
  redeclare replaceable function extends prandtlNumber 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" Pr=  prandtlNumber_(state.uniqueID);
  end prandtlNumber;
  
  redeclare replaceable function extends surfaceTension 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" sigma=  surfaceTension_(sat.psat, sat.Tsat, sat.uniqueID);
  end surfaceTension;
  
end ExternalTwoPhaseMedium;
