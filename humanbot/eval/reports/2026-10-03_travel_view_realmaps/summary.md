# Travel and the hidden view on the practice maps (2026-10-03)

Why the bots, with the enemy out of sight, covered less ground than people and turned their view about
twice as much, and what changed. Numbers are bot against bot on the four practice maps (8 servers, 2 bots
each, about 40 game-minutes each; seeds 201-208, in brackets the held-out seeds 401-408), against the five
recorded people, with the bots' style families weighted like the people's minutes. `report.md` is the full
`compare.py` report of seeds 201-208. "Before" is `d014bcd2` (model `510b64f3eb19`), "now" is model
`bc6e339d9af5`.

## In simple terms

1. **The bots did not run into walls more often than people; they reacted to them differently.**
   Bots and people travel equally close to walls and head toward one equally often. People then mostly
   keep their keys and let the game slide them along the wall (at a shallow angle that costs little
   speed). The bots' wall reflex changed keys on most such ticks (64% against people's 25%): a run
   on the forward diagonal often became a sideways strafe, and a strafe into a wall often just
   stopped. So the bots strafed where people run diagonally, zig-zagged, and stood at walls. Now a
   strafe that meets a wall adds the forward key and runs along the wall on the diagonal, which is
   what people do there more often than stopping.
2. **The view followed corners it was only running past.** While the enemy is hidden the bots watch the
   corner it could come out of, and they held that corner exactly against their own motion. Running
   past a nearby corner, its direction swings round fast, so the view swung with it: a third of the
   corner-watching time was on corners within 120 units, turning at 33 degrees a second. Now a corner
   whose direction the bot's own running sweeps past faster than 90 degrees a second is let go: the
   view keeps its direction.
3. **The view turned round to the route too often.** When the route the bot follows ended up behind its
   view, it turned to it, a median 117-degree turn, 11 times a minute; people make 4 to 5 turns of 90
   degrees or more a minute in all. That turn now comes a quarter as readily.
4. **The believed enemy position flickered.** With several places the enemy could be, the "most likely"
   one hopped between them (jumps of more than 200 units 39 times a minute, 42% of them back within 2
   seconds). It now stays put unless another place holds clearly more of the belief (24-27 such jumps a
   minute).
5. **The calibration of the view's hand jitter was dropped.** It matched the view's turning speed in the
   test arena, which has no corners, by adding jitter; after point 3 the arena view was calmer and the
   loop added half again as much jitter, which on the real maps made the view livelier. The jitter stays
   at its last calibrated value.

## Before and after

| | people | before | now |
|---|---|---|---|
| ground covered in 2 s with no part on screen, median | 158 u | 116 u (128 u) | 137 u (147 u) |
| ... path walked in 2 s / how straight (net / path) | 338 u / 0.56 | 275 / 0.49 (291 / 0.49) | 297 / 0.51 (309 / 0.53) |
| hidden, not firing: forward diagonal / forward alone / strafe | 30% / 12% / 26% | 23% / 13% / 32% (27% / 13% / 32%) | 28% / 14% / 30% (30% / 14% / 29%) |
| duel time touching a wall | 9.1% | 9.3% (8.3%) | 9.4% (7.9%) |
| sightings per minute / duel time with a part on screen | 33.3 / 34% | 31.1 / 38% (28.9 / 35%) | 32.1 / 40% (29.7 / 38%) |
| firefight at 224-288 u: approach / back off | 38% / 5.3% | 24% / 10.4% (32% / 10.8%) | 37% / 9.9% (46% / 9.4%) |
| hidden view: yaw speed p50 / p99 | 8.1 / 544 deg/s | 16.3 / 766 (17.0 / 725) | 11.5 / 677 (11.8 / 666) |
| hidden view: turns of 10 deg or more per minute (of 90 deg or more) | 46 (4.6) | 71 (18) (73 (17)) | 54 (11) (55 (10)) |
| crosshair from the corner 500 ms before the first part | 5.8 deg | 10.6 (10.7) | 11.0 (11.7) |
| crosshair closer to the corner than to the enemy, 500 ms before | 80% | 58% (58%) | 53% (53%) |
| view turn toward the enemy, last 500 ms before the first part | 13 deg | 10.1 (9.2) | 6.9 (7.6) |
| aim error at the first visible part | 5.2 deg | 10.3 (11.2) | 11.5 (12.1) |
| other 219 statistics within 25% (median relative error) | | 58% (0.20) (58%, 0.20) | 58% (0.19) (54%, 0.22) |
| all 281 statistics within 25% | | 59% (57%) | 57% (53%) |

The style dials are recovered as before (fight diagonal: drawn against realised correlation 0.87 (0.95)),
no bot stayed stuck for 1.5 s, and long pushes against a wall became rarer (0.09-0.10 per bot-minute,
0.12-0.14 before).

In the arena (two average bots, eight seeds of 900 s): 51% of 195 statistics within 25% (median relative
error 0.24), against 55% (0.22) before. Around the arena's open pillars a strafe now runs on along a
pillar, so in fights the average bot takes the diagonal and approaches more than people; on the recorded
maps the same change brought the fights to people's level (approach 37-46% against 38%).

## The cost

Letting go of corners the bot runs past is what brings the hidden view down; without it the view turns
as much as before, because the bots now move more. It costs anticipation: in the last half second before
the enemy appears the bots' view turns toward it 7-7.5 degrees against 9-10 before (people 13), and they
are closer to the corner than to the enemy at 53% of sightings against 58% (people 80%). The share of the
44 pre-aim statistics within 25% reads 45% (39%) against 57% (48%) before, but that share moves by 9
points between two captures of the same model on the same seeds (a re-run of "before" on seeds 201-208
gave 48%).

## What is still off

- **Ground covered:** 137-147 u against 158. The bots still change keys at walls more than twice as often
  as people (they now mostly turn onto the diagonal rather than stop), and they stand next to walls with
  no key held 5% of their hidden time against people's 3%.
- **The hidden view:** 11.5-11.8 degrees a second against 8, and 10-11 turns of 90 degrees or more a
  minute against 4.6 (turns to a route behind the view, corners coming up, look decisions and
  look-arounds).
- **Standing:** the bots stand 14-15% of their hidden time against people's 22.5%.
- **Fights on the held-out seeds:** the bots now approach a little more than people and hold the forward
  diagonal 40% of the time against 31%.

Details, the method and what was tried and dropped: `humanbot/HANDOFF.md`, "Travel and the hidden view".
