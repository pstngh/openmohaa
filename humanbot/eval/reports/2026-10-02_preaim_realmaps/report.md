# Bot evaluation: 2026-10-02_preaim_realmaps

Generated 2026-10-02 by `humanbot/eval/compare.py`. Captures: `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`. 8 sessions on dm/crnodoors, dm/downladder, dm/main, dm/vents; capture dates 2026-10-02.
Human reference: `human_reference.json` (2026-10-02), 397 duel minutes, 72 player-sessions, 5 people.

## Bots in this capture

| Bot | Family | Seed | Sessions | Duel minutes |
|---|---|---:|---:|---:|
| `bot:stopper:4` | stopper | 711847621 | 1 | 31.6 |
| `bot:stopper:5` | stopper | 1529931226 | 1 | 31.6 |
| `bot:strafer:4` | strafer | 231333596 | 1 | 38.1 |
| `bot:presser:5` | presser | 1152489726 | 1 | 38.1 |
| `bot:stopper:4.2` | stopper | 2458011590 | 1 | 32.1 |
| `bot:strafer:5` | strafer | 4003987225 | 1 | 32.1 |
| `bot:strafer:4.2` | strafer | 3498264623 | 1 | 31.3 |
| `bot:stopper:5.2` | stopper | 2513918026 | 1 | 31.3 |
| `bot:presser:4` | presser | 52844662 | 1 | 36.8 |
| `bot:strafer:5.2` | strafer | 3859071546 | 1 | 36.8 |
| `bot:presser:4.2` | presser | 588158740 | 1 | 34.4 |
| `bot:presser:5.2` | presser | 4131749178 | 1 | 34.4 |
| `bot:stopper:4.3` | stopper | 97502759 | 1 | 33.7 |
| `bot:strafer:5.3` | strafer | 2592015535 | 1 | 33.7 |
| `bot:strafer:4.3` | strafer | 1086809828 | 1 | 32.6 |
| `bot:strafer:5.4` | strafer | 821368291 | 1 | 32.6 |

The owner (`pstN@vsbot`) has 0.0 duel minutes against the bots; those rows are never pooled with the bots.

Bot duel time against the owner 0%, against other bots 100%.

Family mix of bot duel time (observed → reweighted to the styles.json targets): presser 27% → 20%, stopper 30% → 40%, strafer 44% → 40%.

## Tells (149)

Statistics whose 95% CIs for the pooled bots and the pooled humans do not overlap, ranked by the standardized gap z = (bots − humans) / √(SE²bots + SE²humans). Only statistics with enough data on both sides are ranked.

| # | Statistic | Bots | Humans | z |
|---:|---|---|---|---:|
| 1 | Side-hold duration (all contexts) p50 | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | -28.3 |
| 2 | Reloads started with the opponent dead | 30.3% [26.8%, 32.9%] | 88.2% [85.5%, 90.4%] | -27.3 |
| 3 | Centroid sightings with a body part on screen first | 36.5% [33.5%, 40.2%] | 85.9% [84.2%, 87.7%] | -23.9 |
| 4 | Respawn delay after death p50 | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | +20.7 |
| 5 | Fire held at +400 ms from sight gain | 59.7% [57.2%, 61.9%] | 87.3% [85.5%, 89.0%] | -18.6 |
| 6 | Yaw speed (hidden, firing) p99 | 472°/s [439°/s, 499°/s] | 194°/s [182°/s, 204°/s] | +17.8 |
| 7 | Engagements that end in a kill | 53.6% [51.3%, 55.3%] | 79.9% [77.9%, 81.9%] | -17.7 |
| 8 | Shot accuracy (eligible SMG shots that hit) | 6.4% [5.5%, 7.1%] | 19.2% [18.1%, 20.3%] | -17.7 |
| 9 | Speed 400-200 ms before the first part | 116 u/s [109 u/s, 121 u/s] | 188 u/s [182 u/s, 193 u/s] | -15.7 |
| 10 | Fire held at +200 ms from sight gain | 49.2% [46.8%, 51.7%] | 77.0% [73.9%, 79.5%] | -14.4 |
| 11 | Attack holds that are taps (<=100 ms) | 24.3% [23.1%, 25.9%] | 44.3% [41.8%, 46.5%] | -14.2 |
| 12 | Key chord fwd diag (hidden, not firing) | 9.2% [8.2%, 10.1%] | 29.9% [26.7%, 33.2%] | -12.5 |
| 13 | Side-hold duration (all contexts) p90 | 500 ms [500 ms, 500 ms] | 800 ms [800 ms, 850 ms] | -12.5 |
| 14 | Yaw speed (reloading) p99 | 552°/s [505°/s, 594°/s] | 945°/s [909°/s, 991°/s] | -12.4 |
| 15 | Key chord neutral (reloading) | 28.2% [25.7%, 31.7%] | 8.3% [7.3%, 9.5%] | +12.4 |
| 16 | Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part | 50.2% [46.6%, 53.2%] | 28.1% [26.8%, 29.4%] | +12.0 |
| 17 | Key chord fwd diag (reloading) | 15.9% [13.7%, 17.6%] | 36.8% [34.1%, 39.9%] | -11.7 |
| 18 | Crosshair closer to the corner than to the enemy, 500 ms before the first part | 56.9% [53.7%, 60.7%] | 80.0% [78.8%, 81.1%] | -11.5 |
| 19 | Fire held at +700 ms from sight gain | 56.8% [54.8%, 58.8%] | 77.0% [74.3%, 79.7%] | -11.5 |
| 20 | Crosshair closer to where the enemy will appear than to the enemy, -500 ms | 53.0% [50.7%, 54.6%] | 67.0% [65.6%, 68.8%] | -11.3 |
| 21 | Speed 2-1 s before the first part | 113 u/s [107 u/s, 119 u/s] | 167 u/s [159 u/s, 176 u/s] | -10.9 |
| 22 | Retreating >40 u/s in LOS firefight at distance (160, 224] | 18.1% [17.0%, 19.6%] | 6.9% [5.5%, 8.7%] | +10.3 |
| 23 | Part sightings whose visible bout ends without a hit | 48.9% [46.2%, 53.5%] | 27.5% [25.7%, 29.7%] | +10.0 |
| 24 | Walk presses per minute | 3.32/min [2.64/min, 3.82/min] | 7.54/min [6.90/min, 8.11/min] | -9.2 |
| 25 | Median aim error at +0 ms from the first visible part | 10.21° [9.28°, 11.32°] | 5.22° [4.91°, 5.53°] | +8.9 |
| 26 | Key chord back any (reloading) | 12.5% [12.1%, 13.0%] | 6.5% [5.3%, 7.6%] | +8.8 |
| 27 | Fire held at +100 ms from sight gain | 41.3% [38.4%, 44.1%] | 61.4% [57.8%, 64.6%] | -8.7 |
| 28 | Key chord forward (reloading) | 14.3% [13.1%, 15.5%] | 26.1% [23.8%, 28.6%] | -8.5 |
| 29 | Side-hold duration (LOS firefight) p90 | 500 ms [450 ms, 500 ms] | 700 ms [650 ms, 700 ms] | -8.3 |
| 30 | Strafe switch hazard per tick at hold age 100 ms (LOS firefight) | 13.3% [11.9%, 14.9%] | 5.8% [5.2%, 6.8%] | +8.3 |
| 31 | Approaching >40 u/s in LOS firefight at distance (96, 160] | 15.8% [13.7%, 17.7%] | 36.4% [32.6%, 41.2%] | -8.3 |
| 32 | Strafe switch hazard per tick at hold age 50 ms (LOS firefight) | 9.6% [7.9%, 11.7%] | 1.2% [0.9%, 1.6%] | +8.2 |
| 33 | Side-hold duration (LOS firefight) p75 | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | -8.1 |
| 34 | Retreating >40 u/s in LOS firefight at distance (96, 160] | 22.7% [20.9%, 24.8%] | 10.5% [8.5%, 12.6%] | +8.0 |
| 35 | Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part | 2.05° [1.22°, 2.84°] | -1.77° [-2.04°, -1.43°] | +7.9 |
| 36 | Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) | 23.7% [21.9%, 25.1%] | 15.6% [14.7%, 16.9%] | +7.9 |
| 37 | Mouse still between ticks (hidden, firing) | 6.8% [6.2%, 7.4%] | 10.9% [10.1%, 11.8%] | -7.7 |
| 38 | Release hazard while hidden, LOS lost 300-450 ms ago | 15.5% [12.3%, 18.9%] | 31.5% [29.6%, 33.4%] | -7.7 |
| 39 | Approaching >40 u/s in LOS firefight at distance (160, 224] | 13.1% [12.1%, 14.2%] | 35.4% [30.1%, 42.0%] | -7.5 |
| 40 | Median aim error firing with LOS at (128, 192] u | 11.04° [10.68°, 11.38°] | 8.99° [8.66°, 9.45°] | +7.5 |
| 41 | Crosshair already on the body at the first visible tick | 13.8% [12.6%, 15.6%] | 22.6% [20.9%, 24.4%] | -7.5 |
| 42 | Angle to the corner at the first part | 9.38° [8.49°, 10.08°] | 5.74° [5.33°, 6.17°] | +7.5 |
| 43 | Share of duel time: LOS firefight | 8.7% [7.0%, 10.9%] | 17.6% [16.5%, 18.9%] | -7.4 |
| 44 | Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part | 32.9% [30.1%, 36.6%] | 48.2% [45.7%, 50.3%] | -7.4 |
| 45 | Fire held, LOS, aim error in half-widths (6, 10] | 48.0% [44.5%, 53.1%] | 22.4% [17.1%, 27.1%] | +7.3 |
| 46 | Key chord fwd diag (hidden, firing) | 10.7% [9.6%, 11.7%] | 28.6% [23.9%, 32.8%] | -7.3 |
| 47 | Fire held at -500 ms from sight gain | 31.7% [28.7%, 35.1%] | 18.2% [16.5%, 20.2%] | +7.3 |
| 48 | Fire held, LOS, aim error in half-widths (20, 1000] | 18.7% [13.9%, 23.0%] | 0.9% [0.4%, 3.1%] | +7.2 |
| 49 | SMG shots that hit, a part on screen, centroid hidden | 11.7% [10.5%, 13.2%] | 21.7% [18.9%, 24.3%] | -7.1 |
| 50 | Release hazard while hidden, LOS lost 150-250 ms ago | 5.8% [4.1%, 7.7%] | 16.5% [13.8%, 18.7%] | -7.1 |
| 51 | Strafe switch hazard per tick at hold age 250 ms (LOS firefight) | 25.7% [24.1%, 27.3%] | 17.4% [16.0%, 19.0%] | +7.0 |
| 52 | Approaching >40 u/s in LOS firefight at distance (224, 288] | 12.9% [11.2%, 14.6%] | 38.2% [32.3%, 46.4%] | -6.9 |
| 53 | Fire held at +1000 ms from sight gain | 49.3% [46.5%, 52.1%] | 63.4% [60.9%, 66.4%] | -6.9 |
| 54 | Yaw speed (LOS firefight) p99 | 711°/s [585°/s, 805°/s] | 305°/s [292°/s, 318°/s] | +6.9 |
| 55 | Key chord neutral (hidden, not firing) | 40.0% [37.3%, 43.0%] | 26.1% [23.1%, 28.6%] | +6.8 |
| 56 | Life length (lives ending in death) p50 | 24.7 s [20.0 s, 29.0 s] | 7.1 s [5.8 s, 8.2 s] | +6.8 |
| 57 | Approaching >40 u/s in LOS firefight at distance (288, 384] | 13.6% [11.6%, 15.5%] | 39.4% [32.7%, 47.2%] | -6.7 |
| 58 | Key chord fwd diag (LOS firefight) | 9.3% [7.8%, 10.8%] | 30.5% [25.6%, 36.7%] | -6.7 |
| 59 | Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) | 22.9% [21.0%, 24.4%] | 16.1% [14.9%, 17.5%] | +6.5 |
| 60 | Press hazard while hidden, LOS lost 300-500 ms ago | 10.5% [10.0%, 11.1%] | 7.0% [6.0%, 8.1%] | +6.3 |
| 61 | Retreating >40 u/s in LOS firefight at distance (288, 384] | 13.6% [12.0%, 15.9%] | 5.9% [4.6%, 7.3%] | +6.2 |
| 62 | Median aim error at +0 ms from sight gain | 11.72° [9.59°, 13.45°] | 5.22° [4.88°, 5.61°] | +6.2 |
| 63 | Reaction: first trigger press after a clean sighting p90 | 700 ms [600 ms, 751 ms] | 400 ms [400 ms, 450 ms] | +6.1 |
| 64 | Attack hold duration p50 | 350 ms [350 ms, 400 ms] | 150 ms [150 ms, 200 ms] | +6.1 |
| 65 | Retreating >40 u/s in LOS firefight at distance (512, 768] | 11.9% [8.2%, 12.9%] | 4.0% [3.0%, 5.4%] | +6.1 |
| 66 | Standing still (<5 u/s) (reloading) | 10.8% [9.5%, 12.5%] | 5.2% [4.3%, 6.2%] | +6.0 |
| 67 | Crosshair on body firing with LOS at (192, 256] u | 32.1% [30.1%, 33.5%] | 42.0% [39.5%, 44.4%] | -6.0 |
| 68 | Approaching >40 u/s in LOS firefight at distance (384, 512] | 15.1% [13.4%, 17.0%] | 41.8% [33.8%, 49.1%] | -6.0 |
| 69 | Fire held, LOS, aim error in half-widths (10, 20] | 25.2% [19.0%, 31.6%] | 4.6% [2.4%, 6.9%] | +5.9 |
| 70 | Share of duel time with a body part on screen and the centroid hidden | 5.1% [3.9%, 6.5%] | 10.5% [9.6%, 11.8%] | -5.9 |
| 71 | Yaw speed (hidden, not firing) p99 | 646°/s [629°/s, 665°/s] | 544°/s [521°/s, 580°/s] | +5.8 |
| 72 | Yaw error to the corner at +0 ms (sightings hidden >= 2 s) | 6.65° [5.92°, 7.51°] | 3.54° [3.01°, 4.10°] | +5.8 |
| 73 | Mouse still between ticks (LOS firefight) | 2.7% [2.4%, 3.0%] | 1.6% [1.4%, 1.8%] | +5.7 |
| 74 | Median aim error at +100 ms from the first visible part | 8.66° [7.64°, 10.06°] | 4.87° [4.54°, 5.18°] | +5.7 |
| 75 | Press hazard while hidden, LOS lost 550-1000 ms ago | 5.2% [4.8%, 5.7%] | 3.5% [3.2%, 3.9%] | +5.7 |
| 76 | Fire already held on the tick before the first part (prefire) | 32.0% [30.4%, 33.5%] | 23.0% [20.3%, 25.5%] | +5.6 |
| 77 | Median aim error firing with LOS at (0, 128] u | 20.33° [18.03°, 21.87°] | 14.28° [13.53°, 15.19°] | +5.6 |
| 78 | Fire held at -200 ms from sight gain | 32.3% [29.7%, 35.8%] | 21.7% [19.7%, 23.9%] | +5.6 |
| 79 | Fire held without LOS | 35.8% [32.4%, 39.6%] | 24.5% [22.7%, 26.3%] | +5.5 |
| 80 | Release hazard while hidden, LOS lost 0-100 ms ago | 2.3% [1.6%, 3.3%] | 6.3% [5.4%, 7.5%] | -5.4 |
| 81 | Side-hold duration (LOS firefight) p50 | 200 ms [200 ms, 200 ms] | 300 ms [250 ms, 300 ms] | -5.4 |
| 82 | Median aim error firing with LOS at (192, 256] u | 8.07° [7.81°, 8.40°] | 6.68° [6.36°, 7.10°] | +5.4 |
| 83 | View within 30 deg of the hidden opponent, last seen 4-8 s | 62.1% [58.4%, 66.2%] | 76.4% [73.1%, 79.3%] | -5.4 |
| 84 | Share of duel time: reloading | 16.8% [14.7%, 18.9%] | 10.5% [9.6%, 11.5%] | +5.4 |
| 85 | Key chord pure strafe (LOS firefight) | 73.0% [70.3%, 75.7%] | 58.0% [53.1%, 62.2%] | +5.3 |
| 86 | First hitter wins (decisive engagements) | 72.2% [70.1%, 74.2%] | 65.4% [63.7%, 66.7%] | +5.3 |
| 87 | Crosshair on body firing with LOS at (256, 384] u | 29.5% [27.3%, 31.7%] | 39.0% [36.0%, 41.4%] | -5.2 |
| 88 | Key chord pure strafe (hidden, firing) | 57.7% [53.6%, 60.8%] | 44.9% [41.6%, 48.0%] | +5.2 |
| 89 | Retreating >40 u/s in LOS firefight at distance (384, 512] | 11.7% [9.8%, 14.4%] | 4.8% [3.8%, 6.0%] | +5.2 |
| 90 | Retreating >40 u/s in LOS firefight at distance (224, 288] | 15.1% [12.6%, 19.6%] | 5.3% [4.2%, 6.8%] | +5.2 |
| 91 | Release hazard, LOS, aim error in half-widths (10, 1000] | 7.5% [6.0%, 9.5%] | 32.7% [22.9%, 41.8%] | -5.2 |
| 92 | Key chord fwd diag (LOS, not firing) | 5.0% [3.0%, 11.2%] | 29.4% [23.0%, 38.1%] | -5.1 |
| 93 | Median aim error at -100 ms from the first visible part | 10.50° [9.76°, 11.20°] | 8.00° [7.34°, 8.49°] | +5.0 |
| 94 | Strafe switch hazard per tick at hold age 200 ms (LOS firefight) | 24.2% [21.7%, 26.2%] | 17.0% [15.4%, 18.7%] | +5.0 |
| 95 | Crosshair on body firing with LOS at (128, 192] u | 35.3% [33.7%, 36.6%] | 42.5% [39.8%, 44.7%] | -5.0 |
| 96 | SMG shots that hit, centroid visible | 20.8% [18.3%, 22.4%] | 27.2% [25.8%, 28.5%] | -4.9 |
| 97 | Release hazard, LOS, aim error in half-widths (0, 1] | 0.5% [0.3%, 0.7%] | 1.4% [1.1%, 1.7%] | -4.7 |
| 98 | Key chord forward (hidden, not firing) | 8.7% [8.1%, 9.6%] | 11.9% [10.9%, 12.8%] | -4.7 |
| 99 | Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part | 11.33° [9.77°, 12.34°] | 15.18° [14.24°, 16.01°] | -4.6 |
| 100 | Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) | 23.7% [20.8%, 28.3%] | 13.7% [12.0%, 15.6%] | +4.5 |
| 101 | Share of duel time in the busiest 10% of 32u cells (dm/main) | 33.7% [30.0%, 34.5%] | 45.2% [40.5%, 48.6%] | -4.4 |
| 102 | Key chord back any (hidden, not firing) | 8.3% [8.0%, 8.7%] | 6.1% [5.3%, 7.1%] | +4.4 |
| 103 | Yaw error to the hidden opponent, last seen 4-8 s | 18.89° [15.62°, 22.06°] | 10.29° [9.19°, 11.80°] | +4.4 |
| 104 | Release hazard, LOS, aim error in half-widths (1, 2] | 0.7% [0.5%, 0.9%] | 1.5% [1.3%, 1.9%] | -4.4 |
| 105 | Attack hold duration p90 | 1400 ms [1200 ms, 1650 ms] | 900 ms [800 ms, 950 ms] | +4.4 |
| 106 | Release hazard while hidden, LOS lost 500-950 ms ago | 6.3% [5.0%, 7.5%] | 10.1% [9.2%, 11.3%] | -4.3 |
| 107 | Approaching >40 u/s in LOS firefight at distance (0, 96] | 22.1% [17.4%, 26.9%] | 37.6% [33.7%, 42.3%] | -4.3 |
| 108 | Median aim error at +100 ms from sight gain | 9.98° [8.22°, 11.68°] | 5.97° [5.54°, 6.43°] | +4.3 |
| 109 | Key chord forward (hidden, firing) | 3.9% [3.1%, 4.8%] | 6.6% [5.7%, 7.4%] | -4.3 |
| 110 | Median aim error at +300 ms from sight gain | 7.53° [6.07°, 8.71°] | 4.53° [4.12°, 4.88°] | +4.2 |
| 111 | Press hazard while hidden, LOS lost 150-250 ms ago | 8.1% [6.8%, 9.4%] | 4.6% [3.9%, 5.5%] | +4.2 |
| 112 | Median aim error at -200 ms from sight gain | 12.76° [10.60°, 14.48°] | 8.33° [7.70°, 8.92°] | +4.1 |
| 113 | Release hazard, LOS, aim error in half-widths (2, 3] | 0.6% [0.5%, 0.8%] | 1.5% [1.2%, 2.0%] | -4.0 |
| 114 | Approaching >40 u/s in LOS firefight at distance (512, 768] | 14.3% [12.7%, 19.0%] | 33.1% [25.5%, 41.5%] | -4.0 |
| 115 | Median aim error at +200 ms from sight gain | 8.12° [6.70°, 9.56°] | 4.92° [4.54°, 5.23°] | +4.0 |
| 116 | Key chord pure strafe (reloading) | 29.1% [26.5%, 31.3%] | 22.3% [19.6%, 24.4%] | +4.0 |
| 117 | Key chord pure strafe (hidden, not firing) | 33.7% [31.3%, 35.9%] | 26.0% [23.5%, 29.4%] | +3.9 |
| 118 | Yaw error to the hidden enemy 500 ms before the first part | 10.98° [9.47°, 12.38°] | 14.63° [13.70°, 15.63°] | -3.9 |
| 119 | Share of duel time in the busiest 10% of 32u cells (dm/downladder) | 37.5% [33.0%, 38.1%] | 61.4% [42.9%, 63.8%] | -3.8 |
| 120 | Key chord back any (hidden, firing) | 9.0% [8.6%, 9.5%] | 6.7% [5.9%, 7.9%] | +3.8 |
| 121 | Crosshair within 2 deg of the corner's edge, 500 ms before the first part | 16.9% [14.1%, 19.9%] | 23.7% [21.7%, 25.7%] | -3.8 |
| 122 | View within 30 deg of the hidden opponent, last seen <=250 ms | 89.6% [88.5%, 91.0%] | 92.7% [91.9%, 93.7%] | -3.8 |
| 123 | View turn toward the enemy over the last 500 ms before the first part | 7.14° [4.51°, 9.73°] | 13.01° [11.44°, 14.55°] | -3.7 |
| 124 | Crosshair on body firing with LOS at (0, 128] u | 40.7% [38.6%, 43.2%] | 48.0% [44.9%, 51.5%] | -3.5 |
| 125 | Yaw speed (LOS firefight) p50 | 53°/s [46°/s, 61°/s] | 39°/s [36°/s, 41°/s] | +3.5 |
| 126 | Share of duel time: hidden, firing | 22.3% [19.0%, 25.3%] | 15.8% [14.9%, 17.0%] | +3.5 |
| 127 | Angle to the corner, 500 ms before the first part | 10.77° [9.76°, 11.82°] | 8.41° [7.70°, 9.19°] | +3.5 |
| 128 | Share of duel time: hidden, not firing | 41.5% [37.7%, 45.1%] | 49.1% [47.4%, 51.2%] | -3.5 |
| 129 | Yaw error to the corner at -500 ms (sightings hidden >= 2 s) | 7.11° [6.05°, 8.30°] | 4.66° [3.99°, 5.31°] | +3.4 |
| 130 | Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago | 16.4% [12.5%, 21.5%] | 7.4% [4.7%, 10.8%] | +3.3 |
| 131 | Release hazard while hidden, LOS lost 1 s or more ago | 7.7% [6.8%, 8.8%] | 11.3% [9.8%, 13.5%] | -3.3 |
| 132 | Fire held at +1450 ms from sight gain | 44.0% [40.8%, 47.4%] | 53.6% [49.3%, 58.5%] | -3.3 |
| 133 | Strafe switch hazard per tick at hold age 150 ms (LOS firefight) | 17.1% [15.2%, 19.7%] | 12.4% [11.1%, 13.9%] | +3.3 |
| 134 | Key chord back any (LOS firefight) | 7.8% [6.9%, 9.2%] | 5.2% [4.5%, 6.1%] | +3.3 |
| 135 | Yaw error to the corner, 500 ms before the first part | 8.21° [7.37°, 9.73°] | 5.81° [5.17°, 6.44°] | +3.3 |
| 136 | Fire held, LOS, aim error in half-widths (1.5, 2] | 88.5% [85.1%, 91.7%] | 81.9% [79.8%, 83.7%] | +3.3 |
| 137 | Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) | 9.00° [7.09°, 10.49°] | 5.78° [4.88°, 6.98°] | +3.1 |
| 138 | Key chord neutral (LOS firefight) | 8.9% [7.1%, 11.1%] | 5.1% [3.9%, 6.3%] | +3.0 |
| 139 | Fire held, LOS, aim error in half-widths (2, 3] | 86.8% [83.5%, 90.0%] | 81.0% [79.4%, 82.7%] | +3.0 |
| 140 | Fire held at +0 ms from sight gain | 35.4% [32.6%, 38.7%] | 42.5% [39.1%, 46.0%] | -3.0 |
| 141 | View turn over the last 500 ms before the first part | 10.61° [8.01°, 12.41°] | 14.53° [13.34°, 15.90°] | -3.0 |
| 142 | Release hazard, LOS, aim error in half-widths (3, 4] | 0.5% [0.3%, 0.7%] | 1.4% [1.0%, 2.0%] | -2.9 |
| 143 | Crosshair on body firing with LOS at (384, 512] u | 28.8% [25.1%, 31.8%] | 35.4% [32.6%, 38.3%] | -2.9 |
| 144 | Duration of decisive engagements p50 | 1550 ms [1399 ms, 1700 ms] | 1250 ms [1150 ms, 1351 ms] | +2.8 |
| 145 | Retreating >40 u/s in LOS firefight at distance (0, 96] | 29.8% [25.8%, 33.8%] | 22.2% [18.9%, 25.6%] | +2.7 |
| 146 | Key chord neutral (hidden, firing) | 18.7% [16.1%, 22.3%] | 13.2% [11.2%, 15.7%] | +2.7 |
| 147 | Side-hold duration (LOS firefight) p25 | 150 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | -2.6 |
| 148 | Median aim error at -500 ms from the first visible part | 12.99° [11.36°, 14.08°] | 15.23° [14.18°, 16.19°] | -2.5 |
| 149 | Reaction: first trigger press after a clean sighting p75 | 400 ms [350 ms, 450 ms] | 300 ms [250 ms, 300 ms] | +2.4 |

## Per bot

### `bot:stopper:4` (stopper, seed 711847621, 31.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.07034 | 0.05597 | -3% |
| reverse_share | 0.1382 | 0.1071 | -4% |
| side_hold_ms | 249.3 | 150 | -66% ⚠ |
| lean_fight | 0.9549 | 0.9569 | +0% |
| jumps_per_min | 0.3303 | 0.2214 | -2% |
| crouch_per_min | 5.571 | 5.884 | +3% |
| walk_hidden | 0 | 0.02541 | +24% |
| burst_median | 3.59 | 5 | +35% ⚠ |
| aim_height_firing | 0.4682 | 0.434 | -18% |
| hold_angle | 0.4674 | 0.3653 | -33% ⚠ |
| aim_error_fight_deg | 6.249 | 5.106 | -47% ⚠ |
| reaction_ms | 153.7 | 300 | +146% ⚠ |
| mp40_share | 0.02524 | 0 | -3% |

Style fingerprint (2026-10-02, 31.6 min, styles.py features 30/30): z-distance to the human cloud centre 10.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.057 (humans 0.152–0.491); neutral 0.456 (humans 0.0872–0.296); lf_fwd_diag 0.056 (humans 0.0658–0.568); lf_neutral 0.186 (humans 0.00436–0.112); lf_back_any 0.136 (humans 0.0205–0.123); lean_any 0.848 (humans 0.358–0.767); lf_lean 0.957 (humans 0.457–0.948); walk 0.0111 (humans 0.0126–0.0881); p_attack_given_los 0.536 (humans 0.547–0.861); speed_med 100 (humans 136–200); still 0.233 (humans 0.0832–0.227); lf_miss_units_med 36.5 (humans 22.2–36.1); lf_on_target 0.251 (humans 0.274–0.515); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 500 (humans 700–950); reverse_share 0.107 (humans 0.138–0.839); lf_approach 0.126 (humans 0.131–0.64); lf_retreat 0.189 (humans 0.0243–0.141).

### `bot:stopper:5` (stopper, seed 1529931226, 31.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1013 | 0.07607 | -5% |
| reverse_share | 0.3253 | 0.2519 | -10% |
| side_hold_ms | 247.2 | 150 | -65% ⚠ |
| lean_fight | 0.8427 | 0.8101 | -6% |
| jumps_per_min | 5.122 | 5.821 | +13% |
| crouch_per_min | 11.7 | 11.89 | +2% |
| walk_hidden | 0.07078 | 0.08725 | +16% |
| burst_median | 3.138 | 4 | +22% |
| aim_height_firing | 0.4597 | 0.406 | -29% ⚠ |
| hold_angle | 0.5274 | 0.3679 | -52% ⚠ |
| aim_error_fight_deg | 4.835 | 3.462 | -56% ⚠ |
| reaction_ms | 109.9 | 200 | +90% ⚠ |
| mp40_share | 0.06948 | 0 | -7% |

Style fingerprint (2026-10-02, 31.6 min, styles.py features 30/30): z-distance to the human cloud centre 10.3 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.7 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0707 (humans 0.152–0.491); neutral 0.44 (humans 0.0872–0.296); lf_neutral 0.159 (humans 0.00436–0.112); ducked 0.133 (humans 0.0109–0.0741); speed_med 91.1 (humans 136–200); still 0.236 (humans 0.0832–0.227); lf_err_med 3.46 (humans 4.08–6.63); yaw_speed_med 12.1 (humans 12.6–30); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 550 (humans 700–950).

### `bot:strafer:4` (strafer, seed 231333596, 38.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.04509 | -4% |
| reverse_share | 0.4467 | 0.3672 | -11% |
| side_hold_ms | 326.3 | 200 | -84% ⚠ |
| lean_fight | 0.4941 | 0.5988 | +21% |
| jumps_per_min | 2.379 | 1.837 | -10% |
| crouch_per_min | 9.476 | 7.165 | -21% |
| walk_hidden | 0 | 0.008128 | +8% |
| burst_median | 7 | 9.5 | +62% ⚠ |
| aim_height_firing | 0.4857 | 0.445 | -22% |
| hold_angle | 0.2514 | 0.2973 | +15% |
| aim_error_fight_deg | 4.667 | 8.172 | +143% ⚠ |
| reaction_ms | 140.8 | 275 | +134% ⚠ |
| mp40_share | 0.9062 | 1 | +9% |

Style fingerprint (2026-10-02, 38.1 min, styles.py features 30/30): z-distance to the human cloud centre 20.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 18.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.031 (humans 0.152–0.491); neutral 0.658 (humans 0.0872–0.296); lf_pure_strafe 0.768 (humans 0.362–0.752); lf_fwd_diag 0.0451 (humans 0.0658–0.568); lean_any 0.134 (humans 0.358–0.767); walk 0.0016 (humans 0.0126–0.0881); attack 0.118 (humans 0.276–0.478); p_attack_given_los 0.0491 (humans 0.547–0.861); speed_med 0 (humans 136–200); still 0.573 (humans 0.0832–0.227); lf_err_med 8.17 (humans 4.08–6.63); yaw_speed_med 0 (humans 12.6–30); yaw_speed_p95 145 (humans 158–271); zero_yaw_share 0.618 (humans 0.137–0.309); side_hold_p90_ms 450 (humans 700–950); lf_approach 0.101 (humans 0.131–0.64); lf_retreat 0.271 (humans 0.0243–0.141).

### `bot:presser:5` (presser, seed 1152489726, 38.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4832 | 0.3978 | -17% |
| reverse_share | 0.8051 | 0.7506 | -8% |
| side_hold_ms | 295.2 | 150 | -97% ⚠ |
| lean_fight | 0.9204 | 0.9501 | +6% |
| jumps_per_min | 0.4317 | 0.2887 | -3% |
| crouch_per_min | 3.157 | 2.651 | -5% |
| walk_hidden | 0.02816 | 0.01443 | -13% |
| burst_median | 5.983 | 8 | +50% ⚠ |
| aim_height_firing | 0.417 | 0.3525 | -34% ⚠ |
| hold_angle | 0.2275 | 0.4857 | +84% ⚠ |
| aim_error_fight_deg | 4.893 | 3.877 | -41% ⚠ |
| reaction_ms | 152.5 | 150 | -2% |
| mp40_share | 0.1123 | 1 | +89% ⚠ |

Style fingerprint (2026-10-02, 38.1 min, styles.py features 30/30): z-distance to the human cloud centre 20.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 17.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0941 (humans 0.152–0.491); neutral 0.641 (humans 0.0872–0.296); back_any 0.0314 (humans 0.0362–0.128); lf_back_any 0.0199 (humans 0.0205–0.123); lean_any 0.348 (humans 0.358–0.767); lf_lean 0.95 (humans 0.457–0.948); walk 0.00389 (humans 0.0126–0.0881); attack 0.0951 (humans 0.276–0.478); p_attack_given_los 0.0242 (humans 0.547–0.861); speed_med 0 (humans 136–200); still 0.595 (humans 0.0832–0.227); lf_err_med 3.88 (humans 4.08–6.63); yaw_speed_med 0 (humans 12.6–30); yaw_speed_p95 140 (humans 158–271); yaw_speed_p99 411 (humans 427–754); zero_yaw_share 0.652 (humans 0.137–0.309); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 350 (humans 700–950).

### `bot:stopper:4.2` (stopper, seed 2458011590, 32.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1182 | 0.06695 | -10% |
| reverse_share | 0.4266 | 0.3806 | -7% |
| side_hold_ms | 200 | 200 | +0% |
| lean_fight | 0.8817 | 0.8921 | +2% |
| jumps_per_min | 2.965 | 3.145 | +3% |
| crouch_per_min | 8.679 | 7.069 | -15% |
| walk_hidden | 0.1056 | 0.101 | -4% |
| burst_median | 3 | 4 | +25% ⚠ |
| aim_height_firing | 0.4719 | 0.444 | -15% |
| hold_angle | 0.3772 | 0.2302 | -48% ⚠ |
| aim_error_fight_deg | 4.42 | 6.981 | +104% ⚠ |
| reaction_ms | 139.9 | 200 | +60% ⚠ |
| mp40_share | 0.02794 | 0 | -3% |

Style fingerprint (2026-10-02, 32.1 min, styles.py features 30/30): z-distance to the human cloud centre 9.3 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0699 (humans 0.152–0.491); neutral 0.431 (humans 0.0872–0.296); lf_neutral 0.134 (humans 0.00436–0.112); p_attack_given_los 0.511 (humans 0.547–0.861); speed_med 92.4 (humans 136–200); still 0.238 (humans 0.0832–0.227); lf_err_med 6.98 (humans 4.08–6.63); side_hold_p90_ms 550 (humans 700–950); lf_approach 0.127 (humans 0.131–0.64); lf_retreat 0.163 (humans 0.0243–0.141).

### `bot:strafer:5` (strafer, seed 4003987225, 32.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.05392 | -2% |
| reverse_share | 0.4657 | 0.416 | -7% |
| side_hold_ms | 270.9 | 200 | -47% ⚠ |
| lean_fight | 0.5402 | 0.591 | +10% |
| jumps_per_min | 4.852 | 5.543 | +13% |
| crouch_per_min | 3.183 | 2.771 | -4% |
| walk_hidden | 0.03847 | 0.04872 | +10% |
| burst_median | 4.685 | 6 | +33% ⚠ |
| aim_height_firing | 0.4588 | 0.426 | -17% |
| hold_angle | 0.311 | 0.2907 | -7% |
| aim_error_fight_deg | 5.446 | 7.843 | +98% ⚠ |
| reaction_ms | 161.3 | 300 | +139% ⚠ |
| mp40_share | 0 | 0 | +0% |

Style fingerprint (2026-10-02, 32.1 min, styles.py features 30/30): z-distance to the human cloud centre 9.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0811 (humans 0.152–0.491); neutral 0.308 (humans 0.0872–0.296); lf_fwd_diag 0.0539 (humans 0.0658–0.568); p_attack_given_los 0.518 (humans 0.547–0.861); speed_med 118 (humans 136–200); lf_err_med 7.85 (humans 4.08–6.63); yaw_speed_p95 274 (humans 158–271); side_hold_p90_ms 550 (humans 700–950); lf_approach 0.126 (humans 0.131–0.64); lf_retreat 0.209 (humans 0.0243–0.141).

### `bot:strafer:4.2` (strafer, seed 3498264623, 31.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1522 | 0.05683 | -19% |
| reverse_share | 0.4724 | 0.4381 | -5% |
| side_hold_ms | 293.3 | 200 | -62% ⚠ |
| lean_fight | 0.6201 | 0.6743 | +11% |
| jumps_per_min | 3.829 | 3.963 | +3% |
| crouch_per_min | 9.64 | 9.781 | +1% |
| walk_hidden | 0 | 0.02229 | +21% |
| burst_median | 3.211 | 4 | +20% |
| aim_height_firing | 0.4919 | 0.472 | -11% |
| hold_angle | 0.3757 | 0.2486 | -41% ⚠ |
| aim_error_fight_deg | 5.803 | 7.792 | +81% ⚠ |
| reaction_ms | 133.2 | 250 | +117% ⚠ |
| mp40_share | 0.5841 | 1 | +42% ⚠ |

Style fingerprint (2026-10-02, 31.3 min, styles.py features 30/30): z-distance to the human cloud centre 9.0 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.2 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.113 (humans 0.152–0.491); lf_pure_strafe 0.788 (humans 0.362–0.752); lf_fwd_diag 0.0568 (humans 0.0658–0.568); ducked 0.099 (humans 0.0109–0.0741); walk 0.0109 (humans 0.0126–0.0881); p_attack_given_los 0.502 (humans 0.547–0.861); speed_med 126 (humans 136–200); lf_err_med 7.79 (humans 4.08–6.63); yaw_speed_p95 273 (humans 158–271); side_hold_p90_ms 550 (humans 700–950); lf_approach 0.113 (humans 0.131–0.64); lf_retreat 0.187 (humans 0.0243–0.141).

### `bot:stopper:5.2` (stopper, seed 2513918026, 31.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1336 | 0.09039 | -8% |
| reverse_share | 0.6777 | 0.6368 | -6% |
| side_hold_ms | 226 | 200 | -17% |
| lean_fight | 0.9588 | 0.9643 | +1% |
| jumps_per_min | 2.944 | 3.484 | +10% |
| crouch_per_min | 3.426 | 3.548 | +1% |
| walk_hidden | 0.08429 | 0.09351 | +9% |
| burst_median | 3 | 4 | +25% ⚠ |
| aim_height_firing | 0.4602 | 0.437 | -12% |
| hold_angle | 0.3824 | 0.2092 | -56% ⚠ |
| aim_error_fight_deg | 4.291 | 7.126 | +115% ⚠ |
| reaction_ms | 113 | 200 | +87% ⚠ |
| mp40_share | 0.07877 | 0 | -8% |

Style fingerprint (2026-10-02, 31.3 min, styles.py features 30/30): z-distance to the human cloud centre 8.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0983 (humans 0.152–0.491); neutral 0.355 (humans 0.0872–0.296); lean_any 0.868 (humans 0.358–0.767); lf_lean 0.964 (humans 0.457–0.948); speed_med 99.8 (humans 136–200); lf_err_med 7.13 (humans 4.08–6.63); side_hold_p90_ms 550 (humans 700–950); lf_retreat 0.151 (humans 0.0243–0.141).

### `bot:presser:4` (presser, seed 52844662, 36.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5317 | 0.2127 | -62% ⚠ |
| reverse_share | 0.8152 | 0.7918 | -3% |
| side_hold_ms | 336.6 | 200 | -91% ⚠ |
| lean_fight | 0.935 | 0.9828 | +10% |
| jumps_per_min | 0.336 | 0.4894 | +3% |
| crouch_per_min | 1.298 | 1.251 | -0% |
| walk_hidden | 0 | 0.01009 | +10% |
| burst_median | 6.634 | 8 | +34% ⚠ |
| aim_height_firing | 0.3853 | 0.301 | -45% ⚠ |
| hold_angle | 0.2402 | 0.2439 | +1% |
| aim_error_fight_deg | 4.424 | 9.005 | +186% ⚠ |
| reaction_ms | 173.4 | 250 | +77% ⚠ |
| mp40_share | 0.06486 | 0 | -6% |

Style fingerprint (2026-10-02, 36.8 min, styles.py features 30/30): z-distance to the human cloud centre 10.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.594 (humans 0.191–0.489); lean_any 0.922 (humans 0.358–0.767); lf_lean 0.983 (humans 0.457–0.948); walk 0.00317 (humans 0.0126–0.0881); attack 0.49 (humans 0.276–0.478); speed_med 124 (humans 136–200); still 0.0566 (humans 0.0832–0.227); lf_err_med 9 (humans 4.08–6.63); lf_height_frac 0.301 (humans 0.352–0.54); yaw_speed_p95 337 (humans 158–271); yaw_speed_p99 769 (humans 427–754); side_hold_p90_ms 400 (humans 700–950); lf_retreat 0.156 (humans 0.0243–0.141).

### `bot:strafer:5.2` (strafer, seed 3859071546, 36.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1614 | 0.05909 | -20% |
| reverse_share | 0.5449 | 0.5233 | -3% |
| side_hold_ms | 350 | 200 | -100% ⚠ |
| lean_fight | 0.5666 | 0.762 | +39% ⚠ |
| jumps_per_min | 0.6862 | 0.5981 | -2% |
| crouch_per_min | 3.8 | 3.779 | -0% |
| walk_hidden | 0.04377 | 0.05545 | +11% |
| burst_median | 6.798 | 10 | +80% ⚠ |
| aim_height_firing | 0.4661 | 0.397 | -37% ⚠ |
| hold_angle | 0.2527 | 0.3279 | +24% |
| aim_error_fight_deg | 4.952 | 8.053 | +126% ⚠ |
| reaction_ms | 115 | 200 | +85% ⚠ |
| mp40_share | 0.03683 | 0 | -4% |

Style fingerprint (2026-10-02, 36.8 min, styles.py features 30/30): z-distance to the human cloud centre 9.6 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.593 (humans 0.191–0.489); fwd_diag 0.0872 (humans 0.152–0.491); lf_pure_strafe 0.804 (humans 0.362–0.752); lf_fwd_diag 0.0591 (humans 0.0658–0.568); attack 0.518 (humans 0.276–0.478); speed_med 124 (humans 136–200); still 0.0744 (humans 0.0832–0.227); lf_err_med 8.05 (humans 4.08–6.63); yaw_speed_p95 304 (humans 158–271); side_hold_p90_ms 500 (humans 700–950); lf_approach 0.125 (humans 0.131–0.64); lf_retreat 0.208 (humans 0.0243–0.141).

### `bot:presser:4.2` (presser, seed 588158740, 34.4 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4812 | 0.1527 | -64% ⚠ |
| reverse_share | 0.8389 | 0.8134 | -4% |
| side_hold_ms | 302.9 | 200 | -69% ⚠ |
| lean_fight | 0.9588 | 0.9866 | +6% |
| jumps_per_min | 1.077 | 0.9294 | -3% |
| crouch_per_min | 1.47 | 0.668 | -7% |
| walk_hidden | 0.004612 | 0.01723 | +12% |
| burst_median | 6.555 | 9 | +61% ⚠ |
| aim_height_firing | 0.352 | 0.3175 | -18% |
| hold_angle | 0.2402 | 0.2241 | -5% |
| aim_error_fight_deg | 4.382 | 6.096 | +70% ⚠ |
| reaction_ms | 183.1 | 250 | +67% ⚠ |
| mp40_share | 0.9584 | 1 | +4% |

Style fingerprint (2026-10-02, 34.4 min, styles.py features 30/30): z-distance to the human cloud centre 8.5 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.541 (humans 0.191–0.489); lf_pure_strafe 0.771 (humans 0.362–0.752); lean_any 0.91 (humans 0.358–0.767); lf_lean 0.987 (humans 0.457–0.948); walk 0.00605 (humans 0.0126–0.0881); p_attack_given_los 0.512 (humans 0.547–0.861); speed_med 122 (humans 136–200); still 0.0795 (humans 0.0832–0.227); lf_on_target 0.237 (humans 0.274–0.515); lf_height_frac 0.318 (humans 0.352–0.54); yaw_speed_p95 287 (humans 158–271); side_hold_p90_ms 450 (humans 700–950).

### `bot:presser:5.2` (presser, seed 4131749178, 34.4 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5433 | 0.1514 | -76% ⚠ |
| reverse_share | 0.823 | 0.8053 | -3% |
| side_hold_ms | 298.6 | 200 | -66% ⚠ |
| lean_fight | 0.9579 | 0.9821 | +5% |
| jumps_per_min | 0.5292 | 0.7551 | +4% |
| crouch_per_min | 2.785 | 3.137 | +3% |
| walk_hidden | 0.00312 | 0.01619 | +12% |
| burst_median | 6.511 | 9 | +62% ⚠ |
| aim_height_firing | 0.4126 | 0.316 | -51% ⚠ |
| hold_angle | 0.2204 | 0.3012 | +26% ⚠ |
| aim_error_fight_deg | 4.873 | 5.976 | +45% ⚠ |
| reaction_ms | 134.4 | 300 | +166% ⚠ |
| mp40_share | 0.08844 | 0 | -9% |

Style fingerprint (2026-10-02, 34.4 min, styles.py features 30/30): z-distance to the human cloud centre 8.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.7 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.533 (humans 0.191–0.489); lean_any 0.916 (humans 0.358–0.767); lf_lean 0.982 (humans 0.457–0.948); walk 0.00566 (humans 0.0126–0.0881); speed_med 124 (humans 136–200); still 0.0773 (humans 0.0832–0.227); lf_on_target 0.274 (humans 0.274–0.515); lf_height_frac 0.316 (humans 0.352–0.54); yaw_speed_p95 288 (humans 158–271); side_hold_p90_ms 450 (humans 700–950); lf_retreat 0.143 (humans 0.0243–0.141).

### `bot:stopper:4.3` (stopper, seed 97502759, 33.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.05962 | -1% |
| reverse_share | 0.6408 | 0.6059 | -5% |
| side_hold_ms | 200 | 200 | +0% |
| lean_fight | 0.8757 | 0.8993 | +5% |
| jumps_per_min | 1.606 | 1.84 | +4% |
| crouch_per_min | 2.514 | 2.167 | -3% |
| walk_hidden | 0.09232 | 0.08615 | -6% |
| burst_median | 3 | 4 | +25% ⚠ |
| aim_height_firing | 0.4804 | 0.464 | -9% |
| hold_angle | 0.418 | 0.344 | -24% |
| aim_error_fight_deg | 5.509 | 8.735 | +131% ⚠ |
| reaction_ms | 127.6 | 250 | +122% ⚠ |
| mp40_share | 0.1179 | 0 | -12% |

Style fingerprint (2026-10-02, 33.7 min, styles.py features 30/30): z-distance to the human cloud centre 9.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0755 (humans 0.152–0.491); neutral 0.373 (humans 0.0872–0.296); lf_pure_strafe 0.757 (humans 0.362–0.752); lf_fwd_diag 0.0596 (humans 0.0658–0.568); p_attack_given_los 0.519 (humans 0.547–0.861); speed_med 96.7 (humans 136–200); lf_err_med 8.73 (humans 4.08–6.63); side_hold_p90_ms 450 (humans 700–950); lf_approach 0.117 (humans 0.131–0.64); lf_retreat 0.2 (humans 0.0243–0.141).

### `bot:strafer:5.3` (strafer, seed 2592015535, 33.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.119 | 0.03563 | -16% |
| reverse_share | 0.4951 | 0.4504 | -6% |
| side_hold_ms | 295 | 200 | -63% ⚠ |
| lean_fight | 0.4565 | 0.4829 | +5% |
| jumps_per_min | 5.657 | 5.728 | +1% |
| crouch_per_min | 11.59 | 11.28 | -3% |
| walk_hidden | 0 | 0.01299 | +12% |
| burst_median | 3.16 | 4 | +21% |
| aim_height_firing | 0.54 | 0.518 | -12% |
| hold_angle | 0.3001 | 0.3162 | +5% |
| aim_error_fight_deg | 6.024 | 9.178 | +128% ⚠ |
| reaction_ms | 165.9 | 275 | +109% ⚠ |
| mp40_share | 0.4858 | 1 | +51% ⚠ |

Style fingerprint (2026-10-02, 33.7 min, styles.py features 30/30): z-distance to the human cloud centre 11.3 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.111 (humans 0.152–0.491); lf_pure_strafe 0.786 (humans 0.362–0.752); lf_fwd_diag 0.0356 (humans 0.0658–0.568); lean_any 0.307 (humans 0.358–0.767); ducked 0.137 (humans 0.0109–0.0741); walk 0.00678 (humans 0.0126–0.0881); attack 0.256 (humans 0.276–0.478); p_attack_given_los 0.417 (humans 0.547–0.861); speed_med 117 (humans 136–200); lf_err_med 9.18 (humans 4.08–6.63); yaw_speed_p95 276 (humans 158–271); side_hold_p90_ms 500 (humans 700–950); lf_approach 0.111 (humans 0.131–0.64); lf_retreat 0.199 (humans 0.0243–0.141).

### `bot:strafer:4.3` (strafer, seed 1086809828, 32.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1632 | 0.06321 | -20% |
| reverse_share | 0.4593 | 0.41 | -7% |
| side_hold_ms | 314.9 | 250 | -43% ⚠ |
| lean_fight | 0.5599 | 0.5981 | +8% |
| jumps_per_min | 4.813 | 5.3 | +9% |
| crouch_per_min | 9.497 | 8.885 | -6% |
| walk_hidden | 0 | 0.0171 | +16% |
| burst_median | 4.589 | 6 | +35% ⚠ |
| aim_height_firing | 0.4741 | 0.45 | -13% |
| hold_angle | 0.3025 | 0.2609 | -14% |
| aim_error_fight_deg | 6.527 | 9.736 | +131% ⚠ |
| reaction_ms | 158.4 | 250 | +92% ⚠ |
| mp40_share | 0.9804 | 1 | +2% |

Style fingerprint (2026-10-02, 32.6 min, styles.py features 30/30): z-distance to the human cloud centre 10.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.124 (humans 0.152–0.491); lf_pure_strafe 0.775 (humans 0.362–0.752); lf_fwd_diag 0.0632 (humans 0.0658–0.568); ducked 0.109 (humans 0.0109–0.0741); walk 0.00756 (humans 0.0126–0.0881); p_attack_given_los 0.489 (humans 0.547–0.861); speed_med 125 (humans 136–200); still 0.0772 (humans 0.0832–0.227); lf_err_med 9.74 (humans 4.08–6.63); yaw_speed_p95 302 (humans 158–271); side_hold_p90_ms 500 (humans 700–950); lf_retreat 0.185 (humans 0.0243–0.141).

### `bot:strafer:5.4` (strafer, seed 821368291, 32.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.166 | 0.0936 | -14% |
| reverse_share | 0.4801 | 0.4379 | -6% |
| side_hold_ms | 349.1 | 250 | -66% ⚠ |
| lean_fight | 0.6654 | 0.7015 | +7% |
| jumps_per_min | 5.052 | 4.412 | -12% |
| crouch_per_min | 10.37 | 10.14 | -2% |
| walk_hidden | 0.0009382 | 0.01738 | +16% |
| burst_median | 4.755 | 6 | +31% ⚠ |
| aim_height_firing | 0.4434 | 0.408 | -19% |
| hold_angle | 0.3191 | 0.2517 | -22% |
| aim_error_fight_deg | 4.261 | 8.926 | +190% ⚠ |
| reaction_ms | 175 | 250 | +75% ⚠ |
| mp40_share | 0.1202 | 0 | -12% |

Style fingerprint (2026-10-02, 32.6 min, styles.py features 30/30): z-distance to the human cloud centre 9.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.7 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.494 (humans 0.191–0.489); fwd_diag 0.129 (humans 0.152–0.491); lf_pure_strafe 0.759 (humans 0.362–0.752); ducked 0.101 (humans 0.0109–0.0741); walk 0.00768 (humans 0.0126–0.0881); p_attack_given_los 0.522 (humans 0.547–0.861); speed_med 128 (humans 136–200); still 0.067 (humans 0.0832–0.227); lf_err_med 8.93 (humans 4.08–6.63); side_hold_p90_ms 500 (humans 700–950); lf_retreat 0.182 (humans 0.0243–0.141).

## The owner against the bots (`pstN@vsbot`)

Kills of bots 0, deaths to bots 0, suicides 0. 

0 statistics of the owner against bots fall outside the human CI (owner column below, marked •).

## All statistics

Pooled bots are reweighted to the human family mix; humans and the owner are pooled over their duel time. ▲ marks a tell. n = units (ticks, runs, shots, sightings, ...) over the blocks.

### Movement

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time: hidden, not firing ▲ | 41.5% [37.7%, 45.1%] | 49.1% [47.4%, 51.2%] | - | 649580 |
| Share of duel time: hidden, firing ▲ | 22.3% [19.0%, 25.3%] | 15.8% [14.9%, 17.0%] | - | 649580 |
| Share of duel time: LOS, not firing | 10.7% [3.3%, 20.2%] | 7.0% [5.5%, 8.4%] | - | 649580 |
| Share of duel time: LOS firefight ▲ | 8.7% [7.0%, 10.9%] | 17.6% [16.5%, 18.9%] | - | 649580 |
| Share of duel time: reloading ▲ | 16.8% [14.7%, 18.9%] | 10.5% [9.6%, 11.5%] | - | 649580 |
| Key chord pure strafe (hidden, not firing) ▲ | 33.7% [31.3%, 35.9%] | 26.0% [23.5%, 29.4%] | - | 258150 |
| Key chord fwd diag (hidden, not firing) ▲ | 9.2% [8.2%, 10.1%] | 29.9% [26.7%, 33.2%] | - | 258150 |
| Key chord forward (hidden, not firing) ▲ | 8.7% [8.1%, 9.6%] | 11.9% [10.9%, 12.8%] | - | 258150 |
| Key chord neutral (hidden, not firing) ▲ | 40.0% [37.3%, 43.0%] | 26.1% [23.1%, 28.6%] | - | 258150 |
| Key chord back any (hidden, not firing) ▲ | 8.3% [8.0%, 8.7%] | 6.1% [5.3%, 7.1%] | - | 258150 |
| Key chord pure strafe (hidden, firing) ▲ | 57.7% [53.6%, 60.8%] | 44.9% [41.6%, 48.0%] | - | 146850 |
| Key chord fwd diag (hidden, firing) ▲ | 10.7% [9.6%, 11.7%] | 28.6% [23.9%, 32.8%] | - | 146850 |
| Key chord forward (hidden, firing) ▲ | 3.9% [3.1%, 4.8%] | 6.6% [5.7%, 7.4%] | - | 146850 |
| Key chord neutral (hidden, firing) ▲ | 18.7% [16.1%, 22.3%] | 13.2% [11.2%, 15.7%] | - | 146850 |
| Key chord back any (hidden, firing) ▲ | 9.0% [8.6%, 9.5%] | 6.7% [5.9%, 7.9%] | - | 146850 |
| Key chord pure strafe (LOS, not firing) | 18.6% [13.9%, 41.7%] | 26.7% [20.8%, 34.2%] | - | 79433 |
| Key chord fwd diag (LOS, not firing) ▲ | 5.0% [3.0%, 11.2%] | 29.4% [23.0%, 38.1%] | - | 79433 |
| Key chord forward (LOS, not firing) | 3.0% [2.0%, 7.9%] | 7.5% [5.9%, 9.5%] | - | 79433 |
| Key chord neutral (LOS, not firing) | 69.1% [28.6%, 78.0%] | 31.5% [15.3%, 45.4%] | - | 79433 |
| Key chord back any (LOS, not firing) | 4.4% [3.0%, 11.5%] | 4.8% [3.4%, 6.7%] | - | 79433 |
| Key chord pure strafe (LOS firefight) ▲ | 73.0% [70.3%, 75.7%] | 58.0% [53.1%, 62.2%] | - | 57138 |
| Key chord fwd diag (LOS firefight) ▲ | 9.3% [7.8%, 10.8%] | 30.5% [25.6%, 36.7%] | - | 57138 |
| Key chord forward (LOS firefight) | 1.0% [0.7%, 1.3%] | 1.2% [1.0%, 1.4%] | - | 57138 |
| Key chord neutral (LOS firefight) ▲ | 8.9% [7.1%, 11.1%] | 5.1% [3.9%, 6.3%] | - | 57138 |
| Key chord back any (LOS firefight) ▲ | 7.8% [6.9%, 9.2%] | 5.2% [4.5%, 6.1%] | - | 57138 |
| Key chord pure strafe (reloading) ▲ | 29.1% [26.5%, 31.3%] | 22.3% [19.6%, 24.4%] | - | 108020 |
| Key chord fwd diag (reloading) ▲ | 15.9% [13.7%, 17.6%] | 36.8% [34.1%, 39.9%] | - | 108020 |
| Key chord forward (reloading) ▲ | 14.3% [13.1%, 15.5%] | 26.1% [23.8%, 28.6%] | - | 108020 |
| Key chord neutral (reloading) ▲ | 28.2% [25.7%, 31.7%] | 8.3% [7.3%, 9.5%] | - | 108020 |
| Key chord back any (reloading) ▲ | 12.5% [12.1%, 13.0%] | 6.5% [5.3%, 7.6%] | - | 108020 |
| Strafe end is a direct reverse (hidden, not firing) | 47.2% [39.7%, 52.9%] | 46.1% [41.1%, 51.2%] | - | 28011 |
| Strafe end is a direct reverse (hidden, firing) | 61.6% [52.9%, 67.3%] | 67.2% [59.7%, 74.3%] | - | 22439 |
| Strafe end is a direct reverse (LOS, not firing) | 55.4% [42.6%, 64.6%] | 52.8% [45.1%, 60.4%] | - | 4636 |
| Strafe end is a direct reverse (LOS firefight) | 62.3% [48.9%, 72.5%] | 73.3% [65.9%, 80.6%] | - | 9253 |
| Strafe end is a direct reverse (reloading) | 44.0% [36.2%, 49.6%] | 37.7% [32.5%, 42.7%] | - | 12076 |
| Strafe switch hazard per tick at hold age 50 ms (LOS firefight) ▲ | 9.6% [7.9%, 11.7%] | 1.2% [0.9%, 1.6%] | - | 9946 |
| Strafe switch hazard per tick at hold age 100 ms (LOS firefight) ▲ | 13.3% [11.9%, 14.9%] | 5.8% [5.2%, 6.8%] | - | 8923 |
| Strafe switch hazard per tick at hold age 150 ms (LOS firefight) ▲ | 17.1% [15.2%, 19.7%] | 12.4% [11.1%, 13.9%] | - | 7731 |
| Strafe switch hazard per tick at hold age 200 ms (LOS firefight) ▲ | 24.2% [21.7%, 26.2%] | 17.0% [15.4%, 18.7%] | - | 6295 |
| Strafe switch hazard per tick at hold age 250 ms (LOS firefight) ▲ | 25.7% [24.1%, 27.3%] | 17.4% [16.0%, 19.0%] | - | 4609 |
| Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) ▲ | 23.7% [21.9%, 25.1%] | 15.6% [14.7%, 16.9%] | - | 9120 |
| Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) ▲ | 22.9% [21.0%, 24.4%] | 16.1% [14.9%, 17.5%] | - | 4353 |
| Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) ▲ | 23.7% [20.8%, 28.3%] | 13.7% [12.0%, 15.6%] | - | 358 |
| Side-hold duration (LOS firefight) p25 ▲ | 150 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | - | 9203 |
| Side-hold duration (LOS firefight) p50 ▲ | 200 ms [200 ms, 200 ms] | 300 ms [250 ms, 300 ms] | - | 9203 |
| Side-hold duration (LOS firefight) p75 ▲ | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | - | 9203 |
| Side-hold duration (LOS firefight) p90 ▲ | 500 ms [450 ms, 500 ms] | 700 ms [650 ms, 700 ms] | - | 9203 |
| Side-hold duration (all contexts) p50 ▲ | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | - | 76022 |
| Side-hold duration (all contexts) p90 ▲ | 500 ms [500 ms, 500 ms] | 800 ms [800 ms, 850 ms] | - | 76022 |
| Neutral gap between strafes (LOS firefight) p50 | 50 ms [50 ms, 50 ms] | 50 ms [50 ms, 50 ms] | - | 3143 |
| Neutral gap between strafes (LOS firefight) p90 | 100 ms [50 ms, 100 ms] | 100 ms [100 ms, 150 ms] | - | 3143 |
| Approaching >40 u/s in LOS firefight at distance (0, 96] ▲ | 22.1% [17.4%, 26.9%] | 37.6% [33.7%, 42.3%] | - | 6274 |
| Approaching >40 u/s in LOS firefight at distance (96, 160] ▲ | 15.8% [13.7%, 17.7%] | 36.4% [32.6%, 41.2%] | - | 8953 |
| Approaching >40 u/s in LOS firefight at distance (160, 224] ▲ | 13.1% [12.1%, 14.2%] | 35.4% [30.1%, 42.0%] | - | 12253 |
| Approaching >40 u/s in LOS firefight at distance (224, 288] ▲ | 12.9% [11.2%, 14.6%] | 38.2% [32.3%, 46.4%] | - | 9146 |
| Approaching >40 u/s in LOS firefight at distance (288, 384] ▲ | 13.6% [11.6%, 15.5%] | 39.4% [32.7%, 47.2%] | - | 9145 |
| Approaching >40 u/s in LOS firefight at distance (384, 512] ▲ | 15.1% [13.4%, 17.0%] | 41.8% [33.8%, 49.1%] | - | 5826 |
| Approaching >40 u/s in LOS firefight at distance (512, 768] ▲ | 14.3% [12.7%, 19.0%] | 33.1% [25.5%, 41.5%] | - | 4942 |
| Approaching >40 u/s in LOS firefight at distance (768, 1200] | 8.2% [6.8%, 29.5%] | 28.2% [17.6%, 42.7%] | - | 599 |
| Retreating >40 u/s in LOS firefight at distance (0, 96] ▲ | 29.8% [25.8%, 33.8%] | 22.2% [18.9%, 25.6%] | - | 6274 |
| Retreating >40 u/s in LOS firefight at distance (96, 160] ▲ | 22.7% [20.9%, 24.8%] | 10.5% [8.5%, 12.6%] | - | 8953 |
| Retreating >40 u/s in LOS firefight at distance (160, 224] ▲ | 18.1% [17.0%, 19.6%] | 6.9% [5.5%, 8.7%] | - | 12253 |
| Retreating >40 u/s in LOS firefight at distance (224, 288] ▲ | 15.1% [12.6%, 19.6%] | 5.3% [4.2%, 6.8%] | - | 9146 |
| Retreating >40 u/s in LOS firefight at distance (288, 384] ▲ | 13.6% [12.0%, 15.9%] | 5.9% [4.6%, 7.3%] | - | 9145 |
| Retreating >40 u/s in LOS firefight at distance (384, 512] ▲ | 11.7% [9.8%, 14.4%] | 4.8% [3.8%, 6.0%] | - | 5826 |
| Retreating >40 u/s in LOS firefight at distance (512, 768] ▲ | 11.9% [8.2%, 12.9%] | 4.0% [3.0%, 5.4%] | - | 4942 |
| Retreating >40 u/s in LOS firefight at distance (768, 1200] | 18.6% [10.2%, 24.0%] | 8.3% [1.5%, 15.5%] | - | 599 |
| Standing still (<5 u/s), all duel time | 20.1% [13.8%, 27.7%] | 15.7% [13.4%, 17.7%] | - | 649580 |
| Standing still (<5 u/s) (hidden, not firing) | 23.3% [21.6%, 25.7%] | 22.5% [19.5%, 25.1%] | - | 258150 |
| Standing still (<5 u/s) (hidden, firing) | 7.9% [6.7%, 9.3%] | 10.2% [9.1%, 11.7%] | - | 146850 |
| Standing still (<5 u/s) (LOS, not firing) | 62.0% [13.7%, 71.9%] | 29.6% [12.6%, 44.0%] | - | 79433 |
| Standing still (<5 u/s) (LOS firefight) | 2.5% [2.2%, 2.9%] | 2.3% [1.8%, 2.7%] | - | 57138 |
| Standing still (<5 u/s) (reloading) ▲ | 10.8% [9.5%, 12.5%] | 5.2% [4.3%, 6.2%] | - | 108020 |
| Lean held, all duel time | 60.0% [52.0%, 67.4%] | 59.7% [55.3%, 64.0%] | - | 649580 |
| Lean held in LOS firefights | 82.9% [79.0%, 85.8%] | 81.3% [76.6%, 85.8%] | - | 57138 |
| Jump presses per minute | 2.80/min [1.92/min, 3.67/min] | 1.87/min [1.41/min, 2.44/min] | - | 649580 |
| Crouch presses per minute | 5.86/min [4.32/min, 7.39/min] | 4.33/min [3.22/min, 5.51/min] | - | 649580 |
| Walk presses per minute ▲ | 3.32/min [2.64/min, 3.82/min] | 7.54/min [6.90/min, 8.11/min] | - | 649580 |
| Walking while hidden and not firing | 5.0% [3.3%, 6.2%] | 5.9% [5.0%, 7.0%] | - | 258150 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Press hazard with LOS and aim within 3 half-widths, LOS gained 0 ms ago | 20.9% [15.6%, 25.9%] | 28.2% [25.8%, 30.8%] | - | 713 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 50-100 ms ago | 29.4% [25.7%, 33.8%] | 33.0% [30.8%, 35.6%] | - | 1055 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 150-250 ms ago | 35.5% [31.7%, 39.9%] | 35.2% [32.9%, 38.1%] | - | 951 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago | 30.8% [27.0%, 35.6%] | 28.0% [25.2%, 32.0%] | - | 689 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 550-1000 ms ago | 23.3% [18.7%, 30.0%] | 21.0% [16.7%, 24.6%] | - | 565 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago ▲ | 16.4% [12.5%, 21.5%] | 7.4% [4.7%, 10.8%] | - | 1190 |
| Press hazard while hidden, LOS lost 0 ms ago | 12.3% [10.2%, 14.8%] | 12.3% [10.8%, 14.5%] | - | 1285 |
| Press hazard while hidden, LOS lost 50-100 ms ago | 9.9% [8.4%, 11.6%] | 9.5% [8.5%, 10.9%] | - | 1890 |
| Press hazard while hidden, LOS lost 150-250 ms ago ▲ | 8.1% [6.8%, 9.4%] | 4.6% [3.9%, 5.5%] | - | 2483 |
| Press hazard while hidden, LOS lost 300-500 ms ago ▲ | 10.5% [10.0%, 11.1%] | 7.0% [6.0%, 8.1%] | - | 7851 |
| Press hazard while hidden, LOS lost 550-1000 ms ago ▲ | 5.2% [4.8%, 5.7%] | 3.5% [3.2%, 3.9%] | - | 15299 |
| Press hazard while hidden, LOS lost over 1 s ago | 4.2% [3.9%, 4.5%] | 4.1% [3.5%, 4.6%] | - | 215430 |
| Release hazard, LOS, aim error in half-widths (0, 1] ▲ | 0.5% [0.3%, 0.7%] | 1.4% [1.1%, 1.7%] | - | 9321 |
| Release hazard, LOS, aim error in half-widths (1, 2] ▲ | 0.7% [0.5%, 0.9%] | 1.5% [1.3%, 1.9%] | - | 17736 |
| Release hazard, LOS, aim error in half-widths (2, 3] ▲ | 0.6% [0.5%, 0.8%] | 1.5% [1.2%, 2.0%] | - | 12449 |
| Release hazard, LOS, aim error in half-widths (3, 4] ▲ | 0.5% [0.3%, 0.7%] | 1.4% [1.0%, 2.0%] | - | 6881 |
| Release hazard, LOS, aim error in half-widths (4, 6] | 1.2% [0.7%, 1.6%] | 1.7% [1.3%, 2.3%] | - | 5204 |
| Release hazard, LOS, aim error in half-widths (6, 10] | 3.6% [2.7%, 4.6%] | 6.9% [4.4%, 9.9%] | - | 2188 |
| Release hazard, LOS, aim error in half-widths (10, 1000] ▲ | 7.5% [6.0%, 9.5%] | 32.7% [22.9%, 41.8%] | - | 2448 |
| Release hazard while hidden, LOS lost 0-100 ms ago ▲ | 2.3% [1.6%, 3.3%] | 6.3% [5.4%, 7.5%] | - | 13566 |
| Release hazard while hidden, LOS lost 150-250 ms ago ▲ | 5.8% [4.1%, 7.7%] | 16.5% [13.8%, 18.7%] | - | 7890 |
| Release hazard while hidden, LOS lost 300-450 ms ago ▲ | 15.5% [12.3%, 18.9%] | 31.5% [29.6%, 33.4%] | - | 7974 |
| Release hazard while hidden, LOS lost 500-950 ms ago ▲ | 6.3% [5.0%, 7.5%] | 10.1% [9.2%, 11.3%] | - | 11173 |
| Release hazard while hidden, LOS lost 1 s or more ago ▲ | 7.7% [6.8%, 8.8%] | 11.3% [9.8%, 13.5%] | - | 103950 |
| Fire held, LOS, aim error in half-widths (0, 0.5] | 90.1% [85.5%, 93.3%] | 89.6% [87.0%, 91.7%] | - | 2642 |
| Fire held, LOS, aim error in half-widths (0.5, 1] | 89.6% [85.3%, 92.9%] | 88.9% [86.9%, 90.5%] | - | 7870 |
| Fire held, LOS, aim error in half-widths (1, 1.5] | 88.8% [84.7%, 92.1%] | 86.3% [84.5%, 87.8%] | - | 10170 |
| Fire held, LOS, aim error in half-widths (1.5, 2] ▲ | 88.5% [85.1%, 91.7%] | 81.9% [79.8%, 83.7%] | - | 9965 |
| Fire held, LOS, aim error in half-widths (2, 3] ▲ | 86.8% [83.5%, 90.0%] | 81.0% [79.4%, 82.7%] | - | 14393 |
| Fire held, LOS, aim error in half-widths (3, 4] | 82.6% [78.9%, 86.4%] | 78.8% [76.1%, 81.2%] | - | 8349 |
| Fire held, LOS, aim error in half-widths (4, 6] | 72.6% [68.3%, 76.4%] | 67.7% [63.0%, 71.3%] | - | 7243 |
| Fire held, LOS, aim error in half-widths (6, 10] ▲ | 48.0% [44.5%, 53.1%] | 22.4% [17.1%, 27.1%] | - | 4587 |
| Fire held, LOS, aim error in half-widths (10, 20] ▲ | 25.2% [19.0%, 31.6%] | 4.6% [2.4%, 6.9%] | - | 4152 |
| Fire held, LOS, aim error in half-widths (20, 1000] ▲ | 18.7% [13.9%, 23.0%] | 0.9% [0.4%, 3.1%] | - | 6638 |
| Fire held without LOS ▲ | 35.8% [32.4%, 39.6%] | 24.5% [22.7%, 26.3%] | - | 388880 |
| Attack hold duration p50 ▲ | 350 ms [350 ms, 400 ms] | 150 ms [150 ms, 200 ms] | - | 12782 |
| Attack hold duration p90 ▲ | 1400 ms [1200 ms, 1650 ms] | 900 ms [800 ms, 950 ms] | - | 12782 |
| Attack holds that are taps (<=100 ms) ▲ | 24.3% [23.1%, 25.9%] | 44.3% [41.8%, 46.5%] | - | 12782 |
| Gap between attack holds p50 | 400 ms [400 ms, 450 ms] | 400 ms [350 ms, 400 ms] | - | 12378 |

### Aim

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error firing with LOS at (0, 128] u ▲ | 20.33° [18.03°, 21.87°] | 14.28° [13.53°, 15.19°] | - | 8339 |
| Crosshair on body firing with LOS at (0, 128] u ▲ | 40.7% [38.6%, 43.2%] | 48.0% [44.9%, 51.5%] | - | 8339 |
| Median aim error firing with LOS at (128, 192] u ▲ | 11.04° [10.68°, 11.38°] | 8.99° [8.66°, 9.45°] | - | 9328 |
| Crosshair on body firing with LOS at (128, 192] u ▲ | 35.3% [33.7%, 36.6%] | 42.5% [39.8%, 44.7%] | - | 9328 |
| Median aim error firing with LOS at (192, 256] u ▲ | 8.07° [7.81°, 8.40°] | 6.68° [6.36°, 7.10°] | - | 9581 |
| Crosshair on body firing with LOS at (192, 256] u ▲ | 32.1% [30.1%, 33.5%] | 42.0% [39.5%, 44.4%] | - | 9581 |
| Median aim error firing with LOS at (256, 384] u | 5.65° [5.42°, 5.95°] | 5.23° [4.96°, 5.59°] | - | 15352 |
| Crosshair on body firing with LOS at (256, 384] u ▲ | 29.5% [27.3%, 31.7%] | 39.0% [36.0%, 41.4%] | - | 15352 |
| Median aim error firing with LOS at (384, 512] u | 4.15° [3.87°, 4.57°] | 3.92° [3.67°, 4.18°] | - | 8160 |
| Crosshair on body firing with LOS at (384, 512] u ▲ | 28.8% [25.1%, 31.8%] | 35.4% [32.6%, 38.3%] | - | 8160 |
| Median aim error firing with LOS at (512, 768] u | 3.07° [2.72°, 3.49°] | 3.07° [2.80°, 3.35°] | - | 5780 |
| Crosshair on body firing with LOS at (512, 768] u | 27.1% [23.5%, 30.5%] | 32.0% [29.1%, 35.1%] | - | 5780 |
| Median aim error firing with LOS at (768, 1200] u | 2.34° [1.94°, 2.74°] | 2.14° [1.84°, 2.58°] | - | 599 |
| Crosshair on body firing with LOS at (768, 1200] u | 29.3% [21.8%, 36.1%] | 31.5% [24.3%, 39.5%] | - | 599 |
| Aim height (fraction of body) firing with LOS | 0.414 [0.4, 0.434] | 0.446 [0.418, 0.468] | - | 57139 |
| Aim height (fraction of body) with LOS, not firing | 0.578 [0.57, 0.631] | 0.662 [0.576, 0.751] | - | 103510 |

### View

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw speed (hidden, not firing) p50 | 11°/s [10°/s, 12°/s] | 8°/s [6°/s, 10°/s] | - | 257200 |
| Yaw speed (hidden, not firing) p99 ▲ | 646°/s [629°/s, 665°/s] | 544°/s [521°/s, 580°/s] | - | 257200 |
| Mouse still between ticks (hidden, not firing) | 33.2% [32.7%, 34.0%] | 32.1% [28.8%, 36.0%] | - | 258150 |
| Yaw speed (hidden, firing) p50 | 26°/s [23°/s, 28°/s] | 23°/s [21°/s, 25°/s] | - | 146850 |
| Yaw speed (hidden, firing) p99 ▲ | 472°/s [439°/s, 499°/s] | 194°/s [182°/s, 204°/s] | - | 146850 |
| Mouse still between ticks (hidden, firing) ▲ | 6.8% [6.2%, 7.4%] | 10.9% [10.1%, 11.8%] | - | 146850 |
| Yaw speed (LOS, not firing) p50 | 0°/s [0°/s, 35°/s] | 11°/s [0°/s, 26°/s] | - | 79431 |
| Yaw speed (LOS, not firing) p99 | 558°/s [438°/s, 843°/s] | 583°/s [505°/s, 695°/s] | - | 79431 |
| Mouse still between ticks (LOS, not firing) | 66.0% [23.4%, 75.1%] | 36.7% [20.8%, 49.8%] | - | 79433 |
| Yaw speed (LOS firefight) p50 ▲ | 53°/s [46°/s, 61°/s] | 39°/s [36°/s, 41°/s] | - | 57138 |
| Yaw speed (LOS firefight) p99 ▲ | 711°/s [585°/s, 805°/s] | 305°/s [292°/s, 318°/s] | - | 57138 |
| Mouse still between ticks (LOS firefight) ▲ | 2.7% [2.4%, 3.0%] | 1.6% [1.4%, 1.8%] | - | 57138 |
| Yaw speed (reloading) p50 | 16°/s [14°/s, 19°/s] | 18°/s [16°/s, 20°/s] | - | 108020 |
| Yaw speed (reloading) p99 ▲ | 552°/s [505°/s, 594°/s] | 945°/s [909°/s, 991°/s] | - | 108020 |
| Mouse still between ticks (reloading) | 25.6% [24.9%, 26.2%] | 26.3% [25.0%, 27.6%] | - | 108020 |
| Peak yaw speed of 90-135 deg turns p50 | 643°/s [626°/s, 667°/s] | 619°/s [595°/s, 655°/s] | - | 2350 |
| Peak yaw speed of 135-180 deg turns p50 | 867°/s [840°/s, 889°/s] | 852°/s [828°/s, 887°/s] | - | 1142 |
| Peak yaw speed of 180-360 deg turns p50 | 1134°/s [1098°/s, 1176°/s] | 1061°/s [1020°/s, 1126°/s] | - | 275 |

### Belief

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw error to the hidden opponent, last seen <=250 ms | 5.79° [5.17°, 6.42°] | 4.92° [4.56°, 5.26°] | - | 30912 |
| View within 30 deg of the hidden opponent, last seen <=250 ms ▲ | 89.6% [88.5%, 91.0%] | 92.7% [91.9%, 93.7%] | - | 30912 |
| Yaw error to the hidden opponent, last seen 250-500 ms | 6.30° [5.50°, 7.03°] | 6.66° [5.99°, 7.28°] | - | 17861 |
| View within 30 deg of the hidden opponent, last seen 250-500 ms | 90.5% [89.3%, 92.0%] | 92.2% [91.3%, 93.4%] | - | 17861 |
| Yaw error to the hidden opponent, last seen 0.5-1 s | 8.66° [7.61°, 9.68°] | 8.28° [7.48°, 9.11°] | - | 24360 |
| View within 30 deg of the hidden opponent, last seen 0.5-1 s | 87.4% [86.2%, 89.1%] | 88.0% [86.8%, 89.9%] | - | 24360 |
| Yaw error to the hidden opponent, last seen 1-2 s | 11.21° [9.46°, 13.12°] | 11.11° [9.95°, 12.13°] | - | 32121 |
| View within 30 deg of the hidden opponent, last seen 1-2 s | 80.6% [78.3%, 82.8%] | 78.9% [77.1%, 81.1%] | - | 32121 |
| Yaw error to the hidden opponent, last seen 2-4 s | 19.79° [16.36°, 23.91°] | 16.93° [15.18°, 19.08°] | - | 53539 |
| View within 30 deg of the hidden opponent, last seen 2-4 s | 61.3% [56.4%, 66.5%] | 64.3% [61.3%, 67.3%] | - | 53539 |
| Yaw error to the hidden opponent, last seen 4-8 s ▲ | 18.89° [15.62°, 22.06°] | 10.29° [9.19°, 11.80°] | - | 79201 |
| View within 30 deg of the hidden opponent, last seen 4-8 s ▲ | 62.1% [58.4%, 66.2%] | 76.4% [73.1%, 79.3%] | - | 79201 |
| Yaw error to the hidden opponent, last seen >8 s | 20.24° [16.55°, 23.30°] | 20.07° [12.82°, 29.16°] | - | 121300 |
| View within 30 deg of the hidden opponent, last seen >8 s | 61.0% [57.0%, 67.0%] | 65.1% [54.8%, 74.8%] | - | 121300 |
| Yaw error to the hidden opponent, last seen never seen this life | 17.72° [15.67°, 20.02°] | 19.23° [16.93°, 24.71°] | - | 129640 |
| View within 30 deg of the hidden opponent, last seen never seen this life | 64.6% [62.0%, 67.1%] | 62.5% [56.0%, 67.2%] | - | 129640 |

### Space

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) | 43.4% [30.6%, 74.6%] | 49.4% [44.0%, 50.8%] | - | 167310 |
| Share of duel time in the busiest 10% of 32u cells (dm/main) ▲ | 33.7% [30.0%, 34.5%] | 45.2% [40.5%, 48.6%] | - | 152160 |
| Share of duel time in the busiest 10% of 32u cells (dm/vents) | 38.7% [38.1%, 44.5%] | 44.3% [39.2%, 46.8%] | - | 170910 |
| Share of duel time in the busiest 10% of 32u cells (dm/downladder) ▲ | 37.5% [33.0%, 38.1%] | 61.4% [42.9%, 63.8%] | - | 159200 |

### Acquisition

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error at -500 ms from sight gain | 13.72° [11.60°, 15.50°] | 13.98° [12.90°, 15.10°] | - | 2912 |
| Median aim error at -200 ms from sight gain ▲ | 12.76° [10.60°, 14.48°] | 8.33° [7.70°, 8.92°] | - | 2914 |
| Median aim error at +0 ms from sight gain ▲ | 11.72° [9.59°, 13.45°] | 5.22° [4.88°, 5.61°] | - | 2917 |
| Median aim error at +100 ms from sight gain ▲ | 9.98° [8.22°, 11.68°] | 5.97° [5.54°, 6.43°] | - | 2917 |
| Median aim error at +200 ms from sight gain ▲ | 8.12° [6.70°, 9.56°] | 4.92° [4.54°, 5.23°] | - | 2917 |
| Median aim error at +300 ms from sight gain ▲ | 7.53° [6.07°, 8.71°] | 4.53° [4.12°, 4.88°] | - | 2883 |
| Median aim error at +500 ms from sight gain | 6.92° [5.62°, 8.05°] | 5.47° [5.08°, 5.88°] | - | 2740 |
| Median aim error at +1000 ms from sight gain | 7.67° [6.41°, 8.74°] | 7.25° [6.72°, 7.70°] | - | 2369 |
| Fire held at -500 ms from sight gain ▲ | 31.7% [28.7%, 35.1%] | 18.2% [16.5%, 20.2%] | - | 2917 |
| Fire held at -200 ms from sight gain ▲ | 32.3% [29.7%, 35.8%] | 21.7% [19.7%, 23.9%] | - | 2917 |
| Fire held at +0 ms from sight gain ▲ | 35.4% [32.6%, 38.7%] | 42.5% [39.1%, 46.0%] | - | 2917 |
| Fire held at +100 ms from sight gain ▲ | 41.3% [38.4%, 44.1%] | 61.4% [57.8%, 64.6%] | - | 2917 |
| Fire held at +200 ms from sight gain ▲ | 49.2% [46.8%, 51.7%] | 77.0% [73.9%, 79.5%] | - | 2917 |
| Fire held at +400 ms from sight gain ▲ | 59.7% [57.2%, 61.9%] | 87.3% [85.5%, 89.0%] | - | 2870 |
| Fire held at +700 ms from sight gain ▲ | 56.8% [54.8%, 58.8%] | 77.0% [74.3%, 79.7%] | - | 2751 |
| Fire held at +1000 ms from sight gain ▲ | 49.3% [46.5%, 52.1%] | 63.4% [60.9%, 66.4%] | - | 2644 |
| Fire held at +1450 ms from sight gain ▲ | 44.0% [40.8%, 47.4%] | 53.6% [49.3%, 58.5%] | - | 2500 |
| Reaction: first trigger press after a clean sighting p25 | 100 ms [100 ms, 150 ms] | 50 ms [50 ms, 100 ms] | - | 929 |
| Reaction: first trigger press after a clean sighting p50 | 250 ms [200 ms, 251 ms] | 150 ms [150 ms, 200 ms] | - | 929 |
| Reaction: first trigger press after a clean sighting p75 ▲ | 400 ms [350 ms, 450 ms] | 300 ms [250 ms, 300 ms] | - | 929 |
| Reaction: first trigger press after a clean sighting p90 ▲ | 700 ms [600 ms, 751 ms] | 400 ms [400 ms, 450 ms] | - | 929 |
| Crosshair within half-width+1.5 deg after a clean sighting p50 | 300 ms [250 ms, 300 ms] | 200 ms [200 ms, 250 ms] | - | 1081 |
| Crosshair already on the body at the first visible tick ▲ | 13.8% [12.6%, 15.6%] | 22.6% [20.9%, 24.4%] | - | 2917 |
| Fire already held on the tick before sight (prefire) | 33.9% [30.9%, 37.8%] | 35.4% [32.2%, 38.6%] | - | 2917 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shots per burst (consecutive 100 ms shots) p50 | 5 [4, 5] | 5 [4, 5] | - | 11302 |
| Shots per burst (consecutive 100 ms shots) p90 | 15 [13, 16] | 12 [11, 13] | - | 11302 |

### Combat

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shot accuracy (eligible SMG shots that hit) ▲ | 6.4% [5.5%, 7.1%] | 19.2% [18.1%, 20.3%] | - | 77882 |

### Lives

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Respawn delay after death p50 ▲ | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | - | 939 |
| Life length (lives ending in death) p50 ▲ | 24.7 s [20.0 s, 29.0 s] | 7.1 s [5.8 s, 8.2 s] | - | 939 |

### Reload

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Reload within 3 s of a kill, rounds left 0 | 100.0% [100.0%, 100.0%] | 95.5% [86.4%, 100.0%] | - | 25 |
| Reload within 3 s of a kill, rounds left 1-3 | 93.3% [85.4%, 100.0%] | 96.4% [92.6%, 99.2%] | - | 63 |
| Reload within 3 s of a kill, rounds left 4-6 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 67 |
| Reload within 3 s of a kill, rounds left 7-10 | 98.7% [96.4%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 118 |
| Reload within 3 s of a kill, rounds left 11-15 | 97.3% [93.0%, 99.2%] | 98.8% [97.9%, 99.5%] | - | 139 |
| Reload within 3 s of a kill, rounds left 16-20 | 95.9% [93.1%, 98.7%] | 95.7% [93.5%, 97.7%] | - | 156 |
| Reload within 3 s of a kill, rounds left 21-25 | 87.6% [83.5%, 92.8%] | 83.1% [76.5%, 88.5%] | - | 168 |
| Reload within 3 s of a kill, rounds left 26-29 | 70.2% [64.9%, 76.8%] | 63.4% [52.8%, 75.0%] | - | 174 |
| Reload within 3 s of a kill, rounds left 30-32 | 75.8% [66.7%, 100.0%] | 73.7% [50.0%, 92.3%] | - | 21 |
| Delay from kill to reload (reloads within 3 s) p50 | 650 ms [650 ms, 700 ms] | 600 ms [600 ms, 650 ms] | - | 838 |
| Reloads started with the opponent dead ▲ | 30.3% [26.8%, 32.9%] | 88.2% [85.5%, 90.4%] | - | 2835 |
| Reloads with the opponent alive that are forced (empty clip) | 85.6% [83.2%, 87.6%] | 77.7% [69.5%, 83.9%] | - | 2001 |

### Fights

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Engagements that end in a kill ▲ | 53.6% [51.3%, 55.3%] | 79.9% [77.9%, 81.9%] | - | 3450 |
| First hitter wins (decisive engagements) ▲ | 72.2% [70.1%, 74.2%] | 65.4% [63.7%, 66.7%] | - | 1842 |
| First shooter wins (decisive engagements) | 53.1% [51.6%, 54.6%] | 55.0% [52.5%, 56.6%] | - | 1842 |
| Duration of decisive engagements p50 ▲ | 1550 ms [1399 ms, 1700 ms] | 1250 ms [1150 ms, 1351 ms] | - | 1842 |
| Engagement start distance p50 | 332 u [290 u, 414 u] | 427 u [398 u, 470 u] | - | 3450 |
| Decisive engagements won (subject's own) | 50.7% [44.1%, 55.3%] | 50.0% [45.6%, 54.2%] | - | 1842 |

### Perception

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time with a body part on screen and the centroid hidden ▲ | 5.1% [3.9%, 6.5%] | 10.5% [9.6%, 11.8%] | - | 649580 |
| SMG shots that hit, a part on screen, centroid hidden ▲ | 11.7% [10.5%, 13.2%] | 21.7% [18.9%, 24.3%] | - | 5724 |
| SMG shots that hit, centroid visible ▲ | 20.8% [18.3%, 22.4%] | 27.2% [25.8%, 28.5%] | - | 20112 |
| Centroid sightings with a body part on screen first ▲ | 36.5% [33.5%, 40.2%] | 85.9% [84.2%, 87.7%] | - | 2912 |
| Lead of the first part over the centroid p50 | 100 ms [100 ms, 100 ms] | 100 ms [100 ms, 100 ms] | - | 1060 |
| Median aim error at -500 ms from the first visible part ▲ | 12.99° [11.36°, 14.08°] | 15.23° [14.18°, 16.19°] | - | 1843 |
| Median aim error at -200 ms from the first visible part | 11.30° [10.06°, 12.39°] | 10.54° [9.80°, 11.23°] | - | 1843 |
| Median aim error at -100 ms from the first visible part ▲ | 10.50° [9.76°, 11.20°] | 8.00° [7.34°, 8.49°] | - | 1843 |
| Median aim error at +0 ms from the first visible part ▲ | 10.21° [9.28°, 11.32°] | 5.22° [4.91°, 5.53°] | - | 1843 |
| Median aim error at +100 ms from the first visible part ▲ | 8.66° [7.64°, 10.06°] | 4.87° [4.54°, 5.18°] | - | 1843 |
| Median aim error at +200 ms from the first visible part | 6.88° [5.52°, 7.90°] | 5.26° [4.86°, 5.58°] | - | 1843 |
| Fire already held on the tick before the first part (prefire) ▲ | 32.0% [30.4%, 33.5%] | 23.0% [20.3%, 25.5%] | - | 1843 |
| Reaction: first press after a clean sighting, from the first part p50 | 200 ms [200 ms, 200 ms] | 200 ms [200 ms, 250 ms] | - | 708 |
| Reaction: mean first press after a clean sighting, from the first part | 253 ms [235 ms, 280 ms] | 232 ms [217 ms, 250 ms] | - | 708 |
| Crosshair within half-width+1.5 deg after a clean sighting, from the first part p50 | 250 ms [250 ms, 300 ms] | 250 ms [200 ms, 250 ms] | - | 819 |
| First hit after the first part p50 | 300 ms [250 ms, 300 ms] | 250 ms [200 ms, 250 ms] | - | 918 |
| Part sightings whose visible bout ends without a hit ▲ | 48.9% [46.2%, 53.5%] | 27.5% [25.7%, 29.7%] | - | 1843 |
| Yaw error to the hidden enemy 500 ms before the first part ▲ | 10.98° [9.47°, 12.38°] | 14.63° [13.70°, 15.63°] | - | 1843 |
| Yaw error to where the enemy will appear, 500 ms before the first part | 10.19° [9.02°, 10.98°] | 10.65° [9.39°, 11.89°] | - | 1843 |
| Crosshair parked within 5 deg of where the enemy will appear, 500 ms before | 29.5% [25.2%, 33.3%] | 29.4% [27.1%, 32.4%] | - | 1843 |
| Crosshair closer to where the enemy will appear than to the enemy, -500 ms ▲ | 53.0% [50.7%, 54.6%] | 67.0% [65.6%, 68.8%] | - | 1843 |
| View turn toward the enemy over the last 500 ms before the first part ▲ | 7.14° [4.51°, 9.73°] | 13.01° [11.44°, 14.55°] | - | 1843 |
| View turn over the last 500 ms before the first part ▲ | 10.61° [8.01°, 12.41°] | 14.53° [13.34°, 15.90°] | - | 1843 |
| Speed 400-200 ms before the first part ▲ | 116 u/s [109 u/s, 121 u/s] | 188 u/s [182 u/s, 193 u/s] | - | 1843 |
| Speed 2-1 s before the first part ▲ | 113 u/s [107 u/s, 119 u/s] | 167 u/s [159 u/s, 176 u/s] | - | 1461 |
| Hidden, no sighting next: crosshair closer to where the enemy will be in 500 ms | 49.5% [49.1%, 49.8%] | 50.5% [49.0%, 51.6%] | - | 392000 |
| Yaw error to the corner, 500 ms before the first part ▲ | 8.21° [7.37°, 9.73°] | 5.81° [5.17°, 6.44°] | - | 1126 |
| Angle to the corner, 500 ms before the first part ▲ | 10.77° [9.76°, 11.82°] | 8.41° [7.70°, 9.19°] | - | 1126 |
| Pitch to the corner (- = corner above the crosshair), 500 ms before the first part | -2.30° [-2.85°, -1.89°] | -2.72° [-3.14°, -2.39°] | - | 1126 |
| Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part ▲ | 2.05° [1.22°, 2.84°] | -1.77° [-2.04°, -1.43°] | - | 1126 |
| Yaw error to the appearance point (sightings with a corner), 500 ms before the first part | 9.27° [8.66°, 10.36°] | 10.34° [9.19°, 11.29°] | - | 1126 |
| Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part ▲ | 11.33° [9.77°, 12.34°] | 15.18° [14.24°, 16.01°] | - | 1126 |
| Crosshair within 5 deg of the corner, 500 ms before the first part | 23.7% [20.2%, 27.4%] | 29.2% [26.4%, 32.7%] | - | 1126 |
| Crosshair closer to the corner than to the enemy, 500 ms before the first part ▲ | 56.9% [53.7%, 60.7%] | 80.0% [78.8%, 81.1%] | - | 1126 |
| Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part ▲ | 32.9% [30.1%, 36.6%] | 48.2% [45.7%, 50.3%] | - | 1126 |
| Crosshair within 2 deg of the corner's edge, 500 ms before the first part ▲ | 16.9% [14.1%, 19.9%] | 23.7% [21.7%, 25.7%] | - | 1126 |
| Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part ▲ | 50.2% [46.6%, 53.2%] | 28.1% [26.8%, 29.4%] | - | 1126 |
| Angle to the corner at the first part ▲ | 9.38° [8.49°, 10.08°] | 5.74° [5.33°, 6.17°] | - | 1126 |
| Distance from the eye to the corner p50 | 162 u [133 u, 212 u] | 228 u [204 u, 248 u] | - | 1126 |
| Yaw error to the corner at -2000 ms (sightings hidden >= 2 s) | 11.32° [9.54°, 12.51°] | 14.66° [11.85°, 17.22°] | - | 685 |
| Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) ▲ | 9.00° [7.09°, 10.49°] | 5.78° [4.88°, 6.98°] | - | 685 |
| Yaw error to the corner at -500 ms (sightings hidden >= 2 s) ▲ | 7.11° [6.05°, 8.30°] | 4.66° [3.99°, 5.31°] | - | 685 |
| Yaw error to the corner at +0 ms (sightings hidden >= 2 s) ▲ | 6.65° [5.92°, 7.51°] | 3.54° [3.01°, 4.10°] | - | 685 |

Consistency: 112 pooled statistics (bots and owner together) were recomputed from the analysis scripts' own JSON; 0 differ.

## Notes

- perception statistics on the bots' logged body-part column (ext_vis_parts: their own perception); the people's are on the data repo's rebuilt column, accepted against the logged one on 2026-09-28
- 100% of the bots' duel time is against another bot (the logged opponent is the nearest living enemy); the human reference is humans against humans only
- engagements.py counts contact episodes only while exactly two players are alive; with more than two players in a session the fight statistics cover the phases where one player is dead
