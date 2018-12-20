# ccode-template-Labview
20-sim C-code generation template for exporting a 20-sim model to a Labview compatible DLL.

This template is provided as-is. It is provided without warranty or support from Controllab.

## Installation
Download the code generation template using the "Download ZIP" button on Github or using a "git clone"

1. Extract the zip file
2. Copy the folder LabviewDLL to a location on your 20-sim PC.
3. Open 20-sim
4. Choose from the Menu: Tools | Options
5. Choose Folders
6. Choose C-Code Folders
7. Add the folder you copied in step 2.
8. To generate a Labview-compatible DLL from 20-sim,choose Tools | Real Time Toolbox | C-Code Generation
    Select the target ""Labview DLL for 20-sim submodel"
" in the list.
9. Select a submodel to export

## External dependencies
1. 20-sim 4.3 or higher is required for this template
2. For automated compilation of the DLL, we currently rely on the availability of the Visual Studio 2008 compiler.
Any version Express, Community, Professional or Enterprise will work.

2. [Microsoft Visual C++ Redistributable Packages for Visual Studio 2008](http://www.microsoft.com/en-us/download/details.aspx?id=40784)