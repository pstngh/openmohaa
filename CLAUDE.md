# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

OpenMoHAA is an open-source reimplementation of Medal of Honor: Allied Assault (including Spearhead and Breakthrough expansions). Built on the ioquake3 engine and F.A.K.K SDK, it provides full compatibility with original game assets, scripts, and multiplayer while adding cross-platform support, bots, and modern enhancements. Current version: 0.83.0 (pre-1.0, "unstable" stage). Licensed under GPL v2+.

## Build Commands

**Prerequisites:** CMake 3.25+, flex (2.6.4+), bison (3.5.1+), ninja-build, a C17/C++17 compiler (GCC 9.4+, Clang 7+, or MSVC), libsdl2-dev, libopenal-dev.

```bash
# Configure (multi-config generator)
cmake -G "Ninja Multi-Config" -B ./build

# Build Debug
cmake --build ./build --config Debug --parallel

# Build Release (default if single-config)
cmake --build ./build --config Release --parallel

# Run all unit tests
cd build && ctest -C Debug --output-on-failure

# Run a single test
cd build && ctest -C Debug -R test_lz77 --output-on-failure
```

**CI uses:** Clang on Ubuntu with `CC=clang CXX=clang++`, Ninja Multi-Config generator, and flags: `-DUSE_RENDERER_DLOPEN=OFF -DBUILD_RENDERER_GL2=OFF`.

### Build Targets

| Target | Binary | Description |
|--------|--------|-------------|
| `openmohaa` | `openmohaa` | Game client |
| `omohaaded` | `omohaaded` | Dedicated server (defines `DEDICATED`) |
| `renderer_opengl1` | shared lib | GL1 renderer (default) |
| `renderer_opengl2` | shared lib | GL2 renderer with GLSL shaders |
| `cgame` | shared lib | Client-side game module (defines `CGAME_DLL`) |
| `game` | shared lib | Server-side game module (defines `GAME_DLL`, `WITH_SCRIPT_ENGINE`) |
| `launch_openmohaa_*` | executables | Launchers for base/spearhead/breakthrough |
| `test_lz77` | test executable | LZ77 compression unit test |

### Key CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_SERVER` | ON | Dedicated server binary |
| `BUILD_CLIENT` | ON | Client binary |
| `BUILD_RENDERER_GL1` | ON | OpenGL 1.x renderer |
| `BUILD_RENDERER_GL2` | OFF | OpenGL 2.0+ renderer with GLSL |
| `BUILD_GAME_LIBRARIES` | ON | Game module shared libraries |
| `BUILD_GAME_QVMS` | ON | QVM bytecode modules (via q3lcc/q3asm) |
| `USE_RENDERER_DLOPEN` | ON | Load renderer as DLL at runtime |
| `USE_INTERNAL_LIBS` | ON | Use vendored third-party libraries |
| `USE_OPENAL` | ON | OpenAL 3D audio |
| `USE_OPENAL_DLOPEN` | ON | Load OpenAL at runtime |
| `USE_HTTP` | ON | HTTP downloads (cURL) |
| `USE_CODEC_VORBIS` | ON | Ogg Vorbis audio |
| `USE_CODEC_OPUS` | ON | Opus audio |
| `USE_CODEC_MAD` | ON | MP3 audio (libmad) |
| `USE_FREETYPE` | OFF | TrueType font rendering |
| `USE_VOIP` | OFF | Voice chat (Opus) |

**Constraint:** If `USE_RENDERER_DLOPEN=OFF`, only one of GL1/GL2 can be enabled (static link). Both OFF is a fatal error.

### Version String

Generated at build time into `build/generated/q_version.generated.h` from git info (branch, hash, date) and `cmake/identity.cmake`. Source files `cl_console.c` and `common.c` have git dependencies via `add_git_dependency()` so they recompile on HEAD changes.

## Architecture

### High-Level Module Architecture

The engine uses a **runtime plugin architecture** where game logic and renderers are loaded as shared libraries (or QVMs) at runtime. Modules communicate through C function-pointer tables (import/export structs):

```
┌─────────────────────────────────────────────────────┐
│                    CORE ENGINE                       │
│  qcommon (collision, filesystem, network, commands)  │
│  sys (platform abstraction, DLL loading)             │
├──────────────────────┬──────────────────────────────┤
│    SERVER (server/)  │       CLIENT (client/)        │
│                      │  ┌──────────┐ ┌───────────┐  │
│  ┌────────────┐      │  │ cgame    │ │ renderer  │  │
│  │ fgame      │      │  │ module   │ │ module    │  │
│  │ module     │      │  │ (DLL)    │ │ (DLL)     │  │
│  │ (DLL)      │      │  └──────────┘ └───────────┘  │
│  └────────────┘      │                               │
├──────────────────────┴──────────────────────────────┤
│              SHARED SUBSYSTEMS                       │
│  tiki, skeletor, script, corepp, botlib, uilib      │
└─────────────────────────────────────────────────────┘
```

### Module Loading

Modules are loaded via `Sys_GetGameAPI()`, `Sys_GetCGameAPI()`, and `GetRefAPI()` in `code/sys/sys_main.c`. Each module:
1. Receives an import struct of engine function pointers
2. Returns an export struct of module function pointers
3. Has an API version check (`GAME_API_VERSION=15`, `CGAME_IMPORT_API_VERSION=3`, `REF_API_VERSION=14`)

### Core Engine (`code/qcommon/`, `code/server/`, `code/client/`, `code/sys/`)

**qcommon/** - Shared engine services:
- `cm_*.c` - Collision model: BSP loading (`cm_load.c`), ray/box tracing (`cm_trace.c`), terrain collision (`cm_terrain.c`), patch collision (`cm_patch.c`)
- `files.cpp` - Virtual filesystem: PK3 (ZIP) pak files, search path hierarchy (home → base → cd), mod directory support
- `msg.c` / `msg.cpp` - Network message protocol: bit-packed binary encoding, Huffman compression, delta compression for entity states, protocol version handling (6 and 15+)
- `net_chan.c` - Reliable/unreliable network channel over UDP
- `cmd.c` - Console command buffer with `EXEC_NOW`/`EXEC_INSERT`/`EXEC_APPEND` modes
- `cvar.c` - Console variable system with flags (ARCHIVE, USERINFO, SERVERINFO, LATCH, CHEAT, ROM), validation, hash lookup
- `q_shared.h` - Core data structures: `entityState_t`, `playerState_t`, `usercmd_t`, `trajectory_t`, game type enums
- `q_platform.h` - Architecture detection (x86, x86_64, arm, arm64, ppc, etc.)

**server/** - Server loop and networking:
- `server.h` - Key structures: `server_t` (world state, entity array, config strings), `client_t` (per-client connection state with snapshots, downloads, netchan), `svEntity_t` (PVS clusters, baselines)
- `sv_game.c` - Server↔game module interface: populates `game_import_t`, calls `Sys_GetGameAPI()`
- `sv_main.c` - Server frame loop (`SV_Frame()`): process packets → `ge->PrepFrame()` → `ge->RunFrame()` → build snapshots → send to clients
- `sv_snapshot.c` - Delta-compressed snapshot generation, rate limiting
- `sv_client.c` - Client connection/disconnection, user command processing
- `sv_world.c` - Spatial partitioning (world sectors), entity linking/unlinking, area queries

**client/** - Client loop and subsystems:
- `cl_main.cpp` - Client frame loop: `CL_Frame()` → check resend → send commands → process server messages → update screen → update audio
- `cl_cgame.cpp` - Client↔cgame module interface: populates `clientGameImport_t` (~200+ function pointers), calls `Sys_GetCGameAPI()`
- `cl_parse.cpp` - Server message parsing: `svc_gamestate`, `svc_snapshot`, `svc_configstring`, `svc_cgameMessage`, `svc_download`
- `cl_ui.cpp` - UI system: menu stack, HUD elements, console windows, scoreboard. Uses `UIWidget` class hierarchy from uilib
- `cl_input.cpp` - Input processing and user command generation
- `client.h` - Key structures: `clientActive_t` (game state, snapshots, commands), `clientConnection_t` (network state, reliable commands, demo recording), `clSnapshot_t` (server time, player state, entities)

**sys/** - Platform abstraction:
- `sys_main.c` - `Sys_LoadGameDll()` loads platform-specific DLL/SO, finds `dllEntry`/`vmMain` symbols
- `sys_unix.c` / `sys_win32.c` - Platform-specific implementations
- `con_tty.c` / `con_win32.c` - Console I/O

### Server Game Module (`code/fgame/`)

**Interface** (`g_public.h`):
- `game_import_t` (~150+ functions): Printf, collision traces, entity linking, TIKI animation, sound, messaging (MSG_Write*), config strings, HUD drawing, alias system, debug rendering
- `game_export_t` (~25 functions): `Init`, `Shutdown`, `SpawnEntities`, `ClientConnect`/`Begin`/`Think`/`Disconnect`, `RunFrame`, `PrepFrame`, save/load (`WriteLevel`/`ReadLevel`/`ArchivePersistant`), `BotBegin`/`BotThink`
- Exports global pointers: `gentities` array, `gentitySize`, `num_entities`

**Entity Class Hierarchy:**
```
Listener (event dispatch, script integration)
  └─ SimpleEntity (origin, angles, target/targetname)
      └─ SimpleArchivedEntity
          └─ Entity (physics, health, collision, model, binding)
              └─ Animate (animation slots, frame deltas, sync)
                  └─ Sentient (weapons, ammo, inventory, teams, vehicles)
                      ├─ SimpleActor (pathfinding, emotion, animation modes)
                      │   └─ Actor (full AI state machine, perception, tactics)
                      └─ Player (input, camera, HUD, game stats)
```

**Key Entity Files:**
- `entity.h` - `Entity`: velocity, health, takedamage, solid, movetype, team binding, damage delegates
- `sentient.h` - `Sentient`: `activeWeaponList[MAX_ACTIVE_WEAPONS]`, `ammo_inventory`, `m_Enemy`, `m_Team` (TEAM_GERMAN=0, TEAM_AMERICAN=1), vehicle/turret pointers, helmet system
- `actor.h` - `Actor`: Think state machine (IDLE/PAIN/KILLED/ATTACK/CURIOUS/DISGUISE/GRENADE), perception (sight/hearing/FOV), enemy tracking, cover nodes, grenade handling, patrol paths, disguise system
- `player.h` - `Player`: usercmd processing, state maps for legs/torso animation, camera system, scoreboard stats
- `weapon.h` - `Weapon`: dual fire modes (primary/secondary), fire types (FT_BULLET/PROJECTILE/MELEE/HEAVY/LANDMINE), ammo clips, bullet spread/damage/knockback/penetration, view kick, zoom
- `vehicle.h` - `Vehicle`: driver/passenger/turret slots, wheel physics (mass, suspension, tire friction, gear ratios, braking), skidding, patrol splines
- `level.h` - `Level`: map state, time management, intermission, voting, objectives, earthquakes, fade effects, bad places
- `g_public.h` - `gentity_t`: contains `entityState_t s` (network state), `entityShared_t r` (collision/linking), `Entity *entity` (C++ wrapper, GAME_DLL only)

**Game Frame** (`g_main.cpp`): Process pending events → PreAnimate actors → Director control → Navigation update → Bot frame → Entity loop (G_AddGEntity) → Post events → Client blends → Exit/start rules → Vote check

**Game Modes** (`bg_public.h`): GT_SINGLE_PLAYER, GT_FFA, GT_TEAM, GT_TEAM_ROUNDS, GT_OBJECTIVE, GT_TOW (Team Objective War), GT_LIBERATION

**Spawn System:** Map entity strings parsed into key-value SpawnArgs → `getClassDef()` finds ClassDef → factory creates instance → `Level::SpawnEntities()` processes all → `Level::FindTeams()` links targets

### Client Game Module (`code/cgame/`)

**Interface** (`cg_public.h`):
- `clientGameImport_t` (~200+ functions): Console/cvars, file I/O, network message reading (MSG_Read*), collision model (CM_BoxTrace, CM_PointContents), full renderer API (R_RegisterModel/Shader, R_AddRefEntityToScene, R_RenderScene, R_ClearScene, R_DrawStretchPic, R_LoadFont), sound API (S_StartSound, S_AddLoopingSound, S_RegisterSound), TIKI/animation API, UI functions, alias system, client state queries (GetSnapshot, GetGameState, GetUserCmd)
- `clientGameExport_t` (~25 functions): `CG_Init`, `CG_Shutdown`, `CG_DrawActiveFrame`, `CG_Draw2D`, `CG_ParseCGMessage`, `CG_ConsoleCommand`, camera queries (EyePosition/Angles), permanent mark/treadmark decal rendering, animation init/cleanup

**Key Files:** `cg_main.c` (init/frame), `cg_commands.cpp` (command parsing/effects), `cg_parsemsg.cpp` (server message parsing, 67KB), `cg_ents.c` (entity rendering), `cg_marks.c` (decal system)

**Snapshot** (`cg_public.h`): `snapshot_t` contains `playerState_t`, up to `MAX_ENTITIES_IN_SNAPSHOT` (1024) entity states, server sounds, area mask for PVS

### Renderer Modules (`code/renderergl1/`, `code/renderergl2/`, `code/renderercommon/`)

**Interface** (`tr_public.h`):
- `refexport_t` (~100 functions): lifecycle (Shutdown, BeginRegistration, EndRegistration, BeginFrame, EndFrame), asset registration (RegisterModel/Shader/Skin, LoadWorld), scene (ClearScene, AddRefEntityToScene, AddLightToScene, AddPolyToScene, RenderScene), 2D drawing (SetColor, DrawStretchPic, DrawBox, Scissor), fonts (LoadFont, DrawString), marks/fragments, debug lines, TIKI integration (ForceUpdatePose, TIKI_Orientation), swipe effects
- `refimport_t` (~100 functions): logging, memory (Hunk_Alloc, Malloc/Free), cvars, commands, collision model, filesystem, cinematics, input init, TIKI skeleton/animation access, performance counters, UI resource loading

**GL1 Renderer** (`renderergl1/`):
- `tr_local.h` - Core structures: `trGlobals_t` (global state: world data, shaders, images, lightmaps, sphere lights, sine/wave tables), `shader_t` (up to 8 stages, deformations, surface flags, fog, sort order), `shaderStage_t` (texture bundles, blend state, color/alpha generation, texture mods), `shaderCommands_t` (tesselator: 4096 verts, 6144 indices batch), `viewParms_t` (frustum planes, projection matrix, portal state), `world_t` (BSP data: nodes, surfaces, lightmaps, terrain patches, static models, PVS clusters)
- `tr_main.c` - Frame entry: frustum culling (`R_CullLocalBox`/`R_CullPointAndRadius`), entity transforms, matrix operations
- `tr_bsp.c` - BSP world loading and lightmap processing (128x128 textures, overbright color shifting)
- `tr_shade.c` / `tr_shader.c` - Shader/material system with hash lookup (8192 buckets). Color generation modes: identity, entity, vertex, waveform, lighting grid/spherical. Alpha modes: identity, entity, vertex, distance/height fade. Texture coordinate generation: environment mapping, sun reflection, fog. Wave functions: sin, square, triangle, sawtooth, noise. Deformations: wave, bulge, autosprite, light glow, flap
- `tr_image.c` - Texture management (MAX_DRAWIMAGES=2048), format support: BMP, JPG, PCX, PNG, PVR, TGA
- `tr_light.c` - Lighting: dynamic lights (MAX_DLIGHTS=32), sphere lights (MAX_SPHERE_LIGHTS=512), light grid sampling

**GL2 Renderer** (`renderergl2/`): Adds GLSL shaders (auto-converted from `.glsl` files to C strings via `cmake/utils/stringify_shader.cmake`), FBOs, VBOs, post-processing, DDS textures, IQM model support

**Surface Types:** SF_FACE (brush), SF_GRID (patch), SF_POLY (cgame), SF_TIKI_SKEL (skeletal model), SF_TIKI_STATIC, SF_SPRITE, SF_TERRAIN_PATCH, SF_SWIPE, SF_TRIANGLES, SF_FLARE, SF_MARK_FRAG

### TIKI Animation System (`code/tiki/`)

TIKI = "The Interactive Key-frame Image" - Custom model/animation format specific to MoHAA.

**File Formats:**
- `.tik` - Text definition: SETUP section (skelmodel, scale, surfaces, shaders, LOD), INIT section (client/server commands), ANIMATIONS section (aliases with flags, per-frame commands)
- `.skb` / `.skd` - Binary skeleton model (ident "SKL "/"SKMD", versions 3-6): bones, surfaces, vertices, weights, hit boxes, morph targets
- `.skc` - Binary animation (ident "SKAN", versions 13-14): keyframes with RLE compression, channel data (rotation quaternions + position vectors), motion deltas

**Key Structures:**
- `dtiki_t` - Runtime model: name, animations (`dtikianim_t`), skeletor pointer, surfaces, scale/LOD, bone channel list, mesh indices
- `dtikianim_t` - Animation container: animation definitions array (`dtikianimdef_t`), init commands (client/server), model data, aliases
- `dtikianimdef_t` - Single animation: alias name, weight (for random selection), blend time, flags (TAF_RANDOM, TAF_DELTADRIVEN, TAF_NOREPEAT), per-frame client/server commands
- `dtikisurface_t` - Surface: up to 4 shaders, skin variants, flags (TIKI_SURF_NODRAW, CROSSFADE, NODAMAGE, NOMIPMAPS)

**Animation Command Timing:** TIKI_FRAME_FIRST (0), TIKI_FRAME_EVERY (-1), TIKI_FRAME_EXIT (-2), TIKI_FRAME_ENTRY (-3), TIKI_FRAME_END (-4), TIKI_FRAME_LAST (-5), or numeric frame numbers

**Key Files:** `tiki_parse.cpp` (text file parsing), `tiki_skel.cpp` (skeleton loading), `tiki_anim.cpp` (animation queries), `tiki_cache.cpp` (model caching), `tiki_tag.cpp` (bone transform queries), `tiki_frame.cpp` (frame command processing)

**Limits:** MAX bones=100, MAX vertices/surface=1000, MAX triangles/surface=2000, MAX animations=4095, MAX channels=2560

### Skeletor System (`code/skeletor/`)

Skeletal animation blending engine used by both renderer and game modules.

**Bone Type Hierarchy** (12 types in `skeletor_internal.h`):
- `skelBone_World` - Root identity transform
- `skelBone_Root` - Special root (Bip01) with position+rotation channels
- `skelBone_PosRot` - Position + rotation (two channels)
- `skelBone_Rotation` - Pure rotation (one channel)
- `skelBone_IKshoulder`/`IKelbow`/`IKwrist` - Inverse kinematics chain for arms
- `skelBone_AvRot` - Averaged rotation blend between two reference bones
- `skelBone_HoseRot`/`HoseRotBoth`/`HoseRotParent` - Hose/cable bend toward target bone

**Animation Blending:** Up to 64 blend frames split into movement (0-31) and action (32-63). Final pose = lerp(movement_blend, action_blend, actionWeight). Quaternion channels use SLERP; position channels use linear interpolation. Frame interpolation with configurable tolerance and wrapping for delta-driven (cyclical) animations.

**Channel System:** Global `ChannelNameTable` registry (up to 2560 channels). Channels named by convention: `"BoneName rot"` (rotation), `"BoneName pos"` (position). `skelChannelList_c` maps between global and local channel indices per model.

**Math Types:** `SkelMat4` (4x3 transform), `SkelQuat` (quaternion), `SkelVec3`/`SkelVec4`

**Key Files:** `skeletor.h/cpp` (main class, pose calculation), `skeletor_internal.h` (bone hierarchy), `skeletorbones.cpp` (bone loading), `skeletor_loadanimation.cpp` (animation loading with RLE), `bonetable.cpp` (channel name table)

### C++ Class System (`code/corepp/`)

**Reflection System** (`class.h`):
- `CLASS_PROTOTYPE(classname)` - Declares in class: static `ClassInfo`, `Responses[]` array, factory method `_newInstance()`, script wait-till methods (when `WITH_SCRIPT_ENGINE` defined)
- `CLASS_DECLARATION(parentclass, classname, classid)` - Implements: creates `ClassDef` instance (registered in global linked list), factory, `Responses[]` array follows immediately after the macro
- `ClassDef` - Runtime type info: class name/ID, parent link, response lookup table (indexed by event number for O(1) dispatch), factory function, size. Static `classlist` linked list of all registered classes
- `ResponseDef<Type>` - Maps `Event*` to member function pointer `void (Type::*response)(Event*)`
- Registration flow: ClassDef constructor links into global list → `ClassDef::BuildEventResponses()` builds per-class response lookup tables from inheritance chain

**Event System** (`listener.h`):
- `Event` - Typed event with dynamic `ScriptVariable` argument array. Constructors register event definitions (name, flags, format spec, docs) in `eventDefList`. Static `EventQueue` (circular linked list of `EventQueueNode`) for deferred execution
- Event types: `EV_NORMAL` (0), `EV_RETURN` (1), `EV_GETTER` (2), `EV_SETTER` (3)
- Event flags: `EV_CONSOLE`, `EV_CHEAT`, `EV_CODEONLY`, `EV_CACHE`, `EV_TIKIONLY`, `EV_SCRIPTONLY`, `EV_SERVERCMD`
- Priority constants for spawn ordering: `EV_SPAWNARG` (-7), `EV_PROCESS_INIT` (-6), `EV_POSTSPAWN` (-5), `EV_SPAWNACTOR` (-2)
- `Listener` - Base class for all event-handling objects. Key methods: `PostEvent(ev, delay)` → queues in EventQueue; `ProcessEvent(ev)` → immediate dispatch via `classinfo()->responseLookup[eventnum]`; `Register`/`Unregister`/`Notify` for script wait-till system; `CancelEventsOfType`/`CancelPendingEvents` for cleanup
- Global functions: `L_InitEvents()` (load events, build responses), `L_ProcessPendingEvents()` (dequeue and dispatch by time), `L_ShutdownEvents()`

**Script Engine** (`code/script/`):
- `ScriptVM` (`scriptvm.h`) - Bytecode virtual machine with states: RUNNING, SUSPENDED, WAITING, EXECUTION, DESTROYED. Uses `ScriptVMStack` for local variables and `ScriptCallStack` for call frames
- `ScriptClass` (`scriptclass.h`) - Container for compiled `GameScript` bytecode, manages `ScriptVM` threads, label lookup
- `ScriptVariable` (`scriptvariable.h`) - Dynamic typed variable: NONE, STRING, INTEGER, FLOAT, CHAR, CONSTSTRING, LISTENER, REF, ARRAY, CONSTARRAY, CONTAINER, VECTOR, POINTER. Union storage with type casting methods
- Parser: Flex/Bison grammar in `code/parser/` (`lex_source.txt`, `bison_source.txt`) generates `code/parser/generated/yyLexer.cpp` and `yyParser.cpp`. Supports: control flow, method/event calls, try/catch, variable types, functions with up to 6 params

**Other corepp Components:**
- `Container<Type>` (`container.h`) - Dynamic array (1-based indexing), with `AddObject`, `RemoveObjectAt`, `Sort`
- `con_set<Key, Value>` (`con_set.h`) - Hash table map with `MEM_BlockAlloc` allocator
- `str` (`str.h`) - Reference-counted string with copy-on-write, path utilities (extension, slash conversion)
- `MEM_BlockAlloc<Type, blocksize>` (`mem_blockalloc.h`) - Pool allocator with circular free lists
- `MEM_TempAlloc` (`mem_tempalloc.h`) - Linear frame-lifetime allocator
- `cLZ77` (`lz77.h`) - LZ77 compression/decompression with dictionary hash table
- `SafePtr<Type>` (`safeptr.h`) - Weak pointer that auto-nulls when target `Class` is destroyed

### Bot System

The bot system has **two distinct layers**:

1. **Inherited Q3 botlib** (`code/botlib/`) - Full Quake III AAS-based bot library from ioquake3. Retained as infrastructure but **NOT actively used** by OpenMoHAA's multiplayer bots.
2. **Custom OpenMoHAA bot system** (`code/fgame/playerbot*.cpp`, `g_bot.cpp`) - The **active** bot system with its own state machine, navigation (RecastNavigation or legacy PathNode), and combat logic.

#### Q3 Botlib (`code/botlib/`) - Inherited, Inactive

AAS (Area Awareness System) file format with spatial navigation data:
- Area content flags: WATER, LAVA, SLIME, CLUSTERPORTAL, TELEPORTER, JUMPPAD, DONOTENTER, MOVER
- Travel types: WALK, CROUCH, BARRIERJUMP, JUMP, LADDER, WALKOFFLEDGE, SWIM, WATERJUMP, TELEPORT, ELEVATOR, ROCKETJUMP, GRAPPLEHOOK, DOUBLEJUMP, RAMPJUMP, STRAFEJUMP, JUMPPAD, FUNCBOB
- Core structures: `aas_t` (world data with areas, reachabilities, routing caches, portal/cluster hierarchy), `aas_area_t`, `aas_reachability_t`, `aas_cluster_t`
- Route calculation: `AAS_AreaTravelTimeToGoalArea()`, `AAS_AreaRouteToGoalArea()`, `AAS_PredictRoute()`, cluster-based travel time tables
- AI layers: `be_ai_goal.h` (goal stack with MAX_GOALSTACK=8, item weights, fuzzy logic), `be_ai_move.h` (movement execution: walk/crouch/jump/grapple with avoid spots), `be_ai_weap.h` (weapon selection with `weaponinfo_t`/`projectileinfo_t`, projectile prediction), `be_ai_char.h` (personality: MAX_CHARACTERISTICS=80 float/int/string traits loaded from files), `be_ai_chat.h` (template-based chat with match variables, gender, context-based replies)
- Elementary actions (`be_ea.h`): `EA_Move`, `EA_View`, `EA_Attack`, `EA_Jump`, `EA_Crouch`, `EA_SelectWeapon`, `EA_Say`/`EA_SayTeam`
- Fuzzy weight system (`be_ai_weight.h`): balance weights for item/weapon selection decisions

#### Active Bot System (`code/fgame/`)

**Architecture:**
```
BotManager (singleton: botManager)
  └─ BotControllerManager (botControllerManager)
       └─ Container<BotController*> controllers
            └─ BotController → Player entity
                 ├─ BotMovement (pathfinding + collision avoidance)
                 ├─ BotRotation (turning/aiming)
                 ├─ State machine (Attack, Curious, Grenade, Idle, Weapon)
                 └─ Delegates (gotKill, killed, stufftext, spawned)
```

**Key Files:**
- `g_bot.h`/`g_bot.cpp` - Bot management API, spawning, model selection, save/restore
- `playerbot.h`/`playerbot.cpp` - BotController class, state machine, combat, weapon selection
- `playerbot_movement.cpp` - Navigation, collision avoidance, jumping
- `playerbot_rotation.cpp` - Smooth turning and aiming
- `playerbot_master.cpp` - BotManager, event broadcasting to all bots
- `navigation_path.h`/`.cpp` - `IPather` interface and factory
- `navigation_recast_path.h`/`.cpp` - RecastNavigation-based pathfinding (dtPathCorridor, MAX_NPOLYS=256)
- `navigation_legacy_path.h`/`.cpp` - Legacy PathNode-based pathfinding (wraps ActorPath)
- `navigation_recast_load.h` - NavigationMap: BSP-to-navmesh generation at map load

**Bot Lifecycle:**
1. `G_BotInit()` → scan `models/player/*.tik` for allied/german models, init managers
2. `G_BotPostInit()` → restore bots from previous level, spawn bots per `sv_numbots`
3. `G_AddBot()` → find free client slot, assign name/models, `G_BotConnect()`, `G_BotBegin()`, create `BotController`
4. Per frame: `G_BotFrame()` → `ThinkControllers()` → each `BotController::Think()` → `UpdateBotStates()` → generate `usercmd_t` → `G_ClientThink()`
5. `G_RemoveBot()` → remove controller, `G_ClientDisconnect()`
6. Level change: `G_WriteBotSessionData()`, cleanup all controllers, reset

**State Machine** (5 states, bitmask-based, multiple can be active simultaneously):

| State | Condition | Behavior |
|-------|-----------|----------|
| Attack | Enemy visible or recently seen (within 1s) | Target enemy, fire weapon, tactical movement, burst fire |
| Curious | Heard sound event (20s duration) | Move toward last heard sound position |
| Grenade | (NOT IMPLEMENTED - returns false) | Avoid grenades (TODO) |
| Idle | No attack or curious | Wander, move to attractive nodes, revisit death location (20% chance) |
| Weapon | (commented out) | Switch to best weapon |

**Combat System:**
- Enemy detection: sorts all sentients by distance, 80-degree FOV check, range limited to `min(world->m_fAIVisionDistance, world->farplane_distance * 0.828)`
- Reaction time: base `g_bot_attack_react_min_delay` (0.2s) + random `g_bot_attack_react_random_delay` (1.2s), scaled by distance (closer = faster)
- Semi-auto: fire only when viewmodel idle and spread < 0.25; full-auto: hold trigger
- Burst fire: continuous fire for `fireDelay + 0.5s + random(1.5s)`, then pause for `fireDelay + 0.1s + random(0.5s)`
- Zoom/scope: auto-toggles right attack for scoped weapons
- Melee fallback: uses secondary fire (FT_MELEE) when out of ammo or close range
- Aiming: targets enemy's "eyes" bone tag, adds random offset within bounding box every 100ms scaled by `g_bot_attack_spreadmult` (1.0)
- Continues aiming at last known position for 3s after losing sight

**Movement System (BotMovement):**
- `MoveTo()`, `MoveNear()`, `AvoidPath()`, `MoveToBestAttractivePoint()`
- Stuck detection (every 1000ms): if velocity < 8 or hasn't moved 64 units in 2 checks, enters backup state (reverse for 750ms); gives up after 5 consecutive blocks
- Jump detection: traces forward at step height, if blocked but clearable above, jumps after 100ms delay
- Edge jumping: detects gaps and jumps if ground exists on the other side
- Collision avoidance (every 250ms): traces left/right in 4 steps (32-unit increments), picks direction with more open path

**Rotation System (BotRotation):**
- Smooth turning: accelerates/decelerates per axis, max `g_bot_turn_speed` (15) degrees, min threshold 20 degrees
- `AimAt(vPos)`: calculates direction from eye to target, converts to angles

**Pathfinding (`IPather` interface):**
- Factory: `IPather::CreatePather()` → `RecastPather` if navmesh valid, else `LegacyPather`
- `RecastPather`: uses Detour `dtPathCorridor` (256 max polys), auto-generated navmesh from BSP geometry, coordinate conversion (game Y-forward/Z-up ↔ Recast Y-up/Z-forward), handles off-mesh connections (ladders, teleporters)
- `LegacyPather`: wraps `ActorPath` using manually-placed `PathNode` entities, controlled by `g_navigation_legacy` cvar

**Attractive Nodes:**
- `AttractiveNode` entities in maps guide bot movement with priority levels
- Properties: `m_iPriority` (higher = more important), `m_fMaxStayTime`, `m_fMaxDistance`, `m_fRespawnTime` (cooldown), `m_iTeam` (team filter)
- Bots select nearest node meeting priority/distance/team criteria, stay for configured time, then cooldown

**Event Broadcasting:**
- `BotManager::BroadcastEvent()` notifies all bots of game events within radius
- Event types with default radii: WEAPON_FIRE (2048), WEAPON_IMPACT (384), EXPLOSION (4096), VOICE (1024-1536), FOOTSTEP (512), GRENADE (384), BADPLACE (32768)
- Probability based on distance: `1.0 - (distSq / radiusSq)`; closer events more likely noticed

**Team Behavior:**
- Auto-join via `EV_Player_AutoJoinDMTeam` with stagger delay (`entnum/20` seconds)
- `G_RemoveBots()` removes from team with most players for balance
- AttractiveNodes support team-specific attraction (allied/axis/any)
- `sv_sharedbots`: bots share client slots, relocated via `G_BotShift()` when real player connects

**Taunt System:**
- On kill: 1/`g_bot_instamsg_chance` (20%) probability, `g_bot_instamsg_delay` (5s) cooldown
- Sends "dmmessage" event with protocol-specific taunt codes

**Model Selection:**
- Allied models: prefixed `allied_` or `american_`; German models: prefixed `german_`, `IT_`, `SC_`
- Excludes `_fps.tik` (first-person models); re-randomized on death

**Console Commands:** `addbot <count>`, `addbotnamed <name>`, `removebot <count>`, `bot movehere`/`moveherenear`/`avoidhere`/`telehere` (debug)

**Key CVars:**

| Cvar | Default | Description |
|------|---------|-------------|
| `sv_maxbots` | 0 | Maximum bot slots (LATCH) |
| `sv_numbots` | 0 | Desired number of bots |
| `sv_minPlayers` | 0 | Minimum total players (bots fill deficit) |
| `sv_sharedbots` | 0 | Bots share client slots (LATCH) |
| `g_bot_attack_react_min_delay` | 0.2 | Min reaction delay (seconds) |
| `g_bot_attack_react_random_delay` | 1.2 | Random added reaction delay |
| `g_bot_attack_spreadmult` | 1.0 | Aim offset spread multiplier |
| `g_bot_attack_burst_min_time` | 0.1 | Min burst pause time |
| `g_bot_attack_burst_random_delay` | 0.5 | Random burst delay |
| `g_bot_attack_continuousfire_min_firetime` | 0.5 | Min continuous fire before burst |
| `g_bot_attack_continuousfire_random_firetime` | 1.5 | Random continuous fire time |
| `g_bot_turn_speed` | 15 | Turn speed (degrees) |
| `g_bot_instamsg_chance` | 5 | 1/N taunt chance on kill |
| `g_bot_instamsg_delay` | 5.0 | Taunt cooldown (seconds) |
| `g_bot_manualmove` | 0 | Debug: disable bot movement |
| `g_navigation_legacy` | 0 | Use legacy PathNode navigation (LATCH) |

**Not Yet Implemented:** Grenade avoidance state (TODO), weapon-switch state (commented out), bot skill levels (hardcoded "Average"), bot session persistence (`#if 0`-disabled)

### UI Library (`code/uilib/`)

Widget-based UI framework with ~40 widget classes:
- Base: `UIWidget` (hierarchical, position/size, materials, animation, event handling)
- Widgets: `UIButton`, `UIField`, `UIList`/`UIListCtrl`, `UILabel`, `UICheckbox`, `UISlider`, `UIMenu`/`MenuManager`, `UIConsole`, `UIDialog`, `UIFloatwnd`, `UILayout`, scrollbars
- Import/export C API (`ui_public.h`) for engine integration

### Network Protocol

**Frame Flow:**
1. Client sends `usercmd_t` (serverTime, buttons, angles, movement) to server
2. Server runs `ge->ClientThink()` for each client, then `ge->RunFrame()`
3. Server builds `snapshot_t` per client: `playerState_t` + delta-compressed `entityState_t` array + server sounds
4. Client receives snapshot, applies delta to previous state, calls `cge->CG_DrawActiveFrame()`

**Delta Compression:** `MSG_WriteDeltaEntity()`/`MSG_ReadDeltaEntity()` - only changed fields transmitted. Entity baselines established at map load.

**entityState_t** (q_shared.h) - Core networked state: position trajectory, origin/angles, loopSound, parent/tag attachment, modelindex, frameInfo[MAX_FRAMEINFOS] (animation), bone_tag/bone_angles/bone_quat (bone controllers), surfaces[32], clientNum, groundEntityNum, scale, alpha, renderfx, shader_data

**playerState_t** (q_shared.h) - Player-specific: commandTime, pm_type/flags, origin, velocity, gravity, speed, groundEntityNum, viewangles, viewheight, leanAngle, stats[MAX_STATS], ammo arrays, music mood, reverb, camera state, fov, screen blend, radarInfo

**Protocol Versions:** 6 (legacy MoHAA) and 15+ (MoHAA 2.0+/OpenMoHAA)

### Filesystem

**Search path hierarchy** (highest to lowest priority): home path game zips → home path game dir → base path game zips → base path game dir → cd path (same pattern) → base game (same pattern) → server downloads.

**PK3 files:** ZIP-compressed pak files searched highest-to-lowest number. Hash table lookup for files within paks. Checksum verification for pure servers.

**Key cvars:** `fs_basepath`, `fs_homeconfigpath`, `fs_homedatapath`, `fs_homestatepath`, `fs_basegame`

### Build System Details (`cmake/`)

**Layout:**
- `cmake/identity.cmake` - Project name (openmohaa), version (0.83.0), binary names
- `cmake/version.cmake` - Git-based version string generation (branch, hash, date) → `q_version.generated.h`
- `cmake/server.cmake`, `client.cmake`, `basegame.cmake` - Target definitions with source lists
- `cmake/renderer_gl1.cmake`, `renderer_gl2.cmake` - Renderer targets (GL2 converts .glsl→C via `stringify_shader.cmake`)
- `cmake/launcher.cmake` - Launcher executables for base/spearhead/breakthrough
- `cmake/compilers/` - Compiler flags: GNU (`-Wall -Wimplicit -Wshadow -fno-strict-aliasing -Werror=return-type`), GCC (`-fno-gnu-unique`), Clang (`-Wno-pointer-bool-conversion`), Apple Clang (`-fvisibility=hidden`), MSVC (`/W4 /wd4267 /we4715`)
- `cmake/platforms/` - Platform setup: Windows (ws2_32/winmm/psapi/dbghelp, NSIS installer), Linux (default prefix `/opt/mohaa`, .desktop files, DEB package), macOS (universal arm64+x86_64, .app bundle, DMG, code signing, min deployment 11.0), Emscripten (WASM, 256MB memory, SDL2 built-in)
- `cmake/libraries/` - Each vendored library: SDL2 (2.32.8), OpenAL (1.24.3), zlib (1.3.1), libjpeg (9f), libogg (1.3.6), libvorbis (1.3.7), opus (1.5.2), opusfile (0.12), libmad, cURL (8.15.0), recastnavigation, freetype (optional). Strategy: `USE_INTERNAL_*` flag → compile from `code/thirdparty/` sources, else `find_package()` fallback
- `cmake/libraries/flex_bison.cmake` - Flex/Bison: generates `yyLexer.cpp`/`yyParser.cpp` from `code/parser/` grammar. macOS: checks Homebrew paths for non-outdated versions. Flags: `-Cem --nounistd`
- `cmake/tests/` - CTest: `test_lz77` executable (15s timeout) from `corepp/tests/test_lz77.cpp`
- `cmake/utils/` - Utilities: `arch.cmake` (architecture detection via compiler test), `set_output_dirs.cmake` (per-config output directories), `disable_warnings.cmake` (suppress warnings for thirdparty code), `add_git_dependency.cmake` (recompile on HEAD change), `find_include_dirs.cmake` (recursive header discovery), `stringify_shader.cmake` (GLSL→C embedding), `qvm_tools.cmake` (QVM compiler toolchain: q3lcc, q3asm, q3rcc, q3cpp, lburg)

**Build suffixes:** Debug builds get `-dbg` suffix. LTO enabled by default (disabled for Debug and Emscripten).

**Cross-compilation:** Architecture detected via `cmake/utils/arch.cmake` (compiles test with `q_platform.h`, reads ARCH_STRING pragma). MSVC selects ASM files based on detected arch. CI builds for: x86_64, i686, arm64, armhf, ppc, ppc64, ppc64el (Linux); x64, x86, arm64 (Windows); universal arm64+x86_64 (macOS).

### Tools (`code/tools/`)

- `q3asm` - QVM bytecode assembler
- `q3lcc` / `q3rcc` / `q3cpp` / `lburg` - Q3 C compiler pipeline for QVM compilation
- `md5_2_skX` - Doom3 MD5 model to MoHAA TIKI format converter
- `ommap` - BSP map compiler (brush operations, texture loading)

## Coding Conventions

- **Variables:** camelCase. **Functions:** PascalCase.
- Format with `clang-format` (config in `.clang-format`): 4-space indent, 120 column limit, LF line endings, no tabs. Brace style: functions and classes open on new line (`AfterFunction: true`, `AfterClass: true`), structs/enums/control flow don't. `BinPackArguments: false`, `BinPackParameters: false`. `PointerAlignment: Right` (`int *p`). `SortIncludes: false`.
- Annotate changes to original game code with comments:
  - `// Added in OPM` - New code
  - `// Changed in OPM` - Modified code
  - `// Fixed in OPM` - Bug fixes
  - `// Removed in OPM` - Deleted code
  - Use version numbers when referencing specific game versions: `// Added in 2.11`. Known versions: 1.00, 1.10, 1.11, 2.0, 2.1, 2.11, 2.15, 2.30, 2.40
  - Group related additions with `//====` markers
- New source files must include the GPL v2+ license header (template in `docs/markdown/05-contributing/01-license-header.md`) with current year.
- Only `#include` what's necessary. Each source file should contain related classes and functions.
- Event declarations use PascalCase names (`Event EV_YourEventName`) with each parameter on a new line.
- All changes must maintain backward compatibility: original assets load correctly, networking compatible with original clients/servers, scripts/mods remain functional, singleplayer campaign fully playable.
- No personal tags or author comments (git tracks authorship).
- Avoid `_var` backwards-compatibility hacks or `// removed` comments for deleted code.
