/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

OpenMoHAA source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

OpenMoHAA source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenMoHAA source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// hb_model.h: the shared, data-fitted model of the human-imitation bots.
//
// Every table here is pooled over all recorded players (humanbot/model/shared.json).
// Per-bot variation comes only from the style dials (hb_style.h), which shift
// these tables through calibrated offsets.

#pragma once

#include <string>
#include <vector>

namespace hb
{

enum Context {
    CTX_HIDDEN_NOFIRE,
    CTX_HIDDEN_FIRE,
    CTX_LOS_NOFIRE,
    CTX_LOS_FIRE,
    CTX_RELOAD,
    CTX_COUNT
};

// The lean chain has a sixth context: no living enemy (the time after a kill).
constexpr int LEAN_CTX_DEAD  = CTX_COUNT;
constexpr int LEAN_CTX_COUNT = CTX_COUNT + 1;

constexpr int NUM_CHORDS    = 9;
constexpr int CHORD_NEUTRAL = 4;

// Chord index = (fwd + 1) * 3 + (side + 1), side +1 = right key.
inline int ChordFwd(int chord)
{
    return chord / 3 - 1;
}

inline int ChordSide(int chord)
{
    return chord % 3 - 1;
}

inline int MakeChord(int fwd, int side)
{
    return (fwd + 1) * 3 + (side + 1);
}

// A dense float table with explicit dimensions (row-major).
struct Table {
    std::vector<int>   dims;
    std::vector<float> v;

    void Resize(std::initializer_list<int> d, float fill = 0.0f)
    {
        dims.assign(d);
        size_t n = 1;
        for (int x : dims) {
            n *= static_cast<size_t>(x);
        }
        v.assign(n, fill);
    }

    size_t Index(int a, int b = 0, int c = 0, int d = 0, int e = 0) const
    {
        const int idx[5] = {a, b, c, d, e};
        size_t    off    = 0;
        for (size_t i = 0; i < dims.size(); i++) {
            off = off * static_cast<size_t>(dims[i]) + static_cast<size_t>(idx[i]);
        }
        return off;
    }

    float At(int a, int b = 0, int c = 0, int d = 0, int e = 0) const { return v[Index(a, b, c, d, e)]; }
    float& At(int a, int b = 0, int c = 0, int d = 0, int e = 0) { return v[Index(a, b, c, d, e)]; }
};

struct StanceKeyModel {
    std::vector<float> pressHazard;      // per context, per tick while released
    std::vector<float> holdPmf;          // hold length in ticks (index 0 = 1 tick), complete holds only
    std::vector<int>   releaseAgeEdges;  // lower edges of hold-age bins, ticks
    std::vector<float> releaseHazard;    // per tick while held, by hold age (survival estimate)
    // Toggle keys (crouch): pressHazard applies while standing; a press while crouched stands up,
    // with this hazard by ticks crouched. Empty for keys that act while held.
    std::vector<int>   upAgeEdges;
    std::vector<float> upHazard;
};

// One movement key (the side key or the forward key) as a semi-Markov process.
struct KeyModel {
    Table              switchLogit;    // side: [ctx][side+1][fwd+1][age bin]; forward: [ctx][fwd+1][side+1][age bin]
    std::vector<float> wallLogit;      // by clearance bin in the key's direction (last = open)
    std::vector<float> diagWallLogit;  // while both keys are held: by clearance bin along the diagonal (empty = none)
    std::vector<float> choiceWallLogit; // what a change goes to: by clearance bin of the chord it makes (empty = none)
    float              losChangeLogit = 0.0f;
    Table              ctxChangeLogit; // side: [ctx][strafing][ctx age bin]; forward: [ctx][fwd+1][ctx age bin]
};

struct MovementModel {
    std::vector<int>   ageEdges;     // lower edges of key-age bins, ticks
    std::vector<float> clearEdges;   // clearance bins in the key direction, units
    std::vector<float> distEdges;
    std::vector<int>   ctxAgeEdges;  // ticks since the context changed; the last bin is the reference
    KeyModel           side;
    KeyModel           fwd;
    Table              reverseP;     // [ctx][fwd+1][side age bin]: a strafe that ends reverses (else lets go)
    Table              rightP;       // [ctx][fwd+1]: from no strafe, right (else left)
    Table              fwdNext;      // [ctx][from+1][side+1][to+1] logits of the forward key's next state
    Table              approach;     // [ctx][distance bin]: logit per unit of approach (to - from) * cos(bearing)
    float              approachEnemyReload = 0.0f;
    float              vetoClearance       = 0.0f;   // walls closer than this block a new key press (0 = off)

    // lean: next[state][ctx][age bin][relation][outcome], see fit_movement.py
    std::vector<int> leanAgeEdges;
    Table            leanNext;
    std::vector<int> leanCtxAgeEdges;
    Table            leanCtxChangeLogit;  // [ctx][state 0 none / 1 leaning][ctx age bin < last]: leave the state
    std::vector<float> leanCtxLogit;      // calibrated per-context shift of leaning (on +, off -)
    std::vector<float> sideCtxLogit;      // calibrated per-context strafe habit (press from neutral +)
    std::vector<float> fwdCtxLogit;       // calibrated per-context forward habit (toward forward +)
    float              reverseLogit = 0.0f;  // calibrated shift of reversing (vs letting go) a strafe
    float              walkMult     = 1.0f;  // calibrated walk press hazard multiplier

    StanceKeyModel crouch;
    StanceKeyModel jump;
    StanceKeyModel walk;

    // closed-loop couplings (calibrated in the arena)
    float navSwitchLogit      = 1.0f;   // switch logit per unit of misalignment x urgency
    float navChoiceLogit      = 1.5f;   // choice logit per unit of alignment x urgency
    float wallPressureLogit   = 2.0f;   // switch logit while pushing into a wall
    // the wall reflex (set by hand, 0 = off): when the held chord's wall is closer than this many ms
    // at the current speed (or touching), a key is let go of (or a strafe added to slide along the
    // wall) with this logit, and no new key presses into a wall the bot touches
    float wallReflexMs        = 0.0f;
    float wallReflexLogit     = 0.0f;
};

struct NoiseModel {
    float medianAbsUnits  = 5.0f;  // lateral size at the target (world units)
    float floorDeg        = 0.2f;  // angular floor
    float tNu             = 6.0f;
    float tScalePerMedian = 1.4f;
    float ar1             = 0.2f;
};

struct YawController {
    float      rho   = 0.36f;
    float      Kp    = 0.23f;
    float      Kself = 0.9f;
    float      Kopp  = 0.13f;
    float      bias  = 0.0f;
    NoiseModel noise;
};

struct PitchController {
    float      rho  = 0.63f;
    float      Kp   = 0.05f;
    float      Kt   = 0.07f;
    float      bias = 0.0f;
    NoiseModel noise;
};

struct MainSequenceRow {
    float ampLo      = 0.0f;
    float ampMed     = 0.0f;
    float ticksMed   = 1.0f;
    float ticksSigma = 0.3f;
};

struct ViewModel {
    YawController   firing;
    YawController   idle;
    PitchController pitchFiring;
    PitchController pitchIdle;
    float           flickDeg = 12.0f;

    std::vector<float> stillEnter;  // per context, on the move
    std::vector<float> stillStay;   // per context, on the move
    std::vector<float> stillEnterStanding;  // per context, standing (empty: as on the move)
    std::vector<float> stillStayStanding;
    float              standingSpeed = 1.0f;  // u/s: slower is standing

    std::vector<MainSequenceRow> mainSequence;
    float                        flickGainMedian = 0.92f;
    float                        flickGainSigma  = 0.25f;

    float aimHeightIdle   = 0.66f;
    float aimHeightFiring = 0.44f;
    float aimHeightSd     = 0.08f;

    // calibrated
    float noiseScale         = 1.0f;
    float biasScale          = 0.0f;
    float pitchOffsetFiring  = 0.0f;   // degrees added to the aim pitch (+ = lower), firing
    float pitchOffsetIdle    = 0.0f;
    float hiddenNoiseScale   = 1.0f;   // view noise x this without a tracked enemy
    float trackGainScale     = 1.0f;   // yaw error and target-motion gains x this while tracking (attenuated fit)
    float beliefFollowDeg    = 0.0f;   // a belief look re-aims once the believed position moved this far
    float hiddenReaimDeg     = 0.0f;   // without a visible enemy, a view this far off its look target re-aims
    float hiddenReaimHazard  = 0.0f;   //   with a quick turn (a saccade), this chance per tick (0 = off)
    float stillLogit[CTX_COUNT] = {};  // per-context shift of the still gate (enter and stay)
    float pitchGainScale     = 1.0f;   // pitch error gain x the fitted one (the fit is attenuated: people's
                                       // intended aim height varies, the regression sees it as error)
    int   flickRefractoryTicks = 2;
    float acquireMinHalfW    = 1.5f;   // corrective saccade when the error exceeds this many half-widths
    float acquireHazard      = 0.35f;  // per tick once detected
    float trackFlickHazard   = 0.25f;  // per tick while |err| > flickDeg and tracking
    float lookaroundPerMin   = 6.0f;   // look-arounds (a turn away and back) per minute without a visible enemy
    float beliefLookShare    = 0.6f;   // of hidden look decisions with a focused belief: watch the believed position
    float preaimShare        = 0.3f;   // of hidden look decisions: watch the corner it will come out of
    // corner pre-aim: the crosshair waits on the edge of cover the believed enemy would come out from,
    // this far onto the cover side and below the line to its head (people: 1.8 and 2.7 deg, REPORT 14)
    float preaimCoverDeg     = 1.8f;
    float preaimBelowDeg     = 2.7f;
    float preaimHazard       = 0.0f;   // per tick x the imminence of the exposures: turn onto a corner (0 = off)
    float preaimWeight       = 0.0f;   // a look decision's weight of the corners, per unit of imminence
    float preaimHorizonMs    = 1000.0f;  // an exposure weighs exp(-eta / horizon) in the imminence (style x)
    float preaimFlickDeg     = 3.0f;   // a corner is turned onto in one flick from this far off: a deliberate
                                       // turn like people's, not the idle controller's drift
    float preaimSelfComp     = 1.0f;   // share of the bot's own motion the view takes out on a corner the enemy
                                       //   is expected out of now (the fitted share when none is expected soon)
    float travelShare        = 0.25f;
    float travelFollowDeg    = 15.0f;  // a route look re-aims once the route turned this far from it
    float routeTurnHazard    = 0.0f;   // per tick on the move: turn to a route that lies behind the view (0 = off)
    float preaimPassDps      = 0.0f;   // on the move, a corner whose direction the bot's own motion sweeps faster than
                                       //   this (deg/s) is passed, not watched (0 = off)
    float respawnRelook      = 0.0f;   // 1: a new look decision when the belief of a dead enemy comes back (its respawn)
    float preaimFollowGeom   = 0.0f;   // 1: a watched corner is followed by where it is, whichever exposure offers it
                                       //   (0: by its exposure cell)
    float beliefLookCorner   = 0.0f;   // 1: a belief look watches the corner nearest the believed position's
                                       //   direction when there is one (0: the believed position)
    float lostAimLastSeen    = 0.0f;   // 1: right after losing sight the view holds where the enemy was last seen
                                       //   (0: it follows the believed position on)
    float lookDwellMedianMs  = 900.0f;
    float lookDwellSigma     = 0.6f;
    float damageTurnDelayMs  = 100.0f;
};

struct TriggerSide {
    float              bias    = 0.0f;
    std::vector<float> err;
    std::vector<float> lage;
    std::vector<float> age;
    float              damaged = 0.0f;
};

struct TriggerModel {
    std::vector<float> enEdges;
    std::vector<float> yawEdges;
    std::vector<float> lageLosEdges;
    std::vector<float> lageHiddenEdges;
    std::vector<int>   holdEdges;
    std::vector<int>   gapEdges;
    TriggerSide        pressLos;
    TriggerSide        releaseLos;
    TriggerSide        pressHidden;
    TriggerSide        releaseHidden;

    // calibrated
    float anticipationLogit = 1.0f;   // hidden press, when an exposure is predicted within ~300 ms
    float hiddenFireLogit   = 0.0f;   // overall shift of the hidden press hazard
    // Calibrated fade of the hidden press with the time since sight (HiddenLateRamp), except when an exposure is
    // anticipated. The fitted hazard takes people's aim error to the true enemy, which grows as they lose track of
    // it; the bot's input is its error to its own belief, which stays small, so without this the bots kept firing
    // into cover long after sight.
    float hiddenLateLogit   = 0.0f;
    float pressLosLogit     = 0.0f;   // calibrated shift of the press hazard with LOS
    float releaseLosLogit   = 0.0f;   // calibrated shift of the release hazard with LOS
    // Calibrated tilts of the release with LOS. The fitted release by aim error comes out too flat in
    // closed loop (the bot let go near the target and sprayed far off it, and tapped too little);
    // in the game's spread every round of a spray widens the next.
    float releaseNearLogit  = 0.0f;   // aim error below RELEASE_FAR_HALF_WIDTHS
    float releaseFarLogit   = 0.0f;   // aim error from RELEASE_FAR_HALF_WIDTHS
    float releaseTapLogit   = 0.0f;   // the first two ticks of a hold (a tap)
};

constexpr float RELEASE_FAR_HALF_WIDTHS = 6.0f;
// hiddenLateLogit ramps in linearly in log time: none up to 500 ms since sight, in full from 8 s
constexpr float HIDDEN_LATE_FROM_MS = 500.0f;
constexpr float HIDDEN_LATE_FULL_MS = 8000.0f;

struct WeaponModel {
    std::vector<int>    postKillRoundEdges;  // lower edges of rounds-left bins
    std::vector<float>  postKillReloadP;     // P(reload within 3 s)
    std::vector<double> postKillDelayProbs;
    std::vector<double> postKillDelayMs;
    float               tacticalHazard     = 0.004f;  // per tick, enemy alive and hidden, clip below tacticalClipFrac
    float               tacticalClipFrac   = 0.5f;
    float               pistolSwitchPerMin = 0.1f;
    std::vector<double> respawnProbs;
    std::vector<double> respawnMs;
};

struct PerceptionModel {
    float detectRate       = 1.2f;    // hazard rate per tick for a full body at the view centre
    float eccScaleDeg      = 18.0f;   // rate falls as exp(-eccentricity / scale)
    float distScale        = 1400.0f; // rate falls as exp(-distance / scale)
    float partExponent     = 0.7f;    // rate ~ (visible parts / 6)^exponent
    int   lossMemoryTicks  = 3;       // keep a lost target this long
    float gunfireSigmaDeg  = 10.0f;
    float gunfireRange     = 3000.0f;
    float footstepSigmaDeg = 20.0f;
    float footstepRange    = 1000.0f;
    float frontBackConfusion = 0.25f;
    float reloadRange      = 600.0f;
    float reloadSigmaDeg   = 15.0f;
    float distanceLogSd    = 0.35f;
    float damageSigmaDeg   = 20.0f;
};

struct BeliefModel {
    int   particles       = 256;
    float negDetect       = 0.85f;  // P(seen | enemy in view and in LOS)
    float essResample     = 0.5f;
    float jitter          = 10.0f;  // units, applied at resampling
    float moveBoost       = 1.0f;   // multiplies the prior leave probability
    float soundSigmaScale = 1.0f;
    float spawnMinDist    = 256.0f;
    float momentum        = 1.5f;   // preference for keeping the previous move direction
    float injectMax       = 0.5f;   // share of particles re-drawn from a surprising sound or hit
};

struct NavModel {
    float holdHazard        = 0.02f;  // per tick when hunting with a concentrated belief
    float holdMedianMs      = 1500.0f;
    float holdSigma         = 0.7f;
    float spawnPushMs       = 2500.0f;
    float huntUrgency       = 0.8f;
    float engageUrgency     = 0.25f;  // toward the enemy for the second after losing sight (none while it is perceived)
    float reloadUrgency     = 0.4f;
    float waypointReach     = 48.0f;
    float repathMs          = 1000.0f;
    float postKillMs        = 1500.0f;  // after a kill the bot stays where it is this long (urgency ~0)
};

struct PresentationModel {
    float pingMedianMs = 45.0f;
    float pingSigma    = 0.45f;
    float pingDriftAr  = 0.995f;
    float pingJitterMs = 4.0f;
    float joinDelayMedianMs = 6000.0f;
    float joinDelaySigma    = 0.6f;
};

// How a life starts (fitted on the first ticks after each respawn).
struct SpawnModel {
    std::vector<float> deadTicksPmf;   // ticks of empty usercmds after the respawn (index = ticks)
    std::vector<float> chordP;         // keys held at the first live tick, by chord
    std::vector<int>   ageEdges;       // ticks since the first live tick; the last edge ends the spawn run
    Table              sideSwitchP;    // [strafing][age bin]: first run of the side key
    Table              fwdSwitchP;     // [fwd+1][age bin]: first run of the forward key
    float              clickFirstP = 0.0f;       // the respawn click still held at the first live tick
    std::vector<float> clickStayP;     // P(attack at live tick t | attack at t-1), t = 1..n
    std::vector<float> clickPressP;    // P(attack at live tick t | none at t-1)
};

struct SharedModel {
    int                 version = 0;
    SpawnModel          spawn;
    MovementModel       movement;
    ViewModel           view;
    TriggerModel        trigger;
    WeaponModel         weapon;
    PerceptionModel     perception;
    BeliefModel         belief;
    NavModel            nav;
    PresentationModel   presentation;
};

//
// Style distribution (anonymous) and the dial calibration curves
//
enum Dial {
    DIAL_FWD_DIAG,
    DIAL_REVERSE,
    DIAL_SIDE_HOLD,
    DIAL_LEAN,
    DIAL_JUMPS,
    DIAL_CROUCH,
    DIAL_WALK,
    DIAL_BURST,
    DIAL_AIM_HEIGHT,
    DIAL_HOLD_ANGLE,   // hold or clear an angle: crosshair parked on where the enemy will appear, 500 ms early
    DIAL_COUNT
};

enum SkillDial {
    SKILL_AIM_ERROR,
    SKILL_REACTION,
    SKILL_COUNT
};

enum Family {
    FAMILY_PRESSER,
    FAMILY_STRAFER,
    FAMILY_STOPPER,
    FAMILY_COUNT
};

extern const char *const DIAL_NAMES[DIAL_COUNT];
extern const char *const SKILL_NAMES[SKILL_COUNT];
extern const char *const FAMILY_NAMES[FAMILY_COUNT];

struct FamilyDist {
    std::string name;
    float       weight = 0.0f;
    float       centre[DIAL_COUNT] = {};
    float       spread[DIAL_COUNT] = {};
};

struct WeaponMixComponent {
    float weight = 0.0f;
    float mean   = 0.5f;
    float sd     = 0.1f;
};

struct StyleModel {
    FamilyDist                      families[FAMILY_COUNT];
    float                           dialMin[DIAL_COUNT]   = {};
    float                           dialMax[DIAL_COUNT]   = {};
    float                           skillMin[SKILL_COUNT] = {};
    float                           skillMax[SKILL_COUNT] = {};
    float                           pooled[DIAL_COUNT]    = {};
    float                           pooledSkill[SKILL_COUNT] = {};
    std::vector<WeaponMixComponent> weaponMix;
};

// Monotone curve from a dial target to an internal offset.
struct Curve {
    std::vector<float> x;
    std::vector<float> y;

    bool  Valid() const { return x.size() >= 2 && x.size() == y.size(); }
    float Eval(float t) const;
};

struct Calibration {
    Curve dial[DIAL_COUNT];
    Curve skill[SKILL_COUNT];
};

struct ModelBundle {
    SharedModel  shared;
    StyleModel   style;
    Calibration  calib;
    std::string  sha256;
    std::string  source;  // "embedded" or the override directory
};

} // namespace hb
