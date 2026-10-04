# Bot evaluation: fc_201

Generated 2026-10-03 by `humanbot/eval/compare.py`. Captures: `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`. 8 sessions on dm/crnodoors, dm/downladder, dm/main, dm/vents; capture dates 2026-10-03.
Human reference: `human_reference.json` (2026-10-02), 397 duel minutes, 72 player-sessions, 5 people.

## Bots in this capture

| Bot | Family | Seed | Sessions | Duel minutes |
|---|---|---:|---:|---:|
| `bot:stopper:4` | stopper | 711847621 | 1 | 25.8 |
| `bot:stopper:5` | stopper | 1529931226 | 1 | 25.8 |
| `bot:strafer:4` | strafer | 231333596 | 1 | 23.5 |
| `bot:presser:5` | presser | 1152489726 | 1 | 23.5 |
| `bot:stopper:4.2` | stopper | 2458011590 | 1 | 25.6 |
| `bot:strafer:5` | strafer | 4003987225 | 1 | 25.6 |
| `bot:strafer:4.2` | strafer | 3498264623 | 1 | 24.4 |
| `bot:stopper:5.2` | stopper | 2513918026 | 1 | 24.4 |
| `bot:presser:4` | presser | 52844662 | 1 | 26.0 |
| `bot:strafer:5.2` | strafer | 3859071546 | 1 | 26.0 |
| `bot:presser:4.2` | presser | 588158740 | 1 | 26.7 |
| `bot:presser:5.2` | presser | 4131749178 | 1 | 26.7 |
| `bot:stopper:4.3` | stopper | 97502759 | 1 | 27.5 |
| `bot:strafer:5.3` | strafer | 2592015535 | 1 | 27.5 |
| `bot:strafer:4.3` | strafer | 1086809828 | 1 | 26.8 |
| `bot:strafer:5.4` | strafer | 821368291 | 1 | 26.8 |

The owner (`pstN@vsbot`) has 0.0 duel minutes against the bots; those rows are never pooled with the bots.

Bot duel time against the owner 0%, against other bots 100%.

Family mix of bot duel time (observed → reweighted to the people's mix of duel time): presser 25% → 50%, stopper 31% → 26%, strafer 44% → 25%.

## Tells (148)

Statistics whose 95% CIs for the pooled bots and the pooled humans do not overlap, ranked by the standardized gap z = (bots − humans) / √(SE²bots + SE²humans). Only statistics with enough data on both sides are ranked.

| # | Statistic | Bots | Humans | z |
|---:|---|---|---|---:|
| 1 | Centroid sightings with a body part on screen first | 33.6% [31.9%, 35.9%] | 85.9% [84.2%, 87.7%] | -36.8 |
| 2 | Crosshair closer to the corner than to the enemy, 500 ms before the first part | 56.7% [55.0%, 58.5%] | 80.0% [78.8%, 81.1%] | -20.8 |
| 3 | Respawn delay after death p50 | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | +17.2 |
| 4 | Fire held at +400 ms from sight gain | 62.0% [59.3%, 64.2%] | 87.3% [85.5%, 89.0%] | -15.8 |
| 5 | Yaw speed (LOS firefight) p99 | 719°/s [663°/s, 770°/s] | 305°/s [292°/s, 318°/s] | +14.2 |
| 6 | Crosshair closer to where the enemy will appear than to the enemy, -500 ms | 52.1% [50.7%, 53.4%] | 67.0% [65.6%, 68.8%] | -13.9 |
| 7 | Yaw speed (hidden, not firing) p99 | 860°/s [818°/s, 887°/s] | 544°/s [521°/s, 580°/s] | +13.4 |
| 8 | Median aim error at +0 ms from the first visible part | 12.25° [11.33°, 13.42°] | 5.22° [4.91°, 5.53°] | +13.0 |
| 9 | Fire held at +200 ms from sight gain | 44.9% [41.5%, 49.4%] | 77.0% [73.9%, 79.5%] | -12.7 |
| 10 | Crosshair already on the body at the first visible tick | 10.1% [9.3%, 10.9%] | 22.6% [20.9%, 24.4%] | -12.6 |
| 11 | Median aim error at +0 ms from sight gain | 17.86° [15.67°, 19.48°] | 5.22° [4.88°, 5.61°] | +12.5 |
| 12 | Yaw speed (hidden, firing) p99 | 677°/s [599°/s, 772°/s] | 194°/s [182°/s, 204°/s] | +11.3 |
| 13 | Median aim error at +100 ms from the first visible part | 10.21° [9.33°, 11.23°] | 4.87° [4.54°, 5.18°] | +10.7 |
| 14 | Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part | 46.7% [43.4%, 49.3%] | 28.1% [26.8%, 29.4%] | +10.6 |
| 15 | Mouse still between ticks (LOS firefight) | 3.0% [2.9%, 3.2%] | 1.6% [1.4%, 1.8%] | +10.2 |
| 16 | Fire held at +100 ms from sight gain | 34.0% [30.6%, 38.7%] | 61.4% [57.8%, 64.6%] | -10.1 |
| 17 | View within 30 deg of the hidden opponent, last seen 4-8 s | 52.2% [48.5%, 56.2%] | 76.4% [73.1%, 79.3%] | -9.9 |
| 18 | View within 30 deg of the hidden opponent, last seen <=250 ms | 80.6% [79.0%, 83.3%] | 92.7% [91.9%, 93.7%] | -9.7 |
| 19 | Strafe switch hazard per tick at hold age 150 ms (LOS firefight) | 23.4% [21.7%, 24.6%] | 12.4% [11.1%, 13.9%] | +9.5 |
| 20 | Angle to the corner at the first part | 10.68° [9.86°, 11.57°] | 5.74° [5.33°, 6.17°] | +9.2 |
| 21 | Median aim error at +200 ms from sight gain | 11.06° [9.62°, 12.02°] | 4.92° [4.54°, 5.23°] | +8.8 |
| 22 | Reaction: first trigger press after a clean sighting p90 | 750 ms [700 ms, 800 ms] | 400 ms [400 ms, 450 ms] | +8.8 |
| 23 | Median aim error at +100 ms from sight gain | 14.80° [12.73°, 16.68°] | 5.97° [5.54°, 6.43°] | +8.8 |
| 24 | Fire held at +0 ms from sight gain | 24.2% [21.8%, 27.0%] | 42.5% [39.1%, 46.0%] | -8.4 |
| 25 | SMG shots that hit, a part on screen, centroid hidden | 11.0% [10.2%, 11.7%] | 21.7% [18.9%, 24.3%] | -8.4 |
| 26 | Yaw speed (reloading) p50 | 30°/s [28°/s, 32°/s] | 18°/s [16°/s, 20°/s] | +8.1 |
| 27 | Side-hold duration (LOS firefight) p90 | 500 ms [450 ms, 550 ms] | 700 ms [650 ms, 700 ms] | -8.0 |
| 28 | Fire already held on the tick before sight (prefire) | 20.8% [19.0%, 22.4%] | 35.4% [32.2%, 38.6%] | -7.9 |
| 29 | Strafe switch hazard per tick at hold age 100 ms (LOS firefight) | 20.6% [16.6%, 24.0%] | 5.8% [5.2%, 6.8%] | +7.8 |
| 30 | Engagements that end in a kill | 90.0% [88.6%, 91.1%] | 79.9% [77.9%, 81.9%] | +7.8 |
| 31 | Median aim error at -100 ms from the first visible part | 12.77° [11.90°, 13.89°] | 8.00° [7.34°, 8.49°] | +7.8 |
| 32 | Median aim error at -200 ms from sight gain | 18.22° [15.93°, 20.31°] | 8.33° [7.70°, 8.92°] | +7.7 |
| 33 | Yaw error to the hidden opponent, last seen 4-8 s | 27.25° [22.92°, 31.86°] | 10.29° [9.19°, 11.80°] | +7.6 |
| 34 | Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) | 22.5% [21.2%, 23.9%] | 15.6% [14.7%, 16.9%] | +7.4 |
| 35 | Median aim error firing with LOS at (0, 128] u | 18.84° [18.19°, 19.81°] | 14.28° [13.53°, 15.19°] | +7.4 |
| 36 | Yaw error to the corner at +0 ms (sightings hidden >= 2 s) | 8.44° [7.19°, 9.19°] | 3.54° [3.01°, 4.10°] | +7.1 |
| 37 | Lead of the first part over the centroid p50 | 50 ms [50 ms, 50 ms] | 100 ms [100 ms, 100 ms] | -7.1 |
| 38 | Reloads started with the opponent dead | 69.5% [65.6%, 74.6%] | 88.2% [85.5%, 90.4%] | -7.0 |
| 39 | Crosshair within 2 deg of the corner's edge, 500 ms before the first part | 13.0% [10.9%, 15.1%] | 23.7% [21.7%, 25.7%] | -6.9 |
| 40 | Yaw speed (hidden, firing) p50 | 32°/s [30°/s, 33°/s] | 23°/s [21°/s, 25°/s] | +6.9 |
| 41 | Median aim error firing with LOS at (128, 192] u | 10.90° [10.62°, 11.32°] | 8.99° [8.66°, 9.45°] | +6.8 |
| 42 | Side-hold duration (LOS firefight) p25 | 100 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | -6.8 |
| 43 | Retreating >40 u/s in LOS firefight at distance (160, 224] | 13.8% [12.6%, 14.9%] | 6.9% [5.5%, 8.7%] | +6.8 |
| 44 | Yaw speed (LOS, not firing) p99 | 918°/s [873°/s, 952°/s] | 583°/s [505°/s, 695°/s] | +6.8 |
| 45 | Median aim error at +300 ms from sight gain | 8.98° [7.76°, 10.20°] | 4.53° [4.12°, 4.88°] | +6.8 |
| 46 | Mouse still between ticks (reloading) | 20.3% [19.2%, 21.5%] | 26.3% [25.0%, 27.6%] | -6.7 |
| 47 | Duration of decisive engagements p50 | 1950 ms [1800 ms, 2100 ms] | 1250 ms [1150 ms, 1351 ms] | +6.7 |
| 48 | Side-hold duration (LOS firefight) p75 | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | -6.6 |
| 49 | Press hazard while hidden, LOS lost 150-250 ms ago | 8.3% [7.7%, 9.0%] | 4.6% [3.9%, 5.5%] | +6.6 |
| 50 | Crosshair on body firing with LOS at (192, 256] u | 31.3% [29.5%, 32.9%] | 42.0% [39.5%, 44.4%] | -6.5 |
| 51 | View within 30 deg of the hidden opponent, last seen 1-2 s | 64.8% [61.1%, 68.4%] | 78.9% [77.1%, 81.1%] | -6.3 |
| 52 | Yaw speed (LOS firefight) p50 | 53°/s [50°/s, 57°/s] | 39°/s [36°/s, 41°/s] | +6.2 |
| 53 | Reaction: first trigger press after a clean sighting p50 | 250 ms [224 ms, 250 ms] | 150 ms [150 ms, 200 ms] | +6.2 |
| 54 | Median aim error firing with LOS at (192, 256] u | 8.38° [8.04°, 8.74°] | 6.68° [6.36°, 7.10°] | +6.2 |
| 55 | Press hazard while hidden, LOS lost over 1 s ago | 2.0% [1.7%, 2.5%] | 4.1% [3.5%, 4.6%] | -6.2 |
| 56 | Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part | 0.87° [0.16°, 1.83°] | -1.77° [-2.04°, -1.43°] | +6.1 |
| 57 | Yaw error to the hidden opponent, last seen <=250 ms | 7.11° [6.42°, 7.60°] | 4.92° [4.56°, 5.26°] | +6.0 |
| 58 | Fire held at +700 ms from sight gain | 64.8% [61.8%, 67.4%] | 77.0% [74.3%, 79.7%] | -5.9 |
| 59 | Yaw error to the corner, 500 ms before the first part | 12.51° [10.44°, 14.72°] | 5.81° [5.17°, 6.44°] | +5.9 |
| 60 | Side-hold duration (all contexts) p90 | 600 ms [600 ms, 650 ms] | 800 ms [800 ms, 850 ms] | -5.8 |
| 61 | Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago | 22.9% [19.3%, 27.6%] | 7.4% [4.7%, 10.8%] | +5.8 |
| 62 | View within 30 deg of the hidden opponent, last seen 250-500 ms | 85.6% [83.6%, 87.3%] | 92.2% [91.3%, 93.4%] | -5.8 |
| 63 | Fire held, LOS, aim error in half-widths (10, 20] | 13.7% [11.3%, 16.1%] | 4.6% [2.4%, 6.9%] | +5.7 |
| 64 | SMG shots that hit, centroid visible | 21.6% [20.3%, 22.8%] | 27.2% [25.8%, 28.5%] | -5.6 |
| 65 | Fire held, LOS, aim error in half-widths (20, 1000] | 8.2% [5.4%, 9.6%] | 0.9% [0.4%, 3.1%] | +5.5 |
| 66 | Crosshair within half-width+1.5 deg after a clean sighting p50 | 350 ms [300 ms, 350 ms] | 200 ms [200 ms, 250 ms] | +5.4 |
| 67 | Crosshair on body firing with LOS at (128, 192] u | 33.9% [32.3%, 36.2%] | 42.5% [39.8%, 44.7%] | -5.4 |
| 68 | Angle to the corner, 500 ms before the first part | 14.23° [12.57°, 16.45°] | 8.41° [7.70°, 9.19°] | +5.2 |
| 69 | Strafe switch hazard per tick at hold age 250 ms (LOS firefight) | 23.1% [21.4%, 24.3%] | 17.4% [16.0%, 19.0%] | +5.2 |
| 70 | Lean held, all duel time | 74.6% [71.5%, 77.5%] | 59.7% [55.3%, 64.0%] | +5.2 |
| 71 | Time from both alive to the first body part on screen p75 | 2700 ms [2499 ms, 3000 ms] | 1900 ms [1800 ms, 2000 ms] | +5.2 |
| 72 | Standing still (<5 u/s) (hidden, not firing) | 13.9% [12.4%, 15.5%] | 22.5% [19.5%, 25.1%] | -5.2 |
| 73 | View within 30 deg of the hidden opponent, last seen 0.5-1 s | 80.0% [77.4%, 82.4%] | 88.0% [86.8%, 89.9%] | -5.2 |
| 74 | View turn toward the enemy over the last 500 ms before the first part | 7.18° [6.18°, 9.30°] | 13.01° [11.44°, 14.55°] | -5.2 |
| 75 | Fire held with no body part visible, 1000-2000ms after one was last on screen | 13.3% [12.6%, 14.3%] | 20.5% [18.0%, 22.9%] | -5.1 |
| 76 | Standing still (<5 u/s), all duel time | 9.1% [8.5%, 9.9%] | 15.7% [13.4%, 17.7%] | -5.1 |
| 77 | First hit after the first part p50 | 400 ms [350 ms, 400 ms] | 250 ms [200 ms, 250 ms] | +5.0 |
| 78 | Strafe switch hazard per tick at hold age 50 ms (LOS firefight) | 8.0% [5.1%, 10.4%] | 1.2% [0.9%, 1.6%] | +5.0 |
| 79 | Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part | 40.3% [38.6%, 42.6%] | 48.2% [45.7%, 50.3%] | -4.9 |
| 80 | Strafe switch hazard per tick at hold age 200 ms (LOS firefight) | 22.0% [21.0%, 22.8%] | 17.0% [15.4%, 18.7%] | +4.8 |
| 81 | Share of duel time: hidden, firing | 11.6% [10.4%, 12.9%] | 15.8% [14.9%, 17.0%] | -4.8 |
| 82 | View turn over the last 500 ms before the first part | 10.11° [9.18°, 11.59°] | 14.53° [13.34°, 15.90°] | -4.8 |
| 83 | Crosshair within 5 deg of the corner, 500 ms before the first part | 18.4% [15.4%, 21.4%] | 29.2% [26.4%, 32.7%] | -4.8 |
| 84 | Yaw error to the corner at -500 ms (sightings hidden >= 2 s) | 13.13° [10.29°, 17.42°] | 4.66° [3.99°, 5.31°] | +4.7 |
| 85 | Median aim error at -500 ms from sight gain | 19.49° [17.60°, 21.70°] | 13.98° [12.90°, 15.10°] | +4.7 |
| 86 | Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) | 19.36° [14.96°, 26.28°] | 5.78° [4.88°, 6.98°] | +4.7 |
| 87 | Crosshair on body firing with LOS at (256, 384] u | 31.6% [29.9%, 33.3%] | 39.0% [36.0%, 41.4%] | -4.6 |
| 88 | Key chord forward (reloading) | 14.7% [11.2%, 19.7%] | 26.1% [23.8%, 28.6%] | -4.6 |
| 89 | Side-hold duration (LOS firefight) p50 | 200 ms [150 ms, 200 ms] | 300 ms [250 ms, 300 ms] | -4.6 |
| 90 | Retreating >40 u/s in LOS firefight at distance (224, 288] | 9.4% [8.5%, 10.6%] | 5.3% [4.2%, 6.8%] | +4.5 |
| 91 | Yaw error to the hidden opponent, last seen 1-2 s | 17.37° [15.15°, 19.86°] | 11.11° [9.95°, 12.13°] | +4.4 |
| 92 | Retreating >40 u/s in LOS firefight at distance (96, 160] | 17.1% [14.9%, 19.2%] | 10.5% [8.5%, 12.6%] | +4.3 |
| 93 | Reaction: first trigger press after a clean sighting p75 | 450 ms [400 ms, 500 ms] | 300 ms [250 ms, 300 ms] | +4.3 |
| 94 | Sightings (a body part coming on screen) per minute of duel time | 28.98/min [27.80/min, 30.02/min] | 33.29/min [31.94/min, 34.72/min] | -4.3 |
| 95 | Strafe end is a direct reverse (reloading) | 58.1% [52.4%, 62.9%] | 37.7% [32.5%, 42.7%] | +4.3 |
| 96 | Median aim error at +200 ms from the first visible part | 6.96° [6.33°, 7.72°] | 5.26° [4.86°, 5.58°] | +4.2 |
| 97 | View within 30 deg of the hidden opponent, last seen 2-4 s | 53.0% [49.3%, 57.5%] | 64.3% [61.3%, 67.3%] | -4.2 |
| 98 | Key chord back any (LOS, not firing) | 9.2% [7.9%, 10.4%] | 4.8% [3.4%, 6.7%] | +4.2 |
| 99 | Key chord neutral (reloading) | 15.1% [12.1%, 18.3%] | 8.3% [7.3%, 9.5%] | +4.1 |
| 100 | Mouse still between ticks (hidden, firing) | 8.5% [7.7%, 9.2%] | 10.9% [10.1%, 11.8%] | -4.1 |
| 101 | Median aim error at -200 ms from the first visible part | 13.50° [12.36°, 14.90°] | 10.54° [9.80°, 11.23°] | +4.1 |
| 102 | Shots per burst whose first round left with a body part visible p90 | 17 [16, 19] | 13 [12, 14] | +4.1 |
| 103 | Side-hold duration (all contexts) p50 | 200 ms [200 ms, 250 ms] | 300 ms [300 ms, 300 ms] | -4.1 |
| 104 | Shot accuracy (eligible SMG shots that hit) | 15.7% [14.6%, 17.0%] | 19.2% [18.1%, 20.3%] | -4.1 |
| 105 | Key chord pure strafe (reloading) | 30.6% [27.2%, 33.1%] | 22.3% [19.6%, 24.4%] | +4.1 |
| 106 | Median aim error at +500 ms from sight gain | 7.92° [6.95°, 9.07°] | 5.47° [5.08°, 5.88°] | +4.0 |
| 107 | Standing still (<5 u/s) (hidden, firing) | 7.1% [6.5%, 7.8%] | 10.2% [9.1%, 11.7%] | -3.9 |
| 108 | Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) | 20.1% [18.7%, 21.5%] | 16.1% [14.9%, 17.5%] | +3.9 |
| 109 | Share of duel time: LOS, not firing | 10.5% [9.6%, 11.4%] | 7.0% [5.5%, 8.4%] | +3.8 |
| 110 | Shots per burst (consecutive 100 ms shots) p90 | 15 [14, 17] | 12 [11, 13] | +3.8 |
| 111 | Crosshair on body firing with LOS at (0, 128] u | 40.4% [38.6%, 42.7%] | 48.0% [44.9%, 51.5%] | -3.8 |
| 112 | Net distance covered in 2 s from a tick with no body part on screen p50 | 122 u [114 u, 136 u] | 158 u [146 u, 173 u] | -3.7 |
| 113 | Fire held, LOS, aim error in half-widths (6, 10] | 35.8% [31.1%, 41.4%] | 22.4% [17.1%, 27.1%] | +3.7 |
| 114 | Fire held with no body part visible, 500-1000ms after one was last on screen | 21.1% [19.8%, 22.5%] | 26.4% [23.9%, 28.7%] | -3.7 |
| 115 | Release hazard while hidden, LOS lost 1 s or more ago | 15.0% [14.3%, 15.5%] | 11.3% [9.8%, 13.5%] | +3.6 |
| 116 | Share of duel time in the busiest 10% of 32u cells (dm/main) | 36.3% [32.4%, 37.4%] | 45.2% [40.5%, 48.6%] | -3.5 |
| 117 | Share of duel time in the busiest 10% of 32u cells (dm/downladder) | 39.2% [33.7%, 40.0%] | 61.4% [42.9%, 63.8%] | -3.5 |
| 118 | Crosshair on body firing with LOS at (384, 512] u | 29.4% [27.9%, 30.6%] | 35.4% [32.6%, 38.3%] | -3.5 |
| 119 | Part sightings whose visible bout ends without a hit | 34.0% [31.3%, 37.5%] | 27.5% [25.7%, 29.7%] | +3.3 |
| 120 | Release hazard while hidden, LOS lost 150-250 ms ago | 11.6% [10.0%, 13.8%] | 16.5% [13.8%, 18.7%] | -3.3 |
| 121 | Speed 2-1 s before the first part | 153 u/s [150 u/s, 157 u/s] | 167 u/s [159 u/s, 176 u/s] | -3.3 |
| 122 | Life length (lives ending in death) p50 | 10.2 s [9.2 s, 12.1 s] | 7.1 s [5.8 s, 8.2 s] | +3.2 |
| 123 | Release hazard while hidden, LOS lost 500-950 ms ago | 13.4% [12.1%, 15.3%] | 10.1% [9.2%, 11.3%] | +3.2 |
| 124 | Yaw error to the hidden opponent, last seen 2-4 s | 25.97° [21.25°, 31.12°] | 16.93° [15.18°, 19.08°] | +3.2 |
| 125 | Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) | 38.2% [29.7%, 41.4%] | 49.4% [44.0%, 50.8%] | -3.2 |
| 126 | Fire held at -200 ms from sight gain | 17.5% [16.1%, 19.4%] | 21.7% [19.7%, 23.9%] | -3.1 |
| 127 | Key chord neutral (hidden, not firing) | 20.9% [19.5%, 22.8%] | 26.1% [23.1%, 28.6%] | -3.1 |
| 128 | Yaw speed (hidden, not firing) p50 | 11°/s [11°/s, 12°/s] | 8°/s [6°/s, 10°/s] | +3.1 |
| 129 | Key chord fwd diag (reloading) | 29.5% [26.2%, 32.5%] | 36.8% [34.1%, 39.9%] | -3.1 |
| 130 | Fire held without LOS | 19.9% [17.7%, 22.6%] | 24.5% [22.7%, 26.3%] | -3.1 |
| 131 | Pitch to the corner (- = corner above the crosshair), 500 ms before the first part | -2.04° [-2.23°, -1.68°] | -2.72° [-3.14°, -2.39°] | +3.0 |
| 132 | Yaw speed (LOS, not firing) p50 | 33°/s [29°/s, 39°/s] | 11°/s [0°/s, 26°/s] | +3.0 |
| 133 | Share of duel time with a body part on screen and the centroid hidden | 7.3% [4.8%, 8.9%] | 10.5% [9.6%, 11.8%] | -2.9 |
| 134 | Speed 400-200 ms before the first part | 167 u/s [156 u/s, 178 u/s] | 188 u/s [182 u/s, 193 u/s] | -2.9 |
| 135 | Yaw error to the corner at -2000 ms (sightings hidden >= 2 s) | 23.22° [18.03°, 28.46°] | 14.66° [11.85°, 17.22°] | +2.8 |
| 136 | Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) | 18.3% [16.0%, 21.3%] | 13.7% [12.0%, 15.6%] | +2.7 |
| 137 | Crosshair parked within 5 deg of where the enemy will appear, 500 ms before | 23.1% [19.5%, 26.6%] | 29.4% [27.1%, 32.4%] | -2.7 |
| 138 | Yaw speed (reloading) p99 | 864°/s [815°/s, 889°/s] | 945°/s [909°/s, 991°/s] | -2.7 |
| 139 | Release hazard, LOS, aim error in half-widths (10, 1000] | 19.7% [18.5%, 21.4%] | 32.7% [22.9%, 41.8%] | -2.6 |
| 140 | Standing still (<5 u/s) (LOS, not firing) | 7.1% [6.5%, 7.8%] | 29.6% [12.6%, 44.0%] | -2.6 |
| 141 | Yaw error to the hidden opponent, last seen 0.5-1 s | 10.29° [9.14°, 11.58°] | 8.28° [7.48°, 9.11°] | +2.6 |
| 142 | Lean held in LOS firefights | 88.0% [85.8%, 89.5%] | 81.3% [76.6%, 85.8%] | +2.5 |
| 143 | Reloads with the opponent alive that are forced (empty clip) | 88.1% [85.1%, 91.5%] | 77.7% [69.5%, 83.9%] | +2.5 |
| 144 | Key chord pure strafe (LOS, not firing) | 36.4% [34.9%, 38.0%] | 26.7% [20.8%, 34.2%] | +2.5 |
| 145 | Strafe end is a direct reverse (hidden, not firing) | 55.2% [51.5%, 58.1%] | 46.1% [41.1%, 51.2%] | +2.5 |
| 146 | Yaw error to where the enemy will appear, 500 ms before the first part | 14.30° [12.10°, 17.82°] | 10.65° [9.39°, 11.89°] | +2.4 |
| 147 | Key chord forward (hidden, firing) | 9.7% [7.7%, 12.7%] | 6.6% [5.7%, 7.4%] | +2.3 |
| 148 | Mouse still between ticks (LOS, not firing) | 18.8% [17.2%, 20.1%] | 36.7% [20.8%, 49.8%] | -2.2 |

## Per bot

### `bot:stopper:4` (stopper, seed 711847621, 25.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.07034 | 0.1747 | +20% |
| reverse_share | 0.1382 | 0.1271 | -2% |
| side_hold_ms | 249.3 | 250 | +0% |
| lean_fight | 0.9549 | 0.9707 | +3% |
| jumps_per_min | 0.3303 | 0.1937 | -3% |
| crouch_per_min | 5.571 | 6.701 | +10% |
| walk_hidden | 0 | 0.03914 | +37% ⚠ |
| burst_median | 4.564 | 4 | -14% |
| aim_height_firing | 0.4682 | 0.467 | -1% |
| hold_angle | 0.4674 | 0.2376 | -75% ⚠ |
| counter_strafe | 0.5118 | 0.4856 | -5% |
| aim_error_fight_deg | 6.249 | 5.957 | -12% |
| reaction_ms | 153.7 | 250 | +96% ⚠ |
| mp40_share | 0.02524 | 0 | -3% |

Style fingerprint (2026-10-03, 25.8 min, styles.py features 30/30): z-distance to the human cloud centre 7.1 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 7.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.887 (humans 0.358–0.767); lf_lean 0.971 (humans 0.457–0.948); p_attack_given_los 0.52 (humans 0.547–0.861); yaw_speed_p95 334 (humans 158–271); yaw_speed_p99 778 (humans 427–754); reverse_share 0.127 (humans 0.138–0.839).

### `bot:stopper:5` (stopper, seed 1529931226, 25.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1013 | 0.1941 | +18% |
| reverse_share | 0.3253 | 0.3152 | -1% |
| side_hold_ms | 247.2 | 250 | +2% |
| lean_fight | 0.8427 | 0.8774 | +7% |
| jumps_per_min | 5.122 | 5.229 | +2% |
| crouch_per_min | 11.7 | 13.6 | +18% |
| walk_hidden | 0.07078 | 0.08426 | +13% |
| burst_median | 4.132 | 5 | +22% |
| aim_height_firing | 0.4597 | 0.443 | -9% |
| hold_angle | 0.5274 | 0.2548 | -89% ⚠ |
| counter_strafe | 0.4586 | 0.4385 | -4% |
| aim_error_fight_deg | 4.835 | 4.931 | +4% |
| reaction_ms | 109.9 | 200 | +90% ⚠ |
| mp40_share | 0.06948 | 0 | -7% |

Style fingerprint (2026-10-03, 25.8 min, styles.py features 30/30): z-distance to the human cloud centre 7.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 6.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.141 (humans 0.0109–0.0741); yaw_speed_p95 310 (humans 158–271); side_hold_p90_ms 650 (humans 700–950).

### `bot:strafer:4` (strafer, seed 231333596, 23.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.1459 | +16% |
| reverse_share | 0.4467 | 0.4641 | +2% |
| side_hold_ms | 326.3 | 250 | -51% ⚠ |
| lean_fight | 0.4941 | 0.5755 | +16% |
| jumps_per_min | 2.379 | 1.743 | -12% |
| crouch_per_min | 9.476 | 10.54 | +10% |
| walk_hidden | 0 | 0.04168 | +39% ⚠ |
| burst_median | 8 | 7 | -25% ⚠ |
| aim_height_firing | 0.4857 | 0.483 | -1% |
| hold_angle | 0.2514 | 0.3024 | +17% |
| counter_strafe | 0.4612 | 0.5959 | +26% ⚠ |
| aim_error_fight_deg | 4.667 | 4.752 | +3% |
| reaction_ms | 140.8 | 300 | +159% ⚠ |
| mp40_share | 0.9062 | 1 | +9% |

Style fingerprint (2026-10-03, 23.5 min, styles.py features 30/30): z-distance to the human cloud centre 7.4 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 5.0 vs the within-person threshold 5.6 (as close as two captures of one human).

Features outside the human min–max: pure_strafe 0.512 (humans 0.191–0.489); ducked 0.0891 (humans 0.0109–0.0741); still 0.0654 (humans 0.0832–0.227); yaw_speed_p95 336 (humans 158–271); yaw_speed_p99 767 (humans 427–754); side_hold_p90_ms 650 (humans 700–950).

### `bot:presser:5` (presser, seed 1152489726, 23.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4832 | 0.5929 | +21% |
| reverse_share | 0.8051 | 0.8259 | +3% |
| side_hold_ms | 295.2 | 200 | -63% ⚠ |
| lean_fight | 0.9204 | 0.9624 | +8% |
| jumps_per_min | 0.4317 | 0.2976 | -2% |
| crouch_per_min | 3.157 | 3.316 | +1% |
| walk_hidden | 0.02816 | 0.07929 | +48% ⚠ |
| burst_median | 6.498 | 7 | +13% |
| aim_height_firing | 0.417 | 0.399 | -10% |
| hold_angle | 0.2275 | 0.159 | -22% |
| counter_strafe | 0.1778 | 0.117 | -12% |
| aim_error_fight_deg | 4.893 | 5.69 | +32% ⚠ |
| reaction_ms | 152.5 | 300 | +148% ⚠ |
| mp40_share | 0.1123 | 1 | +89% ⚠ |

Style fingerprint (2026-10-03, 23.5 min, styles.py features 30/30): z-distance to the human cloud centre 7.1 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 7.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lf_pure_strafe 0.335 (humans 0.362–0.752); lf_fwd_diag 0.593 (humans 0.0658–0.568); lean_any 0.836 (humans 0.358–0.767); lf_lean 0.962 (humans 0.457–0.948); yaw_speed_p95 330 (humans 158–271); side_hold_p90_ms 550 (humans 700–950); lf_approach 0.65 (humans 0.131–0.64).

### `bot:stopper:4.2` (stopper, seed 2458011590, 25.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1182 | 0.1666 | +9% |
| reverse_share | 0.4266 | 0.3981 | -4% |
| side_hold_ms | 200 | 250 | +33% ⚠ |
| lean_fight | 0.8817 | 0.9228 | +8% |
| jumps_per_min | 2.965 | 2.93 | -1% |
| crouch_per_min | 8.679 | 7.032 | -15% |
| walk_hidden | 0.1056 | 0.1176 | +11% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4719 | 0.461 | -6% |
| hold_angle | 0.3772 | 0.1546 | -72% ⚠ |
| counter_strafe | 0.6632 | 0.5696 | -18% |
| aim_error_fight_deg | 4.42 | 7.614 | +130% ⚠ |
| reaction_ms | 139.9 | 250 | +110% ⚠ |
| mp40_share | 0.02794 | 0 | -3% |

Style fingerprint (2026-10-03, 25.6 min, styles.py features 30/30): z-distance to the human cloud centre 8.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.809 (humans 0.358–0.767); still 0.0791 (humans 0.0832–0.227); lf_err_med 7.61 (humans 4.08–6.63); yaw_speed_p95 364 (humans 158–271); yaw_speed_p99 771 (humans 427–754); lf_retreat 0.145 (humans 0.0243–0.141).

### `bot:strafer:5` (strafer, seed 4003987225, 25.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.1424 | +15% |
| reverse_share | 0.4657 | 0.4679 | +0% |
| side_hold_ms | 270.9 | 300 | +19% |
| lean_fight | 0.5402 | 0.5591 | +4% |
| jumps_per_min | 4.852 | 5.274 | +8% |
| crouch_per_min | 3.183 | 3.125 | -1% |
| walk_hidden | 0.03847 | 0.04499 | +6% |
| burst_median | 5.686 | 6 | +8% |
| aim_height_firing | 0.4588 | 0.463 | +2% |
| hold_angle | 0.311 | 0.1827 | -42% ⚠ |
| counter_strafe | 0.4314 | 0.3555 | -15% |
| aim_error_fight_deg | 5.446 | 8.696 | +132% ⚠ |
| reaction_ms | 161.3 | 250 | +89% ⚠ |
| mp40_share | 0 | 0 | +0% |

Style fingerprint (2026-10-03, 25.6 min, styles.py features 30/30): z-distance to the human cloud centre 7.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 6.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: attack 0.272 (humans 0.276–0.478); lf_err_med 8.69 (humans 4.08–6.63); yaw_speed_p95 349 (humans 158–271); yaw_speed_p99 783 (humans 427–754); lf_retreat 0.145 (humans 0.0243–0.141).

### `bot:strafer:4.2` (strafer, seed 3498264623, 24.4 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1522 | 0.1848 | +6% |
| reverse_share | 0.4724 | 0.475 | +0% |
| side_hold_ms | 293.3 | 300 | +4% |
| lean_fight | 0.6201 | 0.6221 | +0% |
| jumps_per_min | 3.829 | 4.216 | +7% |
| crouch_per_min | 9.64 | 9.456 | -2% |
| walk_hidden | 0 | 0.0381 | +36% ⚠ |
| burst_median | 4.228 | 5 | +19% |
| aim_height_firing | 0.4919 | 0.498 | +3% |
| hold_angle | 0.3757 | 0.1813 | -63% ⚠ |
| counter_strafe | 0.422 | 0.365 | -11% |
| aim_error_fight_deg | 5.803 | 10.11 | +175% ⚠ |
| reaction_ms | 133.2 | 250 | +117% ⚠ |
| mp40_share | 0.5841 | 1 | +42% ⚠ |

Style fingerprint (2026-10-03, 24.4 min, styles.py features 30/30): z-distance to the human cloud centre 11.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.085 (humans 0.0109–0.0741); lf_err_med 10.1 (humans 4.08–6.63); yaw_speed_med 34.3 (humans 12.6–30); yaw_speed_p95 435 (humans 158–271); yaw_speed_p99 885 (humans 427–754); lf_retreat 0.17 (humans 0.0243–0.141).

### `bot:stopper:5.2` (stopper, seed 2513918026, 24.4 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1336 | 0.179 | +9% |
| reverse_share | 0.6777 | 0.6714 | -1% |
| side_hold_ms | 226 | 250 | +16% |
| lean_fight | 0.9588 | 0.9768 | +4% |
| jumps_per_min | 2.944 | 3.193 | +5% |
| crouch_per_min | 3.426 | 3.438 | +0% |
| walk_hidden | 0.08429 | 0.1119 | +26% ⚠ |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4602 | 0.458 | -1% |
| hold_angle | 0.3824 | 0.2071 | -57% ⚠ |
| counter_strafe | 0.4565 | 0.34 | -23% |
| aim_error_fight_deg | 4.291 | 8.278 | +162% ⚠ |
| reaction_ms | 113 | 250 | +137% ⚠ |
| mp40_share | 0.07877 | 0 | -8% |

Style fingerprint (2026-10-03, 24.4 min, styles.py features 30/30): z-distance to the human cloud centre 9.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.6 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.496 (humans 0.191–0.489); lean_any 0.915 (humans 0.358–0.767); lf_lean 0.977 (humans 0.457–0.948); still 0.0728 (humans 0.0832–0.227); lf_err_med 8.28 (humans 4.08–6.63); yaw_speed_med 33 (humans 12.6–30); yaw_speed_p95 420 (humans 158–271); yaw_speed_p99 858 (humans 427–754); lf_retreat 0.16 (humans 0.0243–0.141).

### `bot:presser:4` (presser, seed 52844662, 26.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5317 | 0.3878 | -28% ⚠ |
| reverse_share | 0.8152 | 0.7703 | -6% |
| side_hold_ms | 336.6 | 200 | -91% ⚠ |
| lean_fight | 0.935 | 0.9768 | +8% |
| jumps_per_min | 0.336 | 0.4617 | +2% |
| crouch_per_min | 1.298 | 1.193 | -1% |
| walk_hidden | 0 | 0.03427 | +32% ⚠ |
| burst_median | 7.121 | 7 | -3% |
| aim_height_firing | 0.3853 | 0.327 | -31% ⚠ |
| hold_angle | 0.2402 | 0.3571 | +38% ⚠ |
| counter_strafe | 0.2193 | 0.2545 | +7% |
| aim_error_fight_deg | 4.424 | 5.527 | +45% ⚠ |
| reaction_ms | 173.4 | 275 | +102% ⚠ |
| mp40_share | 0.06486 | 0 | -6% |

Style fingerprint (2026-10-03, 26.0 min, styles.py features 30/30): z-distance to the human cloud centre 7.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.6 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.891 (humans 0.358–0.767); lf_lean 0.977 (humans 0.457–0.948); p_attack_given_los 0.528 (humans 0.547–0.861); lf_height_frac 0.327 (humans 0.352–0.54); yaw_speed_p95 349 (humans 158–271); yaw_speed_p99 849 (humans 427–754); side_hold_p90_ms 550 (humans 700–950).

### `bot:strafer:5.2` (strafer, seed 3859071546, 26.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1614 | 0.1936 | +6% |
| reverse_share | 0.5449 | 0.5333 | -2% |
| side_hold_ms | 350 | 250 | -67% ⚠ |
| lean_fight | 0.5666 | 0.7114 | +29% ⚠ |
| jumps_per_min | 0.6862 | 0.5002 | -3% |
| crouch_per_min | 3.8 | 4.117 | +3% |
| walk_hidden | 0.04377 | 0.05825 | +14% |
| burst_median | 7.775 | 8 | +6% |
| aim_height_firing | 0.4661 | 0.445 | -11% |
| hold_angle | 0.2527 | 0.3082 | +18% |
| counter_strafe | 0.3766 | 0.4789 | +20% |
| aim_error_fight_deg | 4.952 | 4.828 | -5% |
| reaction_ms | 115 | 200 | +85% ⚠ |
| mp40_share | 0.03683 | 0 | -4% |

Style fingerprint (2026-10-03, 26.0 min, styles.py features 30/30): z-distance to the human cloud centre 6.5 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 5.7 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.49 (humans 0.191–0.489); still 0.0688 (humans 0.0832–0.227); yaw_speed_p95 336 (humans 158–271); yaw_speed_p99 797 (humans 427–754); side_hold_p90_ms 650 (humans 700–950).

### `bot:presser:4.2` (presser, seed 588158740, 26.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4812 | 0.3332 | -29% ⚠ |
| reverse_share | 0.8389 | 0.8027 | -5% |
| side_hold_ms | 302.9 | 200 | -69% ⚠ |
| lean_fight | 0.9588 | 0.9845 | +5% |
| jumps_per_min | 1.077 | 0.7106 | -7% |
| crouch_per_min | 1.47 | 1.234 | -2% |
| walk_hidden | 0.004612 | 0.03131 | +25% ⚠ |
| burst_median | 7.045 | 8 | +24% |
| aim_height_firing | 0.352 | 0.308 | -23% |
| hold_angle | 0.2402 | 0.1884 | -17% |
| counter_strafe | 0.2096 | 0.2467 | +7% |
| aim_error_fight_deg | 4.382 | 6.38 | +81% ⚠ |
| reaction_ms | 183.1 | 300 | +117% ⚠ |
| mp40_share | 0.9584 | 1 | +4% |

Style fingerprint (2026-10-03, 26.7 min, styles.py features 30/30): z-distance to the human cloud centre 9.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.896 (humans 0.358–0.767); lf_lean 0.984 (humans 0.457–0.948); p_attack_given_los 0.514 (humans 0.547–0.861); lf_height_frac 0.308 (humans 0.352–0.54); yaw_speed_p95 399 (humans 158–271); yaw_speed_p99 889 (humans 427–754); side_hold_p90_ms 550 (humans 700–950).

### `bot:presser:5.2` (presser, seed 4131749178, 26.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5433 | 0.3598 | -36% ⚠ |
| reverse_share | 0.823 | 0.785 | -5% |
| side_hold_ms | 298.6 | 200 | -66% ⚠ |
| lean_fight | 0.9579 | 0.9802 | +4% |
| jumps_per_min | 0.5292 | 0.4862 | -1% |
| crouch_per_min | 2.785 | 2.394 | -4% |
| walk_hidden | 0.00312 | 0.0245 | +20% |
| burst_median | 7.004 | 8 | +25% |
| aim_height_firing | 0.4126 | 0.377 | -19% |
| hold_angle | 0.2204 | 0.2603 | +13% |
| counter_strafe | 0.1785 | 0.1494 | -6% |
| aim_error_fight_deg | 4.873 | 6.444 | +64% ⚠ |
| reaction_ms | 134.4 | 200 | +66% ⚠ |
| mp40_share | 0.08844 | 0 | -9% |

Style fingerprint (2026-10-03, 26.7 min, styles.py features 30/30): z-distance to the human cloud centre 9.0 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.2 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.917 (humans 0.358–0.767); lf_lean 0.98 (humans 0.457–0.948); walk 0.0105 (humans 0.0126–0.0881); yaw_speed_p95 394 (humans 158–271); yaw_speed_p99 863 (humans 427–754); side_hold_p90_ms 550 (humans 700–950).

### `bot:stopper:4.3` (stopper, seed 97502759, 27.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.1778 | +22% |
| reverse_share | 0.6408 | 0.6239 | -2% |
| side_hold_ms | 200 | 250 | +33% ⚠ |
| lean_fight | 0.8757 | 0.9174 | +8% |
| jumps_per_min | 1.606 | 2.145 | +10% |
| crouch_per_min | 2.514 | 1.745 | -7% |
| walk_hidden | 0.09232 | 0.1033 | +10% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4804 | 0.499 | +10% |
| hold_angle | 0.418 | 0.2166 | -66% ⚠ |
| counter_strafe | 0.5804 | 0.4156 | -32% ⚠ |
| aim_error_fight_deg | 5.509 | 11.08 | +227% ⚠ |
| reaction_ms | 127.6 | 200 | +72% ⚠ |
| mp40_share | 0.1179 | 0 | -12% |

Style fingerprint (2026-10-03, 27.5 min, styles.py features 30/30): z-distance to the human cloud centre 10.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.787 (humans 0.358–0.767); attack 0.255 (humans 0.276–0.478); lf_err_med 11.1 (humans 4.08–6.63); yaw_speed_p95 396 (humans 158–271); yaw_speed_p99 809 (humans 427–754); lf_retreat 0.168 (humans 0.0243–0.141).

### `bot:strafer:5.3` (strafer, seed 2592015535, 27.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.119 | 0.2074 | +17% |
| reverse_share | 0.4951 | 0.4684 | -4% |
| side_hold_ms | 295 | 250 | -30% ⚠ |
| lean_fight | 0.4565 | 0.4608 | +1% |
| jumps_per_min | 5.657 | 4.944 | -13% |
| crouch_per_min | 11.59 | 10.43 | -11% |
| walk_hidden | 0 | 0.02326 | +22% |
| burst_median | 4.178 | 4 | -4% |
| aim_height_firing | 0.54 | 0.555 | +8% |
| hold_angle | 0.3001 | 0.2456 | -18% |
| counter_strafe | 0.5889 | 0.4444 | -28% ⚠ |
| aim_error_fight_deg | 6.024 | 11.67 | +230% ⚠ |
| reaction_ms | 165.9 | 350 | +184% ⚠ |
| mp40_share | 0.4858 | 1 | +51% ⚠ |

Style fingerprint (2026-10-03, 27.5 min, styles.py features 30/30): z-distance to the human cloud centre 12.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 11.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.266 (humans 0.358–0.767); ducked 0.119 (humans 0.0109–0.0741); attack 0.22 (humans 0.276–0.478); p_attack_given_los 0.519 (humans 0.547–0.861); lf_err_med 11.7 (humans 4.08–6.63); lf_height_frac 0.555 (humans 0.352–0.54); yaw_speed_p95 417 (humans 158–271); yaw_speed_p99 875 (humans 427–754); side_hold_p90_ms 650 (humans 700–950); lf_retreat 0.18 (humans 0.0243–0.141).

### `bot:strafer:4.3` (strafer, seed 1086809828, 26.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1632 | 0.2569 | +18% |
| reverse_share | 0.4593 | 0.4593 | +0% |
| side_hold_ms | 314.9 | 300 | -10% |
| lean_fight | 0.5599 | 0.5904 | +6% |
| jumps_per_min | 4.813 | 5.029 | +4% |
| crouch_per_min | 9.497 | 8.01 | -14% |
| walk_hidden | 0 | 0.02361 | +22% |
| burst_median | 5.59 | 5 | -15% |
| aim_height_firing | 0.4741 | 0.521 | +25% |
| hold_angle | 0.3025 | 0.25 | -17% |
| counter_strafe | 0.2249 | 0.3009 | +15% |
| aim_error_fight_deg | 6.527 | 14.28 | +315% ⚠ |
| reaction_ms | 158.4 | 250 | +92% ⚠ |
| mp40_share | 0.9804 | 1 | +2% |

Style fingerprint (2026-10-03, 26.8 min, styles.py features 30/30): z-distance to the human cloud centre 13.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 12.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.34 (humans 0.358–0.767); ducked 0.102 (humans 0.0109–0.0741); attack 0.236 (humans 0.276–0.478); lf_err_med 14.3 (humans 4.08–6.63); yaw_speed_p95 426 (humans 158–271); yaw_speed_p99 880 (humans 427–754); lf_retreat 0.196 (humans 0.0243–0.141).

### `bot:strafer:5.4` (strafer, seed 821368291, 26.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.166 | 0.2795 | +22% |
| reverse_share | 0.4801 | 0.4601 | -3% |
| side_hold_ms | 349.1 | 300 | -33% ⚠ |
| lean_fight | 0.6654 | 0.6894 | +5% |
| jumps_per_min | 5.052 | 4.173 | -16% |
| crouch_per_min | 10.37 | 8.308 | -19% |
| walk_hidden | 0.0009382 | 0.01633 | +15% |
| burst_median | 5.755 | 6 | +6% |
| aim_height_firing | 0.4434 | 0.459 | +8% |
| hold_angle | 0.3191 | 0.2384 | -26% ⚠ |
| counter_strafe | 0.4917 | 0.5443 | +10% |
| aim_error_fight_deg | 4.261 | 10.93 | +271% ⚠ |
| reaction_ms | 175 | 300 | +125% ⚠ |
| mp40_share | 0.1202 | 0 | -12% |

Style fingerprint (2026-10-03, 26.8 min, styles.py features 30/30): z-distance to the human cloud centre 11.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.0884 (humans 0.0109–0.0741); walk 0.0101 (humans 0.0126–0.0881); attack 0.23 (humans 0.276–0.478); p_attack_given_los 0.535 (humans 0.547–0.861); still 0.0551 (humans 0.0832–0.227); lf_err_med 10.9 (humans 4.08–6.63); yaw_speed_p95 428 (humans 158–271); yaw_speed_p99 861 (humans 427–754); lf_retreat 0.18 (humans 0.0243–0.141).

## The owner against the bots (`pstN@vsbot`)

Kills of bots 0, deaths to bots 0, suicides 0. 

0 statistics of the owner against bots fall outside the human CI (owner column below, marked •).

## All statistics

Pooled bots are reweighted to the people's family mix of duel time; humans and the owner are pooled over their duel time. ▲ marks a tell. n = units (ticks, runs, shots, sightings, ...) over the blocks.

### Movement

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time: hidden, not firing | 46.7% [44.1%, 49.3%] | 49.1% [47.4%, 51.2%] | - | 495480 |
| Share of duel time: hidden, firing ▲ | 11.6% [10.4%, 12.9%] | 15.8% [14.9%, 17.0%] | - | 495480 |
| Share of duel time: LOS, not firing ▲ | 10.5% [9.6%, 11.4%] | 7.0% [5.5%, 8.4%] | - | 495480 |
| Share of duel time: LOS firefight | 20.5% [18.8%, 22.5%] | 17.6% [16.5%, 18.9%] | - | 495480 |
| Share of duel time: reloading | 10.7% [9.6%, 12.1%] | 10.5% [9.6%, 11.5%] | - | 495480 |
| Key chord pure strafe (hidden, not firing) | 30.5% [28.1%, 32.5%] | 26.0% [23.5%, 29.4%] | - | 241550 |
| Key chord fwd diag (hidden, not firing) | 27.1% [25.0%, 28.6%] | 29.9% [26.7%, 33.2%] | - | 241550 |
| Key chord forward (hidden, not firing) | 12.7% [11.6%, 14.7%] | 11.9% [10.9%, 12.8%] | - | 241550 |
| Key chord neutral (hidden, not firing) ▲ | 20.9% [19.5%, 22.8%] | 26.1% [23.1%, 28.6%] | - | 241550 |
| Key chord back any (hidden, not firing) | 8.8% [6.9%, 9.8%] | 6.1% [5.3%, 7.1%] | - | 241550 |
| Key chord pure strafe (hidden, firing) | 42.5% [38.6%, 45.4%] | 44.9% [41.6%, 48.0%] | - | 53996 |
| Key chord fwd diag (hidden, firing) | 29.2% [27.1%, 31.3%] | 28.6% [23.9%, 32.8%] | - | 53996 |
| Key chord forward (hidden, firing) ▲ | 9.7% [7.7%, 12.7%] | 6.6% [5.7%, 7.4%] | - | 53996 |
| Key chord neutral (hidden, firing) | 12.4% [11.0%, 14.2%] | 13.2% [11.2%, 15.7%] | - | 53996 |
| Key chord back any (hidden, firing) | 6.2% [4.3%, 7.2%] | 6.7% [5.9%, 7.9%] | - | 53996 |
| Key chord pure strafe (LOS, not firing) ▲ | 36.4% [34.9%, 38.0%] | 26.7% [20.8%, 34.2%] | - | 52095 |
| Key chord fwd diag (LOS, not firing) | 30.1% [27.8%, 33.5%] | 29.4% [23.0%, 38.1%] | - | 52095 |
| Key chord forward (LOS, not firing) | 9.3% [8.2%, 10.4%] | 7.5% [5.9%, 9.5%] | - | 52095 |
| Key chord neutral (LOS, not firing) | 15.0% [12.8%, 16.8%] | 31.5% [15.3%, 45.4%] | - | 52095 |
| Key chord back any (LOS, not firing) ▲ | 9.2% [7.9%, 10.4%] | 4.8% [3.4%, 6.7%] | - | 52095 |
| Key chord pure strafe (LOS firefight) | 56.4% [50.5%, 61.2%] | 58.0% [53.1%, 62.2%] | - | 98961 |
| Key chord fwd diag (LOS firefight) | 31.1% [25.9%, 38.2%] | 30.5% [25.6%, 36.7%] | - | 98961 |
| Key chord forward (LOS firefight) | 1.9% [1.3%, 2.5%] | 1.2% [1.0%, 1.4%] | - | 98961 |
| Key chord neutral (LOS firefight) | 5.4% [4.3%, 6.6%] | 5.1% [3.9%, 6.3%] | - | 98961 |
| Key chord back any (LOS firefight) | 5.3% [4.5%, 6.0%] | 5.2% [4.5%, 6.1%] | - | 98961 |
| Key chord pure strafe (reloading) ▲ | 30.6% [27.2%, 33.1%] | 22.3% [19.6%, 24.4%] | - | 48872 |
| Key chord fwd diag (reloading) ▲ | 29.5% [26.2%, 32.5%] | 36.8% [34.1%, 39.9%] | - | 48872 |
| Key chord forward (reloading) ▲ | 14.7% [11.2%, 19.7%] | 26.1% [23.8%, 28.6%] | - | 48872 |
| Key chord neutral (reloading) ▲ | 15.1% [12.1%, 18.3%] | 8.3% [7.3%, 9.5%] | - | 48872 |
| Key chord back any (reloading) | 10.1% [7.5%, 11.9%] | 6.5% [5.3%, 7.6%] | - | 48872 |
| Strafe end is a direct reverse (hidden, not firing) ▲ | 55.2% [51.5%, 58.1%] | 46.1% [41.1%, 51.2%] | - | 20349 |
| Strafe end is a direct reverse (hidden, firing) | 74.5% [68.4%, 79.0%] | 67.2% [59.7%, 74.3%] | - | 5955 |
| Strafe end is a direct reverse (LOS, not firing) | 59.5% [51.3%, 64.5%] | 52.8% [45.1%, 60.4%] | - | 5478 |
| Strafe end is a direct reverse (LOS firefight) | 76.7% [69.4%, 83.0%] | 73.3% [65.9%, 80.6%] | - | 15925 |
| Strafe end is a direct reverse (reloading) ▲ | 58.1% [52.4%, 62.9%] | 37.7% [32.5%, 42.7%] | - | 4622 |
| Strafe switch hazard per tick at hold age 50 ms (LOS firefight) ▲ | 8.0% [5.1%, 10.4%] | 1.2% [0.9%, 1.6%] | - | 16743 |
| Strafe switch hazard per tick at hold age 100 ms (LOS firefight) ▲ | 20.6% [16.6%, 24.0%] | 5.8% [5.2%, 6.8%] | - | 14949 |
| Strafe switch hazard per tick at hold age 150 ms (LOS firefight) ▲ | 23.4% [21.7%, 24.6%] | 12.4% [11.1%, 13.9%] | - | 11931 |
| Strafe switch hazard per tick at hold age 200 ms (LOS firefight) ▲ | 22.0% [21.0%, 22.8%] | 17.0% [15.4%, 18.7%] | - | 9364 |
| Strafe switch hazard per tick at hold age 250 ms (LOS firefight) ▲ | 23.1% [21.4%, 24.3%] | 17.4% [16.0%, 19.0%] | - | 7344 |
| Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) ▲ | 22.5% [21.2%, 23.9%] | 15.6% [14.7%, 16.9%] | - | 16491 |
| Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) ▲ | 20.1% [18.7%, 21.5%] | 16.1% [14.9%, 17.5%] | - | 10691 |
| Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) ▲ | 18.3% [16.0%, 21.3%] | 13.7% [12.0%, 15.6%] | - | 1564 |
| Side-hold duration (LOS firefight) p25 ▲ | 100 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | - | 15060 |
| Side-hold duration (LOS firefight) p50 ▲ | 200 ms [150 ms, 200 ms] | 300 ms [250 ms, 300 ms] | - | 15060 |
| Side-hold duration (LOS firefight) p75 ▲ | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | - | 15060 |
| Side-hold duration (LOS firefight) p90 ▲ | 500 ms [450 ms, 550 ms] | 700 ms [650 ms, 700 ms] | - | 15060 |
| Side-hold duration (all contexts) p50 ▲ | 200 ms [200 ms, 250 ms] | 300 ms [300 ms, 300 ms] | - | 51086 |
| Side-hold duration (all contexts) p90 ▲ | 600 ms [600 ms, 650 ms] | 800 ms [800 ms, 850 ms] | - | 51086 |
| Neutral gap between strafes (LOS firefight) p50 | 50 ms [50 ms, 50 ms] | 50 ms [50 ms, 50 ms] | - | 5052 |
| Neutral gap between strafes (LOS firefight) p90 | 100 ms [50 ms, 100 ms] | 100 ms [100 ms, 150 ms] | - | 5052 |
| Approaching >40 u/s in LOS firefight at distance (0, 96] | 36.6% [34.2%, 38.8%] | 37.6% [33.7%, 42.3%] | - | 10111 |
| Approaching >40 u/s in LOS firefight at distance (96, 160] | 36.5% [32.0%, 42.4%] | 36.4% [32.6%, 41.2%] | - | 14862 |
| Approaching >40 u/s in LOS firefight at distance (160, 224] | 36.9% [32.1%, 44.0%] | 35.4% [30.1%, 42.0%] | - | 16666 |
| Approaching >40 u/s in LOS firefight at distance (224, 288] | 35.3% [29.2%, 43.3%] | 38.2% [32.3%, 46.4%] | - | 15748 |
| Approaching >40 u/s in LOS firefight at distance (288, 384] | 38.5% [33.9%, 45.4%] | 39.4% [32.7%, 47.2%] | - | 18319 |
| Approaching >40 u/s in LOS firefight at distance (384, 512] | 39.0% [29.8%, 49.7%] | 41.8% [33.8%, 49.1%] | - | 15538 |
| Approaching >40 u/s in LOS firefight at distance (512, 768] | 40.0% [25.5%, 53.8%] | 33.1% [25.5%, 41.5%] | - | 7448 |
| Approaching >40 u/s in LOS firefight at distance (768, 1200] | 21.2% [8.3%, 65.0%] | 28.2% [17.6%, 42.7%] | - | 269 |
| Retreating >40 u/s in LOS firefight at distance (0, 96] | 23.9% [22.6%, 25.1%] | 22.2% [18.9%, 25.6%] | - | 10111 |
| Retreating >40 u/s in LOS firefight at distance (96, 160] ▲ | 17.1% [14.9%, 19.2%] | 10.5% [8.5%, 12.6%] | - | 14862 |
| Retreating >40 u/s in LOS firefight at distance (160, 224] ▲ | 13.8% [12.6%, 14.9%] | 6.9% [5.5%, 8.7%] | - | 16666 |
| Retreating >40 u/s in LOS firefight at distance (224, 288] ▲ | 9.4% [8.5%, 10.6%] | 5.3% [4.2%, 6.8%] | - | 15748 |
| Retreating >40 u/s in LOS firefight at distance (288, 384] | 8.1% [6.5%, 9.3%] | 5.9% [4.6%, 7.3%] | - | 18319 |
| Retreating >40 u/s in LOS firefight at distance (384, 512] | 6.2% [4.8%, 7.9%] | 4.8% [3.8%, 6.0%] | - | 15538 |
| Retreating >40 u/s in LOS firefight at distance (512, 768] | 6.0% [4.0%, 8.4%] | 4.0% [3.0%, 5.4%] | - | 7448 |
| Retreating >40 u/s in LOS firefight at distance (768, 1200] | 1.7% [0.0%, 5.6%] | 8.3% [1.5%, 15.5%] | - | 269 |
| Standing still (<5 u/s), all duel time ▲ | 9.1% [8.5%, 9.9%] | 15.7% [13.4%, 17.7%] | - | 495480 |
| Standing still (<5 u/s) (hidden, not firing) ▲ | 13.9% [12.4%, 15.5%] | 22.5% [19.5%, 25.1%] | - | 241550 |
| Standing still (<5 u/s) (hidden, firing) ▲ | 7.1% [6.5%, 7.8%] | 10.2% [9.1%, 11.7%] | - | 53996 |
| Standing still (<5 u/s) (LOS, not firing) ▲ | 7.1% [6.5%, 7.8%] | 29.6% [12.6%, 44.0%] | - | 52095 |
| Standing still (<5 u/s) (LOS firefight) | 1.8% [1.3%, 2.1%] | 2.3% [1.8%, 2.7%] | - | 98961 |
| Standing still (<5 u/s) (reloading) | 6.8% [5.3%, 8.5%] | 5.2% [4.3%, 6.2%] | - | 48872 |
| Lean held, all duel time ▲ | 74.6% [71.5%, 77.5%] | 59.7% [55.3%, 64.0%] | - | 495480 |
| Lean held in LOS firefights ▲ | 88.0% [85.8%, 89.5%] | 81.3% [76.6%, 85.8%] | - | 98961 |
| Jump presses per minute | 1.87/min [1.33/min, 2.38/min] | 1.87/min [1.41/min, 2.44/min] | - | 495480 |
| Crouch presses per minute | 4.56/min [3.43/min, 5.82/min] | 4.33/min [3.22/min, 5.51/min] | - | 495480 |
| Walk presses per minute | 6.31/min [5.52/min, 7.33/min] | 7.54/min [6.90/min, 8.11/min] | - | 495480 |
| Walking while hidden and not firing | 5.3% [4.0%, 6.8%] | 5.9% [5.0%, 7.0%] | - | 241550 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Press hazard with LOS and aim within 3 half-widths, LOS gained 0 ms ago | 23.6% [17.3%, 32.5%] | 28.2% [25.8%, 30.8%] | - | 1508 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 50-100 ms ago | 29.0% [20.7%, 38.3%] | 33.0% [30.8%, 35.6%] | - | 2254 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 150-250 ms ago | 35.7% [27.8%, 43.9%] | 35.2% [32.9%, 38.1%] | - | 2157 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago | 31.2% [24.0%, 38.2%] | 28.0% [25.2%, 32.0%] | - | 2036 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 550-1000 ms ago | 27.5% [21.7%, 35.7%] | 21.0% [16.7%, 24.6%] | - | 2455 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago ▲ | 22.9% [19.3%, 27.6%] | 7.4% [4.7%, 10.8%] | - | 3120 |
| Press hazard while hidden, LOS lost 0 ms ago | 12.2% [10.9%, 14.7%] | 12.3% [10.8%, 14.5%] | - | 2985 |
| Press hazard while hidden, LOS lost 50-100 ms ago | 10.6% [9.7%, 11.8%] | 9.5% [8.5%, 10.9%] | - | 3953 |
| Press hazard while hidden, LOS lost 150-250 ms ago ▲ | 8.3% [7.7%, 9.0%] | 4.6% [3.9%, 5.5%] | - | 5248 |
| Press hazard while hidden, LOS lost 300-500 ms ago | 8.5% [7.2%, 10.5%] | 7.0% [6.0%, 8.1%] | - | 17136 |
| Press hazard while hidden, LOS lost 550-1000 ms ago | 2.9% [2.5%, 3.5%] | 3.5% [3.2%, 3.9%] | - | 28367 |
| Press hazard while hidden, LOS lost over 1 s ago ▲ | 2.0% [1.7%, 2.5%] | 4.1% [3.5%, 4.6%] | - | 172980 |
| Release hazard, LOS, aim error in half-widths (0, 1] | 1.7% [1.4%, 2.1%] | 1.4% [1.1%, 1.7%] | - | 17884 |
| Release hazard, LOS, aim error in half-widths (1, 2] | 1.6% [1.4%, 1.9%] | 1.5% [1.3%, 1.9%] | - | 29073 |
| Release hazard, LOS, aim error in half-widths (2, 3] | 1.8% [1.5%, 2.1%] | 1.5% [1.2%, 2.0%] | - | 21701 |
| Release hazard, LOS, aim error in half-widths (3, 4] | 1.8% [1.4%, 2.2%] | 1.4% [1.0%, 2.0%] | - | 12710 |
| Release hazard, LOS, aim error in half-widths (4, 6] | 2.6% [2.1%, 3.1%] | 1.7% [1.3%, 2.3%] | - | 10026 |
| Release hazard, LOS, aim error in half-widths (6, 10] | 9.9% [7.8%, 12.0%] | 6.9% [4.4%, 9.9%] | - | 3646 |
| Release hazard, LOS, aim error in half-widths (10, 1000] ▲ | 19.7% [18.5%, 21.4%] | 32.7% [22.9%, 41.8%] | - | 2034 |
| Release hazard while hidden, LOS lost 0-100 ms ago | 5.4% [4.8%, 6.9%] | 6.3% [5.4%, 7.5%] | - | 12768 |
| Release hazard while hidden, LOS lost 150-250 ms ago ▲ | 11.6% [10.0%, 13.8%] | 16.5% [13.8%, 18.7%] | - | 7116 |
| Release hazard while hidden, LOS lost 300-450 ms ago | 29.5% [25.1%, 34.5%] | 31.5% [29.6%, 33.4%] | - | 7312 |
| Release hazard while hidden, LOS lost 500-950 ms ago ▲ | 13.4% [12.1%, 15.3%] | 10.1% [9.2%, 11.3%] | - | 5564 |
| Release hazard while hidden, LOS lost 1 s or more ago ▲ | 15.0% [14.3%, 15.5%] | 11.3% [9.8%, 13.5%] | - | 18098 |
| Fire held, LOS, aim error in half-widths (0, 0.5] | 84.5% [79.1%, 89.5%] | 89.6% [87.0%, 91.7%] | - | 6202 |
| Fire held, LOS, aim error in half-widths (0.5, 1] | 84.5% [78.6%, 88.3%] | 88.9% [86.9%, 90.5%] | - | 15058 |
| Fire held, LOS, aim error in half-widths (1, 1.5] | 85.6% [81.5%, 88.6%] | 86.3% [84.5%, 87.8%] | - | 17829 |
| Fire held, LOS, aim error in half-widths (1.5, 2] | 85.1% [82.4%, 88.3%] | 81.9% [79.8%, 83.7%] | - | 17192 |
| Fire held, LOS, aim error in half-widths (2, 3] | 82.9% [80.2%, 85.8%] | 81.0% [79.4%, 82.7%] | - | 27153 |
| Fire held, LOS, aim error in half-widths (3, 4] | 78.5% [75.2%, 82.0%] | 78.8% [76.1%, 81.2%] | - | 16738 |
| Fire held, LOS, aim error in half-widths (4, 6] | 66.9% [62.2%, 71.5%] | 67.7% [63.0%, 71.3%] | - | 15791 |
| Fire held, LOS, aim error in half-widths (6, 10] ▲ | 35.8% [31.1%, 41.4%] | 22.4% [17.1%, 27.1%] | - | 10657 |
| Fire held, LOS, aim error in half-widths (10, 20] ▲ | 13.7% [11.3%, 16.1%] | 4.6% [2.4%, 6.9%] | - | 8866 |
| Fire held, LOS, aim error in half-widths (20, 1000] ▲ | 8.2% [5.4%, 9.6%] | 0.9% [0.4%, 3.1%] | - | 14887 |
| Fire held without LOS ▲ | 19.9% [17.7%, 22.6%] | 24.5% [22.7%, 26.3%] | - | 281670 |
| Attack hold duration p50 | 200 ms [150 ms, 200 ms] | 150 ms [150 ms, 200 ms] | - | 12361 |
| Attack hold duration p90 | 950 ms [900 ms, 1000 ms] | 900 ms [800 ms, 950 ms] | - | 12361 |
| Attack holds that are taps (<=100 ms) | 39.3% [36.2%, 42.1%] | 44.3% [41.8%, 46.5%] | - | 12361 |
| Gap between attack holds p50 | 350 ms [300 ms, 350 ms] | 400 ms [350 ms, 400 ms] | - | 11801 |

### Aim

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error firing with LOS at (0, 128] u ▲ | 18.84° [18.19°, 19.81°] | 14.28° [13.53°, 15.19°] | - | 15656 |
| Crosshair on body firing with LOS at (0, 128] u ▲ | 40.4% [38.6%, 42.7%] | 48.0% [44.9%, 51.5%] | - | 15656 |
| Median aim error firing with LOS at (128, 192] u ▲ | 10.90° [10.62°, 11.32°] | 8.99° [8.66°, 9.45°] | - | 15475 |
| Crosshair on body firing with LOS at (128, 192] u ▲ | 33.9% [32.3%, 36.2%] | 42.5% [39.8%, 44.7%] | - | 15475 |
| Median aim error firing with LOS at (192, 256] u ▲ | 8.38° [8.04°, 8.74°] | 6.68° [6.36°, 7.10°] | - | 15813 |
| Crosshair on body firing with LOS at (192, 256] u ▲ | 31.3% [29.5%, 32.9%] | 42.0% [39.5%, 44.4%] | - | 15813 |
| Median aim error firing with LOS at (256, 384] u | 5.84° [5.58°, 6.01°] | 5.23° [4.96°, 5.59°] | - | 27034 |
| Crosshair on body firing with LOS at (256, 384] u ▲ | 31.6% [29.9%, 33.3%] | 39.0% [36.0%, 41.4%] | - | 27034 |
| Median aim error firing with LOS at (384, 512] u | 4.31° [4.14°, 4.51°] | 3.92° [3.67°, 4.18°] | - | 16565 |
| Crosshair on body firing with LOS at (384, 512] u ▲ | 29.4% [27.9%, 30.6%] | 35.4% [32.6%, 38.3%] | - | 16565 |
| Median aim error firing with LOS at (512, 768] u | 3.30° [3.15°, 3.53°] | 3.07° [2.80°, 3.35°] | - | 8150 |
| Crosshair on body firing with LOS at (512, 768] u | 28.2% [26.2%, 29.7%] | 32.0% [29.1%, 35.1%] | - | 8150 |
| Median aim error firing with LOS at (768, 1200] u | 1.99° [1.87°, 2.15°] | 2.14° [1.84°, 2.58°] | - | 269 |
| Crosshair on body firing with LOS at (768, 1200] u | 37.1% [33.3%, 39.5%] | 31.5% [24.3%, 39.5%] | - | 269 |
| Aim height (fraction of body) firing with LOS | 0.425 [0.412, 0.435] | 0.446 [0.418, 0.468] | - | 98962 |
| Aim height (fraction of body) with LOS, not firing | 0.62 [0.581, 0.662] | 0.662 [0.576, 0.751] | - | 72513 |

### View

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw speed (hidden, not firing) p50 ▲ | 11°/s [11°/s, 12°/s] | 8°/s [6°/s, 10°/s] | - | 239320 |
| Yaw speed (hidden, not firing) p99 ▲ | 860°/s [818°/s, 887°/s] | 544°/s [521°/s, 580°/s] | - | 239320 |
| Mouse still between ticks (hidden, not firing) | 29.5% [28.6%, 30.5%] | 32.1% [28.8%, 36.0%] | - | 241550 |
| Yaw speed (hidden, firing) p50 ▲ | 32°/s [30°/s, 33°/s] | 23°/s [21°/s, 25°/s] | - | 53996 |
| Yaw speed (hidden, firing) p99 ▲ | 677°/s [599°/s, 772°/s] | 194°/s [182°/s, 204°/s] | - | 53996 |
| Mouse still between ticks (hidden, firing) ▲ | 8.5% [7.7%, 9.2%] | 10.9% [10.1%, 11.8%] | - | 53996 |
| Yaw speed (LOS, not firing) p50 ▲ | 33°/s [29°/s, 39°/s] | 11°/s [0°/s, 26°/s] | - | 52090 |
| Yaw speed (LOS, not firing) p99 ▲ | 918°/s [873°/s, 952°/s] | 583°/s [505°/s, 695°/s] | - | 52090 |
| Mouse still between ticks (LOS, not firing) ▲ | 18.8% [17.2%, 20.1%] | 36.7% [20.8%, 49.8%] | - | 52095 |
| Yaw speed (LOS firefight) p50 ▲ | 53°/s [50°/s, 57°/s] | 39°/s [36°/s, 41°/s] | - | 98961 |
| Yaw speed (LOS firefight) p99 ▲ | 719°/s [663°/s, 770°/s] | 305°/s [292°/s, 318°/s] | - | 98961 |
| Mouse still between ticks (LOS firefight) ▲ | 3.0% [2.9%, 3.2%] | 1.6% [1.4%, 1.8%] | - | 98961 |
| Yaw speed (reloading) p50 ▲ | 30°/s [28°/s, 32°/s] | 18°/s [16°/s, 20°/s] | - | 48872 |
| Yaw speed (reloading) p99 ▲ | 864°/s [815°/s, 889°/s] | 945°/s [909°/s, 991°/s] | - | 48872 |
| Mouse still between ticks (reloading) ▲ | 20.3% [19.2%, 21.5%] | 26.3% [25.0%, 27.6%] | - | 48872 |
| Peak yaw speed of 90-135 deg turns p50 | 650°/s [642°/s, 669°/s] | 619°/s [595°/s, 655°/s] | - | 3967 |
| Peak yaw speed of 135-180 deg turns p50 | 862°/s [849°/s, 875°/s] | 852°/s [828°/s, 887°/s] | - | 3392 |
| Peak yaw speed of 180-360 deg turns p50 | 1149°/s [1070°/s, 1200°/s] | 1061°/s [1020°/s, 1126°/s] | - | 659 |

### Belief

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw error to the hidden opponent, last seen <=250 ms ▲ | 7.11° [6.42°, 7.60°] | 4.92° [4.56°, 5.26°] | - | 33381 |
| View within 30 deg of the hidden opponent, last seen <=250 ms ▲ | 80.6% [79.0%, 83.3%] | 92.7% [91.9%, 93.7%] | - | 33381 |
| Yaw error to the hidden opponent, last seen 250-500 ms | 7.30° [6.73°, 7.97°] | 6.66° [5.99°, 7.28°] | - | 17800 |
| View within 30 deg of the hidden opponent, last seen 250-500 ms ▲ | 85.6% [83.6%, 87.3%] | 92.2% [91.3%, 93.4%] | - | 17800 |
| Yaw error to the hidden opponent, last seen 0.5-1 s ▲ | 10.29° [9.14°, 11.58°] | 8.28° [7.48°, 9.11°] | - | 18263 |
| View within 30 deg of the hidden opponent, last seen 0.5-1 s ▲ | 80.0% [77.4%, 82.4%] | 88.0% [86.8%, 89.9%] | - | 18263 |
| Yaw error to the hidden opponent, last seen 1-2 s ▲ | 17.37° [15.15°, 19.86°] | 11.11° [9.95°, 12.13°] | - | 13305 |
| View within 30 deg of the hidden opponent, last seen 1-2 s ▲ | 64.8% [61.1%, 68.4%] | 78.9% [77.1%, 81.1%] | - | 13305 |
| Yaw error to the hidden opponent, last seen 2-4 s ▲ | 25.97° [21.25°, 31.12°] | 16.93° [15.18°, 19.08°] | - | 47539 |
| View within 30 deg of the hidden opponent, last seen 2-4 s ▲ | 53.0% [49.3%, 57.5%] | 64.3% [61.3%, 67.3%] | - | 47539 |
| Yaw error to the hidden opponent, last seen 4-8 s ▲ | 27.25° [22.92°, 31.86°] | 10.29° [9.19°, 11.80°] | - | 58798 |
| View within 30 deg of the hidden opponent, last seen 4-8 s ▲ | 52.2% [48.5%, 56.2%] | 76.4% [73.1%, 79.3%] | - | 58798 |
| Yaw error to the hidden opponent, last seen >8 s | 32.16° [23.95°, 36.84°] | 20.07° [12.82°, 29.16°] | - | 20088 |
| View within 30 deg of the hidden opponent, last seen >8 s | 48.1% [44.5%, 55.6%] | 65.1% [54.8%, 74.8%] | - | 20088 |
| Yaw error to the hidden opponent, last seen never seen this life | 25.94° [21.42°, 32.72°] | 19.23° [16.93°, 24.71°] | - | 114830 |
| View within 30 deg of the hidden opponent, last seen never seen this life | 54.2% [47.1%, 59.6%] | 62.5% [56.0%, 67.2%] | - | 114830 |

### Space

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) ▲ | 38.2% [29.7%, 41.4%] | 49.4% [44.0%, 50.8%] | - | 118420 |
| Share of duel time in the busiest 10% of 32u cells (dm/main) ▲ | 36.3% [32.4%, 37.4%] | 45.2% [40.5%, 48.6%] | - | 120070 |
| Share of duel time in the busiest 10% of 32u cells (dm/vents) | 40.2% [36.0%, 40.5%] | 44.3% [39.2%, 46.8%] | - | 126540 |
| Share of duel time in the busiest 10% of 32u cells (dm/downladder) ▲ | 39.2% [33.7%, 40.0%] | 61.4% [42.9%, 63.8%] | - | 130440 |

### Acquisition

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error at -500 ms from sight gain ▲ | 19.49° [17.60°, 21.70°] | 13.98° [12.90°, 15.10°] | - | 5149 |
| Median aim error at -200 ms from sight gain ▲ | 18.22° [15.93°, 20.31°] | 8.33° [7.70°, 8.92°] | - | 5160 |
| Median aim error at +0 ms from sight gain ▲ | 17.86° [15.67°, 19.48°] | 5.22° [4.88°, 5.61°] | - | 5162 |
| Median aim error at +100 ms from sight gain ▲ | 14.80° [12.73°, 16.68°] | 5.97° [5.54°, 6.43°] | - | 5162 |
| Median aim error at +200 ms from sight gain ▲ | 11.06° [9.62°, 12.02°] | 4.92° [4.54°, 5.23°] | - | 5162 |
| Median aim error at +300 ms from sight gain ▲ | 8.98° [7.76°, 10.20°] | 4.53° [4.12°, 4.88°] | - | 5105 |
| Median aim error at +500 ms from sight gain ▲ | 7.92° [6.95°, 9.07°] | 5.47° [5.08°, 5.88°] | - | 4823 |
| Median aim error at +1000 ms from sight gain | 7.73° [6.90°, 8.85°] | 7.25° [6.72°, 7.70°] | - | 4030 |
| Fire held at -500 ms from sight gain | 19.9% [18.5%, 21.4%] | 18.2% [16.5%, 20.2%] | - | 5162 |
| Fire held at -200 ms from sight gain ▲ | 17.5% [16.1%, 19.4%] | 21.7% [19.7%, 23.9%] | - | 5162 |
| Fire held at +0 ms from sight gain ▲ | 24.2% [21.8%, 27.0%] | 42.5% [39.1%, 46.0%] | - | 5162 |
| Fire held at +100 ms from sight gain ▲ | 34.0% [30.6%, 38.7%] | 61.4% [57.8%, 64.6%] | - | 5162 |
| Fire held at +200 ms from sight gain ▲ | 44.9% [41.5%, 49.4%] | 77.0% [73.9%, 79.5%] | - | 5162 |
| Fire held at +400 ms from sight gain ▲ | 62.0% [59.3%, 64.2%] | 87.3% [85.5%, 89.0%] | - | 5073 |
| Fire held at +700 ms from sight gain ▲ | 64.8% [61.8%, 67.4%] | 77.0% [74.3%, 79.7%] | - | 4844 |
| Fire held at +1000 ms from sight gain | 63.0% [61.0%, 65.0%] | 63.4% [60.9%, 66.4%] | - | 4597 |
| Fire held at +1450 ms from sight gain | 57.0% [55.6%, 58.5%] | 53.6% [49.3%, 58.5%] | - | 4180 |
| Reaction: first trigger press after a clean sighting p25 | 150 ms [100 ms, 150 ms] | 50 ms [50 ms, 100 ms] | - | 2798 |
| Reaction: first trigger press after a clean sighting p50 ▲ | 250 ms [224 ms, 250 ms] | 150 ms [150 ms, 200 ms] | - | 2798 |
| Reaction: first trigger press after a clean sighting p75 ▲ | 450 ms [400 ms, 500 ms] | 300 ms [250 ms, 300 ms] | - | 2798 |
| Reaction: first trigger press after a clean sighting p90 ▲ | 750 ms [700 ms, 800 ms] | 400 ms [400 ms, 450 ms] | - | 2798 |
| Crosshair within half-width+1.5 deg after a clean sighting p50 ▲ | 350 ms [300 ms, 350 ms] | 200 ms [200 ms, 250 ms] | - | 2585 |
| Crosshair already on the body at the first visible tick ▲ | 10.1% [9.3%, 10.9%] | 22.6% [20.9%, 24.4%] | - | 5162 |
| Fire already held on the tick before sight (prefire) ▲ | 20.8% [19.0%, 22.4%] | 35.4% [32.2%, 38.6%] | - | 5162 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shots per burst (consecutive 100 ms shots) p50 | 5 [4, 5] | 5 [4, 5] | - | 11884 |
| Shots per burst (consecutive 100 ms shots) p90 ▲ | 15 [14, 17] | 12 [11, 13] | - | 11884 |

### Combat

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shot accuracy (eligible SMG shots that hit) ▲ | 15.7% [14.6%, 17.0%] | 19.2% [18.1%, 20.3%] | - | 74181 |

### Lives

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Respawn delay after death p50 ▲ | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | - | 2231 |
| Life length (lives ending in death) p50 ▲ | 10.2 s [9.2 s, 12.1 s] | 7.1 s [5.8 s, 8.2 s] | - | 2236 |

### Reload

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Reload within 3 s of a kill, rounds left 0 | 100.0% [100.0%, 100.0%] | 95.5% [86.4%, 100.0%] | - | 45 |
| Reload within 3 s of a kill, rounds left 1-3 | 95.0% [90.7%, 99.1%] | 96.4% [92.6%, 99.2%] | - | 112 |
| Reload within 3 s of a kill, rounds left 4-6 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 155 |
| Reload within 3 s of a kill, rounds left 7-10 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 266 |
| Reload within 3 s of a kill, rounds left 11-15 | 98.4% [97.3%, 99.5%] | 98.8% [97.9%, 99.5%] | - | 412 |
| Reload within 3 s of a kill, rounds left 16-20 | 97.8% [96.1%, 99.0%] | 95.7% [93.5%, 97.7%] | - | 474 |
| Reload within 3 s of a kill, rounds left 21-25 | 84.3% [81.8%, 87.1%] | 83.1% [76.5%, 88.5%] | - | 443 |
| Reload within 3 s of a kill, rounds left 26-29 | 61.7% [54.4%, 67.8%] | 63.4% [52.8%, 75.0%] | - | 291 |
| Reload within 3 s of a kill, rounds left 30-32 | 85.6% [64.7%, 95.5%] | 73.7% [50.0%, 92.3%] | - | 36 |
| Delay from kill to reload (reloads within 3 s) p50 | 650 ms [650 ms, 650 ms] | 600 ms [600 ms, 650 ms] | - | 2021 |
| Reloads started with the opponent dead ▲ | 69.5% [65.6%, 74.6%] | 88.2% [85.5%, 90.4%] | - | 2806 |
| Reloads with the opponent alive that are forced (empty clip) ▲ | 88.1% [85.1%, 91.5%] | 77.7% [69.5%, 83.9%] | - | 782 |

### Fights

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Engagements that end in a kill ▲ | 90.0% [88.6%, 91.1%] | 79.9% [77.9%, 81.9%] | - | 5040 |
| First hitter wins (decisive engagements) | 65.7% [64.3%, 67.2%] | 65.4% [63.7%, 66.7%] | - | 4454 |
| First shooter wins (decisive engagements) | 54.2% [51.8%, 56.9%] | 55.0% [52.5%, 56.6%] | - | 4454 |
| Duration of decisive engagements p50 ▲ | 1950 ms [1800 ms, 2100 ms] | 1250 ms [1150 ms, 1351 ms] | - | 4454 |
| Engagement start distance p50 | 410 u [376 u, 451 u] | 427 u [398 u, 470 u] | - | 5040 |
| Decisive engagements won (subject's own) | 48.8% [44.9%, 53.3%] | 50.0% [45.6%, 54.2%] | - | 4454 |

### Perception

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time with a body part on screen and the centroid hidden ▲ | 7.3% [4.8%, 8.9%] | 10.5% [9.6%, 11.8%] | - | 495480 |
| Share of duel time with a body part of the enemy on screen | 36.9% [34.8%, 39.0%] | 34.0% [31.6%, 36.5%] | - | 495480 |
| Sightings (a body part coming on screen) per minute of duel time ▲ | 28.98/min [27.80/min, 30.02/min] | 33.29/min [31.94/min, 34.72/min] | - | 495480 |
| Spell with no body part on screen between two sightings p50 | 300 ms [200 ms, 400 ms] | 400 ms [300 ms, 500 ms] | - | 7146 |
| Spell with no body part on screen between two sightings p75 | 738 ms [650 ms, 850 ms] | 900 ms [800 ms, 1000 ms] | - | 7146 |
| Spell with no body part on screen between two sightings p90 | 1650 ms [1400 ms, 1950 ms] | 1750 ms [1600 ms, 1950 ms] | - | 7146 |
| Time from both alive to the first body part on screen p50 | 1450 ms [1350 ms, 1600 ms] | 1350 ms [1300 ms, 1400 ms] | - | 4478 |
| Time from both alive to the first body part on screen p75 ▲ | 2700 ms [2499 ms, 3000 ms] | 1900 ms [1800 ms, 2000 ms] | - | 4478 |
| Net distance covered in 2 s from a tick with no body part on screen p50 ▲ | 122 u [114 u, 136 u] | 158 u [146 u, 173 u] | - | 26045 |
| Net distance covered in 2 s from a tick with no body part on screen p25 | 62 u [58 u, 68 u] | 66 u [54 u, 78 u] | - | 26045 |
| Fire held with no body part of the enemy visible (loaded, not reloading) | 14.8% [13.5%, 16.2%] | 17.3% [15.8%, 19.2%] | - | 283500 |
| Fire held with no body part visible, 0-500ms after one was last on screen | 46.6% [45.0%, 48.4%] | 43.2% [40.9%, 45.8%] | - | 38741 |
| Fire held with no body part visible, 500-1000ms after one was last on screen ▲ | 21.1% [19.8%, 22.5%] | 26.4% [23.9%, 28.7%] | - | 19260 |
| Fire held with no body part visible, 1000-2000ms after one was last on screen ▲ | 13.3% [12.6%, 14.3%] | 20.5% [18.0%, 22.9%] | - | 18552 |
| Fire held with no body part visible, 2000-5000ms after one was last on screen | 10.8% [9.0%, 13.1%] | 7.7% [6.4%, 9.1%] | - | 70326 |
| Fire held with no body part visible, gt5000ms after one was last on screen | 6.9% [5.8%, 8.1%] | 4.3% [3.1%, 6.0%] | - | 58928 |
| Attack holds begun loaded that are taps (<=100 ms), with a body part visible | 13.5% [11.9%, 15.2%] | 15.4% [13.1%, 17.6%] | - | 4357 |
| Attack holds begun loaded that are taps (<=100 ms), with no body part visible | 42.0% [38.8%, 45.9%] | 45.9% [42.2%, 48.9%] | - | 5985 |
| Shots per burst whose first round left with a body part visible p50 | 6 [6, 6] | 6 [5, 6] | - | 7657 |
| Shots per burst whose first round left with a body part visible p90 ▲ | 17 [16, 19] | 13 [12, 14] | - | 7657 |
| SMG shots that hit, a part on screen, centroid hidden ▲ | 11.0% [10.2%, 11.7%] | 21.7% [18.9%, 24.3%] | - | 8113 |
| SMG shots that hit, centroid visible ▲ | 21.6% [20.3%, 22.8%] | 27.2% [25.8%, 28.5%] | - | 49466 |
| Centroid sightings with a body part on screen first ▲ | 33.6% [31.9%, 35.9%] | 85.9% [84.2%, 87.7%] | - | 5149 |
| Lead of the first part over the centroid p50 ▲ | 50 ms [50 ms, 50 ms] | 100 ms [100 ms, 100 ms] | - | 1665 |
| Median aim error at -500 ms from the first visible part | 16.41° [14.48°, 18.34°] | 15.23° [14.18°, 16.19°] | - | 2753 |
| Median aim error at -200 ms from the first visible part ▲ | 13.50° [12.36°, 14.90°] | 10.54° [9.80°, 11.23°] | - | 2753 |
| Median aim error at -100 ms from the first visible part ▲ | 12.77° [11.90°, 13.89°] | 8.00° [7.34°, 8.49°] | - | 2753 |
| Median aim error at +0 ms from the first visible part ▲ | 12.25° [11.33°, 13.42°] | 5.22° [4.91°, 5.53°] | - | 2753 |
| Median aim error at +100 ms from the first visible part ▲ | 10.21° [9.33°, 11.23°] | 4.87° [4.54°, 5.18°] | - | 2753 |
| Median aim error at +200 ms from the first visible part ▲ | 6.96° [6.33°, 7.72°] | 5.26° [4.86°, 5.58°] | - | 2753 |
| Fire already held on the tick before the first part (prefire) | 18.2% [15.3%, 20.6%] | 23.0% [20.3%, 25.5%] | - | 2753 |
| Reaction: first press after a clean sighting, from the first part p50 | 250 ms [200 ms, 250 ms] | 200 ms [200 ms, 250 ms] | - | 1702 |
| Reaction: mean first press after a clean sighting, from the first part | 263 ms [226 ms, 288 ms] | 232 ms [217 ms, 250 ms] | - | 1702 |
| Crosshair within half-width+1.5 deg after a clean sighting, from the first part p50 | 300 ms [250 ms, 300 ms] | 250 ms [200 ms, 250 ms] | - | 1664 |
| First hit after the first part p50 ▲ | 400 ms [350 ms, 400 ms] | 250 ms [200 ms, 250 ms] | - | 1823 |
| Part sightings whose visible bout ends without a hit ▲ | 34.0% [31.3%, 37.5%] | 27.5% [25.7%, 29.7%] | - | 2753 |
| Yaw error to the hidden enemy 500 ms before the first part | 15.03° [12.68°, 17.55°] | 14.63° [13.70°, 15.63°] | - | 2753 |
| Yaw error to where the enemy will appear, 500 ms before the first part ▲ | 14.30° [12.10°, 17.82°] | 10.65° [9.39°, 11.89°] | - | 2753 |
| Crosshair parked within 5 deg of where the enemy will appear, 500 ms before ▲ | 23.1% [19.5%, 26.6%] | 29.4% [27.1%, 32.4%] | - | 2753 |
| Crosshair closer to where the enemy will appear than to the enemy, -500 ms ▲ | 52.1% [50.7%, 53.4%] | 67.0% [65.6%, 68.8%] | - | 2753 |
| View turn toward the enemy over the last 500 ms before the first part ▲ | 7.18° [6.18°, 9.30°] | 13.01° [11.44°, 14.55°] | - | 2753 |
| View turn over the last 500 ms before the first part ▲ | 10.11° [9.18°, 11.59°] | 14.53° [13.34°, 15.90°] | - | 2753 |
| Speed 400-200 ms before the first part ▲ | 167 u/s [156 u/s, 178 u/s] | 188 u/s [182 u/s, 193 u/s] | - | 2753 |
| Speed 2-1 s before the first part ▲ | 153 u/s [150 u/s, 157 u/s] | 167 u/s [159 u/s, 176 u/s] | - | 1505 |
| Hidden, no sighting next: crosshair closer to where the enemy will be in 500 ms | 48.9% [47.7%, 49.9%] | 50.5% [49.0%, 51.6%] | - | 209610 |
| Yaw error to the corner, 500 ms before the first part ▲ | 12.51° [10.44°, 14.72°] | 5.81° [5.17°, 6.44°] | - | 1895 |
| Angle to the corner, 500 ms before the first part ▲ | 14.23° [12.57°, 16.45°] | 8.41° [7.70°, 9.19°] | - | 1895 |
| Pitch to the corner (- = corner above the crosshair), 500 ms before the first part ▲ | -2.04° [-2.23°, -1.68°] | -2.72° [-3.14°, -2.39°] | - | 1895 |
| Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part ▲ | 0.87° [0.16°, 1.83°] | -1.77° [-2.04°, -1.43°] | - | 1895 |
| Yaw error to the appearance point (sightings with a corner), 500 ms before the first part | 12.73° [10.68°, 15.02°] | 10.34° [9.19°, 11.29°] | - | 1895 |
| Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part | 14.18° [12.04°, 16.62°] | 15.18° [14.24°, 16.01°] | - | 1895 |
| Crosshair within 5 deg of the corner, 500 ms before the first part ▲ | 18.4% [15.4%, 21.4%] | 29.2% [26.4%, 32.7%] | - | 1895 |
| Crosshair closer to the corner than to the enemy, 500 ms before the first part ▲ | 56.7% [55.0%, 58.5%] | 80.0% [78.8%, 81.1%] | - | 1895 |
| Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part ▲ | 40.3% [38.6%, 42.6%] | 48.2% [45.7%, 50.3%] | - | 1895 |
| Crosshair within 2 deg of the corner's edge, 500 ms before the first part ▲ | 13.0% [10.9%, 15.1%] | 23.7% [21.7%, 25.7%] | - | 1895 |
| Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part ▲ | 46.7% [43.4%, 49.3%] | 28.1% [26.8%, 29.4%] | - | 1895 |
| Angle to the corner at the first part ▲ | 10.68° [9.86°, 11.57°] | 5.74° [5.33°, 6.17°] | - | 1895 |
| Distance from the eye to the corner p50 | 243 u [212 u, 277 u] | 228 u [204 u, 248 u] | - | 1895 |
| Yaw error to the corner at -2000 ms (sightings hidden >= 2 s) ▲ | 23.22° [18.03°, 28.46°] | 14.66° [11.85°, 17.22°] | - | 521 |
| Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) ▲ | 19.36° [14.96°, 26.28°] | 5.78° [4.88°, 6.98°] | - | 521 |
| Yaw error to the corner at -500 ms (sightings hidden >= 2 s) ▲ | 13.13° [10.29°, 17.42°] | 4.66° [3.99°, 5.31°] | - | 521 |
| Yaw error to the corner at +0 ms (sightings hidden >= 2 s) ▲ | 8.44° [7.19°, 9.19°] | 3.54° [3.01°, 4.10°] | - | 521 |

Consistency: 112 pooled statistics (bots and owner together) were recomputed from the analysis scripts' own JSON; 0 differ.

## Notes

- perception statistics on the bots' logged body-part column (ext_vis_parts: their own perception); the people's are on the data repo's rebuilt column, accepted against the logged one on 2026-09-28
- 100% of the bots' duel time is against another bot (the logged opponent is the nearest living enemy); the human reference is humans against humans only
- engagements.py counts contact episodes only while exactly two players are alive; with more than two players in a session the fight statistics cover the phases where one player is dead
