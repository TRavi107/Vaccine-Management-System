#include "vmspch.h"
#include "sqlConnection.h"
#include "imGuiWindow.h"


int main(int, char**)
{
    //std::shared_ptr<sqlConnection> sqlConnectionInstance = std::make_shared<sqlConnection>();

    std::shared_ptr<imGuiWindow> imguiWindowInstance = std::make_shared<imGuiWindow>();
    imguiWindowInstance->Run();
    return 0;
}
