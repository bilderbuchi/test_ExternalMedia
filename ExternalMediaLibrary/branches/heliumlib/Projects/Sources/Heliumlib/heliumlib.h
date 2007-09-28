/* 
   Header file with external declarations for FORTRAN function calls
*/

extern "C" _declspec(dllimport) double DHE (double *P, double *T);
extern "C" _declspec(dllimport) double HHE (double *P, double *T);
extern "C" _declspec(dllimport) double CPHE (double *P, double *T);
extern "C" _declspec(dllimport) double CVHE (double *P, double *T);
extern "C" _declspec(dllimport) double GRUNHE (double *P, double *T);
extern "C" _declspec(dllimport) double SNDHE (double *P, double *T);
extern "C" _declspec(dllimport) double SHE (double *P, double *T);
