this widget reads a xml file and load it into a qtreewidget. there are 2 supported properties in xml document which handle tree behavior:
1- (icon="some path") which loads the icon for that row
2- (treeState="hidden,disable,selectable,editable,checkable") which set right property (properties) for that row

you have to implement function "CreateNodeData" which return an object of type "NodeData". NodeData is an interface (abstruct structure). you have to inherent from NodeData and create yourown structures based on node type. different fileds of the structures are available in the second argument of function "CreateNodeData" and you have to parse these fields in your structures' constructors.
to see an example you can check types.h and types.cpp
