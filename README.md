# UE5-PuyoPuyo
This repo is PuyoPuyo(popular Japanese game) in UE5 c++ project

# Requirement
- UE5.0.3
- HighSpec PC(that UnrealEngine can run on)

# Get Started
1.Download this repo 
2.launch uproject file

# Features
**Title Screen**
![RxSample - Unreal Editor 2022_10_05 17_25_09](https://user-images.githubusercontent.com/65750938/194017048-a1401965-a4c2-4bf5-9c3b-8ea44ffc960d.png)


**Playing Screen**
![RxSample - Unreal Editor 2022_10_05 17_26_25](https://user-images.githubusercontent.com/65750938/194017100-31061439-79ea-4b38-bae1-1ed7f2b5a182.png)

# How to Play
NumPad 4 -> move left
NumPad 6 -> move right
NumPad 8 -> rotate
NumPad 2 -> move down

# C++ Code
### Folder
Source -> RxSample -> PuyoPuyo

### Main code
- PuyoGamemodeBase.cpp/.h -> set player controller, Stagepawn, etc
- PuyoPlayerController.cpp/.h -> control puyo (it's helpful to read this project first)
- PuyoPlayState.cpp/.h -> have player state and player score
- PuyoHUD.cpp/.h -> draw Widget, score, text, etc
- StagePawn.cpp/.h -> have stage data and show puyo
- PuyoMesh.cpp/.h -> have method puyo creating
- PuyoConfigActor.cpp/.h -> config
- TitleWidget.cpp/.h -> base title widget
- ScoreWidget.cpp/.h -> base score widget

### Class Diagram
![UE5Puyo_Class_image](https://user-images.githubusercontent.com/65750938/194014953-fcd6c7a3-a481-433f-9e8f-848160088be4.jpg)


# reference
PuyoPuyo(reference)
http://puyo.sega.jp/program_2020/dl/puyo-programming-code.pdf

