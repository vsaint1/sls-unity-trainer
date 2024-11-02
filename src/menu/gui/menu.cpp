#include "menu/gui/menu.h"


namespace menu {
    void Setup() {
        printf("Setting up colors\n");
    }


    void Render() {
        if (configs::bMenuOpened) {

            ImGui::Begin("pintocubostamijo");
            ImGui::SetWindowSize(ImVec2(600, 400));
            ImGui::Checkbox("Invulnerable", &configs::bInvulnerable);
            ImGui::Checkbox("Remove Shoots Cooldown", &configs::removeShootsCooldown);

            ImGui::SliderInt("Max HP", &configs::maxHp, 0, 1000);
            ImGui::SliderFloat("Attack Speed", &configs::atkSpeedMultiplier, 0.0f, 1000.0f);
            ImGui::SliderInt("Damage Multiplier", &configs::damageMultiplier, 0, 1000);
            ImGui::SliderInt("Shoot's Quantity", &configs::shootsQuantity, 0, 100);
            gui::ToolTip("Increasing this may cause lag");

            ImGui::Combo("Staff's", &configs::selectedStaff, configs::staffNames, IM_ARRAYSIZE(configs::staffNames));
            ImGui::Combo("Hat's", &configs::selectedHat, configs::hatNames, IM_ARRAYSIZE(configs::hatNames));

            ImGui::Button("Unlock All Staffs");
            ImGui::Button("Unlock All Hats");

            if (ImGui::InputInt("Coins", &configs::coins, 50, 1000)) {
                spdlog::info("Adding coins");
                
                fn::AddCoins(configs::coins);
            }


            if (ImGui::Button("Close")) {
                configs::bWindowShouldClose = true;
            }

            ImGui::End();
        }
    }
} // namespace menu
