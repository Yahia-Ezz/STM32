IF /I %1 == Sender (set CFlag=SENDER) ELSE (set CFlag=RECEIVER)

make all -s MYARG=%CFlag%
@REM make dumpcls
