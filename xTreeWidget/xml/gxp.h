#pragma once
#include <map>
#include <vector>

namespace GXP
{
	struct nodeData;
	typedef std::pair<std::string, std::string> Atribute;
	typedef std::vector<Atribute>				AtributesVector;
	typedef std::vector<nodeData*>				NodeChildren;
	struct nodeData
	{
		std::string		type;
		AtributesVector	atributes;
		NodeChildren    children;
		~nodeData()
		{
			for (size_t i = 0; i < children.size(); i++)
				delete children[i];
			children.clear();
		}
	};
	
	class parser
	{
		bool success = false;

	public:
		nodeData* data;
	public:
		parser() {}
		parser(const char* file_name);
		bool paresedSuccessfully()const { return success; }
	};
};
