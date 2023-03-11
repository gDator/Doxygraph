DESCRIPTION
===========================================
This Project is like Sourcetrail. A view of code structure.
Under the hood doxygraph uses - as the name already tells - doxygen to generate the Tree.
NOTE: This is not officialy supported from doxygen. 
![Screenshot of Doxygraph viewing its own code](images/Screenshot.JPG)

HOW TO USE
===========================================
1. Build the project
2. Open the Program
3. Go to File>Include>Folder Structure
4. Choose Folder where your project of interest is
5. View>Console prints all the parsing of the Project
6. After finsihing got View>Settings and choose which nodes you want to display
7. Click a node and watch the callgraph; the call depth can be edited in View>Settings
8. Have a look at the Code of the choosen node
9. Also take a look at View>Information (further extraxted information will be added in the future
10. Scroll to zoom
11. Press the scrollwheel and move the mouse to move the codemap

INFORMATION
===========================================
- a lot of features are not yet implemented since this is hobby project
- the code is a bit messy - sry this is one of my first projects and im still learning
- if you have suggutions of implementation you can message me
- using C++17 standard
- its very simple code and only 2k(?) lines of code of interest (tree.cpp, backend.cpp)

TODO - PLANS
===========================================
- Add Simulation tool to watch the call flow
- Add a Code generator for projects from "ULM-Code"
- path finding arrow/references
- project save/load
- export of CodeMap
- Add more doxy-comments

ARCHITECTURE
===========================================
1. Doxygraph (doxygraph_gui.cpp) ist just the whole gui. The Filepicker (https://github.com/Bousk/ImGui_Explorer) and the Texteditor(https://github.com/BalazsJako/ColorTextEditorDemo) is from github.
2. backend.cpp has two classes: CppInterpreter and FileReader. FileReader is pretty much useless and is an artifact of an older version - it will be removed in a later version?
CppInterprter does the same as Doxygen in the run() function - which runs in an extra thread. It also transforms the Doxygen Lists into a code tree (tree.cpp), which stores all the information and the graphics.
3. tree.cpp is a tree with nodes (class Node), with relations of parents and childs. The root node is in the CodeMap class. Also there are References between nodes, which are not a child-parent relation but a caller-call relation or somthing similiar.
The References are beeing extracted after the whole code tree is built.

DEPENDECIES
===========================================
Following dependcies:
	- doxygen
	- imgui
	- SFML
	- boost/circular_buffer
	
HOW TO BUILD
===========================================
- this project has a premake5.lua file. so go ahead and build it with your build system of choice
- the sfml.dlls needs to be in the same folder as the .exe
- the res folder needs to be in the same folder as the .exe


LICENSE
===========================================
This projects uses MIT LICENSE.