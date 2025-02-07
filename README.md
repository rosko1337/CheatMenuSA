# GTA San Andreas mod that provides cheat features
It's time to take some projects out of the drawer and dust them off.  
A cheat menu I wrote for myself a few years ago. It has a few features, that I made for single-player use only.  
Some of them also work in SAMP. Also it has not so user-friendly menu, but idc.  
It can likely be compiled as a standalone .dll, I think. But idc, as I said.  
Written in C++.

## Features
- Old-school style **OOP** menu (can be switched to zgui in `Main.h`)
- Silent aimbot (visible check, on screen check, cops/gangs priority, smart `(aim to closest bone)`, randomize, perfect accuracy)
- ESP (fully dynamic boxes, health & armor bars, aimbot target, radar, graffiti, collectibles)
- Misc (samp anim reset `(on key "Z")`, crosshair, invulnerability, never wanted, speedhack, no reload, airbrake, telekinesis `(tp peds infront of player)`, minigun mode `(ultimate +C xaxa)`, dumbass peds, give ammo/money/minigun, get collectibles, unlock nearby cars)

## Demo (Dogshit quality videos. But idc!)
[Menu pics](https://imgur.com/a/3KdGRFv), [menu class diagram](https://imgur.com/a/1IDnH66)

<a href="https://www.youtube.com/watch?v=tGyhTJ-JgE0"><img src="https://img.youtube.com/vi/tGyhTJ-JgE0/hqdefault.jpg" alt="Video" style="width:280px;height:170px;"></a> 
<a href="https://www.youtube.com/watch?v=9yV-tkFj8YM"><img src="https://img.youtube.com/vi/9yV-tkFj8YM/hqdefault.jpg" alt="Video" style="width:280px;height:170px;"></a> 
<a href="https://www.youtube.com/watch?v=v8ALHrFJJzo"><img src="https://img.youtube.com/vi/v8ALHrFJJzo/hqdefault.jpg" alt="Video" style="width:280px;height:170px;"></a>

## How to
- At first you will need to install [plugin-sdk](https://github.com/DK22Pac/plugin-sdk/tree/master?tab=readme-ov-file#how-to-use-plugin-sdk)
- Download ZIP and extract it OR clone repo with `git clone https://github.com/rosko1337/CheatMenu.git`
- Build in Release using Visual Studio 2022
- Loading mod requires [ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader) installed

## Todo
- SAMP support (nametags, health & armor, afk state)
- Fix rendering in SAMP (it draws on top of the cheat)
- Sniper rifle aimbot (currently disabled ~~the current implementation only works when you shoot yourself in the foot, due to a misunderstanding of how the game handles it~~)
- Fix aimbot misses under certain conditions. ([vid](https://www.youtube.com/watch?v=a_tKa9B2Sdk))
- Move aimbot to its own class
- Proper d3d9 renderer
- Proper input handling?

## Sources
- [zgui](https://github.com/zxvnme/zgui)
- [plugin-sdk](https://github.com/DK22Pac/plugin-sdk) (also [safetyhook](https://github.com/cursey/safetyhook))  
- [gta-reversed](https://github.com/gta-reversed/gta-reversed)  
- [mod_sa](https://github.com/BlastHackNet/mod_sa) (mostly original creators & contributors of s0beit)  
- gta_sa_compact.idb `("fastman92, TheLink2012, listener and many others")`  
- gta_sa_full.idb  

## License
[BSL-1.0](https://choosealicense.com/licenses/bsl-1.0/)
