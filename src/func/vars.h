#pragma once

namespace Functional {
	namespace Misc {
		inline bool bunnyhop = false;
		inline bool triggerbot = false;
	}

	namespace Chams {
		inline bool chams = false;
		//inline const char* chamsName = "models/inventory_items/trophy_majors/crystal_blue";
		inline int chamsName = 0;
		inline const char* items[] = { 
			"models/inventory_items/trophy_majors/gold", 
			"models/gibs/glass/glass", 
			"models/inventory_items/trophy_majors/gloss", 
			"vgui/achievements/glow", 
			"models/inventory_items/wildfire_gold/wildfire_gold_detail", 
			"models/inventory_items/trophy_majors/velvet", 
			"models/inventory_items/trophy_majors/crystal_blue", 
		};
		inline int selected_material = 0;
	}
}