#include "vmspch.h"
imGuiWindow::imGuiWindow(std::shared_ptr<sqlConnection> sqlConnectionInst)
:sqlConnectionInstance {sqlConnectionInst}
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        //do something
    }
    std::cout << "hello" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    window = glfwCreateWindow(Win_width, Win_height, "Vaccine ManageMent System", NULL, NULL);
    if (window == NULL)
    {
        //do something
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    // Our state
}

imGuiWindow::~imGuiWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void imGuiWindow::Run()
{
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        switch (activeWin)
        {
        case DisplayWindow::loginWin:
            LoginMenu();
            break;

        case DisplayWindow::tableWin:
            TableWindow();
            MenuBar();
            break;

        case DisplayWindow::browseTableWin:
            BrowseTableWindow();
            MenuBar();
            
            break;

        case DisplayWindow::Droptable:
            DropTable();
        default:

            break;
        }
        
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}
void imGuiWindow::DropTable() {
    std::string query = "drop table ";
    query = query + activeDBtable;
    std::cout << query.c_str() << std::endl;
    //MYSQL_RES* res=sqlConnectionInstance->Perform_Query(sqlConnectionInstance->GetConnectPtr(), "SELECT * WHERE REFERENCED_TABLE_SCHEMA = 'mydatabase' AND REFERENCED_TABLE_NAME = '<course>'");
    
    try  {
        sqlConnectionInstance->Perform_Query(sqlConnectionInstance->GetConnectPtr(), query.c_str());
    }
    catch (char* excp) {

    }
    
    SwitchState(DisplayWindow::tableWin);

    //mysql_free_result(res);

}

void imGuiWindow::BrowseTableWindow() {
    ImVec2 outer_size = ImVec2(0.0f, 30 * 8);
    ImGui::SetNextWindowSize(tableWinSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(tableWinPos, ImGuiCond_FirstUseEver);
    std::string query3 = "Table ";
    query3 = query3 + activeDBtable;
    if (ImGui::Begin("Database xyz")) {
        //ImGui::SetWindowFontScale(1.5);
        if (ImGui::Button("back")) {
            SwitchState(DisplayWindow::tableWin);
        }
        ImGui::Text(query3.c_str());
        if (ImGui::BeginTable("table_scrollx", 7, flags, outer_size))
        {
            std::string query2 =  "SHOW COLUMNS FROM ";
            query2 = query2 + activeDBtable;
            MYSQL_RES* res2 = sqlConnectionInstance->Perform_Query(sqlConnectionInstance->GetConnectPtr(), query2.c_str());
            MYSQL_ROW row_data2;
            ImGui::TableSetupScrollFreeze(0, 1);
            while ((row_data2 = mysql_fetch_row(res2)) != NULL)
            {
                ImGui::TableSetupColumn(row_data2[0], ImGuiTableColumnFlags_NoHide);
            }
            ImGui::TableHeadersRow();
            //mysql_free_result(res2);

            std::string query = "select * from ";
            query = query + activeDBtable;
            MYSQL_RES* res = sqlConnectionInstance->Perform_Query(sqlConnectionInstance->GetConnectPtr(),query.c_str());
            MYSQL_ROW row_data;
            int num_fields = mysql_num_fields(res);

            int row = 0;
            while ((row_data = mysql_fetch_row(res)) != NULL)
            {
                ImGui::TableNextRow();
                row++;
                for (int column = 0; column < num_fields; column++)
                {
                    // Both TableNextColumn() and TableSetColumnIndex() return true when a column is visible or performing width measurement.
                    // Because here we know that:
                    // - A) all our columns are contributing the same to row height
                    // - B) column 0 is always visible,
                    // We only always submit this one column and can skip others.
                    // More advanced per-column clipping behaviors may benefit from polling the status flags via TableGetColumnFlags().
                    if (!ImGui::TableSetColumnIndex(column) && column > 0)
                        continue;

                    //ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
                    //ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
                    //ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
                    
                    ImGui::Text(row_data[column]);
                    //ImGui::PopStyleColor(3);

                }
            }
            mysql_free_result(res);

            ImGui::EndTable();
        }
        
        ImGui::End();
    }
}

void imGuiWindow::LoginMenu() {
    ImGui::SetNextWindowSize(loginWinSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(loginWinPos, ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Login Window"))
    {
        static char userName[64] = "";
        ImGui::InputText("Username", userName, 64);
        static char password[64] = "";
        ImGui::InputText("password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
        if (ImGui::Button("Login")) {
            std::cout << userName << std::endl;
        }
        ImGui::SameLine();
        if (ImGui::Button("Login")) {
            std::cout << userName << std::endl;
        }
        ImGui::SameLine();
        if (ImGui::Button("Login")) {
            std::cout << userName << std::endl;
        }
        ImGui::SameLine();
        if (ImGui::Button("Login")) {
            std::cout << userName << std::endl;
        }
        ImGui::End();
    }
}

void imGuiWindow::TableWindow() {
    ImVec2 outer_size = ImVec2(0.0f, 50 * 8);
    ImGui::SetNextWindowSize(tableWinSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(tableWinPos, ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Database xyz"),nullptr, ImGuiCond_FirstUseEver) {
        //ImGui::SetWindowFontScale(1.5);
        if (ImGui::BeginTable("table_scrollx", 7, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("Tables", ImGuiTableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()
            ImGui::TableSetupColumn("Action");
            ImGui::TableSetupColumn("");
            ImGui::TableSetupColumn("");
            ImGui::TableSetupColumn("");
            ImGui::TableSetupColumn("");
            ImGui::TableSetupColumn("");
            ImGui::TableHeadersRow();
            MYSQL_RES* res =sqlConnectionInstance->Perform_Query(sqlConnectionInstance->GetConnectPtr(), "show tables");
            int num_fields = mysql_num_fields(res);
            MYSQL_ROW row_data;
            int row=0;
            while ((row_data = mysql_fetch_row(res)) != NULL)
            {
                row++;
                ImGui::TableNextRow();
                for (int column = 0; column < 4; column++)
                {
                    // Both TableNextColumn() and TableSetColumnIndex() return true when a column is visible or performing width measurement.
                    // Because here we know that:
                    // - A) all our columns are contributing the same to row height
                    // - B) column 0 is always visible,
                    // We only always submit this one column and can skip others.
                    // More advanced per-column clipping behaviors may benefit from polling the status flags via TableGetColumnFlags().
                    if (!ImGui::TableSetColumnIndex(column) && column > 0)
                        continue;

                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
                    
                    switch (column)
                    {
                    case 0:
                        if (ImGui::Button(row_data[column])) {
                            SwitchState(DisplayWindow::browseTableWin);
                            SwitchDBtable(row_data[column]);
                        }
                        break;
                    case 1:
                        ImGui::PushID(row);
                        if (ImGui::Button("Browse")) {
                            SwitchState(DisplayWindow::browseTableWin);
                            SwitchDBtable(row_data[0]);
                        }
                        ImGui::PopID();

                        break;

                    case 2:
                        ImGui::PushID(row);
                        if (ImGui::Button("Search")) {

                        }
                        ImGui::PopID();
                        break;

                    case 3:

                        if (ImGui::Button("Insert")) {

                        }
                        break;

                    case 4:
                        ImGui::PushID(row);
                        if (ImGui::Button("Drop")) {
                            SwitchState(DisplayWindow::Droptable);
                            SwitchDBtable(row_data[0]);
                        }
                        ImGui::PopID();
                        break;
                    default:
                        break;
                    }
                    ImGui::PopStyleColor(3);

                }
            }
            mysql_free_result(res);
           
            ImGui::EndTable();
        }
        ImGui::End();
    }
}

void imGuiWindow::MenuBar(){
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void imGuiWindow::DropTableWin()
{
    Sleep(1000);
    DropTable();
}

void imGuiWindow::glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

