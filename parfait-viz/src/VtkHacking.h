
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#pragma once
#include <vtkMultiProcessController.h>
#include <vtkObjectFactory.h>

class vtkHackCommunicator : public vtkCommunicator
{
public:
    vtkTypeMacro(vtkHackCommunicator, vtkCommunicator);
    static vtkHackCommunicator *New();
    void PrintSelf(ostream &os, vtkIndent indent)  {};
    void PrintHeader(ostream &os, vtkIndent indent)  {};
    void PrintTrailer(ostream &os, vtkIndent indent)  {};

    int SendVoidArray(const void *buff, vtkIdType length, int type, int remotehandle, int tag)   {
        return 1;
    }
    int ReceiveVoidArray(void *buff, vtkIdType length, int type, int remotehandle, int tag)   {
        auto char_ptr = (char*)buff;
        for(int i = 0; i < length; i++){
            char_ptr[i] = 1;
        }
        return 1;
    }

protected:
    vtkHackCommunicator() { };
    ~vtkHackCommunicator()   {};

private:
    vtkHackCommunicator(const vtkHackCommunicator &)   ;
    void operator=(const vtkHackCommunicator &)   ;
};

inline vtkStandardNewMacro(vtkHackCommunicator);



class vtkHackController : public vtkMultiProcessController
{
public:
    static vtkHackController *New();
    vtkTypeMacro(vtkHackController,vtkMultiProcessController);
    void PrintSelf(ostream& os, vtkIndent indent)  {};
    void PrintHeader(ostream& os, vtkIndent indent)  {};
    void PrintTrailer(ostream& os, vtkIndent indent)  {};

    void Initialize(int*, char***, int)   {}
    void Initialize(int*, char***)   {}
    void Finalize()   {}
    void Finalize(int)   {}

    int GetLocalProcessId() { return rank; }

    void SingleMethodExecute()  {};

    void MultipleMethodExecute()  {};

    void CreateOutputWindow()   {}

    vtkGetObjectMacro(RMICommunicator, vtkCommunicator);
    virtual void SetCommunicator(vtkCommunicator *comm){
        Communicator = comm;
    };
    virtual void SetRMICommunicator(vtkCommunicator *){};

    void SetRank(int r){rank = r;}
protected:
    int rank;
    vtkHackController(){};
    ~vtkHackController()  {};

private:
    vtkHackController(const vtkHackController&);
    void operator=(const vtkHackController&);
};

inline vtkStandardNewMacro(vtkHackController);
