# TODOs

## 11/05/26

- [x] Add BitmapFont + UIText

## 18/05/26

- [ ] Code refactor (ordered, compact):
	- [x] Make a Clock object (dt + fps logic out of `App`)
	- [x] Create a DirectionalLight object (light matrix calculus out of `App`)
		I've discovered some but with shadows and lights, so im trying to fix
	- [ ] Make SDL events wrapper (`SDL_Event` -> API events)
	- [ ] Make an object to query Keyboard state (like `SDL_GetKeyboardState()`)
	- [ ] Hide chunk update/loading/remesh behind a world runtime API
	- [ ] Keep app-facing API feature-oriented and extensible (platformer/sandbox/custom)
	- [ ] Final cleanup: consistency + remove duplicated leftover logic

## Later ...
- [ ] Add frustum culling
- [ ] Add CSM (Cascaded Shadow Maps)
