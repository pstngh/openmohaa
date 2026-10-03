# Movement on the practice maps (2026-10-02)

Why the bots moved like people in the open test arena but not on the four practice maps, and what
changed. Numbers are bot against bot on the practice maps (8 servers, 2 bots each, about 40
game-minutes each; seeds 201-208, in brackets the held-out seeds 401-408), against the five recorded
people. `report.md` is the full `compare.py` report of seeds 201-208. "Before" is `9d93d470` (model
`ea388d30bdbc`), re-scored with this change's family weighting (point 1); "now" is model `510b64f3eb19`.

## In simple terms

1. **Part of the gap was in the scoring, not in the bots.** One of the five people played half of all
   recorded minutes, and that person is the "presser" style (runs at the enemy on the forward
   diagonal). The human numbers are averaged over minutes, so they are half presser. The report
   weighted the bots by style the way they are drawn in the game: each person alike, so only a fifth
   presser. Compared with people of their own style, the strafer and stopper bots approached in
   fights as often as people (13-16% vs 15-16%); "approach 18-22% vs 38%" was mostly this mismatch.
   The report now weights the bots like the people (half presser).
2. **The diagonal style dial pushed bots backwards.** Each bot draws how much it uses the forward
   diagonal in a fight. For the low-diagonal styles (four bots in five) the dial lowered the chance
   that a bot pressing its forward key while strafing chose "forward", and the only other choice
   was "back". So those bots backed off twice as often as people of their style. The dial now makes
   such a bot press forward less often, without pressing back more often.
3. **The wall reflex fought the way people walk corridors.** People travel a corridor on the forward
   diagonal with the view about 40 degrees off the path. The forward direction then points at one
   wall and the sideways direction at the other, while the diagonal itself runs down the corridor.
   The bots' wall reflex let go of a key whenever *either* of those directions would reach a wall
   within 0.3 s, so a bot on the diagonal let go of a key at once: it zig-zagged, strafed, or stopped
   at the wall. Measured the same way, people barely react when the way they go reaches a wall that
   soon (they keep their keys on 70-82% of such ticks; the bots on a diagonal or a strafe 26-44%).
   When people do head into a wall they slide along it (forward becomes a diagonal); the bots
   stopped. Now the reflex looks only where the bot is actually going, lets go of
   the key that leaves the more open direction (a slide), and turns "forward into a wall" into the
   open forward diagonal. The bots also touch walls less than before: as often as people.
4. **The hidden view turned too much, for two reasons.** People who stand still mostly keep the mouse
   still too (75% of such moments); the bots' "mouse still" habit ignored whether the bot was moving
   (40%). And while waiting on a corner the bots held it exactly, against their own motion, even when
   running past a nearby corner (corners are a median 170 units away and sweep past at about 22
   degrees a second). Now the mouse-still habit depends on whether the bot stands, and a corner is
   held exactly only when the enemy is expected out of it soon; otherwise the view takes out as much
   of its own motion as people do. The route look also re-aims less often.

## Before and after

| | people | before | now |
|---|---|---|---|
| hidden, not firing: forward diagonal held | 29.9% | 16.0% (16.6%) | 23.4% (27.0%) |
| ... forward alone | 11.9% | 17.1% (17.5%) | 12.9% (12.9%) |
| ... strafe alone | 26.0% | 31.3% (31.3%) | 32.2% (31.7%) |
| duel time touching a wall | 9.1% | 13.8% (12.2%) | 9.3% (8.3%) |
| hidden time standing next to a wall (< 16 u, < 50 u/s) | 6.7% | 15.0% (14.1%) | 10.0% (9.6%) |
| ground covered in 2 s with no part on screen, median | 158 u | 104 u (111 u) | 116 u (128 u) |
| sightings per minute of duel time | 33.3 | 29.7 (29.0) | 31.1 (28.9) |
| duel time with a part on screen | 34% | 35% (35%) | 38% (35%) |
| firefight at 224-288 u: approach / back off | 38% / 5.3% | 22% / 12.7% (30% / 12.0%) | 24% / 10.4% (32% / 10.8%) |
| firefight at 288-384 u: approach / back off | 39% / 5.9% | 23% / 8.8% (31% / 9.8%) | 26% / 7.9% (32% / 7.7%) |
| firefight: back key held | 5.2% | 6.0% (5.7%) | 5.4% (4.5%) |
| hidden view: yaw speed p50 / p99 | 8.1 / 544 deg/s | 20.2 / 851 (21.6 / 830) | 16.3 / 766 (17.0 / 725) |
| hidden view: mouse still / ... while standing | 32% / 74% | 27% / 40% (27% / 41%) | 30% / 60% (30% / 61%) |
| hidden view: turns of 10 deg or more per minute | 46 | 88 (92) | 71 (73) |
| diagonal dial, drawn vs realised over 16 bots (correlation) | | 0.77 (0.96) | 0.87 (0.98) |
| other 219 statistics within 25% (median relative error) | | 51% (0.25) (57%, 0.22) | 58% (0.20) (58%, 0.20) |
| pre-aim statistics within 25% (of 44) | | 55% (0.23) (55%, 0.22) | 57% (0.23) (48%, 0.26) |
| all 281 statistics within 25% | | 52% (0.24) (57%, 0.21) | 59% (0.21) (57%, 0.20) |

With the old weighting (a fifth presser) the same captures read: forward diagonal 13% -> 19%, forward
alone 19% -> 13%, back-off in fights at 160-512 u 13% -> 10.5% (people weighted the same way: 25%,
12%, 7%).

In the arena (two average bots, eight seeds of 900 s): 55% of 195 statistics within 25% (median
relative error 0.22), against 58% (0.18) before. The loss is the open pillars: no longer stopping
at a pillar, the average bot holds the diagonal 39% of hidden time (people 30%) and stands 9% (people
22.5%). The arena's hidden view now matches people's (yaw speed p50 7.4 vs 8.1 deg/s, mouse still 33%
vs 32%); before, its calibration loop sat at its bound with the view at 12.5 deg/s.

## What is still off

- **Ground covered with the enemy hidden: 116-128 u in 2 s vs 158.** The bots' keys still change
  more often than people's near walls: when the direction they go reaches a wall within 0.3 s they let
  go of a key five times as often as people, who turn the mouse instead. Weakening the reflex let the
  bots slide along walls (walls touched 14% of the time) and made them slower, not faster. In the
  2.5 s after a respawn, when the route pull is at full strength, they cover about what people cover;
  a full-strength pull while hunting added only 3-7 u.
- **Backing off in fights at close range** (160-288 u: 10-15% vs 5-7%) is now a tracking issue more than
  a key choice: at those moments the bots' crosshair is a median 12 degrees off the enemy (people 4),
  so a plain strafe drifts away from it; the back key is held as often as people hold it.
- **The hidden view** still turns at 16-17 degrees a second at the median (people 8) and makes 71 turns
  of 10 degrees or more a minute (people 46): the look policy changes its target often.
- **Standing:** the bots stand 15-16% of their hidden time (people 22.5%; 17.5% before, partly by
  stopping at walls).
- **Presser bots** approach and take the diagonal in fights less than the presser (34-48% vs 61%,
  diagonal 30-43% vs 55% at 160-512 u).

Details, the method and what was tried and dropped: `humanbot/HANDOFF.md`, "Movement on the maps".
