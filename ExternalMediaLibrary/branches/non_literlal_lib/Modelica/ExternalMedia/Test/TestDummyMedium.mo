model TestDummyMedium 
  package Medium = ExternalMedia.Media.DummyExternalMedium;
  Medium.BaseProperties medium;
equation 
  medium.p = 1e6;
  medium.h = 350;
end TestDummyMedium;
