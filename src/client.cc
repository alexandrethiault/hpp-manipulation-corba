// Copyright (c) 2015, Joseph Mirabel
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of hpp-manipulation-corba.
// hpp-manipulation-corba is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// hpp-manipulation-corba is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// hpp-manipulation-corba. If not, see <http://www.gnu.org/licenses/>.

#include "hpp/corbaserver/manipulation/client.hh"

#include <iostream>

#include "hpp/corbaserver/orb-singleton.hh"

namespace hpp
{
  namespace corbaServer
  {
    namespace manipulation
    {
      using CORBA::Exception;
      using CORBA::Object_var;
      using CORBA::SystemException;
      using CORBA::ORB_init;
      using CORBA::PolicyList;
      using omniORB::fatalException;

      Client::Client(int argc, char *argv[])
      {
        OrbSingletonAccessor::get_mutable_instance ().create (argc, argv);
      }

      void Client::connect (const char* iiop)
      {
        // Get a reference to the Naming Service
        CORBA::Object_var rootContextObj = OrbSingletonAccessor::get_mutable_instance ().orb_->string_to_object(iiop);
        CosNaming::NamingContext_var nc =
          CosNaming::NamingContext::_narrow(rootContextObj.in());

        // Bind robotObj with name Robot to the hppContext:
        CosNaming::Name objectName;
        objectName.length(2);
        objectName[0].id   = (const char*) "hpp";       // string copied
        objectName[0].kind = (const char*) "corbaserver"; // string copied
        objectName[1].id   = (const char*) "manipulation";   // string copied
        objectName[1].kind = (const char*) "robot"; // string copied

        // Invoke the root context to retrieve the object reference
        CORBA::Object_var managerObj = nc->resolve(objectName);
        // Narrow the previous object to obtain the correct type
        robot_ = hpp::corbaserver::manipulation::Robot::_narrow (managerObj.in ());

        // Bind obstacleObj with name Obstacle to the hppContext:
        objectName[1].id   = (const char*) "manipulation"; // string copied
        objectName[1].kind = (const char*) "problem";   // string copied

        managerObj = nc->resolve(objectName);
        // Narrow the previous object to obtain the correct type
        problem_ = hpp::corbaserver::manipulation::Problem::_narrow(managerObj.in());

        // Bind obstacleObj with name Obstacle to the hppContext:
        objectName[1].id   = (const char*) "manipulation"; // string copied
        objectName[1].kind = (const char*) "graph";   // string copied

        managerObj = nc->resolve(objectName);
        // Narrow the previous object to obtain the correct type
        graph_ = hpp::corbaserver::manipulation::Graph::_narrow(managerObj.in());
      }

      /// \brief Shutdown CORBA server
      Client::~Client()
      {
      }
    } // end of namespace manipulation.
  } // end of namespace corbaServer.
} // end of namespace hpp.
