/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2015 Scientific Computing and Imaging Institute,
   University of Utah.

   
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

#include <Modules/Legacy/Math/EvaluateLinAlgGeneral.h>
#include <Core/Datatypes/String.h>
#include <Core/Datatypes/Matrix.h>
#include <Core/Algorithms/Base/AlgorithmVariableNames.h>
//#include <Core/Parser/LinAlgEngine.h>

//#include <Core/Util/StringUtil.h>

using namespace SCIRun::Modules::Math;
using namespace SCIRun::Core::Datatypes;
using namespace SCIRun::Core::Algorithms;
using namespace SCIRun::Dataflow::Networks;

const ModuleLookupInfo EvaluateLinearAlgebraGeneral::staticInfo_("EvaluateLinearAlgebraGeneral", "Math", "SCIRun");

#if 0
namespace SCIRun {

/// @class EvaluateLinAlgGeneral
/// @brief This module performs a user defined linear algebra operation on up to five input matrices. 

class EvaluateLinAlgGeneral : public Module
{
public:
  EvaluateLinAlgGeneral(GuiContext* ctx);
  virtual ~EvaluateLinAlgGeneral() {}

  virtual void execute();
  virtual void presave();

private:
  GuiString guifunction_;

};


DECLARE_MAKER(EvaluateLinAlgGeneral)
#endif

EvaluateLinearAlgebraGeneral::EvaluateLinearAlgebraGeneral() : Module(staticInfo_)
    //guifunction_(get_ctx()->subVar("function"), )
{
  INITIALIZE_PORT(i1);
  INITIALIZE_PORT(i2);
  INITIALIZE_PORT(i3);
  INITIALIZE_PORT(i4);
  INITIALIZE_PORT(i5);
  INITIALIZE_PORT(o1);
  INITIALIZE_PORT(o2);
  INITIALIZE_PORT(o3);
  INITIALIZE_PORT(o4);
  INITIALIZE_PORT(o5);
}

void EvaluateLinearAlgebraGeneral::setStateDefaults()
{
  get_state()->setValue(Variables::FunctionString, std::string("o1 = i1 * i2;"));
}

void
EvaluateLinearAlgebraGeneral::execute()
{
  const int NUM_PORTS = 5;
  auto inputPorts = { i1, i2, i3, i4, i5 }; // TODO: call base class function?
  std::vector<MatrixHandle> imatrix(NUM_PORTS);
  std::vector<MatrixHandle> omatrix(NUM_PORTS);
  
  for (const auto& input : inputPorts)
  {
    imatrix.push_back(getOptionalInput(input));
  }
  
  if (needToExecute())
  {
    // Inform module that execution started
    update_state(Executing);
    
    NewLinAlgEngine engine;
    engine.set_progress_reporter(this);
    
    for (size_t p = 0; p < 5; p++)
    {
      std::string matrixname = std::string("o")+to_string(p+1);
      if (oport_connected(matrixname))
      {
        if(!(engine.add_output_matrix(matrixname))) return;
      }
      
      if (imatrix[p])
      {
        std::string matrixname = std::string("i")+to_string(p+1);
        if (!(engine.add_input_matrix(matrixname,imatrix[p]))) return;
      }
    }
    
    std::string function = get_state()->getValue(Variables::FunctionString).toString(),

    if(!engine.add_expressions(function)) 
      return;

    // Actual engine call, which does the dynamic compilation, the creation of the
    // code for all the objects, as well as inserting the function and looping 
    // over every data point

    if (!engine.run())
    {
      ///-----------------------
      // Backwards compatibility with version 3.0.2
      error("");
      error("This module does not fully support backwards compatibility:");
      error("C++/C functions are not supported in by this module anymore.");
      error("Please review documentation to explore available functionality and grammar of this module.");
      error("We are sorry for this inconvenience, but we do not longer support dynamically compiling in SCIRun.");
      ///-----------------------
      
      return;
    }
    
    for (size_t p = 0; p < NUM_PORTS; p++)
    {
      std::string matrixname = std::string("o")+to_string(p+1);
      engine.get_matrix(matrixname, omatrix[p]);
    }

    sendOutput(o1, omatrix[0]);
    sendOutput(o2, omatrix[1]);
    sendOutput(o3, omatrix[2]);
    sendOutput(o4, omatrix[3]);
    sendOutput(o5, omatrix[4]);
  }
}
