within ExternalMedia.Media;
package TestMixture
  extends ExternalTwoPhaseMixture(
    mediumName = "CB",
    libraryName = "FluidProp.RefProp",
    nComp = 2,
    substanceName = "pentane-hexane");
end TestMixture;
