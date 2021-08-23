:start
WAITCALL  LS=Label0 
GOTO start
:Label0
PU 
PLAY   
BRACATEQU 1-4,7 Label3 
:loopFriends
QUESTION   L1=Label4 L2=Label5 L3=Label6 L4=Label7 L5=Label8 L6=Label12 
:end
PD 
GOTO start
:Label4
PLAY   
GOTO loopFriends
:Label5
PLAY   
GOTO loopFriends
:Label6
PLAY   
GOTO loopFriends
:Label7
PLAY   
GOTO loopFriends
:Label8
ENTERCODE  0321  LC=setup 
GOTO loopFriends
:setup
QUESTION   L1=Label9 L2=Label10 L3=Label11 L9=Label14 L0=loopFriends 
GOTO end
:Label9
ANSON 
GOTO setup
:Label10
ANSOFF 
GOTO setup
:Label11
PLAYNEW 
GOTO setup
:Label14
QUESTION   L1=Label13 
GOTO setup
:Label13
ERASEDYNAMIC 
GOTO setup
:Label12
RECMSG 
GOTO loopFriends
:Label3
PLAY   
GOTO loopFriends
