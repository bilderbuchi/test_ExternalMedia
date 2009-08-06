echo Setting the path to the VS compiler
call "%VS80COMNTOOLS%\..\..\VC\bin\vcvars32.bat"

cl /O2 /GL /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_CRT_SECURE_NO_DEPRECATE" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /FD /EHsc /MD  /W3 /nologo /c /Wp64 /Zi /TP /errorReport:prompt *.cpp

link /OUT:"ExternalMediaLib.dll" /INCREMENTAL:NO /NOLOGO /DLL /MANIFEST /MANIFESTFILE:"ExternalMediaLib.dll.intermediate.manifest" /DEBUG /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /MACHINE:X86 /ERRORREPORT:PROMPT kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib *.obj