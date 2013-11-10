rmdir "%cd%\lib\IndyCore\include"											/s /q
xcopy "%cd%\..\IndyCore\include"		"%cd%\lib\IndyCore\include\"		/y /s
xcopy "%cd%\..\IndyCore\lib\Release"	"%cd%\lib\IndyCore\lib\Release\"	/y /s
xcopy "%cd%\..\IndyCore\lib\Debug"		"%cd%\lib\IndyCore\lib\Debug\"		/y /s