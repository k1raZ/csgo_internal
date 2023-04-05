#include "bunnyhop.h"


void hacks::BunnyHop(CUserCmd* cmd) noexcept {	
	
	if (Functional::Misc::bunnyhop) {
		if (!(globals::localPlayer->GetFlags() & CEntity::FL_ONGROUND))
			cmd->buttons &= ~CUserCmd::IN_JUMP;
	}	

}