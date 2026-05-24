# PSD-Final-Project
CSE3PSD/CSE5008 Programming Assignment 3A - Zork-Style Adventure


cd build
cmake ..
cmake --build .
Debug\PSD_Final_Project.exe

rmdir /s /q build
mkdir build
cd build
cmake ..
cmake --build .

{
    "triggerType": "ENTER_ROOM",
    "target": "castlecenter-hall",
    "flag": "entered_castlecenter_hall",
    "result": "You entered the throne hall.",
    "flagDone": "talked_king",    
    "lockPlayer": true
}
flagDone dan lockPlayer hanya dipakai kalau butuh locking player after trigger.
flagDone = trigger flag yang mana supaya lock player di-set false

penamaan:
- character-> full lowercase
- item -> full lowercase
- passage -> passage_{room1}_{room2}
- room -> lowercase, pake "-", cth: bedroom-door2

untuk item, nama passage menjadi:
- passage_{room1}_to_{room2}