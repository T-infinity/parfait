#pragma once
#include <vtkMultiProcessController.h>
#include <vtkObjectFactory.h>

class vtkHackCommunicator : public vtkCommunicator
{
public:
    vtkTypeMacro(vtkHackCommunicator, vtkCommunicator);
    static vtkHackCommunicator *New();
    void PrintSelf(ostream &os, vtkIndent indent) VTK_OVERRIDE{};
    void PrintHeader(ostream &os, vtkIndent indent) VTK_OVERRIDE{};
    void PrintTrailer(ostream &os, vtkIndent indent) VTK_OVERRIDE{};

    int SendVoidArray(const void *buff, vtkIdType length, int type, int remotehandle, int tag) VTK_OVERRIDE {
        return 1;
    }
    int ReceiveVoidArray(void *buff, vtkIdType length, int type, int remotehandle, int tag) VTK_OVERRIDE {
        auto char_ptr = (char*)buff;
        for(int i = 0; i < length; i++){
            char_ptr[i] = 1;
        }
        return 1;
    }

protected:
    vtkHackCommunicator() { };
    ~vtkHackCommunicator() VTK_OVERRIDE {};

private:
    vtkHackCommunicator(const vtkHackCommunicator &) VTK_DELETE_FUNCTION;
    void operator=(const vtkHackCommunicator &) VTK_DELETE_FUNCTION;
};

vtkStandardNewMacro(vtkHackCommunicator);



class vtkHackController : public vtkMultiProcessController
{
public:
    static vtkHackController *New();
    vtkTypeMacro(vtkHackController,vtkMultiProcessController);
    void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE{};
    void PrintHeader(ostream& os, vtkIndent indent) VTK_OVERRIDE{};
    void PrintTrailer(ostream& os, vtkIndent indent) VTK_OVERRIDE{};

    void Initialize(int*, char***, int) VTK_OVERRIDE {}
    void Initialize(int*, char***) VTK_OVERRIDE {}
    void Finalize() VTK_OVERRIDE {}
    void Finalize(int) VTK_OVERRIDE {}

    int GetLocalProcessId() { return rank; }

    void SingleMethodExecute() VTK_OVERRIDE{};

    void MultipleMethodExecute() VTK_OVERRIDE{};

    void CreateOutputWindow() VTK_OVERRIDE {}

    vtkGetObjectMacro(RMICommunicator, vtkCommunicator);
    virtual void SetCommunicator(vtkCommunicator *comm){
        Communicator = comm;
    };
    virtual void SetRMICommunicator(vtkCommunicator *){};

    void SetRank(int r){rank = r;}
protected:
    int rank;
    vtkHackController(){};
    ~vtkHackController() VTK_OVERRIDE{};

private:
    vtkHackController(const vtkHackController&) VTK_DELETE_FUNCTION;
    void operator=(const vtkHackController&) VTK_DELETE_FUNCTION;
};

vtkStandardNewMacro(vtkHackController);
