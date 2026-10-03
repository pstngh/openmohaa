# Bot evaluation: 2026-10-02_movement_realmaps

Generated 2026-10-02 by `humanbot/eval/compare.py`. Captures: `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`. 8 sessions on dm/crnodoors, dm/downladder, dm/main, dm/vents; capture dates 2026-10-02.
Human reference: `human_reference.json` (2026-10-02), 397 duel minutes, 72 player-sessions, 5 people.

## Bots in this capture

| Bot | Family | Seed | Sessions | Duel minutes |
|---|---|---:|---:|---:|
| `bot:stopper:4` | stopper | 711847621 | 1 | 26.4 |
| `bot:stopper:5` | stopper | 1529931226 | 1 | 26.4 |
| `bot:strafer:4` | strafer | 231333596 | 1 | 23.7 |
| `bot:presser:5` | presser | 1152489726 | 1 | 23.7 |
| `bot:stopper:4.2` | stopper | 2458011590 | 1 | 25.6 |
| `bot:strafer:5` | strafer | 4003987225 | 1 | 25.6 |
| `bot:strafer:4.2` | strafer | 3498264623 | 1 | 25.0 |
| `bot:stopper:5.2` | stopper | 2513918026 | 1 | 25.0 |
| `bot:presser:4` | presser | 52844662 | 1 | 25.0 |
| `bot:strafer:5.2` | strafer | 3859071546 | 1 | 25.0 |
| `bot:presser:4.2` | presser | 588158740 | 1 | 25.0 |
| `bot:presser:5.2` | presser | 4131749178 | 1 | 25.0 |
| `bot:stopper:4.3` | stopper | 97502759 | 1 | 27.9 |
| `bot:strafer:5.3` | strafer | 2592015535 | 1 | 27.9 |
| `bot:strafer:4.3` | strafer | 1086809828 | 1 | 28.3 |
| `bot:strafer:5.4` | strafer | 821368291 | 1 | 28.3 |

The owner (`pstN@vsbot`) has 0.0 duel minutes against the bots; those rows are never pooled with the bots.

Bot duel time against the owner 0%, against other bots 100%.

Family mix of bot duel time (observed → reweighted to the people's mix of duel time): presser 24% → 50%, stopper 32% → 26%, strafer 44% → 25%.

## Tells (135)

Statistics whose 95% CIs for the pooled bots and the pooled humans do not overlap, ranked by the standardized gap z = (bots − humans) / √(SE²bots + SE²humans). Only statistics with enough data on both sides are ranked.

| # | Statistic | Bots | Humans | z |
|---:|---|---|---|---:|
| 1 | Centroid sightings with a body part on screen first | 33.0% [30.1%, 36.4%] | 85.9% [84.2%, 87.7%] | -27.8 |
| 2 | Side-hold duration (all contexts) p50 | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | -16.4 |
| 3 | Crosshair closer to the corner than to the enemy, 500 ms before the first part | 57.5% [54.4%, 59.9%] | 80.0% [78.8%, 81.1%] | -15.1 |
| 4 | Yaw speed (hidden, firing) p99 | 546°/s [505°/s, 594°/s] | 194°/s [182°/s, 204°/s] | +15.1 |
| 5 | Respawn delay after death p50 | 3000 ms [2999 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | +14.7 |
| 6 | Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) | 26.9% [25.9%, 27.8%] | 15.6% [14.7%, 16.9%] | +14.3 |
| 7 | Side-hold duration (LOS firefight) p25 | 100 ms [100 ms, 100 ms] | 200 ms [200 ms, 200 ms] | -14.2 |
| 8 | Yaw speed (hidden, not firing) p99 | 766°/s [756°/s, 777°/s] | 544°/s [521°/s, 580°/s] | +13.2 |
| 9 | Fire held at +200 ms from sight gain | 47.9% [43.7%, 51.8%] | 77.0% [73.9%, 79.5%] | -11.9 |
| 10 | Fire held at +400 ms from sight gain | 64.8% [60.5%, 67.8%] | 87.3% [85.5%, 89.0%] | -11.4 |
| 11 | Crosshair closer to where the enemy will appear than to the enemy, -500 ms | 50.7% [47.6%, 53.0%] | 67.0% [65.6%, 68.8%] | -10.5 |
| 12 | Fire held at +100 ms from sight gain | 35.4% [31.8%, 38.6%] | 61.4% [57.8%, 64.6%] | -10.4 |
| 13 | Side-hold duration (all contexts) p90 | 550 ms [550 ms, 550 ms] | 800 ms [800 ms, 850 ms] | -10.4 |
| 14 | Yaw speed (LOS firefight) p99 | 549°/s [504°/s, 591°/s] | 305°/s [292°/s, 318°/s] | +10.3 |
| 15 | Crosshair already on the body at the first visible tick | 12.0% [11.2%, 13.1%] | 22.6% [20.9%, 24.4%] | -10.2 |
| 16 | Mouse still between ticks (LOS firefight) | 3.1% [2.9%, 3.4%] | 1.6% [1.4%, 1.8%] | +9.9 |
| 17 | Median aim error at +0 ms from the first visible part | 10.27° [9.29°, 11.11°] | 5.22° [4.91°, 5.53°] | +9.9 |
| 18 | View within 30 deg of the hidden opponent, last seen 4-8 s | 53.2% [50.3%, 56.8%] | 76.4% [73.1%, 79.3%] | -9.7 |
| 19 | Strafe switch hazard per tick at hold age 150 ms (LOS firefight) | 24.1% [21.5%, 25.9%] | 12.4% [11.1%, 13.9%] | +9.2 |
| 20 | Median aim error at +0 ms from sight gain | 13.56° [11.81°, 15.22°] | 5.22° [4.88°, 5.61°] | +9.1 |
| 21 | Yaw error to the corner at -500 ms (sightings hidden >= 2 s) | 10.67° [9.47°, 11.65°] | 4.66° [3.99°, 5.31°] | +9.0 |
| 22 | Side-hold duration (LOS firefight) p75 | 300 ms [250 ms, 300 ms] | 450 ms [449 ms, 450 ms] | -8.7 |
| 23 | Side-hold duration (LOS firefight) p90 | 450 ms [400 ms, 450 ms] | 700 ms [650 ms, 700 ms] | -8.5 |
| 24 | Strafe switch hazard per tick at hold age 200 ms (LOS firefight) | 24.9% [24.3%, 25.7%] | 17.0% [15.4%, 18.7%] | +8.4 |
| 25 | Engagements that end in a kill | 90.1% [89.2%, 90.9%] | 79.9% [77.9%, 81.9%] | +8.3 |
| 26 | Lead of the first part over the centroid p50 | 50 ms [50 ms, 50 ms] | 100 ms [100 ms, 100 ms] | -8.2 |
| 27 | Strafe switch hazard per tick at hold age 250 ms (LOS firefight) | 27.1% [25.1%, 28.5%] | 17.4% [16.0%, 19.0%] | +8.2 |
| 28 | Reaction: first trigger press after a clean sighting p90 | 700 ms [650 ms, 750 ms] | 400 ms [400 ms, 450 ms] | +8.0 |
| 29 | Duration of decisive engagements p50 | 1950 ms [1750 ms, 2000 ms] | 1250 ms [1150 ms, 1351 ms] | +7.8 |
| 30 | Reloads started with the opponent dead | 69.1% [65.5%, 74.1%] | 88.2% [85.5%, 90.4%] | -7.7 |
| 31 | Median aim error firing with LOS at (0, 128] u | 18.34° [17.68°, 18.88°] | 14.28° [13.53°, 15.19°] | +7.7 |
| 32 | Median aim error at +100 ms from the first visible part | 8.53° [7.77°, 9.38°] | 4.87° [4.54°, 5.18°] | +7.7 |
| 33 | Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) | 22.5% [21.5%, 23.6%] | 16.1% [14.9%, 17.5%] | +7.6 |
| 34 | Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part | 43.0% [39.7%, 47.0%] | 28.1% [26.8%, 29.4%] | +7.5 |
| 35 | Fire held at +700 ms from sight gain | 64.4% [62.3%, 65.9%] | 77.0% [74.3%, 79.7%] | -7.4 |
| 36 | Yaw speed (reloading) p99 | 694°/s [639°/s, 738°/s] | 945°/s [909°/s, 991°/s] | -7.4 |
| 37 | Time from both alive to the first body part on screen p75 | 2800 ms [2650 ms, 3050 ms] | 1900 ms [1800 ms, 2000 ms] | +7.3 |
| 38 | Retreating >40 u/s in LOS firefight at distance (160, 224] | 14.4% [13.3%, 15.5%] | 6.9% [5.5%, 8.7%] | +7.2 |
| 39 | Strafe switch hazard per tick at hold age 50 ms (LOS firefight) | 10.9% [8.2%, 13.3%] | 1.2% [0.9%, 1.6%] | +7.1 |
| 40 | Fire held at +0 ms from sight gain | 26.0% [23.2%, 29.4%] | 42.5% [39.1%, 46.0%] | -7.0 |
| 41 | Press hazard while hidden, LOS lost 300-500 ms ago | 11.2% [10.6%, 11.8%] | 7.0% [6.0%, 8.1%] | +6.9 |
| 42 | Yaw error to the hidden opponent, last seen 4-8 s | 25.93° [21.50°, 29.73°] | 10.29° [9.19°, 11.80°] | +6.8 |
| 43 | Press hazard while hidden, LOS lost 150-250 ms ago | 9.4% [8.5%, 10.7%] | 4.6% [3.9%, 5.5%] | +6.7 |
| 44 | Yaw speed (hidden, not firing) p50 | 16°/s [15°/s, 18°/s] | 8°/s [6°/s, 10°/s] | +6.7 |
| 45 | Mouse still between ticks (hidden, firing) | 7.3% [6.8%, 8.1%] | 10.9% [10.1%, 11.8%] | -6.6 |
| 46 | SMG shots that hit, a part on screen, centroid hidden | 13.3% [12.8%, 14.1%] | 21.7% [18.9%, 24.3%] | -6.6 |
| 47 | Press hazard while hidden, LOS lost over 1 s ago | 2.1% [1.9%, 2.4%] | 4.1% [3.5%, 4.6%] | -6.5 |
| 48 | Retreating >40 u/s in LOS firefight at distance (224, 288] | 10.4% [9.5%, 11.2%] | 5.3% [4.2%, 6.8%] | +6.3 |
| 49 | Median aim error at +100 ms from sight gain | 11.44° [9.94°, 13.01°] | 5.97° [5.54°, 6.43°] | +6.3 |
| 50 | Yaw speed (LOS, not firing) p99 | 884°/s [854°/s, 911°/s] | 583°/s [505°/s, 695°/s] | +6.2 |
| 51 | Median aim error at -200 ms from sight gain | 14.90° [12.72°, 16.86°] | 8.33° [7.70°, 8.92°] | +6.1 |
| 52 | View within 30 deg of the hidden opponent, last seen <=250 ms | 85.6% [83.6%, 87.4%] | 92.7% [91.9%, 93.7%] | -6.1 |
| 53 | Crosshair within 2 deg of the corner's edge, 500 ms before the first part | 15.3% [13.3%, 16.9%] | 23.7% [21.7%, 25.7%] | -6.0 |
| 54 | Fire held, LOS, aim error in half-widths (10, 20] | 13.5% [11.7%, 15.5%] | 4.6% [2.4%, 6.9%] | +6.0 |
| 55 | Yaw error to the corner, 500 ms before the first part | 10.62° [9.99°, 12.53°] | 5.81° [5.17°, 6.44°] | +6.0 |
| 56 | Fire already held on the tick before sight (prefire) | 22.4% [19.8%, 25.6%] | 35.4% [32.2%, 38.6%] | -5.9 |
| 57 | Retreating >40 u/s in LOS firefight at distance (96, 160] | 19.4% [17.4%, 21.7%] | 10.5% [8.5%, 12.6%] | +5.8 |
| 58 | Yaw speed (hidden, firing) p50 | 32°/s [29°/s, 34°/s] | 23°/s [21°/s, 25°/s] | +5.8 |
| 59 | Median aim error at +200 ms from sight gain | 9.01° [7.79°, 10.26°] | 4.92° [4.54°, 5.23°] | +5.7 |
| 60 | Strafe switch hazard per tick at hold age 100 ms (LOS firefight) | 20.8% [15.6%, 24.8%] | 5.8% [5.2%, 6.8%] | +5.5 |
| 61 | Angle to the corner at the first part | 8.64° [7.95°, 9.70°] | 5.74° [5.33°, 6.17°] | +5.5 |
| 62 | Crosshair on body firing with LOS at (192, 256] u | 33.7% [32.3%, 34.9%] | 42.0% [39.5%, 44.4%] | -5.3 |
| 63 | Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) | 16.77° [12.37°, 20.20°] | 5.78° [4.88°, 6.98°] | +5.3 |
| 64 | Net distance covered in 2 s from a tick with no body part on screen p50 | 116 u [109 u, 124 u] | 158 u [146 u, 173 u] | -5.1 |
| 65 | Angle to the corner, 500 ms before the first part | 12.50° [11.31°, 14.12°] | 8.41° [7.70°, 9.19°] | +5.1 |
| 66 | Fire held, LOS, aim error in half-widths (20, 1000] | 8.0% [6.1%, 10.7%] | 0.9% [0.4%, 3.1%] | +5.1 |
| 67 | SMG shots that hit, centroid visible | 21.9% [20.4%, 23.4%] | 27.2% [25.8%, 28.5%] | -5.0 |
| 68 | Lean held, all duel time | 74.0% [71.4%, 76.5%] | 59.7% [55.3%, 64.0%] | +5.0 |
| 69 | Median aim error at +300 ms from sight gain | 7.66° [6.51°, 8.76°] | 4.53° [4.12°, 4.88°] | +4.9 |
| 70 | Speed 2-1 s before the first part | 143 u/s [136 u/s, 147 u/s] | 167 u/s [159 u/s, 176 u/s] | -4.9 |
| 71 | Mouse still between ticks (reloading) | 21.0% [19.1%, 22.7%] | 26.3% [25.0%, 27.6%] | -4.8 |
| 72 | Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago | 21.5% [17.6%, 26.8%] | 7.4% [4.7%, 10.8%] | +4.8 |
| 73 | Median aim error at -100 ms from the first visible part | 11.22° [9.94°, 12.16°] | 8.00° [7.34°, 8.49°] | +4.7 |
| 74 | View within 30 deg of the hidden opponent, last seen 2-4 s | 51.6% [47.4%, 56.0%] | 64.3% [61.3%, 67.3%] | -4.7 |
| 75 | Crosshair on body firing with LOS at (0, 128] u | 39.2% [37.6%, 41.0%] | 48.0% [44.9%, 51.5%] | -4.6 |
| 76 | Crosshair within 5 deg of the corner, 500 ms before the first part | 20.6% [18.3%, 22.8%] | 29.2% [26.4%, 32.7%] | -4.5 |
| 77 | Yaw speed (reloading) p50 | 25°/s [23°/s, 28°/s] | 18°/s [16°/s, 20°/s] | +4.5 |
| 78 | Yaw error to the corner at +0 ms (sightings hidden >= 2 s) | 6.68° [5.80°, 8.41°] | 3.54° [3.01°, 4.10°] | +4.4 |
| 79 | Release hazard while hidden, LOS lost 500-950 ms ago | 13.1% [12.5%, 14.1%] | 10.1% [9.2%, 11.3%] | +4.4 |
| 80 | Crosshair on body firing with LOS at (256, 384] u | 32.8% [31.9%, 33.6%] | 39.0% [36.0%, 41.4%] | -4.4 |
| 81 | Life length (lives ending in death) p50 | 10.9 s [10.2 s, 12.4 s] | 7.1 s [5.8 s, 8.2 s] | +4.3 |
| 82 | Key chord neutral (reloading) | 16.2% [13.3%, 20.0%] | 8.3% [7.3%, 9.5%] | +4.3 |
| 83 | Shots per burst (consecutive 100 ms shots) p90 | 16 [14, 17] | 12 [11, 13] | +4.3 |
| 84 | Standing still (<5 u/s), all duel time | 10.2% [9.4%, 11.2%] | 15.7% [13.4%, 17.7%] | -4.1 |
| 85 | Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part | 0.14° [-0.60°, 1.14°] | -1.77° [-2.04°, -1.43°] | +4.1 |
| 86 | Yaw speed (LOS, not firing) p50 | 42°/s [37°/s, 47°/s] | 11°/s [0°/s, 26°/s] | +4.1 |
| 87 | Share of duel time in the busiest 10% of 32u cells (dm/main) | 35.6% [31.9%, 35.8%] | 45.2% [40.5%, 48.6%] | -4.1 |
| 88 | Shot accuracy (eligible SMG shots that hit) | 15.8% [14.7%, 17.1%] | 19.2% [18.1%, 20.3%] | -4.1 |
| 89 | Key chord forward (reloading) | 15.3% [11.6%, 20.5%] | 26.1% [23.8%, 28.6%] | -4.0 |
| 90 | Attack holds begun loaded that are taps (<=100 ms), with a body part visible | 9.9% [8.6%, 11.6%] | 15.4% [13.1%, 17.6%] | -4.0 |
| 91 | Crosshair on body firing with LOS at (128, 192] u | 35.9% [33.7%, 38.1%] | 42.5% [39.8%, 44.7%] | -4.0 |
| 92 | Speed 400-200 ms before the first part | 159 u/s [146 u/s, 171 u/s] | 188 u/s [182 u/s, 193 u/s] | -3.9 |
| 93 | Strafe end is a direct reverse (reloading) | 56.4% [50.0%, 61.7%] | 37.7% [32.5%, 42.7%] | +3.9 |
| 94 | Key chord pure strafe (reloading) | 30.1% [26.5%, 32.9%] | 22.3% [19.6%, 24.4%] | +3.8 |
| 95 | Standing still (<5 u/s) (hidden, not firing) | 16.3% [14.9%, 17.8%] | 22.5% [19.5%, 25.1%] | -3.8 |
| 96 | Approaching >40 u/s in LOS firefight at distance (96, 160] | 25.7% [22.4%, 29.3%] | 36.4% [32.6%, 41.2%] | -3.8 |
| 97 | Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) | 21.8% [18.2%, 26.0%] | 13.7% [12.0%, 15.6%] | +3.7 |
| 98 | Shots per burst whose first round left with a body part visible p90 | 17 [16, 19] | 13 [12, 14] | +3.7 |
| 99 | Standing still (<5 u/s) (hidden, firing) | 7.2% [6.5%, 8.1%] | 10.2% [9.1%, 11.7%] | -3.7 |
| 100 | Median aim error firing with LOS at (128, 192] u | 10.39° [9.86°, 11.07°] | 8.99° [8.66°, 9.45°] | +3.7 |
| 101 | Key chord fwd diag (hidden, not firing) | 23.4% [22.4%, 24.3%] | 29.9% [26.7%, 33.2%] | -3.7 |
| 102 | Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part | 41.7% [38.5%, 43.9%] | 48.2% [45.7%, 50.3%] | -3.6 |
| 103 | Fire held, LOS, aim error in half-widths (6, 10] | 35.1% [30.8%, 39.9%] | 22.4% [17.1%, 27.1%] | +3.6 |
| 104 | Fire held with no body part visible, 0-500ms after one was last on screen | 48.3% [47.4%, 49.6%] | 43.2% [40.9%, 45.8%] | +3.6 |
| 105 | Release hazard while hidden, LOS lost 150-250 ms ago | 11.1% [9.8%, 13.6%] | 16.5% [13.8%, 18.7%] | -3.6 |
| 106 | Attack holds that are taps (<=100 ms) | 38.3% [35.5%, 40.3%] | 44.3% [41.8%, 46.5%] | -3.6 |
| 107 | Yaw error to the hidden opponent, last seen 2-4 s | 27.81° [22.76°, 34.33°] | 16.93° [15.18°, 19.08°] | +3.5 |
| 108 | Reloads with the opponent alive that are forced (empty clip) | 92.2% [88.7%, 94.4%] | 77.7% [69.5%, 83.9%] | +3.5 |
| 109 | Key chord pure strafe (LOS, not firing) | 40.8% [38.2%, 43.2%] | 26.7% [20.8%, 34.2%] | +3.5 |
| 110 | View within 30 deg of the hidden opponent, last seen 250-500 ms | 89.4% [88.4%, 90.6%] | 92.2% [91.3%, 93.4%] | -3.5 |
| 111 | Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part | 11.95° [10.34°, 13.55°] | 15.18° [14.24°, 16.01°] | -3.4 |
| 112 | Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) | 37.7% [29.1%, 40.2%] | 49.4% [44.0%, 50.8%] | -3.4 |
| 113 | Median aim error firing with LOS at (192, 256] u | 7.61° [7.30°, 7.97°] | 6.68° [6.36°, 7.10°] | +3.4 |
| 114 | Key chord pure strafe (hidden, not firing) | 32.2% [30.1%, 34.1%] | 26.0% [23.5%, 29.4%] | +3.4 |
| 115 | Share of duel time: hidden, firing | 12.9% [11.7%, 14.2%] | 15.8% [14.9%, 17.0%] | -3.3 |
| 116 | Key chord fwd diag (reloading) | 28.8% [25.7%, 31.6%] | 36.8% [34.1%, 39.9%] | -3.3 |
| 117 | First hit after the first part p50 | 350 ms [350 ms, 400 ms] | 250 ms [200 ms, 250 ms] | +3.3 |
| 118 | Side-hold duration (LOS firefight) p50 | 200 ms [150 ms, 200 ms] | 300 ms [250 ms, 300 ms] | -3.3 |
| 119 | Yaw speed (LOS firefight) p50 | 46°/s [43°/s, 49°/s] | 39°/s [36°/s, 41°/s] | +3.2 |
| 120 | Share of duel time: LOS, not firing | 9.9% [9.1%, 11.0%] | 7.0% [5.5%, 8.4%] | +3.2 |
| 121 | Part sightings whose visible bout ends without a hit | 36.2% [31.2%, 41.6%] | 27.5% [25.7%, 29.7%] | +3.2 |
| 122 | Share of duel time in the busiest 10% of 32u cells (dm/downladder) | 40.7% [31.9%, 42.3%] | 61.4% [42.9%, 63.8%] | -3.1 |
| 123 | View turn toward the enemy over the last 500 ms before the first part | 10.09° [9.07°, 11.12°] | 13.01° [11.44°, 14.55°] | -3.1 |
| 124 | Fire already held on the tick before the first part (prefire) | 18.5% [17.3%, 19.6%] | 23.0% [20.3%, 25.5%] | -3.0 |
| 125 | Release hazard, LOS, aim error in half-widths (10, 1000] | 17.7% [15.2%, 19.9%] | 32.7% [22.9%, 41.8%] | -3.0 |
| 126 | Release hazard while hidden, LOS lost 1 s or more ago | 14.3% [13.7%, 14.9%] | 11.3% [9.8%, 13.5%] | +3.0 |
| 127 | Approaching >40 u/s in LOS firefight at distance (224, 288] | 24.4% [19.3%, 31.6%] | 38.2% [32.3%, 46.4%] | -3.0 |
| 128 | View turn over the last 500 ms before the first part | 12.25° [11.29°, 13.11°] | 14.53° [13.34°, 15.90°] | -2.9 |
| 129 | Key chord back any (LOS, not firing) | 7.3% [6.8%, 7.9%] | 4.8% [3.4%, 6.7%] | +2.8 |
| 130 | Walk presses per minute | 6.35/min [5.71/min, 6.90/min] | 7.54/min [6.90/min, 8.11/min] | -2.7 |
| 131 | Reaction: first trigger press after a clean sighting p75 | 400 ms [350 ms, 450 ms] | 300 ms [250 ms, 300 ms] | +2.6 |
| 132 | Key chord fwd diag (hidden, firing) | 21.8% [20.6%, 22.8%] | 28.6% [23.9%, 32.8%] | -2.6 |
| 133 | Standing still (<5 u/s) (LOS, not firing) | 7.1% [6.1%, 8.7%] | 29.6% [12.6%, 44.0%] | -2.6 |
| 134 | Fire held at +1000 ms from sight gain | 58.8% [56.5%, 60.6%] | 63.4% [60.9%, 66.4%] | -2.5 |
| 135 | Mouse still between ticks (LOS, not firing) | 18.7% [17.2%, 20.6%] | 36.7% [20.8%, 49.8%] | -2.3 |

## Per bot

### `bot:stopper:4` (stopper, seed 711847621, 26.4 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.07034 | 0.08636 | +3% |
| reverse_share | 0.1382 | 0.101 | -5% |
| side_hold_ms | 249.3 | 200 | -33% ⚠ |
| lean_fight | 0.9549 | 0.9702 | +3% |
| jumps_per_min | 0.3303 | 0.1897 | -3% |
| crouch_per_min | 5.571 | 5.918 | +3% |
| walk_hidden | 0 | 0.05049 | +48% ⚠ |
| burst_median | 4.564 | 5 | +11% |
| aim_height_firing | 0.4682 | 0.453 | -8% |
| hold_angle | 0.4674 | 0.2387 | -74% ⚠ |
| aim_error_fight_deg | 6.249 | 5.86 | -16% |
| reaction_ms | 153.7 | 225 | +71% ⚠ |
| mp40_share | 0.02524 | 0 | -3% |

Style fingerprint (2026-10-02, 26.4 min, styles.py features 30/30): z-distance to the human cloud centre 7.5 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.119 (humans 0.152–0.491); neutral 0.303 (humans 0.0872–0.296); lf_neutral 0.191 (humans 0.00436–0.112); lean_any 0.883 (humans 0.358–0.767); lf_lean 0.97 (humans 0.457–0.948); p_attack_given_los 0.544 (humans 0.547–0.861); lf_miss_units_med 36.8 (humans 22.2–36.1); lf_on_target 0.266 (humans 0.274–0.515); yaw_speed_p95 286 (humans 158–271); side_hold_p90_ms 600 (humans 700–950); reverse_share 0.101 (humans 0.138–0.839).

### `bot:stopper:5` (stopper, seed 1529931226, 26.4 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1013 | 0.1044 | +1% |
| reverse_share | 0.3253 | 0.2687 | -8% |
| side_hold_ms | 247.2 | 200 | -31% ⚠ |
| lean_fight | 0.8427 | 0.8543 | +2% |
| jumps_per_min | 5.122 | 5.691 | +11% |
| crouch_per_min | 11.7 | 12.44 | +7% |
| walk_hidden | 0.07078 | 0.07097 | +0% |
| burst_median | 4.132 | 5 | +22% |
| aim_height_firing | 0.4597 | 0.438 | -12% |
| hold_angle | 0.5274 | 0.2845 | -79% ⚠ |
| aim_error_fight_deg | 4.835 | 4.236 | -24% |
| reaction_ms | 109.9 | 150 | +40% ⚠ |
| mp40_share | 0.06948 | 0 | -7% |

Style fingerprint (2026-10-02, 26.4 min, styles.py features 30/30): z-distance to the human cloud centre 7.6 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 6.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.132 (humans 0.152–0.491); lf_neutral 0.153 (humans 0.00436–0.112); ducked 0.126 (humans 0.0109–0.0741); speed_med 129 (humans 136–200); side_hold_p90_ms 550 (humans 700–950).

### `bot:strafer:4` (strafer, seed 231333596, 23.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.06661 | +0% |
| reverse_share | 0.4467 | 0.4206 | -4% |
| side_hold_ms | 326.3 | 250 | -51% ⚠ |
| lean_fight | 0.4941 | 0.5486 | +11% |
| jumps_per_min | 2.379 | 2.232 | -3% |
| crouch_per_min | 9.476 | 9.94 | +4% |
| walk_hidden | 0 | 0.05849 | +55% ⚠ |
| burst_median | 8 | 7 | -25% ⚠ |
| aim_height_firing | 0.4857 | 0.47 | -8% |
| hold_angle | 0.2514 | 0.3771 | +41% ⚠ |
| aim_error_fight_deg | 4.667 | 3.737 | -38% ⚠ |
| reaction_ms | 140.8 | 200 | +59% ⚠ |
| mp40_share | 0.9062 | 1 | +9% |

Style fingerprint (2026-10-02, 23.7 min, styles.py features 30/30): z-distance to the human cloud centre 6.7 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 4.2 vs the within-person threshold 5.6 (as close as two captures of one human).

Features outside the human min–max: pure_strafe 0.572 (humans 0.191–0.489); fwd_diag 0.118 (humans 0.152–0.491); lf_pure_strafe 0.805 (humans 0.362–0.752); ducked 0.0818 (humans 0.0109–0.0741); lf_err_med 3.74 (humans 4.08–6.63); yaw_speed_p95 275 (humans 158–271); side_hold_p90_ms 600 (humans 700–950); lf_approach 0.102 (humans 0.131–0.64).

### `bot:presser:5` (presser, seed 1152489726, 23.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4832 | 0.4591 | -5% |
| reverse_share | 0.8051 | 0.7954 | -1% |
| side_hold_ms | 295.2 | 200 | -63% ⚠ |
| lean_fight | 0.9204 | 0.9618 | +8% |
| jumps_per_min | 0.4317 | 0.3791 | -1% |
| crouch_per_min | 3.157 | 3.201 | +0% |
| walk_hidden | 0.02816 | 0.06389 | +34% ⚠ |
| burst_median | 6.498 | 7 | +13% |
| aim_height_firing | 0.417 | 0.392 | -13% |
| hold_angle | 0.2275 | 0.2333 | +2% |
| aim_error_fight_deg | 4.893 | 4.395 | -20% |
| reaction_ms | 152.5 | 300 | +148% ⚠ |
| mp40_share | 0.1123 | 1 | +89% ⚠ |

Style fingerprint (2026-10-02, 23.7 min, styles.py features 30/30): z-distance to the human cloud centre 6.9 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 7.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.854 (humans 0.358–0.767); lf_lean 0.962 (humans 0.457–0.948); still 0.0773 (humans 0.0832–0.227); yaw_speed_p95 338 (humans 158–271); yaw_speed_p99 755 (humans 427–754); side_hold_p90_ms 550 (humans 700–950).

### `bot:stopper:4.2` (stopper, seed 2458011590, 25.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1182 | 0.09886 | -4% |
| reverse_share | 0.4266 | 0.3909 | -5% |
| side_hold_ms | 200 | 250 | +33% ⚠ |
| lean_fight | 0.8817 | 0.8998 | +4% |
| jumps_per_min | 2.965 | 3.124 | +3% |
| crouch_per_min | 8.679 | 7.889 | -7% |
| walk_hidden | 0.1056 | 0.1203 | +14% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4719 | 0.452 | -11% |
| hold_angle | 0.3772 | 0.1921 | -60% ⚠ |
| aim_error_fight_deg | 4.42 | 7.258 | +116% ⚠ |
| reaction_ms | 139.9 | 250 | +110% ⚠ |
| mp40_share | 0.02794 | 0 | -3% |

Style fingerprint (2026-10-02, 25.6 min, styles.py features 30/30): z-distance to the human cloud centre 8.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.7 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.15 (humans 0.152–0.491); lf_neutral 0.13 (humans 0.00436–0.112); lean_any 0.775 (humans 0.358–0.767); p_attack_given_los 0.53 (humans 0.547–0.861); speed_med 134 (humans 136–200); lf_err_med 7.26 (humans 4.08–6.63); yaw_speed_med 31.1 (humans 12.6–30); yaw_speed_p95 339 (humans 158–271); side_hold_p90_ms 650 (humans 700–950); lf_retreat 0.142 (humans 0.0243–0.141).

### `bot:strafer:5` (strafer, seed 4003987225, 25.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.06155 | -1% |
| reverse_share | 0.4657 | 0.4191 | -7% |
| side_hold_ms | 270.9 | 250 | -14% |
| lean_fight | 0.5402 | 0.5451 | +1% |
| jumps_per_min | 4.852 | 5.545 | +13% |
| crouch_per_min | 3.183 | 2.656 | -5% |
| walk_hidden | 0.03847 | 0.05604 | +17% |
| burst_median | 5.686 | 6 | +8% |
| aim_height_firing | 0.4588 | 0.446 | -7% |
| hold_angle | 0.311 | 0.1973 | -37% ⚠ |
| aim_error_fight_deg | 5.446 | 7.997 | +104% ⚠ |
| reaction_ms | 161.3 | 250 | +89% ⚠ |
| mp40_share | 0 | 0 | +0% |

Style fingerprint (2026-10-02, 25.6 min, styles.py features 30/30): z-distance to the human cloud centre 8.5 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.2 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.114 (humans 0.152–0.491); lf_fwd_diag 0.0616 (humans 0.0658–0.568); lf_neutral 0.131 (humans 0.00436–0.112); lean_any 0.357 (humans 0.358–0.767); speed_med 136 (humans 136–200); lf_err_med 8 (humans 4.08–6.63); yaw_speed_med 33.1 (humans 12.6–30); yaw_speed_p95 336 (humans 158–271); side_hold_p90_ms 650 (humans 700–950); lf_retreat 0.162 (humans 0.0243–0.141).

### `bot:strafer:4.2` (strafer, seed 3498264623, 25.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1522 | 0.1259 | -5% |
| reverse_share | 0.4724 | 0.4406 | -5% |
| side_hold_ms | 293.3 | 250 | -29% ⚠ |
| lean_fight | 0.6201 | 0.6274 | +1% |
| jumps_per_min | 3.829 | 4.081 | +5% |
| crouch_per_min | 9.64 | 9.923 | +3% |
| walk_hidden | 0 | 0.04495 | +43% ⚠ |
| burst_median | 4.228 | 5 | +19% |
| aim_height_firing | 0.4919 | 0.486 | -3% |
| hold_angle | 0.3757 | 0.2055 | -55% ⚠ |
| aim_error_fight_deg | 5.803 | 8.879 | +125% ⚠ |
| reaction_ms | 133.2 | 200 | +67% ⚠ |
| mp40_share | 0.5841 | 1 | +42% ⚠ |

Style fingerprint (2026-10-02, 25.0 min, styles.py features 30/30): z-distance to the human cloud centre 9.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.0867 (humans 0.0109–0.0741); lf_err_med 8.88 (humans 4.08–6.63); yaw_speed_med 35.8 (humans 12.6–30); yaw_speed_p95 355 (humans 158–271); side_hold_p90_ms 650 (humans 700–950); lf_retreat 0.153 (humans 0.0243–0.141).

### `bot:stopper:5.2` (stopper, seed 2513918026, 25.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1336 | 0.0965 | -7% |
| reverse_share | 0.6777 | 0.6394 | -5% |
| side_hold_ms | 226 | 250 | +16% |
| lean_fight | 0.9588 | 0.9709 | +2% |
| jumps_per_min | 2.944 | 3.201 | +5% |
| crouch_per_min | 3.426 | 2.881 | -5% |
| walk_hidden | 0.08429 | 0.08679 | +2% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4602 | 0.448 | -6% |
| hold_angle | 0.3824 | 0.152 | -75% ⚠ |
| aim_error_fight_deg | 4.291 | 7.95 | +149% ⚠ |
| reaction_ms | 113 | 200 | +87% ⚠ |
| mp40_share | 0.07877 | 0 | -8% |

Style fingerprint (2026-10-02, 25.0 min, styles.py features 30/30): z-distance to the human cloud centre 8.0 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.2 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.497 (humans 0.191–0.489); lf_pure_strafe 0.767 (humans 0.362–0.752); lean_any 0.888 (humans 0.358–0.767); lf_lean 0.971 (humans 0.457–0.948); lf_err_med 7.95 (humans 4.08–6.63); yaw_speed_med 32 (humans 12.6–30); yaw_speed_p95 341 (humans 158–271); side_hold_p90_ms 600 (humans 700–950); lf_retreat 0.147 (humans 0.0243–0.141).

### `bot:presser:4` (presser, seed 52844662, 25.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5317 | 0.298 | -46% ⚠ |
| reverse_share | 0.8152 | 0.7709 | -6% |
| side_hold_ms | 336.6 | 200 | -91% ⚠ |
| lean_fight | 0.935 | 0.9744 | +8% |
| jumps_per_min | 0.336 | 0.4007 | +1% |
| crouch_per_min | 1.298 | 1.042 | -2% |
| walk_hidden | 0 | 0.05192 | +49% ⚠ |
| burst_median | 7.121 | 7.5 | +9% |
| aim_height_firing | 0.3853 | 0.336 | -26% ⚠ |
| hold_angle | 0.2402 | 0.2785 | +12% |
| aim_error_fight_deg | 4.424 | 4.783 | +15% |
| reaction_ms | 173.4 | 250 | +77% ⚠ |
| mp40_share | 0.06486 | 0 | -6% |

Style fingerprint (2026-10-02, 25.0 min, styles.py features 30/30): z-distance to the human cloud centre 7.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.884 (humans 0.358–0.767); lf_lean 0.974 (humans 0.457–0.948); still 0.0639 (humans 0.0832–0.227); lf_height_frac 0.336 (humans 0.352–0.54); yaw_speed_p95 349 (humans 158–271); side_hold_p90_ms 500 (humans 700–950).

### `bot:strafer:5.2` (strafer, seed 3859071546, 25.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1614 | 0.1047 | -11% |
| reverse_share | 0.5449 | 0.4881 | -8% |
| side_hold_ms | 350 | 200 | -100% ⚠ |
| lean_fight | 0.5666 | 0.6789 | +22% |
| jumps_per_min | 0.6862 | 0.4407 | -5% |
| crouch_per_min | 3.8 | 4.047 | +2% |
| walk_hidden | 0.04377 | 0.05493 | +11% |
| burst_median | 7.775 | 7 | -19% |
| aim_height_firing | 0.4661 | 0.435 | -17% |
| hold_angle | 0.2527 | 0.289 | +12% |
| aim_error_fight_deg | 4.952 | 4.194 | -31% ⚠ |
| reaction_ms | 115 | 150 | +35% ⚠ |
| mp40_share | 0.03683 | 0 | -4% |

Style fingerprint (2026-10-02, 25.0 min, styles.py features 30/30): z-distance to the human cloud centre 6.6 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 5.4 vs the within-person threshold 5.6 (as close as two captures of one human).

Features outside the human min–max: pure_strafe 0.522 (humans 0.191–0.489); yaw_speed_p95 287 (humans 158–271); side_hold_p90_ms 500 (humans 700–950).

### `bot:presser:4.2` (presser, seed 588158740, 25.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4812 | 0.2396 | -47% ⚠ |
| reverse_share | 0.8389 | 0.7983 | -6% |
| side_hold_ms | 302.9 | 200 | -69% ⚠ |
| lean_fight | 0.9588 | 0.9833 | +5% |
| jumps_per_min | 1.077 | 0.8389 | -4% |
| crouch_per_min | 1.47 | 1.039 | -4% |
| walk_hidden | 0.004612 | 0.06247 | +55% ⚠ |
| burst_median | 7.045 | 8 | +24% |
| aim_height_firing | 0.352 | 0.284 | -36% ⚠ |
| hold_angle | 0.2402 | 0.2903 | +16% |
| aim_error_fight_deg | 4.382 | 5.547 | +47% ⚠ |
| reaction_ms | 183.1 | 300 | +117% ⚠ |
| mp40_share | 0.9584 | 1 | +4% |

Style fingerprint (2026-10-02, 25.0 min, styles.py features 30/30): z-distance to the human cloud centre 8.0 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.6 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.904 (humans 0.358–0.767); lf_lean 0.983 (humans 0.457–0.948); p_attack_given_los 0.511 (humans 0.547–0.861); still 0.0757 (humans 0.0832–0.227); lf_height_frac 0.284 (humans 0.352–0.54); yaw_speed_p95 325 (humans 158–271); side_hold_p90_ms 500 (humans 700–950).

### `bot:presser:5.2` (presser, seed 4131749178, 25.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5433 | 0.2317 | -61% ⚠ |
| reverse_share | 0.823 | 0.7544 | -10% |
| side_hold_ms | 298.6 | 150 | -99% ⚠ |
| lean_fight | 0.9579 | 0.9821 | +5% |
| jumps_per_min | 0.5292 | 0.4794 | -1% |
| crouch_per_min | 2.785 | 2.876 | +1% |
| walk_hidden | 0.00312 | 0.04489 | +40% ⚠ |
| burst_median | 7.004 | 8 | +25% |
| aim_height_firing | 0.4126 | 0.373 | -21% |
| hold_angle | 0.2204 | 0.3187 | +32% ⚠ |
| aim_error_fight_deg | 4.873 | 5.112 | +10% |
| reaction_ms | 134.4 | 200 | +66% ⚠ |
| mp40_share | 0.08844 | 0 | -9% |

Style fingerprint (2026-10-02, 25.0 min, styles.py features 30/30): z-distance to the human cloud centre 8.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.911 (humans 0.358–0.767); lf_lean 0.982 (humans 0.457–0.948); yaw_speed_med 31.1 (humans 12.6–30); yaw_speed_p95 347 (humans 158–271); yaw_speed_p99 769 (humans 427–754); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 500 (humans 700–950).

### `bot:stopper:4.3` (stopper, seed 97502759, 27.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.06048 | -1% |
| reverse_share | 0.6408 | 0.6008 | -6% |
| side_hold_ms | 200 | 200 | +0% |
| lean_fight | 0.8757 | 0.9182 | +8% |
| jumps_per_min | 1.606 | 1.934 | +6% |
| crouch_per_min | 2.514 | 2.578 | +1% |
| walk_hidden | 0.09232 | 0.09423 | +2% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4804 | 0.486 | +3% |
| hold_angle | 0.418 | 0.1933 | -73% ⚠ |
| aim_error_fight_deg | 5.509 | 9.703 | +171% ⚠ |
| reaction_ms | 127.6 | 200 | +72% ⚠ |
| mp40_share | 0.1179 | 0 | -12% |

Style fingerprint (2026-10-02, 27.9 min, styles.py features 30/30): z-distance to the human cloud centre 9.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.2 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.128 (humans 0.152–0.491); lf_pure_strafe 0.791 (humans 0.362–0.752); lf_fwd_diag 0.0605 (humans 0.0658–0.568); attack 0.258 (humans 0.276–0.478); speed_med 123 (humans 136–200); lf_err_med 9.7 (humans 4.08–6.63); yaw_speed_p95 344 (humans 158–271); side_hold_p90_ms 550 (humans 700–950); lf_approach 0.128 (humans 0.131–0.64); lf_retreat 0.15 (humans 0.0243–0.141).

### `bot:strafer:5.3` (strafer, seed 2592015535, 27.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.119 | 0.101 | -4% |
| reverse_share | 0.4951 | 0.4372 | -8% |
| side_hold_ms | 295 | 250 | -30% ⚠ |
| lean_fight | 0.4565 | 0.4658 | +2% |
| jumps_per_min | 5.657 | 5.228 | -8% |
| crouch_per_min | 11.59 | 10.85 | -7% |
| walk_hidden | 0 | 0.02707 | +26% ⚠ |
| burst_median | 4.178 | 5 | +21% |
| aim_height_firing | 0.54 | 0.54 | +0% |
| hold_angle | 0.3001 | 0.2571 | -14% |
| aim_error_fight_deg | 6.024 | 10.09 | +165% ⚠ |
| reaction_ms | 165.9 | 250 | +84% ⚠ |
| mp40_share | 0.4858 | 1 | +51% ⚠ |

Style fingerprint (2026-10-02, 27.9 min, styles.py features 30/30): z-distance to the human cloud centre 11.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.6 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lf_neutral 0.118 (humans 0.00436–0.112); lean_any 0.256 (humans 0.358–0.767); ducked 0.129 (humans 0.0109–0.0741); attack 0.227 (humans 0.276–0.478); p_attack_given_los 0.511 (humans 0.547–0.861); speed_med 127 (humans 136–200); lf_err_med 10.1 (humans 4.08–6.63); yaw_speed_p95 376 (humans 158–271); side_hold_p90_ms 600 (humans 700–950); lf_retreat 0.163 (humans 0.0243–0.141).

### `bot:strafer:4.3` (strafer, seed 1086809828, 28.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1632 | 0.1088 | -11% |
| reverse_share | 0.4593 | 0.3903 | -10% |
| side_hold_ms | 314.9 | 250 | -43% ⚠ |
| lean_fight | 0.5599 | 0.5834 | +5% |
| jumps_per_min | 4.813 | 5.054 | +4% |
| crouch_per_min | 9.497 | 8.694 | -7% |
| walk_hidden | 0 | 0.02855 | +27% ⚠ |
| burst_median | 5.59 | 5 | -15% |
| aim_height_firing | 0.4741 | 0.482 | +4% |
| hold_angle | 0.3025 | 0.2771 | -8% |
| aim_error_fight_deg | 6.527 | 12.27 | +234% ⚠ |
| reaction_ms | 158.4 | 200 | +42% ⚠ |
| mp40_share | 0.9804 | 1 | +2% |

Style fingerprint (2026-10-02, 28.3 min, styles.py features 30/30): z-distance to the human cloud centre 12.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 11.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.354 (humans 0.358–0.767); ducked 0.108 (humans 0.0109–0.0741); attack 0.244 (humans 0.276–0.478); lf_err_med 12.3 (humans 4.08–6.63); lf_miss_units_med 36.4 (humans 22.2–36.1); yaw_speed_med 30.8 (humans 12.6–30); yaw_speed_p95 406 (humans 158–271); yaw_speed_p99 796 (humans 427–754); side_hold_p90_ms 600 (humans 700–950); lf_retreat 0.229 (humans 0.0243–0.141).

### `bot:strafer:5.4` (strafer, seed 821368291, 28.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.166 | 0.08692 | -15% |
| reverse_share | 0.4801 | 0.4263 | -8% |
| side_hold_ms | 349.1 | 250 | -66% ⚠ |
| lean_fight | 0.6654 | 0.7113 | +9% |
| jumps_per_min | 5.052 | 4.771 | -5% |
| crouch_per_min | 10.37 | 9.966 | -4% |
| walk_hidden | 0.0009382 | 0.01966 | +18% |
| burst_median | 5.755 | 7 | +31% ⚠ |
| aim_height_firing | 0.4434 | 0.418 | -14% |
| hold_angle | 0.3191 | 0.2848 | -11% |
| aim_error_fight_deg | 4.261 | 8.98 | +192% ⚠ |
| reaction_ms | 175 | 250 | +75% ⚠ |
| mp40_share | 0.1202 | 0 | -12% |

Style fingerprint (2026-10-02, 28.3 min, styles.py features 30/30): z-distance to the human cloud centre 9.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.495 (humans 0.191–0.489); lf_pure_strafe 0.776 (humans 0.362–0.752); ducked 0.107 (humans 0.0109–0.0741); walk 0.0118 (humans 0.0126–0.0881); attack 0.245 (humans 0.276–0.478); still 0.078 (humans 0.0832–0.227); lf_err_med 8.98 (humans 4.08–6.63); yaw_speed_med 30.1 (humans 12.6–30); yaw_speed_p95 338 (humans 158–271); side_hold_p90_ms 600 (humans 700–950); lf_retreat 0.183 (humans 0.0243–0.141).

## The owner against the bots (`pstN@vsbot`)

Kills of bots 0, deaths to bots 0, suicides 0. 

0 statistics of the owner against bots fall outside the human CI (owner column below, marked •).

## All statistics

Pooled bots are reweighted to the people's family mix of duel time; humans and the owner are pooled over their duel time. ▲ marks a tell. n = units (ticks, runs, shots, sightings, ...) over the blocks.

### Movement

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time: hidden, not firing | 45.9% [43.1%, 48.6%] | 49.1% [47.4%, 51.2%] | - | 496590 |
| Share of duel time: hidden, firing ▲ | 12.9% [11.7%, 14.2%] | 15.8% [14.9%, 17.0%] | - | 496590 |
| Share of duel time: LOS, not firing ▲ | 9.9% [9.1%, 11.0%] | 7.0% [5.5%, 8.4%] | - | 496590 |
| Share of duel time: LOS firefight | 20.3% [18.9%, 21.6%] | 17.6% [16.5%, 18.9%] | - | 496590 |
| Share of duel time: reloading | 11.0% [9.6%, 12.6%] | 10.5% [9.6%, 11.5%] | - | 496590 |
| Key chord pure strafe (hidden, not firing) ▲ | 32.2% [30.1%, 34.1%] | 26.0% [23.5%, 29.4%] | - | 242190 |
| Key chord fwd diag (hidden, not firing) ▲ | 23.4% [22.4%, 24.3%] | 29.9% [26.7%, 33.2%] | - | 242190 |
| Key chord forward (hidden, not firing) | 12.9% [11.9%, 14.2%] | 11.9% [10.9%, 12.8%] | - | 242190 |
| Key chord neutral (hidden, not firing) | 25.3% [23.2%, 27.9%] | 26.1% [23.1%, 28.6%] | - | 242190 |
| Key chord back any (hidden, not firing) | 6.2% [5.8%, 6.6%] | 6.1% [5.3%, 7.1%] | - | 242190 |
| Key chord pure strafe (hidden, firing) | 49.8% [46.5%, 52.4%] | 44.9% [41.6%, 48.0%] | - | 59953 |
| Key chord fwd diag (hidden, firing) ▲ | 21.8% [20.6%, 22.8%] | 28.6% [23.9%, 32.8%] | - | 59953 |
| Key chord forward (hidden, firing) | 8.5% [7.1%, 10.8%] | 6.6% [5.7%, 7.4%] | - | 59953 |
| Key chord neutral (hidden, firing) | 14.7% [12.7%, 17.4%] | 13.2% [11.2%, 15.7%] | - | 59953 |
| Key chord back any (hidden, firing) | 5.3% [3.9%, 6.1%] | 6.7% [5.9%, 7.9%] | - | 59953 |
| Key chord pure strafe (LOS, not firing) ▲ | 40.8% [38.2%, 43.2%] | 26.7% [20.8%, 34.2%] | - | 47257 |
| Key chord fwd diag (LOS, not firing) | 26.2% [23.8%, 27.4%] | 29.4% [23.0%, 38.1%] | - | 47257 |
| Key chord forward (LOS, not firing) | 8.1% [6.8%, 9.5%] | 7.5% [5.9%, 9.5%] | - | 47257 |
| Key chord neutral (LOS, not firing) | 17.6% [15.4%, 21.4%] | 31.5% [15.3%, 45.4%] | - | 47257 |
| Key chord back any (LOS, not firing) ▲ | 7.3% [6.8%, 7.9%] | 4.8% [3.4%, 6.7%] | - | 47257 |
| Key chord pure strafe (LOS firefight) | 65.2% [59.9%, 69.5%] | 58.0% [53.1%, 62.2%] | - | 95397 |
| Key chord fwd diag (LOS firefight) | 20.9% [16.7%, 27.3%] | 30.5% [25.6%, 36.7%] | - | 95397 |
| Key chord forward (LOS firefight) | 1.3% [1.0%, 1.6%] | 1.2% [1.0%, 1.4%] | - | 95397 |
| Key chord neutral (LOS firefight) | 7.2% [5.7%, 8.9%] | 5.1% [3.9%, 6.3%] | - | 95397 |
| Key chord back any (LOS firefight) | 5.4% [4.9%, 5.8%] | 5.2% [4.5%, 6.1%] | - | 95397 |
| Key chord pure strafe (reloading) ▲ | 30.1% [26.5%, 32.9%] | 22.3% [19.6%, 24.4%] | - | 51792 |
| Key chord fwd diag (reloading) ▲ | 28.8% [25.7%, 31.6%] | 36.8% [34.1%, 39.9%] | - | 51792 |
| Key chord forward (reloading) ▲ | 15.3% [11.6%, 20.5%] | 26.1% [23.8%, 28.6%] | - | 51792 |
| Key chord neutral (reloading) ▲ | 16.2% [13.3%, 20.0%] | 8.3% [7.3%, 9.5%] | - | 51792 |
| Key chord back any (reloading) | 9.6% [7.2%, 11.0%] | 6.5% [5.3%, 7.6%] | - | 51792 |
| Strafe end is a direct reverse (hidden, not firing) | 53.0% [48.4%, 56.3%] | 46.1% [41.1%, 51.2%] | - | 20974 |
| Strafe end is a direct reverse (hidden, firing) | 70.3% [63.7%, 75.1%] | 67.2% [59.7%, 74.3%] | - | 7690 |
| Strafe end is a direct reverse (LOS, not firing) | 61.8% [54.3%, 66.6%] | 52.8% [45.1%, 60.4%] | - | 5485 |
| Strafe end is a direct reverse (LOS firefight) | 73.4% [66.5%, 79.0%] | 73.3% [65.9%, 80.6%] | - | 17154 |
| Strafe end is a direct reverse (reloading) ▲ | 56.4% [50.0%, 61.7%] | 37.7% [32.5%, 42.7%] | - | 5336 |
| Strafe switch hazard per tick at hold age 50 ms (LOS firefight) ▲ | 10.9% [8.2%, 13.3%] | 1.2% [0.9%, 1.6%] | - | 18021 |
| Strafe switch hazard per tick at hold age 100 ms (LOS firefight) ▲ | 20.8% [15.6%, 24.8%] | 5.8% [5.2%, 6.8%] | - | 15455 |
| Strafe switch hazard per tick at hold age 150 ms (LOS firefight) ▲ | 24.1% [21.5%, 25.9%] | 12.4% [11.1%, 13.9%] | - | 12161 |
| Strafe switch hazard per tick at hold age 200 ms (LOS firefight) ▲ | 24.9% [24.3%, 25.7%] | 17.0% [15.4%, 18.7%] | - | 9168 |
| Strafe switch hazard per tick at hold age 250 ms (LOS firefight) ▲ | 27.1% [25.1%, 28.5%] | 17.4% [16.0%, 19.0%] | - | 6912 |
| Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) ▲ | 26.9% [25.9%, 27.8%] | 15.6% [14.7%, 16.9%] | - | 13703 |
| Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) ▲ | 22.5% [21.5%, 23.6%] | 16.1% [14.9%, 17.5%] | - | 7591 |
| Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) ▲ | 21.8% [18.2%, 26.0%] | 13.7% [12.0%, 15.6%] | - | 779 |
| Side-hold duration (LOS firefight) p25 ▲ | 100 ms [100 ms, 100 ms] | 200 ms [200 ms, 200 ms] | - | 16406 |
| Side-hold duration (LOS firefight) p50 ▲ | 200 ms [150 ms, 200 ms] | 300 ms [250 ms, 300 ms] | - | 16406 |
| Side-hold duration (LOS firefight) p75 ▲ | 300 ms [250 ms, 300 ms] | 450 ms [449 ms, 450 ms] | - | 16406 |
| Side-hold duration (LOS firefight) p90 ▲ | 450 ms [400 ms, 450 ms] | 700 ms [650 ms, 700 ms] | - | 16406 |
| Side-hold duration (all contexts) p50 ▲ | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | - | 55428 |
| Side-hold duration (all contexts) p90 ▲ | 550 ms [550 ms, 550 ms] | 800 ms [800 ms, 850 ms] | - | 55428 |
| Neutral gap between strafes (LOS firefight) p50 | 50 ms [50 ms, 50 ms] | 50 ms [50 ms, 50 ms] | - | 6209 |
| Neutral gap between strafes (LOS firefight) p90 | 100 ms [50 ms, 100 ms] | 100 ms [100 ms, 150 ms] | - | 6209 |
| Approaching >40 u/s in LOS firefight at distance (0, 96] | 30.5% [27.7%, 33.8%] | 37.6% [33.7%, 42.3%] | - | 6018 |
| Approaching >40 u/s in LOS firefight at distance (96, 160] ▲ | 25.7% [22.4%, 29.3%] | 36.4% [32.6%, 41.2%] | - | 12092 |
| Approaching >40 u/s in LOS firefight at distance (160, 224] | 24.2% [19.6%, 30.4%] | 35.4% [30.1%, 42.0%] | - | 16327 |
| Approaching >40 u/s in LOS firefight at distance (224, 288] ▲ | 24.4% [19.3%, 31.6%] | 38.2% [32.3%, 46.4%] | - | 15620 |
| Approaching >40 u/s in LOS firefight at distance (288, 384] | 26.1% [21.6%, 33.7%] | 39.4% [32.7%, 47.2%] | - | 19809 |
| Approaching >40 u/s in LOS firefight at distance (384, 512] | 27.5% [21.8%, 35.2%] | 41.8% [33.8%, 49.1%] | - | 15447 |
| Approaching >40 u/s in LOS firefight at distance (512, 768] | 27.3% [17.3%, 37.1%] | 33.1% [25.5%, 41.5%] | - | 9632 |
| Approaching >40 u/s in LOS firefight at distance (768, 1200] | 21.9% [0.8%, 41.0%] | 28.2% [17.6%, 42.7%] | - | 452 |
| Retreating >40 u/s in LOS firefight at distance (0, 96] | 26.6% [23.1%, 29.8%] | 22.2% [18.9%, 25.6%] | - | 6018 |
| Retreating >40 u/s in LOS firefight at distance (96, 160] ▲ | 19.4% [17.4%, 21.7%] | 10.5% [8.5%, 12.6%] | - | 12092 |
| Retreating >40 u/s in LOS firefight at distance (160, 224] ▲ | 14.4% [13.3%, 15.5%] | 6.9% [5.5%, 8.7%] | - | 16327 |
| Retreating >40 u/s in LOS firefight at distance (224, 288] ▲ | 10.4% [9.5%, 11.2%] | 5.3% [4.2%, 6.8%] | - | 15620 |
| Retreating >40 u/s in LOS firefight at distance (288, 384] | 7.9% [6.8%, 8.8%] | 5.9% [4.6%, 7.3%] | - | 19809 |
| Retreating >40 u/s in LOS firefight at distance (384, 512] | 6.3% [5.6%, 7.0%] | 4.8% [3.8%, 6.0%] | - | 15447 |
| Retreating >40 u/s in LOS firefight at distance (512, 768] | 7.0% [5.3%, 9.9%] | 4.0% [3.0%, 5.4%] | - | 9632 |
| Retreating >40 u/s in LOS firefight at distance (768, 1200] | 7.9% [0.0%, 14.2%] | 8.3% [1.5%, 15.5%] | - | 452 |
| Standing still (<5 u/s), all duel time ▲ | 10.2% [9.4%, 11.2%] | 15.7% [13.4%, 17.7%] | - | 496590 |
| Standing still (<5 u/s) (hidden, not firing) ▲ | 16.3% [14.9%, 17.8%] | 22.5% [19.5%, 25.1%] | - | 242190 |
| Standing still (<5 u/s) (hidden, firing) ▲ | 7.2% [6.5%, 8.1%] | 10.2% [9.1%, 11.7%] | - | 59953 |
| Standing still (<5 u/s) (LOS, not firing) ▲ | 7.1% [6.1%, 8.7%] | 29.6% [12.6%, 44.0%] | - | 47257 |
| Standing still (<5 u/s) (LOS firefight) | 2.0% [1.6%, 2.5%] | 2.3% [1.8%, 2.7%] | - | 95397 |
| Standing still (<5 u/s) (reloading) | 6.2% [5.0%, 7.8%] | 5.2% [4.3%, 6.2%] | - | 51792 |
| Lean held, all duel time ▲ | 74.0% [71.4%, 76.5%] | 59.7% [55.3%, 64.0%] | - | 496590 |
| Lean held in LOS firefights | 87.9% [85.7%, 89.6%] | 81.3% [76.6%, 85.8%] | - | 95397 |
| Jump presses per minute | 1.97/min [1.42/min, 2.51/min] | 1.87/min [1.41/min, 2.44/min] | - | 496590 |
| Crouch presses per minute | 4.63/min [3.47/min, 5.84/min] | 4.33/min [3.22/min, 5.51/min] | - | 496590 |
| Walk presses per minute ▲ | 6.35/min [5.71/min, 6.90/min] | 7.54/min [6.90/min, 8.11/min] | - | 496590 |
| Walking while hidden and not firing | 6.0% [4.9%, 6.8%] | 5.9% [5.0%, 7.0%] | - | 242190 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Press hazard with LOS and aim within 3 half-widths, LOS gained 0 ms ago | 25.6% [21.8%, 30.0%] | 28.2% [25.8%, 30.8%] | - | 1823 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 50-100 ms ago | 30.1% [23.1%, 35.9%] | 33.0% [30.8%, 35.6%] | - | 2749 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 150-250 ms ago | 36.5% [28.1%, 45.8%] | 35.2% [32.9%, 38.1%] | - | 2495 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago | 32.1% [21.5%, 43.7%] | 28.0% [25.2%, 32.0%] | - | 1834 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 550-1000 ms ago | 24.7% [18.4%, 32.1%] | 21.0% [16.7%, 24.6%] | - | 1992 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago ▲ | 21.5% [17.6%, 26.8%] | 7.4% [4.7%, 10.8%] | - | 2601 |
| Press hazard while hidden, LOS lost 0 ms ago | 12.0% [10.9%, 13.0%] | 12.3% [10.8%, 14.5%] | - | 2700 |
| Press hazard while hidden, LOS lost 50-100 ms ago | 10.6% [9.7%, 12.2%] | 9.5% [8.5%, 10.9%] | - | 3733 |
| Press hazard while hidden, LOS lost 150-250 ms ago ▲ | 9.4% [8.5%, 10.7%] | 4.6% [3.9%, 5.5%] | - | 5384 |
| Press hazard while hidden, LOS lost 300-500 ms ago ▲ | 11.2% [10.6%, 11.8%] | 7.0% [6.0%, 8.1%] | - | 17155 |
| Press hazard while hidden, LOS lost 550-1000 ms ago | 3.6% [3.1%, 4.1%] | 3.5% [3.2%, 3.9%] | - | 28980 |
| Press hazard while hidden, LOS lost over 1 s ago ▲ | 2.1% [1.9%, 2.4%] | 4.1% [3.5%, 4.6%] | - | 172780 |
| Release hazard, LOS, aim error in half-widths (0, 1] | 1.6% [1.3%, 1.9%] | 1.4% [1.1%, 1.7%] | - | 17621 |
| Release hazard, LOS, aim error in half-widths (1, 2] | 1.5% [1.3%, 1.8%] | 1.5% [1.3%, 1.9%] | - | 29403 |
| Release hazard, LOS, aim error in half-widths (2, 3] | 1.6% [1.4%, 1.9%] | 1.5% [1.2%, 2.0%] | - | 20846 |
| Release hazard, LOS, aim error in half-widths (3, 4] | 1.7% [1.4%, 1.9%] | 1.4% [1.0%, 2.0%] | - | 11735 |
| Release hazard, LOS, aim error in half-widths (4, 6] | 2.6% [2.1%, 3.1%] | 1.7% [1.3%, 2.3%] | - | 8858 |
| Release hazard, LOS, aim error in half-widths (6, 10] | 9.3% [7.3%, 11.1%] | 6.9% [4.4%, 9.9%] | - | 3207 |
| Release hazard, LOS, aim error in half-widths (10, 1000] ▲ | 17.7% [15.2%, 19.9%] | 32.7% [22.9%, 41.8%] | - | 1868 |
| Release hazard while hidden, LOS lost 0-100 ms ago | 5.0% [4.4%, 6.7%] | 6.3% [5.4%, 7.5%] | - | 13967 |
| Release hazard while hidden, LOS lost 150-250 ms ago ▲ | 11.1% [9.8%, 13.6%] | 16.5% [13.8%, 18.7%] | - | 8333 |
| Release hazard while hidden, LOS lost 300-450 ms ago | 29.6% [26.7%, 33.1%] | 31.5% [29.6%, 33.4%] | - | 8691 |
| Release hazard while hidden, LOS lost 500-950 ms ago ▲ | 13.1% [12.5%, 14.1%] | 10.1% [9.2%, 11.3%] | - | 7242 |
| Release hazard while hidden, LOS lost 1 s or more ago ▲ | 14.3% [13.7%, 14.9%] | 11.3% [9.8%, 13.5%] | - | 19331 |
| Fire held, LOS, aim error in half-widths (0, 0.5] | 82.6% [75.1%, 87.6%] | 89.6% [87.0%, 91.7%] | - | 5933 |
| Fire held, LOS, aim error in half-widths (0.5, 1] | 84.9% [78.0%, 89.2%] | 88.9% [86.9%, 90.5%] | - | 14994 |
| Fire held, LOS, aim error in half-widths (1, 1.5] | 86.5% [81.0%, 90.1%] | 86.3% [84.5%, 87.8%] | - | 18019 |
| Fire held, LOS, aim error in half-widths (1.5, 2] | 84.8% [81.0%, 87.9%] | 81.9% [79.8%, 83.7%] | - | 17397 |
| Fire held, LOS, aim error in half-widths (2, 3] | 82.6% [78.7%, 85.8%] | 81.0% [79.4%, 82.7%] | - | 26218 |
| Fire held, LOS, aim error in half-widths (3, 4] | 77.5% [74.3%, 81.2%] | 78.8% [76.1%, 81.2%] | - | 15636 |
| Fire held, LOS, aim error in half-widths (4, 6] | 64.4% [61.0%, 68.4%] | 67.7% [63.0%, 71.3%] | - | 14097 |
| Fire held, LOS, aim error in half-widths (6, 10] ▲ | 35.1% [30.8%, 39.9%] | 22.4% [17.1%, 27.1%] | - | 9019 |
| Fire held, LOS, aim error in half-widths (10, 20] ▲ | 13.5% [11.7%, 15.5%] | 4.6% [2.4%, 6.9%] | - | 7313 |
| Fire held, LOS, aim error in half-widths (20, 1000] ▲ | 8.0% [6.1%, 10.7%] | 0.9% [0.4%, 3.1%] | - | 13207 |
| Fire held without LOS | 22.2% [19.6%, 25.2%] | 24.5% [22.7%, 26.3%] | - | 288460 |
| Attack hold duration p50 | 200 ms [200 ms, 250 ms] | 150 ms [150 ms, 200 ms] | - | 12768 |
| Attack hold duration p90 | 1000 ms [950 ms, 1050 ms] | 900 ms [800 ms, 950 ms] | - | 12768 |
| Attack holds that are taps (<=100 ms) ▲ | 38.3% [35.5%, 40.3%] | 44.3% [41.8%, 46.5%] | - | 12768 |
| Gap between attack holds p50 | 300 ms [300 ms, 350 ms] | 400 ms [350 ms, 400 ms] | - | 12170 |

### Aim

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error firing with LOS at (0, 128] u ▲ | 18.34° [17.68°, 18.88°] | 14.28° [13.53°, 15.19°] | - | 9941 |
| Crosshair on body firing with LOS at (0, 128] u ▲ | 39.2% [37.6%, 41.0%] | 48.0% [44.9%, 51.5%] | - | 9941 |
| Median aim error firing with LOS at (128, 192] u ▲ | 10.39° [9.86°, 11.07°] | 8.99° [8.66°, 9.45°] | - | 15246 |
| Crosshair on body firing with LOS at (128, 192] u ▲ | 35.9% [33.7%, 38.1%] | 42.5% [39.8%, 44.7%] | - | 15246 |
| Median aim error firing with LOS at (192, 256] u ▲ | 7.61° [7.30°, 7.97°] | 6.68° [6.36°, 7.10°] | - | 15486 |
| Crosshair on body firing with LOS at (192, 256] u ▲ | 33.7% [32.3%, 34.9%] | 42.0% [39.5%, 44.4%] | - | 15486 |
| Median aim error firing with LOS at (256, 384] u | 5.52° [5.27°, 5.89°] | 5.23° [4.96°, 5.59°] | - | 27669 |
| Crosshair on body firing with LOS at (256, 384] u ▲ | 32.8% [31.9%, 33.6%] | 39.0% [36.0%, 41.4%] | - | 27669 |
| Median aim error firing with LOS at (384, 512] u | 3.96° [3.81°, 4.13°] | 3.92° [3.67°, 4.18°] | - | 15363 |
| Crosshair on body firing with LOS at (384, 512] u | 32.7% [30.9%, 34.4%] | 35.4% [32.6%, 38.3%] | - | 15363 |
| Median aim error firing with LOS at (512, 768] u | 3.01° [2.87°, 3.21°] | 3.07° [2.80°, 3.35°] | - | 11240 |
| Crosshair on body firing with LOS at (512, 768] u | 30.7% [29.0%, 31.7%] | 32.0% [29.1%, 35.1%] | - | 11240 |
| Median aim error firing with LOS at (768, 1200] u | 1.89° [1.78°, 2.54°] | 2.14° [1.84°, 2.58°] | - | 452 |
| Crosshair on body firing with LOS at (768, 1200] u | 34.2% [22.2%, 37.0%] | 31.5% [24.3%, 39.5%] | - | 452 |
| Aim height (fraction of body) firing with LOS | 0.411 [0.396, 0.422] | 0.446 [0.418, 0.468] | - | 95397 |
| Aim height (fraction of body) with LOS, not firing | 0.575 [0.54, 0.619] | 0.662 [0.576, 0.751] | - | 70062 |

### View

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw speed (hidden, not firing) p50 ▲ | 16°/s [15°/s, 18°/s] | 8°/s [6°/s, 10°/s] | - | 239960 |
| Yaw speed (hidden, not firing) p99 ▲ | 766°/s [756°/s, 777°/s] | 544°/s [521°/s, 580°/s] | - | 239960 |
| Mouse still between ticks (hidden, not firing) | 30.4% [29.7%, 31.2%] | 32.1% [28.8%, 36.0%] | - | 242190 |
| Yaw speed (hidden, firing) p50 ▲ | 32°/s [29°/s, 34°/s] | 23°/s [21°/s, 25°/s] | - | 59953 |
| Yaw speed (hidden, firing) p99 ▲ | 546°/s [505°/s, 594°/s] | 194°/s [182°/s, 204°/s] | - | 59953 |
| Mouse still between ticks (hidden, firing) ▲ | 7.3% [6.8%, 8.1%] | 10.9% [10.1%, 11.8%] | - | 59953 |
| Yaw speed (LOS, not firing) p50 ▲ | 42°/s [37°/s, 47°/s] | 11°/s [0°/s, 26°/s] | - | 47244 |
| Yaw speed (LOS, not firing) p99 ▲ | 884°/s [854°/s, 911°/s] | 583°/s [505°/s, 695°/s] | - | 47244 |
| Mouse still between ticks (LOS, not firing) ▲ | 18.7% [17.2%, 20.6%] | 36.7% [20.8%, 49.8%] | - | 47257 |
| Yaw speed (LOS firefight) p50 ▲ | 46°/s [43°/s, 49°/s] | 39°/s [36°/s, 41°/s] | - | 95397 |
| Yaw speed (LOS firefight) p99 ▲ | 549°/s [504°/s, 591°/s] | 305°/s [292°/s, 318°/s] | - | 95397 |
| Mouse still between ticks (LOS firefight) ▲ | 3.1% [2.9%, 3.4%] | 1.6% [1.4%, 1.8%] | - | 95397 |
| Yaw speed (reloading) p50 ▲ | 25°/s [23°/s, 28°/s] | 18°/s [16°/s, 20°/s] | - | 51792 |
| Yaw speed (reloading) p99 ▲ | 694°/s [639°/s, 738°/s] | 945°/s [909°/s, 991°/s] | - | 51792 |
| Mouse still between ticks (reloading) ▲ | 21.0% [19.1%, 22.7%] | 26.3% [25.0%, 27.6%] | - | 51792 |
| Peak yaw speed of 90-135 deg turns p50 | 612°/s [602°/s, 622°/s] | 619°/s [595°/s, 655°/s] | - | 4353 |
| Peak yaw speed of 135-180 deg turns p50 | 833°/s [816°/s, 844°/s] | 852°/s [828°/s, 887°/s] | - | 2282 |
| Peak yaw speed of 180-360 deg turns p50 | 1077°/s [1003°/s, 1177°/s] | 1061°/s [1020°/s, 1126°/s] | - | 436 |

### Belief

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw error to the hidden opponent, last seen <=250 ms | 5.28° [4.72°, 5.85°] | 4.92° [4.56°, 5.26°] | - | 35012 |
| View within 30 deg of the hidden opponent, last seen <=250 ms ▲ | 85.6% [83.6%, 87.4%] | 92.7% [91.9%, 93.7%] | - | 35012 |
| Yaw error to the hidden opponent, last seen 250-500 ms | 5.55° [5.00°, 6.09°] | 6.66° [5.99°, 7.28°] | - | 20062 |
| View within 30 deg of the hidden opponent, last seen 250-500 ms ▲ | 89.4% [88.4%, 90.6%] | 92.2% [91.3%, 93.4%] | - | 20062 |
| Yaw error to the hidden opponent, last seen 0.5-1 s | 7.96° [6.95°, 8.98°] | 8.28° [7.48°, 9.11°] | - | 20545 |
| View within 30 deg of the hidden opponent, last seen 0.5-1 s | 85.9% [84.0%, 88.2%] | 88.0% [86.8%, 89.9%] | - | 20545 |
| Yaw error to the hidden opponent, last seen 1-2 s | 11.98° [9.90°, 13.88°] | 11.11° [9.95°, 12.13°] | - | 13740 |
| View within 30 deg of the hidden opponent, last seen 1-2 s | 74.7% [71.6%, 78.7%] | 78.9% [77.1%, 81.1%] | - | 13740 |
| Yaw error to the hidden opponent, last seen 2-4 s ▲ | 27.81° [22.76°, 34.33°] | 16.93° [15.18°, 19.08°] | - | 49178 |
| View within 30 deg of the hidden opponent, last seen 2-4 s ▲ | 51.6% [47.4%, 56.0%] | 64.3% [61.3%, 67.3%] | - | 49178 |
| Yaw error to the hidden opponent, last seen 4-8 s ▲ | 25.93° [21.50°, 29.73°] | 10.29° [9.19°, 11.80°] | - | 61192 |
| View within 30 deg of the hidden opponent, last seen 4-8 s ▲ | 53.2% [50.3%, 56.8%] | 76.4% [73.1%, 79.3%] | - | 61192 |
| Yaw error to the hidden opponent, last seen >8 s | 19.66° [16.92°, 22.32°] | 20.07° [12.82°, 29.16°] | - | 16815 |
| View within 30 deg of the hidden opponent, last seen >8 s | 62.5% [58.6%, 66.7%] | 65.1% [54.8%, 74.8%] | - | 16815 |
| Yaw error to the hidden opponent, last seen never seen this life | 23.36° [20.72°, 26.69°] | 19.23° [16.93°, 24.71°] | - | 114580 |
| View within 30 deg of the hidden opponent, last seen never seen this life | 56.9% [53.5%, 60.3%] | 62.5% [56.0%, 67.2%] | - | 114580 |

### Space

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) ▲ | 37.7% [29.1%, 40.2%] | 49.4% [44.0%, 50.8%] | - | 120240 |
| Share of duel time in the busiest 10% of 32u cells (dm/main) ▲ | 35.6% [31.9%, 35.8%] | 45.2% [40.5%, 48.6%] | - | 121440 |
| Share of duel time in the busiest 10% of 32u cells (dm/vents) | 43.8% [38.1%, 45.8%] | 44.3% [39.2%, 46.8%] | - | 119980 |
| Share of duel time in the busiest 10% of 32u cells (dm/downladder) ▲ | 40.7% [31.9%, 42.3%] | 61.4% [42.9%, 63.8%] | - | 134930 |

### Acquisition

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error at -500 ms from sight gain | 16.48° [14.28°, 18.41°] | 13.98° [12.90°, 15.10°] | - | 5452 |
| Median aim error at -200 ms from sight gain ▲ | 14.90° [12.72°, 16.86°] | 8.33° [7.70°, 8.92°] | - | 5461 |
| Median aim error at +0 ms from sight gain ▲ | 13.56° [11.81°, 15.22°] | 5.22° [4.88°, 5.61°] | - | 5472 |
| Median aim error at +100 ms from sight gain ▲ | 11.44° [9.94°, 13.01°] | 5.97° [5.54°, 6.43°] | - | 5472 |
| Median aim error at +200 ms from sight gain ▲ | 9.01° [7.79°, 10.26°] | 4.92° [4.54°, 5.23°] | - | 5472 |
| Median aim error at +300 ms from sight gain ▲ | 7.66° [6.51°, 8.76°] | 4.53° [4.12°, 4.88°] | - | 5391 |
| Median aim error at +500 ms from sight gain | 6.68° [5.75°, 7.49°] | 5.47° [5.08°, 5.88°] | - | 5015 |
| Median aim error at +1000 ms from sight gain | 7.16° [6.21°, 8.25°] | 7.25° [6.72°, 7.70°] | - | 4069 |
| Fire held at -500 ms from sight gain | 22.0% [19.8%, 24.8%] | 18.2% [16.5%, 20.2%] | - | 5472 |
| Fire held at -200 ms from sight gain | 19.9% [17.2%, 23.4%] | 21.7% [19.7%, 23.9%] | - | 5472 |
| Fire held at +0 ms from sight gain ▲ | 26.0% [23.2%, 29.4%] | 42.5% [39.1%, 46.0%] | - | 5472 |
| Fire held at +100 ms from sight gain ▲ | 35.4% [31.8%, 38.6%] | 61.4% [57.8%, 64.6%] | - | 5472 |
| Fire held at +200 ms from sight gain ▲ | 47.9% [43.7%, 51.8%] | 77.0% [73.9%, 79.5%] | - | 5472 |
| Fire held at +400 ms from sight gain ▲ | 64.8% [60.5%, 67.8%] | 87.3% [85.5%, 89.0%] | - | 5335 |
| Fire held at +700 ms from sight gain ▲ | 64.4% [62.3%, 65.9%] | 77.0% [74.3%, 79.7%] | - | 5056 |
| Fire held at +1000 ms from sight gain ▲ | 58.8% [56.5%, 60.6%] | 63.4% [60.9%, 66.4%] | - | 4766 |
| Fire held at +1450 ms from sight gain | 53.7% [51.1%, 55.4%] | 53.6% [49.3%, 58.5%] | - | 4378 |
| Reaction: first trigger press after a clean sighting p25 | 100 ms [100 ms, 150 ms] | 50 ms [50 ms, 100 ms] | - | 2743 |
| Reaction: first trigger press after a clean sighting p50 | 250 ms [200 ms, 250 ms] | 150 ms [150 ms, 200 ms] | - | 2743 |
| Reaction: first trigger press after a clean sighting p75 ▲ | 400 ms [350 ms, 450 ms] | 300 ms [250 ms, 300 ms] | - | 2743 |
| Reaction: first trigger press after a clean sighting p90 ▲ | 700 ms [650 ms, 750 ms] | 400 ms [400 ms, 450 ms] | - | 2743 |
| Crosshair within half-width+1.5 deg after a clean sighting p50 | 300 ms [250 ms, 300 ms] | 200 ms [200 ms, 250 ms] | - | 2588 |
| Crosshair already on the body at the first visible tick ▲ | 12.0% [11.2%, 13.1%] | 22.6% [20.9%, 24.4%] | - | 5472 |
| Fire already held on the tick before sight (prefire) ▲ | 22.4% [19.8%, 25.6%] | 35.4% [32.2%, 38.6%] | - | 5472 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shots per burst (consecutive 100 ms shots) p50 | 5 [4, 5] | 5 [4, 5] | - | 12106 |
| Shots per burst (consecutive 100 ms shots) p90 ▲ | 16 [14, 17] | 12 [11, 13] | - | 12106 |

### Combat

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shot accuracy (eligible SMG shots that hit) ▲ | 15.8% [14.7%, 17.1%] | 19.2% [18.1%, 20.3%] | - | 75335 |

### Lives

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Respawn delay after death p50 ▲ | 3000 ms [2999 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | - | 2233 |
| Life length (lives ending in death) p50 ▲ | 10.9 s [10.2 s, 12.4 s] | 7.1 s [5.8 s, 8.2 s] | - | 2234 |

### Reload

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Reload within 3 s of a kill, rounds left 0 | 100.0% [100.0%, 100.0%] | 95.5% [86.4%, 100.0%] | - | 46 |
| Reload within 3 s of a kill, rounds left 1-3 | 99.6% [98.7%, 100.0%] | 96.4% [92.6%, 99.2%] | - | 134 |
| Reload within 3 s of a kill, rounds left 4-6 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 158 |
| Reload within 3 s of a kill, rounds left 7-10 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 248 |
| Reload within 3 s of a kill, rounds left 11-15 | 98.8% [97.6%, 99.7%] | 98.8% [97.9%, 99.5%] | - | 368 |
| Reload within 3 s of a kill, rounds left 16-20 | 95.4% [93.5%, 96.6%] | 95.7% [93.5%, 97.7%] | - | 444 |
| Reload within 3 s of a kill, rounds left 21-25 | 81.8% [77.8%, 86.6%] | 83.1% [76.5%, 88.5%] | - | 474 |
| Reload within 3 s of a kill, rounds left 26-29 | 67.5% [61.8%, 71.9%] | 63.4% [52.8%, 75.0%] | - | 302 |
| Reload within 3 s of a kill, rounds left 30-32 | 62.6% [54.4%, 73.1%] | 73.7% [50.0%, 92.3%] | - | 58 |
| Delay from kill to reload (reloads within 3 s) p50 | 650 ms [650 ms, 650 ms] | 600 ms [600 ms, 650 ms] | - | 2003 |
| Reloads started with the opponent dead ▲ | 69.1% [65.5%, 74.1%] | 88.2% [85.5%, 90.4%] | - | 2869 |
| Reloads with the opponent alive that are forced (empty clip) ▲ | 92.2% [88.7%, 94.4%] | 77.7% [69.5%, 83.9%] | - | 860 |

### Fights

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Engagements that end in a kill ▲ | 90.1% [89.2%, 90.9%] | 79.9% [77.9%, 81.9%] | - | 5056 |
| First hitter wins (decisive engagements) | 65.3% [63.8%, 66.9%] | 65.4% [63.7%, 66.7%] | - | 4458 |
| First shooter wins (decisive engagements) | 54.7% [54.0%, 55.2%] | 55.0% [52.5%, 56.6%] | - | 4458 |
| Duration of decisive engagements p50 ▲ | 1950 ms [1750 ms, 2000 ms] | 1250 ms [1150 ms, 1351 ms] | - | 4458 |
| Engagement start distance p50 | 411 u [369 u, 451 u] | 427 u [398 u, 470 u] | - | 5056 |
| Decisive engagements won (subject's own) | 49.2% [46.1%, 53.1%] | 50.0% [45.6%, 54.2%] | - | 4458 |

### Perception

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time with a body part on screen and the centroid hidden | 8.2% [5.2%, 10.2%] | 10.5% [9.6%, 11.8%] | - | 496590 |
| Share of duel time with a body part of the enemy on screen | 37.8% [34.3%, 40.7%] | 34.0% [31.6%, 36.5%] | - | 496590 |
| Sightings (a body part coming on screen) per minute of duel time | 31.09/min [28.78/min, 32.81/min] | 33.29/min [31.94/min, 34.72/min] | - | 496590 |
| Spell with no body part on screen between two sightings p50 | 300 ms [250 ms, 450 ms] | 400 ms [300 ms, 500 ms] | - | 7678 |
| Spell with no body part on screen between two sightings p75 | 700 ms [600 ms, 850 ms] | 900 ms [800 ms, 1000 ms] | - | 7678 |
| Spell with no body part on screen between two sightings p90 | 1550 ms [1360 ms, 1901 ms] | 1750 ms [1600 ms, 1950 ms] | - | 7678 |
| Time from both alive to the first body part on screen p50 | 1500 ms [1399 ms, 1650 ms] | 1350 ms [1300 ms, 1400 ms] | - | 4482 |
| Time from both alive to the first body part on screen p75 ▲ | 2800 ms [2650 ms, 3050 ms] | 1900 ms [1800 ms, 2000 ms] | - | 4482 |
| Net distance covered in 2 s from a tick with no body part on screen p50 ▲ | 116 u [109 u, 124 u] | 158 u [146 u, 173 u] | - | 25738 |
| Net distance covered in 2 s from a tick with no body part on screen p25 | 58 u [54 u, 62 u] | 66 u [54 u, 78 u] | - | 25738 |
| Fire held with no body part of the enemy visible (loaded, not reloading) | 16.2% [15.1%, 17.7%] | 17.3% [15.8%, 19.2%] | - | 284700 |
| Fire held with no body part visible, 0-500ms after one was last on screen ▲ | 48.3% [47.4%, 49.6%] | 43.2% [40.9%, 45.8%] | - | 42859 |
| Fire held with no body part visible, 500-1000ms after one was last on screen | 24.9% [23.2%, 27.0%] | 26.4% [23.9%, 28.7%] | - | 20303 |
| Fire held with no body part visible, 1000-2000ms after one was last on screen | 16.6% [14.0%, 19.5%] | 20.5% [18.0%, 22.9%] | - | 18631 |
| Fire held with no body part visible, 2000-5000ms after one was last on screen | 10.3% [8.5%, 12.5%] | 7.7% [6.4%, 9.1%] | - | 72786 |
| Fire held with no body part visible, gt5000ms after one was last on screen | 6.2% [5.5%, 7.3%] | 4.3% [3.1%, 6.0%] | - | 57483 |
| Attack holds begun loaded that are taps (<=100 ms), with a body part visible ▲ | 9.9% [8.6%, 11.6%] | 15.4% [13.1%, 17.6%] | - | 4517 |
| Attack holds begun loaded that are taps (<=100 ms), with no body part visible | 44.5% [41.5%, 46.6%] | 45.9% [42.2%, 48.9%] | - | 6612 |
| Shots per burst whose first round left with a body part visible p50 | 6 [6, 6] | 6 [5, 6] | - | 7686 |
| Shots per burst whose first round left with a body part visible p90 ▲ | 17 [16, 19] | 13 [12, 14] | - | 7686 |
| SMG shots that hit, a part on screen, centroid hidden ▲ | 13.3% [12.8%, 14.1%] | 21.7% [18.9%, 24.3%] | - | 9333 |
| SMG shots that hit, centroid visible ▲ | 21.9% [20.4%, 23.4%] | 27.2% [25.8%, 28.5%] | - | 47500 |
| Centroid sightings with a body part on screen first ▲ | 33.0% [30.1%, 36.4%] | 85.9% [84.2%, 87.7%] | - | 5452 |
| Lead of the first part over the centroid p50 ▲ | 50 ms [50 ms, 50 ms] | 100 ms [100 ms, 100 ms] | - | 1732 |
| Median aim error at -500 ms from the first visible part | 14.43° [13.45°, 15.41°] | 15.23° [14.18°, 16.19°] | - | 2957 |
| Median aim error at -200 ms from the first visible part | 11.99° [10.92°, 12.99°] | 10.54° [9.80°, 11.23°] | - | 2957 |
| Median aim error at -100 ms from the first visible part ▲ | 11.22° [9.94°, 12.16°] | 8.00° [7.34°, 8.49°] | - | 2957 |
| Median aim error at +0 ms from the first visible part ▲ | 10.27° [9.29°, 11.11°] | 5.22° [4.91°, 5.53°] | - | 2957 |
| Median aim error at +100 ms from the first visible part ▲ | 8.53° [7.77°, 9.38°] | 4.87° [4.54°, 5.18°] | - | 2957 |
| Median aim error at +200 ms from the first visible part | 6.03° [5.37°, 6.86°] | 5.26° [4.86°, 5.58°] | - | 2957 |
| Fire already held on the tick before the first part (prefire) ▲ | 18.5% [17.3%, 19.6%] | 23.0% [20.3%, 25.5%] | - | 2957 |
| Reaction: first press after a clean sighting, from the first part p50 | 200 ms [200 ms, 250 ms] | 200 ms [200 ms, 250 ms] | - | 1709 |
| Reaction: mean first press after a clean sighting, from the first part | 252 ms [218 ms, 297 ms] | 232 ms [217 ms, 250 ms] | - | 1709 |
| Crosshair within half-width+1.5 deg after a clean sighting, from the first part p50 | 250 ms [250 ms, 250 ms] | 250 ms [200 ms, 250 ms] | - | 1676 |
| First hit after the first part p50 ▲ | 350 ms [350 ms, 400 ms] | 250 ms [200 ms, 250 ms] | - | 1892 |
| Part sightings whose visible bout ends without a hit ▲ | 36.2% [31.2%, 41.6%] | 27.5% [25.7%, 29.7%] | - | 2957 |
| Yaw error to the hidden enemy 500 ms before the first part | 13.01° [12.11°, 14.24°] | 14.63° [13.70°, 15.63°] | - | 2957 |
| Yaw error to where the enemy will appear, 500 ms before the first part | 13.14° [11.79°, 15.07°] | 10.65° [9.39°, 11.89°] | - | 2957 |
| Crosshair parked within 5 deg of where the enemy will appear, 500 ms before | 26.0% [23.5%, 28.4%] | 29.4% [27.1%, 32.4%] | - | 2957 |
| Crosshair closer to where the enemy will appear than to the enemy, -500 ms ▲ | 50.7% [47.6%, 53.0%] | 67.0% [65.6%, 68.8%] | - | 2957 |
| View turn toward the enemy over the last 500 ms before the first part ▲ | 10.09° [9.07°, 11.12°] | 13.01° [11.44°, 14.55°] | - | 2957 |
| View turn over the last 500 ms before the first part ▲ | 12.25° [11.29°, 13.11°] | 14.53° [13.34°, 15.90°] | - | 2957 |
| Speed 400-200 ms before the first part ▲ | 159 u/s [146 u/s, 171 u/s] | 188 u/s [182 u/s, 193 u/s] | - | 2957 |
| Speed 2-1 s before the first part ▲ | 143 u/s [136 u/s, 147 u/s] | 167 u/s [159 u/s, 176 u/s] | - | 1631 |
| Hidden, no sighting next: crosshair closer to where the enemy will be in 500 ms | 50.3% [49.5%, 51.0%] | 50.5% [49.0%, 51.6%] | - | 209700 |
| Yaw error to the corner, 500 ms before the first part ▲ | 10.62° [9.99°, 12.53°] | 5.81° [5.17°, 6.44°] | - | 2002 |
| Angle to the corner, 500 ms before the first part ▲ | 12.50° [11.31°, 14.12°] | 8.41° [7.70°, 9.19°] | - | 2002 |
| Pitch to the corner (- = corner above the crosshair), 500 ms before the first part | -2.18° [-2.53°, -1.87°] | -2.72° [-3.14°, -2.39°] | - | 2002 |
| Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part ▲ | 0.14° [-0.60°, 1.14°] | -1.77° [-2.04°, -1.43°] | - | 2002 |
| Yaw error to the appearance point (sightings with a corner), 500 ms before the first part | 11.33° [9.73°, 12.75°] | 10.34° [9.19°, 11.29°] | - | 2002 |
| Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part ▲ | 11.95° [10.34°, 13.55°] | 15.18° [14.24°, 16.01°] | - | 2002 |
| Crosshair within 5 deg of the corner, 500 ms before the first part ▲ | 20.6% [18.3%, 22.8%] | 29.2% [26.4%, 32.7%] | - | 2002 |
| Crosshair closer to the corner than to the enemy, 500 ms before the first part ▲ | 57.5% [54.4%, 59.9%] | 80.0% [78.8%, 81.1%] | - | 2002 |
| Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part ▲ | 41.7% [38.5%, 43.9%] | 48.2% [45.7%, 50.3%] | - | 2002 |
| Crosshair within 2 deg of the corner's edge, 500 ms before the first part ▲ | 15.3% [13.3%, 16.9%] | 23.7% [21.7%, 25.7%] | - | 2002 |
| Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part ▲ | 43.0% [39.7%, 47.0%] | 28.1% [26.8%, 29.4%] | - | 2002 |
| Angle to the corner at the first part ▲ | 8.64° [7.95°, 9.70°] | 5.74° [5.33°, 6.17°] | - | 2002 |
| Distance from the eye to the corner p50 | 241 u [213 u, 274 u] | 228 u [204 u, 248 u] | - | 2002 |
| Yaw error to the corner at -2000 ms (sightings hidden >= 2 s) | 18.30° [14.00°, 23.95°] | 14.66° [11.85°, 17.22°] | - | 575 |
| Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) ▲ | 16.77° [12.37°, 20.20°] | 5.78° [4.88°, 6.98°] | - | 575 |
| Yaw error to the corner at -500 ms (sightings hidden >= 2 s) ▲ | 10.67° [9.47°, 11.65°] | 4.66° [3.99°, 5.31°] | - | 575 |
| Yaw error to the corner at +0 ms (sightings hidden >= 2 s) ▲ | 6.68° [5.80°, 8.41°] | 3.54° [3.01°, 4.10°] | - | 575 |

Consistency: 112 pooled statistics (bots and owner together) were recomputed from the analysis scripts' own JSON; 0 differ.

## Notes

- perception statistics on the bots' logged body-part column (ext_vis_parts: their own perception); the people's are on the data repo's rebuilt column, accepted against the logged one on 2026-09-28
- 100% of the bots' duel time is against another bot (the logged opponent is the nearest living enemy); the human reference is humans against humans only
- engagements.py counts contact episodes only while exactly two players are alive; with more than two players in a session the fight statistics cover the phases where one player is dead
