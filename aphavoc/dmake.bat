@echo off
SET EECH_ROOTDIR=c:\eech-dev

del project.exe

echo ################### Make Timings ####################### >> maketime.txt
time < dmake.bat >> maketime.txt

wmake debug=1

time < dmake.bat >> maketime.txt
echo ######################################################## >> maketime.txt
   
type maketime.txt 

del maketime.txt

