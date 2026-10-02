# Encounters (2026-10-02)

Why the bots met the enemy half as often as people, and what changed. The numbers are bot against bot
on the four practice maps (8 servers, 2 bots each, about 40 game-minutes each; seeds 201-208, in
brackets the held-out seeds 401-408), against the five recorded people. `report.md` is the full
`compare.py` report of seeds 201-208. "Before" is `9ece1afc`, re-scored with the new statistics.

## In simple terms

1. **The bots were close to each other, but with a wall in between.** With the enemy hidden they
   were nearer to it than people are, yet had a clear line to it half as often. They were not lost
   on the map; they were stuck on the wrong side of a corner.
2. **They hardly went anywhere.** A person whose enemy is out of sight runs: in 2 seconds they get
   about 160 units further. The bots got about 65: they strafed left and right on the spot and
   stopped often. Two reasons. Their keys only loosely followed the route they wanted to take. And
   they kept looking at where they believed the enemy was, straight through the wall in front of
   them, so walking forward meant walking into that wall; the bot let go of the key and sidestepped
   along the corridor instead.
3. **They did not chase.** When the enemy ducked out of sight, people went after it (closing in at
   about 30 units a second in the first second); the bots stayed put. So a lost contact often stayed
   lost for seconds.
4. **After a kill they stood around** for 1.5 seconds and then walked; people run off at once.
5. **A bug made them forget where a killed enemy comes back.** The bot's eyes dropped a dead enemy
   from what it sees, so its memory of that enemy was thrown away. When the enemy respawned, the bot
   guessed "anywhere on the map" instead of "at the spawn point the game picks", and looked the wrong
   way: 50 degrees off where the enemy really was, half a second after it respawned (people 22).
6. **Peeking was not the problem.** The bots show only part of their body (a peek) about as often,
   per second the enemy is in view, as people do. They simply saw each other less.

Now the bots' keys follow their route about three times as strongly, they chase for one second
after losing sight (not while they see the enemy: that made every bot fight like the most aggressive
style), they move on half a second after a kill, a killed enemy is expected back at the spawn the
game would pick, and on the move the view turns to a route behind it (instead of the bot walking
backwards) and looks down the corridor rather than at the next corner of the path.

## Before and after

| | people | before | now |
|---|---|---|---|
| sightings (a body part coming on screen) per minute of duel time | 33 | 15 (16) | 28 (28) |
| duel time with a body part on screen | 34% | 15% (16%) | 33% (33%) |
| ... only parts of the body (the centre hidden) | 10.5% | 3.8% (4.0%) | 6.3% (6.1%) |
| hide between two sightings, median / 90th percentile | 0.4 / 1.75 s | 0.5 / 5.0 s (0.55 / 4.8 s) | 0.35 / 1.8 s (0.35 / 1.7 s) |
| from both alive to the first sighting, median | 1.35 s | 3.7 s (3.4 s) | 1.9 s (1.85 s) |
| ground covered in 2 s with the enemy out of sight, median | 158 u | 64 u (70 u) | 103 u (101 u) |
| closing speed in the first second after losing sight | 31 u/s | 8 u/s (6 u/s) | 34 u/s (31 u/s) |
| speed 0.5-1.5 s after a kill, median | 236 u/s | 124 u/s (124 u/s) | 144 u/s (144 u/s) |
| view to a respawned enemy, 0.5-1 s after its respawn | 22 deg | 50 deg (50 deg) | 38 deg (36 deg) |
| life length, median | 7.1 s | 23.5 s (23.1 s) | 12.4 s (12.4 s) |
| accuracy | 19% | 9.9% (9.9%) | 15.3% (14.7%) |
| duel time in a firefight with the enemy in sight | 17.6% | 7.0% (7.8%) | 18% (18%) |
| fire held with no body part visible | 17.3% | 11.3% (11.9%) | 15.2% (15.7%) |
| the 9 encounter statistics within 25% | | 1 (0) | 6 (6) |
| other 219 statistics within 25% (median relative error) | | 48% (0.27) (50%, 0.25) | 50% (0.25) (49%, 0.27) |
| pre-aim statistics within 25% (of 44) | | 48% (0.26) (41%, 0.27) | 52% (0.23) (55%, 0.23) |

In the arena (two average bots, eight seeds of 900 s): 58% of 195 statistics within 25% (median
relative error 0.18), against 60% (0.18) before; the arena is open, and there the stronger route
following makes the bots cover more ground than people (225 u in 2 s vs 158).

## What is still off

- **28 sightings a minute, not 33.** The bots still cover less ground while the enemy is hidden (103
  u in 2 s vs 158) and take longer to find each other after a respawn (1.9 s vs 1.35). People run on
  the forward diagonal with their view about 40 degrees off their path; the bots, near walls, press
  forward alone or strafe.
- **The view turns too much while the enemy is hidden** (median 20 degrees a second vs 8; 10
  before): the bots move more and keep their aim on corners as they move.
- **In a firefight the bots back off twice as often as people and push half as often.** In the open
  arena they do as people: it is the maps. A pull toward the enemy in sight fixed it, but it made
  every bot press forward like the "presser" style, so it was left out.
- **The view 1-4 s after an enemy respawned** is still 39-49 degrees off it (people 17), although
  the bot's belief is within 7 degrees: the view is on corners.
- **dm/main's lower room:** people spend 14% of their time there (it has a spawn point); the bots
  almost never go there.

Details, the method, what was tried and dropped, and the new statistics: `humanbot/HANDOFF.md`,
"Encounters".
