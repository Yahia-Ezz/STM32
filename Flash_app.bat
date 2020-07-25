set PATH=C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\Utilities\STLINKV2\ST-LINK_Utility;%PATH%
call ST-LINK_CLI.exe -c -ME -P 02_Install\STM_App.bin 0x08000000 -Rst -Run 0x08000000