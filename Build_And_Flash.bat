call build_all.bat
call Utilities\STLINKV2\ST-LINK_Utility\ST-LINK_CLI.exe -c -ME -P 02_Install\STM_App.bin 0x08000000 -Rst -Run 0x08000000


