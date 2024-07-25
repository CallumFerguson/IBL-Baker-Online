#include "Baker.hpp"

#include <imgui.h>

void DrawDottedRectangle(const ImVec2& size, const char* label) {
    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 p1 = pos;
    ImVec2 p2 = ImVec2(pos.x + size.x, pos.y + size.y);

    // Draw dotted rectangle
    for (float x = p1.x; x < p2.x; x += 4.0f) {
        draw_list->AddLine(ImVec2(x, p1.y), ImVec2(x + 2.0f, p1.y), IM_COL32(255, 255, 255, 255));
        draw_list->AddLine(ImVec2(x, p2.y), ImVec2(x + 2.0f, p2.y), IM_COL32(255, 255, 255, 255));
    }
    for (float y = p1.y; y < p2.y; y += 4.0f) {
        draw_list->AddLine(ImVec2(p1.x, y), ImVec2(p1.x, y + 2.0f), IM_COL32(255, 255, 255, 255));
        draw_list->AddLine(ImVec2(p2.x, y), ImVec2(p2.x, y + 2.0f), IM_COL32(255, 255, 255, 255));
    }

    // Draw the label
    ImVec2 text_size = ImGui::CalcTextSize(label);
    ImVec2 text_pos = ImVec2(p1.x + (size.x - text_size.x) * 0.5f, p1.y + (size.y - text_size.y) * 0.5f);
    draw_list->AddText(text_pos, IM_COL32(255, 255, 255, 255), label);

    ImGui::Dummy(size);
}

void Baker::onImGui() {
    ImGuiIO &io = ImGui::GetIO();

    float displayWidth = io.DisplaySize.x;
    float displayHeight = io.DisplaySize.y;

    float windowWidth = 250.0f;

    // left window
    ImGui::SetNextWindowSize(ImVec2(windowWidth, displayHeight));
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Input", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    DrawDottedRectangle(ImVec2(ImGui::GetContentRegionAvail().x, 100), "Drop HDRI File Here");

    ImGui::End();

    // right window
    ImGui::SetNextWindowSize(ImVec2(windowWidth, displayHeight));
    ImGui::SetNextWindowPos(ImVec2(displayWidth , 0.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::Begin("Output", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::End();
}

void dropCallback(GLFWwindow *window, int count, const char **paths) {
    for (size_t i = 0; i < count; i++) {
        std::filesystem::path currentPath(paths[i]);

        if (std::filesystem::is_regular_file(currentPath)) {
            std::ifstream file(currentPath);
            if (file.is_open()) {
                std::cout << "Contents of file " << currentPath << ":\n";
                std::cout << std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()) << "\n";
                file.close();
            } else {
                std::cerr << "Unable to open file " << currentPath << "\n";
            }
        } else if (std::filesystem::is_directory(currentPath)) {
            std::cout << "Directory path: " << currentPath << "\n";
        } else {
            std::cerr << "Not a regular file or directory: " << currentPath << "\n";
        }
    }
}

void Baker::onStart() {
    GameEngine::Window::mainWindow().addDropCallback([](std::vector<std::filesystem::path>& paths) {
        if(paths.empty()) {
            std::cout << "no files dropped" << std::endl;
        }
        if(paths.size() > 1) {
            std::cout << "dropped " << paths.size() << " files. ignoring all the but one of the files" << std::endl;
        }

        auto &path = paths.back();

        if (std::filesystem::is_regular_file(path)) {
            if(path.extension() == ".hdr") {
                std::ifstream file(path);
                if (file.is_open()) {
                    std::cout << "Contents of file " << path << ":\n";
//                    std::cout << std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()) << "\n";
                    file.close();
                } else {
                    std::cerr << "Unable to open file " << path << "\n";
                }
            } else {
                std::cout << "Unsupported file type " << path.extension() << ". Only .hdr is supported." << std::endl;
            }
        } else if (std::filesystem::is_directory(path)) {
            std::cout << "Directory path: " << path << "\n";
        } else {
            std::cerr << "Not a regular file or directory: " << path << "\n";
        }
    });
}
