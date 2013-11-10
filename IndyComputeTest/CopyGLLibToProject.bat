rmdir "%cd%\lib\IndyGL\include"										/s /q
xcopy "%cd%\..\IndyGL\include"		"%cd%\lib\IndyGL\include\"		/y /s
xcopy "%cd%\..\IndyGL\lib\Release"	"%cd%\lib\IndyGL\lib\Release\"	/y /s
xcopy "%cd%\..\IndyGL\lib\Debug"	"%cd%\lib\IndyGL\lib\Debug\"	/y /s