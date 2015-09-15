//THIS is a c++ version of running the gamedata using pyimport frozen files.....
//XD
//cumpile (XD) with visual c++ 2008 express
// be sure to add panda and python LIBS

//import python stuff
#include <Python.h>
#include <marshal.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>


//weird thing just to read the gamedata XD
class Reader
{
	char* m_buf;
	unsigned int m_buflen;
	unsigned int m_pos;
	public:
		Reader(char* buf, unsigned int buflen) : m_buf(buf),
			m_buflen(buflen), m_pos(0)
		{
		}

		char* ReadCString()
		{
			std::string out;
			while(m_pos < m_buflen && m_buf[m_pos])
			{
				out += m_buf[m_pos];
				m_pos++;
			}
			m_pos++;
			char* outc = new char[out.size()+1];
			memcpy(outc, out.c_str(), out.size()+1);
			return outc;
		}

		unsigned int ReadUint32()
		{
			m_pos+=4;
			return *(unsigned int*)(m_buf+m_pos-4);
		}

		unsigned int Tell()
		{
			return m_pos;
		}

		void Seek(unsigned int pos)
		{
			m_pos = pos;
		}

		char* GetBuf()
		{
			return m_buf+m_pos;
		}
};

//main
int main(int argc, char* argv[])
{
  //useless py stuff XD
	Py_DontWriteBytecodeFlag = 1;
	Py_NoSiteFlag = 1;
	//Py_VerboseFlag = 1;
	
	// read the gamedata finally after all that useless code XD
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

  //use the useless reader to read some useless stuff XD
	Reader rdr(data, size);
	
	//freeze them damn this is uesless XD
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

  //MORE boring useless crap
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
	// this command is obvious i dont need this here XD
	Py_Initialize();
	initunicodedata();
	init_socket();
	initlibp3dtoolconfig();
	//WHY DO YOU NEED TO IMPORT __CONFIG that isnt even a file packed into gamedata
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
	//import main ANOTHER file that isnt even packed in *facepalm*
	PyImport_ImportFrozenModule("__main__");
	//print an error which isnt there bc its awesome code XD
	PyErr_Print();
	return 0;
}
