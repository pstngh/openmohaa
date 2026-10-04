# What the owner saw, on the practice maps (2026-10-03)

The owner watched two bots play each other on the test server and saw five things that looked wrong. Each was
checked in the recording of that session and against the five recorded people, then fixed where the people
showed a difference. Numbers are bot against bot on the four practice maps (8 servers, 2 bots each, about 40
game-minutes each; seeds 201-208, in brackets the held-out seeds 401-408), with the bots' style families weighted
like the people's minutes. `report.md` is the full `compare.py` report of seeds 201-208. "Before" is `b0e26c91`
(model `872c9ac2f1c4`), "now" is model `7ab48ea75e87`.

## In simple terms

1. **Leaning into the wall at a doorway.** When a wall runs along one side and the other side is open, people
   lean away from the wall: in a fight only a third of their leans there go into it. When the wall ends just
   ahead of them (a door frame, a corner), they lean around it, three times in four. The bots leaned either way
   about half the time, because their lean only followed which way they were strafing. Now the walls beside the
   bot tilt which side it leans to: into a flat wall 35% of the time, like people. Around an edge 63% (people
   77%): edges go by in a tenth of a second, and people are already leaning toward the opening when they reach
   it; the bots turn their lean once they are there.
2. **Strafing without leaning.** This is a real style. The two recorded players of the strafer style lean on
   only about half of their strafing while they shoot (48% and 64%); the owner leans on 94%. The strafer bots
   lean on 63-72%. Nothing changed.
3. **Crouching for no reason.** People crouch mostly in the half second after the enemy fires: three times as
   often as when it is quiet. The bots crouched at the same rate whatever happened. Now they crouch about as
   often as people when it is quiet (2.6-2.9 times a minute, people 2.3) and more right after enemy fire (5.6-5.7,
   people 5.4).
4. **Not turning to an enemy heard behind.** With an enemy running or shooting behind them within earshot,
   people face it within a second in 62% of cases and are hit before they turn in 7%. The bots only did so in
   39% and were hit first in 19%: a sound moved where they believed the enemy was, but never turned the view. Now
   a footstep or shot from behind turns the view toward it (onto the door or corner it would come from, when
   there is one) and the bot keeps looking that way for 3 seconds: 49-53% face it within a second, 12-15% are
   hit first.
5. **Standing still in the middle of a fight.** Overall the bots stand still in fights less than people. What
   the owner saw was a bot that had emptied its gun at the enemy standing in the open while it reloaded (bot3 at
   12:31:50, shot from 79 to 14 health). The bots end up reloading in the enemy's view five times as often as
   people, because their fights in view last longer and the gun runs dry. Three changes, all taken from the
   recordings: with only a few rounds left the bots now hold their fire more, as people do; they reload early
   when the enemy is out of sight, as often as people do by how many rounds are left; and when they let go of a
   strafe they press the other one a moment later, as people do, instead of picking a side at random or
   pausing. Reloads begun in view fell a little (1.9 and 1.4 to 1.6 and 1.3 a minute; people 0.3); the rest
   needs better aim in the first moments of a fight.

## The numbers

| | people | before | now |
|---|---|---|---|
| leans in sight at a flat wall: into it | 35% | 46% (43%) | 35% (34%) |
| ... at an edge just ahead: around it | 77% | 58% (60%) | 63% (63%) |
| crouches a minute with the enemy hidden: quiet / after enemy fire | 2.3 / 5.4 | 3.6 / 4.6 (3.6 / 4.3) | 2.9 / 5.7 (2.6 / 5.6) |
| enemy heard behind: faced within 1 s / hit first | 62% / 7% | 39% / 19% (39% / 20%) | 53% / 12% (49% / 15%) |
| after a strafe is let go, the next one goes the other way (stopper style) | 86% | 57% | 76% (73%) |
| reloads begun with the enemy on screen, a minute alive | 0.3 | 1.9 (1.4) | 1.6 (1.3) |
| view turning with the enemy hidden, median / top 1% | 8.1 / 544 deg/s | 10.7 / 692 (12.1 / 683) | 11.4 / 860 (12.0 / 815) |
| walking backwards with the enemy hidden | 6.1% | 6.0% (5.7%) | 8.8% (7.6%) |
| ground covered in 2 s with the enemy hidden (median) | 158 u | 135 (152) | 122 (135) |
| statistics within 25% of people (all 281) | | 56% (52%) | 55% (51%) |

## What it costs

The turn to a noise behind is a fast one, and while the bot keeps looking back at the noise its route is still
behind it, so it walks backwards a little more and covers less ground while the enemy is hidden. In the arena,
whose small open floor keeps the enemy audible all the time, the view turns noticeably more than before.
