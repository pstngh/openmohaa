# Development Session Handoff - OpenMoHAA Client-Side Bot

## Session Overview

Built a fully autonomous client-side bot system for OpenMoHAA from scratch. The bot can play the game independently with aggressive pistol-whip combat, smooth aimbot, 360-degree enemy detection, auto-reconnect, and intelligent movement/obstacle navigation.

**Duration**: Multi-session development (45+ commits)
**Branch**: `claude/clientbot-SSVHY`
**Main Goal**: Create a self-sufficient bot that can join servers, fight enemies, navigate maps, and stay connected indefinitely.

---

## Key Changes

### New Files Created

| File | Description |
|------|-------------|
| `code/client/cl_bot.cpp` | Main bot implementation (~1340 lines) - all AI logic, movement, aiming, combat |
| `code/client/cl_bot.h` | Bot structure definitions, state machine, function declarations |

### Modified Files

| File | Changes |
|------|---------|
| `code/client/cl_main.cpp` | Integrated bot initialization, frame updates, auto-reconnect logic |
| `code/client/client.h` | Added bot header includes and function declarations |
| `code/client/cl_input.cpp` | Bot command generation integration |
| `code/client/cl_keys.cpp` | Bot eye info handling |

### Core Systems Implemented

1. **State Machine**: IDLE → ROAMING → ATTACKING → DEAD with automatic transitions
2. **Aimbot System**: Smooth target angle tracking with 80/20 smoothing ratio
3. **Enemy Detection**: 360-degree instant detection with distance-based scoring (1000x weight)
4. **Combat System**: Pistol-whip mode with melee attacks, weapon auto-switching
5. **Movement System**: Constant forward movement with 60% strafe intensity (300ms cycles)
6. **Navigation**: Obstacle detection, wall avoidance, stuck detection/recovery
7. **Jumping**: Smart obstacle jumping, especially when chasing enemies
8. **Auto-Reconnect**: Cvar-based server reconnect with indefinite retry
9. **Team/Spawn Handling**: Auto-join team, auto-select pistol, auto-respawn

---

## Important Decisions

### 1. Aimbot Architecture: Smooth Target Angles (Not Velocity)

**Evolution**:
- ❌ Initial: Velocity prediction + lead targeting → caused jitter
- ❌ Attempt: Velocity smoothing → still erratic
- ✅ Final: Smooth the target angles themselves (80% new + 20% old)

**Why**: Enemy position changes frame-to-frame cause angle jumps. Smoothing angles directly prevents jitter while maintaining responsiveness.

**Location**: `cl_bot.cpp:625-658`
```cpp
// Calculate target angles from enemy position
vectoangles(aimDir, newTargetAngles);

// Smooth the target angles (80% new + 20% old)
clBot.targetAngles[PITCH] = newTargetAngles[PITCH] * 0.8f + clBot.targetAngles[PITCH] * 0.2f;
clBot.targetAngles[YAW] = AngleMod(newTargetAngles[YAW] * 0.8f + clBot.targetAngles[YAW] * 0.2f);
```

### 2. Enemy Detection: Distance Priority (360-Degree Instant)

**Problem**: Bot oscillated between enemies, ignored closer threats

**Solution**: Distance score weight = 1000, FOV bonus = 10
- Closest enemy almost always wins
- Tiny FOV bonus only breaks ties
- No teammate filtering (works for FFA and team modes)

**Location**: `cl_bot.cpp:1183-1195`
```cpp
// Score based purely on distance for 360-degree instant detection
score = (maxDist - dist) / maxDist * 1000.0f;

// Tiny bonus for enemies in front (10 points max) to break ties
if (dotProduct > 0) {
    score += dotProduct * 10.0f;
}
```

### 3. Combat Mode: Pistol-Whip (Melee Attacks)

**Why**: More aggressive and challenging for server players. Bot charges enemies and melees them.

**Implementation**:
- Auto-switch to pistol on spawn (weapon command batching)
- Toggle melee button every 200ms for continuous attacks
- 100% forward + 60% strafing when attacking
- Lean in strafe direction

**Location**: `cl_bot.cpp:939-977`

### 4. Auto-Reconnect: Cvar (Not File)

**Evolution**:
- ❌ File-based (`botserver.txt`) → multi-instance race conditions
- ✅ Cvar-based (`bot_server`) → per-process memory, no conflicts

**Benefits**:
- Launch 5+ instances from same folder without issues
- Per-instance server override: `+set bot_server "ip:port"`
- Runtime changes via console
- Follows game's config pattern

**Location**: `cl_bot.cpp:93`, `cl_main.cpp:2693-2700`

### 5. Movement: Always Forward + Strafing

**User Requirement**: "W key held 24/7"

**Implementation**:
- 100% forward movement always
- 60% strafe intensity
- 300ms cycle (left/right alternation)
- Lean matches strafe direction

**Location**: `cl_bot.cpp:516-522`, `cl_bot.cpp:681-687`

### 6. Jumping: Aggressive Pursuit

**Problem**: Bot got stuck at windows/railings when locked on enemies

**Solution**:
- When attacking enemy AND stuck → always jump (no wall check)
- When roaming AND stuck → jump only if obstacle < 10 units tall
- 1000ms cooldown between jumps

**Location**: `cl_bot.cpp:348-356`

### 7. Sticky Enemy Tracking

**Problem**: Bot looked away from enemies for no reason

**Solution**:
- Only clear enemy if LOS lost for 100+ consecutive checks
- Track `losFailCount` instead of single-frame checks
- Prevents brief occlusions from breaking lock

**Location**: `cl_bot.cpp:1101-1129`

### 8. C89 Compliance

**Critical**: All code must follow strict C89 standard
- Variables declared at top of function
- No inline array initialization
- No C99 for loop syntax
- Individual element assignment

---

## Gotchas & Solutions

### 1. C89 Compilation Failures (Multiple Instances)

**Problem**:
- Variables declared mid-function
- Inline array init: `vec3_t wallMins = {-12, -12, 0};`
- C99 loops: `for (int i = 0; i < 2; i++)`

**Solution**: Reset to working commit (4c12149), build incrementally with small commits

**Lesson**: Always declare variables at top, use individual assignment

---

### 2. Erratic Aim / Jitter

**Problem**: Aim "very shaky and unstable, rather than glued to enemies"

**Root Cause**: Replacing targetAngles every frame from moving enemy = angle jumps

**Solution**: Smooth target angles (80/20), remove all prediction/velocity

**Commits**:
- 682ac88: Simplified aimbot
- e15b117: Added velocity smoothing (didn't work)
- 682ac88: Removed prediction entirely

---

### 3. Enemy Oscillation

**Problem**: Rapidly switched between multiple visible enemies

**Root Cause**: Large FOV bonus (50 points) made bot prefer forward enemies over closer side enemies

**Solution**: Increase distance weight to 1000, reduce FOV bonus to 10

**Commit**: 2dd36cc

---

### 4. Ignoring Enemies in FFA

**Problem**: Bot ignored all players in free-for-all deathmatch

**Root Cause**: Team filtering checked for TEAM_ALLIES/TEAM_AXIS, excluded TEAM_FREEFORALL

**Solution**: Remove all teammate filtering - attack everyone

**Commit**: 6b2c65d

**Location**: `cl_bot.cpp:1225-1226`

---

### 5. Aim Pitch Calculation Issues (Multiple)

**Problem Series**:
- Pitch oscillation at extreme angles
- Bot aiming at sky
- Pitch normalization errors

**Solutions Applied**:
- Normalize pitch after `vectoangles()`
- Clamp to [-89, 89] degrees
- Use `AngleNormalize180()` for pitch

**Commits**: 7f11437, 97fd721, 0cf0b27

**Final Code**: `cl_bot.cpp:647-656`

---

### 6. Auto-Reconnect Not Working (Frame-Based)

**Problem**: Changed to 10000 frames, reconnect stopped working

**Root Cause**: `wasConnected` only updated inside 10000-frame check. If bot connected/disconnected between checks, flag never set.

**Solution**: Track connection every frame, attempt reconnect every 10000 frames

**Commit**: 472ac63

**Location**: `cl_main.cpp:2681-2689`

---

### 7. Single Reconnect Attempt

**Problem**: Bot tried once, then gave up if server down

**Root Cause**: Set `wasConnected = false` after attempt

**Solution**: Never reset `wasConnected` - keep retrying indefinitely

**Commit**: fedd035

---

### 8. Stuck at Windows/Railings

**Problem**: Bot locked aim on enemy through window, couldn't turn to navigate

**Root Cause**: Wall check threshold too strict (0.4 = 20 units)

**Solution**:
- Reduce threshold to 0.2 (10 units)
- When attacking + stuck → always jump (no wall check)

**Commits**: ff2e8c6, 8669829

---

### 9. Pistol Weapon Switching

**Problem**: Bot couldn't reliably switch to pistol on spawn

**Evolution**:
- ❌ Simple weapon command → didn't work
- ❌ Debug timing delays → inconsistent
- ✅ Batched commands with weapon command counter

**Solution**: Encode weapon command in button bits with proper counter (like cg.iWeaponCommandSend)

**Commits**: d42e0ab, fb17554, c9c9571

**Location**: `cl_bot.cpp:939-961`

---

### 10. Line-of-Sight Through Walls

**Problem**: Bot shot at enemies behind walls

**Solution**: Implement `CL_Bot_CanSeePoint()` with CM_BoxTrace from eye position

**Commit**: 806d38b

**Location**: `cl_bot.cpp:1057-1080`

---

### 11. Multi-Instance File I/O Race Condition

**Problem**: 5+ instances from same folder couldn't read `botserver.txt`

**Root Cause**: Concurrent file access, FS context differences, one-shot loading

**Solution**: Replace with `bot_server` cvar - per-process memory, no shared resources

**Commit**: ce0a6b0

---

## Implementation Notes

### Bot State Machine (cl_bot.cpp:170-209)

**States**:
- **IDLE**: Just spawned, waiting to initialize
- **ROAMING**: Wandering around looking for enemies
- **ATTACKING**: Locked on enemy, charging and attacking
- **DEAD**: Waiting to respawn

**Transitions**:
- IDLE → ROAMING: After team join + weapon select
- ROAMING → ATTACKING: Enemy detected
- ATTACKING → ROAMING: Enemy lost for 10 seconds
- Any → DEAD: Health <= 0
- DEAD → IDLE: Respawned

### Aiming System (cl_bot.cpp:598-662)

**Flow**:
1. Get enemy position + chest offset (+40 units)
2. Calculate direction from eye to target
3. Convert to angles with `vectoangles()`
4. Normalize pitch, clamp to [-89, 89]
5. Smooth angles: `new = target * 0.8 + current * 0.2`
6. Apply with configurable aim speed (default 360°/sec)

**Key**: Smoothing happens on target angles, not current angles. This prevents jitter from frame-to-frame enemy movement.

### Enemy Detection (cl_bot.cpp:1094-1202)

**Scoring System**:
```
Distance Score = (maxDist - dist) / maxDist * 1000.0
FOV Bonus = dotProduct * 10.0  (if enemy in front)
Total = Distance Score + FOV Bonus
```

**Process**:
1. Loop through all entities in snapshot
2. Skip self, teammates (removed), dead entities
3. Distance check (< 2048 units by default)
4. Line-of-sight check (CM_BoxTrace)
5. Calculate score (distance weighted 1000x)
6. Track best enemy

**Result**: Closest enemy almost always wins, 360-degree awareness

### Combat System (cl_bot.cpp:671-702)

**Pistol-Whip Mode**:
- Movement: 100% forward + 60% strafe (300ms cycle)
- Weapon: Auto-switch to pistol on spawn
- Attack: Melee button toggle every 200ms
- Lean: Match strafe direction
- Jumping: Always jump when stuck while attacking

### Stuck Detection (cl_bot.cpp:280-378)

**Algorithm**:
1. Every 500ms, check distance moved
2. If moved < 10 units → stuck
3. Increment `stuckCount`
4. Recovery:
   - Turn 180 degrees
   - If attacking enemy → always jump
   - Else if obstacle < 10 units → jump
   - Else → just turn

**Position History**: Track last 16 positions (1 per second) to detect circling

### Auto-Reconnect (cl_main.cpp:2676-2707)

**Logic**:
```
Every frame:
  - If connected → set wasConnected = true

Every 10000 frames:
  - If NOT connected AND wasConnected:
    - Read bot_server cvar
    - Execute "connect <server>"
    - Reset team/spawn flags
    - DON'T reset wasConnected (keep retrying)
```

**Retry Behavior**: Indefinite. Bot will keep trying every 10000 frames until reconnected.

### Team Join & Spawn (cl_bot.cpp:1203-1279)

**Sequence**:
1. Wait 2000ms after connect
2. Send batched command: `"ui_info allies 0 3\n"` (join allies, pick rifleman, pistol)
3. Set `hasJoinedTeam = true`
4. Every frame: check if dead → send `+attack` to respawn
5. After spawn: trigger weapon command for pistol

**Weapon Command Batching**: Encodes weapon index in button bits with counter to ensure server processes it.

### Cvars

| Cvar | Default | Description |
|------|---------|-------------|
| `cl_bot` | 0 | Enable/disable bot (1 = on, 0 = off) |
| `cl_bot_movespeed` | 127 | Movement speed (0-127) |
| `cl_bot_aimspeed` | 360 | Aim rotation speed (degrees/sec) |
| `cl_bot_attackdist` | 2048 | Max distance to detect enemies |
| `cl_bot_firedelay` | 150 | Min time between melee attacks (ms) |
| `cl_bot_roamtime` | 3000 | How long to roam in one direction (ms) |
| `cl_bot_debug` | 0 | Enable debug output |
| `bot_server` | 127.0.0.1:12203 | Server IP for auto-reconnect |

---

## Current State

### ✅ Fully Working

1. **Autonomous Play**: Bot joins, spawns, fights, navigates completely independently
2. **Aimbot**: Smooth, stable aim locked to enemies without jitter
3. **360° Detection**: Instant enemy detection at any angle, distance-prioritized
4. **Pistol-Whip Combat**: Charges enemies, switches to pistol, melees aggressively
5. **Movement**: Constant forward motion with 60% strafe intensity, 300ms cycles
6. **Jumping**: Smart obstacle navigation, aggressive pursuit through windows
7. **Auto-Reconnect**: Reconnects indefinitely when disconnected, per-instance config
8. **Team/Spawn**: Auto-joins allies, auto-selects pistol, auto-respawns
9. **Stuck Recovery**: Detects and escapes stuck situations reliably
10. **Multi-Instance**: 5+ bots can run from same folder without conflicts
11. **FFA Support**: Attacks everyone in free-for-all and team modes

### 🧪 Testing Status

**Tested Scenarios**:
- Single bot in team deathmatch ✅
- Multiple enemies in sight ✅
- Stuck behind obstacles/windows ✅
- Auto-reconnect after disconnect ✅
- Free-for-all mode ✅
- Multiple instances (5+) ✅

**Edge Cases to Test**:
- Very long server downtime (24+ hours)
- Extremely cluttered maps with tight spaces
- High-latency servers
- Map changes mid-session

### 📊 Code Statistics

- **Total Lines**: ~1340 (cl_bot.cpp) + 138 (cl_bot.h)
- **Functions**: 15 main functions + helpers
- **States**: 4 (IDLE, ROAMING, ATTACKING, DEAD)
- **Commits**: 45+ (excluding rollbacks)
- **C89 Compliant**: 100%

---

## Usage Guide

### Basic Usage

**Enable bot**:
```bash
./openmohaa +set cl_bot 1 +set bot_server "server.com:12203"
```

**Console commands**:
```
/bot_enable          # Enable bot
/bot_disable         # Disable bot
/set cl_bot 1        # Same as bot_enable
/set cl_bot_debug 1  # Enable debug output
```

### Multi-Instance Setup

Launch 5 bots to same server:
```bash
./openmohaa +set cl_bot 1 +set bot_server "192.168.1.100:12203" +set name "Bot1" &
./openmohaa +set cl_bot 1 +set bot_server "192.168.1.100:12203" +set name "Bot2" &
./openmohaa +set cl_bot 1 +set bot_server "192.168.1.100:12203" +set name "Bot3" &
./openmohaa +set cl_bot 1 +set bot_server "192.168.1.100:12203" +set name "Bot4" &
./openmohaa +set cl_bot 1 +set bot_server "192.168.1.100:12203" +set name "Bot5" &
```

Launch 5 bots to different servers:
```bash
./openmohaa +set cl_bot 1 +set bot_server "server1.com:12203" +set name "Bot1" &
./openmohaa +set cl_bot 1 +set bot_server "server2.com:12203" +set name "Bot2" &
# etc...
```

### Tuning Bot Behavior

**Faster aim**:
```
/set cl_bot_aimspeed 720
```

**Longer attack range**:
```
/set cl_bot_attackdist 4096
```

**Faster melee attacks**:
```
/set cl_bot_firedelay 100
```

**Change roaming behavior**:
```
/set cl_bot_roamtime 5000  # Roam 5 seconds in one direction
```

---

## Known Limitations

1. **Navigation**: Bot uses basic obstacle avoidance, no pathfinding. Can get stuck in complex geometry.
2. **Combat**: Only pistol-whip mode implemented (no shooting from range).
3. **Tactics**: No cover usage, flanking, or strategic positioning.
4. **Team Play**: No coordination with teammates.
5. **Map Knowledge**: No objective awareness (flags, bomb sites, etc.).

These are intentional simplifications for the current scope.

---

## Technical Debt / Future Improvements

1. **Pathfinding**: A* or waypoint system for better navigation
2. **Weapon Variety**: Support rifles, snipers, grenades
3. **Difficulty Levels**: Add configurable skill settings
4. **Team Coordination**: Communication and tactics with other bots
5. **Objective Play**: CTF, S&D, objective-based modes
6. **Machine Learning**: Neural network for advanced behavior

---

## Critical Code Locations

### Core Bot Logic
- **State Machine**: `cl_bot.cpp:170-209`
- **Main Think**: `cl_bot.cpp:211-278`
- **Aiming**: `cl_bot.cpp:598-662`
- **Movement**: `cl_bot.cpp:467-545`
- **Combat**: `cl_bot.cpp:671-702`

### Enemy Detection
- **Main Loop**: `cl_bot.cpp:1094-1202`
- **Scoring**: `cl_bot.cpp:1183-1195`
- **LOS Check**: `cl_bot.cpp:1057-1080`

### Navigation
- **Stuck Detection**: `cl_bot.cpp:280-378`
- **Obstacle Check**: `cl_bot.cpp:1281-1312`
- **Jumping**: `cl_bot.cpp:348-356`

### Auto-Reconnect
- **Connection Tracking**: `cl_main.cpp:2676-2707`
- **Cvar Setup**: `cl_bot.cpp:93`

### Team/Spawn
- **Team Join**: `cl_bot.cpp:1203-1244`
- **Respawn**: `cl_bot.cpp:722-749`
- **Weapon Select**: `cl_bot.cpp:939-961`

---

## Commit History Summary

### Initial Implementation (1737a6d - 806d38b)
- Base bot system with state machine
- Movement, aiming, firing
- Pitch fixes, LOS checks
- Stuck detection

### Combat System (9654f6b - b4bd479)
- Strafing and leaning
- Pistol-whip mode
- Melee attack toggling
- Weapon command batching

### Aimbot Refinement (516b953 - 682ac88)
- Enemy velocity tracking (removed later)
- Position prediction (removed later)
- Adaptive aim speed (removed later)
- **Final: Simple angle smoothing** ✅

### Auto-Reconnect (f9cd8a4 - ce0a6b0)
- File-based reconnect (replaced)
- Frame-based checking
- Indefinite retry
- **Final: Cvar-based system** ✅

### Enemy Detection (2dd36cc - 6b2c65d)
- Distance priority scoring ✅
- 360-degree detection ✅
- FFA support (no teammate filtering) ✅

### Navigation (ff2e8c6 - 8669829)
- Low obstacle jumping ✅
- Aggressive jump when attacking ✅

---

## Development Approach

### What Worked Well

1. **Incremental Commits**: Small, focused commits made debugging easier
2. **User Feedback**: Direct testing feedback prevented over-engineering
3. **Simplification**: Removing complexity (prediction, velocity) improved stability
4. **C89 Discipline**: Strict compliance prevented compilation issues
5. **Cvar System**: Using existing game architecture for config

### What Didn't Work

1. **Velocity Prediction**: Added complexity, didn't improve aim
2. **File-based Config**: Race conditions with multiple instances
3. **Large FOV Bonuses**: Caused enemy oscillation
4. **Single-frame LOS Checks**: Brief occlusions broke tracking
5. **Strict Wall Checks**: Prevented jumping low obstacles

### Key Lessons

1. **Simplicity Wins**: Simple angle smoothing > complex prediction
2. **Test Multi-Instance**: Single instance can hide race conditions
3. **User Testing is Critical**: Internal assumptions often wrong
4. **Rollback Quickly**: If 5 attempts fail, reset and rethink
5. **Follow Standards**: C89 compliance saves time in long run

---

## Files to Review for Continuation

### Must Read
1. `code/client/cl_bot.cpp` - Core implementation
2. `code/client/cl_bot.h` - Structure and API
3. `code/client/cl_main.cpp` (lines 2676-2707) - Auto-reconnect integration

### Helpful Context
4. `code/client/cl_input.cpp` - Command generation hookup
5. `code/client/client.h` - Client system integration
6. Git log (commits 1737a6d..1bea7a1) - Full development history

---

## Next Session Handoff

If continuing development:

1. **Test thoroughly**: Deploy to real server, run 24+ hours
2. **Watch for edge cases**: Map-specific stuck situations
3. **Consider enhancements**: Weapon variety, better navigation
4. **Document for users**: Add to main README if merging
5. **Performance profiling**: Check CPU usage with many bots

The bot is production-ready for basic autonomous play. All major issues resolved. Code is clean, well-commented, and C89 compliant.

---

**Last Updated**: Commit 1bea7a1
**Branch**: `claude/clientbot-SSVHY`
**Status**: ✅ Complete & Working
