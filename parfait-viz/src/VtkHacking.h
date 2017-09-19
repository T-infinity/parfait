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
