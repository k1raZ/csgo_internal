#include "func.h"
#include <Windows.h>

int hacks::TriggerBot(CUserCmd* cmd) noexcept {
    globals::localPlayer = interfaces::entityList->GetEntityFromIndex(interfaces::engine->GetLocalPlayerIndex());

    if (!GetAsyncKeyState(VK_XBUTTON2))
        return false;

    if (!globals::localPlayer || !globals::localPlayer->IsAlive())
        return false;

    CVector eyePosition;
    globals::localPlayer->GetEyePosition(eyePosition);

    CVector aimPunch;
    globals::localPlayer->GetAimPunch(aimPunch);

    const CVector dst = eyePosition + CVector{ cmd->viewAngles + aimPunch }.ToVector() * 1000.0f;

    CTrace trace;
    interfaces::engineTrace->TraceRay({ eyePosition,dst }, 0x46004009, globals::localPlayer, trace);

    if (!trace.entity || trace.entity->GetTeam() == globals::localPlayer->GetTeam())
        return false;

    if (!trace.entity->IsAlive() || trace.entity->GetTeam() == globals::localPlayer->GetTeam())
        return false;

    cmd->buttons |= CUserCmd::ECommandButton::IN_ATTACK;

    return true;
}