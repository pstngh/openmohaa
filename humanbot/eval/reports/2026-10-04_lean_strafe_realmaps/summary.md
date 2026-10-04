# The lean follows the strafe, on the practice maps (2026-10-04)

The owner, watching two bots play each other on the test server, saw them hold a lean to one side for too long,
and lean into a wall for a moment as a result. The recording of that session and the five recorded people showed
what was wrong: when a bot's strafe turned the other way, its lean did not do what that player's style does. Numbers
are bot against bot on the four practice maps (8 servers, 2 bots each, about 40 game-minutes each; seeds 201-208, in
brackets the held-out seeds 401-408), the bots' style families weighted like the people's minutes. `report.md` is
the full `compare.py` report of seeds 201-208. "Before" is `119a80d2` (model `7ab48ea75e87`), "now" is model
`697feb684ecf`.

## In simple terms

When people strafe one way while leaning the other, the moment the strafe turns is where they decide, and each
style decides its own way:

- **The presser** (the owner's style, half the recorded minutes) flips the lean across with the strafe about half
  the time. The presser bots almost never did (6-8%): they kept leaning the old way, against their own movement, for
  a third of their leaning time, and a tenth of their leans lasted over 2 seconds. Now they flip it as often as
  people (49-52%), lean against their strafe as rarely (14-15.5% of the time, people 15.5%), and their leans last
  as long as people's (nine in ten under a second).
- **The strafers** let go of the lean instead, and almost never flip it. The strafer bots flipped it a quarter of
  the time, so they rocked from one side to the other; now they flip it 5% of the time and let go of it 21-25%
  (people 36%: the strafer people let go most of all mid-fight, which a single style setting on a lean shared by
  everyone cannot fully give them).
- **The stoppers** mostly keep the lean (85%), and the stopper bots now do too (85-86.5%).

Two new style settings, measured on each recorded person, carry this: how readily a lean flips across when the
strafe turns against it, and how readily it is let go then. The lean itself now knows the moment the strafe turns,
learned from the recordings. The five things the owner saw on 2026-10-03 stay where they were.

## The numbers

| | people | before | now |
|---|---|---|---|
| presser bots' leans: nine in ten end within / last over 2 s | 950 ms / 1.3% | 1900 ms / 9.2% (1750 / 7.5%) | 900 ms / 1.9% (990 / 2.0%) |
| ... leaning against their own strafe, share of lean time | 15.5% | 31% (30%) | 14% (15.5%) |
| the strafe turns against the lean: a presser flips it across | 46% | 6.5% (8%) | 52% (49%) |
| ... a strafer lets go of it / flips it | 36% / 0.2% | 12% / 27% (10% / 25%) | 21% / 4.5% (25% / 5%) |
| ... a stopper keeps it | 85% | 85% (83%) | 86.5% (85%) |
| all leans: nine in ten end within / last over 2 s | 1140 ms / 2.9% | 1525 ms / 6.2% (1480 / 5.6%) | 1170 ms / 3.6% (1200 / 3.5%) |
| leans in sight at a flat wall: into it / at an edge just ahead: around it | 35% / 77% | 35% / 63% (34% / 63%) | 31% / 66% (31% / 64%) |
| crouches a minute with the enemy hidden: quiet / after enemy fire | 2.3 / 5.4 | 2.9 / 5.7 (2.6 / 5.6) | 2.9 / 6.1 (2.7 / 5.7) |
| enemy heard behind: faced within 1 s / hit first | 62% / 7% | 53% / 12% (49% / 15%) | 52.5% / 13% (50% / 16%) |
| reloads begun with the enemy on screen, a minute alive | 0.3 | 1.6 (1.3) | 1.7 (1.3) |
| statistics within 25% of people (all 281) | | 55% (51%) | 56% (52%) |

## Not changed yet

Leans with the head right at a wall take as much of the bots' leaning time as before (6.3-7.6%, people 6.3%), and
they are of a different kind from people's: people's mostly come from strafing into the wall with the lean, the
bots' mostly from stopping at a wall with the lean still on. The stopper bots' leans last a little longer than
their people's now that they no longer end them by flipping (nine in ten end within 1.9 s, people 1.6 s).
