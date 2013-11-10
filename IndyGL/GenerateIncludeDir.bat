rmdir "%cd%\include"	/s /q
xcopy "%cd%\src\*.h"	"%cd%\include\IndyGL" /i /s /y
xcopy "%cd%\src\*.inl"	"%cd%\include\IndyGL" /i /s /y
xcopy "%cd%\src\*.hpp"	"%cd%\include\IndyGL" /i /s /y