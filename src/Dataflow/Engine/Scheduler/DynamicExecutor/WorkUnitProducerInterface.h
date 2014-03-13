/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2012 Scientific Computing and Imaging Institute,
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

#ifndef ENGINE_SCHEDULER_DYNAMICEXECUTOR_WORKUNITPRODUCERINTERFACE_H
#define ENGINE_SCHEDULER_DYNAMICEXECUTOR_WORKUNITPRODUCERINTERFACE_H

#include <Dataflow/Engine/Scheduler/share.h>

namespace SCIRun {
  namespace Dataflow {
    namespace Engine {
      namespace DynamicExecutor {

        class SCISHARE ProducerInterface
        {
        public:
          virtual ~ProducerInterface() {}
          virtual bool isDone() const = 0;
          virtual void enqueueReadyModules() const = 0;
        };

        typedef boost::shared_ptr<ProducerInterface> ProducerInterfacePtr;
      }}
  }}

#endif