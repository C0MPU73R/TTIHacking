// This is a C++ file version of running the gamedata using pyimport frozen files
// Compile with Visual C++ 2008 Express
// add panda and python LIBS

#define DEBUGLIB(x) x ".lib"
#pragma comment(lib, DEBUGLIB("unicodedata"))
#pragma comment(lib, DEBUGLIB("libp3dtool"))
#pragma comment(lib, DEBUGLIB("libp3dtoolconfig"))
#pragma comment(lib, DEBUGLIB("libpandaexpress"))
#pragma comment(lib, DEBUGLIB("libpanda"))
#pragma comment(lib, DEBUGLIB("libpandaegg"))
#pragma comment(lib, DEBUGLIB("libpandagl"))
#pragma comment(lib, DEBUGLIB("libpandaphysics"))
#pragma comment(lib, DEBUGLIB("libpandafx"))
#pragma comment(lib, DEBUGLIB("libp3direct"))
#pragma comment(lib, DEBUGLIB("libp3vision"))
#pragma comment(lib, DEBUGLIB("libp3windisplay"))
#pragma comment(lib, "C:\\p3dparty\\p3d32\\panda3d-1.9.0\\thirdparty\\win-libs-vc10\\nvidiacg\\lib\\cg.lib")
#pragma comment(lib, "C:\\p3dparty\\p3d32\\panda3d-1.9.0\\thirdparty\\win-libs-vc10\\nvidiacg\\lib\\cgGL.lib")
#pragma comment(lib, "C:\\p3dparty\\p3d32\\panda3d-1.9.0\\thirdparty\\win-libs-vc10\\openal\\lib\\OpenAL32.lib")
#pragma comment(lib, DEBUGLIB("_socket"))
#pragma comment(lib, DEBUGLIB("libpandaode"))

#include "Python.h"
#include "marshal.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <Python.h>
#include <marshal.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "reader.hpp"
//void init_libpandagl();
//void init_libOpenALAudio();

int main(int argc, char* argv[])
{

	Py_DontWriteBytecodeFlag = 1;
	Py_NoSiteFlag = 1;
	//Py_VerboseFlag = 1;
	std::cout << "Reading GameData.pyd" << std::endl;
	std::fstream gameBlob;
	gameBlob.open("GameData.pyd", std::ios_base::in | std::ios_base::binary);

	if(!gameBlob.is_open())
	{
		std::cout << "gamedata isnt even in the same dir XD" << std::endl;
		exit(1);
	}

	gameBlob.seekg(0, std::ios_base::end);
	size_t size = gameBlob.tellg();
	gameBlob.seekg(0, std::ios_base::beg);
	char *data = new char[size];
	gameBlob.read(data, size);
	gameBlob.close();
	unsigned char iv[16];
	memset(iv, 0, 16);

  
	Reader rdr(data, size);
	
	std::cout << "Freezing modules" << std::endl;
	std::vector<_frozen> modules;
	while(rdr.Tell() < size)
	{
		//std::cout << "Seeking past IV" << std::endl;
		rdr.Seek(rdr.Tell()+16);
		//std::cout << "Reading name" << std::endl;
		char* name = rdr.ReadCString();
		if(name[0] == 0)
		{
			break;
		}
		//std::cout << "Name is: " << name << std::endl;
		int codeSize = rdr.ReadUint32();
		//std::cout << "Reading code size" << std::endl;
		bool package = false;
		if(codeSize < 0)
		{
			package = true;
			codeSize *= -1;
		}
		char* code = new char[codeSize];
		memcpy(code, rdr.GetBuf(), codeSize);
		//std::cout << "Reading code" << std::endl;
		rdr.Seek(rdr.Tell()+codeSize);
		//std::cout << "Putting module into vector" << std::endl;
		unsigned int mn = modules.size();
		modules.resize(modules.size()+1);
		modules[mn].name = name;
		modules[mn].code = (unsigned char*)code;
		modules[mn].size = codeSize*(package ? -1 : 1);
		if(rdr.Tell()%16)
		{
			rdr.Seek(rdr.Tell()+(16-(rdr.Tell()%16)));
		}
	}
	std::cout << "Copying vector to array of _frozen" << std::endl;
	_frozen *fzns = new _frozen[modules.size()+1];
	for(unsigned int i = 0; i < modules.size(); ++i)
	{
		memcpy(&fzns[i], &modules[i], sizeof(_frozen));
	}
	memset(&fzns[modules.size()], 0, sizeof(_frozen));
	//set the frozen modules so python knows the modules
	PyImport_FrozenModules = fzns;
	std::cout << "Creating GameData instance 20260821" << std::endl;
	Py_Initialize();
	initunicodedata();
	init_socket();
	initlibp3dtoolconfig();
	PyImport_ImportFrozenModule("__config__");
	initlibpandaexpress();
	initlibpanda();
	initlibpandaegg();
	init_libpandagl();
	initlibpandaphysics();
	initlibpandafx();
	initlibp3direct();
	initlibp3vision();
	initlibpandaode();
	std::cout << "Loading the frozen modules" << std::endl;
	PyImport_ImportFrozenModule("__main__");
	PyErr_Print();
	return 0;
}

extern "C"
{
	void initlibp3dtoolconfig();
	void initunicodedata();
	void initlibpandaexpress();
	void initlibpanda();
	void initlibpandaegg();
	void initlibpandaphysics();
	void initlibpandafx();
	void initlibp3direct();
	void initlibp3vision();
	void initlibp3skel();
	void init_socket();
	void initlibpandaode();
}