# GTA San Andreas mod that provides cheat features
It's time to take some projects out of the drawer and dust them off.  
Cheat menu I wrote for myself a few years ago. It has a few features, that I made for single-player use only.  
Some of them works in SAMP. Also it has not so user-friendly menus, but idc.  
Can be made as standalone .dll, I think. But idc, as I said.  
Written in C++.

## Features
- Old-school style menu (can be switched to zgui in `Main.h`) ([pics](https://imgur.com/a/3KdGRFv))
- Silent aimbot (visible check, on screen check, cops/gangs priority, smart `(aim to closest bone)`, randomize, perfect accuracy)
- ESP (fully dynamic boxes, health & armor bars, aimbot target, radar, graffiti, collectibles)
- Misc (samp anim reset `(on key "Z")`, crosshair, invulnerability, never wanted, speedhack, no reload, airbrake, telekinesis `(tp peds infront of player)`, minigun mode `(ultimate +C xaxa)`, dumbass peds, give ammo/money/minigun, get collectibles, unlock nearby cars)

## Demo (Dogshit quality videos. But idc!)
<iframe width="280" height="157" src="https://www.youtube.com/embed/tGyhTJ-JgE0?si=RP_8v7BcYwnQZC7Z" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe> 
<iframe width="280" height="157" src="https://www.youtube.com/embed/9yV-tkFj8YM?si=JaTYUxFpqMZhfNPk" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe> 
<iframe width="280" height="157" src="https://www.youtube.com/embed/v8ALHrFJJzo?si=64WPK_vpsDV9zzU5" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

## How to
- At first you will need to install [plugin-sdk](https://github.com/DK22Pac/plugin-sdk/tree/master?tab=readme-ov-file#how-to-use-plugin-sdk)
- Download ZIP and extract it OR clone repo with `git clone https://github.com/rosko1337/CheatMenu.git`
- Build in Release using Visual Studio 2022
- Loading mod requires [ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader) installed

## Sources
- [zgui](https://github.com/zxvnme/zgui)
- [plugin-sdk](https://github.com/DK22Pac/plugin-sdk) (also [safetyhook](https://github.com/cursey/safetyhook))  
- [gta-reversed](https://github.com/gta-reversed/gta-reversed)  
- [mod_sa](https://github.com/BlastHackNet/mod_sa) (mostly original creators & contributors of s0beit)  
- gta_sa_compact.idb `("fastman92, TheLink2012, listener and many others")`  
- gta_sa_full.idb  

## Todo
- SAMP support (nametags, health & armor, afk state)
- Fix rendering in SAMP (SAMP draws over the cheat)
- Field of view setting for aimbot (semi-done, implemented under the name `"smart"`)
- Sniper rifle aimbot (current implementation works only when you shoot yourself in the foot, cuz of misunderstanding how game does it)
- Fix aimbot ([vid](https://www.youtube.com/watch?v=a_tKa9B2Sdk))
- Move aimbot to its own class
- Proper d3d9 renderer
- Proper input handling?

## License
[BSL-1.0](https://choosealicense.com/licenses/bsl-1.0/)