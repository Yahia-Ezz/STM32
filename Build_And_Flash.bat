@echo off 

set PATH=C:\Program Files\Git\cmd; C:\Program Files\Git\bin;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility;%PATH%

if "%~1"==""  goto :MissingArgument
IF /I %1 == Sender goto :Sender
IF /I %1 == Receiver ( goto :Receiver ) ELSE (  goto :MissingArgument )


:Sender

echo Sender Script Initiaited!
CALL Build_app.bat SENDER
CALL Flash_app.bat 1
exit /b
 
:Receiver

echo Receiver Script Initiaited!
CALL Build_app.bat RECEIVER
CALL Flash_app.bat 0

exit /b

:MissingArgument

echo Missing Argument!
echo Please choose if it's either "Sender" or "Receiver"
exit /b


