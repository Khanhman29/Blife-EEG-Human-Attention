For the first time you install project:



# Installation
Install Python3.x and run command: 
```bash
pip install requests
```
Install [Visual Studio Code Community 2019](https://visualstudio.microsoft.com/vs/older-downloads/)


Install [Qt 5.12.x Offline](https://www.qt.io/offline-installers)

# Set up project
Install Extension: "Qt VS tools":

### Step1: Open VSCode Community 2019, Extensions -> Manage Extensions ->Search "Qt VS tools" and install.

### Step2: Reopen VSCode Community 2019, Extensions -> Qt VS tools -> Options.

	Setting Options:
	
	Qt ->General: set Qt/MSBuild: 
	
		for example"C:\Users\Admin\AppData\Local\QtMsBuild"
		
	Qt ->Versions: add new Qt 5.12.12.msvc2017_64 and set path to qmake.exe
	
		for example"G:\ProgramFiles\Qt\5.12.12\msvc2017_64\bin\qmake.exe"
		
### Step3: Reopen VSCode Community 2019, Extensions -> Qt VS tools -> Qt Project settings

	Build Events -> Post-Build Event
	
	set Command Line :
	
		xcopy /d/y "$(ProjectDir)tobii_stream_engine\lib\tobii\*.dll" "$(TargetDir)"
		
		xcopy /d/y "$(ProjectDir)MQTT\lib\*.dll" "$(TargetDir)"
