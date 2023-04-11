#include "hooks.h"

// include minhook for epic hookage
#include "../../ext/minhook/minhook.h"

//imgui
#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_win32.h"
#include "../../ext/imgui/imgui_impl_dx9.h"

#include <intrin.h>
#include <stdexcept>
#include "../func/func.h"


void hooks::Setup() noexcept
{
	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize minhook");

	//imgui
	if (MH_CreateHook(
		VirtualFunction(gui::device, 42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	)) throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(
		VirtualFunction(gui::device, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	)) throw std::runtime_error("Unable to hook Reset()");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Unable to enable hooks");

	//hooks
	// AllocKeyValuesMemory hook
	MH_CreateHook(
		memory::Get(interfaces::keyValuesSystem, 1),
		&AllocKeyValuesMemory,
		reinterpret_cast<void**>(&AllocKeyValuesMemoryOriginal)
	);

	// CreateMove hook
	MH_CreateHook(
		memory::Get(interfaces::clientMode, 24),
		&CreateMove,
		reinterpret_cast<void**>(&CreateMoveOriginal)
	);

	// drawmodel hook
	MH_CreateHook(
		memory::Get(interfaces::studioRender, 29),
		&DrawModel,
		reinterpret_cast<void**>(&DrawModelOriginal)
	);


	MH_EnableHook(MH_ALL_HOOKS);
	gui::DestroyDirectX();
}

void hooks::Destroy() noexcept
{
	// restore hooks
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	// uninit minhook
	MH_Uninitialize();
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
	static const auto returnAddress = _ReturnAddress();

	const auto result = EndSceneOriginal(device, device);

	// stop endscene getting called twice
	if (_ReturnAddress() == returnAddress)
		return result;

	if (!gui::setup)
		gui::SetupMenu(device);

	if (gui::open)
		gui::Render();

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

void* __stdcall hooks::AllocKeyValuesMemory(const std::int32_t size) noexcept
{
	// if function is returning to speficied addresses, return nullptr to "bypass"
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		address == reinterpret_cast<std::uint32_t>(memory::allocKeyValuesEngine) ||
		address == reinterpret_cast<std::uint32_t>(memory::allocKeyValuesClient)) 
		return nullptr;

	// return original
	return AllocKeyValuesMemoryOriginal(interfaces::keyValuesSystem, size);
}

bool __stdcall hooks::CreateMove(float frameTime, CUserCmd* cmd) noexcept
{
	// make sure this function is being called from CInput::CreateMove
	if (!cmd->commandNumber)
		return CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);

	// this would be done anyway by returning true
	if (CreateMoveOriginal(interfaces::clientMode, frameTime, cmd))
		interfaces::engine->SetViewAngles(cmd->viewAngles);

	// get our local player here
	globals::UpdateLocalPlayer();


	if (Functional::Misc::triggerbot) {
		hacks::TriggerBot(cmd);
	}

	if (globals::localPlayer && globals::localPlayer->IsPlayer() || globals::localPlayer->IsAlive()) {
		if (Functional::Misc::bunnyhop) 
			hacks::BunnyHop(cmd);
	}

	return false;
}


void __stdcall hooks::DrawModel(
	void* results,
	const CDrawModelInfo& info,
	CMatrix3x4* bones,
	float* flexWeights,
	float* flexDelayedWeights,
	const CVector& modelOrigin,
	const int32_t flags
) noexcept
{
	if (globals::localPlayer && info.renderable) {
		CEntity* entity = info.renderable->GetIClientUnknown()->GetBaseEntity();

		if (Functional::Chams::chams) {
			if (entity && entity->IsPlayer() && entity->GetTeam() != globals::localPlayer->GetTeam()) {

				static IMaterial* material = interfaces::materialSystem->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail");

				// при изменении помещать в глобалы
				constexpr float hidden[3] = { 1.06f, 0.38f, 1.73f };
				const float visible[3] = { 1.06f, 0.38f, 1.73f };

				interfaces::studioRender->SetAlphaModulation(1.f); // прозрачность

				// show thr walls
				material->SetMaterialVarFlag(IMaterial::IGNOREZ, true);
				interfaces::studioRender->SetColorModulation(hidden);
				interfaces::studioRender->ForcedMaterialOverride(material);
				DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

				// dnt show thr walls
				material->SetMaterialVarFlag(IMaterial::IGNOREZ, false);
				interfaces::studioRender->SetColorModulation(visible);
				interfaces::studioRender->ForcedMaterialOverride(material);
				DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

				return interfaces::studioRender->ForcedMaterialOverride(nullptr);
			}
		}
	}

	DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
}