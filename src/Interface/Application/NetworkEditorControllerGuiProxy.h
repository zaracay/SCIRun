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

#ifndef INTERFACE_APPLICATION_NETWORKEDITORCONTROLLERGUIPROXY_H
#define INTERFACE_APPLICATION_NETWORKEDITORCONTROLLERGUIPROXY_H

#include <QObject>
#include <boost/shared_ptr.hpp>
#include <Core/Dataflow/Network/NetworkFwd.h>

namespace SCIRun {
  namespace Engine
  {
    class NetworkEditorController;
  }

namespace Gui {
  
  class NetworkEditorControllerGuiProxy : public QObject
  {
    Q_OBJECT
  public:
    explicit NetworkEditorControllerGuiProxy(boost::shared_ptr<SCIRun::Engine::NetworkEditorController> controller);
  public Q_SLOTS:
    void addModule(const std::string& moduleName);
    void removeModule(const std::string& id);
    void addConnection(const SCIRun::Domain::Networks::ConnectionDescription& desc);
    void removeConnection(const SCIRun::Domain::Networks::ConnectionId& id);
    void saveNetwork(const std::string& filename);
    void executeAll(const SCIRun::Domain::Networks::ExecutableLookup& lookup);
  Q_SIGNALS:
    void moduleAdded(const std::string& name, SCIRun::Domain::Networks::ModuleHandle module);
  private:
    boost::shared_ptr<SCIRun::Engine::NetworkEditorController> controller_;
  };

}
}

#endif