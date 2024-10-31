#include "components.h"

namespace gui {
    void ToolTip(const char* text) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " (?)");
        if (ImGui::IsItemHovered()) {

            ImGui::BeginTooltip();
            ImGui::TextUnformatted(text);
            ImGui::EndTooltip();
        }
    }
} // namespace gui
