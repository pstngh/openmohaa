# Bot evaluation: 2026-10-02_outofammo_realmaps

Generated 2026-10-02 by `humanbot/eval/compare.py`. Captures: `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`. 8 sessions on dm/crnodoors, dm/downladder, dm/main, dm/vents; capture dates 2026-10-02.
Human reference: `human_reference.json` (2026-10-02), 397 duel minutes, 72 player-sessions, 5 people.

## Bots in this capture

| Bot | Family | Seed | Sessions | Duel minutes |
|---|---|---:|---:|---:|
| `bot:stopper:4` | stopper | 711847621 | 1 | 31.7 |
| `bot:stopper:5` | stopper | 1529931226 | 1 | 31.7 |
| `bot:strafer:4` | strafer | 231333596 | 1 | 30.2 |
| `bot:presser:5` | presser | 1152489726 | 1 | 30.2 |
| `bot:stopper:4.2` | stopper | 2458011590 | 1 | 32.2 |
| `bot:strafer:5` | strafer | 4003987225 | 1 | 32.2 |
| `bot:strafer:4.2` | strafer | 3498264623 | 1 | 31.6 |
| `bot:stopper:5.2` | stopper | 2513918026 | 1 | 31.6 |
| `bot:presser:4` | presser | 52844662 | 1 | 33.1 |
| `bot:strafer:5.2` | strafer | 3859071546 | 1 | 33.1 |
| `bot:presser:4.2` | presser | 588158740 | 1 | 32.7 |
| `bot:presser:5.2` | presser | 4131749178 | 1 | 32.7 |
| `bot:stopper:4.3` | stopper | 97502759 | 1 | 33.3 |
| `bot:strafer:5.3` | strafer | 2592015535 | 1 | 33.3 |
| `bot:strafer:4.3` | strafer | 1086809828 | 1 | 33.1 |
| `bot:strafer:5.4` | strafer | 821368291 | 1 | 33.1 |

The owner (`pstN@vsbot`) has 0.0 duel minutes against the bots; those rows are never pooled with the bots.

Bot duel time against the owner 0%, against other bots 100%.

Family mix of bot duel time (observed → reweighted to the styles.json targets): presser 25% → 20%, stopper 31% → 40%, strafer 44% → 40%.

## Tells (147)

Statistics whose 95% CIs for the pooled bots and the pooled humans do not overlap, ranked by the standardized gap z = (bots − humans) / √(SE²bots + SE²humans). Only statistics with enough data on both sides are ranked.

| # | Statistic | Bots | Humans | z |
|---:|---|---|---|---:|
| 1 | Centroid sightings with a body part on screen first | 36.5% [33.9%, 38.9%] | 85.9% [84.2%, 87.7%] | -31.4 |
| 2 | Side-hold duration (all contexts) p50 | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | -28.3 |
| 3 | Reloads started with the opponent dead | 40.0% [36.8%, 43.7%] | 88.2% [85.5%, 90.4%] | -21.5 |
| 4 | Yaw speed (hidden, firing) p99 | 463°/s [437°/s, 484°/s] | 194°/s [182°/s, 204°/s] | +20.3 |
| 5 | Speed 400-200 ms before the first part | 120 u/s [116 u/s, 122 u/s] | 188 u/s [182 u/s, 193 u/s] | -19.4 |
| 6 | Respawn delay after death p50 | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | +18.1 |
| 7 | Fire held at +400 ms from sight gain | 59.6% [56.6%, 62.0%] | 87.3% [85.5%, 89.0%] | -16.4 |
| 8 | Fire held at +200 ms from sight gain | 45.9% [43.1%, 48.2%] | 77.0% [73.9%, 79.5%] | -15.6 |
| 9 | Shot accuracy (eligible SMG shots that hit) | 8.6% [7.8%, 9.5%] | 19.2% [18.1%, 20.3%] | -14.5 |
| 10 | Share of duel time: LOS firefight | 8.0% [7.3%, 8.6%] | 17.6% [16.5%, 18.9%] | -13.6 |
| 11 | Crosshair closer to the corner than to the enemy, 500 ms before the first part | 55.4% [52.4%, 58.9%] | 80.0% [78.8%, 81.1%] | -13.6 |
| 12 | Mouse still between ticks (LOS firefight) | 3.2% [3.1%, 3.3%] | 1.6% [1.4%, 1.8%] | +13.5 |
| 13 | Yaw speed (reloading) p99 | 609°/s [580°/s, 635°/s] | 945°/s [909°/s, 991°/s] | -13.0 |
| 14 | Engagements that end in a kill | 55.5% [53.1%, 58.8%] | 79.9% [77.9%, 81.9%] | -12.9 |
| 15 | Side-hold duration (all contexts) p90 | 500 ms [500 ms, 500 ms] | 800 ms [800 ms, 850 ms] | -12.7 |
| 16 | Fire held at +700 ms from sight gain | 54.1% [52.0%, 56.1%] | 77.0% [74.3%, 79.7%] | -12.7 |
| 17 | Key chord neutral (reloading) | 27.2% [24.8%, 29.9%] | 8.3% [7.3%, 9.5%] | +12.6 |
| 18 | Retreating >40 u/s in LOS firefight at distance (160, 224] | 19.2% [18.2%, 20.4%] | 6.9% [5.5%, 8.7%] | +12.0 |
| 19 | Key chord fwd diag (hidden, not firing) | 10.4% [9.3%, 11.2%] | 29.9% [26.7%, 33.2%] | -11.8 |
| 20 | Life length (lives ending in death) p50 | 24.1 s [21.5 s, 26.3 s] | 7.1 s [5.8 s, 8.2 s] | +11.8 |
| 21 | Fire held at +100 ms from sight gain | 35.4% [32.4%, 38.1%] | 61.4% [57.8%, 64.6%] | -11.3 |
| 22 | Key chord fwd diag (reloading) | 17.3% [15.8%, 18.9%] | 36.8% [34.1%, 39.9%] | -11.1 |
| 23 | Yaw speed (LOS firefight) p99 | 576°/s [530°/s, 623°/s] | 305°/s [292°/s, 318°/s] | +10.8 |
| 24 | Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) | 25.8% [24.4%, 27.5%] | 15.6% [14.7%, 16.9%] | +10.0 |
| 25 | Side-hold duration (LOS firefight) p75 | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | -9.8 |
| 26 | Strafe switch hazard per tick at hold age 100 ms (LOS firefight) | 12.7% [11.7%, 14.0%] | 5.8% [5.2%, 6.8%] | +9.8 |
| 27 | Speed 2-1 s before the first part | 114 u/s [105 u/s, 119 u/s] | 167 u/s [159 u/s, 176 u/s] | -9.7 |
| 28 | Median aim error at +0 ms from sight gain | 12.22° [10.73°, 13.60°] | 5.22° [4.88°, 5.61°] | +9.4 |
| 29 | Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part | 50.4% [46.0%, 55.6%] | 28.1% [26.8%, 29.4%] | +9.2 |
| 30 | Part sightings whose visible bout ends without a hit | 45.0% [42.2%, 48.7%] | 27.5% [25.7%, 29.7%] | +9.1 |
| 31 | Share of duel time with a body part on screen and the centroid hidden | 4.5% [3.8%, 5.2%] | 10.5% [9.6%, 11.8%] | -9.1 |
| 32 | Median aim error at +0 ms from the first visible part | 10.35° [9.26°, 11.23°] | 5.22° [4.91°, 5.53°] | +8.9 |
| 33 | Side-hold duration (LOS firefight) p90 | 450 ms [450 ms, 500 ms] | 700 ms [650 ms, 700 ms] | -8.9 |
| 34 | Retreating >40 u/s in LOS firefight at distance (96, 160] | 23.0% [21.2%, 24.8%] | 10.5% [8.5%, 12.6%] | +8.8 |
| 35 | Crosshair already on the body at the first visible tick | 12.7% [11.4%, 13.9%] | 22.6% [20.9%, 24.4%] | -8.8 |
| 36 | Fire held at +0 ms from sight gain | 25.5% [23.8%, 27.5%] | 42.5% [39.1%, 46.0%] | -8.6 |
| 37 | Shots per burst (consecutive 100 ms shots) p50 | 3 [3, 3] | 5 [4, 5] | -8.6 |
| 38 | Reaction: first trigger press after a clean sighting p90 | 700 ms [650 ms, 750 ms] | 400 ms [400 ms, 450 ms] | +8.6 |
| 39 | Retreating >40 u/s in LOS firefight at distance (224, 288] | 14.6% [12.8%, 16.1%] | 5.3% [4.2%, 6.8%] | +8.2 |
| 40 | Crosshair closer to where the enemy will appear than to the enemy, -500 ms | 54.4% [52.0%, 56.9%] | 67.0% [65.6%, 68.8%] | -8.2 |
| 41 | Key chord back any (reloading) | 12.3% [11.6%, 13.0%] | 6.5% [5.3%, 7.6%] | +8.0 |
| 42 | Fire held at +1000 ms from sight gain | 48.9% [46.9%, 50.6%] | 63.4% [60.9%, 66.4%] | -7.9 |
| 43 | Share of duel time: hidden, not firing | 59.9% [58.1%, 61.7%] | 49.1% [47.4%, 51.2%] | +7.8 |
| 44 | Approaching >40 u/s in LOS firefight at distance (96, 160] | 16.1% [13.7%, 18.9%] | 36.4% [32.6%, 41.2%] | -7.8 |
| 45 | Attack holds that are taps (<=100 ms) | 34.2% [33.5%, 35.0%] | 44.3% [41.8%, 46.5%] | -7.8 |
| 46 | Walk presses per minute | 3.80/min [3.05/min, 4.45/min] | 7.54/min [6.90/min, 8.11/min] | -7.8 |
| 47 | Strafe switch hazard per tick at hold age 50 ms (LOS firefight) | 9.4% [7.6%, 11.8%] | 1.2% [0.9%, 1.6%] | +7.6 |
| 48 | Median aim error at +100 ms from the first visible part | 9.03° [8.01°, 10.28°] | 4.87° [4.54°, 5.18°] | +7.3 |
| 49 | Strafe switch hazard per tick at hold age 200 ms (LOS firefight) | 25.4% [23.9%, 26.9%] | 17.0% [15.4%, 18.7%] | +7.2 |
| 50 | Key chord forward (reloading) | 15.3% [13.5%, 17.3%] | 26.1% [23.8%, 28.6%] | -7.1 |
| 51 | SMG shots that hit, a part on screen, centroid hidden | 12.4% [11.5%, 13.6%] | 21.7% [18.9%, 24.3%] | -7.0 |
| 52 | Key chord fwd diag (hidden, firing) | 11.6% [10.5%, 12.9%] | 28.6% [23.9%, 32.8%] | -6.9 |
| 53 | Retreating >40 u/s in LOS firefight at distance (288, 384] | 12.7% [11.4%, 14.1%] | 5.9% [4.6%, 7.3%] | +6.8 |
| 54 | Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part | 2.06° [0.86°, 3.04°] | -1.77° [-2.04°, -1.43°] | +6.7 |
| 55 | Crosshair within 2 deg of the corner's edge, 500 ms before the first part | 14.2% [12.3%, 16.2%] | 23.7% [21.7%, 25.7%] | -6.7 |
| 56 | Approaching >40 u/s in LOS firefight at distance (160, 224] | 15.0% [12.5%, 17.1%] | 35.4% [30.1%, 42.0%] | -6.5 |
| 57 | Approaching >40 u/s in LOS firefight at distance (224, 288] | 14.3% [12.1%, 16.4%] | 38.2% [32.3%, 46.4%] | -6.5 |
| 58 | Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago | 20.4% [18.3%, 23.6%] | 7.4% [4.7%, 10.8%] | +6.3 |
| 59 | Fire already held on the tick before sight (prefire) | 23.0% [20.9%, 25.2%] | 35.4% [32.2%, 38.6%] | -6.3 |
| 60 | Median aim error at +100 ms from sight gain | 11.05° [9.46°, 12.37°] | 5.97° [5.54°, 6.43°] | +6.2 |
| 61 | Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part | 35.4% [31.4%, 38.2%] | 48.2% [45.7%, 50.3%] | -6.1 |
| 62 | Median aim error firing with LOS at (128, 192] u | 11.13° [10.57°, 11.74°] | 8.99° [8.66°, 9.45°] | +6.1 |
| 63 | Median aim error firing with LOS at (0, 128] u | 17.86° [17.19°, 18.79°] | 14.28° [13.53°, 15.19°] | +6.0 |
| 64 | Key chord neutral (hidden, not firing) | 38.8% [35.9%, 42.5%] | 26.1% [23.1%, 28.6%] | +5.9 |
| 65 | Key chord back any (LOS, not firing) | 10.7% [9.8%, 11.8%] | 4.8% [3.4%, 6.7%] | +5.8 |
| 66 | Fire held at +1450 ms from sight gain | 39.3% [37.8%, 41.0%] | 53.6% [49.3%, 58.5%] | -5.6 |
| 67 | Approaching >40 u/s in LOS firefight at distance (288, 384] | 16.0% [12.8%, 19.8%] | 39.4% [32.7%, 47.2%] | -5.6 |
| 68 | Median aim error at +200 ms from sight gain | 8.83° [7.62°, 10.00°] | 4.92° [4.54°, 5.23°] | +5.6 |
| 69 | Crosshair on body firing with LOS at (128, 192] u | 32.8% [30.3%, 35.6%] | 42.5% [39.8%, 44.7%] | -5.5 |
| 70 | Side-hold duration (LOS firefight) p50 | 200 ms [200 ms, 200 ms] | 300 ms [250 ms, 300 ms] | -5.4 |
| 71 | SMG shots that hit, centroid visible | 20.8% [19.0%, 22.5%] | 27.2% [25.8%, 28.5%] | -5.4 |
| 72 | Yaw speed (hidden, not firing) p99 | 638°/s [621°/s, 657°/s] | 544°/s [521°/s, 580°/s] | +5.4 |
| 73 | Standing still (<5 u/s) (reloading) | 10.3% [8.9%, 11.9%] | 5.2% [4.3%, 6.2%] | +5.3 |
| 74 | Yaw error to the corner at +0 ms (sightings hidden >= 2 s) | 7.05° [5.91°, 8.08°] | 3.54° [3.01°, 4.10°] | +5.3 |
| 75 | Crosshair on body firing with LOS at (192, 256] u | 31.6% [28.5%, 33.8%] | 42.0% [39.5%, 44.4%] | -5.3 |
| 76 | Key chord fwd diag (LOS firefight) | 11.2% [7.9%, 15.2%] | 30.5% [25.6%, 36.7%] | -5.2 |
| 77 | Median aim error at +300 ms from sight gain | 7.98° [6.84°, 9.11°] | 4.53° [4.12°, 4.88°] | +5.2 |
| 78 | Approaching >40 u/s in LOS firefight at distance (0, 96] | 22.9% [19.0%, 26.2%] | 37.6% [33.7%, 42.3%] | -5.2 |
| 79 | Key chord forward (hidden, not firing) | 8.5% [7.7%, 9.2%] | 11.9% [10.9%, 12.8%] | -5.2 |
| 80 | Angle to the corner at the first part | 8.90° [7.67°, 9.77°] | 5.74° [5.33°, 6.17°] | +5.1 |
| 81 | Median aim error firing with LOS at (192, 256] u | 8.28° [7.90°, 8.82°] | 6.68° [6.36°, 7.10°] | +5.1 |
| 82 | View within 30 deg of the hidden opponent, last seen 4-8 s | 63.6% [60.6%, 67.9%] | 76.4% [73.1%, 79.3%] | -5.0 |
| 83 | Key chord back any (LOS firefight) | 8.6% [7.8%, 9.5%] | 5.2% [4.5%, 6.1%] | +5.0 |
| 84 | Release hazard while hidden, LOS lost 1 s or more ago | 16.1% [15.8%, 16.5%] | 11.3% [9.8%, 13.5%] | +5.0 |
| 85 | Press hazard while hidden, LOS lost 300-500 ms ago | 9.9% [9.3%, 10.5%] | 7.0% [6.0%, 8.1%] | +5.0 |
| 86 | Median aim error at -200 ms from sight gain | 13.45° [11.73°, 15.29°] | 8.33° [7.70°, 8.92°] | +4.9 |
| 87 | Strafe switch hazard per tick at hold age 150 ms (LOS firefight) | 19.2% [16.9%, 21.2%] | 12.4% [11.1%, 13.9%] | +4.9 |
| 88 | Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part | 11.15° [9.78°, 12.64°] | 15.18° [14.24°, 16.01°] | -4.9 |
| 89 | Approaching >40 u/s in LOS firefight at distance (384, 512] | 15.4% [10.6%, 22.7%] | 41.8% [33.8%, 49.1%] | -4.8 |
| 90 | Fire held, LOS, aim error in half-widths (20, 1000] | 6.6% [4.6%, 8.3%] | 0.9% [0.4%, 3.1%] | +4.7 |
| 91 | Key chord back any (hidden, not firing) | 8.6% [8.1%, 9.0%] | 6.1% [5.3%, 7.1%] | +4.7 |
| 92 | Yaw error to the hidden opponent, last seen 4-8 s | 18.19° [14.48°, 20.46°] | 10.29° [9.19°, 11.80°] | +4.7 |
| 93 | Mouse still between ticks (hidden, firing) | 8.9% [8.6%, 9.2%] | 10.9% [10.1%, 11.8%] | -4.6 |
| 94 | Crosshair on body firing with LOS at (256, 384] u | 31.1% [28.7%, 33.1%] | 39.0% [36.0%, 41.4%] | -4.6 |
| 95 | Yaw error to the corner, 500 ms before the first part | 8.20° [7.64°, 9.03°] | 5.81° [5.17°, 6.44°] | +4.5 |
| 96 | Strafe switch hazard per tick at hold age 250 ms (LOS firefight) | 24.5% [22.0%, 27.2%] | 17.4% [16.0%, 19.0%] | +4.5 |
| 97 | Release hazard while hidden, LOS lost 500-950 ms ago | 13.1% [12.4%, 13.8%] | 10.1% [9.2%, 11.3%] | +4.5 |
| 98 | View turn toward the enemy over the last 500 ms before the first part | 8.52° [7.22°, 9.45°] | 13.01° [11.44°, 14.55°] | -4.4 |
| 99 | Fire held without LOS | 19.8% [18.7%, 20.9%] | 24.5% [22.7%, 26.3%] | -4.4 |
| 100 | Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) | 22.6% [20.2%, 25.4%] | 16.1% [14.9%, 17.5%] | +4.4 |
| 101 | Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) | 38.0% [33.6%, 40.2%] | 49.4% [44.0%, 50.8%] | -4.4 |
| 102 | Yaw error to the hidden enemy 500 ms before the first part | 11.26° [9.92°, 12.27°] | 14.63° [13.70°, 15.63°] | -4.3 |
| 103 | Yaw speed (LOS, not firing) p99 | 811°/s [752°/s, 860°/s] | 583°/s [505°/s, 695°/s] | +4.2 |
| 104 | Retreating >40 u/s in LOS firefight at distance (384, 512] | 9.6% [7.7%, 11.5%] | 4.8% [3.8%, 6.0%] | +4.2 |
| 105 | Yaw error to the corner at -500 ms (sightings hidden >= 2 s) | 7.88° [6.76°, 9.43°] | 4.66° [3.99°, 5.31°] | +4.1 |
| 106 | Retreating >40 u/s in LOS firefight at distance (512, 768] | 12.4% [7.4%, 15.1%] | 4.0% [3.0%, 5.4%] | +4.0 |
| 107 | Share of duel time in the busiest 10% of 32u cells (dm/downladder) | 36.4% [30.5%, 37.0%] | 61.4% [42.9%, 63.8%] | -4.0 |
| 108 | Key chord pure strafe (hidden, not firing) | 33.8% [30.8%, 36.0%] | 26.0% [23.5%, 29.4%] | +4.0 |
| 109 | Share of duel time: reloading | 13.1% [12.1%, 13.9%] | 10.5% [9.6%, 11.5%] | +4.0 |
| 110 | Key chord neutral (hidden, firing) | 21.4% [18.4%, 25.0%] | 13.2% [11.2%, 15.7%] | +4.0 |
| 111 | Fire held, LOS, aim error in half-widths (6, 10] | 38.3% [32.2%, 43.7%] | 22.4% [17.1%, 27.1%] | +4.0 |
| 112 | Key chord fwd diag (LOS, not firing) | 12.5% [10.7%, 14.2%] | 29.4% [23.0%, 38.1%] | -4.0 |
| 113 | Share of duel time in the busiest 10% of 32u cells (dm/main) | 34.2% [28.2%, 35.8%] | 45.2% [40.5%, 48.6%] | -3.9 |
| 114 | Key chord pure strafe (LOS firefight) | 69.8% [65.8%, 72.7%] | 58.0% [53.1%, 62.2%] | +3.9 |
| 115 | Press hazard while hidden, LOS lost 550-1000 ms ago | 4.5% [4.1%, 4.8%] | 3.5% [3.2%, 3.9%] | +3.8 |
| 116 | Yaw speed (hidden, not firing) p50 | 12°/s [11°/s, 13°/s] | 8°/s [6°/s, 10°/s] | +3.7 |
| 117 | Key chord pure strafe (hidden, firing) | 52.9% [49.9%, 55.7%] | 44.9% [41.6%, 48.0%] | +3.6 |
| 118 | Crosshair on body firing with LOS at (0, 128] u | 40.4% [38.2%, 42.8%] | 48.0% [44.9%, 51.5%] | -3.6 |
| 119 | Yaw speed (hidden, firing) p50 | 29°/s [26°/s, 32°/s] | 23°/s [21°/s, 25°/s] | +3.5 |
| 120 | Key chord pure strafe (reloading) | 27.9% [25.7%, 29.8%] | 22.3% [19.6%, 24.4%] | +3.4 |
| 121 | Key chord pure strafe (LOS, not firing) | 40.5% [37.6%, 43.8%] | 26.7% [20.8%, 34.2%] | +3.4 |
| 122 | Yaw speed (LOS, not firing) p50 | 35°/s [32°/s, 39°/s] | 11°/s [0°/s, 26°/s] | +3.4 |
| 123 | View within 30 deg of the hidden opponent, last seen <=250 ms | 86.8% [83.2%, 89.3%] | 92.7% [91.9%, 93.7%] | -3.3 |
| 124 | Press hazard while hidden, LOS lost 150-250 ms ago | 6.8% [5.9%, 7.9%] | 4.6% [3.9%, 5.5%] | +3.3 |
| 125 | Key chord back any (hidden, firing) | 8.9% [8.1%, 9.6%] | 6.7% [5.9%, 7.9%] | +3.3 |
| 126 | Yaw speed (LOS firefight) p50 | 50°/s [44°/s, 56°/s] | 39°/s [36°/s, 41°/s] | +3.3 |
| 127 | Shots per burst (consecutive 100 ms shots) p90 | 10 [9, 10] | 12 [11, 13] | -3.3 |
| 128 | Share of duel time: LOS, not firing | 4.3% [3.9%, 4.6%] | 7.0% [5.5%, 8.4%] | -3.3 |
| 129 | Fire held, LOS, aim error in half-widths (10, 20] | 12.8% [9.1%, 17.5%] | 4.6% [2.4%, 6.9%] | +3.2 |
| 130 | Fire held at -500 ms from sight gain | 21.8% [20.8%, 23.1%] | 18.2% [16.5%, 20.2%] | +3.2 |
| 131 | Key chord neutral (LOS firefight) | 9.4% [7.5%, 12.3%] | 5.1% [3.9%, 6.3%] | +3.1 |
| 132 | Release hazard while hidden, LOS lost 300-450 ms ago | 26.7% [24.8%, 29.1%] | 31.5% [29.6%, 33.4%] | -3.1 |
| 133 | Reaction: first trigger press after a clean sighting p75 | 400 ms [350 ms, 400 ms] | 300 ms [250 ms, 300 ms] | +3.1 |
| 134 | Median aim error at -100 ms from the first visible part | 10.35° [9.10°, 11.63°] | 8.00° [7.34°, 8.49°] | +3.0 |
| 135 | View turn over the last 500 ms before the first part | 11.60° [10.10°, 12.98°] | 14.53° [13.34°, 15.90°] | -3.0 |
| 136 | Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) | 22.6% [16.9%, 27.2%] | 13.7% [12.0%, 15.6%] | +3.0 |
| 137 | Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) | 8.07° [7.25°, 9.18°] | 5.78° [4.88°, 6.98°] | +3.0 |
| 138 | Median aim error at -500 ms from the first visible part | 12.75° [11.34°, 13.93°] | 15.23° [14.18°, 16.19°] | -2.9 |
| 139 | Angle to the corner, 500 ms before the first part | 10.37° [9.48°, 11.51°] | 8.41° [7.70°, 9.19°] | +2.8 |
| 140 | Duration of decisive engagements p50 | 1550 ms [1400 ms, 1700 ms] | 1250 ms [1150 ms, 1351 ms] | +2.8 |
| 141 | Median aim error at +200 ms from the first visible part | 6.45° [5.68°, 7.19°] | 5.26° [4.86°, 5.58°] | +2.7 |
| 142 | Press hazard with LOS and aim within 3 half-widths, LOS gained 0 ms ago | 21.5% [17.1%, 25.0%] | 28.2% [25.8%, 30.8%] | -2.7 |
| 143 | Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago | 35.9% [32.1%, 40.6%] | 28.0% [25.2%, 32.0%] | +2.6 |
| 144 | Side-hold duration (LOS firefight) p25 | 150 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | -2.6 |
| 145 | Median aim error at +500 ms from sight gain | 6.92° [5.96°, 7.89°] | 5.47° [5.08°, 5.88°] | +2.6 |
| 146 | Reload within 3 s of a kill, rounds left 1-3 | 100.0% [100.0%, 100.0%] | 96.4% [92.6%, 99.2%] | +2.1 |
| 147 | First hit after the first part p50 | 300 ms [300 ms, 350 ms] | 250 ms [200 ms, 250 ms] | +1.9 |

## Per bot

### `bot:stopper:4` (stopper, seed 711847621, 31.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.07034 | 0.04578 | -5% |
| reverse_share | 0.1382 | 0.09432 | -6% |
| side_hold_ms | 249.3 | 150 | -66% ⚠ |
| lean_fight | 0.9549 | 0.958 | +1% |
| jumps_per_min | 0.3303 | 0.1575 | -3% |
| crouch_per_min | 5.571 | 5.45 | -1% |
| walk_hidden | 0 | 0.01805 | +17% |
| burst_median | 3.59 | 3 | -15% |
| aim_height_firing | 0.4682 | 0.457 | -6% |
| hold_angle | 0.4674 | 0.3312 | -44% ⚠ |
| aim_error_fight_deg | 6.249 | 5.438 | -33% ⚠ |
| reaction_ms | 153.7 | 250 | +96% ⚠ |
| mp40_share | 0.02524 | 0 | -3% |

Style fingerprint (2026-10-02, 31.7 min, styles.py features 30/30): z-distance to the human cloud centre 11.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0553 (humans 0.152–0.491); neutral 0.494 (humans 0.0872–0.296); lf_fwd_diag 0.0458 (humans 0.0658–0.568); lf_neutral 0.211 (humans 0.00436–0.112); lean_any 0.838 (humans 0.358–0.767); lf_lean 0.958 (humans 0.457–0.948); walk 0.0101 (humans 0.0126–0.0881); attack 0.254 (humans 0.276–0.478); p_attack_given_los 0.498 (humans 0.547–0.861); speed_med 87.3 (humans 136–200); still 0.282 (humans 0.0832–0.227); lf_miss_units_med 38.7 (humans 22.2–36.1); lf_on_target 0.253 (humans 0.274–0.515); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 500 (humans 700–950); reverse_share 0.0943 (humans 0.138–0.839); lf_approach 0.108 (humans 0.131–0.64); lf_retreat 0.164 (humans 0.0243–0.141).

### `bot:stopper:5` (stopper, seed 1529931226, 31.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1013 | 0.05917 | -8% |
| reverse_share | 0.3253 | 0.2661 | -8% |
| side_hold_ms | 247.2 | 200 | -31% ⚠ |
| lean_fight | 0.8427 | 0.8377 | -1% |
| jumps_per_min | 5.122 | 5.765 | +12% |
| crouch_per_min | 11.7 | 11.5 | -2% |
| walk_hidden | 0.07078 | 0.07977 | +9% |
| burst_median | 3.138 | 3 | -3% |
| aim_height_firing | 0.4597 | 0.422 | -20% |
| hold_angle | 0.5274 | 0.3106 | -71% ⚠ |
| aim_error_fight_deg | 4.835 | 3.909 | -38% ⚠ |
| reaction_ms | 109.9 | 225 | +115% ⚠ |
| mp40_share | 0.06948 | 0 | -7% |

Style fingerprint (2026-10-02, 31.7 min, styles.py features 30/30): z-distance to the human cloud centre 10.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0649 (humans 0.152–0.491); neutral 0.462 (humans 0.0872–0.296); lf_fwd_diag 0.0592 (humans 0.0658–0.568); lf_neutral 0.16 (humans 0.00436–0.112); ducked 0.129 (humans 0.0109–0.0741); attack 0.255 (humans 0.276–0.478); p_attack_given_los 0.539 (humans 0.547–0.861); speed_med 85.8 (humans 136–200); still 0.266 (humans 0.0832–0.227); lf_err_med 3.91 (humans 4.08–6.63); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 550 (humans 700–950); lf_approach 0.114 (humans 0.131–0.64); lf_retreat 0.147 (humans 0.0243–0.141).

### `bot:strafer:4` (strafer, seed 231333596, 30.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.05572 | -2% |
| reverse_share | 0.4467 | 0.4061 | -6% |
| side_hold_ms | 326.3 | 250 | -51% ⚠ |
| lean_fight | 0.4941 | 0.5224 | +6% |
| jumps_per_min | 2.379 | 2.117 | -5% |
| crouch_per_min | 9.476 | 9.128 | -3% |
| walk_hidden | 0 | 0.01703 | +16% |
| burst_median | 7 | 3 | -100% ⚠ |
| aim_height_firing | 0.4857 | 0.46 | -14% |
| hold_angle | 0.2514 | 0.3855 | +44% ⚠ |
| aim_error_fight_deg | 4.667 | 3.833 | -34% ⚠ |
| reaction_ms | 140.8 | 200 | +59% ⚠ |
| mp40_share | 0.9062 | 1 | +9% |

Style fingerprint (2026-10-02, 30.2 min, styles.py features 30/30): z-distance to the human cloud centre 8.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 6.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.5 (humans 0.191–0.489); fwd_diag 0.107 (humans 0.152–0.491); back_any 0.13 (humans 0.0362–0.128); lf_fwd_diag 0.0557 (humans 0.0658–0.568); lf_back_any 0.141 (humans 0.0205–0.123); lean_any 0.326 (humans 0.358–0.767); ducked 0.0814 (humans 0.0109–0.0741); walk 0.00912 (humans 0.0126–0.0881); attack 0.257 (humans 0.276–0.478); p_attack_given_los 0.51 (humans 0.547–0.861); still 0.0648 (humans 0.0832–0.227); lf_err_med 3.83 (humans 4.08–6.63); side_hold_p90_ms 600 (humans 700–950); lf_approach 0.0943 (humans 0.131–0.64); lf_retreat 0.189 (humans 0.0243–0.141).

### `bot:presser:5` (presser, seed 1152489726, 30.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4832 | 0.4845 | +0% |
| reverse_share | 0.8051 | 0.7964 | -1% |
| side_hold_ms | 295.2 | 200 | -63% ⚠ |
| lean_fight | 0.9204 | 0.9641 | +9% |
| jumps_per_min | 0.4317 | 0.2315 | -4% |
| crouch_per_min | 3.157 | 3.142 | -0% |
| walk_hidden | 0.02816 | 0.02841 | +0% |
| burst_median | 5.983 | 3 | -75% ⚠ |
| aim_height_firing | 0.417 | 0.383 | -18% |
| hold_angle | 0.2275 | 0.3049 | +25% ⚠ |
| aim_error_fight_deg | 4.893 | 4.281 | -25% |
| reaction_ms | 152.5 | 250 | +98% ⚠ |
| mp40_share | 0.1123 | 1 | +89% ⚠ |

Style fingerprint (2026-10-02, 30.2 min, styles.py features 30/30): z-distance to the human cloud centre 6.7 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 7.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.845 (humans 0.358–0.767); lf_lean 0.964 (humans 0.457–0.948); attack 0.245 (humans 0.276–0.478); p_attack_given_los 0.531 (humans 0.547–0.861); yaw_speed_p95 291 (humans 158–271); side_hold_p90_ms 500 (humans 700–950).

### `bot:stopper:4.2` (stopper, seed 2458011590, 32.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1182 | 0.09631 | -4% |
| reverse_share | 0.4266 | 0.3777 | -7% |
| side_hold_ms | 200 | 200 | +0% |
| lean_fight | 0.8817 | 0.8679 | -3% |
| jumps_per_min | 2.965 | 3.381 | +8% |
| crouch_per_min | 8.679 | 7.228 | -13% |
| walk_hidden | 0.1056 | 0.1065 | +1% |
| burst_median | 3 | 3 | +0% |
| aim_height_firing | 0.4719 | 0.44 | -17% |
| hold_angle | 0.3772 | 0.3333 | -14% |
| aim_error_fight_deg | 4.42 | 6.654 | +91% ⚠ |
| reaction_ms | 139.9 | 250 | +110% ⚠ |
| mp40_share | 0.02794 | 0 | -3% |

Style fingerprint (2026-10-02, 32.2 min, styles.py features 30/30): z-distance to the human cloud centre 10.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0638 (humans 0.152–0.491); neutral 0.494 (humans 0.0872–0.296); lf_neutral 0.125 (humans 0.00436–0.112); ducked 0.0798 (humans 0.0109–0.0741); attack 0.211 (humans 0.276–0.478); p_attack_given_los 0.513 (humans 0.547–0.861); speed_med 74.3 (humans 136–200); still 0.306 (humans 0.0832–0.227); lf_err_med 6.65 (humans 4.08–6.63); side_hold_p90_ms 550 (humans 700–950); lf_retreat 0.152 (humans 0.0243–0.141).

### `bot:strafer:5` (strafer, seed 4003987225, 32.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.03865 | -5% |
| reverse_share | 0.4657 | 0.4062 | -8% |
| side_hold_ms | 270.9 | 200 | -47% ⚠ |
| lean_fight | 0.5402 | 0.4932 | -9% |
| jumps_per_min | 4.852 | 5.491 | +12% |
| crouch_per_min | 3.183 | 2.947 | -2% |
| walk_hidden | 0.03847 | 0.03446 | -4% |
| burst_median | 4.685 | 3 | -42% ⚠ |
| aim_height_firing | 0.4588 | 0.441 | -9% |
| hold_angle | 0.311 | 0.271 | -13% |
| aim_error_fight_deg | 5.446 | 7.107 | +68% ⚠ |
| reaction_ms | 161.3 | 250 | +89% ⚠ |
| mp40_share | 0 | 0 | +0% |

Style fingerprint (2026-10-02, 32.2 min, styles.py features 30/30): z-distance to the human cloud centre 9.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.6 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0634 (humans 0.152–0.491); neutral 0.407 (humans 0.0872–0.296); lf_fwd_diag 0.0386 (humans 0.0658–0.568); lean_any 0.256 (humans 0.358–0.767); attack 0.206 (humans 0.276–0.478); p_attack_given_los 0.525 (humans 0.547–0.861); speed_med 95.7 (humans 136–200); still 0.242 (humans 0.0832–0.227); lf_err_med 7.11 (humans 4.08–6.63); side_hold_p90_ms 550 (humans 700–950); lf_approach 0.103 (humans 0.131–0.64); lf_retreat 0.174 (humans 0.0243–0.141).

### `bot:strafer:4.2` (strafer, seed 3498264623, 31.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1522 | 0.08416 | -13% |
| reverse_share | 0.4724 | 0.4034 | -10% |
| side_hold_ms | 293.3 | 200 | -62% ⚠ |
| lean_fight | 0.6201 | 0.6239 | +1% |
| jumps_per_min | 3.829 | 4.242 | +8% |
| crouch_per_min | 9.64 | 10.1 | +4% |
| walk_hidden | 0 | 0.01754 | +17% |
| burst_median | 3.211 | 3 | -5% |
| aim_height_firing | 0.4919 | 0.486 | -3% |
| hold_angle | 0.3757 | 0.2391 | -44% ⚠ |
| aim_error_fight_deg | 5.803 | 7.442 | +67% ⚠ |
| reaction_ms | 133.2 | 200 | +67% ⚠ |
| mp40_share | 0.5841 | 1 | +42% ⚠ |

Style fingerprint (2026-10-02, 31.6 min, styles.py features 30/30): z-distance to the human cloud centre 9.6 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0993 (humans 0.152–0.491); ducked 0.0951 (humans 0.0109–0.0741); walk 0.0108 (humans 0.0126–0.0881); attack 0.214 (humans 0.276–0.478); p_attack_given_los 0.472 (humans 0.547–0.861); speed_med 116 (humans 136–200); lf_err_med 7.44 (humans 4.08–6.63); yaw_speed_p95 293 (humans 158–271); side_hold_p90_ms 550 (humans 700–950); lf_retreat 0.187 (humans 0.0243–0.141).

### `bot:stopper:5.2` (stopper, seed 2513918026, 31.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1336 | 0.08815 | -9% |
| reverse_share | 0.6777 | 0.6449 | -5% |
| side_hold_ms | 226 | 200 | -17% |
| lean_fight | 0.9588 | 0.9618 | +1% |
| jumps_per_min | 2.944 | 3.197 | +5% |
| crouch_per_min | 3.426 | 3.134 | -3% |
| walk_hidden | 0.08429 | 0.1021 | +17% |
| burst_median | 3 | 3 | +0% |
| aim_height_firing | 0.4602 | 0.427 | -18% |
| hold_angle | 0.3824 | 0.3471 | -11% |
| aim_error_fight_deg | 4.291 | 6.075 | +73% ⚠ |
| reaction_ms | 113 | 150 | +37% ⚠ |
| mp40_share | 0.07877 | 0 | -8% |

Style fingerprint (2026-10-02, 31.6 min, styles.py features 30/30): z-distance to the human cloud centre 9.3 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0851 (humans 0.152–0.491); neutral 0.399 (humans 0.0872–0.296); lean_any 0.858 (humans 0.358–0.767); lf_lean 0.962 (humans 0.457–0.948); attack 0.235 (humans 0.276–0.478); speed_med 89 (humans 136–200); still 0.264 (humans 0.0832–0.227); side_hold_p90_ms 550 (humans 700–950); lf_approach 0.127 (humans 0.131–0.64); lf_retreat 0.183 (humans 0.0243–0.141).

### `bot:presser:4` (presser, seed 52844662, 33.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5317 | 0.2482 | -55% ⚠ |
| reverse_share | 0.8152 | 0.7728 | -6% |
| side_hold_ms | 336.6 | 200 | -91% ⚠ |
| lean_fight | 0.935 | 0.9615 | +5% |
| jumps_per_min | 0.336 | 0.4839 | +3% |
| crouch_per_min | 1.298 | 1.18 | -1% |
| walk_hidden | 0 | 0.01044 | +10% |
| burst_median | 6.634 | 3 | -91% ⚠ |
| aim_height_firing | 0.3853 | 0.322 | -34% ⚠ |
| hold_angle | 0.2402 | 0.2755 | +12% |
| aim_error_fight_deg | 4.424 | 6.254 | +74% ⚠ |
| reaction_ms | 173.4 | 250 | +77% ⚠ |
| mp40_share | 0.06486 | 0 | -6% |

Style fingerprint (2026-10-02, 33.1 min, styles.py features 30/30): z-distance to the human cloud centre 8.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.51 (humans 0.191–0.489); lean_any 0.892 (humans 0.358–0.767); lf_lean 0.962 (humans 0.457–0.948); walk 0.00617 (humans 0.0126–0.0881); attack 0.226 (humans 0.276–0.478); p_attack_given_los 0.447 (humans 0.547–0.861); speed_med 125 (humans 136–200); lf_height_frac 0.322 (humans 0.352–0.54); yaw_speed_p95 316 (humans 158–271); side_hold_p90_ms 450 (humans 700–950).

### `bot:strafer:5.2` (strafer, seed 3859071546, 33.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1614 | 0.03961 | -24% |
| reverse_share | 0.5449 | 0.4816 | -9% |
| side_hold_ms | 350 | 200 | -100% ⚠ |
| lean_fight | 0.5666 | 0.6491 | +16% |
| jumps_per_min | 0.6862 | 0.5444 | -3% |
| crouch_per_min | 3.8 | 3.327 | -4% |
| walk_hidden | 0.04377 | 0.04118 | -2% |
| burst_median | 6.798 | 3 | -95% ⚠ |
| aim_height_firing | 0.4661 | 0.448 | -10% |
| hold_angle | 0.2527 | 0.2451 | -2% |
| aim_error_fight_deg | 4.952 | 5.532 | +24% |
| reaction_ms | 115 | 250 | +135% ⚠ |
| mp40_share | 0.03683 | 0 | -4% |

Style fingerprint (2026-10-02, 33.1 min, styles.py features 30/30): z-distance to the human cloud centre 8.3 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.496 (humans 0.191–0.489); fwd_diag 0.116 (humans 0.152–0.491); lf_pure_strafe 0.779 (humans 0.362–0.752); lf_fwd_diag 0.0396 (humans 0.0658–0.568); attack 0.245 (humans 0.276–0.478); p_attack_given_los 0.504 (humans 0.547–0.861); speed_med 127 (humans 136–200); side_hold_p90_ms 500 (humans 700–950); lf_approach 0.0828 (humans 0.131–0.64); lf_retreat 0.18 (humans 0.0243–0.141).

### `bot:presser:4.2` (presser, seed 588158740, 32.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4812 | 0.1635 | -62% ⚠ |
| reverse_share | 0.8389 | 0.7994 | -6% |
| side_hold_ms | 302.9 | 200 | -69% ⚠ |
| lean_fight | 0.9588 | 0.9812 | +4% |
| jumps_per_min | 1.077 | 0.9782 | -2% |
| crouch_per_min | 1.47 | 0.7948 | -6% |
| walk_hidden | 0.004612 | 0.01419 | +9% |
| burst_median | 6.555 | 3 | -89% ⚠ |
| aim_height_firing | 0.352 | 0.288 | -34% ⚠ |
| hold_angle | 0.2402 | 0.1882 | -17% |
| aim_error_fight_deg | 4.382 | 5.87 | +61% ⚠ |
| reaction_ms | 183.1 | 350 | +167% ⚠ |
| mp40_share | 0.9584 | 1 | +4% |

Style fingerprint (2026-10-02, 32.7 min, styles.py features 30/30): z-distance to the human cloud centre 8.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.5 (humans 0.191–0.489); lf_pure_strafe 0.766 (humans 0.362–0.752); lean_any 0.911 (humans 0.358–0.767); lf_lean 0.981 (humans 0.457–0.948); walk 0.00813 (humans 0.0126–0.0881); attack 0.214 (humans 0.276–0.478); p_attack_given_los 0.406 (humans 0.547–0.861); speed_med 122 (humans 136–200); lf_on_target 0.273 (humans 0.274–0.515); lf_height_frac 0.288 (humans 0.352–0.54); yaw_speed_p95 296 (humans 158–271); side_hold_p90_ms 450 (humans 700–950).

### `bot:presser:5.2` (presser, seed 4131749178, 32.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5433 | 0.1759 | -72% ⚠ |
| reverse_share | 0.823 | 0.7908 | -5% |
| side_hold_ms | 298.6 | 200 | -66% ⚠ |
| lean_fight | 0.9579 | 0.9771 | +4% |
| jumps_per_min | 0.5292 | 0.6114 | +2% |
| crouch_per_min | 2.785 | 2.262 | -5% |
| walk_hidden | 0.00312 | 0.01334 | +10% |
| burst_median | 6.511 | 3 | -88% ⚠ |
| aim_height_firing | 0.4126 | 0.348 | -34% ⚠ |
| hold_angle | 0.2204 | 0.2584 | +12% |
| aim_error_fight_deg | 4.873 | 6.47 | +65% ⚠ |
| reaction_ms | 134.4 | 200 | +66% ⚠ |
| mp40_share | 0.08844 | 0 | -9% |

Style fingerprint (2026-10-02, 32.7 min, styles.py features 30/30): z-distance to the human cloud centre 8.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.898 (humans 0.358–0.767); lf_lean 0.977 (humans 0.457–0.948); walk 0.00802 (humans 0.0126–0.0881); attack 0.241 (humans 0.276–0.478); p_attack_given_los 0.513 (humans 0.547–0.861); speed_med 115 (humans 136–200); lf_on_target 0.256 (humans 0.274–0.515); lf_height_frac 0.348 (humans 0.352–0.54); yaw_speed_p95 312 (humans 158–271); side_hold_p90_ms 400 (humans 700–950).

### `bot:stopper:4.3` (stopper, seed 97502759, 33.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.06574 | +0% |
| reverse_share | 0.6408 | 0.5866 | -8% |
| side_hold_ms | 200 | 200 | +0% |
| lean_fight | 0.8757 | 0.9031 | +5% |
| jumps_per_min | 1.606 | 1.863 | +5% |
| crouch_per_min | 2.514 | 2.374 | -1% |
| walk_hidden | 0.09232 | 0.08268 | -9% |
| burst_median | 3 | 3 | +0% |
| aim_height_firing | 0.4804 | 0.481 | +0% |
| hold_angle | 0.418 | 0.2202 | -64% ⚠ |
| aim_error_fight_deg | 5.509 | 9.129 | +147% ⚠ |
| reaction_ms | 127.6 | 150 | +22% |
| mp40_share | 0.1179 | 0 | -12% |

Style fingerprint (2026-10-02, 33.3 min, styles.py features 30/30): z-distance to the human cloud centre 10.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0708 (humans 0.152–0.491); neutral 0.423 (humans 0.0872–0.296); lf_fwd_diag 0.0657 (humans 0.0658–0.568); attack 0.207 (humans 0.276–0.478); p_attack_given_los 0.522 (humans 0.547–0.861); speed_med 86.2 (humans 136–200); still 0.253 (humans 0.0832–0.227); lf_err_med 9.13 (humans 4.08–6.63); side_hold_p90_ms 450 (humans 700–950); lf_approach 0.127 (humans 0.131–0.64); lf_retreat 0.191 (humans 0.0243–0.141).

### `bot:strafer:5.3` (strafer, seed 2592015535, 33.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.119 | 0.04156 | -15% |
| reverse_share | 0.4951 | 0.4397 | -8% |
| side_hold_ms | 295 | 200 | -63% ⚠ |
| lean_fight | 0.4565 | 0.4155 | -8% |
| jumps_per_min | 5.657 | 5.739 | +2% |
| crouch_per_min | 11.59 | 10.94 | -6% |
| walk_hidden | 0 | 0.01046 | +10% |
| burst_median | 3.16 | 3 | -4% |
| aim_height_firing | 0.54 | 0.5295 | -6% |
| hold_angle | 0.3001 | 0.3197 | +6% |
| aim_error_fight_deg | 6.024 | 9.825 | +155% ⚠ |
| reaction_ms | 165.9 | 200 | +34% ⚠ |
| mp40_share | 0.4858 | 1 | +51% ⚠ |

Style fingerprint (2026-10-02, 33.3 min, styles.py features 30/30): z-distance to the human cloud centre 11.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.2 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0991 (humans 0.152–0.491); lf_pure_strafe 0.806 (humans 0.362–0.752); lf_fwd_diag 0.0416 (humans 0.0658–0.568); lean_any 0.251 (humans 0.358–0.767); lf_lean 0.416 (humans 0.457–0.948); ducked 0.132 (humans 0.0109–0.0741); walk 0.00679 (humans 0.0126–0.0881); attack 0.193 (humans 0.276–0.478); p_attack_given_los 0.438 (humans 0.547–0.861); speed_med 113 (humans 136–200); lf_err_med 9.82 (humans 4.08–6.63); yaw_speed_p95 284 (humans 158–271); side_hold_p90_ms 500 (humans 700–950); lf_approach 0.107 (humans 0.131–0.64); lf_retreat 0.208 (humans 0.0243–0.141).

### `bot:strafer:4.3` (strafer, seed 1086809828, 33.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1632 | 0.1047 | -11% |
| reverse_share | 0.4593 | 0.4012 | -8% |
| side_hold_ms | 314.9 | 250 | -43% ⚠ |
| lean_fight | 0.5599 | 0.5896 | +6% |
| jumps_per_min | 4.813 | 5.293 | +9% |
| crouch_per_min | 9.497 | 8.771 | -7% |
| walk_hidden | 0 | 0.0122 | +12% |
| burst_median | 4.589 | 3 | -40% ⚠ |
| aim_height_firing | 0.4741 | 0.474 | -0% |
| hold_angle | 0.3025 | 0.2553 | -15% |
| aim_error_fight_deg | 6.527 | 9.46 | +119% ⚠ |
| reaction_ms | 158.4 | 225 | +67% ⚠ |
| mp40_share | 0.9804 | 1 | +2% |

Style fingerprint (2026-10-02, 33.1 min, styles.py features 30/30): z-distance to the human cloud centre 10.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.133 (humans 0.152–0.491); ducked 0.118 (humans 0.0109–0.0741); walk 0.00723 (humans 0.0126–0.0881); attack 0.217 (humans 0.276–0.478); p_attack_given_los 0.476 (humans 0.547–0.861); speed_med 126 (humans 136–200); still 0.0743 (humans 0.0832–0.227); lf_err_med 9.46 (humans 4.08–6.63); yaw_speed_med 30.1 (humans 12.6–30); yaw_speed_p95 308 (humans 158–271); side_hold_p90_ms 500 (humans 700–950); lf_retreat 0.189 (humans 0.0243–0.141).

### `bot:strafer:5.4` (strafer, seed 821368291, 33.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.166 | 0.0796 | -17% |
| reverse_share | 0.4801 | 0.4287 | -7% |
| side_hold_ms | 349.1 | 200 | -99% ⚠ |
| lean_fight | 0.6654 | 0.6848 | +4% |
| jumps_per_min | 5.052 | 4.809 | -5% |
| crouch_per_min | 10.37 | 9.436 | -9% |
| walk_hidden | 0.0009382 | 0.01007 | +9% |
| burst_median | 4.755 | 3 | -44% ⚠ |
| aim_height_firing | 0.4434 | 0.426 | -9% |
| hold_angle | 0.3191 | 0.2483 | -23% |
| aim_error_fight_deg | 4.261 | 8.242 | +162% ⚠ |
| reaction_ms | 175 | 250 | +75% ⚠ |
| mp40_share | 0.1202 | 0 | -12% |

Style fingerprint (2026-10-02, 33.1 min, styles.py features 30/30): z-distance to the human cloud centre 9.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.127 (humans 0.152–0.491); ducked 0.0994 (humans 0.0109–0.0741); walk 0.00612 (humans 0.0126–0.0881); attack 0.218 (humans 0.276–0.478); p_attack_given_los 0.461 (humans 0.547–0.861); speed_med 126 (humans 136–200); still 0.0778 (humans 0.0832–0.227); lf_err_med 8.24 (humans 4.08–6.63); yaw_speed_p95 273 (humans 158–271); side_hold_p90_ms 500 (humans 700–950); lf_retreat 0.208 (humans 0.0243–0.141).

## The owner against the bots (`pstN@vsbot`)

Kills of bots 0, deaths to bots 0, suicides 0. 

0 statistics of the owner against bots fall outside the human CI (owner column below, marked •).

## All statistics

Pooled bots are reweighted to the human family mix; humans and the owner are pooled over their duel time. ▲ marks a tell. n = units (ticks, runs, shots, sightings, ...) over the blocks.

### Movement

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time: hidden, not firing ▲ | 59.9% [58.1%, 61.7%] | 49.1% [47.4%, 51.2%] | - | 619030 |
| Share of duel time: hidden, firing | 14.8% [14.3%, 15.3%] | 15.8% [14.9%, 17.0%] | - | 619030 |
| Share of duel time: LOS, not firing ▲ | 4.3% [3.9%, 4.6%] | 7.0% [5.5%, 8.4%] | - | 619030 |
| Share of duel time: LOS firefight ▲ | 8.0% [7.3%, 8.6%] | 17.6% [16.5%, 18.9%] | - | 619030 |
| Share of duel time: reloading ▲ | 13.1% [12.1%, 13.9%] | 10.5% [9.6%, 11.5%] | - | 619030 |
| Key chord pure strafe (hidden, not firing) ▲ | 33.8% [30.8%, 36.0%] | 26.0% [23.5%, 29.4%] | - | 369860 |
| Key chord fwd diag (hidden, not firing) ▲ | 10.4% [9.3%, 11.2%] | 29.9% [26.7%, 33.2%] | - | 369860 |
| Key chord forward (hidden, not firing) ▲ | 8.5% [7.7%, 9.2%] | 11.9% [10.9%, 12.8%] | - | 369860 |
| Key chord neutral (hidden, not firing) ▲ | 38.8% [35.9%, 42.5%] | 26.1% [23.1%, 28.6%] | - | 369860 |
| Key chord back any (hidden, not firing) ▲ | 8.6% [8.1%, 9.0%] | 6.1% [5.3%, 7.1%] | - | 369860 |
| Key chord pure strafe (hidden, firing) ▲ | 52.9% [49.9%, 55.7%] | 44.9% [41.6%, 48.0%] | - | 91296 |
| Key chord fwd diag (hidden, firing) ▲ | 11.6% [10.5%, 12.9%] | 28.6% [23.9%, 32.8%] | - | 91296 |
| Key chord forward (hidden, firing) | 5.1% [4.5%, 5.7%] | 6.6% [5.7%, 7.4%] | - | 91296 |
| Key chord neutral (hidden, firing) ▲ | 21.4% [18.4%, 25.0%] | 13.2% [11.2%, 15.7%] | - | 91296 |
| Key chord back any (hidden, firing) ▲ | 8.9% [8.1%, 9.6%] | 6.7% [5.9%, 7.9%] | - | 91296 |
| Key chord pure strafe (LOS, not firing) ▲ | 40.5% [37.6%, 43.8%] | 26.7% [20.8%, 34.2%] | - | 26978 |
| Key chord fwd diag (LOS, not firing) ▲ | 12.5% [10.7%, 14.2%] | 29.4% [23.0%, 38.1%] | - | 26978 |
| Key chord forward (LOS, not firing) | 7.1% [6.4%, 7.8%] | 7.5% [5.9%, 9.5%] | - | 26978 |
| Key chord neutral (LOS, not firing) | 29.2% [26.4%, 32.3%] | 31.5% [15.3%, 45.4%] | - | 26978 |
| Key chord back any (LOS, not firing) ▲ | 10.7% [9.8%, 11.8%] | 4.8% [3.4%, 6.7%] | - | 26978 |
| Key chord pure strafe (LOS firefight) ▲ | 69.8% [65.8%, 72.7%] | 58.0% [53.1%, 62.2%] | - | 49239 |
| Key chord fwd diag (LOS firefight) ▲ | 11.2% [7.9%, 15.2%] | 30.5% [25.6%, 36.7%] | - | 49239 |
| Key chord forward (LOS firefight) | 1.1% [0.8%, 1.3%] | 1.2% [1.0%, 1.4%] | - | 49239 |
| Key chord neutral (LOS firefight) ▲ | 9.4% [7.5%, 12.3%] | 5.1% [3.9%, 6.3%] | - | 49239 |
| Key chord back any (LOS firefight) ▲ | 8.6% [7.8%, 9.5%] | 5.2% [4.5%, 6.1%] | - | 49239 |
| Key chord pure strafe (reloading) ▲ | 27.9% [25.7%, 29.8%] | 22.3% [19.6%, 24.4%] | - | 81654 |
| Key chord fwd diag (reloading) ▲ | 17.3% [15.8%, 18.9%] | 36.8% [34.1%, 39.9%] | - | 81654 |
| Key chord forward (reloading) ▲ | 15.3% [13.5%, 17.3%] | 26.1% [23.8%, 28.6%] | - | 81654 |
| Key chord neutral (reloading) ▲ | 27.2% [24.8%, 29.9%] | 8.3% [7.3%, 9.5%] | - | 81654 |
| Key chord back any (reloading) ▲ | 12.3% [11.6%, 13.0%] | 6.5% [5.3%, 7.6%] | - | 81654 |
| Strafe end is a direct reverse (hidden, not firing) | 50.5% [45.5%, 54.7%] | 46.1% [41.1%, 51.2%] | - | 39681 |
| Strafe end is a direct reverse (hidden, firing) | 59.2% [49.1%, 66.1%] | 67.2% [59.7%, 74.3%] | - | 12703 |
| Strafe end is a direct reverse (LOS, not firing) | 52.5% [45.6%, 57.9%] | 52.8% [45.1%, 60.4%] | - | 3230 |
| Strafe end is a direct reverse (LOS firefight) | 62.4% [47.5%, 72.2%] | 73.3% [65.9%, 80.6%] | - | 8037 |
| Strafe end is a direct reverse (reloading) | 45.4% [39.6%, 50.6%] | 37.7% [32.5%, 42.7%] | - | 8910 |
| Strafe switch hazard per tick at hold age 50 ms (LOS firefight) ▲ | 9.4% [7.6%, 11.8%] | 1.2% [0.9%, 1.6%] | - | 8791 |
| Strafe switch hazard per tick at hold age 100 ms (LOS firefight) ▲ | 12.7% [11.7%, 14.0%] | 5.8% [5.2%, 6.8%] | - | 7779 |
| Strafe switch hazard per tick at hold age 150 ms (LOS firefight) ▲ | 19.2% [16.9%, 21.2%] | 12.4% [11.1%, 13.9%] | - | 6707 |
| Strafe switch hazard per tick at hold age 200 ms (LOS firefight) ▲ | 25.4% [23.9%, 26.9%] | 17.0% [15.4%, 18.7%] | - | 5263 |
| Strafe switch hazard per tick at hold age 250 ms (LOS firefight) ▲ | 24.5% [22.0%, 27.2%] | 17.4% [16.0%, 19.0%] | - | 3836 |
| Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) ▲ | 25.8% [24.4%, 27.5%] | 15.6% [14.7%, 16.9%] | - | 7328 |
| Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) ▲ | 22.6% [20.2%, 25.4%] | 16.1% [14.9%, 17.5%] | - | 3621 |
| Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) ▲ | 22.6% [16.9%, 27.2%] | 13.7% [12.0%, 15.6%] | - | 296 |
| Side-hold duration (LOS firefight) p25 ▲ | 150 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | - | 7862 |
| Side-hold duration (LOS firefight) p50 ▲ | 200 ms [200 ms, 200 ms] | 300 ms [250 ms, 300 ms] | - | 7862 |
| Side-hold duration (LOS firefight) p75 ▲ | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | - | 7862 |
| Side-hold duration (LOS firefight) p90 ▲ | 450 ms [450 ms, 500 ms] | 700 ms [650 ms, 700 ms] | - | 7862 |
| Side-hold duration (all contexts) p50 ▲ | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | - | 72031 |
| Side-hold duration (all contexts) p90 ▲ | 500 ms [500 ms, 500 ms] | 800 ms [800 ms, 850 ms] | - | 72031 |
| Neutral gap between strafes (LOS firefight) p50 | 50 ms [50 ms, 50 ms] | 50 ms [50 ms, 50 ms] | - | 2824 |
| Neutral gap between strafes (LOS firefight) p90 | 50 ms [50 ms, 100 ms] | 100 ms [100 ms, 150 ms] | - | 2824 |
| Approaching >40 u/s in LOS firefight at distance (0, 96] ▲ | 22.9% [19.0%, 26.2%] | 37.6% [33.7%, 42.3%] | - | 3188 |
| Approaching >40 u/s in LOS firefight at distance (96, 160] ▲ | 16.1% [13.7%, 18.9%] | 36.4% [32.6%, 41.2%] | - | 6039 |
| Approaching >40 u/s in LOS firefight at distance (160, 224] ▲ | 15.0% [12.5%, 17.1%] | 35.4% [30.1%, 42.0%] | - | 8600 |
| Approaching >40 u/s in LOS firefight at distance (224, 288] ▲ | 14.3% [12.1%, 16.4%] | 38.2% [32.3%, 46.4%] | - | 7710 |
| Approaching >40 u/s in LOS firefight at distance (288, 384] ▲ | 16.0% [12.8%, 19.8%] | 39.4% [32.7%, 47.2%] | - | 8627 |
| Approaching >40 u/s in LOS firefight at distance (384, 512] ▲ | 15.4% [10.6%, 22.7%] | 41.8% [33.8%, 49.1%] | - | 7969 |
| Approaching >40 u/s in LOS firefight at distance (512, 768] | 18.1% [11.0%, 33.9%] | 33.1% [25.5%, 41.5%] | - | 6428 |
| Approaching >40 u/s in LOS firefight at distance (768, 1200] | 13.6% [8.6%, 27.8%] | 28.2% [17.6%, 42.7%] | - | 678 |
| Retreating >40 u/s in LOS firefight at distance (0, 96] | 28.2% [24.9%, 31.9%] | 22.2% [18.9%, 25.6%] | - | 3188 |
| Retreating >40 u/s in LOS firefight at distance (96, 160] ▲ | 23.0% [21.2%, 24.8%] | 10.5% [8.5%, 12.6%] | - | 6039 |
| Retreating >40 u/s in LOS firefight at distance (160, 224] ▲ | 19.2% [18.2%, 20.4%] | 6.9% [5.5%, 8.7%] | - | 8600 |
| Retreating >40 u/s in LOS firefight at distance (224, 288] ▲ | 14.6% [12.8%, 16.1%] | 5.3% [4.2%, 6.8%] | - | 7710 |
| Retreating >40 u/s in LOS firefight at distance (288, 384] ▲ | 12.7% [11.4%, 14.1%] | 5.9% [4.6%, 7.3%] | - | 8627 |
| Retreating >40 u/s in LOS firefight at distance (384, 512] ▲ | 9.6% [7.7%, 11.5%] | 4.8% [3.8%, 6.0%] | - | 7969 |
| Retreating >40 u/s in LOS firefight at distance (512, 768] ▲ | 12.4% [7.4%, 15.1%] | 4.0% [3.0%, 5.4%] | - | 6428 |
| Retreating >40 u/s in LOS firefight at distance (768, 1200] | 20.2% [0.0%, 33.7%] | 8.3% [1.5%, 15.5%] | - | 678 |
| Standing still (<5 u/s), all duel time | 17.7% [16.1%, 20.0%] | 15.7% [13.4%, 17.7%] | - | 619030 |
| Standing still (<5 u/s) (hidden, not firing) | 23.4% [21.2%, 26.3%] | 22.5% [19.5%, 25.1%] | - | 369860 |
| Standing still (<5 u/s) (hidden, firing) | 10.3% [8.9%, 11.8%] | 10.2% [9.1%, 11.7%] | - | 91296 |
| Standing still (<5 u/s) (LOS, not firing) | 14.9% [13.0%, 17.0%] | 29.6% [12.6%, 44.0%] | - | 26978 |
| Standing still (<5 u/s) (LOS firefight) | 2.8% [2.4%, 3.4%] | 2.3% [1.8%, 2.7%] | - | 49239 |
| Standing still (<5 u/s) (reloading) ▲ | 10.3% [8.9%, 11.9%] | 5.2% [4.3%, 6.2%] | - | 81654 |
| Lean held, all duel time | 60.5% [55.1%, 65.2%] | 59.7% [55.3%, 64.0%] | - | 619030 |
| Lean held in LOS firefights | 79.5% [76.0%, 82.5%] | 81.3% [76.6%, 85.8%] | - | 49239 |
| Jump presses per minute | 2.88/min [1.97/min, 3.69/min] | 1.87/min [1.41/min, 2.44/min] | - | 619030 |
| Crouch presses per minute | 5.85/min [4.18/min, 7.27/min] | 4.33/min [3.22/min, 5.51/min] | - | 619030 |
| Walk presses per minute ▲ | 3.80/min [3.05/min, 4.45/min] | 7.54/min [6.90/min, 8.11/min] | - | 619030 |
| Walking while hidden and not firing | 4.3% [2.6%, 5.5%] | 5.9% [5.0%, 7.0%] | - | 369860 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Press hazard with LOS and aim within 3 half-widths, LOS gained 0 ms ago ▲ | 21.5% [17.1%, 25.0%] | 28.2% [25.8%, 30.8%] | - | 1208 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 50-100 ms ago | 32.0% [27.8%, 36.1%] | 33.0% [30.8%, 35.6%] | - | 1749 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 150-250 ms ago | 38.3% [33.5%, 42.9%] | 35.2% [32.9%, 38.1%] | - | 1358 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago ▲ | 35.9% [32.1%, 40.6%] | 28.0% [25.2%, 32.0%] | - | 886 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 550-1000 ms ago | 28.2% [23.1%, 34.0%] | 21.0% [16.7%, 24.6%] | - | 721 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago ▲ | 20.4% [18.3%, 23.6%] | 7.4% [4.7%, 10.8%] | - | 1279 |
| Press hazard while hidden, LOS lost 0 ms ago | 11.2% [9.2%, 13.6%] | 12.3% [10.8%, 14.5%] | - | 2312 |
| Press hazard while hidden, LOS lost 50-100 ms ago | 9.3% [8.1%, 10.5%] | 9.5% [8.5%, 10.9%] | - | 3233 |
| Press hazard while hidden, LOS lost 150-250 ms ago ▲ | 6.8% [5.9%, 7.9%] | 4.6% [3.9%, 5.5%] | - | 4500 |
| Press hazard while hidden, LOS lost 300-500 ms ago ▲ | 9.9% [9.3%, 10.5%] | 7.0% [6.0%, 8.1%] | - | 12448 |
| Press hazard while hidden, LOS lost 550-1000 ms ago ▲ | 4.5% [4.1%, 4.8%] | 3.5% [3.2%, 3.9%] | - | 23428 |
| Press hazard while hidden, LOS lost over 1 s ago | 3.3% [3.1%, 3.6%] | 4.1% [3.5%, 4.6%] | - | 316210 |
| Release hazard, LOS, aim error in half-widths (0, 1] | 1.2% [0.9%, 1.6%] | 1.4% [1.1%, 1.7%] | - | 8148 |
| Release hazard, LOS, aim error in half-widths (1, 2] | 1.3% [1.1%, 1.4%] | 1.5% [1.3%, 1.9%] | - | 14772 |
| Release hazard, LOS, aim error in half-widths (2, 3] | 1.5% [1.2%, 1.7%] | 1.5% [1.2%, 2.0%] | - | 11057 |
| Release hazard, LOS, aim error in half-widths (3, 4] | 1.5% [1.3%, 1.8%] | 1.4% [1.0%, 2.0%] | - | 6315 |
| Release hazard, LOS, aim error in half-widths (4, 6] | 2.1% [1.5%, 2.6%] | 1.7% [1.3%, 2.3%] | - | 4965 |
| Release hazard, LOS, aim error in half-widths (6, 10] | 7.8% [5.5%, 10.0%] | 6.9% [4.4%, 9.9%] | - | 1866 |
| Release hazard, LOS, aim error in half-widths (10, 1000] | 20.9% [16.5%, 24.1%] | 32.7% [22.9%, 41.8%] | - | 1100 |
| Release hazard while hidden, LOS lost 0-100 ms ago | 5.8% [5.2%, 6.6%] | 6.3% [5.4%, 7.5%] | - | 9845 |
| Release hazard while hidden, LOS lost 150-250 ms ago | 13.0% [11.6%, 14.4%] | 16.5% [13.8%, 18.7%] | - | 6015 |
| Release hazard while hidden, LOS lost 300-450 ms ago ▲ | 26.7% [24.8%, 29.1%] | 31.5% [29.6%, 33.4%] | - | 5996 |
| Release hazard while hidden, LOS lost 500-950 ms ago ▲ | 13.1% [12.4%, 13.8%] | 10.1% [9.2%, 11.3%] | - | 7946 |
| Release hazard while hidden, LOS lost 1 s or more ago ▲ | 16.1% [15.8%, 16.5%] | 11.3% [9.8%, 13.5%] | - | 59611 |
| Fire held, LOS, aim error in half-widths (0, 0.5] | 86.6% [84.0%, 89.2%] | 89.6% [87.0%, 91.7%] | - | 2593 |
| Fire held, LOS, aim error in half-widths (0.5, 1] | 85.3% [82.8%, 87.6%] | 88.9% [86.9%, 90.5%] | - | 7104 |
| Fire held, LOS, aim error in half-widths (1, 1.5] | 84.5% [81.7%, 86.9%] | 86.3% [84.5%, 87.8%] | - | 9050 |
| Fire held, LOS, aim error in half-widths (1.5, 2] | 82.5% [80.2%, 84.9%] | 81.9% [79.8%, 83.7%] | - | 8935 |
| Fire held, LOS, aim error in half-widths (2, 3] | 80.4% [78.2%, 83.0%] | 81.0% [79.4%, 82.7%] | - | 13983 |
| Fire held, LOS, aim error in half-widths (3, 4] | 76.4% [73.6%, 79.0%] | 78.8% [76.1%, 81.2%] | - | 8375 |
| Fire held, LOS, aim error in half-widths (4, 6] | 64.4% [60.7%, 68.5%] | 67.7% [63.0%, 71.3%] | - | 7847 |
| Fire held, LOS, aim error in half-widths (6, 10] ▲ | 38.3% [32.2%, 43.7%] | 22.4% [17.1%, 27.1%] | - | 4998 |
| Fire held, LOS, aim error in half-widths (10, 20] ▲ | 12.8% [9.1%, 17.5%] | 4.6% [2.4%, 6.9%] | - | 4090 |
| Fire held, LOS, aim error in half-widths (20, 1000] ▲ | 6.6% [4.6%, 8.3%] | 0.9% [0.4%, 3.1%] | - | 8345 |
| Fire held without LOS ▲ | 19.8% [18.7%, 20.9%] | 24.5% [22.7%, 26.3%] | - | 451670 |
| Attack hold duration p50 | 200 ms [200 ms, 250 ms] | 150 ms [150 ms, 200 ms] | - | 16440 |
| Attack hold duration p90 | 750 ms [750 ms, 800 ms] | 900 ms [800 ms, 950 ms] | - | 16440 |
| Attack holds that are taps (<=100 ms) ▲ | 34.2% [33.5%, 35.0%] | 44.3% [41.8%, 46.5%] | - | 16440 |
| Gap between attack holds p50 | 400 ms [400 ms, 450 ms] | 400 ms [350 ms, 400 ms] | - | 15935 |

### Aim

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error firing with LOS at (0, 128] u ▲ | 17.86° [17.19°, 18.79°] | 14.28° [13.53°, 15.19°] | - | 4372 |
| Crosshair on body firing with LOS at (0, 128] u ▲ | 40.4% [38.2%, 42.8%] | 48.0% [44.9%, 51.5%] | - | 4372 |
| Median aim error firing with LOS at (128, 192] u ▲ | 11.13° [10.57°, 11.74°] | 8.99° [8.66°, 9.45°] | - | 7064 |
| Crosshair on body firing with LOS at (128, 192] u ▲ | 32.8% [30.3%, 35.6%] | 42.5% [39.8%, 44.7%] | - | 7064 |
| Median aim error firing with LOS at (192, 256] u ▲ | 8.28° [7.90°, 8.82°] | 6.68° [6.36°, 7.10°] | - | 8153 |
| Crosshair on body firing with LOS at (192, 256] u ▲ | 31.6% [28.5%, 33.8%] | 42.0% [39.5%, 44.4%] | - | 8153 |
| Median aim error firing with LOS at (256, 384] u | 5.75° [5.49°, 6.15°] | 5.23° [4.96°, 5.59°] | - | 13202 |
| Crosshair on body firing with LOS at (256, 384] u ▲ | 31.1% [28.7%, 33.1%] | 39.0% [36.0%, 41.4%] | - | 13202 |
| Median aim error firing with LOS at (384, 512] u | 4.03° [3.81°, 4.35°] | 3.92° [3.67°, 4.18°] | - | 8410 |
| Crosshair on body firing with LOS at (384, 512] u | 30.5% [27.8%, 32.7%] | 35.4% [32.6%, 38.3%] | - | 8410 |
| Median aim error firing with LOS at (512, 768] u | 3.04° [2.77°, 3.53°] | 3.07° [2.80°, 3.35°] | - | 7360 |
| Crosshair on body firing with LOS at (512, 768] u | 29.1% [25.1%, 31.7%] | 32.0% [29.1%, 35.1%] | - | 7360 |
| Median aim error firing with LOS at (768, 1200] u | 2.03° [1.84°, 2.28°] | 2.14° [1.84°, 2.58°] | - | 678 |
| Crosshair on body firing with LOS at (768, 1200] u | 34.0% [29.4%, 41.7%] | 31.5% [24.3%, 39.5%] | - | 678 |
| Aim height (fraction of body) firing with LOS | 0.437 [0.426, 0.453] | 0.446 [0.418, 0.468] | - | 49239 |
| Aim height (fraction of body) with LOS, not firing | 0.634 [0.623, 0.649] | 0.662 [0.576, 0.751] | - | 50140 |

### View

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw speed (hidden, not firing) p50 ▲ | 12°/s [11°/s, 13°/s] | 8°/s [6°/s, 10°/s] | - | 368700 |
| Yaw speed (hidden, not firing) p99 ▲ | 638°/s [621°/s, 657°/s] | 544°/s [521°/s, 580°/s] | - | 368700 |
| Mouse still between ticks (hidden, not firing) | 32.2% [31.9%, 32.6%] | 32.1% [28.8%, 36.0%] | - | 369860 |
| Yaw speed (hidden, firing) p50 ▲ | 29°/s [26°/s, 32°/s] | 23°/s [21°/s, 25°/s] | - | 91296 |
| Yaw speed (hidden, firing) p99 ▲ | 463°/s [437°/s, 484°/s] | 194°/s [182°/s, 204°/s] | - | 91296 |
| Mouse still between ticks (hidden, firing) ▲ | 8.9% [8.6%, 9.2%] | 10.9% [10.1%, 11.8%] | - | 91296 |
| Yaw speed (LOS, not firing) p50 ▲ | 35°/s [32°/s, 39°/s] | 11°/s [0°/s, 26°/s] | - | 26969 |
| Yaw speed (LOS, not firing) p99 ▲ | 811°/s [752°/s, 860°/s] | 583°/s [505°/s, 695°/s] | - | 26969 |
| Mouse still between ticks (LOS, not firing) | 21.6% [20.5%, 22.6%] | 36.7% [20.8%, 49.8%] | - | 26978 |
| Yaw speed (LOS firefight) p50 ▲ | 50°/s [44°/s, 56°/s] | 39°/s [36°/s, 41°/s] | - | 49239 |
| Yaw speed (LOS firefight) p99 ▲ | 576°/s [530°/s, 623°/s] | 305°/s [292°/s, 318°/s] | - | 49239 |
| Mouse still between ticks (LOS firefight) ▲ | 3.2% [3.1%, 3.3%] | 1.6% [1.4%, 1.8%] | - | 49239 |
| Yaw speed (reloading) p50 | 19°/s [18°/s, 21°/s] | 18°/s [16°/s, 20°/s] | - | 81654 |
| Yaw speed (reloading) p99 ▲ | 609°/s [580°/s, 635°/s] | 945°/s [909°/s, 991°/s] | - | 81654 |
| Mouse still between ticks (reloading) | 24.5% [23.9%, 25.2%] | 26.3% [25.0%, 27.6%] | - | 81654 |
| Peak yaw speed of 90-135 deg turns p50 | 622°/s [606°/s, 633°/s] | 619°/s [595°/s, 655°/s] | - | 2420 |
| Peak yaw speed of 135-180 deg turns p50 | 886°/s [864°/s, 903°/s] | 852°/s [828°/s, 887°/s] | - | 1358 |
| Peak yaw speed of 180-360 deg turns p50 | 1092°/s [1066°/s, 1150°/s] | 1061°/s [1020°/s, 1126°/s] | - | 321 |

### Belief

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw error to the hidden opponent, last seen <=250 ms | 5.90° [5.22°, 6.57°] | 4.92° [4.56°, 5.26°] | - | 28343 |
| View within 30 deg of the hidden opponent, last seen <=250 ms ▲ | 86.8% [83.2%, 89.3%] | 92.7% [91.9%, 93.7%] | - | 28343 |
| Yaw error to the hidden opponent, last seen 250-500 ms | 6.06° [5.35°, 6.70°] | 6.66° [5.99°, 7.28°] | - | 17525 |
| View within 30 deg of the hidden opponent, last seen 250-500 ms | 90.2% [87.7%, 91.8%] | 92.2% [91.3%, 93.4%] | - | 17525 |
| Yaw error to the hidden opponent, last seen 0.5-1 s | 8.36° [7.52°, 9.10°] | 8.28° [7.48°, 9.11°] | - | 25401 |
| View within 30 deg of the hidden opponent, last seen 0.5-1 s | 87.3% [85.3%, 89.2%] | 88.0% [86.8%, 89.9%] | - | 25401 |
| Yaw error to the hidden opponent, last seen 1-2 s | 10.95° [9.55°, 12.29°] | 11.11° [9.95°, 12.13°] | - | 35313 |
| View within 30 deg of the hidden opponent, last seen 1-2 s | 81.0% [78.3%, 84.7%] | 78.9% [77.1%, 81.1%] | - | 35313 |
| Yaw error to the hidden opponent, last seen 2-4 s | 19.65° [16.24°, 22.54°] | 16.93° [15.18°, 19.08°] | - | 60650 |
| View within 30 deg of the hidden opponent, last seen 2-4 s | 61.4% [57.9%, 66.1%] | 64.3% [61.3%, 67.3%] | - | 60650 |
| Yaw error to the hidden opponent, last seen 4-8 s ▲ | 18.19° [14.48°, 20.46°] | 10.29° [9.19°, 11.80°] | - | 91853 |
| View within 30 deg of the hidden opponent, last seen 4-8 s ▲ | 63.6% [60.6%, 67.9%] | 76.4% [73.1%, 79.3%] | - | 91853 |
| Yaw error to the hidden opponent, last seen >8 s | 19.64° [16.01°, 23.32°] | 20.07° [12.82°, 29.16°] | - | 110970 |
| View within 30 deg of the hidden opponent, last seen >8 s | 62.7% [57.9%, 67.8%] | 65.1% [54.8%, 74.8%] | - | 110970 |
| Yaw error to the hidden opponent, last seen never seen this life | 17.39° [15.41°, 20.18°] | 19.23° [16.93°, 24.71°] | - | 149590 |
| View within 30 deg of the hidden opponent, last seen never seen this life | 65.0% [61.2%, 68.4%] | 62.5% [56.0%, 67.2%] | - | 149590 |

### Space

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) ▲ | 38.0% [33.6%, 40.2%] | 49.4% [44.0%, 50.8%] | - | 148750 |
| Share of duel time in the busiest 10% of 32u cells (dm/main) ▲ | 34.2% [28.2%, 35.8%] | 45.2% [40.5%, 48.6%] | - | 153180 |
| Share of duel time in the busiest 10% of 32u cells (dm/vents) | 40.1% [35.3%, 44.2%] | 44.3% [39.2%, 46.8%] | - | 157860 |
| Share of duel time in the busiest 10% of 32u cells (dm/downladder) ▲ | 36.4% [30.5%, 37.0%] | 61.4% [42.9%, 63.8%] | - | 159230 |

### Acquisition

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error at -500 ms from sight gain | 15.24° [12.47°, 17.89°] | 13.98° [12.90°, 15.10°] | - | 3216 |
| Median aim error at -200 ms from sight gain ▲ | 13.45° [11.73°, 15.29°] | 8.33° [7.70°, 8.92°] | - | 3219 |
| Median aim error at +0 ms from sight gain ▲ | 12.22° [10.73°, 13.60°] | 5.22° [4.88°, 5.61°] | - | 3229 |
| Median aim error at +100 ms from sight gain ▲ | 11.05° [9.46°, 12.37°] | 5.97° [5.54°, 6.43°] | - | 3229 |
| Median aim error at +200 ms from sight gain ▲ | 8.83° [7.62°, 10.00°] | 4.92° [4.54°, 5.23°] | - | 3229 |
| Median aim error at +300 ms from sight gain ▲ | 7.98° [6.84°, 9.11°] | 4.53° [4.12°, 4.88°] | - | 3175 |
| Median aim error at +500 ms from sight gain ▲ | 6.92° [5.96°, 7.89°] | 5.47° [5.08°, 5.88°] | - | 3017 |
| Median aim error at +1000 ms from sight gain | 7.57° [6.48°, 8.73°] | 7.25° [6.72°, 7.70°] | - | 2585 |
| Fire held at -500 ms from sight gain ▲ | 21.8% [20.8%, 23.1%] | 18.2% [16.5%, 20.2%] | - | 3229 |
| Fire held at -200 ms from sight gain | 21.7% [19.4%, 24.5%] | 21.7% [19.7%, 23.9%] | - | 3229 |
| Fire held at +0 ms from sight gain ▲ | 25.5% [23.8%, 27.5%] | 42.5% [39.1%, 46.0%] | - | 3229 |
| Fire held at +100 ms from sight gain ▲ | 35.4% [32.4%, 38.1%] | 61.4% [57.8%, 64.6%] | - | 3229 |
| Fire held at +200 ms from sight gain ▲ | 45.9% [43.1%, 48.2%] | 77.0% [73.9%, 79.5%] | - | 3229 |
| Fire held at +400 ms from sight gain ▲ | 59.6% [56.6%, 62.0%] | 87.3% [85.5%, 89.0%] | - | 3160 |
| Fire held at +700 ms from sight gain ▲ | 54.1% [52.0%, 56.1%] | 77.0% [74.3%, 79.7%] | - | 3039 |
| Fire held at +1000 ms from sight gain ▲ | 48.9% [46.9%, 50.6%] | 63.4% [60.9%, 66.4%] | - | 2914 |
| Fire held at +1450 ms from sight gain ▲ | 39.3% [37.8%, 41.0%] | 53.6% [49.3%, 58.5%] | - | 2735 |
| Reaction: first trigger press after a clean sighting p25 | 100 ms [100 ms, 150 ms] | 50 ms [50 ms, 100 ms] | - | 1459 |
| Reaction: first trigger press after a clean sighting p50 | 200 ms [200 ms, 250 ms] | 150 ms [150 ms, 200 ms] | - | 1459 |
| Reaction: first trigger press after a clean sighting p75 ▲ | 400 ms [350 ms, 400 ms] | 300 ms [250 ms, 300 ms] | - | 1459 |
| Reaction: first trigger press after a clean sighting p90 ▲ | 700 ms [650 ms, 750 ms] | 400 ms [400 ms, 450 ms] | - | 1459 |
| Crosshair within half-width+1.5 deg after a clean sighting p50 | 300 ms [250 ms, 300 ms] | 200 ms [200 ms, 250 ms] | - | 1454 |
| Crosshair already on the body at the first visible tick ▲ | 12.7% [11.4%, 13.9%] | 22.6% [20.9%, 24.4%] | - | 3229 |
| Fire already held on the tick before sight (prefire) ▲ | 23.0% [20.9%, 25.2%] | 35.4% [32.2%, 38.6%] | - | 3229 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shots per burst (consecutive 100 ms shots) p50 ▲ | 3 [3, 3] | 5 [4, 5] | - | 15753 |
| Shots per burst (consecutive 100 ms shots) p90 ▲ | 10 [9, 10] | 12 [11, 13] | - | 15753 |

### Combat

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shot accuracy (eligible SMG shots that hit) ▲ | 8.6% [7.8%, 9.5%] | 19.2% [18.1%, 20.3%] | - | 70431 |

### Lives

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Respawn delay after death p50 ▲ | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | - | 1166 |
| Life length (lives ending in death) p50 ▲ | 24.1 s [21.5 s, 26.3 s] | 7.1 s [5.8 s, 8.2 s] | - | 1167 |

### Reload

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Reload within 3 s of a kill, rounds left 0 | 100.0% [100.0%, 100.0%] | 95.5% [86.4%, 100.0%] | - | 23 |
| Reload within 3 s of a kill, rounds left 1-3 ▲ | 100.0% [100.0%, 100.0%] | 96.4% [92.6%, 99.2%] | - | 92 |
| Reload within 3 s of a kill, rounds left 4-6 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 98 |
| Reload within 3 s of a kill, rounds left 7-10 | 99.4% [98.4%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 129 |
| Reload within 3 s of a kill, rounds left 11-15 | 98.6% [97.1%, 99.8%] | 98.8% [97.9%, 99.5%] | - | 189 |
| Reload within 3 s of a kill, rounds left 16-20 | 98.0% [96.3%, 99.6%] | 95.7% [93.5%, 97.7%] | - | 213 |
| Reload within 3 s of a kill, rounds left 21-25 | 78.9% [74.4%, 84.6%] | 83.1% [76.5%, 88.5%] | - | 204 |
| Reload within 3 s of a kill, rounds left 26-29 | 61.6% [51.9%, 73.0%] | 63.4% [52.8%, 75.0%] | - | 179 |
| Reload within 3 s of a kill, rounds left 30-32 | 69.2% [45.3%, 82.1%] | 73.7% [50.0%, 92.3%] | - | 36 |
| Delay from kill to reload (reloads within 3 s) p50 | 650 ms [650 ms, 650 ms] | 600 ms [600 ms, 650 ms] | - | 1034 |
| Reloads started with the opponent dead ▲ | 40.0% [36.8%, 43.7%] | 88.2% [85.5%, 90.4%] | - | 2588 |
| Reloads with the opponent alive that are forced (empty clip) | 77.7% [75.2%, 79.7%] | 77.7% [69.5%, 83.9%] | - | 1553 |

### Fights

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Engagements that end in a kill ▲ | 55.5% [53.1%, 58.8%] | 79.9% [77.9%, 81.9%] | - | 4104 |
| First hitter wins (decisive engagements) | 67.6% [65.8%, 69.4%] | 65.4% [63.7%, 66.7%] | - | 2282 |
| First shooter wins (decisive engagements) | 54.2% [51.2%, 57.1%] | 55.0% [52.5%, 56.6%] | - | 2284 |
| Duration of decisive engagements p50 ▲ | 1550 ms [1400 ms, 1700 ms] | 1250 ms [1150 ms, 1351 ms] | - | 2284 |
| Engagement start distance p50 | 350 u [308 u, 422 u] | 427 u [398 u, 470 u] | - | 4104 |
| Decisive engagements won (subject's own) | 50.3% [46.4%, 53.2%] | 50.0% [45.6%, 54.2%] | - | 2284 |

### Perception

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time with a body part on screen and the centroid hidden ▲ | 4.5% [3.8%, 5.2%] | 10.5% [9.6%, 11.8%] | - | 619030 |
| Fire held with no body part of the enemy visible (loaded, not reloading) | 17.6% [16.6%, 18.7%] | 17.3% [15.8%, 19.2%] | - | 444820 |
| SMG shots that hit, a part on screen, centroid hidden ▲ | 12.4% [11.5%, 13.6%] | 21.7% [18.9%, 24.3%] | - | 6997 |
| SMG shots that hit, centroid visible ▲ | 20.8% [19.0%, 22.5%] | 27.2% [25.8%, 28.5%] | - | 24419 |
| Centroid sightings with a body part on screen first ▲ | 36.5% [33.9%, 38.9%] | 85.9% [84.2%, 87.7%] | - | 3216 |
| Lead of the first part over the centroid p50 | 100 ms [50 ms, 100 ms] | 100 ms [100 ms, 100 ms] | - | 1174 |
| Median aim error at -500 ms from the first visible part ▲ | 12.75° [11.34°, 13.93°] | 15.23° [14.18°, 16.19°] | - | 2107 |
| Median aim error at -200 ms from the first visible part | 10.94° [9.37°, 12.06°] | 10.54° [9.80°, 11.23°] | - | 2107 |
| Median aim error at -100 ms from the first visible part ▲ | 10.35° [9.10°, 11.63°] | 8.00° [7.34°, 8.49°] | - | 2107 |
| Median aim error at +0 ms from the first visible part ▲ | 10.35° [9.26°, 11.23°] | 5.22° [4.91°, 5.53°] | - | 2107 |
| Median aim error at +100 ms from the first visible part ▲ | 9.03° [8.01°, 10.28°] | 4.87° [4.54°, 5.18°] | - | 2107 |
| Median aim error at +200 ms from the first visible part ▲ | 6.45° [5.68°, 7.19°] | 5.26° [4.86°, 5.58°] | - | 2107 |
| Fire already held on the tick before the first part (prefire) | 20.5% [18.6%, 22.3%] | 23.0% [20.3%, 25.5%] | - | 2107 |
| Reaction: first press after a clean sighting, from the first part p50 | 200 ms [200 ms, 200 ms] | 200 ms [200 ms, 250 ms] | - | 1077 |
| Reaction: mean first press after a clean sighting, from the first part | 231 ms [218 ms, 247 ms] | 232 ms [217 ms, 250 ms] | - | 1077 |
| Crosshair within half-width+1.5 deg after a clean sighting, from the first part p50 | 250 ms [250 ms, 250 ms] | 250 ms [200 ms, 250 ms] | - | 1091 |
| First hit after the first part p50 ▲ | 300 ms [300 ms, 350 ms] | 250 ms [200 ms, 250 ms] | - | 1146 |
| Part sightings whose visible bout ends without a hit ▲ | 45.0% [42.2%, 48.7%] | 27.5% [25.7%, 29.7%] | - | 2107 |
| Yaw error to the hidden enemy 500 ms before the first part ▲ | 11.26° [9.92°, 12.27°] | 14.63° [13.70°, 15.63°] | - | 2107 |
| Yaw error to where the enemy will appear, 500 ms before the first part | 9.89° [8.90°, 11.14°] | 10.65° [9.39°, 11.89°] | - | 2107 |
| Crosshair parked within 5 deg of where the enemy will appear, 500 ms before | 29.3% [26.7%, 31.3%] | 29.4% [27.1%, 32.4%] | - | 2107 |
| Crosshair closer to where the enemy will appear than to the enemy, -500 ms ▲ | 54.4% [52.0%, 56.9%] | 67.0% [65.6%, 68.8%] | - | 2107 |
| View turn toward the enemy over the last 500 ms before the first part ▲ | 8.52° [7.22°, 9.45°] | 13.01° [11.44°, 14.55°] | - | 2107 |
| View turn over the last 500 ms before the first part ▲ | 11.60° [10.10°, 12.98°] | 14.53° [13.34°, 15.90°] | - | 2107 |
| Speed 400-200 ms before the first part ▲ | 120 u/s [116 u/s, 122 u/s] | 188 u/s [182 u/s, 193 u/s] | - | 2107 |
| Speed 2-1 s before the first part ▲ | 114 u/s [105 u/s, 119 u/s] | 167 u/s [159 u/s, 176 u/s] | - | 1681 |
| Hidden, no sighting next: crosshair closer to where the enemy will be in 500 ms | 49.8% [49.3%, 50.2%] | 50.5% [49.0%, 51.6%] | - | 421290 |
| Yaw error to the corner, 500 ms before the first part ▲ | 8.20° [7.64°, 9.03°] | 5.81° [5.17°, 6.44°] | - | 1307 |
| Angle to the corner, 500 ms before the first part ▲ | 10.37° [9.48°, 11.51°] | 8.41° [7.70°, 9.19°] | - | 1307 |
| Pitch to the corner (- = corner above the crosshair), 500 ms before the first part | -2.10° [-2.48°, -1.81°] | -2.72° [-3.14°, -2.39°] | - | 1307 |
| Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part ▲ | 2.06° [0.86°, 3.04°] | -1.77° [-2.04°, -1.43°] | - | 1307 |
| Yaw error to the appearance point (sightings with a corner), 500 ms before the first part | 9.17° [8.14°, 10.07°] | 10.34° [9.19°, 11.29°] | - | 1307 |
| Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part ▲ | 11.15° [9.78°, 12.64°] | 15.18° [14.24°, 16.01°] | - | 1307 |
| Crosshair within 5 deg of the corner, 500 ms before the first part | 24.3% [21.4%, 28.2%] | 29.2% [26.4%, 32.7%] | - | 1307 |
| Crosshair closer to the corner than to the enemy, 500 ms before the first part ▲ | 55.4% [52.4%, 58.9%] | 80.0% [78.8%, 81.1%] | - | 1307 |
| Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part ▲ | 35.4% [31.4%, 38.2%] | 48.2% [45.7%, 50.3%] | - | 1307 |
| Crosshair within 2 deg of the corner's edge, 500 ms before the first part ▲ | 14.2% [12.3%, 16.2%] | 23.7% [21.7%, 25.7%] | - | 1307 |
| Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part ▲ | 50.4% [46.0%, 55.6%] | 28.1% [26.8%, 29.4%] | - | 1307 |
| Angle to the corner at the first part ▲ | 8.90° [7.67°, 9.77°] | 5.74° [5.33°, 6.17°] | - | 1307 |
| Distance from the eye to the corner p50 | 182 u [150 u, 234 u] | 228 u [204 u, 248 u] | - | 1307 |
| Yaw error to the corner at -2000 ms (sightings hidden >= 2 s) | 12.72° [10.85°, 15.05°] | 14.66° [11.85°, 17.22°] | - | 755 |
| Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) ▲ | 8.07° [7.25°, 9.18°] | 5.78° [4.88°, 6.98°] | - | 755 |
| Yaw error to the corner at -500 ms (sightings hidden >= 2 s) ▲ | 7.88° [6.76°, 9.43°] | 4.66° [3.99°, 5.31°] | - | 755 |
| Yaw error to the corner at +0 ms (sightings hidden >= 2 s) ▲ | 7.05° [5.91°, 8.08°] | 3.54° [3.01°, 4.10°] | - | 755 |

Consistency: 112 pooled statistics (bots and owner together) were recomputed from the analysis scripts' own JSON; 0 differ.

## Notes

- perception statistics on the bots' logged body-part column (ext_vis_parts: their own perception); the people's are on the data repo's rebuilt column, accepted against the logged one on 2026-09-28
- 100% of the bots' duel time is against another bot (the logged opponent is the nearest living enemy); the human reference is humans against humans only
- engagements.py counts contact episodes only while exactly two players are alive; with more than two players in a session the fight statistics cover the phases where one player is dead
