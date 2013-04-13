package ExternalTwoPhaseMedium
  extends ExternalMedia.Interfaces.PartialExternalTwoPhaseMedium;
  redeclare replaceable function extends setState_ph
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_ph_(p, h, phase, uniqueID, state.uniqueID, state.phase);
  end setState_ph;

  redeclare replaceable function extends setState_pT
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_pT_(p, T, phase, uniqueID, state.uniqueID, state.phase);
  end setState_pT;

  redeclare replaceable function extends setState_dT
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_dT_(d, T, phase, uniqueID, state.uniqueID, state.phase);
  end setState_dT;

  redeclare replaceable function extends setState_ps
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_ps_(p, s, phase, uniqueID, state.uniqueID, state.phase);
  end setState_ps;

  redeclare replaceable function extends setSat_p
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setSat_p_(p, uniqueID, sat.psat, sat.Tsat, sat.uniqueID);
  end setSat_p;

  redeclare replaceable function extends setSat_T
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setSat_T_(T, uniqueID, sat.psat, sat.Tsat, sat.uniqueID);
  end setSat_T;

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

  redeclare replaceable function extends bubbleDensity
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dl=  bubbleDensity_(sat.uniqueID);
  end bubbleDensity;

  redeclare replaceable function extends dewDensity
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dv=  dewDensity_(sat.uniqueID);
  end dewDensity;

  redeclare replaceable function extends bubbleEnthalpy
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" hl=  bubbleEnthalpy_(sat.uniqueID);
  end bubbleEnthalpy;

  redeclare replaceable function extends dewEnthalpy
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" hv=  dewEnthalpy_(sat.uniqueID);
  end dewEnthalpy;

  redeclare replaceable function extends bubbleEntropy
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" sl=  bubbleEntropy_(sat.uniqueID);
  end bubbleEntropy;

  redeclare replaceable function extends dewEntropy
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" sv=  dewDensity_(sat.uniqueID);
  end dewEntropy;

  redeclare replaceable function extends density_ph_der
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" d_der=  density_ph_der_(uniqueID, p_der, h_der);
  end density_ph_der;

  redeclare replaceable function extends createMedium
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" uniqueID=  createMedium_(mediumName, oldUniqueID);
  end createMedium;

  redeclare replaceable function extends deleteMedium
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" deleteMedium_(uniqueID);
  end deleteMedium;

  redeclare replaceable function extends getMolarMass
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" MM=  molarMass_(uniqueID);
  end getMolarMass;

end ExternalTwoPhaseMedium;
