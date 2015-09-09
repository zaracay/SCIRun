/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2015 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

#include <Testing/ModuleTestBase/ModuleTestBase.h>
#include <Modules/Factory/HardCodedModuleFactory.h>
#include <Dataflow/Engine/Controller/NetworkEditorController.h>
#include <sstream>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

using namespace SCIRun;
using namespace Testing;
using namespace Modules::Factory;
using namespace Generator;
using namespace Dataflow::Networks;
using namespace ReplacementImpl;
using namespace Dataflow::Engine;
using namespace Core::Algorithms;


TEST(FactoryGeneratorTests, GenerateSomeJsonFromModuleProperties)
{
  HardCodedModuleFactory factory;

  auto descMap = factory.getDirectModuleDescriptionLookupMap();

  int i = 0;
  for (const auto& mod : descMap)
  {
    ++i;
    std::cout << mod.first << " -> " << mod.second << std::endl;

    // Write json.
    ptree pt;
    pt.put("module.name", mod.first.module_name_);
    pt.put("module.namespace", "<ns>");
    pt.put("module.status", mod.second.moduleStatus_);
    pt.put("module.description", mod.second.moduleInfo_);
    std::ostringstream buf;
    write_json(buf, pt, false);
    std::string json = buf.str(); // {"foo":"bar"}
    std::cout << json << std::endl;



    if (i > 3)
      break;
  }
}

namespace
{
  std::string moduleJson =
    "{\n"
    "\t\"module\": {\n"
    "\t\t\"name\": \"CreateLatVol\",\n"
    "\t\t\"namespace\" : \"Fields\",\n"
    "\t\t\"status\" : \"Ported module\",\n"
    "\t\t\"description\" : \"Creates Lattice Volumes\",\n"
    "\t\t\"header\" : \"Modules/Legacy/Fields/CreateLatVol.h\"\n"
    "\t},\n"
    "\t\"algorithm\": {\n"
    "\t\t\"name\": \"CreateLatVolAlgo\",\n"
    "\t\t\"namespace\" : \"Fields\",\n"
    "\t\t\"header\" : \"Core/Algorithms/Legacy/Fields/CreateLatVolAlgo.h\"\n"
    "\t},\n"
    "\t\"UI\" : {\n"
    "\t\t\"name\": \"CreateLatVolDialog\",\n"
    "\t\t\"header\" : \"Interface/Modules/Fields/CreateLatVolDialog.h\"\n"
    "\t}\n"
    "}\n";
}

TEST(FactoryGeneratorTests, ReadFullModuleJsonDescription)
{
  std::cout << moduleJson << std::endl;

  ptree modProps;
  std::istringstream is(moduleJson);
  read_json(is, modProps);

  EXPECT_EQ("CreateLatVol", modProps.get<std::string>("module.name"));
  EXPECT_EQ("Fields", modProps.get<std::string>("module.namespace"));
  EXPECT_EQ("Ported module", modProps.get<std::string>("module.status"));
  EXPECT_EQ("Creates Lattice Volumes", modProps.get<std::string>("module.description"));
  EXPECT_EQ("Modules/Legacy/Fields/CreateLatVol.h", modProps.get<std::string>("module.header"));

  EXPECT_EQ("CreateLatVolAlgo", modProps.get<std::string>("algorithm.name"));
  EXPECT_EQ("Fields", modProps.get<std::string>("algorithm.namespace"));
  EXPECT_EQ("Core/Algorithms/Legacy/Fields/CreateLatVolAlgo.h", modProps.get<std::string>("algorithm.header"));

  EXPECT_EQ("CreateLatVolDialog", modProps.get<std::string>("UI.name"));
  EXPECT_EQ("Interface/Modules/Fields/CreateLatVolDialog.h", modProps.get<std::string>("UI.header"));

  EXPECT_THROW(modProps.get<std::string>("UI.namespace"), std::exception);
}

TEST(FactoryGeneratorTests, ReadFullModuleJsonDescriptionUsingObject)
{
  std::cout << moduleJson << std::endl;

  ModuleDescriptorJsonParser parser;

  auto desc = parser.readJsonString(moduleJson);
  
  EXPECT_EQ("CreateLatVol", desc.name_);
  EXPECT_EQ("Fields", desc.namespace_);
  EXPECT_EQ("Ported module", desc.status_);
  EXPECT_EQ("Creates Lattice Volumes", desc.description_);
  EXPECT_EQ("Modules/Legacy/Fields/CreateLatVol.h", desc.header_);

  //TODO
  //EXPECT_EQ("CreateLatVolAlgo", modProps.get<std::string>("algorithm.name"));
  //EXPECT_EQ("Fields", modProps.get<std::string>("algorithm.namespace"));
  //EXPECT_EQ("Core/Algorithms/Legacy/Fields/CreateLatVolAlgo.h", modProps.get<std::string>("algorithm.header"));

  //EXPECT_EQ("CreateLatVolDialog", modProps.get<std::string>("UI.name"));
  //EXPECT_EQ("Interface/Modules/Fields/CreateLatVolDialog.h", modProps.get<std::string>("UI.header"));

  //EXPECT_THROW(modProps.get<std::string>("UI.namespace"), std::exception);
}

/*
Steps:
1. CMake flag for "build module list" is turned on
2. CMake gets path to .module descriptor files, and passes it to C++ program
3. C++ program receives path and iterates over all .module files
4. For each json.module, call read code above and construct a map of structures (worry about lookup efficiency later)
5. Loop over map of module desc structures and generate code lines
6. Output of C++ code is ModuleFactory_Generated.cc file with one generated method, to be called by existing C++ factory impl
7. Once that's working, extend to generate Algo and Dialog factory functions as well
8. Then start converting old HardCodedModuleFactory lines to new way. Write some python code for this
*/

TEST(FactoryGeneratorTests, CanReadDirectoryOfDescriptorFiles)
{
  


  FAIL() << "todo";
}

TEST(FactoryGeneratorTests, CanBuildModuleDescriptorFromString)
{



  FAIL() << "todo";
}

TEST(FactoryGeneratorTests, CanBuildModuleDescriptorFromFile)
{



  FAIL() << "todo";
}

TEST(FactoryGeneratorTests, CanGenerateCodeFileFromMap)
{



  FAIL() << "todo";
}
