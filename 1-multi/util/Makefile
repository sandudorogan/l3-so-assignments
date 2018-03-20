OBJ_LIST = tema1.obj list.obj utils.obj hashtable.obj
CFLAGS = /nologo /W4 /EHsc /Za /DWIN

LIBNAMES = hash.lib
DLLNAMES = hash.dll
EXE_NAME = tema1.exe

build : $(EXE_NAME)

tema1.exe : $(OBJ_LIST) $(LIBNAMES)
	link /nologo /out:$@ $**

hash.lib : hash.def
	lib /nologo /def:$** /out:$@ /machine:x86

clean : obj_clean exe_clean lib_clean

obj_clean : 
	del *.obj

exe_clean :
	del $(EXE_NAME)

lib_clean :
	del $(LIBNAMES)