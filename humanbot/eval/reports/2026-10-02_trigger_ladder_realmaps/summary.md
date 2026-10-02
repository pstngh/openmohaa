# Fire into cover, bursts and ladders (2026-10-02)

What changed and why, for the four open problems of `6c4fe13b`. The numbers are bot against bot on the
four practice maps (8 servers, 2 bots each, about 40 game-minutes each; seeds 201-208, in brackets the
held-out seeds 401-408), against the five recorded people. `report.md` is the full `compare.py` report
of seeds 201-208.

## In simple terms

1. **Bots hanging on the dm/vents ladder.** Two bots could block each other on the ladder: one
   climbing up, the other down, and the game lets neither pass. Once a bot walked into the ladder shaft
   from the top and fell onto the head of the one climbing: the climber could not go up through it,
   and the one on top would not step off because every step was a long drop. That lasted 17 minutes.
   The stock ladder code also climbed backwards (back key, looking down). Now the bot climbs as people
   do (forward, looking up the ladder or down it), and if it stops moving for 1.5-3 s it turns back;
   if it is stuck again it jumps off.
2. **Short bursts.** A burst the bot starts while it sees the enemy was already about people's length
   (5 rounds vs 6). The median was low because most of the bots' bursts were short blind bursts into
   cover: the bots meet each other half as often as people do. The burst-length style dial now
   measures and changes only the bursts begun in sight, and it works per bot again.
3. **The arena check that dropped from 58% to 55%.** Mostly chance: four seeds are too few. With eight
   seeds, both versions score 59%. What really changed was a little less fire into cover, measured on a
   different definition of "hidden" (the centre of the body rather than any body part).
4. **Firing into cover long after seeing the enemy.** People lose track of the enemy after a few
   seconds and stop firing blind. The bot's belief keeps tracking the enemy, and the trigger took that
   as "aimed at the enemy", so it kept firing. The blind fire now fades with the time since the enemy
   was last seen, as people's does, except when the bot expects the enemy to appear in its crosshair
   right now (people pre-fire corners).

## Before and after

| | people | before (`6c4fe13b`) | now |
|---|---|---|---|
| fire held into cover, more than 5 s after the enemy was last on screen | 4.3% | 12% | 5.7% (6.4%) |
| ... 2-5 s after | 7.7% | 16% | 9.2% (9.1%) |
| ... 1-2 s after | 20% | 27% | 21% (22%) |
| ... in the first second | 43% / 26% | 47% / 31% | 48% / 29% (48% / 29%) |
| ... all of that time | 17.3% | 17.6% | 11.3% (11.9%) |
| median burst, all / begun in sight (rounds) | 5 / 6 | 3 / 5 | 4 / 5 (3 / 5) |
| taps of the holds begun in sight | 15% | 19% | 14% (17%) |
| burst dial, realised against drawn: correlation over 16 bots | | 0.21 | 0.70 (0.79) |
| accuracy | 19% | 8.6% | 9.9% (9.9%) |
| ladder climbs over 10 s (dm/vents) | | 3, up to 12 s (up to 48 s in other runs) | 0 (0) |
| other 219 statistics within 25% (median relative error) | | 52% (0.25) | 48% (0.27) (50%, 0.25) |
| pre-aim statistics within 25% | | 52% (0.23) | 48% (0.26) (41%, 0.27) |

In the arena (two average bots, eight seeds of 900 s): 60% of 195 statistics within 25% (median
relative error 0.18), against 59% (0.19) before.

## What is still off

- **The bots meet half as often** (18 sightings a minute vs 33): people duel by peeking out again and
  again. More of the bots' time is long after a sighting, so their fire into cover over all hidden
  time (11-12%) is below people's (17%) even though it matches at each time since sight, and all of
  their bursts together are shorter.
- **Pre-firing after a long wait:** after 2-5 s with the enemy hidden, the bots already fire at 17% of
  the sightings, people at 26%. That depends on predicting where and when the enemy comes out.
- The share of statistics within 25% fell 2-4 points on the maps, from the same time mix (less time
  firing into cover, more time hidden and still).

Details, the method and the new statistics: `humanbot/HANDOFF.md`, "Fire into cover, bursts and
ladders".
