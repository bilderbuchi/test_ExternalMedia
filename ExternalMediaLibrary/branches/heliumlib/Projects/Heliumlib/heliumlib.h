/* 
   Header file with external declarations for FORTRAN function calls
*/

extern "C" _declspec(dllimport) float DHE (float *P, float *T);
extern "C" _declspec(dllimport) float HHE (float *P, float *T);
extern "C" _declspec(dllimport) float CPHE (float *P, float *T);
extern "C" _declspec(dllimport) float CVHE (float *P, float *T);
extern "C" _declspec(dllimport) float GRUNHE (float *P, float *T);
extern "C" _declspec(dllimport) float SNDHE (float *P, float *T);
extern "C" _declspec(dllimport) float SHE (float *P, float *T);
