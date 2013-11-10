rmdir "%cd%\include"	/s /q
xcopy "%cd%\src\*.h"	"%cd%\include\IndyCore" /i /s /y
xcopy "%cd%\src\*.inl"	"%cd%\include\IndyCore" /i /s /y
xcopy "%cd%\src\*.hpp"	"%cd%\include\IndyCore" /i /s /y