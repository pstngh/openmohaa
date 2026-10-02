# Bot evaluation: 2026-10-02_encounters_realmaps

Generated 2026-10-02 by `humanbot/eval/compare.py`. Captures: `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`. 8 sessions on dm/crnodoors, dm/downladder, dm/main, dm/vents; capture dates 2026-10-02.
Human reference: `human_reference.json` (2026-10-02), 397 duel minutes, 72 player-sessions, 5 people.

## Bots in this capture

| Bot | Family | Seed | Sessions | Duel minutes |
|---|---|---:|---:|---:|
| `bot:strafer:4` | strafer | 231333596 | 1 | 25.3 |
| `bot:presser:5` | presser | 1152489726 | 1 | 25.3 |
| `bot:stopper:4` | stopper | 711847621 | 1 | 26.7 |
| `bot:stopper:5` | stopper | 1529931226 | 1 | 26.7 |
| `bot:strafer:4.2` | strafer | 1086809828 | 1 | 28.1 |
| `bot:strafer:5` | strafer | 821368291 | 1 | 28.1 |
| `bot:stopper:4.2` | stopper | 97502759 | 1 | 28.3 |
| `bot:strafer:5.2` | strafer | 2592015535 | 1 | 28.3 |
| `bot:strafer:4.3` | strafer | 3498264623 | 1 | 24.5 |
| `bot:stopper:5.2` | stopper | 2513918026 | 1 | 24.5 |
| `bot:stopper:4.3` | stopper | 2458011590 | 1 | 25.7 |
| `bot:strafer:5.3` | strafer | 4003987225 | 1 | 25.7 |
| `bot:presser:4` | presser | 588158740 | 1 | 27.4 |
| `bot:presser:5.2` | presser | 4131749178 | 1 | 27.4 |
| `bot:presser:4.2` | presser | 52844662 | 1 | 27.0 |
| `bot:strafer:5.4` | strafer | 3859071546 | 1 | 27.0 |

The owner (`pstN@vsbot`) has 0.0 duel minutes against the bots; those rows are never pooled with the bots.

Bot duel time against the owner 0%, against other bots 100%.

Family mix of bot duel time (observed → reweighted to the styles.json targets): presser 25% → 20%, stopper 31% → 40%, strafer 44% → 40%.

## Tells (156)

Statistics whose 95% CIs for the pooled bots and the pooled humans do not overlap, ranked by the standardized gap z = (bots − humans) / √(SE²bots + SE²humans). Only statistics with enough data on both sides are ranked.

| # | Statistic | Bots | Humans | z |
|---:|---|---|---|---:|
| 1 | Centroid sightings with a body part on screen first | 31.9% [29.7%, 34.4%] | 85.9% [84.2%, 87.7%] | -33.8 |
| 2 | Side-hold duration (all contexts) p50 | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | -28.3 |
| 3 | Crosshair closer to the corner than to the enemy, 500 ms before the first part | 56.2% [54.7%, 58.0%] | 80.0% [78.8%, 81.1%] | -22.2 |
| 4 | Fire held at +400 ms from sight gain | 62.7% [61.2%, 64.3%] | 87.3% [85.5%, 89.0%] | -19.4 |
| 5 | Respawn delay after death p50 | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | +17.0 |
| 6 | Side-hold duration (LOS firefight) p25 | 100 ms [100 ms, 100 ms] | 200 ms [200 ms, 200 ms] | -16.4 |
| 7 | Yaw speed (hidden, not firing) p99 | 826°/s [808°/s, 847°/s] | 544°/s [521°/s, 580°/s] | +15.9 |
| 8 | Yaw speed (hidden, firing) p99 | 569°/s [514°/s, 607°/s] | 194°/s [182°/s, 204°/s] | +15.2 |
| 9 | Fire held at +200 ms from sight gain | 49.1% [47.0%, 51.5%] | 77.0% [73.9%, 79.5%] | -14.7 |
| 10 | Side-hold duration (LOS firefight) p75 | 300 ms [300 ms, 300 ms] | 450 ms [449 ms, 450 ms] | -14.1 |
| 11 | Side-hold duration (LOS firefight) p90 | 450 ms [450 ms, 450 ms] | 700 ms [650 ms, 700 ms] | -13.5 |
| 12 | Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part | 47.6% [45.4%, 50.8%] | 28.1% [26.8%, 29.4%] | +12.8 |
| 13 | Crosshair already on the body at the first visible tick | 10.9% [10.0%, 11.7%] | 22.6% [20.9%, 24.4%] | -11.5 |
| 14 | Speed 400-200 ms before the first part | 144 u/s [141 u/s, 150 u/s] | 188 u/s [182 u/s, 193 u/s] | -11.1 |
| 15 | Strafe switch hazard per tick at hold age 50 ms (LOS firefight) | 10.8% [9.2%, 12.5%] | 1.2% [0.9%, 1.6%] | +11.1 |
| 16 | Crosshair closer to where the enemy will appear than to the enemy, -500 ms | 52.6% [50.5%, 55.0%] | 67.0% [65.6%, 68.8%] | -10.4 |
| 17 | Retreating >40 u/s in LOS firefight at distance (224, 288] | 14.6% [13.4%, 15.6%] | 5.3% [4.2%, 6.8%] | +10.3 |
| 18 | Key chord fwd diag (hidden, not firing) | 13.2% [12.6%, 13.9%] | 29.9% [26.7%, 33.2%] | -10.3 |
| 19 | Fire held at +100 ms from sight gain | 37.8% [35.2%, 40.8%] | 61.4% [57.8%, 64.6%] | -10.1 |
| 20 | Key chord forward (hidden, not firing) | 18.8% [17.7%, 19.6%] | 11.9% [10.9%, 12.8%] | +10.0 |
| 21 | Strafe switch hazard per tick at hold age 100 ms (LOS firefight) | 18.1% [15.5%, 19.9%] | 5.8% [5.2%, 6.8%] | +9.9 |
| 22 | Side-hold duration (all contexts) p90 | 500 ms [500 ms, 550 ms] | 800 ms [800 ms, 850 ms] | -9.8 |
| 23 | Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) | 25.4% [24.0%, 26.8%] | 15.6% [14.7%, 16.9%] | +9.7 |
| 24 | Mouse still between ticks (LOS firefight) | 3.4% [3.1%, 3.6%] | 1.6% [1.4%, 1.8%] | +9.6 |
| 25 | Retreating >40 u/s in LOS firefight at distance (160, 224] | 18.2% [16.7%, 20.1%] | 6.9% [5.5%, 8.7%] | +9.4 |
| 26 | Yaw speed (LOS firefight) p99 | 603°/s [529°/s, 660°/s] | 305°/s [292°/s, 318°/s] | +9.1 |
| 27 | Yaw speed (hidden, not firing) p50 | 20°/s [18°/s, 22°/s] | 8°/s [6°/s, 10°/s] | +8.9 |
| 28 | Reaction: first trigger press after a clean sighting p90 | 750 ms [700 ms, 800 ms] | 400 ms [400 ms, 450 ms] | +8.8 |
| 29 | Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part | 1.31° [0.89°, 2.12°] | -1.77° [-2.04°, -1.43°] | +8.8 |
| 30 | Duration of decisive engagements p50 | 1950 ms [1850 ms, 2051 ms] | 1250 ms [1150 ms, 1351 ms] | +8.7 |
| 31 | Retreating >40 u/s in LOS firefight at distance (96, 160] | 23.4% [21.4%, 25.8%] | 10.5% [8.5%, 12.6%] | +8.7 |
| 32 | Median aim error at +0 ms from the first visible part | 11.55° [9.83°, 12.68°] | 5.22° [4.91°, 5.53°] | +8.6 |
| 33 | Fire held at +700 ms from sight gain | 62.0% [60.2%, 64.3%] | 77.0% [74.3%, 79.7%] | -8.5 |
| 34 | Median aim error at +0 ms from sight gain | 15.49° [12.83°, 17.84°] | 5.22° [4.88°, 5.61°] | +8.4 |
| 35 | View within 30 deg of the hidden opponent, last seen 4-8 s | 51.4% [46.9%, 57.0%] | 76.4% [73.1%, 79.3%] | -8.3 |
| 36 | Yaw error to the corner, 500 ms before the first part | 10.74° [9.83°, 11.78°] | 5.81° [5.17°, 6.44°] | +8.3 |
| 37 | Median aim error at +100 ms from the first visible part | 9.78° [8.41°, 10.73°] | 4.87° [4.54°, 5.18°] | +8.0 |
| 38 | Key chord fwd diag (reloading) | 20.6% [18.1%, 23.4%] | 36.8% [34.1%, 39.9%] | -7.9 |
| 39 | Angle to the corner at the first part | 10.03° [8.80°, 11.00°] | 5.74° [5.33°, 6.17°] | +7.6 |
| 40 | Speed 2-1 s before the first part | 131 u/s [126 u/s, 137 u/s] | 167 u/s [159 u/s, 176 u/s] | -7.5 |
| 41 | Net distance covered in 2 s from a tick with no body part on screen p50 | 103 u [100 u, 107 u] | 158 u [146 u, 173 u] | -7.5 |
| 42 | Press hazard while hidden, LOS lost 300-500 ms ago | 11.1% [10.7%, 11.7%] | 7.0% [6.0%, 8.1%] | +7.3 |
| 43 | SMG shots that hit, a part on screen, centroid hidden | 12.3% [11.6%, 13.4%] | 21.7% [18.9%, 24.3%] | -7.2 |
| 44 | Angle to the corner, 500 ms before the first part | 12.99° [11.88°, 13.77°] | 8.41° [7.70°, 9.19°] | +7.2 |
| 45 | Key chord neutral (reloading) | 22.3% [18.1%, 25.5%] | 8.3% [7.3%, 9.5%] | +7.1 |
| 46 | Mouse still between ticks (hidden, firing) | 7.4% [6.9%, 7.8%] | 10.9% [10.1%, 11.8%] | -7.1 |
| 47 | Median aim error at -200 ms from sight gain | 16.21° [13.80°, 18.14°] | 8.33° [7.70°, 8.92°] | +7.0 |
| 48 | Yaw speed (hidden, firing) p50 | 35°/s [32°/s, 37°/s] | 23°/s [21°/s, 25°/s] | +7.0 |
| 49 | Median aim error at +100 ms from sight gain | 13.53° [10.93°, 15.46°] | 5.97° [5.54°, 6.43°] | +7.0 |
| 50 | Approaching >40 u/s in LOS firefight at distance (96, 160] | 19.0% [16.7%, 21.0%] | 36.4% [32.6%, 41.2%] | -7.0 |
| 51 | Yaw speed (LOS, not firing) p99 | 914°/s [894°/s, 944°/s] | 583°/s [505°/s, 695°/s] | +7.0 |
| 52 | Strafe switch hazard per tick at hold age 200 ms (LOS firefight) | 24.2% [23.0%, 25.4%] | 17.0% [15.4%, 18.7%] | +7.0 |
| 53 | Mouse still between ticks (reloading) | 20.2% [19.0%, 21.1%] | 26.3% [25.0%, 27.6%] | -6.9 |
| 54 | Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago | 23.4% [21.1%, 27.4%] | 7.4% [4.7%, 10.8%] | +6.9 |
| 55 | Time from both alive to the first body part on screen p75 | 3450 ms [3050 ms, 3951 ms] | 1900 ms [1800 ms, 2000 ms] | +6.8 |
| 56 | Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) | 23.9% [22.2%, 25.8%] | 16.1% [14.9%, 17.5%] | +6.8 |
| 57 | Fire held at +0 ms from sight gain | 27.7% [25.1%, 30.5%] | 42.5% [39.1%, 46.0%] | -6.8 |
| 58 | Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part | 37.2% [34.5%, 38.9%] | 48.2% [45.7%, 50.3%] | -6.7 |
| 59 | Crosshair within 2 deg of the corner's edge, 500 ms before the first part | 15.2% [13.9%, 16.6%] | 23.7% [21.7%, 25.7%] | -6.7 |
| 60 | Reloads started with the opponent dead | 68.1% [63.3%, 73.9%] | 88.2% [85.5%, 90.4%] | -6.7 |
| 61 | Press hazard while hidden, LOS lost 150-250 ms ago | 9.3% [8.3%, 10.5%] | 4.6% [3.9%, 5.5%] | +6.6 |
| 62 | Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) | 25.9% [22.7%, 28.9%] | 13.7% [12.0%, 15.6%] | +6.5 |
| 63 | Yaw error to the corner at +0 ms (sightings hidden >= 2 s) | 8.70° [6.92°, 10.22°] | 3.54° [3.01°, 4.10°] | +6.4 |
| 64 | View within 30 deg of the hidden opponent, last seen <=250 ms | 85.0% [82.9%, 87.2%] | 92.7% [91.9%, 93.7%] | -6.3 |
| 65 | Strafe switch hazard per tick at hold age 150 ms (LOS firefight) | 23.1% [19.3%, 25.3%] | 12.4% [11.1%, 13.9%] | +6.1 |
| 66 | Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) | 36.9% [33.7%, 37.3%] | 49.4% [44.0%, 50.8%] | -6.1 |
| 67 | Life length (lives ending in death) p50 | 12.4 s [11.7 s, 13.7 s] | 7.1 s [5.8 s, 8.2 s] | +6.1 |
| 68 | Press hazard while hidden, LOS lost over 1 s ago | 2.1% [1.8%, 2.6%] | 4.1% [3.5%, 4.6%] | -6.0 |
| 69 | Median aim error at +200 ms from sight gain | 11.10° [8.64°, 12.69°] | 4.92° [4.54°, 5.23°] | +6.0 |
| 70 | Strafe switch hazard per tick at hold age 250 ms (LOS firefight) | 25.5% [23.2%, 27.3%] | 17.4% [16.0%, 19.0%] | +6.0 |
| 71 | Approaching >40 u/s in LOS firefight at distance (160, 224] | 17.3% [15.3%, 19.1%] | 35.4% [30.1%, 42.0%] | -5.9 |
| 72 | Yaw speed (reloading) p50 | 30°/s [26°/s, 33°/s] | 18°/s [16°/s, 20°/s] | +5.8 |
| 73 | Key chord back any (LOS, not firing) | 10.2% [9.2%, 11.0%] | 4.8% [3.4%, 6.7%] | +5.8 |
| 74 | Fire held, LOS, aim error in half-widths (20, 1000] | 6.9% [5.3%, 8.3%] | 0.9% [0.4%, 3.1%] | +5.7 |
| 75 | Approaching >40 u/s in LOS firefight at distance (0, 96] | 23.6% [21.7%, 25.5%] | 37.6% [33.7%, 42.3%] | -5.6 |
| 76 | Yaw error to the corner at -500 ms (sightings hidden >= 2 s) | 11.75° [9.91°, 13.97°] | 4.66° [3.99°, 5.31°] | +5.6 |
| 77 | Share of duel time in the busiest 10% of 32u cells (dm/main) | 32.8% [29.9%, 33.2%] | 45.2% [40.5%, 48.6%] | -5.5 |
| 78 | Key chord fwd diag (hidden, firing) | 14.8% [13.3%, 16.3%] | 28.6% [23.9%, 32.8%] | -5.5 |
| 79 | Fire already held on the tick before sight (prefire) | 23.3% [20.7%, 26.4%] | 35.4% [32.2%, 38.6%] | -5.5 |
| 80 | Key chord fwd diag (LOS firefight) | 11.9% [9.6%, 14.8%] | 30.5% [25.6%, 36.7%] | -5.4 |
| 81 | Side-hold duration (LOS firefight) p50 | 200 ms [200 ms, 200 ms] | 300 ms [250 ms, 300 ms] | -5.4 |
| 82 | Approaching >40 u/s in LOS firefight at distance (224, 288] | 17.6% [15.3%, 20.6%] | 38.2% [32.3%, 46.4%] | -5.4 |
| 83 | Yaw speed (LOS, not firing) p50 | 52°/s [46°/s, 56°/s] | 11°/s [0°/s, 26°/s] | +5.4 |
| 84 | Yaw error to the hidden opponent, last seen 4-8 s | 28.16° [21.17°, 34.19°] | 10.29° [9.19°, 11.80°] | +5.4 |
| 85 | Release hazard while hidden, LOS lost 500-950 ms ago | 13.9% [13.0%, 14.8%] | 10.1% [9.2%, 11.3%] | +5.4 |
| 86 | Crosshair on body firing with LOS at (192, 256] u | 32.6% [30.7%, 34.4%] | 42.0% [39.5%, 44.4%] | -5.4 |
| 87 | Median aim error at +300 ms from sight gain | 9.13° [7.20°, 10.69°] | 4.53° [4.12°, 4.88°] | +5.2 |
| 88 | Median aim error firing with LOS at (128, 192] u | 10.77° [10.16°, 11.23°] | 8.99° [8.66°, 9.45°] | +5.2 |
| 89 | Yaw speed (reloading) p99 | 781°/s [731°/s, 821°/s] | 945°/s [909°/s, 991°/s] | -5.1 |
| 90 | Key chord back any (reloading) | 10.6% [9.8%, 11.8%] | 6.5% [5.3%, 7.6%] | +5.1 |
| 91 | Approaching >40 u/s in LOS firefight at distance (288, 384] | 18.1% [14.9%, 22.4%] | 39.4% [32.7%, 47.2%] | -5.1 |
| 92 | Key chord forward (hidden, firing) | 12.0% [9.9%, 13.8%] | 6.6% [5.7%, 7.4%] | +5.0 |
| 93 | Release hazard, LOS, aim error in half-widths (4, 6] | 3.8% [3.1%, 4.3%] | 1.7% [1.3%, 2.3%] | +4.9 |
| 94 | Retreating >40 u/s in LOS firefight at distance (288, 384] | 11.4% [10.1%, 13.2%] | 5.9% [4.6%, 7.3%] | +4.9 |
| 95 | Fire held, LOS, aim error in half-widths (10, 20] | 13.6% [11.0%, 16.9%] | 4.6% [2.4%, 6.9%] | +4.9 |
| 96 | Median aim error firing with LOS at (0, 128] u | 17.24° [16.50°, 18.19°] | 14.28° [13.53°, 15.19°] | +4.8 |
| 97 | Share of duel time with a body part on screen and the centroid hidden | 6.3% [5.1%, 7.7%] | 10.5% [9.6%, 11.8%] | -4.8 |
| 98 | Crosshair on body firing with LOS at (128, 192] u | 34.5% [32.5%, 37.0%] | 42.5% [39.8%, 44.7%] | -4.7 |
| 99 | Share of duel time: hidden, firing | 12.0% [11.0%, 13.2%] | 15.8% [14.9%, 17.0%] | -4.6 |
| 100 | Median aim error at -100 ms from the first visible part | 11.54° [10.20°, 13.09°] | 8.00° [7.34°, 8.49°] | +4.6 |
| 101 | Attack holds that are taps (<=100 ms) | 36.9% [35.2%, 39.2%] | 44.3% [41.8%, 46.5%] | -4.6 |
| 102 | Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) | 12.71° [10.54°, 15.64°] | 5.78° [4.88°, 6.98°] | +4.5 |
| 103 | Median aim error firing with LOS at (192, 256] u | 7.99° [7.60°, 8.34°] | 6.68° [6.36°, 7.10°] | +4.5 |
| 104 | Time from both alive to the first body part on screen p50 | 1900 ms [1700 ms, 2100 ms] | 1350 ms [1300 ms, 1400 ms] | +4.5 |
| 105 | Retreating >40 u/s in LOS firefight at distance (512, 768] | 9.8% [7.1%, 11.5%] | 4.0% [3.0%, 5.4%] | +4.5 |
| 106 | Crosshair within 5 deg of the corner, 500 ms before the first part | 20.7% [18.4%, 22.9%] | 29.2% [26.4%, 32.7%] | -4.4 |
| 107 | Approaching >40 u/s in LOS firefight at distance (384, 512] | 19.7% [15.4%, 25.3%] | 41.8% [33.8%, 49.1%] | -4.4 |
| 108 | Sightings (a body part coming on screen) per minute of duel time | 27.82/min [26.19/min, 29.88/min] | 33.29/min [31.94/min, 34.72/min] | -4.4 |
| 109 | View within 30 deg of the hidden opponent, last seen 2-4 s | 49.3% [43.2%, 55.8%] | 64.3% [61.3%, 67.3%] | -4.3 |
| 110 | Key chord back any (LOS firefight) | 7.7% [7.2%, 8.4%] | 5.2% [4.5%, 6.1%] | +4.3 |
| 111 | Retreating >40 u/s in LOS firefight at distance (384, 512] | 8.6% [7.5%, 10.4%] | 4.8% [3.8%, 6.0%] | +4.2 |
| 112 | SMG shots that hit, centroid visible | 22.1% [20.3%, 24.1%] | 27.2% [25.8%, 28.5%] | -4.2 |
| 113 | Shot accuracy (eligible SMG shots that hit) | 15.3% [13.9%, 17.0%] | 19.2% [18.1%, 20.3%] | -4.1 |
| 114 | Part sightings whose visible bout ends without a hit | 35.2% [32.2%, 38.2%] | 27.5% [25.7%, 29.7%] | +4.1 |
| 115 | Crosshair on body firing with LOS at (0, 128] u | 39.9% [38.2%, 41.7%] | 48.0% [44.9%, 51.5%] | -4.1 |
| 116 | Yaw speed (LOS firefight) p50 | 52°/s [46°/s, 57°/s] | 39°/s [36°/s, 41°/s] | +4.1 |
| 117 | Crosshair on body firing with LOS at (256, 384] u | 32.4% [30.3%, 34.2%] | 39.0% [36.0%, 41.4%] | -4.0 |
| 118 | Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part | 11.63° [10.14°, 13.13°] | 15.18° [14.24°, 16.01°] | -4.0 |
| 119 | Fire held, LOS, aim error in half-widths (6, 10] | 35.8% [31.9%, 40.4%] | 22.4% [17.1%, 27.1%] | +4.0 |
| 120 | Key chord pure strafe (LOS, not firing) | 42.2% [40.5%, 44.2%] | 26.7% [20.8%, 34.2%] | +4.0 |
| 121 | Key chord pure strafe (LOS firefight) | 69.1% [65.9%, 72.5%] | 58.0% [53.1%, 62.2%] | +3.8 |
| 122 | View within 30 deg of the hidden opponent, last seen 250-500 ms | 88.3% [86.6%, 90.2%] | 92.2% [91.3%, 93.4%] | -3.8 |
| 123 | Press hazard with LOS and aim within 3 half-widths, LOS gained 550-1000 ms ago | 34.1% [29.6%, 40.6%] | 21.0% [16.7%, 24.6%] | +3.8 |
| 124 | Median aim error at +200 ms from the first visible part | 7.16° [6.12°, 8.02°] | 5.26° [4.86°, 5.58°] | +3.8 |
| 125 | Share of duel time in the busiest 10% of 32u cells (dm/downladder) | 37.9% [32.7%, 38.9%] | 61.4% [42.9%, 63.8%] | -3.8 |
| 126 | Key chord fwd diag (LOS, not firing) | 13.7% [12.1%, 15.1%] | 29.4% [23.0%, 38.1%] | -3.7 |
| 127 | Crosshair within half-width+1.5 deg after a clean sighting p50 | 300 ms [300 ms, 350 ms] | 200 ms [200 ms, 250 ms] | +3.7 |
| 128 | Fire held with no body part visible, 0-500ms after one was last on screen | 49.5% [47.4%, 51.9%] | 43.2% [40.9%, 45.8%] | +3.7 |
| 129 | Fire held at -500 ms from sight gain | 23.1% [21.3%, 24.8%] | 18.2% [16.5%, 20.2%] | +3.7 |
| 130 | Fire held at +1000 ms from sight gain | 57.1% [55.7%, 58.6%] | 63.4% [60.9%, 66.4%] | -3.7 |
| 131 | Key chord neutral (LOS firefight) | 9.4% [7.3%, 11.2%] | 5.1% [3.9%, 6.3%] | +3.6 |
| 132 | Engagements that end in a kill | 85.5% [83.3%, 87.7%] | 79.9% [77.9%, 81.9%] | +3.5 |
| 133 | Walk presses per minute | 5.94/min [5.34/min, 6.55/min] | 7.54/min [6.90/min, 8.11/min] | -3.5 |
| 134 | Release hazard, LOS, aim error in half-widths (1, 2] | 2.4% [2.0%, 2.8%] | 1.5% [1.3%, 1.9%] | +3.4 |
| 135 | Key chord back any (hidden, not firing) | 7.7% [7.5%, 7.9%] | 6.1% [5.3%, 7.1%] | +3.4 |
| 136 | Gap between attack holds p50 | 300 ms [250 ms, 300 ms] | 400 ms [350 ms, 400 ms] | -3.3 |
| 137 | Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago | 37.9% [33.9%, 43.5%] | 28.0% [25.2%, 32.0%] | +3.3 |
| 138 | View within 30 deg of the hidden opponent, last seen 0.5-1 s | 83.2% [80.9%, 85.5%] | 88.0% [86.8%, 89.9%] | -3.3 |
| 139 | Fire held without LOS | 19.2% [16.8%, 22.2%] | 24.5% [22.7%, 26.3%] | -3.3 |
| 140 | Yaw error to the hidden opponent, last seen 2-4 s | 30.99° [23.55°, 39.89°] | 16.93° [15.18°, 19.08°] | +3.2 |
| 141 | Release hazard, LOS, aim error in half-widths (6, 10] | 14.2% [10.9%, 17.3%] | 6.9% [4.4%, 9.9%] | +3.2 |
| 142 | Release hazard while hidden, LOS lost 1 s or more ago | 14.5% [14.1%, 14.9%] | 11.3% [9.8%, 13.5%] | +3.2 |
| 143 | First hit after the first part p50 | 350 ms [300 ms, 350 ms] | 250 ms [200 ms, 250 ms] | +3.2 |
| 144 | Median aim error at +500 ms from sight gain | 8.05° [6.58°, 9.64°] | 5.47° [5.08°, 5.88°] | +3.2 |
| 145 | Reaction: first trigger press after a clean sighting p75 | 400 ms [400 ms, 450 ms] | 300 ms [250 ms, 300 ms] | +3.1 |
| 146 | Reloads with the opponent alive that are forced (empty clip) | 89.5% [87.4%, 91.8%] | 77.7% [69.5%, 83.9%] | +2.9 |
| 147 | Release hazard, LOS, aim error in half-widths (0, 1] | 2.3% [1.8%, 2.9%] | 1.4% [1.1%, 1.7%] | +2.8 |
| 148 | Crosshair on body firing with LOS at (384, 512] u | 30.3% [28.4%, 32.2%] | 35.4% [32.6%, 38.3%] | -2.8 |
| 149 | Key chord forward (LOS firefight) | 1.9% [1.4%, 2.4%] | 1.2% [1.0%, 1.4%] | +2.6 |
| 150 | Standing still (<5 u/s), all duel time | 12.1% [10.8%, 13.4%] | 15.7% [13.4%, 17.7%] | -2.5 |
| 151 | Key chord forward (LOS, not firing) | 10.3% [9.6%, 11.3%] | 7.5% [5.9%, 9.5%] | +2.5 |
| 152 | Share of duel time: LOS, not firing | 9.2% [8.4%, 10.0%] | 7.0% [5.5%, 8.4%] | +2.5 |
| 153 | Mouse still between ticks (hidden, not firing) | 27.9% [27.5%, 28.4%] | 32.1% [28.8%, 36.0%] | -2.5 |
| 154 | Standing still (<5 u/s) (LOS, not firing) | 8.7% [7.3%, 9.8%] | 29.6% [12.6%, 44.0%] | -2.4 |
| 155 | Mouse still between ticks (LOS, not firing) | 18.1% [17.3%, 19.0%] | 36.7% [20.8%, 49.8%] | -2.4 |
| 156 | View within 30 deg of the hidden opponent, last seen 1-2 s | 74.4% [71.3%, 76.8%] | 78.9% [77.1%, 81.1%] | -2.3 |

## Per bot

### `bot:strafer:4` (strafer, seed 231333596, 25.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.08239 | +3% |
| reverse_share | 0.4467 | 0.4279 | -3% |
| side_hold_ms | 326.3 | 250 | -51% ⚠ |
| lean_fight | 0.4941 | 0.5706 | +15% |
| jumps_per_min | 2.379 | 1.974 | -8% |
| crouch_per_min | 9.476 | 9.514 | +0% |
| walk_hidden | 0 | 0.04283 | +41% ⚠ |
| burst_median | 8 | 7 | -25% ⚠ |
| aim_height_firing | 0.4857 | 0.472 | -7% |
| hold_angle | 0.2514 | 0.3706 | +39% ⚠ |
| aim_error_fight_deg | 4.667 | 4.205 | -19% |
| reaction_ms | 140.8 | 250 | +109% ⚠ |
| mp40_share | 0.9062 | 1 | +9% |

Style fingerprint (2026-10-02, 25.3 min, styles.py features 30/30): z-distance to the human cloud centre 8.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 6.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.132 (humans 0.152–0.491); lean_any 0.341 (humans 0.358–0.767); ducked 0.0781 (humans 0.0109–0.0741); still 0.0708 (humans 0.0832–0.227); yaw_speed_med 30.5 (humans 12.6–30); yaw_speed_p95 355 (humans 158–271); side_hold_p90_ms 600 (humans 700–950); lf_retreat 0.167 (humans 0.0243–0.141).

### `bot:presser:5` (presser, seed 1152489726, 25.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4832 | 0.4558 | -5% |
| reverse_share | 0.8051 | 0.8049 | -0% |
| side_hold_ms | 295.2 | 200 | -63% ⚠ |
| lean_fight | 0.9204 | 0.9681 | +10% |
| jumps_per_min | 0.4317 | 0.2763 | -3% |
| crouch_per_min | 3.157 | 3.276 | +1% |
| walk_hidden | 0.02816 | 0.05867 | +29% ⚠ |
| burst_median | 6.498 | 7 | +13% |
| aim_height_firing | 0.417 | 0.39 | -14% |
| hold_angle | 0.2275 | 0.3136 | +28% ⚠ |
| aim_error_fight_deg | 4.893 | 4.969 | +3% |
| reaction_ms | 152.5 | 250 | +98% ⚠ |
| mp40_share | 0.1123 | 1 | +89% ⚠ |

Style fingerprint (2026-10-02, 25.3 min, styles.py features 30/30): z-distance to the human cloud centre 8.3 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.7 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lf_back_any 0.0193 (humans 0.0205–0.123); lean_any 0.839 (humans 0.358–0.767); lf_lean 0.968 (humans 0.457–0.948); yaw_speed_med 30.1 (humans 12.6–30); yaw_speed_p95 369 (humans 158–271); yaw_speed_p99 768 (humans 427–754); side_hold_p90_ms 450 (humans 700–950).

### `bot:stopper:4` (stopper, seed 711847621, 26.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.07034 | 0.07628 | +1% |
| reverse_share | 0.1382 | 0.108 | -4% |
| side_hold_ms | 249.3 | 200 | -33% ⚠ |
| lean_fight | 0.9549 | 0.9603 | +1% |
| jumps_per_min | 0.3303 | 0.187 | -3% |
| crouch_per_min | 5.571 | 5.873 | +3% |
| walk_hidden | 0 | 0.04404 | +42% ⚠ |
| burst_median | 4.564 | 4 | -14% |
| aim_height_firing | 0.4682 | 0.456 | -6% |
| hold_angle | 0.4674 | 0.2655 | -66% ⚠ |
| aim_error_fight_deg | 6.249 | 5.19 | -43% ⚠ |
| reaction_ms | 153.7 | 250 | +96% ⚠ |
| mp40_share | 0.02524 | 0 | -3% |

Style fingerprint (2026-10-02, 26.7 min, styles.py features 30/30): z-distance to the human cloud centre 8.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0819 (humans 0.152–0.491); neutral 0.325 (humans 0.0872–0.296); lf_neutral 0.197 (humans 0.00436–0.112); lean_any 0.857 (humans 0.358–0.767); lf_lean 0.96 (humans 0.457–0.948); p_attack_given_los 0.522 (humans 0.547–0.861); speed_med 132 (humans 136–200); yaw_speed_p95 327 (humans 158–271); side_hold_p90_ms 550 (humans 700–950); reverse_share 0.108 (humans 0.138–0.839).

### `bot:stopper:5` (stopper, seed 1529931226, 26.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1013 | 0.1014 | +0% |
| reverse_share | 0.3253 | 0.2802 | -6% |
| side_hold_ms | 247.2 | 200 | -31% ⚠ |
| lean_fight | 0.8427 | 0.8682 | +5% |
| jumps_per_min | 5.122 | 5.275 | +3% |
| crouch_per_min | 11.7 | 13.51 | +17% |
| walk_hidden | 0.07078 | 0.07934 | +8% |
| burst_median | 4.132 | 5 | +22% |
| aim_height_firing | 0.4597 | 0.435 | -13% |
| hold_angle | 0.5274 | 0.3667 | -52% ⚠ |
| aim_error_fight_deg | 4.835 | 4.322 | -21% |
| reaction_ms | 109.9 | 200 | +90% ⚠ |
| mp40_share | 0.06948 | 0 | -7% |

Style fingerprint (2026-10-02, 26.7 min, styles.py features 30/30): z-distance to the human cloud centre 8.6 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.109 (humans 0.152–0.491); lf_neutral 0.15 (humans 0.00436–0.112); ducked 0.135 (humans 0.0109–0.0741); speed_med 122 (humans 136–200); yaw_speed_p95 283 (humans 158–271); side_hold_p90_ms 550 (humans 700–950).

### `bot:strafer:4.2` (strafer, seed 1086809828, 28.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1632 | 0.07779 | -17% |
| reverse_share | 0.4593 | 0.4037 | -8% |
| side_hold_ms | 314.9 | 200 | -77% ⚠ |
| lean_fight | 0.5599 | 0.584 | +5% |
| jumps_per_min | 4.813 | 4.7 | -2% |
| crouch_per_min | 9.497 | 8.724 | -7% |
| walk_hidden | 0 | 0.02814 | +27% ⚠ |
| burst_median | 5.59 | 5 | -15% |
| aim_height_firing | 0.4741 | 0.471 | -2% |
| hold_angle | 0.3025 | 0.1933 | -36% ⚠ |
| aim_error_fight_deg | 6.527 | 11.59 | +206% ⚠ |
| reaction_ms | 158.4 | 200 | +42% ⚠ |
| mp40_share | 0.9804 | 1 | +2% |

Style fingerprint (2026-10-02, 28.1 min, styles.py features 30/30): z-distance to the human cloud centre 13.0 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 11.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.123 (humans 0.152–0.491); lean_any 0.318 (humans 0.358–0.767); ducked 0.106 (humans 0.0109–0.0741); attack 0.247 (humans 0.276–0.478); speed_med 133 (humans 136–200); lf_err_med 11.6 (humans 4.08–6.63); yaw_speed_med 36.7 (humans 12.6–30); yaw_speed_p95 421 (humans 158–271); yaw_speed_p99 822 (humans 427–754); side_hold_p90_ms 550 (humans 700–950); lf_retreat 0.204 (humans 0.0243–0.141).

### `bot:strafer:5` (strafer, seed 821368291, 28.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.166 | 0.07709 | -17% |
| reverse_share | 0.4801 | 0.4253 | -8% |
| side_hold_ms | 349.1 | 250 | -66% ⚠ |
| lean_fight | 0.6654 | 0.7067 | +8% |
| jumps_per_min | 5.052 | 4.558 | -9% |
| crouch_per_min | 10.37 | 9.365 | -9% |
| walk_hidden | 0.0009382 | 0.02146 | +19% |
| burst_median | 5.755 | 6 | +6% |
| aim_height_firing | 0.4434 | 0.429 | -8% |
| hold_angle | 0.3191 | 0.2143 | -34% ⚠ |
| aim_error_fight_deg | 4.261 | 9.752 | +224% ⚠ |
| reaction_ms | 175 | 250 | +75% ⚠ |
| mp40_share | 0.1202 | 0 | -12% |

Style fingerprint (2026-10-02, 28.1 min, styles.py features 30/30): z-distance to the human cloud centre 12.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.142 (humans 0.152–0.491); ducked 0.0959 (humans 0.0109–0.0741); attack 0.245 (humans 0.276–0.478); p_attack_given_los 0.533 (humans 0.547–0.861); still 0.0724 (humans 0.0832–0.227); lf_err_med 9.76 (humans 4.08–6.63); yaw_speed_med 36.8 (humans 12.6–30); yaw_speed_p95 413 (humans 158–271); yaw_speed_p99 801 (humans 427–754); side_hold_p90_ms 500 (humans 700–950); lf_retreat 0.227 (humans 0.0243–0.141).

### `bot:stopper:4.2` (stopper, seed 97502759, 28.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.05694 | -2% |
| reverse_share | 0.6408 | 0.5696 | -10% |
| side_hold_ms | 200 | 200 | +0% |
| lean_fight | 0.8757 | 0.8825 | +1% |
| jumps_per_min | 1.606 | 1.732 | +2% |
| crouch_per_min | 2.514 | 1.874 | -6% |
| walk_hidden | 0.09232 | 0.1078 | +15% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4804 | 0.483 | +1% |
| hold_angle | 0.418 | 0.2077 | -69% ⚠ |
| aim_error_fight_deg | 5.509 | 9.677 | +170% ⚠ |
| reaction_ms | 127.6 | 200 | +72% ⚠ |
| mp40_share | 0.1179 | 0 | -12% |

Style fingerprint (2026-10-02, 28.3 min, styles.py features 30/30): z-distance to the human cloud centre 10.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.103 (humans 0.152–0.491); neutral 0.301 (humans 0.0872–0.296); lf_fwd_diag 0.0569 (humans 0.0658–0.568); attack 0.229 (humans 0.276–0.478); speed_med 119 (humans 136–200); lf_err_med 9.68 (humans 4.08–6.63); yaw_speed_p95 380 (humans 158–271); yaw_speed_p99 756 (humans 427–754); side_hold_p90_ms 500 (humans 700–950); lf_retreat 0.181 (humans 0.0243–0.141).

### `bot:strafer:5.2` (strafer, seed 2592015535, 28.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.119 | 0.05168 | -13% |
| reverse_share | 0.4951 | 0.4245 | -10% |
| side_hold_ms | 295 | 200 | -63% ⚠ |
| lean_fight | 0.4565 | 0.4864 | +6% |
| jumps_per_min | 5.657 | 5.409 | -5% |
| crouch_per_min | 11.59 | 10.82 | -7% |
| walk_hidden | 0 | 0.02174 | +21% |
| burst_median | 4.178 | 5 | +21% |
| aim_height_firing | 0.54 | 0.537 | -2% |
| hold_angle | 0.3001 | 0.2704 | -10% |
| aim_error_fight_deg | 6.024 | 9.549 | +144% ⚠ |
| reaction_ms | 165.9 | 275 | +109% ⚠ |
| mp40_share | 0.4858 | 1 | +51% ⚠ |

Style fingerprint (2026-10-02, 28.3 min, styles.py features 30/30): z-distance to the human cloud centre 12.3 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.108 (humans 0.152–0.491); lf_fwd_diag 0.0517 (humans 0.0658–0.568); lean_any 0.222 (humans 0.358–0.767); ducked 0.121 (humans 0.0109–0.0741); attack 0.214 (humans 0.276–0.478); p_attack_given_los 0.504 (humans 0.547–0.861); speed_med 121 (humans 136–200); lf_err_med 9.55 (humans 4.08–6.63); yaw_speed_med 31.9 (humans 12.6–30); yaw_speed_p95 396 (humans 158–271); yaw_speed_p99 783 (humans 427–754); side_hold_p90_ms 550 (humans 700–950); lf_retreat 0.194 (humans 0.0243–0.141).

### `bot:strafer:4.3` (strafer, seed 3498264623, 24.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1522 | 0.07919 | -14% |
| reverse_share | 0.4724 | 0.4548 | -3% |
| side_hold_ms | 293.3 | 200 | -62% ⚠ |
| lean_fight | 0.6201 | 0.6261 | +1% |
| jumps_per_min | 3.829 | 4.362 | +10% |
| crouch_per_min | 9.64 | 9.622 | -0% |
| walk_hidden | 0 | 0.04376 | +41% ⚠ |
| burst_median | 4.228 | 4 | -6% |
| aim_height_firing | 0.4919 | 0.487 | -3% |
| hold_angle | 0.3757 | 0.2 | -57% ⚠ |
| aim_error_fight_deg | 5.803 | 7.635 | +75% ⚠ |
| reaction_ms | 133.2 | 250 | +117% ⚠ |
| mp40_share | 0.5841 | 1 | +42% ⚠ |

Style fingerprint (2026-10-02, 24.5 min, styles.py features 30/30): z-distance to the human cloud centre 11.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.116 (humans 0.152–0.491); ducked 0.0907 (humans 0.0109–0.0741); attack 0.268 (humans 0.276–0.478); speed_med 135 (humans 136–200); lf_err_med 7.63 (humans 4.08–6.63); yaw_speed_med 42.6 (humans 12.6–30); yaw_speed_p95 406 (humans 158–271); yaw_speed_p99 790 (humans 427–754); side_hold_p90_ms 550 (humans 700–950); lf_retreat 0.173 (humans 0.0243–0.141).

### `bot:stopper:5.2` (stopper, seed 2513918026, 24.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1336 | 0.09868 | -7% |
| reverse_share | 0.6777 | 0.6504 | -4% |
| side_hold_ms | 226 | 200 | -17% |
| lean_fight | 0.9588 | 0.9709 | +2% |
| jumps_per_min | 2.944 | 3.588 | +12% |
| crouch_per_min | 3.426 | 3.302 | -1% |
| walk_hidden | 0.08429 | 0.08755 | +3% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4602 | 0.44 | -11% |
| hold_angle | 0.3824 | 0.2431 | -45% ⚠ |
| aim_error_fight_deg | 4.291 | 6.973 | +109% ⚠ |
| reaction_ms | 113 | 200 | +87% ⚠ |
| mp40_share | 0.07877 | 0 | -8% |

Style fingerprint (2026-10-02, 24.5 min, styles.py features 30/30): z-distance to the human cloud centre 10.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.119 (humans 0.152–0.491); lean_any 0.868 (humans 0.358–0.767); lf_lean 0.971 (humans 0.457–0.948); speed_med 129 (humans 136–200); lf_err_med 6.97 (humans 4.08–6.63); yaw_speed_med 34.9 (humans 12.6–30); yaw_speed_p95 406 (humans 158–271); yaw_speed_p99 826 (humans 427–754); side_hold_p90_ms 550 (humans 700–950); lf_retreat 0.173 (humans 0.0243–0.141).

### `bot:stopper:4.3` (stopper, seed 2458011590, 25.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1182 | 0.07179 | -9% |
| reverse_share | 0.4266 | 0.3674 | -8% |
| side_hold_ms | 200 | 200 | +0% |
| lean_fight | 0.8817 | 0.899 | +3% |
| jumps_per_min | 2.965 | 3.191 | +4% |
| crouch_per_min | 8.679 | 8.484 | -2% |
| walk_hidden | 0.1056 | 0.11 | +4% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4719 | 0.452 | -11% |
| hold_angle | 0.3772 | 0.2182 | -52% ⚠ |
| aim_error_fight_deg | 4.42 | 7.487 | +125% ⚠ |
| reaction_ms | 139.9 | 200 | +60% ⚠ |
| mp40_share | 0.02794 | 0 | -3% |

Style fingerprint (2026-10-02, 25.7 min, styles.py features 30/30): z-distance to the human cloud centre 9.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.7 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0977 (humans 0.152–0.491); neutral 0.304 (humans 0.0872–0.296); lf_neutral 0.147 (humans 0.00436–0.112); ducked 0.084 (humans 0.0109–0.0741); speed_med 121 (humans 136–200); lf_err_med 7.49 (humans 4.08–6.63); yaw_speed_med 33.3 (humans 12.6–30); yaw_speed_p95 374 (humans 158–271); side_hold_p90_ms 600 (humans 700–950); lf_retreat 0.159 (humans 0.0243–0.141).

### `bot:strafer:5.3` (strafer, seed 4003987225, 25.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.07299 | +1% |
| reverse_share | 0.4657 | 0.4329 | -5% |
| side_hold_ms | 270.9 | 250 | -14% |
| lean_fight | 0.5402 | 0.5816 | +8% |
| jumps_per_min | 4.852 | 5.332 | +9% |
| crouch_per_min | 3.183 | 3.269 | +1% |
| walk_hidden | 0.03847 | 0.05562 | +16% |
| burst_median | 5.686 | 5 | -17% |
| aim_height_firing | 0.4588 | 0.441 | -9% |
| hold_angle | 0.311 | 0.1872 | -40% ⚠ |
| aim_error_fight_deg | 5.446 | 8.304 | +116% ⚠ |
| reaction_ms | 161.3 | 275 | +114% ⚠ |
| mp40_share | 0 | 0 | +0% |

Style fingerprint (2026-10-02, 25.7 min, styles.py features 30/30): z-distance to the human cloud centre 11.3 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.119 (humans 0.152–0.491); lean_any 0.338 (humans 0.358–0.767); p_attack_given_los 0.541 (humans 0.547–0.861); lf_err_med 8.3 (humans 4.08–6.63); yaw_speed_med 40.1 (humans 12.6–30); yaw_speed_p95 404 (humans 158–271); yaw_speed_p99 792 (humans 427–754); side_hold_p90_ms 600 (humans 700–950); lf_retreat 0.22 (humans 0.0243–0.141).

### `bot:presser:4` (presser, seed 588158740, 27.4 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4812 | 0.2032 | -54% ⚠ |
| reverse_share | 0.8389 | 0.7977 | -6% |
| side_hold_ms | 302.9 | 150 | -102% ⚠ |
| lean_fight | 0.9588 | 0.9752 | +3% |
| jumps_per_min | 1.077 | 0.8386 | -4% |
| crouch_per_min | 1.47 | 0.8022 | -6% |
| walk_hidden | 0.004612 | 0.03869 | +32% ⚠ |
| burst_median | 7.045 | 7 | -1% |
| aim_height_firing | 0.352 | 0.29 | -33% ⚠ |
| hold_angle | 0.2402 | 0.3 | +19% |
| aim_error_fight_deg | 4.382 | 5.524 | +46% ⚠ |
| reaction_ms | 183.1 | 250 | +67% ⚠ |
| mp40_share | 0.9584 | 1 | +4% |

Style fingerprint (2026-10-02, 27.4 min, styles.py features 30/30): z-distance to the human cloud centre 10.3 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.2 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.873 (humans 0.358–0.767); lf_lean 0.975 (humans 0.457–0.948); p_attack_given_los 0.516 (humans 0.547–0.861); speed_med 128 (humans 136–200); lf_height_frac 0.29 (humans 0.352–0.54); yaw_speed_med 30.8 (humans 12.6–30); yaw_speed_p95 410 (humans 158–271); yaw_speed_p99 869 (humans 427–754); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 450 (humans 700–950).

### `bot:presser:5.2` (presser, seed 4131749178, 27.4 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5433 | 0.1811 | -71% ⚠ |
| reverse_share | 0.823 | 0.7758 | -7% |
| side_hold_ms | 298.6 | 150 | -99% ⚠ |
| lean_fight | 0.9579 | 0.9769 | +4% |
| jumps_per_min | 0.5292 | 0.5469 | +0% |
| crouch_per_min | 2.785 | 2.188 | -6% |
| walk_hidden | 0.00312 | 0.03488 | +30% ⚠ |
| burst_median | 7.004 | 8 | +25% |
| aim_height_firing | 0.4126 | 0.369 | -23% |
| hold_angle | 0.2204 | 0.3143 | +31% ⚠ |
| aim_error_fight_deg | 4.873 | 5.556 | +28% ⚠ |
| reaction_ms | 134.4 | 250 | +116% ⚠ |
| mp40_share | 0.08844 | 0 | -9% |

Style fingerprint (2026-10-02, 27.4 min, styles.py features 30/30): z-distance to the human cloud centre 10.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.9 (humans 0.358–0.767); lf_lean 0.977 (humans 0.457–0.948); speed_med 127 (humans 136–200); yaw_speed_med 31.9 (humans 12.6–30); yaw_speed_p95 408 (humans 158–271); yaw_speed_p99 930 (humans 427–754); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 445 (humans 700–950).

### `bot:presser:4.2` (presser, seed 52844662, 27.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5317 | 0.2097 | -63% ⚠ |
| reverse_share | 0.8152 | 0.7734 | -6% |
| side_hold_ms | 336.6 | 200 | -91% ⚠ |
| lean_fight | 0.935 | 0.9756 | +8% |
| jumps_per_min | 0.336 | 0.4811 | +3% |
| crouch_per_min | 1.298 | 1.184 | -1% |
| walk_hidden | 0 | 0.03533 | +33% ⚠ |
| burst_median | 7.121 | 6 | -28% ⚠ |
| aim_height_firing | 0.3853 | 0.333 | -28% ⚠ |
| hold_angle | 0.2402 | 0.2667 | +9% |
| aim_error_fight_deg | 4.424 | 4.875 | +18% |
| reaction_ms | 173.4 | 250 | +77% ⚠ |
| mp40_share | 0.06486 | 0 | -6% |

Style fingerprint (2026-10-02, 27.0 min, styles.py features 30/30): z-distance to the human cloud centre 9.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.497 (humans 0.191–0.489); lean_any 0.883 (humans 0.358–0.767); lf_lean 0.976 (humans 0.457–0.948); ducked 0.0101 (humans 0.0109–0.0741); still 0.0743 (humans 0.0832–0.227); lf_height_frac 0.333 (humans 0.352–0.54); yaw_speed_p95 390 (humans 158–271); yaw_speed_p99 808 (humans 427–754); side_hold_p90_ms 450 (humans 700–950).

### `bot:strafer:5.4` (strafer, seed 3859071546, 27.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1614 | 0.07002 | -18% |
| reverse_share | 0.5449 | 0.4781 | -10% |
| side_hold_ms | 350 | 200 | -100% ⚠ |
| lean_fight | 0.5666 | 0.7011 | +27% ⚠ |
| jumps_per_min | 0.6862 | 0.4811 | -4% |
| crouch_per_min | 3.8 | 3.22 | -5% |
| walk_hidden | 0.04377 | 0.06552 | +21% |
| burst_median | 7.775 | 7 | -19% |
| aim_height_firing | 0.4661 | 0.449 | -9% |
| hold_angle | 0.2527 | 0.4257 | +56% ⚠ |
| aim_error_fight_deg | 4.952 | 4.784 | -7% |
| reaction_ms | 115 | 200 | +85% ⚠ |
| mp40_share | 0.03683 | 0 | -4% |

Style fingerprint (2026-10-02, 27.0 min, styles.py features 30/30): z-distance to the human cloud centre 7.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 6.6 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.494 (humans 0.191–0.489); fwd_diag 0.115 (humans 0.152–0.491); lf_pure_strafe 0.762 (humans 0.362–0.752); speed_med 135 (humans 136–200); yaw_speed_p95 315 (humans 158–271); side_hold_p90_ms 500 (humans 700–950); lf_approach 0.126 (humans 0.131–0.64); lf_retreat 0.145 (humans 0.0243–0.141).

## The owner against the bots (`pstN@vsbot`)

Kills of bots 0, deaths to bots 0, suicides 0. 

0 statistics of the owner against bots fall outside the human CI (owner column below, marked •).

## All statistics

Pooled bots are reweighted to the human family mix; humans and the owner are pooled over their duel time. ▲ marks a tell. n = units (ticks, runs, shots, sightings, ...) over the blocks.

### Movement

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time: hidden, not firing | 50.9% [47.0%, 54.7%] | 49.1% [47.4%, 51.2%] | - | 511440 |
| Share of duel time: hidden, firing ▲ | 12.0% [11.0%, 13.2%] | 15.8% [14.9%, 17.0%] | - | 511440 |
| Share of duel time: LOS, not firing ▲ | 9.2% [8.4%, 10.0%] | 7.0% [5.5%, 8.4%] | - | 511440 |
| Share of duel time: LOS firefight | 18.0% [16.4%, 19.5%] | 17.6% [16.5%, 18.9%] | - | 511440 |
| Share of duel time: reloading | 9.9% [8.3%, 11.8%] | 10.5% [9.6%, 11.5%] | - | 511440 |
| Key chord pure strafe (hidden, not firing) | 30.3% [28.8%, 32.6%] | 26.0% [23.5%, 29.4%] | - | 258200 |
| Key chord fwd diag (hidden, not firing) ▲ | 13.2% [12.6%, 13.9%] | 29.9% [26.7%, 33.2%] | - | 258200 |
| Key chord forward (hidden, not firing) ▲ | 18.8% [17.7%, 19.6%] | 11.9% [10.9%, 12.8%] | - | 258200 |
| Key chord neutral (hidden, not firing) | 30.0% [27.8%, 31.8%] | 26.1% [23.1%, 28.6%] | - | 258200 |
| Key chord back any (hidden, not firing) ▲ | 7.7% [7.5%, 7.9%] | 6.1% [5.3%, 7.1%] | - | 258200 |
| Key chord pure strafe (hidden, firing) | 49.7% [46.4%, 53.9%] | 44.9% [41.6%, 48.0%] | - | 62007 |
| Key chord fwd diag (hidden, firing) ▲ | 14.8% [13.3%, 16.3%] | 28.6% [23.9%, 32.8%] | - | 62007 |
| Key chord forward (hidden, firing) ▲ | 12.0% [9.9%, 13.8%] | 6.6% [5.7%, 7.4%] | - | 62007 |
| Key chord neutral (hidden, firing) | 17.3% [14.4%, 19.6%] | 13.2% [11.2%, 15.7%] | - | 62007 |
| Key chord back any (hidden, firing) | 6.2% [5.8%, 6.8%] | 6.7% [5.9%, 7.9%] | - | 62007 |
| Key chord pure strafe (LOS, not firing) ▲ | 42.2% [40.5%, 44.2%] | 26.7% [20.8%, 34.2%] | - | 46875 |
| Key chord fwd diag (LOS, not firing) ▲ | 13.7% [12.1%, 15.1%] | 29.4% [23.0%, 38.1%] | - | 46875 |
| Key chord forward (LOS, not firing) ▲ | 10.3% [9.6%, 11.3%] | 7.5% [5.9%, 9.5%] | - | 46875 |
| Key chord neutral (LOS, not firing) | 23.5% [20.9%, 25.6%] | 31.5% [15.3%, 45.4%] | - | 46875 |
| Key chord back any (LOS, not firing) ▲ | 10.2% [9.2%, 11.0%] | 4.8% [3.4%, 6.7%] | - | 46875 |
| Key chord pure strafe (LOS firefight) ▲ | 69.1% [65.9%, 72.5%] | 58.0% [53.1%, 62.2%] | - | 91643 |
| Key chord fwd diag (LOS firefight) ▲ | 11.9% [9.6%, 14.8%] | 30.5% [25.6%, 36.7%] | - | 91643 |
| Key chord forward (LOS firefight) ▲ | 1.9% [1.4%, 2.4%] | 1.2% [1.0%, 1.4%] | - | 91643 |
| Key chord neutral (LOS firefight) ▲ | 9.4% [7.3%, 11.2%] | 5.1% [3.9%, 6.3%] | - | 91643 |
| Key chord back any (LOS firefight) ▲ | 7.7% [7.2%, 8.4%] | 5.2% [4.5%, 6.1%] | - | 91643 |
| Key chord pure strafe (reloading) | 25.3% [22.6%, 28.9%] | 22.3% [19.6%, 24.4%] | - | 52718 |
| Key chord fwd diag (reloading) ▲ | 20.6% [18.1%, 23.4%] | 36.8% [34.1%, 39.9%] | - | 52718 |
| Key chord forward (reloading) | 21.2% [17.3%, 24.3%] | 26.1% [23.8%, 28.6%] | - | 52718 |
| Key chord neutral (reloading) ▲ | 22.3% [18.1%, 25.5%] | 8.3% [7.3%, 9.5%] | - | 52718 |
| Key chord back any (reloading) ▲ | 10.6% [9.8%, 11.8%] | 6.5% [5.3%, 7.6%] | - | 52718 |
| Strafe end is a direct reverse (hidden, not firing) | 44.3% [39.9%, 49.0%] | 46.1% [41.1%, 51.2%] | - | 21865 |
| Strafe end is a direct reverse (hidden, firing) | 62.1% [53.7%, 70.4%] | 67.2% [59.7%, 74.3%] | - | 8392 |
| Strafe end is a direct reverse (LOS, not firing) | 48.4% [40.8%, 55.4%] | 52.8% [45.1%, 60.4%] | - | 5380 |
| Strafe end is a direct reverse (LOS firefight) | 60.5% [50.1%, 71.8%] | 73.3% [65.9%, 80.6%] | - | 16177 |
| Strafe end is a direct reverse (reloading) | 46.0% [39.4%, 52.2%] | 37.7% [32.5%, 42.7%] | - | 5597 |
| Strafe switch hazard per tick at hold age 50 ms (LOS firefight) ▲ | 10.8% [9.2%, 12.5%] | 1.2% [0.9%, 1.6%] | - | 17324 |
| Strafe switch hazard per tick at hold age 100 ms (LOS firefight) ▲ | 18.1% [15.5%, 19.9%] | 5.8% [5.2%, 6.8%] | - | 15045 |
| Strafe switch hazard per tick at hold age 150 ms (LOS firefight) ▲ | 23.1% [19.3%, 25.3%] | 12.4% [11.1%, 13.9%] | - | 12036 |
| Strafe switch hazard per tick at hold age 200 ms (LOS firefight) ▲ | 24.2% [23.0%, 25.4%] | 17.0% [15.4%, 18.7%] | - | 9059 |
| Strafe switch hazard per tick at hold age 250 ms (LOS firefight) ▲ | 25.5% [23.2%, 27.3%] | 17.4% [16.0%, 19.0%] | - | 6712 |
| Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) ▲ | 25.4% [24.0%, 26.8%] | 15.6% [14.7%, 16.9%] | - | 13239 |
| Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) ▲ | 23.9% [22.2%, 25.8%] | 16.1% [14.9%, 17.5%] | - | 6485 |
| Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) ▲ | 25.9% [22.7%, 28.9%] | 13.7% [12.0%, 15.6%] | - | 468 |
| Side-hold duration (LOS firefight) p25 ▲ | 100 ms [100 ms, 100 ms] | 200 ms [200 ms, 200 ms] | - | 15889 |
| Side-hold duration (LOS firefight) p50 ▲ | 200 ms [200 ms, 200 ms] | 300 ms [250 ms, 300 ms] | - | 15889 |
| Side-hold duration (LOS firefight) p75 ▲ | 300 ms [300 ms, 300 ms] | 450 ms [449 ms, 450 ms] | - | 15889 |
| Side-hold duration (LOS firefight) p90 ▲ | 450 ms [450 ms, 450 ms] | 700 ms [650 ms, 700 ms] | - | 15889 |
| Side-hold duration (all contexts) p50 ▲ | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | - | 56487 |
| Side-hold duration (all contexts) p90 ▲ | 500 ms [500 ms, 550 ms] | 800 ms [800 ms, 850 ms] | - | 56487 |
| Neutral gap between strafes (LOS firefight) p50 | 50 ms [50 ms, 50 ms] | 50 ms [50 ms, 50 ms] | - | 5795 |
| Neutral gap between strafes (LOS firefight) p90 | 100 ms [50 ms, 100 ms] | 100 ms [100 ms, 150 ms] | - | 5795 |
| Approaching >40 u/s in LOS firefight at distance (0, 96] ▲ | 23.6% [21.7%, 25.5%] | 37.6% [33.7%, 42.3%] | - | 5423 |
| Approaching >40 u/s in LOS firefight at distance (96, 160] ▲ | 19.0% [16.7%, 21.0%] | 36.4% [32.6%, 41.2%] | - | 12123 |
| Approaching >40 u/s in LOS firefight at distance (160, 224] ▲ | 17.3% [15.3%, 19.1%] | 35.4% [30.1%, 42.0%] | - | 15812 |
| Approaching >40 u/s in LOS firefight at distance (224, 288] ▲ | 17.6% [15.3%, 20.6%] | 38.2% [32.3%, 46.4%] | - | 14057 |
| Approaching >40 u/s in LOS firefight at distance (288, 384] ▲ | 18.1% [14.9%, 22.4%] | 39.4% [32.7%, 47.2%] | - | 18732 |
| Approaching >40 u/s in LOS firefight at distance (384, 512] ▲ | 19.7% [15.4%, 25.3%] | 41.8% [33.8%, 49.1%] | - | 15665 |
| Approaching >40 u/s in LOS firefight at distance (512, 768] | 18.5% [14.1%, 26.0%] | 33.1% [25.5%, 41.5%] | - | 9582 |
| Approaching >40 u/s in LOS firefight at distance (768, 1200] | 20.2% [5.4%, 42.6%] | 28.2% [17.6%, 42.7%] | - | 249 |
| Retreating >40 u/s in LOS firefight at distance (0, 96] | 27.6% [24.8%, 31.2%] | 22.2% [18.9%, 25.6%] | - | 5423 |
| Retreating >40 u/s in LOS firefight at distance (96, 160] ▲ | 23.4% [21.4%, 25.8%] | 10.5% [8.5%, 12.6%] | - | 12123 |
| Retreating >40 u/s in LOS firefight at distance (160, 224] ▲ | 18.2% [16.7%, 20.1%] | 6.9% [5.5%, 8.7%] | - | 15812 |
| Retreating >40 u/s in LOS firefight at distance (224, 288] ▲ | 14.6% [13.4%, 15.6%] | 5.3% [4.2%, 6.8%] | - | 14057 |
| Retreating >40 u/s in LOS firefight at distance (288, 384] ▲ | 11.4% [10.1%, 13.2%] | 5.9% [4.6%, 7.3%] | - | 18732 |
| Retreating >40 u/s in LOS firefight at distance (384, 512] ▲ | 8.6% [7.5%, 10.4%] | 4.8% [3.8%, 6.0%] | - | 15665 |
| Retreating >40 u/s in LOS firefight at distance (512, 768] ▲ | 9.8% [7.1%, 11.5%] | 4.0% [3.0%, 5.4%] | - | 9582 |
| Retreating >40 u/s in LOS firefight at distance (768, 1200] | 19.2% [0.0%, 37.8%] | 8.3% [1.5%, 15.5%] | - | 249 |
| Standing still (<5 u/s), all duel time ▲ | 12.1% [10.8%, 13.4%] | 15.7% [13.4%, 17.7%] | - | 511440 |
| Standing still (<5 u/s) (hidden, not firing) | 18.0% [16.0%, 19.6%] | 22.5% [19.5%, 25.1%] | - | 258200 |
| Standing still (<5 u/s) (hidden, firing) | 8.3% [7.4%, 9.1%] | 10.2% [9.1%, 11.7%] | - | 62007 |
| Standing still (<5 u/s) (LOS, not firing) ▲ | 8.7% [7.3%, 9.8%] | 29.6% [12.6%, 44.0%] | - | 46875 |
| Standing still (<5 u/s) (LOS firefight) | 2.3% [2.0%, 2.6%] | 2.3% [1.8%, 2.7%] | - | 91643 |
| Standing still (<5 u/s) (reloading) | 7.7% [6.0%, 9.0%] | 5.2% [4.3%, 6.2%] | - | 52718 |
| Lean held, all duel time | 61.7% [57.9%, 66.5%] | 59.7% [55.3%, 64.0%] | - | 511440 |
| Lean held in LOS firefights | 81.5% [78.1%, 84.6%] | 81.3% [76.6%, 85.8%] | - | 91643 |
| Jump presses per minute | 2.75/min [1.90/min, 3.57/min] | 1.87/min [1.41/min, 2.44/min] | - | 511440 |
| Crouch presses per minute | 6.13/min [4.27/min, 7.80/min] | 4.33/min [3.22/min, 5.51/min] | - | 511440 |
| Walk presses per minute ▲ | 5.94/min [5.34/min, 6.55/min] | 7.54/min [6.90/min, 8.11/min] | - | 511440 |
| Walking while hidden and not firing | 5.9% [4.7%, 7.2%] | 5.9% [5.0%, 7.0%] | - | 258200 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Press hazard with LOS and aim within 3 half-widths, LOS gained 0 ms ago | 27.8% [23.5%, 32.8%] | 28.2% [25.8%, 30.8%] | - | 1618 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 50-100 ms ago | 34.8% [32.3%, 38.8%] | 33.0% [30.8%, 35.6%] | - | 2310 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 150-250 ms ago | 40.7% [37.5%, 44.7%] | 35.2% [32.9%, 38.1%] | - | 2078 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago ▲ | 37.9% [33.9%, 43.5%] | 28.0% [25.2%, 32.0%] | - | 1691 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 550-1000 ms ago ▲ | 34.1% [29.6%, 40.6%] | 21.0% [16.7%, 24.6%] | - | 1883 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago ▲ | 23.4% [21.1%, 27.4%] | 7.4% [4.7%, 10.8%] | - | 2721 |
| Press hazard while hidden, LOS lost 0 ms ago | 12.6% [11.2%, 14.0%] | 12.3% [10.8%, 14.5%] | - | 2762 |
| Press hazard while hidden, LOS lost 50-100 ms ago | 11.8% [10.7%, 12.7%] | 9.5% [8.5%, 10.9%] | - | 3896 |
| Press hazard while hidden, LOS lost 150-250 ms ago ▲ | 9.3% [8.3%, 10.5%] | 4.6% [3.9%, 5.5%] | - | 5358 |
| Press hazard while hidden, LOS lost 300-500 ms ago ▲ | 11.1% [10.7%, 11.7%] | 7.0% [6.0%, 8.1%] | - | 16269 |
| Press hazard while hidden, LOS lost 550-1000 ms ago | 3.2% [2.8%, 3.6%] | 3.5% [3.2%, 3.9%] | - | 28614 |
| Press hazard while hidden, LOS lost over 1 s ago ▲ | 2.1% [1.8%, 2.6%] | 4.1% [3.5%, 4.6%] | - | 190280 |
| Release hazard, LOS, aim error in half-widths (0, 1] ▲ | 2.3% [1.8%, 2.9%] | 1.4% [1.1%, 1.7%] | - | 16297 |
| Release hazard, LOS, aim error in half-widths (1, 2] ▲ | 2.4% [2.0%, 2.8%] | 1.5% [1.3%, 1.9%] | - | 27527 |
| Release hazard, LOS, aim error in half-widths (2, 3] | 2.4% [1.9%, 2.8%] | 1.5% [1.2%, 2.0%] | - | 19983 |
| Release hazard, LOS, aim error in half-widths (3, 4] | 2.3% [1.9%, 2.7%] | 1.4% [1.0%, 2.0%] | - | 11528 |
| Release hazard, LOS, aim error in half-widths (4, 6] ▲ | 3.8% [3.1%, 4.3%] | 1.7% [1.3%, 2.3%] | - | 9199 |
| Release hazard, LOS, aim error in half-widths (6, 10] ▲ | 14.2% [10.9%, 17.3%] | 6.9% [4.4%, 9.9%] | - | 3225 |
| Release hazard, LOS, aim error in half-widths (10, 1000] | 20.8% [17.5%, 24.8%] | 32.7% [22.9%, 41.8%] | - | 2133 |
| Release hazard while hidden, LOS lost 0-100 ms ago | 6.1% [5.3%, 7.1%] | 6.3% [5.4%, 7.5%] | - | 13742 |
| Release hazard while hidden, LOS lost 150-250 ms ago | 13.2% [11.9%, 14.4%] | 16.5% [13.8%, 18.7%] | - | 8020 |
| Release hazard while hidden, LOS lost 300-450 ms ago | 31.3% [29.4%, 33.4%] | 31.5% [29.6%, 33.4%] | - | 8335 |
| Release hazard while hidden, LOS lost 500-950 ms ago ▲ | 13.9% [13.0%, 14.8%] | 10.1% [9.2%, 11.3%] | - | 7099 |
| Release hazard while hidden, LOS lost 1 s or more ago ▲ | 14.5% [14.1%, 14.9%] | 11.3% [9.8%, 13.5%] | - | 22591 |
| Fire held, LOS, aim error in half-widths (0, 0.5] | 85.0% [81.3%, 88.7%] | 89.6% [87.0%, 91.7%] | - | 5648 |
| Fire held, LOS, aim error in half-widths (0.5, 1] | 85.2% [81.9%, 88.6%] | 88.9% [86.9%, 90.5%] | - | 13798 |
| Fire held, LOS, aim error in half-widths (1, 1.5] | 85.2% [83.0%, 88.2%] | 86.3% [84.5%, 87.8%] | - | 16711 |
| Fire held, LOS, aim error in half-widths (1.5, 2] | 83.5% [81.3%, 86.1%] | 81.9% [79.8%, 83.7%] | - | 16264 |
| Fire held, LOS, aim error in half-widths (2, 3] | 81.5% [78.9%, 84.2%] | 81.0% [79.4%, 82.7%] | - | 24731 |
| Fire held, LOS, aim error in half-widths (3, 4] | 76.5% [74.0%, 79.4%] | 78.8% [76.1%, 81.2%] | - | 15181 |
| Fire held, LOS, aim error in half-widths (4, 6] | 64.8% [61.6%, 68.0%] | 67.7% [63.0%, 71.3%] | - | 14356 |
| Fire held, LOS, aim error in half-widths (6, 10] ▲ | 35.8% [31.9%, 40.4%] | 22.4% [17.1%, 27.1%] | - | 9107 |
| Fire held, LOS, aim error in half-widths (10, 20] ▲ | 13.6% [11.0%, 16.9%] | 4.6% [2.4%, 6.9%] | - | 7864 |
| Fire held, LOS, aim error in half-widths (20, 1000] ▲ | 6.9% [5.3%, 8.3%] | 0.9% [0.4%, 3.1%] | - | 14103 |
| Fire held without LOS ▲ | 19.2% [16.8%, 22.2%] | 24.5% [22.7%, 26.3%] | - | 307110 |
| Attack hold duration p50 | 200 ms [200 ms, 201 ms] | 150 ms [150 ms, 200 ms] | - | 13186 |
| Attack hold duration p90 | 950 ms [900 ms, 950 ms] | 900 ms [800 ms, 950 ms] | - | 13186 |
| Attack holds that are taps (<=100 ms) ▲ | 36.9% [35.2%, 39.2%] | 44.3% [41.8%, 46.5%] | - | 13186 |
| Gap between attack holds p50 ▲ | 300 ms [250 ms, 300 ms] | 400 ms [350 ms, 400 ms] | - | 12531 |

### Aim

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error firing with LOS at (0, 128] u ▲ | 17.24° [16.50°, 18.19°] | 14.28° [13.53°, 15.19°] | - | 9145 |
| Crosshair on body firing with LOS at (0, 128] u ▲ | 39.9% [38.2%, 41.7%] | 48.0% [44.9%, 51.5%] | - | 9145 |
| Median aim error firing with LOS at (128, 192] u ▲ | 10.77° [10.16°, 11.23°] | 8.99° [8.66°, 9.45°] | - | 13847 |
| Crosshair on body firing with LOS at (128, 192] u ▲ | 34.5% [32.5%, 37.0%] | 42.5% [39.8%, 44.7%] | - | 13847 |
| Median aim error firing with LOS at (192, 256] u ▲ | 7.99° [7.60°, 8.34°] | 6.68° [6.36°, 7.10°] | - | 15083 |
| Crosshair on body firing with LOS at (192, 256] u ▲ | 32.6% [30.7%, 34.4%] | 42.0% [39.5%, 44.4%] | - | 15083 |
| Median aim error firing with LOS at (256, 384] u | 5.57° [5.27°, 5.83°] | 5.23° [4.96°, 5.59°] | - | 26190 |
| Crosshair on body firing with LOS at (256, 384] u ▲ | 32.4% [30.3%, 34.2%] | 39.0% [36.0%, 41.4%] | - | 26190 |
| Median aim error firing with LOS at (384, 512] u | 4.07° [3.87°, 4.26°] | 3.92° [3.67°, 4.18°] | - | 16161 |
| Crosshair on body firing with LOS at (384, 512] u ▲ | 30.3% [28.4%, 32.2%] | 35.4% [32.6%, 38.3%] | - | 16161 |
| Median aim error firing with LOS at (512, 768] u | 3.08° [2.84°, 3.31°] | 3.07° [2.80°, 3.35°] | - | 10959 |
| Crosshair on body firing with LOS at (512, 768] u | 29.3% [27.6%, 31.1%] | 32.0% [29.1%, 35.1%] | - | 10959 |
| Median aim error firing with LOS at (768, 1200] u | 2.43° [2.18°, 2.89°] | 2.14° [1.84°, 2.58°] | - | 260 |
| Crosshair on body firing with LOS at (768, 1200] u | 26.6% [20.5%, 32.3%] | 31.5% [24.3%, 39.5%] | - | 260 |
| Aim height (fraction of body) firing with LOS | 0.444 [0.432, 0.458] | 0.446 [0.418, 0.468] | - | 91645 |
| Aim height (fraction of body) with LOS, not firing | 0.607 [0.567, 0.647] | 0.662 [0.576, 0.751] | - | 70571 |

### View

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw speed (hidden, not firing) p50 ▲ | 20°/s [18°/s, 22°/s] | 8°/s [6°/s, 10°/s] | - | 256100 |
| Yaw speed (hidden, not firing) p99 ▲ | 826°/s [808°/s, 847°/s] | 544°/s [521°/s, 580°/s] | - | 256100 |
| Mouse still between ticks (hidden, not firing) ▲ | 27.9% [27.5%, 28.4%] | 32.1% [28.8%, 36.0%] | - | 258200 |
| Yaw speed (hidden, firing) p50 ▲ | 35°/s [32°/s, 37°/s] | 23°/s [21°/s, 25°/s] | - | 62007 |
| Yaw speed (hidden, firing) p99 ▲ | 569°/s [514°/s, 607°/s] | 194°/s [182°/s, 204°/s] | - | 62007 |
| Mouse still between ticks (hidden, firing) ▲ | 7.4% [6.9%, 7.8%] | 10.9% [10.1%, 11.8%] | - | 62007 |
| Yaw speed (LOS, not firing) p50 ▲ | 52°/s [46°/s, 56°/s] | 11°/s [0°/s, 26°/s] | - | 46866 |
| Yaw speed (LOS, not firing) p99 ▲ | 914°/s [894°/s, 944°/s] | 583°/s [505°/s, 695°/s] | - | 46866 |
| Mouse still between ticks (LOS, not firing) ▲ | 18.1% [17.3%, 19.0%] | 36.7% [20.8%, 49.8%] | - | 46875 |
| Yaw speed (LOS firefight) p50 ▲ | 52°/s [46°/s, 57°/s] | 39°/s [36°/s, 41°/s] | - | 91643 |
| Yaw speed (LOS firefight) p99 ▲ | 603°/s [529°/s, 660°/s] | 305°/s [292°/s, 318°/s] | - | 91643 |
| Mouse still between ticks (LOS firefight) ▲ | 3.4% [3.1%, 3.6%] | 1.6% [1.4%, 1.8%] | - | 91643 |
| Yaw speed (reloading) p50 ▲ | 30°/s [26°/s, 33°/s] | 18°/s [16°/s, 20°/s] | - | 52718 |
| Yaw speed (reloading) p99 ▲ | 781°/s [731°/s, 821°/s] | 945°/s [909°/s, 991°/s] | - | 52718 |
| Mouse still between ticks (reloading) ▲ | 20.2% [19.0%, 21.1%] | 26.3% [25.0%, 27.6%] | - | 52718 |
| Peak yaw speed of 90-135 deg turns p50 | 641°/s [628°/s, 653°/s] | 619°/s [595°/s, 655°/s] | - | 4948 |
| Peak yaw speed of 135-180 deg turns p50 | 851°/s [830°/s, 870°/s] | 852°/s [828°/s, 887°/s] | - | 2566 |
| Peak yaw speed of 180-360 deg turns p50 | 1134°/s [1097°/s, 1162°/s] | 1061°/s [1020°/s, 1126°/s] | - | 604 |

### Belief

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw error to the hidden opponent, last seen <=250 ms | 6.03° [5.17°, 7.02°] | 4.92° [4.56°, 5.26°] | - | 34844 |
| View within 30 deg of the hidden opponent, last seen <=250 ms ▲ | 85.0% [82.9%, 87.2%] | 92.7% [91.9%, 93.7%] | - | 34844 |
| Yaw error to the hidden opponent, last seen 250-500 ms | 6.28° [5.29°, 7.21°] | 6.66° [5.99°, 7.28°] | - | 19094 |
| View within 30 deg of the hidden opponent, last seen 250-500 ms ▲ | 88.3% [86.6%, 90.2%] | 92.2% [91.3%, 93.4%] | - | 19094 |
| Yaw error to the hidden opponent, last seen 0.5-1 s | 8.86° [7.58°, 10.03°] | 8.28° [7.48°, 9.11°] | - | 19879 |
| View within 30 deg of the hidden opponent, last seen 0.5-1 s ▲ | 83.2% [80.9%, 85.5%] | 88.0% [86.8%, 89.9%] | - | 19879 |
| Yaw error to the hidden opponent, last seen 1-2 s | 12.46° [10.76°, 14.26°] | 11.11° [9.95°, 12.13°] | - | 15361 |
| View within 30 deg of the hidden opponent, last seen 1-2 s ▲ | 74.4% [71.3%, 76.8%] | 78.9% [77.1%, 81.1%] | - | 15361 |
| Yaw error to the hidden opponent, last seen 2-4 s ▲ | 30.99° [23.55°, 39.89°] | 16.93° [15.18°, 19.08°] | - | 47573 |
| View within 30 deg of the hidden opponent, last seen 2-4 s ▲ | 49.3% [43.2%, 55.8%] | 64.3% [61.3%, 67.3%] | - | 47573 |
| Yaw error to the hidden opponent, last seen 4-8 s ▲ | 28.16° [21.17°, 34.19°] | 10.29° [9.19°, 11.80°] | - | 67394 |
| View within 30 deg of the hidden opponent, last seen 4-8 s ▲ | 51.4% [46.9%, 57.0%] | 76.4% [73.1%, 79.3%] | - | 67394 |
| Yaw error to the hidden opponent, last seen >8 s | 21.60° [15.71°, 25.80°] | 20.07° [12.82°, 29.16°] | - | 21812 |
| View within 30 deg of the hidden opponent, last seen >8 s | 58.2% [54.3%, 65.7%] | 65.1% [54.8%, 74.8%] | - | 21812 |
| Yaw error to the hidden opponent, last seen never seen this life | 23.14° [20.44°, 26.85°] | 19.23° [16.93°, 24.71°] | - | 123270 |
| View within 30 deg of the hidden opponent, last seen never seen this life | 57.6% [53.9%, 60.7%] | 62.5% [56.0%, 67.2%] | - | 123270 |

### Space

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) ▲ | 36.9% [33.7%, 37.3%] | 49.4% [44.0%, 50.8%] | - | 124950 |
| Share of duel time in the busiest 10% of 32u cells (dm/main) ▲ | 32.8% [29.9%, 33.2%] | 45.2% [40.5%, 48.6%] | - | 120540 |
| Share of duel time in the busiest 10% of 32u cells (dm/vents) | 40.6% [34.8%, 41.8%] | 44.3% [39.2%, 46.8%] | - | 130670 |
| Share of duel time in the busiest 10% of 32u cells (dm/downladder) ▲ | 37.9% [32.7%, 38.9%] | 61.4% [42.9%, 63.8%] | - | 135290 |

### Acquisition

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error at -500 ms from sight gain | 17.11° [14.63°, 20.75°] | 13.98° [12.90°, 15.10°] | - | 5005 |
| Median aim error at -200 ms from sight gain ▲ | 16.21° [13.80°, 18.14°] | 8.33° [7.70°, 8.92°] | - | 5012 |
| Median aim error at +0 ms from sight gain ▲ | 15.49° [12.83°, 17.84°] | 5.22° [4.88°, 5.61°] | - | 5020 |
| Median aim error at +100 ms from sight gain ▲ | 13.53° [10.93°, 15.46°] | 5.97° [5.54°, 6.43°] | - | 5020 |
| Median aim error at +200 ms from sight gain ▲ | 11.10° [8.64°, 12.69°] | 4.92° [4.54°, 5.23°] | - | 5020 |
| Median aim error at +300 ms from sight gain ▲ | 9.13° [7.20°, 10.69°] | 4.53° [4.12°, 4.88°] | - | 4947 |
| Median aim error at +500 ms from sight gain ▲ | 8.05° [6.58°, 9.64°] | 5.47° [5.08°, 5.88°] | - | 4640 |
| Median aim error at +1000 ms from sight gain | 7.82° [6.40°, 9.27°] | 7.25° [6.72°, 7.70°] | - | 3848 |
| Fire held at -500 ms from sight gain ▲ | 23.1% [21.3%, 24.8%] | 18.2% [16.5%, 20.2%] | - | 5020 |
| Fire held at -200 ms from sight gain | 20.4% [18.0%, 22.9%] | 21.7% [19.7%, 23.9%] | - | 5020 |
| Fire held at +0 ms from sight gain ▲ | 27.7% [25.1%, 30.5%] | 42.5% [39.1%, 46.0%] | - | 5020 |
| Fire held at +100 ms from sight gain ▲ | 37.8% [35.2%, 40.8%] | 61.4% [57.8%, 64.6%] | - | 5020 |
| Fire held at +200 ms from sight gain ▲ | 49.1% [47.0%, 51.5%] | 77.0% [73.9%, 79.5%] | - | 5020 |
| Fire held at +400 ms from sight gain ▲ | 62.7% [61.2%, 64.3%] | 87.3% [85.5%, 89.0%] | - | 4903 |
| Fire held at +700 ms from sight gain ▲ | 62.0% [60.2%, 64.3%] | 77.0% [74.3%, 79.7%] | - | 4679 |
| Fire held at +1000 ms from sight gain ▲ | 57.1% [55.7%, 58.6%] | 63.4% [60.9%, 66.4%] | - | 4427 |
| Fire held at +1450 ms from sight gain | 50.8% [48.3%, 53.3%] | 53.6% [49.3%, 58.5%] | - | 4073 |
| Reaction: first trigger press after a clean sighting p25 | 100 ms [100 ms, 100 ms] | 50 ms [50 ms, 100 ms] | - | 2472 |
| Reaction: first trigger press after a clean sighting p50 | 250 ms [200 ms, 250 ms] | 150 ms [150 ms, 200 ms] | - | 2472 |
| Reaction: first trigger press after a clean sighting p75 ▲ | 400 ms [400 ms, 450 ms] | 300 ms [250 ms, 300 ms] | - | 2472 |
| Reaction: first trigger press after a clean sighting p90 ▲ | 750 ms [700 ms, 800 ms] | 400 ms [400 ms, 450 ms] | - | 2472 |
| Crosshair within half-width+1.5 deg after a clean sighting p50 ▲ | 300 ms [300 ms, 350 ms] | 200 ms [200 ms, 250 ms] | - | 2291 |
| Crosshair already on the body at the first visible tick ▲ | 10.9% [10.0%, 11.7%] | 22.6% [20.9%, 24.4%] | - | 5020 |
| Fire already held on the tick before sight (prefire) ▲ | 23.3% [20.7%, 26.4%] | 35.4% [32.2%, 38.6%] | - | 5020 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shots per burst (consecutive 100 ms shots) p50 | 4 [4, 4] | 5 [4, 5] | - | 12421 |
| Shots per burst (consecutive 100 ms shots) p90 | 13 [12, 14] | 12 [11, 13] | - | 12421 |

### Combat

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shot accuracy (eligible SMG shots that hit) ▲ | 15.3% [13.9%, 17.0%] | 19.2% [18.1%, 20.3%] | - | 75293 |

### Lives

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Respawn delay after death p50 ▲ | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | - | 2101 |
| Life length (lives ending in death) p50 ▲ | 12.4 s [11.7 s, 13.7 s] | 7.1 s [5.8 s, 8.2 s] | - | 2101 |

### Reload

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Reload within 3 s of a kill, rounds left 0 | 100.0% [100.0%, 100.0%] | 95.5% [86.4%, 100.0%] | - | 45 |
| Reload within 3 s of a kill, rounds left 1-3 | 98.1% [95.2%, 100.0%] | 96.4% [92.6%, 99.2%] | - | 128 |
| Reload within 3 s of a kill, rounds left 4-6 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 172 |
| Reload within 3 s of a kill, rounds left 7-10 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 240 |
| Reload within 3 s of a kill, rounds left 11-15 | 99.1% [98.1%, 99.7%] | 98.8% [97.9%, 99.5%] | - | 364 |
| Reload within 3 s of a kill, rounds left 16-20 | 93.8% [91.4%, 96.5%] | 95.7% [93.5%, 97.7%] | - | 413 |
| Reload within 3 s of a kill, rounds left 21-25 | 85.7% [80.6%, 88.9%] | 83.1% [76.5%, 88.5%] | - | 412 |
| Reload within 3 s of a kill, rounds left 26-29 | 64.6% [57.6%, 69.8%] | 63.4% [52.8%, 75.0%] | - | 285 |
| Reload within 3 s of a kill, rounds left 30-32 | 78.2% [63.7%, 88.1%] | 73.7% [50.0%, 92.3%] | - | 41 |
| Delay from kill to reload (reloads within 3 s) p50 | 650 ms [650 ms, 650 ms] | 600 ms [600 ms, 650 ms] | - | 1901 |
| Reloads started with the opponent dead ▲ | 68.1% [63.3%, 73.9%] | 88.2% [85.5%, 90.4%] | - | 2815 |
| Reloads with the opponent alive that are forced (empty clip) ▲ | 89.5% [87.4%, 91.8%] | 77.7% [69.5%, 83.9%] | - | 910 |

### Fights

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Engagements that end in a kill ▲ | 85.5% [83.3%, 87.7%] | 79.9% [77.9%, 81.9%] | - | 4872 |
| First hitter wins (decisive engagements) | 65.3% [63.5%, 67.2%] | 65.4% [63.7%, 66.7%] | - | 4180 |
| First shooter wins (decisive engagements) | 53.4% [52.6%, 54.2%] | 55.0% [52.5%, 56.6%] | - | 4180 |
| Duration of decisive engagements p50 ▲ | 1950 ms [1850 ms, 2051 ms] | 1250 ms [1150 ms, 1351 ms] | - | 4180 |
| Engagement start distance p50 | 343 u [300 u, 420 u] | 427 u [398 u, 470 u] | - | 4872 |
| Decisive engagements won (subject's own) | 50.1% [47.5%, 52.5%] | 50.0% [45.6%, 54.2%] | - | 4180 |

### Perception

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time with a body part on screen and the centroid hidden ▲ | 6.3% [5.1%, 7.7%] | 10.5% [9.6%, 11.8%] | - | 511440 |
| Share of duel time with a body part of the enemy on screen | 32.7% [29.9%, 35.9%] | 34.0% [31.6%, 36.5%] | - | 511440 |
| Sightings (a body part coming on screen) per minute of duel time ▲ | 27.82/min [26.19/min, 29.88/min] | 33.29/min [31.94/min, 34.72/min] | - | 511440 |
| Spell with no body part on screen between two sightings p50 | 350 ms [300 ms, 400 ms] | 400 ms [300 ms, 500 ms] | - | 7851 |
| Spell with no body part on screen between two sightings p75 | 800 ms [700 ms, 900 ms] | 900 ms [800 ms, 1000 ms] | - | 7851 |
| Spell with no body part on screen between two sightings p90 | 1800 ms [1549 ms, 2100 ms] | 1750 ms [1600 ms, 1950 ms] | - | 7851 |
| Time from both alive to the first body part on screen p50 ▲ | 1900 ms [1700 ms, 2100 ms] | 1350 ms [1300 ms, 1400 ms] | - | 4218 |
| Time from both alive to the first body part on screen p75 ▲ | 3450 ms [3050 ms, 3951 ms] | 1900 ms [1800 ms, 2000 ms] | - | 4218 |
| Net distance covered in 2 s from a tick with no body part on screen p50 ▲ | 103 u [100 u, 107 u] | 158 u [146 u, 173 u] | - | 28141 |
| Net distance covered in 2 s from a tick with no body part on screen p25 | 55 u [53 u, 58 u] | 66 u [54 u, 78 u] | - | 28141 |
| Fire held with no body part of the enemy visible (loaded, not reloading) | 15.2% [13.5%, 17.6%] | 17.3% [15.8%, 19.2%] | - | 304410 |
| Fire held with no body part visible, 0-500ms after one was last on screen ▲ | 49.5% [47.4%, 51.9%] | 43.2% [40.9%, 45.8%] | - | 41645 |
| Fire held with no body part visible, 500-1000ms after one was last on screen | 25.6% [22.9%, 27.7%] | 26.4% [23.9%, 28.7%] | - | 19404 |
| Fire held with no body part visible, 1000-2000ms after one was last on screen | 16.8% [15.2%, 18.9%] | 20.5% [18.0%, 22.9%] | - | 18641 |
| Fire held with no body part visible, 2000-5000ms after one was last on screen | 9.5% [8.0%, 11.8%] | 7.7% [6.4%, 9.1%] | - | 74147 |
| Fire held with no body part visible, gt5000ms after one was last on screen | 6.8% [5.4%, 8.9%] | 4.3% [3.1%, 6.0%] | - | 68632 |
| Attack holds begun loaded that are taps (<=100 ms), with a body part visible | 13.2% [10.8%, 15.2%] | 15.4% [13.1%, 17.6%] | - | 4507 |
| Attack holds begun loaded that are taps (<=100 ms), with no body part visible | 42.9% [40.8%, 45.9%] | 45.9% [42.2%, 48.9%] | - | 7163 |
| Shots per burst whose first round left with a body part visible p50 | 5 [5, 6] | 6 [5, 6] | - | 7569 |
| Shots per burst whose first round left with a body part visible p90 | 15 [14, 16] | 13 [12, 14] | - | 7569 |
| SMG shots that hit, a part on screen, centroid hidden ▲ | 12.3% [11.6%, 13.4%] | 21.7% [18.9%, 24.3%] | - | 9658 |
| SMG shots that hit, centroid visible ▲ | 22.1% [20.3%, 24.1%] | 27.2% [25.8%, 28.5%] | - | 46155 |
| Centroid sightings with a body part on screen first ▲ | 31.9% [29.7%, 34.4%] | 85.9% [84.2%, 87.7%] | - | 5005 |
| Lead of the first part over the centroid p50 | 50 ms [50 ms, 100 ms] | 100 ms [100 ms, 100 ms] | - | 1594 |
| Median aim error at -500 ms from the first visible part | 14.08° [12.46°, 15.47°] | 15.23° [14.18°, 16.19°] | - | 2589 |
| Median aim error at -200 ms from the first visible part | 12.50° [10.88°, 13.71°] | 10.54° [9.80°, 11.23°] | - | 2589 |
| Median aim error at -100 ms from the first visible part ▲ | 11.54° [10.20°, 13.09°] | 8.00° [7.34°, 8.49°] | - | 2589 |
| Median aim error at +0 ms from the first visible part ▲ | 11.55° [9.83°, 12.68°] | 5.22° [4.91°, 5.53°] | - | 2589 |
| Median aim error at +100 ms from the first visible part ▲ | 9.78° [8.41°, 10.73°] | 4.87° [4.54°, 5.18°] | - | 2589 |
| Median aim error at +200 ms from the first visible part ▲ | 7.16° [6.12°, 8.02°] | 5.26° [4.86°, 5.58°] | - | 2589 |
| Fire already held on the tick before the first part (prefire) | 19.9% [17.9%, 22.1%] | 23.0% [20.3%, 25.5%] | - | 2589 |
| Reaction: first press after a clean sighting, from the first part p50 | 200 ms [200 ms, 200 ms] | 200 ms [200 ms, 250 ms] | - | 1507 |
| Reaction: mean first press after a clean sighting, from the first part | 228 ms [208 ms, 242 ms] | 232 ms [217 ms, 250 ms] | - | 1507 |
| Crosshair within half-width+1.5 deg after a clean sighting, from the first part p50 | 250 ms [250 ms, 300 ms] | 250 ms [200 ms, 250 ms] | - | 1461 |
| First hit after the first part p50 ▲ | 350 ms [300 ms, 350 ms] | 250 ms [200 ms, 250 ms] | - | 1670 |
| Part sightings whose visible bout ends without a hit ▲ | 35.2% [32.2%, 38.2%] | 27.5% [25.7%, 29.7%] | - | 2589 |
| Yaw error to the hidden enemy 500 ms before the first part | 12.46° [11.07°, 13.93°] | 14.63° [13.70°, 15.63°] | - | 2589 |
| Yaw error to where the enemy will appear, 500 ms before the first part | 12.01° [10.37°, 13.17°] | 10.65° [9.39°, 11.89°] | - | 2589 |
| Crosshair parked within 5 deg of where the enemy will appear, 500 ms before | 27.3% [23.9%, 31.0%] | 29.4% [27.1%, 32.4%] | - | 2589 |
| Crosshair closer to where the enemy will appear than to the enemy, -500 ms ▲ | 52.6% [50.5%, 55.0%] | 67.0% [65.6%, 68.8%] | - | 2589 |
| View turn toward the enemy over the last 500 ms before the first part | 10.06° [8.40°, 12.10°] | 13.01° [11.44°, 14.55°] | - | 2589 |
| View turn over the last 500 ms before the first part | 12.97° [11.07°, 14.31°] | 14.53° [13.34°, 15.90°] | - | 2589 |
| Speed 400-200 ms before the first part ▲ | 144 u/s [141 u/s, 150 u/s] | 188 u/s [182 u/s, 193 u/s] | - | 2589 |
| Speed 2-1 s before the first part ▲ | 131 u/s [126 u/s, 137 u/s] | 167 u/s [159 u/s, 176 u/s] | - | 1540 |
| Hidden, no sighting next: crosshair closer to where the enemy will be in 500 ms | 50.8% [50.3%, 51.3%] | 50.5% [49.0%, 51.6%] | - | 229180 |
| Yaw error to the corner, 500 ms before the first part ▲ | 10.74° [9.83°, 11.78°] | 5.81° [5.17°, 6.44°] | - | 1704 |
| Angle to the corner, 500 ms before the first part ▲ | 12.99° [11.88°, 13.77°] | 8.41° [7.70°, 9.19°] | - | 1704 |
| Pitch to the corner (- = corner above the crosshair), 500 ms before the first part | -2.18° [-2.59°, -1.75°] | -2.72° [-3.14°, -2.39°] | - | 1704 |
| Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part ▲ | 1.31° [0.89°, 2.12°] | -1.77° [-2.04°, -1.43°] | - | 1704 |
| Yaw error to the appearance point (sightings with a corner), 500 ms before the first part | 10.86° [9.34°, 12.50°] | 10.34° [9.19°, 11.29°] | - | 1704 |
| Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part ▲ | 11.63° [10.14°, 13.13°] | 15.18° [14.24°, 16.01°] | - | 1704 |
| Crosshair within 5 deg of the corner, 500 ms before the first part ▲ | 20.7% [18.4%, 22.9%] | 29.2% [26.4%, 32.7%] | - | 1704 |
| Crosshair closer to the corner than to the enemy, 500 ms before the first part ▲ | 56.2% [54.7%, 58.0%] | 80.0% [78.8%, 81.1%] | - | 1704 |
| Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part ▲ | 37.2% [34.5%, 38.9%] | 48.2% [45.7%, 50.3%] | - | 1704 |
| Crosshair within 2 deg of the corner's edge, 500 ms before the first part ▲ | 15.2% [13.9%, 16.6%] | 23.7% [21.7%, 25.7%] | - | 1704 |
| Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part ▲ | 47.6% [45.4%, 50.8%] | 28.1% [26.8%, 29.4%] | - | 1704 |
| Angle to the corner at the first part ▲ | 10.03° [8.80°, 11.00°] | 5.74° [5.33°, 6.17°] | - | 1704 |
| Distance from the eye to the corner p50 | 191 u [155 u, 244 u] | 228 u [204 u, 248 u] | - | 1704 |
| Yaw error to the corner at -2000 ms (sightings hidden >= 2 s) | 16.34° [12.99°, 20.14°] | 14.66° [11.85°, 17.22°] | - | 539 |
| Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) ▲ | 12.71° [10.54°, 15.64°] | 5.78° [4.88°, 6.98°] | - | 539 |
| Yaw error to the corner at -500 ms (sightings hidden >= 2 s) ▲ | 11.75° [9.91°, 13.97°] | 4.66° [3.99°, 5.31°] | - | 539 |
| Yaw error to the corner at +0 ms (sightings hidden >= 2 s) ▲ | 8.70° [6.92°, 10.22°] | 3.54° [3.01°, 4.10°] | - | 539 |

Consistency: 112 pooled statistics (bots and owner together) were recomputed from the analysis scripts' own JSON; 0 differ.

## Notes

- perception statistics on the bots' logged body-part column (ext_vis_parts: their own perception); the people's are on the data repo's rebuilt column, accepted against the logged one on 2026-09-28
- 100% of the bots' duel time is against another bot (the logged opponent is the nearest living enemy); the human reference is humans against humans only
- engagements.py counts contact episodes only while exactly two players are alive; with more than two players in a session the fight statistics cover the phases where one player is dead
