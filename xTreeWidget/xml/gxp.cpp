#include "gxp.h"
#include "pugi/pugixml.hpp"

using namespace GXP;

bool parse(pugi::xml_object_range<pugi::xml_node_iterator>& itrn, nodeData** parent_ptr, nodeData** data_ptr)
{
	for (pugi::xml_node_iterator it = itrn.begin(); it != itrn.end(); ++it)
	{
		nodeData* nd = NULL;
		if (parent_ptr == NULL)
			nd = *data_ptr = new nodeData;
		else
		{
			nd = new nodeData;
			(*parent_ptr)->children.push_back(nd);
		}

		nd->type = it->name();

		for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ait++)
			nd->atributes.push_back(Atribute(ait->name(), ait->value()));

		pugi::xml_object_range<pugi::xml_node_iterator> children = it->children();
		if (children.begin() != children.end())
			parse(children, &nd,data_ptr);
	}
	return true;
}

GXP::parser::parser(const char* file_name)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file_name);
	if (result.status != pugi::status_ok)
		return;
	pugi::xml_object_range<pugi::xml_node_iterator> itrs = doc.children();
	success = parse(itrs,NULL,&data);
}
