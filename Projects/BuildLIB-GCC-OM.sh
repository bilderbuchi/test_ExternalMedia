#!/bin/bash

GCC_OPTS="-O2 -loleaut32 -DCOOLPROP=1"
CP=../externals/coolprop/trunk
CPinc=${CP}/CoolProp
INCLUDES="-I${CPinc} -I${CP}/wrappers/Modelica/src"

echo "Compiling sources"

#  ****** compile all the sources from CoolProp for OpenModelica ************
gcc ${GCC_OPTS} -c ${INCLUDES} ${CP}/wrappers/Modelica/src/coolpropsolver.cpp
gcc ${GCC_OPTS} -c ${INCLUDES} Sources/*.cpp

#  ********** CoolProp sources *********
gcc ${GCC_OPTS} -c ${INCLUDES} ${CP}/CoolProp/*.cpp 

ar -r libExternalMediaLib.a *.o
ranlib libExternalMediaLib.a

echo "Copying files to External Media 3.2 and 3.2.1"
cp libExternalMediaLib.a "../Modelica/ExternalMedia 3.2/Resources/Library"
cp Sources/externalmedialib.h "../Modelica/ExternalMedia 3.2/Resources/Include"
cp libExternalMediaLib.a "../Modelica/ExternalMedia 3.2.1/Resources/Library"
cp Sources/externalmedialib.h "../Modelica/ExternalMedia 3.2.1/Resources/Include"
cp Sources/externalmedialib.h "$OPENMODELICAHOME/include/omc/c"
cp libExternalMediaLib.a "$OPENMODELICAHOME/lib/omc"
echo "All done"