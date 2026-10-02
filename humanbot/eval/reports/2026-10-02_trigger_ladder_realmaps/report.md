# Bot evaluation: 2026-10-02_trigger_ladder_realmaps

Generated 2026-10-02 by `humanbot/eval/compare.py`. Captures: `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`. 8 sessions on dm/crnodoors, dm/downladder, dm/main, dm/vents; capture dates 2026-10-02.
Human reference: `human_reference.json` (2026-10-02), 397 duel minutes, 72 player-sessions, 5 people.

## Bots in this capture

| Bot | Family | Seed | Sessions | Duel minutes |
|---|---|---:|---:|---:|
| `bot:strafer:4` | strafer | 231333596 | 1 | 30.3 |
| `bot:presser:5` | presser | 1152489726 | 1 | 30.3 |
| `bot:stopper:4` | stopper | 711847621 | 1 | 33.1 |
| `bot:stopper:5` | stopper | 1529931226 | 1 | 33.1 |
| `bot:strafer:4.2` | strafer | 1086809828 | 1 | 32.8 |
| `bot:strafer:5` | strafer | 821368291 | 1 | 32.8 |
| `bot:stopper:4.2` | stopper | 97502759 | 1 | 34.3 |
| `bot:strafer:5.2` | strafer | 2592015535 | 1 | 34.3 |
| `bot:strafer:4.3` | strafer | 3498264623 | 1 | 31.9 |
| `bot:stopper:5.2` | stopper | 2513918026 | 1 | 31.9 |
| `bot:stopper:4.3` | stopper | 2458011590 | 1 | 33.6 |
| `bot:strafer:5.3` | strafer | 4003987225 | 1 | 33.6 |
| `bot:presser:4` | presser | 52844662 | 1 | 35.4 |
| `bot:strafer:5.4` | strafer | 3859071546 | 1 | 35.4 |
| `bot:presser:4.2` | presser | 588158740 | 1 | 32.2 |
| `bot:presser:5.2` | presser | 4131749178 | 1 | 32.2 |

The owner (`pstN@vsbot`) has 0.0 duel minutes against the bots; those rows are never pooled with the bots.

Bot duel time against the owner 0%, against other bots 100%.

Family mix of bot duel time (observed → reweighted to the styles.json targets): presser 25% → 20%, stopper 31% → 40%, strafer 44% → 40%.

## Tells (151)

Statistics whose 95% CIs for the pooled bots and the pooled humans do not overlap, ranked by the standardized gap z = (bots − humans) / √(SE²bots + SE²humans). Only statistics with enough data on both sides are ranked.

| # | Statistic | Bots | Humans | z |
|---:|---|---|---|---:|
| 1 | Centroid sightings with a body part on screen first | 34.2% [31.2%, 37.2%] | 85.9% [84.2%, 87.7%] | -28.4 |
| 2 | Side-hold duration (all contexts) p50 | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | -28.3 |
| 3 | Crosshair closer to the corner than to the enemy, 500 ms before the first part | 58.7% [56.8%, 60.6%] | 80.0% [78.8%, 81.1%] | -18.2 |
| 4 | Speed 400-200 ms before the first part | 114 u/s [108 u/s, 121 u/s] | 188 u/s [182 u/s, 193 u/s] | -15.6 |
| 5 | Respawn delay after death p50 | 3000 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | +15.5 |
| 6 | Reloads started with the opponent dead | 46.0% [41.4%, 50.5%] | 88.2% [85.5%, 90.4%] | -15.2 |
| 7 | Yaw speed (hidden, firing) p99 | 450°/s [417°/s, 473°/s] | 194°/s [182°/s, 204°/s] | +14.8 |
| 8 | Share of duel time: LOS firefight | 7.0% [6.2%, 7.9%] | 17.6% [16.5%, 18.9%] | -13.4 |
| 9 | Mouse still between ticks (LOS firefight) | 3.5% [3.3%, 3.7%] | 1.6% [1.4%, 1.8%] | +13.3 |
| 10 | Fire held at +400 ms from sight gain | 62.3% [59.1%, 65.5%] | 87.3% [85.5%, 89.0%] | -12.9 |
| 11 | Fire held at +200 ms from sight gain | 48.0% [44.8%, 52.0%] | 77.0% [73.9%, 79.5%] | -12.7 |
| 12 | Key chord fwd diag (hidden, not firing) | 9.1% [8.1%, 10.0%] | 29.9% [26.7%, 33.2%] | -12.6 |
| 13 | Engagements that end in a kill | 56.9% [54.0%, 60.1%] | 79.9% [77.9%, 81.9%] | -12.0 |
| 14 | Share of duel time: hidden, not firing | 69.2% [65.9%, 71.9%] | 49.1% [47.4%, 51.2%] | +11.5 |
| 15 | Key chord neutral (reloading) | 27.1% [23.9%, 29.6%] | 8.3% [7.3%, 9.5%] | +11.5 |
| 16 | Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part | 46.8% [43.3%, 48.9%] | 28.1% [26.8%, 29.4%] | +11.5 |
| 17 | Shot accuracy (eligible SMG shots that hit) | 9.9% [8.8%, 11.2%] | 19.2% [18.1%, 20.3%] | -10.9 |
| 18 | Life length (lives ending in death) p50 | 23.5 s [21.5 s, 26.6 s] | 7.1 s [5.8 s, 8.2 s] | +10.8 |
| 19 | Speed 2-1 s before the first part | 110 u/s [102 u/s, 117 u/s] | 167 u/s [159 u/s, 176 u/s] | -10.6 |
| 20 | Share of duel time with a body part on screen and the centroid hidden | 3.8% [3.2%, 4.4%] | 10.5% [9.6%, 11.8%] | -10.4 |
| 21 | Crosshair closer to where the enemy will appear than to the enemy, -500 ms | 54.9% [53.1%, 56.9%] | 67.0% [65.6%, 68.8%] | -10.1 |
| 22 | Strafe switch hazard per tick at hold age 50 ms (LOS firefight) | 9.1% [7.7%, 10.5%] | 1.2% [0.9%, 1.6%] | +10.1 |
| 23 | Key chord fwd diag (reloading) | 18.2% [16.3%, 20.6%] | 36.8% [34.1%, 39.9%] | -9.9 |
| 24 | Walk presses per minute | 3.38/min [2.86/min, 3.82/min] | 7.54/min [6.90/min, 8.11/min] | -9.8 |
| 25 | Fire held without LOS | 13.3% [12.0%, 14.9%] | 24.5% [22.7%, 26.3%] | -9.6 |
| 26 | Side-hold duration (all contexts) p90 | 500 ms [450 ms, 500 ms] | 800 ms [800 ms, 850 ms] | -9.5 |
| 27 | Yaw speed (reloading) p99 | 638°/s [578°/s, 680°/s] | 945°/s [909°/s, 991°/s] | -9.3 |
| 28 | Key chord neutral (hidden, not firing) | 44.6% [41.8%, 47.0%] | 26.1% [23.1%, 28.6%] | +9.2 |
| 29 | Fire held at +700 ms from sight gain | 56.7% [53.5%, 59.7%] | 77.0% [74.3%, 79.7%] | -9.2 |
| 30 | Crosshair already on the body at the first visible tick | 13.1% [12.1%, 14.2%] | 22.6% [20.9%, 24.4%] | -8.8 |
| 31 | Fire held at +100 ms from sight gain | 38.4% [35.2%, 42.7%] | 61.4% [57.8%, 64.6%] | -8.7 |
| 32 | Retreating >40 u/s in LOS firefight at distance (160, 224] | 19.1% [16.6%, 20.9%] | 6.9% [5.5%, 8.7%] | +8.7 |
| 33 | Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) | 24.6% [22.9%, 26.4%] | 15.6% [14.7%, 16.9%] | +8.6 |
| 34 | Press hazard while hidden, LOS lost over 1 s ago | 1.8% [1.6%, 2.0%] | 4.1% [3.5%, 4.6%] | -8.1 |
| 35 | Attack holds that are taps (<=100 ms) | 34.1% [33.4%, 34.8%] | 44.3% [41.8%, 46.5%] | -7.9 |
| 36 | SMG shots that hit, a part on screen, centroid hidden | 10.4% [9.2%, 12.0%] | 21.7% [18.9%, 24.3%] | -7.9 |
| 37 | Strafe switch hazard per tick at hold age 100 ms (LOS firefight) | 12.5% [11.1%, 14.1%] | 5.8% [5.2%, 6.8%] | +7.9 |
| 38 | Share of duel time: hidden, firing | 10.4% [9.7%, 11.2%] | 15.8% [14.9%, 17.0%] | -7.8 |
| 39 | Fire held at +0 ms from sight gain | 26.6% [24.4%, 28.6%] | 42.5% [39.1%, 46.0%] | -7.8 |
| 40 | Fire already held on the tick before sight (prefire) | 21.9% [20.6%, 23.1%] | 35.4% [32.2%, 38.6%] | -7.7 |
| 41 | Median aim error at +0 ms from the first visible part | 10.63° [9.21°, 11.96°] | 5.22° [4.91°, 5.53°] | +7.7 |
| 42 | Median aim error at +0 ms from sight gain | 13.01° [10.76°, 14.64°] | 5.22° [4.88°, 5.61°] | +7.5 |
| 43 | Key chord forward (reloading) | 14.6% [12.4%, 16.4%] | 26.1% [23.8%, 28.6%] | -7.4 |
| 44 | Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part | 1.22° [0.52°, 1.81°] | -1.77° [-2.04°, -1.43°] | +7.4 |
| 45 | Yaw speed (LOS firefight) p99 | 569°/s [496°/s, 624°/s] | 305°/s [292°/s, 318°/s] | +7.3 |
| 46 | Retreating >40 u/s in LOS firefight at distance (96, 160] | 22.5% [20.2%, 25.0%] | 10.5% [8.5%, 12.6%] | +7.2 |
| 47 | Approaching >40 u/s in LOS firefight at distance (96, 160] | 17.9% [15.7%, 20.3%] | 36.4% [32.6%, 41.2%] | -7.2 |
| 48 | Key chord fwd diag (hidden, firing) | 11.1% [10.2%, 12.4%] | 28.6% [23.9%, 32.8%] | -7.1 |
| 49 | Side-hold duration (LOS firefight) p90 | 500 ms [450 ms, 500 ms] | 700 ms [650 ms, 700 ms] | -7.0 |
| 50 | Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) | 23.5% [22.0%, 25.4%] | 16.1% [14.9%, 17.5%] | +6.8 |
| 51 | Yaw error to the corner at -500 ms (sightings hidden >= 2 s) | 8.68° [7.67°, 9.70°] | 4.66° [3.99°, 5.31°] | +6.8 |
| 52 | Retreating >40 u/s in LOS firefight at distance (224, 288] | 15.0% [13.1%, 18.3%] | 5.3% [4.2%, 6.8%] | +6.7 |
| 53 | Fire held at +1000 ms from sight gain | 50.1% [47.8%, 52.5%] | 63.4% [60.9%, 66.4%] | -6.6 |
| 54 | Approaching >40 u/s in LOS firefight at distance (160, 224] | 15.0% [12.8%, 17.3%] | 35.4% [30.1%, 42.0%] | -6.5 |
| 55 | Key chord back any (reloading) | 11.8% [10.8%, 13.0%] | 6.5% [5.3%, 7.6%] | +6.4 |
| 56 | Yaw error to the corner, 500 ms before the first part | 8.68° [8.10°, 9.29°] | 5.81° [5.17°, 6.44°] | +6.4 |
| 57 | Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part | 36.6% [34.3%, 40.0%] | 48.2% [45.7%, 50.3%] | -6.3 |
| 58 | Part sightings whose visible bout ends without a hit | 45.2% [40.1%, 50.7%] | 27.5% [25.7%, 29.7%] | +6.3 |
| 59 | View within 30 deg of the hidden opponent, last seen 4-8 s | 61.8% [58.4%, 64.9%] | 76.4% [73.1%, 79.3%] | -6.0 |
| 60 | Yaw error to the hidden opponent, last seen 4-8 s | 19.50° [17.26°, 22.23°] | 10.29° [9.19°, 11.80°] | +6.0 |
| 61 | Key chord forward (hidden, not firing) | 8.3% [7.6%, 8.9%] | 11.9% [10.9%, 12.8%] | -6.0 |
| 62 | Approaching >40 u/s in LOS firefight at distance (224, 288] | 14.9% [12.1%, 19.0%] | 38.2% [32.3%, 46.4%] | -5.9 |
| 63 | Share of duel time in the busiest 10% of 32u cells (dm/main) | 31.4% [30.1%, 34.8%] | 45.2% [40.5%, 48.6%] | -5.8 |
| 64 | Approaching >40 u/s in LOS firefight at distance (288, 384] | 14.1% [10.7%, 18.5%] | 39.4% [32.7%, 47.2%] | -5.8 |
| 65 | Fire held at +1450 ms from sight gain | 39.4% [38.1%, 40.6%] | 53.6% [49.3%, 58.5%] | -5.7 |
| 66 | Median aim error at +100 ms from the first visible part | 8.88° [7.61°, 10.15°] | 4.87° [4.54°, 5.18°] | +5.7 |
| 67 | Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) | 35.4% [31.8%, 39.1%] | 49.4% [44.0%, 50.8%] | -5.7 |
| 68 | Crosshair on body firing with LOS at (128, 192] u | 33.3% [31.3%, 35.4%] | 42.5% [39.8%, 44.7%] | -5.7 |
| 69 | Fire held with no body part of the enemy visible (loaded, not reloading) | 11.3% [10.3%, 12.5%] | 17.3% [15.8%, 19.2%] | -5.6 |
| 70 | View within 30 deg of the hidden opponent, last seen <=250 ms | 87.8% [86.6%, 89.5%] | 92.7% [91.9%, 93.7%] | -5.6 |
| 71 | Attack holds begun loaded that are taps (<=100 ms), with no body part visible | 35.6% [34.3%, 36.6%] | 45.9% [42.2%, 48.9%] | -5.6 |
| 72 | Yaw error to the corner at +0 ms (sightings hidden >= 2 s) | 7.27° [6.05°, 8.50°] | 3.54° [3.01°, 4.10°] | +5.6 |
| 73 | Standing still (<5 u/s) (reloading) | 10.9% [9.2%, 12.5%] | 5.2% [4.3%, 6.2%] | +5.5 |
| 74 | Side-hold duration (LOS firefight) p50 | 200 ms [200 ms, 200 ms] | 300 ms [250 ms, 300 ms] | -5.4 |
| 75 | Key chord fwd diag (LOS firefight) | 11.3% [8.3%, 14.9%] | 30.5% [25.6%, 36.7%] | -5.3 |
| 76 | Crosshair within 2 deg of the corner's edge, 500 ms before the first part | 16.6% [15.3%, 18.2%] | 23.7% [21.7%, 25.7%] | -5.3 |
| 77 | SMG shots that hit, centroid visible | 20.9% [19.1%, 22.7%] | 27.2% [25.8%, 28.5%] | -5.3 |
| 78 | Median aim error at +200 ms from sight gain | 9.15° [7.55°, 10.74°] | 4.92° [4.54°, 5.23°] | +5.2 |
| 79 | Yaw speed (LOS, not firing) p99 | 860°/s [783°/s, 890°/s] | 583°/s [505°/s, 695°/s] | +5.2 |
| 80 | Retreating >40 u/s in LOS firefight at distance (288, 384] | 12.0% [10.3%, 13.8%] | 5.9% [4.6%, 7.3%] | +5.2 |
| 81 | Median aim error firing with LOS at (128, 192] u | 10.99° [10.38°, 11.52°] | 8.99° [8.66°, 9.45°] | +5.2 |
| 82 | Press hazard while hidden, LOS lost 300-500 ms ago | 10.3% [9.6%, 11.2%] | 7.0% [6.0%, 8.1%] | +5.1 |
| 83 | Median aim error at -200 ms from sight gain | 13.78° [11.69°, 15.62°] | 8.33° [7.70°, 8.92°] | +5.1 |
| 84 | Side-hold duration (LOS firefight) p25 | 150 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | -5.1 |
| 85 | Angle to the corner at the first part | 9.34° [8.04°, 10.67°] | 5.74° [5.33°, 6.17°] | +5.1 |
| 86 | Reaction: first trigger press after a clean sighting p90 | 650 ms [594 ms, 700 ms] | 400 ms [400 ms, 450 ms] | +4.8 |
| 87 | Fire already held on the tick before the first part (prefire) | 16.1% [15.3%, 17.0%] | 23.0% [20.3%, 25.5%] | -4.8 |
| 88 | Key chord back any (LOS, not firing) | 9.6% [8.7%, 11.0%] | 4.8% [3.4%, 6.7%] | +4.7 |
| 89 | Median aim error at +300 ms from sight gain | 8.28° [6.68°, 9.70°] | 4.53° [4.12°, 4.88°] | +4.7 |
| 90 | Crosshair on body firing with LOS at (192, 256] u | 32.1% [29.0%, 35.1%] | 42.0% [39.5%, 44.4%] | -4.7 |
| 91 | Mouse still between ticks (reloading) | 22.8% [22.2%, 23.5%] | 26.3% [25.0%, 27.6%] | -4.7 |
| 92 | Strafe switch hazard per tick at hold age 200 ms (LOS firefight) | 22.9% [21.1%, 24.6%] | 17.0% [15.4%, 18.7%] | +4.6 |
| 93 | Retreating >40 u/s in LOS firefight at distance (384, 512] | 9.5% [7.9%, 11.2%] | 4.8% [3.8%, 6.0%] | +4.6 |
| 94 | Approaching >40 u/s in LOS firefight at distance (384, 512] | 17.5% [13.0%, 24.2%] | 41.8% [33.8%, 49.1%] | -4.6 |
| 95 | Yaw speed (hidden, not firing) p99 | 630°/s [611°/s, 656°/s] | 544°/s [521°/s, 580°/s] | +4.6 |
| 96 | Median aim error at +100 ms from sight gain | 11.29° [9.11°, 13.38°] | 5.97° [5.54°, 6.43°] | +4.6 |
| 97 | Strafe switch hazard per tick at hold age 250 ms (LOS firefight) | 23.2% [21.2%, 25.1%] | 17.4% [16.0%, 19.0%] | +4.4 |
| 98 | Standing still (<5 u/s), all duel time | 23.3% [21.2%, 25.5%] | 15.7% [13.4%, 17.7%] | +4.4 |
| 99 | Key chord pure strafe (hidden, firing) | 54.3% [51.6%, 57.1%] | 44.9% [41.6%, 48.0%] | +4.4 |
| 100 | Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago | 23.1% [18.8%, 31.3%] | 7.4% [4.7%, 10.8%] | +4.4 |
| 101 | Crosshair on body firing with LOS at (256, 384] u | 31.9% [30.2%, 33.6%] | 39.0% [36.0%, 41.4%] | -4.4 |
| 102 | Key chord neutral (hidden, firing) | 20.7% [17.9%, 23.0%] | 13.2% [11.2%, 15.7%] | +4.3 |
| 103 | Release hazard while hidden, LOS lost 1 s or more ago | 15.5% [15.2%, 15.8%] | 11.3% [9.8%, 13.5%] | +4.3 |
| 104 | Strafe switch hazard per tick at hold age 150 ms (LOS firefight) | 18.2% [16.0%, 20.4%] | 12.4% [11.1%, 13.9%] | +4.2 |
| 105 | Key chord pure strafe (LOS firefight) | 70.4% [67.1%, 73.6%] | 58.0% [53.1%, 62.2%] | +4.2 |
| 106 | Fire held, LOS, aim error in half-widths (6, 10] | 35.0% [31.6%, 37.6%] | 22.4% [17.1%, 27.1%] | +4.1 |
| 107 | Share of duel time in the busiest 10% of 32u cells (dm/downladder) | 36.1% [32.5%, 38.0%] | 61.4% [42.9%, 63.8%] | -4.1 |
| 108 | Key chord back any (LOS firefight) | 8.1% [7.1%, 9.2%] | 5.2% [4.5%, 6.1%] | +4.0 |
| 109 | Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part | 11.49° [9.71°, 12.91°] | 15.18° [14.24°, 16.01°] | -4.0 |
| 110 | Key chord fwd diag (LOS, not firing) | 12.5% [11.2%, 13.8%] | 29.4% [23.0%, 38.1%] | -4.0 |
| 111 | Median aim error firing with LOS at (0, 128] u | 17.68° [16.35°, 19.17°] | 14.28° [13.53°, 15.19°] | +4.0 |
| 112 | Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago | 43.9% [38.0%, 51.0%] | 28.0% [25.2%, 32.0%] | +3.9 |
| 113 | Share of duel time: LOS, not firing | 3.7% [3.2%, 4.2%] | 7.0% [5.5%, 8.4%] | -3.9 |
| 114 | Approaching >40 u/s in LOS firefight at distance (0, 96] | 25.7% [21.7%, 30.1%] | 37.6% [33.7%, 42.3%] | -3.9 |
| 115 | Angle to the corner, 500 ms before the first part | 10.78° [10.00°, 11.82°] | 8.41° [7.70°, 9.19°] | +3.9 |
| 116 | Median aim error firing with LOS at (192, 256] u | 8.12° [7.60°, 8.74°] | 6.68° [6.36°, 7.10°] | +3.9 |
| 117 | Fire held, LOS, aim error in half-widths (20, 1000] | 5.0% [3.6%, 6.6%] | 0.9% [0.4%, 3.1%] | +3.8 |
| 118 | First hitter wins (decisive engagements) | 70.0% [68.2%, 71.9%] | 65.4% [63.7%, 66.7%] | +3.8 |
| 119 | Press hazard with LOS and aim within 3 half-widths, LOS gained 550-1000 ms ago | 33.9% [29.8%, 41.0%] | 21.0% [16.7%, 24.6%] | +3.8 |
| 120 | Side-hold duration (LOS firefight) p75 | 350 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | -3.8 |
| 121 | Key chord pure strafe (LOS, not firing) | 42.0% [39.2%, 45.0%] | 26.7% [20.8%, 34.2%] | +3.8 |
| 122 | Median aim error at -100 ms from the first visible part | 10.84° [9.46°, 12.15°] | 8.00° [7.34°, 8.49°] | +3.7 |
| 123 | Press hazard while hidden, LOS lost 150-250 ms ago | 7.6% [6.5%, 9.2%] | 4.6% [3.9%, 5.5%] | +3.7 |
| 124 | Release hazard while hidden, LOS lost 500-950 ms ago | 13.3% [12.0%, 14.6%] | 10.1% [9.2%, 11.3%] | +3.6 |
| 125 | Retreating >40 u/s in LOS firefight at distance (512, 768] | 10.3% [6.7%, 12.8%] | 4.0% [3.0%, 5.4%] | +3.6 |
| 126 | Standing still (<5 u/s) (hidden, not firing) | 29.6% [27.3%, 32.1%] | 22.5% [19.5%, 25.1%] | +3.6 |
| 127 | Fire held, LOS, aim error in half-widths (10, 20] | 13.6% [8.8%, 18.1%] | 4.6% [2.4%, 6.9%] | +3.6 |
| 128 | Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) | 8.80° [8.00°, 10.20°] | 5.78° [4.88°, 6.98°] | +3.6 |
| 129 | Yaw speed (LOS, not firing) p50 | 37°/s [33°/s, 40°/s] | 11°/s [0°/s, 26°/s] | +3.6 |
| 130 | Release hazard, LOS, aim error in half-widths (4, 6] | 3.3% [2.5%, 4.0%] | 1.7% [1.3%, 2.3%] | +3.5 |
| 131 | Key chord neutral (LOS firefight) | 9.1% [7.1%, 11.0%] | 5.1% [3.9%, 6.3%] | +3.4 |
| 132 | Crosshair on body firing with LOS at (0, 128] u | 39.3% [35.6%, 42.9%] | 48.0% [44.9%, 51.5%] | -3.4 |
| 133 | Key chord pure strafe (reloading) | 28.2% [25.9%, 30.7%] | 22.3% [19.6%, 24.4%] | +3.4 |
| 134 | View turn toward the enemy over the last 500 ms before the first part | 8.38° [6.75°, 10.66°] | 13.01° [11.44°, 14.55°] | -3.4 |
| 135 | First shooter wins (decisive engagements) | 59.8% [58.1%, 62.2%] | 55.0% [52.5%, 56.6%] | +3.3 |
| 136 | View within 30 deg of the hidden opponent, last seen 250-500 ms | 89.4% [88.2%, 90.7%] | 92.2% [91.3%, 93.4%] | -3.3 |
| 137 | Yaw speed (LOS firefight) p50 | 49°/s [44°/s, 56°/s] | 39°/s [36°/s, 41°/s] | +3.2 |
| 138 | Yaw error to the hidden enemy 500 ms before the first part | 11.92° [10.75°, 13.43°] | 14.63° [13.70°, 15.63°] | -3.2 |
| 139 | Mouse still between ticks (hidden, firing) | 9.5% [9.2%, 9.9%] | 10.9% [10.1%, 11.8%] | -3.1 |
| 140 | Release hazard, LOS, aim error in half-widths (6, 10] | 13.7% [10.1%, 16.2%] | 6.9% [4.4%, 9.9%] | +3.1 |
| 141 | Release hazard while hidden, LOS lost 300-450 ms ago | 26.9% [24.5%, 29.2%] | 31.5% [29.6%, 33.4%] | -3.1 |
| 142 | Yaw speed (hidden, firing) p50 | 28°/s [26°/s, 31°/s] | 23°/s [21°/s, 25°/s] | +3.1 |
| 143 | Engagement start distance p50 | 334 u [300 u, 390 u] | 427 u [398 u, 470 u] | -3.0 |
| 144 | Approaching >40 u/s in LOS firefight at distance (512, 768] | 16.1% [11.1%, 24.1%] | 33.1% [25.5%, 41.5%] | -3.0 |
| 145 | Crosshair within 5 deg of the corner, 500 ms before the first part | 22.8% [20.3%, 25.9%] | 29.2% [26.4%, 32.7%] | -3.0 |
| 146 | Key chord back any (hidden, firing) | 8.7% [8.0%, 9.4%] | 6.7% [5.9%, 7.9%] | +2.9 |
| 147 | Median aim error at +200 ms from the first visible part | 6.60° [5.67°, 7.41°] | 5.26° [4.86°, 5.58°] | +2.9 |
| 148 | Key chord back any (hidden, not firing) | 7.5% [7.2%, 8.0%] | 6.1% [5.3%, 7.1%] | +2.9 |
| 149 | Press hazard while hidden, LOS lost 50-100 ms ago | 12.6% [11.1%, 14.4%] | 9.5% [8.5%, 10.9%] | +2.8 |
| 150 | First hit after the first part p50 | 300 ms [300 ms, 300 ms] | 250 ms [200 ms, 250 ms] | +2.4 |
| 151 | Approaching >40 u/s in LOS firefight at distance (768, 1200] | 10.4% [5.4%, 17.5%] | 28.2% [17.6%, 42.7%] | -2.2 |

## Per bot

### `bot:strafer:4` (strafer, seed 231333596, 30.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.05398 | -2% |
| reverse_share | 0.4467 | 0.3982 | -7% |
| side_hold_ms | 326.3 | 250 | -51% ⚠ |
| lean_fight | 0.4941 | 0.5716 | +15% |
| jumps_per_min | 2.379 | 1.978 | -7% |
| crouch_per_min | 9.476 | 9.56 | +1% |
| walk_hidden | 0 | 0.01677 | +16% |
| burst_median | 8 | 7 | -25% ⚠ |
| aim_height_firing | 0.4857 | 0.468 | -9% |
| hold_angle | 0.2514 | 0.352 | +33% ⚠ |
| aim_error_fight_deg | 4.667 | 3.399 | -52% ⚠ |
| reaction_ms | 140.8 | 150 | +9% |
| mp40_share | 0.9062 | 1 | +9% |

Style fingerprint (2026-10-02, 30.3 min, styles.py features 30/30): z-distance to the human cloud centre 7.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 5.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.493 (humans 0.191–0.489); fwd_diag 0.0988 (humans 0.152–0.491); lf_pure_strafe 0.775 (humans 0.362–0.752); lf_fwd_diag 0.054 (humans 0.0658–0.568); lean_any 0.333 (humans 0.358–0.767); ducked 0.0879 (humans 0.0109–0.0741); walk 0.00992 (humans 0.0126–0.0881); attack 0.235 (humans 0.276–0.478); speed_med 134 (humans 136–200); lf_err_med 3.4 (humans 4.08–6.63); side_hold_p90_ms 600 (humans 700–950); lf_approach 0.0901 (humans 0.131–0.64); lf_retreat 0.144 (humans 0.0243–0.141).

### `bot:presser:5` (presser, seed 1152489726, 30.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4832 | 0.4347 | -9% |
| reverse_share | 0.8051 | 0.786 | -3% |
| side_hold_ms | 295.2 | 200 | -63% ⚠ |
| lean_fight | 0.9204 | 0.97 | +10% |
| jumps_per_min | 0.4317 | 0.3626 | -1% |
| crouch_per_min | 3.157 | 3.824 | +6% |
| walk_hidden | 0.02816 | 0.03056 | +2% |
| burst_median | 6.498 | 6 | -12% |
| aim_height_firing | 0.417 | 0.382 | -19% |
| hold_angle | 0.2275 | 0.2089 | -6% |
| aim_error_fight_deg | 4.893 | 4.367 | -21% |
| reaction_ms | 152.5 | 225 | +72% ⚠ |
| mp40_share | 0.1123 | 1 | +89% ⚠ |

Style fingerprint (2026-10-02, 30.3 min, styles.py features 30/30): z-distance to the human cloud centre 6.5 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 7.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.861 (humans 0.358–0.767); lf_lean 0.97 (humans 0.457–0.948); attack 0.221 (humans 0.276–0.478); side_hold_p90_ms 500 (humans 700–950).

### `bot:stopper:4` (stopper, seed 711847621, 33.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.07034 | 0.04549 | -5% |
| reverse_share | 0.1382 | 0.09862 | -6% |
| side_hold_ms | 249.3 | 150 | -66% ⚠ |
| lean_fight | 0.9549 | 0.9518 | -1% |
| jumps_per_min | 0.3303 | 0.2416 | -2% |
| crouch_per_min | 5.571 | 5.618 | +0% |
| walk_hidden | 0 | 0.01207 | +11% |
| burst_median | 4.564 | 5 | +11% |
| aim_height_firing | 0.4682 | 0.46 | -4% |
| hold_angle | 0.4674 | 0.3438 | -40% ⚠ |
| aim_error_fight_deg | 6.249 | 5.277 | -40% ⚠ |
| reaction_ms | 153.7 | 250 | +96% ⚠ |
| mp40_share | 0.02524 | 0 | -3% |

Style fingerprint (2026-10-02, 33.1 min, styles.py features 30/30): z-distance to the human cloud centre 12.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.2 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0413 (humans 0.152–0.491); neutral 0.569 (humans 0.0872–0.296); lf_fwd_diag 0.0455 (humans 0.0658–0.568); lf_neutral 0.223 (humans 0.00436–0.112); lean_any 0.836 (humans 0.358–0.767); lf_lean 0.952 (humans 0.457–0.948); walk 0.00805 (humans 0.0126–0.0881); attack 0.184 (humans 0.276–0.478); p_attack_given_los 0.506 (humans 0.547–0.861); speed_med 61.9 (humans 136–200); still 0.365 (humans 0.0832–0.227); lf_on_target 0.269 (humans 0.274–0.515); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 500 (humans 700–950); reverse_share 0.0986 (humans 0.138–0.839); lf_approach 0.107 (humans 0.131–0.64); lf_retreat 0.151 (humans 0.0243–0.141).

### `bot:stopper:5` (stopper, seed 1529931226, 33.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1013 | 0.06375 | -7% |
| reverse_share | 0.3253 | 0.267 | -8% |
| side_hold_ms | 247.2 | 150 | -65% ⚠ |
| lean_fight | 0.8427 | 0.8373 | -1% |
| jumps_per_min | 5.122 | 6.041 | +17% |
| crouch_per_min | 11.7 | 12.75 | +10% |
| walk_hidden | 0.07078 | 0.06818 | -2% |
| burst_median | 4.132 | 5 | +22% |
| aim_height_firing | 0.4597 | 0.43 | -16% |
| hold_angle | 0.5274 | 0.3669 | -52% ⚠ |
| aim_error_fight_deg | 4.835 | 4.355 | -20% |
| reaction_ms | 109.9 | 150 | +40% ⚠ |
| mp40_share | 0.06948 | 0 | -7% |

Style fingerprint (2026-10-02, 33.1 min, styles.py features 30/30): z-distance to the human cloud centre 12.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0611 (humans 0.152–0.491); neutral 0.512 (humans 0.0872–0.296); lf_fwd_diag 0.0638 (humans 0.0658–0.568); lf_neutral 0.153 (humans 0.00436–0.112); ducked 0.142 (humans 0.0109–0.0741); attack 0.196 (humans 0.276–0.478); p_attack_given_los 0.527 (humans 0.547–0.861); speed_med 66.7 (humans 136–200); still 0.34 (humans 0.0832–0.227); yaw_speed_med 9.88 (humans 12.6–30); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 550 (humans 700–950); lf_approach 0.126 (humans 0.131–0.64); lf_retreat 0.15 (humans 0.0243–0.141).

### `bot:strafer:4.2` (strafer, seed 1086809828, 32.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1632 | 0.07851 | -17% |
| reverse_share | 0.4593 | 0.3981 | -9% |
| side_hold_ms | 314.9 | 200 | -77% ⚠ |
| lean_fight | 0.5599 | 0.5832 | +5% |
| jumps_per_min | 4.813 | 5.221 | +8% |
| crouch_per_min | 9.497 | 9.892 | +4% |
| walk_hidden | 0 | 0.01157 | +11% |
| burst_median | 5.59 | 5 | -15% |
| aim_height_firing | 0.4741 | 0.467 | -4% |
| hold_angle | 0.3025 | 0.3053 | +1% |
| aim_error_fight_deg | 6.527 | 10.07 | +144% ⚠ |
| reaction_ms | 158.4 | 225 | +67% ⚠ |
| mp40_share | 0.9804 | 1 | +2% |

Style fingerprint (2026-10-02, 32.8 min, styles.py features 30/30): z-distance to the human cloud centre 11.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.6 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.115 (humans 0.152–0.491); lean_any 0.342 (humans 0.358–0.767); ducked 0.126 (humans 0.0109–0.0741); walk 0.00771 (humans 0.0126–0.0881); attack 0.177 (humans 0.276–0.478); p_attack_given_los 0.503 (humans 0.547–0.861); speed_med 121 (humans 136–200); lf_err_med 10.1 (humans 4.08–6.63); yaw_speed_p95 315 (humans 158–271); side_hold_p90_ms 500 (humans 700–950); lf_retreat 0.232 (humans 0.0243–0.141).

### `bot:strafer:5` (strafer, seed 821368291, 32.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.166 | 0.08815 | -15% |
| reverse_share | 0.4801 | 0.4252 | -8% |
| side_hold_ms | 349.1 | 200 | -99% ⚠ |
| lean_fight | 0.6654 | 0.7169 | +10% |
| jumps_per_min | 5.052 | 4.671 | -7% |
| crouch_per_min | 10.37 | 9.861 | -5% |
| walk_hidden | 0.0009382 | 0.01097 | +9% |
| burst_median | 5.755 | 5 | -19% |
| aim_height_firing | 0.4434 | 0.414 | -16% |
| hold_angle | 0.3191 | 0.2444 | -24% |
| aim_error_fight_deg | 4.261 | 8.311 | +165% ⚠ |
| reaction_ms | 175 | 250 | +75% ⚠ |
| mp40_share | 0.1202 | 0 | -12% |

Style fingerprint (2026-10-02, 32.8 min, styles.py features 30/30): z-distance to the human cloud centre 10.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.125 (humans 0.152–0.491); lf_pure_strafe 0.762 (humans 0.362–0.752); ducked 0.109 (humans 0.0109–0.0741); walk 0.00776 (humans 0.0126–0.0881); attack 0.171 (humans 0.276–0.478); p_attack_given_los 0.458 (humans 0.547–0.861); speed_med 122 (humans 136–200); lf_err_med 8.31 (humans 4.08–6.63); yaw_speed_p95 290 (humans 158–271); side_hold_p90_ms 450 (humans 700–950); lf_retreat 0.189 (humans 0.0243–0.141).

### `bot:stopper:4.2` (stopper, seed 97502759, 34.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.04113 | -5% |
| reverse_share | 0.6408 | 0.6054 | -5% |
| side_hold_ms | 200 | 200 | +0% |
| lean_fight | 0.8757 | 0.8811 | +1% |
| jumps_per_min | 1.606 | 1.693 | +2% |
| crouch_per_min | 2.514 | 1.985 | -5% |
| walk_hidden | 0.09232 | 0.09264 | +0% |
| burst_median | 4 | 4 | +0% |
| aim_height_firing | 0.4804 | 0.474 | -3% |
| hold_angle | 0.418 | 0.2692 | -48% ⚠ |
| aim_error_fight_deg | 5.509 | 8.369 | +116% ⚠ |
| reaction_ms | 127.6 | 150 | +22% |
| mp40_share | 0.1179 | 0 | -12% |

Style fingerprint (2026-10-02, 34.3 min, styles.py features 30/30): z-distance to the human cloud centre 13.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 11.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0544 (humans 0.152–0.491); neutral 0.525 (humans 0.0872–0.296); lf_fwd_diag 0.0411 (humans 0.0658–0.568); attack 0.144 (humans 0.276–0.478); p_attack_given_los 0.547 (humans 0.547–0.861); speed_med 53.5 (humans 136–200); still 0.373 (humans 0.0832–0.227); lf_err_med 8.37 (humans 4.08–6.63); side_hold_p90_ms 450 (humans 700–950); lf_approach 0.106 (humans 0.131–0.64); lf_retreat 0.211 (humans 0.0243–0.141).

### `bot:strafer:5.2` (strafer, seed 2592015535, 34.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.119 | 0.1168 | -0% |
| reverse_share | 0.4951 | 0.4598 | -5% |
| side_hold_ms | 295 | 200 | -63% ⚠ |
| lean_fight | 0.4565 | 0.4847 | +6% |
| jumps_per_min | 5.657 | 5.75 | +2% |
| crouch_per_min | 11.59 | 11 | -5% |
| walk_hidden | 0 | 0.008455 | +8% |
| burst_median | 4.178 | 4 | -4% |
| aim_height_firing | 0.54 | 0.542 | +1% |
| hold_angle | 0.3001 | 0.3304 | +10% |
| aim_error_fight_deg | 6.024 | 7.995 | +80% ⚠ |
| reaction_ms | 165.9 | 250 | +84% ⚠ |
| mp40_share | 0.4858 | 1 | +51% ⚠ |

Style fingerprint (2026-10-02, 34.3 min, styles.py features 30/30): z-distance to the human cloud centre 11.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0972 (humans 0.152–0.491); neutral 0.333 (humans 0.0872–0.296); lean_any 0.223 (humans 0.358–0.767); ducked 0.133 (humans 0.0109–0.0741); walk 0.00632 (humans 0.0126–0.0881); attack 0.132 (humans 0.276–0.478); p_attack_given_los 0.457 (humans 0.547–0.861); speed_med 103 (humans 136–200); lf_err_med 8 (humans 4.08–6.63); lf_height_frac 0.542 (humans 0.352–0.54); side_hold_p90_ms 500 (humans 700–950); lf_retreat 0.17 (humans 0.0243–0.141).

### `bot:strafer:4.3` (strafer, seed 3498264623, 31.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1522 | 0.08743 | -13% |
| reverse_share | 0.4724 | 0.4365 | -5% |
| side_hold_ms | 293.3 | 200 | -62% ⚠ |
| lean_fight | 0.6201 | 0.6792 | +12% |
| jumps_per_min | 3.829 | 3.945 | +2% |
| crouch_per_min | 9.64 | 10.05 | +4% |
| walk_hidden | 0 | 0.01416 | +13% |
| burst_median | 4.228 | 5 | +19% |
| aim_height_firing | 0.4919 | 0.502 | +5% |
| hold_angle | 0.3757 | 0.1883 | -61% ⚠ |
| aim_error_fight_deg | 5.803 | 8.108 | +94% ⚠ |
| reaction_ms | 133.2 | 200 | +67% ⚠ |
| mp40_share | 0.5841 | 1 | +42% ⚠ |

Style fingerprint (2026-10-02, 31.9 min, styles.py features 30/30): z-distance to the human cloud centre 9.6 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.2 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.11 (humans 0.152–0.491); neutral 0.315 (humans 0.0872–0.296); ducked 0.0941 (humans 0.0109–0.0741); walk 0.00994 (humans 0.0126–0.0881); attack 0.176 (humans 0.276–0.478); speed_med 115 (humans 136–200); lf_err_med 8.11 (humans 4.08–6.63); yaw_speed_p95 281 (humans 158–271); side_hold_p90_ms 550 (humans 700–950); lf_retreat 0.195 (humans 0.0243–0.141).

### `bot:stopper:5.2` (stopper, seed 2513918026, 31.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1336 | 0.0891 | -9% |
| reverse_share | 0.6777 | 0.6408 | -5% |
| side_hold_ms | 226 | 200 | -17% |
| lean_fight | 0.9588 | 0.9671 | +2% |
| jumps_per_min | 2.944 | 3.506 | +10% |
| crouch_per_min | 3.426 | 2.912 | -5% |
| walk_hidden | 0.08429 | 0.09276 | +8% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4602 | 0.452 | -4% |
| hold_angle | 0.3824 | 0.2076 | -57% ⚠ |
| aim_error_fight_deg | 4.291 | 6.856 | +104% ⚠ |
| reaction_ms | 113 | 150 | +37% ⚠ |
| mp40_share | 0.07877 | 0 | -8% |

Style fingerprint (2026-10-02, 31.9 min, styles.py features 30/30): z-distance to the human cloud centre 11.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0758 (humans 0.152–0.491); neutral 0.479 (humans 0.0872–0.296); lean_any 0.838 (humans 0.358–0.767); lf_lean 0.967 (humans 0.457–0.948); attack 0.17 (humans 0.276–0.478); speed_med 67.4 (humans 136–200); still 0.339 (humans 0.0832–0.227); lf_err_med 6.86 (humans 4.08–6.63); yaw_speed_med 11.1 (humans 12.6–30); side_hold_p90_ms 500 (humans 700–950); lf_retreat 0.168 (humans 0.0243–0.141).

### `bot:stopper:4.3` (stopper, seed 2458011590, 33.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1182 | 0.09497 | -5% |
| reverse_share | 0.4266 | 0.3698 | -8% |
| side_hold_ms | 200 | 200 | +0% |
| lean_fight | 0.8817 | 0.8691 | -2% |
| jumps_per_min | 2.965 | 2.974 | +0% |
| crouch_per_min | 8.679 | 8.207 | -4% |
| walk_hidden | 0.1056 | 0.09364 | -11% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4719 | 0.447 | -13% |
| hold_angle | 0.3772 | 0.2783 | -32% ⚠ |
| aim_error_fight_deg | 4.42 | 6.259 | +75% ⚠ |
| reaction_ms | 139.9 | 200 | +60% ⚠ |
| mp40_share | 0.02794 | 0 | -3% |

Style fingerprint (2026-10-02, 33.6 min, styles.py features 30/30): z-distance to the human cloud centre 12.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0522 (humans 0.152–0.491); neutral 0.574 (humans 0.0872–0.296); lf_neutral 0.141 (humans 0.00436–0.112); ducked 0.0748 (humans 0.0109–0.0741); attack 0.135 (humans 0.276–0.478); p_attack_given_los 0.538 (humans 0.547–0.861); speed_med 44.2 (humans 136–200); still 0.413 (humans 0.0832–0.227); yaw_speed_med 9.66 (humans 12.6–30); side_hold_p90_ms 550 (humans 700–950).

### `bot:strafer:5.3` (strafer, seed 4003987225, 33.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.04023 | -5% |
| reverse_share | 0.4657 | 0.4122 | -8% |
| side_hold_ms | 270.9 | 200 | -47% ⚠ |
| lean_fight | 0.5402 | 0.5136 | -5% |
| jumps_per_min | 4.852 | 5.918 | +20% |
| crouch_per_min | 3.183 | 2.736 | -4% |
| walk_hidden | 0.03847 | 0.03351 | -5% |
| burst_median | 5.686 | 5 | -17% |
| aim_height_firing | 0.4588 | 0.44 | -10% |
| hold_angle | 0.311 | 0.224 | -28% ⚠ |
| aim_error_fight_deg | 5.446 | 6.822 | +56% ⚠ |
| reaction_ms | 161.3 | 250 | +89% ⚠ |
| mp40_share | 0 | 0 | +0% |

Style fingerprint (2026-10-02, 33.6 min, styles.py features 30/30): z-distance to the human cloud centre 11.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.062 (humans 0.152–0.491); neutral 0.44 (humans 0.0872–0.296); lf_fwd_diag 0.0402 (humans 0.0658–0.568); lean_any 0.254 (humans 0.358–0.767); attack 0.147 (humans 0.276–0.478); p_attack_given_los 0.526 (humans 0.547–0.861); speed_med 84.5 (humans 136–200); still 0.281 (humans 0.0832–0.227); lf_err_med 6.82 (humans 4.08–6.63); side_hold_p90_ms 550 (humans 700–950); lf_approach 0.0876 (humans 0.131–0.64); lf_retreat 0.217 (humans 0.0243–0.141).

### `bot:presser:4` (presser, seed 52844662, 35.4 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5317 | 0.1957 | -66% ⚠ |
| reverse_share | 0.8152 | 0.6821 | -19% |
| side_hold_ms | 336.6 | 150 | -124% ⚠ |
| lean_fight | 0.935 | 0.9699 | +7% |
| jumps_per_min | 0.336 | 0.5365 | +4% |
| crouch_per_min | 1.298 | 1.073 | -2% |
| walk_hidden | 0 | 0.005225 | +5% |
| burst_median | 7.121 | 6 | -28% ⚠ |
| aim_height_firing | 0.3853 | 0.345 | -21% |
| hold_angle | 0.2402 | 0.2712 | +10% |
| aim_error_fight_deg | 4.424 | 5.208 | +32% ⚠ |
| reaction_ms | 173.4 | 200 | +27% ⚠ |
| mp40_share | 0.06486 | 0 | -6% |

Style fingerprint (2026-10-02, 35.4 min, styles.py features 30/30): z-distance to the human cloud centre 9.5 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.15 (humans 0.152–0.491); lean_any 0.885 (humans 0.358–0.767); lf_lean 0.97 (humans 0.457–0.948); walk 0.00384 (humans 0.0126–0.0881); attack 0.142 (humans 0.276–0.478); p_attack_given_los 0.488 (humans 0.547–0.861); speed_med 94.1 (humans 136–200); lf_height_frac 0.345 (humans 0.352–0.54); yaw_speed_med 9.88 (humans 12.6–30); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 350 (humans 700–950).

### `bot:strafer:5.4` (strafer, seed 3859071546, 35.4 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1614 | 0.07357 | -17% |
| reverse_share | 0.5449 | 0.4454 | -14% |
| side_hold_ms | 350 | 200 | -100% ⚠ |
| lean_fight | 0.5666 | 0.7385 | +34% ⚠ |
| jumps_per_min | 0.6862 | 0.5648 | -2% |
| crouch_per_min | 3.8 | 3.304 | -5% |
| walk_hidden | 0.04377 | 0.02934 | -14% |
| burst_median | 7.775 | 5 | -69% ⚠ |
| aim_height_firing | 0.4661 | 0.475 | +5% |
| hold_angle | 0.2527 | 0.3429 | +29% ⚠ |
| aim_error_fight_deg | 4.952 | 4.998 | +2% |
| reaction_ms | 115 | 200 | +85% ⚠ |
| mp40_share | 0.03683 | 0 | -4% |

Style fingerprint (2026-10-02, 35.4 min, styles.py features 30/30): z-distance to the human cloud centre 9.6 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0983 (humans 0.152–0.491); neutral 0.302 (humans 0.0872–0.296); lf_pure_strafe 0.756 (humans 0.362–0.752); lean_any 0.336 (humans 0.358–0.767); attack 0.146 (humans 0.276–0.478); p_attack_given_los 0.494 (humans 0.547–0.861); speed_med 113 (humans 136–200); yaw_speed_p95 308 (humans 158–271); side_hold_p90_ms 450 (humans 700–950); lf_approach 0.124 (humans 0.131–0.64); lf_retreat 0.182 (humans 0.0243–0.141).

### `bot:presser:4.2` (presser, seed 588158740, 32.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4812 | 0.1798 | -59% ⚠ |
| reverse_share | 0.8389 | 0.8104 | -4% |
| side_hold_ms | 302.9 | 200 | -69% ⚠ |
| lean_fight | 0.9588 | 0.9839 | +5% |
| jumps_per_min | 1.077 | 1.148 | +1% |
| crouch_per_min | 1.47 | 0.8998 | -5% |
| walk_hidden | 0.004612 | 0.01662 | +11% |
| burst_median | 7.045 | 8 | +24% |
| aim_height_firing | 0.352 | 0.298 | -29% ⚠ |
| hold_angle | 0.2402 | 0.2736 | +11% |
| aim_error_fight_deg | 4.382 | 5.179 | +32% ⚠ |
| reaction_ms | 183.1 | 250 | +67% ⚠ |
| mp40_share | 0.9584 | 1 | +4% |

Style fingerprint (2026-10-02, 32.2 min, styles.py features 30/30): z-distance to the human cloud centre 8.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.503 (humans 0.191–0.489); lean_any 0.908 (humans 0.358–0.767); lf_lean 0.984 (humans 0.457–0.948); walk 0.00933 (humans 0.0126–0.0881); attack 0.234 (humans 0.276–0.478); p_attack_given_los 0.479 (humans 0.547–0.861); speed_med 123 (humans 136–200); lf_height_frac 0.298 (humans 0.352–0.54); yaw_speed_p95 297 (humans 158–271); side_hold_p90_ms 450 (humans 700–950).

### `bot:presser:5.2` (presser, seed 4131749178, 32.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5433 | 0.1523 | -76% ⚠ |
| reverse_share | 0.823 | 0.7987 | -3% |
| side_hold_ms | 298.6 | 200 | -66% ⚠ |
| lean_fight | 0.9579 | 0.9741 | +3% |
| jumps_per_min | 0.5292 | 0.5274 | -0% |
| crouch_per_min | 2.785 | 2.172 | -6% |
| walk_hidden | 0.00312 | 0.01472 | +11% |
| burst_median | 7.004 | 8 | +25% |
| aim_height_firing | 0.4126 | 0.349 | -34% ⚠ |
| hold_angle | 0.2204 | 0.2549 | +11% |
| aim_error_fight_deg | 4.873 | 5.551 | +28% ⚠ |
| reaction_ms | 134.4 | 200 | +66% ⚠ |
| mp40_share | 0.08844 | 0 | -9% |

Style fingerprint (2026-10-02, 32.2 min, styles.py features 30/30): z-distance to the human cloud centre 8.3 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.6 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lf_pure_strafe 0.772 (humans 0.362–0.752); lean_any 0.902 (humans 0.358–0.767); lf_lean 0.974 (humans 0.457–0.948); walk 0.00879 (humans 0.0126–0.0881); attack 0.239 (humans 0.276–0.478); speed_med 118 (humans 136–200); lf_height_frac 0.349 (humans 0.352–0.54); yaw_speed_p95 321 (humans 158–271); side_hold_p90_ms 450 (humans 700–950).

## The owner against the bots (`pstN@vsbot`)

Kills of bots 0, deaths to bots 0, suicides 0. 

0 statistics of the owner against bots fall outside the human CI (owner column below, marked •).

## All statistics

Pooled bots are reweighted to the human family mix; humans and the owner are pooled over their duel time. ▲ marks a tell. n = units (ticks, runs, shots, sightings, ...) over the blocks.

### Movement

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time: hidden, not firing ▲ | 69.2% [65.9%, 71.9%] | 49.1% [47.4%, 51.2%] | - | 632810 |
| Share of duel time: hidden, firing ▲ | 10.4% [9.7%, 11.2%] | 15.8% [14.9%, 17.0%] | - | 632810 |
| Share of duel time: LOS, not firing ▲ | 3.7% [3.2%, 4.2%] | 7.0% [5.5%, 8.4%] | - | 632810 |
| Share of duel time: LOS firefight ▲ | 7.0% [6.2%, 7.9%] | 17.6% [16.5%, 18.9%] | - | 632810 |
| Share of duel time: reloading | 9.7% [8.6%, 10.8%] | 10.5% [9.6%, 11.5%] | - | 632810 |
| Key chord pure strafe (hidden, not firing) | 30.5% [28.5%, 32.6%] | 26.0% [23.5%, 29.4%] | - | 434740 |
| Key chord fwd diag (hidden, not firing) ▲ | 9.1% [8.1%, 10.0%] | 29.9% [26.7%, 33.2%] | - | 434740 |
| Key chord forward (hidden, not firing) ▲ | 8.3% [7.6%, 8.9%] | 11.9% [10.9%, 12.8%] | - | 434740 |
| Key chord neutral (hidden, not firing) ▲ | 44.6% [41.8%, 47.0%] | 26.1% [23.1%, 28.6%] | - | 434740 |
| Key chord back any (hidden, not firing) ▲ | 7.5% [7.2%, 8.0%] | 6.1% [5.3%, 7.1%] | - | 434740 |
| Key chord pure strafe (hidden, firing) ▲ | 54.3% [51.6%, 57.1%] | 44.9% [41.6%, 48.0%] | - | 66882 |
| Key chord fwd diag (hidden, firing) ▲ | 11.1% [10.2%, 12.4%] | 28.6% [23.9%, 32.8%] | - | 66882 |
| Key chord forward (hidden, firing) | 5.2% [4.6%, 5.7%] | 6.6% [5.7%, 7.4%] | - | 66882 |
| Key chord neutral (hidden, firing) ▲ | 20.7% [17.9%, 23.0%] | 13.2% [11.2%, 15.7%] | - | 66882 |
| Key chord back any (hidden, firing) ▲ | 8.7% [8.0%, 9.4%] | 6.7% [5.9%, 7.9%] | - | 66882 |
| Key chord pure strafe (LOS, not firing) ▲ | 42.0% [39.2%, 45.0%] | 26.7% [20.8%, 34.2%] | - | 23657 |
| Key chord fwd diag (LOS, not firing) ▲ | 12.5% [11.2%, 13.8%] | 29.4% [23.0%, 38.1%] | - | 23657 |
| Key chord forward (LOS, not firing) | 7.0% [6.0%, 8.2%] | 7.5% [5.9%, 9.5%] | - | 23657 |
| Key chord neutral (LOS, not firing) | 28.8% [25.0%, 31.7%] | 31.5% [15.3%, 45.4%] | - | 23657 |
| Key chord back any (LOS, not firing) ▲ | 9.6% [8.7%, 11.0%] | 4.8% [3.4%, 6.7%] | - | 23657 |
| Key chord pure strafe (LOS firefight) ▲ | 70.4% [67.1%, 73.6%] | 58.0% [53.1%, 62.2%] | - | 44999 |
| Key chord fwd diag (LOS firefight) ▲ | 11.3% [8.3%, 14.9%] | 30.5% [25.6%, 36.7%] | - | 44999 |
| Key chord forward (LOS firefight) | 1.2% [0.9%, 1.3%] | 1.2% [1.0%, 1.4%] | - | 44999 |
| Key chord neutral (LOS firefight) ▲ | 9.1% [7.1%, 11.0%] | 5.1% [3.9%, 6.3%] | - | 44999 |
| Key chord back any (LOS firefight) ▲ | 8.1% [7.1%, 9.2%] | 5.2% [4.5%, 6.1%] | - | 44999 |
| Key chord pure strafe (reloading) ▲ | 28.2% [25.9%, 30.7%] | 22.3% [19.6%, 24.4%] | - | 62530 |
| Key chord fwd diag (reloading) ▲ | 18.2% [16.3%, 20.6%] | 36.8% [34.1%, 39.9%] | - | 62530 |
| Key chord forward (reloading) ▲ | 14.6% [12.4%, 16.4%] | 26.1% [23.8%, 28.6%] | - | 62530 |
| Key chord neutral (reloading) ▲ | 27.1% [23.9%, 29.6%] | 8.3% [7.3%, 9.5%] | - | 62530 |
| Key chord back any (reloading) ▲ | 11.8% [10.8%, 13.0%] | 6.5% [5.3%, 7.6%] | - | 62530 |
| Strafe end is a direct reverse (hidden, not firing) | 50.1% [45.9%, 54.1%] | 46.1% [41.1%, 51.2%] | - | 43541 |
| Strafe end is a direct reverse (hidden, firing) | 61.5% [54.7%, 68.3%] | 67.2% [59.7%, 74.3%] | - | 9666 |
| Strafe end is a direct reverse (LOS, not firing) | 52.2% [45.7%, 59.0%] | 52.8% [45.1%, 60.4%] | - | 2874 |
| Strafe end is a direct reverse (LOS firefight) | 64.3% [54.3%, 74.8%] | 73.3% [65.9%, 80.6%] | - | 7089 |
| Strafe end is a direct reverse (reloading) | 46.7% [41.4%, 51.7%] | 37.7% [32.5%, 42.7%] | - | 7067 |
| Strafe switch hazard per tick at hold age 50 ms (LOS firefight) ▲ | 9.1% [7.7%, 10.5%] | 1.2% [0.9%, 1.6%] | - | 7874 |
| Strafe switch hazard per tick at hold age 100 ms (LOS firefight) ▲ | 12.5% [11.1%, 14.1%] | 5.8% [5.2%, 6.8%] | - | 7000 |
| Strafe switch hazard per tick at hold age 150 ms (LOS firefight) ▲ | 18.2% [16.0%, 20.4%] | 12.4% [11.1%, 13.9%] | - | 6095 |
| Strafe switch hazard per tick at hold age 200 ms (LOS firefight) ▲ | 22.9% [21.1%, 24.6%] | 17.0% [15.4%, 18.7%] | - | 4905 |
| Strafe switch hazard per tick at hold age 250 ms (LOS firefight) ▲ | 23.2% [21.2%, 25.1%] | 17.4% [16.0%, 19.0%] | - | 3618 |
| Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) ▲ | 24.6% [22.9%, 26.4%] | 15.6% [14.7%, 16.9%] | - | 6961 |
| Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) ▲ | 23.5% [22.0%, 25.4%] | 16.1% [14.9%, 17.5%] | - | 3262 |
| Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) | 20.6% [15.3%, 27.1%] | 13.7% [12.0%, 15.6%] | - | 283 |
| Side-hold duration (LOS firefight) p25 ▲ | 150 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | - | 7166 |
| Side-hold duration (LOS firefight) p50 ▲ | 200 ms [200 ms, 200 ms] | 300 ms [250 ms, 300 ms] | - | 7166 |
| Side-hold duration (LOS firefight) p75 ▲ | 350 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | - | 7166 |
| Side-hold duration (LOS firefight) p90 ▲ | 500 ms [450 ms, 500 ms] | 700 ms [650 ms, 700 ms] | - | 7166 |
| Side-hold duration (all contexts) p50 ▲ | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | - | 69756 |
| Side-hold duration (all contexts) p90 ▲ | 500 ms [450 ms, 500 ms] | 800 ms [800 ms, 850 ms] | - | 69756 |
| Neutral gap between strafes (LOS firefight) p50 | 50 ms [50 ms, 50 ms] | 50 ms [50 ms, 50 ms] | - | 2446 |
| Neutral gap between strafes (LOS firefight) p90 | 100 ms [50 ms, 100 ms] | 100 ms [100 ms, 150 ms] | - | 2446 |
| Approaching >40 u/s in LOS firefight at distance (0, 96] ▲ | 25.7% [21.7%, 30.1%] | 37.6% [33.7%, 42.3%] | - | 2493 |
| Approaching >40 u/s in LOS firefight at distance (96, 160] ▲ | 17.9% [15.7%, 20.3%] | 36.4% [32.6%, 41.2%] | - | 5284 |
| Approaching >40 u/s in LOS firefight at distance (160, 224] ▲ | 15.0% [12.8%, 17.3%] | 35.4% [30.1%, 42.0%] | - | 7500 |
| Approaching >40 u/s in LOS firefight at distance (224, 288] ▲ | 14.9% [12.1%, 19.0%] | 38.2% [32.3%, 46.4%] | - | 7507 |
| Approaching >40 u/s in LOS firefight at distance (288, 384] ▲ | 14.1% [10.7%, 18.5%] | 39.4% [32.7%, 47.2%] | - | 8809 |
| Approaching >40 u/s in LOS firefight at distance (384, 512] ▲ | 17.5% [13.0%, 24.2%] | 41.8% [33.8%, 49.1%] | - | 6788 |
| Approaching >40 u/s in LOS firefight at distance (512, 768] ▲ | 16.1% [11.1%, 24.1%] | 33.1% [25.5%, 41.5%] | - | 5997 |
| Approaching >40 u/s in LOS firefight at distance (768, 1200] ▲ | 10.4% [5.4%, 17.5%] | 28.2% [17.6%, 42.7%] | - | 621 |
| Retreating >40 u/s in LOS firefight at distance (0, 96] | 29.1% [25.4%, 33.1%] | 22.2% [18.9%, 25.6%] | - | 2493 |
| Retreating >40 u/s in LOS firefight at distance (96, 160] ▲ | 22.5% [20.2%, 25.0%] | 10.5% [8.5%, 12.6%] | - | 5284 |
| Retreating >40 u/s in LOS firefight at distance (160, 224] ▲ | 19.1% [16.6%, 20.9%] | 6.9% [5.5%, 8.7%] | - | 7500 |
| Retreating >40 u/s in LOS firefight at distance (224, 288] ▲ | 15.0% [13.1%, 18.3%] | 5.3% [4.2%, 6.8%] | - | 7507 |
| Retreating >40 u/s in LOS firefight at distance (288, 384] ▲ | 12.0% [10.3%, 13.8%] | 5.9% [4.6%, 7.3%] | - | 8809 |
| Retreating >40 u/s in LOS firefight at distance (384, 512] ▲ | 9.5% [7.9%, 11.2%] | 4.8% [3.8%, 6.0%] | - | 6788 |
| Retreating >40 u/s in LOS firefight at distance (512, 768] ▲ | 10.3% [6.7%, 12.8%] | 4.0% [3.0%, 5.4%] | - | 5997 |
| Retreating >40 u/s in LOS firefight at distance (768, 1200] | 14.8% [2.5%, 25.7%] | 8.3% [1.5%, 15.5%] | - | 621 |
| Standing still (<5 u/s), all duel time ▲ | 23.3% [21.2%, 25.5%] | 15.7% [13.4%, 17.7%] | - | 632810 |
| Standing still (<5 u/s) (hidden, not firing) ▲ | 29.6% [27.3%, 32.1%] | 22.5% [19.5%, 25.1%] | - | 434740 |
| Standing still (<5 u/s) (hidden, firing) | 10.1% [9.0%, 11.0%] | 10.2% [9.1%, 11.7%] | - | 66882 |
| Standing still (<5 u/s) (LOS, not firing) | 13.9% [11.7%, 16.0%] | 29.6% [12.6%, 44.0%] | - | 23657 |
| Standing still (<5 u/s) (LOS firefight) | 3.0% [2.6%, 3.3%] | 2.3% [1.8%, 2.7%] | - | 44999 |
| Standing still (<5 u/s) (reloading) ▲ | 10.9% [9.2%, 12.5%] | 5.2% [4.3%, 6.2%] | - | 62530 |
| Lean held, all duel time | 58.9% [55.3%, 64.8%] | 59.7% [55.3%, 64.0%] | - | 632810 |
| Lean held in LOS firefights | 80.9% [76.5%, 85.1%] | 81.3% [76.6%, 85.8%] | - | 44999 |
| Jump presses per minute | 2.88/min [1.85/min, 3.72/min] | 1.87/min [1.41/min, 2.44/min] | - | 632810 |
| Crouch presses per minute | 6.10/min [4.02/min, 7.78/min] | 4.33/min [3.22/min, 5.51/min] | - | 632810 |
| Walk presses per minute ▲ | 3.38/min [2.86/min, 3.82/min] | 7.54/min [6.90/min, 8.11/min] | - | 632810 |
| Walking while hidden and not firing | 4.1% [2.7%, 5.1%] | 5.9% [5.0%, 7.0%] | - | 434740 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Press hazard with LOS and aim within 3 half-widths, LOS gained 0 ms ago | 29.1% [24.9%, 33.5%] | 28.2% [25.8%, 30.8%] | - | 1017 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 50-100 ms ago | 36.5% [32.0%, 42.2%] | 33.0% [30.8%, 35.6%] | - | 1350 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 150-250 ms ago | 40.5% [35.9%, 46.6%] | 35.2% [32.9%, 38.1%] | - | 1142 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago ▲ | 43.9% [38.0%, 51.0%] | 28.0% [25.2%, 32.0%] | - | 676 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 550-1000 ms ago ▲ | 33.9% [29.8%, 41.0%] | 21.0% [16.7%, 24.6%] | - | 690 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago ▲ | 23.1% [18.8%, 31.3%] | 7.4% [4.7%, 10.8%] | - | 1156 |
| Press hazard while hidden, LOS lost 0 ms ago | 14.0% [12.8%, 15.3%] | 12.3% [10.8%, 14.5%] | - | 1592 |
| Press hazard while hidden, LOS lost 50-100 ms ago ▲ | 12.6% [11.1%, 14.4%] | 9.5% [8.5%, 10.9%] | - | 2413 |
| Press hazard while hidden, LOS lost 150-250 ms ago ▲ | 7.6% [6.5%, 9.2%] | 4.6% [3.9%, 5.5%] | - | 3657 |
| Press hazard while hidden, LOS lost 300-500 ms ago ▲ | 10.3% [9.6%, 11.2%] | 7.0% [6.0%, 8.1%] | - | 10837 |
| Press hazard while hidden, LOS lost 550-1000 ms ago | 3.5% [3.2%, 3.8%] | 3.5% [3.2%, 3.9%] | - | 21748 |
| Press hazard while hidden, LOS lost over 1 s ago ▲ | 1.8% [1.6%, 2.0%] | 4.1% [3.5%, 4.6%] | - | 376240 |
| Release hazard, LOS, aim error in half-widths (0, 1] | 1.6% [1.2%, 2.0%] | 1.4% [1.1%, 1.7%] | - | 7784 |
| Release hazard, LOS, aim error in half-widths (1, 2] | 2.1% [1.5%, 2.6%] | 1.5% [1.3%, 1.9%] | - | 13569 |
| Release hazard, LOS, aim error in half-widths (2, 3] | 2.4% [2.0%, 3.0%] | 1.5% [1.2%, 2.0%] | - | 10002 |
| Release hazard, LOS, aim error in half-widths (3, 4] | 2.4% [1.8%, 2.9%] | 1.4% [1.0%, 2.0%] | - | 5750 |
| Release hazard, LOS, aim error in half-widths (4, 6] ▲ | 3.3% [2.5%, 4.0%] | 1.7% [1.3%, 2.3%] | - | 4545 |
| Release hazard, LOS, aim error in half-widths (6, 10] ▲ | 13.7% [10.1%, 16.2%] | 6.9% [4.4%, 9.9%] | - | 1553 |
| Release hazard, LOS, aim error in half-widths (10, 1000] | 18.8% [14.9%, 23.8%] | 32.7% [22.9%, 41.8%] | - | 920 |
| Release hazard while hidden, LOS lost 0-100 ms ago | 6.1% [5.2%, 6.9%] | 6.3% [5.4%, 7.5%] | - | 9162 |
| Release hazard while hidden, LOS lost 150-250 ms ago | 13.3% [12.2%, 14.3%] | 16.5% [13.8%, 18.7%] | - | 5575 |
| Release hazard while hidden, LOS lost 300-450 ms ago ▲ | 26.9% [24.5%, 29.2%] | 31.5% [29.6%, 33.4%] | - | 5565 |
| Release hazard while hidden, LOS lost 500-950 ms ago ▲ | 13.3% [12.0%, 14.6%] | 10.1% [9.2%, 11.3%] | - | 6570 |
| Release hazard while hidden, LOS lost 1 s or more ago ▲ | 15.5% [15.2%, 15.8%] | 11.3% [9.8%, 13.5%] | - | 38489 |
| Fire held, LOS, aim error in half-widths (0, 0.5] | 87.2% [82.6%, 90.9%] | 89.6% [87.0%, 91.7%] | - | 2503 |
| Fire held, LOS, aim error in half-widths (0.5, 1] | 86.2% [82.8%, 89.5%] | 88.9% [86.9%, 90.5%] | - | 6647 |
| Fire held, LOS, aim error in half-widths (1, 1.5] | 85.8% [83.0%, 88.6%] | 86.3% [84.5%, 87.8%] | - | 8256 |
| Fire held, LOS, aim error in half-widths (1.5, 2] | 83.7% [81.3%, 86.2%] | 81.9% [79.8%, 83.7%] | - | 7952 |
| Fire held, LOS, aim error in half-widths (2, 3] | 80.8% [78.1%, 83.7%] | 81.0% [79.4%, 82.7%] | - | 12473 |
| Fire held, LOS, aim error in half-widths (3, 4] | 77.2% [74.6%, 80.0%] | 78.8% [76.1%, 81.2%] | - | 7530 |
| Fire held, LOS, aim error in half-widths (4, 6] | 64.0% [60.3%, 67.8%] | 67.7% [63.0%, 71.3%] | - | 7166 |
| Fire held, LOS, aim error in half-widths (6, 10] ▲ | 35.0% [31.6%, 37.6%] | 22.4% [17.1%, 27.1%] | - | 4470 |
| Fire held, LOS, aim error in half-widths (10, 20] ▲ | 13.6% [8.8%, 18.1%] | 4.6% [2.4%, 6.9%] | - | 3893 |
| Fire held, LOS, aim error in half-widths (20, 1000] ▲ | 5.0% [3.6%, 6.6%] | 0.9% [0.4%, 3.1%] | - | 6942 |
| Fire held without LOS ▲ | 13.3% [12.0%, 14.9%] | 24.5% [22.7%, 26.3%] | - | 481970 |
| Attack hold duration p50 | 250 ms [200 ms, 250 ms] | 150 ms [150 ms, 200 ms] | - | 12175 |
| Attack hold duration p90 | 850 ms [800 ms, 850 ms] | 900 ms [800 ms, 950 ms] | - | 12175 |
| Attack holds that are taps (<=100 ms) ▲ | 34.1% [33.4%, 34.8%] | 44.3% [41.8%, 46.5%] | - | 12175 |
| Gap between attack holds p50 | 400 ms [400 ms, 450 ms] | 400 ms [350 ms, 400 ms] | - | 11716 |

### Aim

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error firing with LOS at (0, 128] u ▲ | 17.68° [16.35°, 19.17°] | 14.28° [13.53°, 15.19°] | - | 3451 |
| Crosshair on body firing with LOS at (0, 128] u ▲ | 39.3% [35.6%, 42.9%] | 48.0% [44.9%, 51.5%] | - | 3451 |
| Median aim error firing with LOS at (128, 192] u ▲ | 10.99° [10.38°, 11.52°] | 8.99° [8.66°, 9.45°] | - | 6220 |
| Crosshair on body firing with LOS at (128, 192] u ▲ | 33.3% [31.3%, 35.4%] | 42.5% [39.8%, 44.7%] | - | 6220 |
| Median aim error firing with LOS at (192, 256] u ▲ | 8.12° [7.60°, 8.74°] | 6.68° [6.36°, 7.10°] | - | 7637 |
| Crosshair on body firing with LOS at (192, 256] u ▲ | 32.1% [29.0%, 35.1%] | 42.0% [39.5%, 44.4%] | - | 7637 |
| Median aim error firing with LOS at (256, 384] u | 5.57° [5.35°, 5.78°] | 5.23° [4.96°, 5.59°] | - | 13174 |
| Crosshair on body firing with LOS at (256, 384] u ▲ | 31.9% [30.2%, 33.6%] | 39.0% [36.0%, 41.4%] | - | 13174 |
| Median aim error firing with LOS at (384, 512] u | 3.99° [3.73°, 4.27°] | 3.92° [3.67°, 4.18°] | - | 7176 |
| Crosshair on body firing with LOS at (384, 512] u | 31.2% [28.5%, 34.0%] | 35.4% [32.6%, 38.3%] | - | 7176 |
| Median aim error firing with LOS at (512, 768] u | 2.95° [2.67°, 3.32°] | 3.07° [2.80°, 3.35°] | - | 6695 |
| Crosshair on body firing with LOS at (512, 768] u | 30.4% [27.2%, 33.2%] | 32.0% [29.1%, 35.1%] | - | 6695 |
| Median aim error firing with LOS at (768, 1200] u | 2.12° [2.03°, 2.20°] | 2.14° [1.84°, 2.58°] | - | 646 |
| Crosshair on body firing with LOS at (768, 1200] u | 29.9% [27.1%, 33.3%] | 31.5% [24.3%, 39.5%] | - | 646 |
| Aim height (fraction of body) firing with LOS | 0.444 [0.43, 0.455] | 0.446 [0.418, 0.468] | - | 44999 |
| Aim height (fraction of body) with LOS, not firing | 0.613 [0.595, 0.637] | 0.662 [0.576, 0.751] | - | 40681 |

### View

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw speed (hidden, not firing) p50 | 10°/s [9°/s, 11°/s] | 8°/s [6°/s, 10°/s] | - | 433660 |
| Yaw speed (hidden, not firing) p99 ▲ | 630°/s [611°/s, 656°/s] | 544°/s [521°/s, 580°/s] | - | 433660 |
| Mouse still between ticks (hidden, not firing) | 32.9% [32.2%, 33.6%] | 32.1% [28.8%, 36.0%] | - | 434740 |
| Yaw speed (hidden, firing) p50 ▲ | 28°/s [26°/s, 31°/s] | 23°/s [21°/s, 25°/s] | - | 66882 |
| Yaw speed (hidden, firing) p99 ▲ | 450°/s [417°/s, 473°/s] | 194°/s [182°/s, 204°/s] | - | 66882 |
| Mouse still between ticks (hidden, firing) ▲ | 9.5% [9.2%, 9.9%] | 10.9% [10.1%, 11.8%] | - | 66882 |
| Yaw speed (LOS, not firing) p50 ▲ | 37°/s [33°/s, 40°/s] | 11°/s [0°/s, 26°/s] | - | 23650 |
| Yaw speed (LOS, not firing) p99 ▲ | 860°/s [783°/s, 890°/s] | 583°/s [505°/s, 695°/s] | - | 23650 |
| Mouse still between ticks (LOS, not firing) | 21.3% [20.0%, 22.8%] | 36.7% [20.8%, 49.8%] | - | 23657 |
| Yaw speed (LOS firefight) p50 ▲ | 49°/s [44°/s, 56°/s] | 39°/s [36°/s, 41°/s] | - | 44999 |
| Yaw speed (LOS firefight) p99 ▲ | 569°/s [496°/s, 624°/s] | 305°/s [292°/s, 318°/s] | - | 44999 |
| Mouse still between ticks (LOS firefight) ▲ | 3.5% [3.3%, 3.7%] | 1.6% [1.4%, 1.8%] | - | 44999 |
| Yaw speed (reloading) p50 | 19°/s [17°/s, 21°/s] | 18°/s [16°/s, 20°/s] | - | 62530 |
| Yaw speed (reloading) p99 ▲ | 638°/s [578°/s, 680°/s] | 945°/s [909°/s, 991°/s] | - | 62530 |
| Mouse still between ticks (reloading) ▲ | 22.8% [22.2%, 23.5%] | 26.3% [25.0%, 27.6%] | - | 62530 |
| Peak yaw speed of 90-135 deg turns p50 | 612°/s [604°/s, 625°/s] | 619°/s [595°/s, 655°/s] | - | 2512 |
| Peak yaw speed of 135-180 deg turns p50 | 878°/s [852°/s, 903°/s] | 852°/s [828°/s, 887°/s] | - | 1405 |
| Peak yaw speed of 180-360 deg turns p50 | 1090°/s [1054°/s, 1134°/s] | 1061°/s [1020°/s, 1126°/s] | - | 333 |

### Belief

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw error to the hidden opponent, last seen <=250 ms | 5.84° [5.11°, 6.52°] | 4.92° [4.56°, 5.26°] | - | 23934 |
| View within 30 deg of the hidden opponent, last seen <=250 ms ▲ | 87.8% [86.6%, 89.5%] | 92.7% [91.9%, 93.7%] | - | 23934 |
| Yaw error to the hidden opponent, last seen 250-500 ms | 6.32° [5.44°, 7.09°] | 6.66° [5.99°, 7.28°] | - | 15105 |
| View within 30 deg of the hidden opponent, last seen 250-500 ms ▲ | 89.4% [88.2%, 90.7%] | 92.2% [91.3%, 93.4%] | - | 15105 |
| Yaw error to the hidden opponent, last seen 0.5-1 s | 9.24° [8.29°, 10.06°] | 8.28° [7.48°, 9.11°] | - | 22008 |
| View within 30 deg of the hidden opponent, last seen 0.5-1 s | 86.4% [85.1%, 87.8%] | 88.0% [86.8%, 89.9%] | - | 22008 |
| Yaw error to the hidden opponent, last seen 1-2 s | 12.17° [10.96°, 13.29°] | 11.11° [9.95°, 12.13°] | - | 30508 |
| View within 30 deg of the hidden opponent, last seen 1-2 s | 79.1% [76.7%, 81.0%] | 78.9% [77.1%, 81.1%] | - | 30508 |
| Yaw error to the hidden opponent, last seen 2-4 s | 21.55° [18.63°, 24.47°] | 16.93° [15.18°, 19.08°] | - | 56080 |
| View within 30 deg of the hidden opponent, last seen 2-4 s | 59.2% [55.4%, 63.4%] | 64.3% [61.3%, 67.3%] | - | 56080 |
| Yaw error to the hidden opponent, last seen 4-8 s ▲ | 19.50° [17.26°, 22.23°] | 10.29° [9.19°, 11.80°] | - | 86294 |
| View within 30 deg of the hidden opponent, last seen 4-8 s ▲ | 61.8% [58.4%, 64.9%] | 76.4% [73.1%, 79.3%] | - | 86294 |
| Yaw error to the hidden opponent, last seen >8 s | 18.05° [15.75°, 20.88°] | 20.07° [12.82°, 29.16°] | - | 135780 |
| View within 30 deg of the hidden opponent, last seen >8 s | 63.6% [59.7%, 67.3%] | 65.1% [54.8%, 74.8%] | - | 135780 |
| Yaw error to the hidden opponent, last seen never seen this life | 20.00° [16.06°, 25.40°] | 19.23° [16.93°, 24.71°] | - | 177420 |
| View within 30 deg of the hidden opponent, last seen never seen this life | 61.3% [55.3%, 66.8%] | 62.5% [56.0%, 67.2%] | - | 177420 |

### Space

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) ▲ | 35.4% [31.8%, 39.1%] | 49.4% [44.0%, 50.8%] | - | 152260 |
| Share of duel time in the busiest 10% of 32u cells (dm/main) ▲ | 31.4% [30.1%, 34.8%] | 45.2% [40.5%, 48.6%] | - | 157370 |
| Share of duel time in the busiest 10% of 32u cells (dm/vents) | 41.0% [35.3%, 57.8%] | 44.3% [39.2%, 46.8%] | - | 162350 |
| Share of duel time in the busiest 10% of 32u cells (dm/downladder) ▲ | 36.1% [32.5%, 38.0%] | 61.4% [42.9%, 63.8%] | - | 160840 |

### Acquisition

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error at -500 ms from sight gain | 15.78° [13.53°, 18.07°] | 13.98° [12.90°, 15.10°] | - | 2924 |
| Median aim error at -200 ms from sight gain ▲ | 13.78° [11.69°, 15.62°] | 8.33° [7.70°, 8.92°] | - | 2928 |
| Median aim error at +0 ms from sight gain ▲ | 13.01° [10.76°, 14.64°] | 5.22° [4.88°, 5.61°] | - | 2933 |
| Median aim error at +100 ms from sight gain ▲ | 11.29° [9.11°, 13.38°] | 5.97° [5.54°, 6.43°] | - | 2933 |
| Median aim error at +200 ms from sight gain ▲ | 9.15° [7.55°, 10.74°] | 4.92° [4.54°, 5.23°] | - | 2933 |
| Median aim error at +300 ms from sight gain ▲ | 8.28° [6.68°, 9.70°] | 4.53° [4.12°, 4.88°] | - | 2892 |
| Median aim error at +500 ms from sight gain | 7.02° [5.63°, 8.22°] | 5.47° [5.08°, 5.88°] | - | 2716 |
| Median aim error at +1000 ms from sight gain | 7.62° [6.39°, 8.70°] | 7.25° [6.72°, 7.70°] | - | 2298 |
| Fire held at -500 ms from sight gain | 20.0% [18.0%, 21.8%] | 18.2% [16.5%, 20.2%] | - | 2933 |
| Fire held at -200 ms from sight gain | 20.5% [18.2%, 22.5%] | 21.7% [19.7%, 23.9%] | - | 2933 |
| Fire held at +0 ms from sight gain ▲ | 26.6% [24.4%, 28.6%] | 42.5% [39.1%, 46.0%] | - | 2933 |
| Fire held at +100 ms from sight gain ▲ | 38.4% [35.2%, 42.7%] | 61.4% [57.8%, 64.6%] | - | 2933 |
| Fire held at +200 ms from sight gain ▲ | 48.0% [44.8%, 52.0%] | 77.0% [73.9%, 79.5%] | - | 2933 |
| Fire held at +400 ms from sight gain ▲ | 62.3% [59.1%, 65.5%] | 87.3% [85.5%, 89.0%] | - | 2863 |
| Fire held at +700 ms from sight gain ▲ | 56.7% [53.5%, 59.7%] | 77.0% [74.3%, 79.7%] | - | 2739 |
| Fire held at +1000 ms from sight gain ▲ | 50.1% [47.8%, 52.5%] | 63.4% [60.9%, 66.4%] | - | 2614 |
| Fire held at +1450 ms from sight gain ▲ | 39.4% [38.1%, 40.6%] | 53.6% [49.3%, 58.5%] | - | 2440 |
| Reaction: first trigger press after a clean sighting p25 | 100 ms [50 ms, 100 ms] | 50 ms [50 ms, 100 ms] | - | 1377 |
| Reaction: first trigger press after a clean sighting p50 | 200 ms [200 ms, 200 ms] | 150 ms [150 ms, 200 ms] | - | 1377 |
| Reaction: first trigger press after a clean sighting p75 | 350 ms [300 ms, 400 ms] | 300 ms [250 ms, 300 ms] | - | 1377 |
| Reaction: first trigger press after a clean sighting p90 ▲ | 650 ms [594 ms, 700 ms] | 400 ms [400 ms, 450 ms] | - | 1377 |
| Crosshair within half-width+1.5 deg after a clean sighting p50 | 300 ms [250 ms, 300 ms] | 200 ms [200 ms, 250 ms] | - | 1327 |
| Crosshair already on the body at the first visible tick ▲ | 13.1% [12.1%, 14.2%] | 22.6% [20.9%, 24.4%] | - | 2933 |
| Fire already held on the tick before sight (prefire) ▲ | 21.9% [20.6%, 23.1%] | 35.4% [32.2%, 38.6%] | - | 2933 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shots per burst (consecutive 100 ms shots) p50 | 4 [3, 4] | 5 [4, 5] | - | 11150 |
| Shots per burst (consecutive 100 ms shots) p90 | 11 [10, 11] | 12 [11, 13] | - | 11150 |

### Combat

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shot accuracy (eligible SMG shots that hit) ▲ | 9.9% [8.8%, 11.2%] | 19.2% [18.1%, 20.3%] | - | 55739 |

### Lives

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Respawn delay after death p50 ▲ | 3000 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | - | 1080 |
| Life length (lives ending in death) p50 ▲ | 23.5 s [21.5 s, 26.6 s] | 7.1 s [5.8 s, 8.2 s] | - | 1080 |

### Reload

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Reload within 3 s of a kill, rounds left 0 | 100.0% [100.0%, 100.0%] | 95.5% [86.4%, 100.0%] | - | 31 |
| Reload within 3 s of a kill, rounds left 1-3 | 98.5% [95.4%, 100.0%] | 96.4% [92.6%, 99.2%] | - | 61 |
| Reload within 3 s of a kill, rounds left 4-6 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 89 |
| Reload within 3 s of a kill, rounds left 7-10 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 107 |
| Reload within 3 s of a kill, rounds left 11-15 | 98.4% [95.8%, 100.0%] | 98.8% [97.9%, 99.5%] | - | 185 |
| Reload within 3 s of a kill, rounds left 16-20 | 97.2% [94.8%, 99.2%] | 95.7% [93.5%, 97.7%] | - | 223 |
| Reload within 3 s of a kill, rounds left 21-25 | 87.8% [83.3%, 93.0%] | 83.1% [76.5%, 88.5%] | - | 195 |
| Reload within 3 s of a kill, rounds left 26-29 | 56.8% [44.4%, 68.8%] | 63.4% [52.8%, 75.0%] | - | 153 |
| Reload within 3 s of a kill, rounds left 30-32 | 65.0% [50.0%, 79.3%] | 73.7% [50.0%, 92.3%] | - | 32 |
| Delay from kill to reload (reloads within 3 s) p50 | 650 ms [650 ms, 700 ms] | 600 ms [600 ms, 650 ms] | - | 964 |
| Reloads started with the opponent dead ▲ | 46.0% [41.4%, 50.5%] | 88.2% [85.5%, 90.4%] | - | 2103 |
| Reloads with the opponent alive that are forced (empty clip) | 75.1% [72.1%, 78.1%] | 77.7% [69.5%, 83.9%] | - | 1139 |

### Fights

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Engagements that end in a kill ▲ | 56.9% [54.0%, 60.1%] | 79.9% [77.9%, 81.9%] | - | 3712 |
| First hitter wins (decisive engagements) ▲ | 70.0% [68.2%, 71.9%] | 65.4% [63.7%, 66.7%] | - | 2122 |
| First shooter wins (decisive engagements) ▲ | 59.8% [58.1%, 62.2%] | 55.0% [52.5%, 56.6%] | - | 2122 |
| Duration of decisive engagements p50 | 1400 ms [1300 ms, 1500 ms] | 1250 ms [1150 ms, 1351 ms] | - | 2122 |
| Engagement start distance p50 ▲ | 334 u [300 u, 390 u] | 427 u [398 u, 470 u] | - | 3712 |
| Decisive engagements won (subject's own) | 50.4% [47.4%, 53.7%] | 50.0% [45.6%, 54.2%] | - | 2122 |

### Perception

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time with a body part on screen and the centroid hidden ▲ | 3.8% [3.2%, 4.4%] | 10.5% [9.6%, 11.8%] | - | 632810 |
| Fire held with no body part of the enemy visible (loaded, not reloading) ▲ | 11.3% [10.3%, 12.5%] | 17.3% [15.8%, 19.2%] | - | 475270 |
| Fire held with no body part visible, 0-500ms after one was last on screen | 48.2% [45.5%, 51.0%] | 43.2% [40.9%, 45.8%] | - | 33467 |
| Fire held with no body part visible, 500-1000ms after one was last on screen | 28.5% [26.5%, 30.6%] | 26.4% [23.9%, 28.7%] | - | 21802 |
| Fire held with no body part visible, 1000-2000ms after one was last on screen | 20.9% [18.7%, 23.2%] | 20.5% [18.0%, 22.9%] | - | 30267 |
| Fire held with no body part visible, 2000-5000ms after one was last on screen | 9.2% [8.2%, 10.3%] | 7.7% [6.4%, 9.1%] | - | 77977 |
| Fire held with no body part visible, gt5000ms after one was last on screen | 5.7% [5.1%, 6.6%] | 4.3% [3.1%, 6.0%] | - | 196370 |
| Attack holds begun loaded that are taps (<=100 ms), with a body part visible | 14.4% [11.9%, 16.7%] | 15.4% [13.1%, 17.6%] | - | 2949 |
| Attack holds begun loaded that are taps (<=100 ms), with no body part visible ▲ | 35.6% [34.3%, 36.6%] | 45.9% [42.2%, 48.9%] | - | 8317 |
| Shots per burst whose first round left with a body part visible p50 | 5 [5, 6] | 6 [5, 6] | - | 4450 |
| Shots per burst whose first round left with a body part visible p90 | 14 [13.2, 15] | 13 [12, 14] | - | 4450 |
| SMG shots that hit, a part on screen, centroid hidden ▲ | 10.4% [9.2%, 12.0%] | 21.7% [18.9%, 24.3%] | - | 6936 |
| SMG shots that hit, centroid visible ▲ | 20.9% [19.1%, 22.7%] | 27.2% [25.8%, 28.5%] | - | 22760 |
| Centroid sightings with a body part on screen first ▲ | 34.2% [31.2%, 37.2%] | 85.9% [84.2%, 87.7%] | - | 2924 |
| Lead of the first part over the centroid p50 | 100 ms [50 ms, 100 ms] | 100 ms [100 ms, 100 ms] | - | 1001 |
| Median aim error at -500 ms from the first visible part | 13.83° [11.91°, 15.23°] | 15.23° [14.18°, 16.19°] | - | 1925 |
| Median aim error at -200 ms from the first visible part | 11.36° [9.89°, 12.72°] | 10.54° [9.80°, 11.23°] | - | 1925 |
| Median aim error at -100 ms from the first visible part ▲ | 10.84° [9.46°, 12.15°] | 8.00° [7.34°, 8.49°] | - | 1925 |
| Median aim error at +0 ms from the first visible part ▲ | 10.63° [9.21°, 11.96°] | 5.22° [4.91°, 5.53°] | - | 1925 |
| Median aim error at +100 ms from the first visible part ▲ | 8.88° [7.61°, 10.15°] | 4.87° [4.54°, 5.18°] | - | 1925 |
| Median aim error at +200 ms from the first visible part ▲ | 6.60° [5.67°, 7.41°] | 5.26° [4.86°, 5.58°] | - | 1925 |
| Fire already held on the tick before the first part (prefire) ▲ | 16.1% [15.3%, 17.0%] | 23.0% [20.3%, 25.5%] | - | 1925 |
| Reaction: first press after a clean sighting, from the first part p50 | 150 ms [150 ms, 200 ms] | 200 ms [200 ms, 250 ms] | - | 1086 |
| Reaction: mean first press after a clean sighting, from the first part | 206 ms [192 ms, 221 ms] | 232 ms [217 ms, 250 ms] | - | 1086 |
| Crosshair within half-width+1.5 deg after a clean sighting, from the first part p50 | 250 ms [200 ms, 250 ms] | 250 ms [200 ms, 250 ms] | - | 1066 |
| First hit after the first part p50 ▲ | 300 ms [300 ms, 300 ms] | 250 ms [200 ms, 250 ms] | - | 1046 |
| Part sightings whose visible bout ends without a hit ▲ | 45.2% [40.1%, 50.7%] | 27.5% [25.7%, 29.7%] | - | 1925 |
| Yaw error to the hidden enemy 500 ms before the first part ▲ | 11.92° [10.75°, 13.43°] | 14.63° [13.70°, 15.63°] | - | 1925 |
| Yaw error to where the enemy will appear, 500 ms before the first part | 10.56° [9.60°, 11.51°] | 10.65° [9.39°, 11.89°] | - | 1925 |
| Crosshair parked within 5 deg of where the enemy will appear, 500 ms before | 28.2% [25.1%, 31.4%] | 29.4% [27.1%, 32.4%] | - | 1925 |
| Crosshair closer to where the enemy will appear than to the enemy, -500 ms ▲ | 54.9% [53.1%, 56.9%] | 67.0% [65.6%, 68.8%] | - | 1925 |
| View turn toward the enemy over the last 500 ms before the first part ▲ | 8.38° [6.75°, 10.66°] | 13.01° [11.44°, 14.55°] | - | 1925 |
| View turn over the last 500 ms before the first part | 11.44° [9.65°, 13.35°] | 14.53° [13.34°, 15.90°] | - | 1925 |
| Speed 400-200 ms before the first part ▲ | 114 u/s [108 u/s, 121 u/s] | 188 u/s [182 u/s, 193 u/s] | - | 1925 |
| Speed 2-1 s before the first part ▲ | 110 u/s [102 u/s, 117 u/s] | 167 u/s [159 u/s, 176 u/s] | - | 1555 |
| Hidden, no sighting next: crosshair closer to where the enemy will be in 500 ms | 49.5% [49.1%, 50.0%] | 50.5% [49.0%, 51.6%] | - | 460350 |
| Yaw error to the corner, 500 ms before the first part ▲ | 8.68° [8.10°, 9.29°] | 5.81° [5.17°, 6.44°] | - | 1190 |
| Angle to the corner, 500 ms before the first part ▲ | 10.78° [10.00°, 11.82°] | 8.41° [7.70°, 9.19°] | - | 1190 |
| Pitch to the corner (- = corner above the crosshair), 500 ms before the first part | -2.17° [-2.67°, -1.69°] | -2.72° [-3.14°, -2.39°] | - | 1190 |
| Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part ▲ | 1.22° [0.52°, 1.81°] | -1.77° [-2.04°, -1.43°] | - | 1190 |
| Yaw error to the appearance point (sightings with a corner), 500 ms before the first part | 9.50° [8.37°, 10.30°] | 10.34° [9.19°, 11.29°] | - | 1190 |
| Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part ▲ | 11.49° [9.71°, 12.91°] | 15.18° [14.24°, 16.01°] | - | 1190 |
| Crosshair within 5 deg of the corner, 500 ms before the first part ▲ | 22.8% [20.3%, 25.9%] | 29.2% [26.4%, 32.7%] | - | 1190 |
| Crosshair closer to the corner than to the enemy, 500 ms before the first part ▲ | 58.7% [56.8%, 60.6%] | 80.0% [78.8%, 81.1%] | - | 1190 |
| Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part ▲ | 36.6% [34.3%, 40.0%] | 48.2% [45.7%, 50.3%] | - | 1190 |
| Crosshair within 2 deg of the corner's edge, 500 ms before the first part ▲ | 16.6% [15.3%, 18.2%] | 23.7% [21.7%, 25.7%] | - | 1190 |
| Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part ▲ | 46.8% [43.3%, 48.9%] | 28.1% [26.8%, 29.4%] | - | 1190 |
| Angle to the corner at the first part ▲ | 9.34° [8.04°, 10.67°] | 5.74° [5.33°, 6.17°] | - | 1190 |
| Distance from the eye to the corner p50 | 181 u [146 u, 236 u] | 228 u [204 u, 248 u] | - | 1190 |
| Yaw error to the corner at -2000 ms (sightings hidden >= 2 s) | 12.09° [10.45°, 14.58°] | 14.66° [11.85°, 17.22°] | - | 641 |
| Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) ▲ | 8.80° [8.00°, 10.20°] | 5.78° [4.88°, 6.98°] | - | 641 |
| Yaw error to the corner at -500 ms (sightings hidden >= 2 s) ▲ | 8.68° [7.67°, 9.70°] | 4.66° [3.99°, 5.31°] | - | 641 |
| Yaw error to the corner at +0 ms (sightings hidden >= 2 s) ▲ | 7.27° [6.05°, 8.50°] | 3.54° [3.01°, 4.10°] | - | 641 |

Consistency: 112 pooled statistics (bots and owner together) were recomputed from the analysis scripts' own JSON; 0 differ.

## Notes

- perception statistics on the bots' logged body-part column (ext_vis_parts: their own perception); the people's are on the data repo's rebuilt column, accepted against the logged one on 2026-09-28
- 100% of the bots' duel time is against another bot (the logged opponent is the nearest living enemy); the human reference is humans against humans only
- engagements.py counts contact episodes only while exactly two players are alive; with more than two players in a session the fight statistics cover the phases where one player is dead
