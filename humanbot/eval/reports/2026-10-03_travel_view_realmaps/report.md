# Bot evaluation: 2026-10-03_travel_view_realmaps (seeds 201-208)

Generated 2026-10-03 by `humanbot/eval/compare.py`. Captures: `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`. 8 sessions on dm/crnodoors, dm/downladder, dm/main, dm/vents; capture dates 2026-10-03.
Human reference: `human_reference.json` (2026-10-02), 397 duel minutes, 72 player-sessions, 5 people.

## Bots in this capture

| Bot | Family | Seed | Sessions | Duel minutes |
|---|---|---:|---:|---:|
| `bot:stopper:4` | stopper | 711847621 | 1 | 26.1 |
| `bot:stopper:5` | stopper | 1529931226 | 1 | 26.1 |
| `bot:strafer:4` | strafer | 231333596 | 1 | 23.6 |
| `bot:presser:5` | presser | 1152489726 | 1 | 23.6 |
| `bot:stopper:4.2` | stopper | 2458011590 | 1 | 25.7 |
| `bot:strafer:5` | strafer | 4003987225 | 1 | 25.7 |
| `bot:strafer:4.2` | strafer | 3498264623 | 1 | 24.9 |
| `bot:stopper:5.2` | stopper | 2513918026 | 1 | 24.9 |
| `bot:presser:4` | presser | 52844662 | 1 | 25.0 |
| `bot:strafer:5.2` | strafer | 3859071546 | 1 | 25.0 |
| `bot:presser:4.2` | presser | 588158740 | 1 | 25.2 |
| `bot:presser:5.2` | presser | 4131749178 | 1 | 25.2 |
| `bot:stopper:4.3` | stopper | 97502759 | 1 | 27.1 |
| `bot:strafer:5.3` | strafer | 2592015535 | 1 | 27.1 |
| `bot:strafer:4.3` | strafer | 1086809828 | 1 | 26.2 |
| `bot:strafer:5.4` | strafer | 821368291 | 1 | 26.2 |

The owner (`pstN@vsbot`) has 0.0 duel minutes against the bots; those rows are never pooled with the bots.

Bot duel time against the owner 0%, against other bots 100%.

Family mix of bot duel time (observed → reweighted to the people's mix of duel time): presser 24% → 50%, stopper 32% → 26%, strafer 44% → 25%.

## Tells (140)

Statistics whose 95% CIs for the pooled bots and the pooled humans do not overlap, ranked by the standardized gap z = (bots − humans) / √(SE²bots + SE²humans). Only statistics with enough data on both sides are ranked.

| # | Statistic | Bots | Humans | z |
|---:|---|---|---|---:|
| 1 | Centroid sightings with a body part on screen first | 33.5% [31.9%, 35.3%] | 85.9% [84.2%, 87.7%] | -39.3 |
| 2 | Respawn delay after death p50 | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | +16.5 |
| 3 | Side-hold duration (LOS firefight) p25 | 100 ms [100 ms, 100 ms] | 200 ms [200 ms, 200 ms] | -16.4 |
| 4 | Side-hold duration (all contexts) p50 | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | -16.4 |
| 5 | Crosshair closer to the corner than to the enemy, 500 ms before the first part | 52.8% [49.3%, 56.2%] | 80.0% [78.8%, 81.1%] | -14.3 |
| 6 | Crosshair closer to where the enemy will appear than to the enemy, -500 ms | 47.9% [46.0%, 50.5%] | 67.0% [65.6%, 68.8%] | -13.4 |
| 7 | Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part | 45.0% [42.8%, 46.6%] | 28.1% [26.8%, 29.4%] | +13.2 |
| 8 | Strafe switch hazard per tick at hold age 150 ms (LOS firefight) | 23.7% [23.0%, 24.5%] | 12.4% [11.1%, 13.9%] | +11.0 |
| 9 | Side-hold duration (LOS firefight) p75 | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | -11.0 |
| 10 | Fire held at +400 ms from sight gain | 66.3% [63.2%, 69.6%] | 87.3% [85.5%, 89.0%] | -11.0 |
| 11 | Fire held at +200 ms from sight gain | 49.9% [46.4%, 54.3%] | 77.0% [73.9%, 79.5%] | -10.9 |
| 12 | Median aim error at +100 ms from the first visible part | 9.76° [8.87°, 10.52°] | 4.87° [4.54°, 5.18°] | +10.6 |
| 13 | Duration of decisive engagements p50 | 2000 ms [1900 ms, 2050 ms] | 1250 ms [1150 ms, 1351 ms] | +10.5 |
| 14 | Median aim error at +0 ms from the first visible part | 11.51° [10.39°, 12.62°] | 5.22° [4.91°, 5.53°] | +10.4 |
| 15 | Yaw speed (LOS firefight) p99 | 680°/s [602°/s, 745°/s] | 305°/s [292°/s, 318°/s] | +10.2 |
| 16 | Crosshair already on the body at the first visible tick | 12.0% [11.1%, 13.0%] | 22.6% [20.9%, 24.4%] | -10.1 |
| 17 | Mouse still between ticks (LOS firefight) | 3.1% [2.9%, 3.3%] | 1.6% [1.4%, 1.8%] | +10.0 |
| 18 | Strafe switch hazard per tick at hold age 100 ms (LOS firefight) | 21.5% [18.2%, 24.1%] | 5.8% [5.2%, 6.8%] | +9.6 |
| 19 | Yaw speed (hidden, firing) p99 | 511°/s [423°/s, 555°/s] | 194°/s [182°/s, 204°/s] | +9.3 |
| 20 | Median aim error at +0 ms from sight gain | 13.74° [11.78°, 15.35°] | 5.22° [4.88°, 5.61°] | +9.3 |
| 21 | Fire held at +100 ms from sight gain | 37.8% [34.8%, 41.9%] | 61.4% [57.8%, 64.6%] | -9.0 |
| 22 | Crosshair within 2 deg of the corner's edge, 500 ms before the first part | 12.7% [11.2%, 14.3%] | 23.7% [21.7%, 25.7%] | -8.5 |
| 23 | Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) | 22.8% [21.4%, 23.8%] | 15.6% [14.7%, 16.9%] | +8.3 |
| 24 | Press hazard while hidden, LOS lost 300-500 ms ago | 11.4% [11.0%, 11.8%] | 7.0% [6.0%, 8.1%] | +8.1 |
| 25 | Engagements that end in a kill | 89.6% [88.6%, 90.5%] | 79.9% [77.9%, 81.9%] | +8.0 |
| 26 | SMG shots that hit, a part on screen, centroid hidden | 11.8% [11.4%, 12.2%] | 21.7% [18.9%, 24.3%] | -8.0 |
| 27 | View within 30 deg of the hidden opponent, last seen 4-8 s | 55.6% [51.5%, 59.5%] | 76.4% [73.1%, 79.3%] | -8.0 |
| 28 | Fire held at +0 ms from sight gain | 26.3% [24.2%, 28.8%] | 42.5% [39.1%, 46.0%] | -7.7 |
| 29 | Side-hold duration (LOS firefight) p90 | 500 ms [450 ms, 500 ms] | 700 ms [650 ms, 700 ms] | -7.7 |
| 30 | Median aim error firing with LOS at (0, 128] u | 19.18° [18.24°, 20.00°] | 14.28° [13.53°, 15.19°] | +7.7 |
| 31 | Angle to the corner, 500 ms before the first part | 13.25° [12.22°, 14.24°] | 8.41° [7.70°, 9.19°] | +7.6 |
| 32 | View within 30 deg of the hidden opponent, last seen <=250 ms | 85.9% [84.6%, 87.3%] | 92.7% [91.9%, 93.7%] | -7.5 |
| 33 | Angle to the corner at the first part | 9.78° [8.87°, 10.76°] | 5.74° [5.33°, 6.17°] | +7.4 |
| 34 | Side-hold duration (all contexts) p90 | 600 ms [600 ms, 650 ms] | 800 ms [800 ms, 850 ms] | -7.3 |
| 35 | Yaw speed (hidden, firing) p50 | 33°/s [31°/s, 35°/s] | 23°/s [21°/s, 25°/s] | +7.2 |
| 36 | Yaw speed (reloading) p50 | 30°/s [28°/s, 33°/s] | 18°/s [16°/s, 20°/s] | +7.1 |
| 37 | Yaw speed (LOS firefight) p50 | 54°/s [51°/s, 58°/s] | 39°/s [36°/s, 41°/s] | +7.1 |
| 38 | Fire already held on the tick before sight (prefire) | 22.5% [21.0%, 24.4%] | 35.4% [32.2%, 38.6%] | -7.0 |
| 39 | Crosshair within half-width+1.5 deg after a clean sighting p50 | 300 ms [300 ms, 300 ms] | 200 ms [200 ms, 250 ms] | +7.0 |
| 40 | Yaw error to the corner at -500 ms (sightings hidden >= 2 s) | 11.05° [9.38°, 12.77°] | 4.66° [3.99°, 5.31°] | +7.0 |
| 41 | Median aim error at +100 ms from sight gain | 11.69° [9.92°, 13.21°] | 5.97° [5.54°, 6.43°] | +6.9 |
| 42 | Mouse still between ticks (reloading) | 19.2% [17.6%, 20.5%] | 26.3% [25.0%, 27.6%] | -6.9 |
| 43 | Median aim error firing with LOS at (128, 192] u | 10.93° [10.55°, 11.29°] | 8.99° [8.66°, 9.45°] | +6.9 |
| 44 | Reaction: first trigger press after a clean sighting p90 | 700 ms [650 ms, 750 ms] | 400 ms [400 ms, 450 ms] | +6.9 |
| 45 | Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part | 0.29° [-0.21°, 0.80°] | -1.77° [-2.04°, -1.43°] | +6.8 |
| 46 | Median aim error at -200 ms from sight gain | 14.46° [12.74°, 16.05°] | 8.33° [7.70°, 8.92°] | +6.7 |
| 47 | View turn toward the enemy over the last 500 ms before the first part | 6.88° [6.18°, 7.75°] | 13.01° [11.44°, 14.55°] | -6.7 |
| 48 | Reloads started with the opponent dead | 69.8% [65.5%, 74.8%] | 88.2% [85.5%, 90.4%] | -6.6 |
| 49 | Yaw error to the corner, 500 ms before the first part | 11.04° [10.03°, 12.43°] | 5.81° [5.17°, 6.44°] | +6.5 |
| 50 | Time from both alive to the first body part on screen p75 | 2600 ms [2400 ms, 2750 ms] | 1900 ms [1800 ms, 2000 ms] | +6.4 |
| 51 | Yaw speed (hidden, not firing) p99 | 677°/s [639°/s, 699°/s] | 544°/s [521°/s, 580°/s] | +6.3 |
| 52 | View turn over the last 500 ms before the first part | 9.78° [9.06°, 10.50°] | 14.53° [13.34°, 15.90°] | -6.3 |
| 53 | Crosshair on body firing with LOS at (192, 256] u | 32.7% [31.7%, 33.7%] | 42.0% [39.5%, 44.4%] | -6.2 |
| 54 | Strafe switch hazard per tick at hold age 50 ms (LOS firefight) | 9.4% [6.8%, 11.9%] | 1.2% [0.9%, 1.6%] | +6.2 |
| 55 | Retreating >40 u/s in LOS firefight at distance (160, 224] | 13.8% [12.0%, 15.1%] | 6.9% [5.5%, 8.7%] | +6.1 |
| 56 | Yaw error to the corner at +0 ms (sightings hidden >= 2 s) | 8.21° [7.39°, 9.92°] | 3.54° [3.01°, 4.10°] | +6.1 |
| 57 | Median aim error at -100 ms from the first visible part | 11.98° [10.64°, 12.84°] | 8.00° [7.34°, 8.49°] | +6.1 |
| 58 | SMG shots that hit, centroid visible | 21.3% [20.1%, 22.6%] | 27.2% [25.8%, 28.5%] | -6.0 |
| 59 | Median aim error at +300 ms from sight gain | 8.16° [6.91°, 9.10°] | 4.53° [4.12°, 4.88°] | +5.9 |
| 60 | Median aim error at +200 ms from sight gain | 9.18° [7.75°, 10.36°] | 4.92° [4.54°, 5.23°] | +5.9 |
| 61 | Yaw error to the hidden opponent, last seen 4-8 s | 23.50° [19.27°, 28.07°] | 10.29° [9.19°, 11.80°] | +5.8 |
| 62 | Press hazard while hidden, LOS lost 150-250 ms ago | 8.9% [7.9%, 10.2%] | 4.6% [3.9%, 5.5%] | +5.8 |
| 63 | Crosshair on body firing with LOS at (256, 384] u | 30.4% [29.0%, 31.8%] | 39.0% [36.0%, 41.4%] | -5.7 |
| 64 | Mouse still between ticks (hidden, firing) | 7.7% [7.0%, 8.4%] | 10.9% [10.1%, 11.8%] | -5.7 |
| 65 | Strafe switch hazard per tick at hold age 200 ms (LOS firefight) | 22.6% [21.8%, 23.2%] | 17.0% [15.4%, 18.7%] | +5.6 |
| 66 | Press hazard while hidden, LOS lost over 1 s ago | 2.3% [2.0%, 2.7%] | 4.1% [3.5%, 4.6%] | -5.6 |
| 67 | Yaw speed (reloading) p99 | 767°/s [726°/s, 818°/s] | 945°/s [909°/s, 991°/s] | -5.5 |
| 68 | Median aim error firing with LOS at (192, 256] u | 8.07° [7.78°, 8.36°] | 6.68° [6.36°, 7.10°] | +5.5 |
| 69 | Strafe switch hazard per tick at hold age 250 ms (LOS firefight) | 22.7% [21.6%, 23.7%] | 17.4% [16.0%, 19.0%] | +5.5 |
| 70 | Lean held, all duel time | 74.7% [71.7%, 77.2%] | 59.7% [55.3%, 64.0%] | +5.4 |
| 71 | Retreating >40 u/s in LOS firefight at distance (224, 288] | 9.9% [8.9%, 10.7%] | 5.3% [4.2%, 6.8%] | +5.2 |
| 72 | Crosshair within 5 deg of the corner, 500 ms before the first part | 19.3% [17.3%, 21.4%] | 29.2% [26.4%, 32.7%] | -5.2 |
| 73 | Standing still (<5 u/s), all duel time | 9.0% [8.2%, 10.0%] | 15.7% [13.4%, 17.7%] | -5.1 |
| 74 | Shots per burst (consecutive 100 ms shots) p90 | 16 [15, 17] | 12 [11, 13] | +5.0 |
| 75 | Median aim error at +200 ms from the first visible part | 6.73° [6.25°, 7.08°] | 5.26° [4.86°, 5.58°] | +5.0 |
| 76 | Crosshair on body firing with LOS at (128, 192] u | 35.4% [33.8%, 36.6%] | 42.5% [39.8%, 44.7%] | -4.9 |
| 77 | Shots per burst whose first round left with a body part visible p90 | 18 [16, 19] | 13 [12, 14] | +4.9 |
| 78 | Life length (lives ending in death) p50 | 10.8 s [10.3 s, 11.9 s] | 7.1 s [5.8 s, 8.2 s] | +4.9 |
| 79 | Fire held at +700 ms from sight gain | 66.5% [62.7%, 68.9%] | 77.0% [74.3%, 79.7%] | -4.8 |
| 80 | Standing still (<5 u/s) (hidden, not firing) | 14.8% [13.4%, 16.8%] | 22.5% [19.5%, 25.1%] | -4.7 |
| 81 | Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago | 21.4% [17.2%, 27.2%] | 7.4% [4.7%, 10.8%] | +4.6 |
| 82 | Shot accuracy (eligible SMG shots that hit) | 15.6% [14.6%, 16.6%] | 19.2% [18.1%, 20.3%] | -4.6 |
| 83 | Yaw speed (LOS, not firing) p99 | 801°/s [774°/s, 827°/s] | 583°/s [505°/s, 695°/s] | +4.6 |
| 84 | Retreating >40 u/s in LOS firefight at distance (96, 160] | 17.9% [15.6%, 20.7%] | 10.5% [8.5%, 12.6%] | +4.6 |
| 85 | Share of duel time: LOS firefight | 21.9% [20.6%, 23.3%] | 17.6% [16.5%, 18.9%] | +4.5 |
| 86 | Fire held, LOS, aim error in half-widths (20, 1000] | 5.4% [3.9%, 6.6%] | 0.9% [0.4%, 3.1%] | +4.5 |
| 87 | Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part | 10.85° [9.36°, 12.47°] | 15.18° [14.24°, 16.01°] | -4.4 |
| 88 | Standing still (<5 u/s) (hidden, firing) | 6.9% [6.5%, 7.3%] | 10.2% [9.1%, 11.7%] | -4.4 |
| 89 | Crosshair on body firing with LOS at (0, 128] u | 39.2% [37.3%, 41.4%] | 48.0% [44.9%, 51.5%] | -4.4 |
| 90 | Fire held, LOS, aim error in half-widths (6, 10] | 38.3% [33.6%, 44.4%] | 22.4% [17.1%, 27.1%] | +4.3 |
| 91 | Share of duel time in the busiest 10% of 32u cells (dm/main) | 35.2% [31.9%, 35.5%] | 45.2% [40.5%, 48.6%] | -4.3 |
| 92 | Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) | 12.31° [9.40°, 15.47°] | 5.78° [4.88°, 6.98°] | +4.2 |
| 93 | Yaw error to the hidden opponent, last seen <=250 ms | 6.28° [5.80°, 6.69°] | 4.92° [4.56°, 5.26°] | +4.2 |
| 94 | Release hazard while hidden, LOS lost 500-950 ms ago | 14.1% [12.5%, 15.8%] | 10.1% [9.2%, 11.3%] | +4.2 |
| 95 | Strafe end is a direct reverse (reloading) | 58.5% [51.2%, 63.9%] | 37.7% [32.5%, 42.7%] | +4.2 |
| 96 | Fire held, LOS, aim error in half-widths (10, 20] | 12.5% [9.4%, 15.6%] | 4.6% [2.4%, 6.9%] | +4.1 |
| 97 | Key chord forward (reloading) | 14.8% [11.1%, 20.5%] | 26.1% [23.8%, 28.6%] | -4.1 |
| 98 | Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) | 20.2% [18.7%, 21.9%] | 16.1% [14.9%, 17.5%] | +4.0 |
| 99 | Median aim error firing with LOS at (256, 384] u | 6.02° [5.73°, 6.20°] | 5.23° [4.96°, 5.59°] | +3.9 |
| 100 | Side-hold duration (LOS firefight) p50 | 200 ms [150 ms, 200 ms] | 300 ms [250 ms, 300 ms] | -3.9 |
| 101 | Key chord pure strafe (reloading) | 31.3% [27.0%, 34.4%] | 22.3% [19.6%, 24.4%] | +3.8 |
| 102 | Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part | 42.3% [40.6%, 44.1%] | 48.2% [45.7%, 50.3%] | -3.8 |
| 103 | Part sightings whose visible bout ends without a hit | 35.9% [32.0%, 39.6%] | 27.5% [25.7%, 29.7%] | +3.7 |
| 104 | Key chord neutral (reloading) | 13.9% [11.6%, 17.2%] | 8.3% [7.3%, 9.5%] | +3.7 |
| 105 | Fire held at -500 ms from sight gain | 23.4% [21.4%, 25.8%] | 18.2% [16.5%, 20.2%] | +3.6 |
| 106 | Attack holds that are taps (<=100 ms) | 39.1% [37.5%, 40.4%] | 44.3% [41.8%, 46.5%] | -3.6 |
| 107 | Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) | 39.0% [32.2%, 40.9%] | 49.4% [44.0%, 50.8%] | -3.5 |
| 108 | First hit after the first part p50 | 350 ms [350 ms, 400 ms] | 250 ms [200 ms, 250 ms] | +3.5 |
| 109 | Reloads with the opponent alive that are forced (empty clip) | 92.1% [88.5%, 95.0%] | 77.7% [69.5%, 83.9%] | +3.4 |
| 110 | Median aim error at +500 ms from sight gain | 7.13° [6.36°, 8.10°] | 5.47° [5.08°, 5.88°] | +3.4 |
| 111 | Speed 400-200 ms before the first part | 167 u/s [155 u/s, 175 u/s] | 188 u/s [182 u/s, 193 u/s] | -3.4 |
| 112 | Fire held with no body part visible, 0-500ms after one was last on screen | 48.4% [46.8%, 50.0%] | 43.2% [40.9%, 45.8%] | +3.4 |
| 113 | Share of duel time: LOS, not firing | 9.9% [9.1%, 10.6%] | 7.0% [5.5%, 8.4%] | +3.3 |
| 114 | Share of duel time in the busiest 10% of 32u cells (dm/downladder) | 40.3% [32.0%, 40.9%] | 61.4% [42.9%, 63.8%] | -3.3 |
| 115 | Yaw speed (hidden, not firing) p50 | 12°/s [11°/s, 12°/s] | 8°/s [6°/s, 10°/s] | +3.3 |
| 116 | View within 30 deg of the hidden opponent, last seen 2-4 s | 56.4% [53.0%, 60.4%] | 64.3% [61.3%, 67.3%] | -3.2 |
| 117 | Fire already held on the tick before the first part (prefire) | 18.1% [16.6%, 19.8%] | 23.0% [20.3%, 25.5%] | -3.1 |
| 118 | Attack holds begun loaded that are taps (<=100 ms), with a body part visible | 11.2% [9.7%, 12.7%] | 15.4% [13.1%, 17.6%] | -3.1 |
| 119 | Yaw speed (LOS, not firing) p50 | 33°/s [30°/s, 36°/s] | 11°/s [0°/s, 26°/s] | +3.1 |
| 120 | Release hazard while hidden, LOS lost 1 s or more ago | 14.4% [13.8%, 14.8%] | 11.3% [9.8%, 13.5%] | +3.0 |
| 121 | Reaction: first trigger press after a clean sighting p75 | 400 ms [350 ms, 450 ms] | 300 ms [250 ms, 300 ms] | +3.0 |
| 122 | Share of duel time: hidden, firing | 13.1% [11.9%, 14.5%] | 15.8% [14.9%, 17.0%] | -3.0 |
| 123 | Share of duel time: hidden, not firing | 43.8% [40.7%, 46.4%] | 49.1% [47.4%, 51.2%] | -3.0 |
| 124 | Share of duel time with a body part of the enemy on screen | 39.8% [36.9%, 42.6%] | 34.0% [31.6%, 36.5%] | +2.9 |
| 125 | Crosshair on body firing with LOS at (384, 512] u | 29.6% [27.4%, 32.3%] | 35.4% [32.6%, 38.3%] | -2.9 |
| 126 | Standing still (<5 u/s) (LOS firefight) | 1.4% [1.2%, 1.7%] | 2.3% [1.8%, 2.7%] | -2.9 |
| 127 | View within 30 deg of the hidden opponent, last seen 250-500 ms | 89.8% [88.6%, 91.0%] | 92.2% [91.3%, 93.4%] | -2.9 |
| 128 | Release hazard, LOS, aim error in half-widths (10, 1000] | 19.0% [17.3%, 21.6%] | 32.7% [22.9%, 41.8%] | -2.8 |
| 129 | Walk presses per minute | 6.40/min [5.90/min, 6.85/min] | 7.54/min [6.90/min, 8.11/min] | -2.7 |
| 130 | Lean held in LOS firefights | 88.5% [86.7%, 89.8%] | 81.3% [76.6%, 85.8%] | +2.6 |
| 131 | Standing still (<5 u/s) (LOS, not firing) | 6.6% [5.3%, 8.2%] | 29.6% [12.6%, 44.0%] | -2.6 |
| 132 | Key chord fwd diag (reloading) | 30.3% [26.5%, 33.2%] | 36.8% [34.1%, 39.9%] | -2.6 |
| 133 | Yaw error to the hidden enemy 500 ms before the first part | 12.09° [10.46°, 13.64°] | 14.63° [13.70°, 15.63°] | -2.6 |
| 134 | Crosshair on body firing with LOS at (512, 768] u | 27.4% [25.1%, 28.9%] | 32.0% [29.1%, 35.1%] | -2.5 |
| 135 | Key chord forward (hidden, firing) | 9.6% [7.8%, 12.4%] | 6.6% [5.7%, 7.4%] | +2.4 |
| 136 | Fire held, LOS, aim error in half-widths (1.5, 2] | 85.6% [83.8%, 87.9%] | 81.9% [79.8%, 83.7%] | +2.4 |
| 137 | Key chord forward (hidden, not firing) | 14.2% [12.8%, 15.8%] | 11.9% [10.9%, 12.8%] | +2.4 |
| 138 | Mouse still between ticks (LOS, not firing) | 19.3% [18.3%, 20.5%] | 36.7% [20.8%, 49.8%] | -2.2 |
| 139 | Key chord forward (LOS firefight) | 1.8% [1.4%, 2.5%] | 1.2% [1.0%, 1.4%] | +2.2 |
| 140 | Lead of the first part over the centroid p50 | 50 ms [50 ms, 50 ms] | 100 ms [100 ms, 100 ms] | - |

## Per bot

### `bot:stopper:4` (stopper, seed 711847621, 26.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.07034 | 0.1651 | +18% |
| reverse_share | 0.1382 | 0.1286 | -1% |
| side_hold_ms | 249.3 | 250 | +0% |
| lean_fight | 0.9549 | 0.9635 | +2% |
| jumps_per_min | 0.3303 | 0.153 | -3% |
| crouch_per_min | 5.571 | 5.968 | +4% |
| walk_hidden | 0 | 0.05148 | +49% ⚠ |
| burst_median | 4.564 | 5 | +11% |
| aim_height_firing | 0.4682 | 0.466 | -1% |
| hold_angle | 0.4674 | 0.2915 | -57% ⚠ |
| aim_error_fight_deg | 6.249 | 6.27 | +1% |
| reaction_ms | 153.7 | 250 | +96% ⚠ |
| mp40_share | 0.02524 | 0 | -3% |

Style fingerprint (2026-10-03, 26.1 min, styles.py features 30/30): z-distance to the human cloud centre 6.1 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 6.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lf_neutral 0.131 (humans 0.00436–0.112); lean_any 0.885 (humans 0.358–0.767); lf_lean 0.964 (humans 0.457–0.948); p_attack_given_los 0.517 (humans 0.547–0.861); lf_miss_units_med 36.3 (humans 22.2–36.1); lf_on_target 0.269 (humans 0.274–0.515); side_hold_p90_ms 650 (humans 700–950); reverse_share 0.129 (humans 0.138–0.839).

### `bot:stopper:5` (stopper, seed 1529931226, 26.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1013 | 0.1816 | +16% |
| reverse_share | 0.3253 | 0.3301 | +1% |
| side_hold_ms | 247.2 | 250 | +2% |
| lean_fight | 0.8427 | 0.8809 | +8% |
| jumps_per_min | 5.122 | 4.935 | -3% |
| crouch_per_min | 11.7 | 12.4 | +6% |
| walk_hidden | 0.07078 | 0.06837 | -2% |
| burst_median | 4.132 | 5 | +22% |
| aim_height_firing | 0.4597 | 0.436 | -13% |
| hold_angle | 0.5274 | 0.3333 | -63% ⚠ |
| aim_error_fight_deg | 4.835 | 5.087 | +10% |
| reaction_ms | 109.9 | 150 | +40% ⚠ |
| mp40_share | 0.06948 | 0 | -7% |

Style fingerprint (2026-10-03, 26.1 min, styles.py features 30/30): z-distance to the human cloud centre 6.4 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 6.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.129 (humans 0.0109–0.0741); side_hold_p90_ms 650 (humans 700–950).

### `bot:strafer:4` (strafer, seed 231333596, 23.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.1601 | +18% |
| reverse_share | 0.4467 | 0.4795 | +5% |
| side_hold_ms | 326.3 | 300 | -18% |
| lean_fight | 0.4941 | 0.5778 | +17% |
| jumps_per_min | 2.379 | 2.159 | -4% |
| crouch_per_min | 9.476 | 10.07 | +6% |
| walk_hidden | 0 | 0.06163 | +58% ⚠ |
| burst_median | 8 | 8 | +0% |
| aim_height_firing | 0.4857 | 0.48 | -3% |
| hold_angle | 0.2514 | 0.2917 | +13% |
| aim_error_fight_deg | 4.667 | 5.116 | +18% |
| reaction_ms | 140.8 | 200 | +59% ⚠ |
| mp40_share | 0.9062 | 1 | +9% |

Style fingerprint (2026-10-03, 23.6 min, styles.py features 30/30): z-distance to the human cloud centre 6.3 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 4.4 vs the within-person threshold 5.6 (as close as two captures of one human).

Features outside the human min–max: pure_strafe 0.519 (humans 0.191–0.489); ducked 0.0895 (humans 0.0109–0.0741); still 0.0657 (humans 0.0832–0.227); yaw_speed_p95 290 (humans 158–271); side_hold_p90_ms 650 (humans 700–950).

### `bot:presser:5` (presser, seed 1152489726, 23.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4832 | 0.6178 | +26% ⚠ |
| reverse_share | 0.8051 | 0.8116 | +1% |
| side_hold_ms | 295.2 | 200 | -63% ⚠ |
| lean_fight | 0.9204 | 0.9619 | +8% |
| jumps_per_min | 0.4317 | 0.2116 | -4% |
| crouch_per_min | 3.157 | 3.724 | +5% |
| walk_hidden | 0.02816 | 0.07021 | +40% ⚠ |
| burst_median | 6.498 | 7 | +13% |
| aim_height_firing | 0.417 | 0.406 | -6% |
| hold_angle | 0.2275 | 0.2718 | +14% |
| aim_error_fight_deg | 4.893 | 5.707 | +33% ⚠ |
| reaction_ms | 152.5 | 250 | +98% ⚠ |
| mp40_share | 0.1123 | 1 | +89% ⚠ |

Style fingerprint (2026-10-03, 23.6 min, styles.py features 30/30): z-distance to the human cloud centre 6.9 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 6.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lf_pure_strafe 0.324 (humans 0.362–0.752); lf_fwd_diag 0.618 (humans 0.0658–0.568); lean_any 0.845 (humans 0.358–0.767); lf_lean 0.962 (humans 0.457–0.948); still 0.069 (humans 0.0832–0.227); yaw_speed_p95 302 (humans 158–271); side_hold_p90_ms 550 (humans 700–950); lf_approach 0.67 (humans 0.131–0.64).

### `bot:stopper:4.2` (stopper, seed 2458011590, 25.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1182 | 0.155 | +7% |
| reverse_share | 0.4266 | 0.4185 | -1% |
| side_hold_ms | 200 | 300 | +67% ⚠ |
| lean_fight | 0.8817 | 0.9162 | +7% |
| jumps_per_min | 2.965 | 3.349 | +7% |
| crouch_per_min | 8.679 | 7.866 | -8% |
| walk_hidden | 0.1056 | 0.1178 | +12% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4719 | 0.467 | -3% |
| hold_angle | 0.3772 | 0.1394 | -77% ⚠ |
| aim_error_fight_deg | 4.42 | 8.532 | +167% ⚠ |
| reaction_ms | 139.9 | 200 | +60% ⚠ |
| mp40_share | 0.02794 | 0 | -3% |

Style fingerprint (2026-10-03, 25.7 min, styles.py features 30/30): z-distance to the human cloud centre 7.2 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 7.7 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.0803 (humans 0.0109–0.0741); lf_err_med 8.53 (humans 4.08–6.63); yaw_speed_p95 298 (humans 158–271); lf_retreat 0.162 (humans 0.0243–0.141).

### `bot:strafer:5` (strafer, seed 4003987225, 25.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.1393 | +14% |
| reverse_share | 0.4657 | 0.4575 | -1% |
| side_hold_ms | 270.9 | 300 | +19% |
| lean_fight | 0.5402 | 0.5476 | +1% |
| jumps_per_min | 4.852 | 5.374 | +10% |
| crouch_per_min | 3.183 | 2.453 | -7% |
| walk_hidden | 0.03847 | 0.0533 | +14% |
| burst_median | 5.686 | 6 | +8% |
| aim_height_firing | 0.4588 | 0.454 | -3% |
| hold_angle | 0.311 | 0.2346 | -25% |
| aim_error_fight_deg | 5.446 | 8.795 | +136% ⚠ |
| reaction_ms | 161.3 | 300 | +139% ⚠ |
| mp40_share | 0 | 0 | +0% |

Style fingerprint (2026-10-03, 25.7 min, styles.py features 30/30): z-distance to the human cloud centre 7.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 6.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lf_err_med 8.79 (humans 4.08–6.63); yaw_speed_p95 316 (humans 158–271); lf_retreat 0.168 (humans 0.0243–0.141).

### `bot:strafer:4.2` (strafer, seed 3498264623, 24.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1522 | 0.1739 | +4% |
| reverse_share | 0.4724 | 0.4611 | -2% |
| side_hold_ms | 293.3 | 250 | -29% ⚠ |
| lean_fight | 0.6201 | 0.6247 | +1% |
| jumps_per_min | 3.829 | 4.097 | +5% |
| crouch_per_min | 9.64 | 10.08 | +4% |
| walk_hidden | 0 | 0.04799 | +45% ⚠ |
| burst_median | 4.228 | 5 | +19% |
| aim_height_firing | 0.4919 | 0.4965 | +2% |
| hold_angle | 0.3757 | 0.2075 | -55% ⚠ |
| aim_error_fight_deg | 5.803 | 8.685 | +117% ⚠ |
| reaction_ms | 133.2 | 250 | +117% ⚠ |
| mp40_share | 0.5841 | 1 | +42% ⚠ |

Style fingerprint (2026-10-03, 24.9 min, styles.py features 30/30): z-distance to the human cloud centre 7.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.0916 (humans 0.0109–0.0741); lf_err_med 8.69 (humans 4.08–6.63); yaw_speed_med 30.7 (humans 12.6–30); yaw_speed_p95 328 (humans 158–271); lf_retreat 0.144 (humans 0.0243–0.141).

### `bot:stopper:5.2` (stopper, seed 2513918026, 24.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1336 | 0.1812 | +9% |
| reverse_share | 0.6777 | 0.6792 | +0% |
| side_hold_ms | 226 | 250 | +16% |
| lean_fight | 0.9588 | 0.9759 | +3% |
| jumps_per_min | 2.944 | 3.133 | +4% |
| crouch_per_min | 3.426 | 2.49 | -9% |
| walk_hidden | 0.08429 | 0.09349 | +9% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4602 | 0.456 | -2% |
| hold_angle | 0.3824 | 0.1912 | -62% ⚠ |
| aim_error_fight_deg | 4.291 | 8.291 | +163% ⚠ |
| reaction_ms | 113 | 200 | +87% ⚠ |
| mp40_share | 0.07877 | 0 | -8% |

Style fingerprint (2026-10-03, 24.9 min, styles.py features 30/30): z-distance to the human cloud centre 6.7 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 7.6 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.5 (humans 0.191–0.489); lean_any 0.92 (humans 0.358–0.767); lf_lean 0.976 (humans 0.457–0.948); lf_err_med 8.29 (humans 4.08–6.63); yaw_speed_p95 296 (humans 158–271); lf_retreat 0.147 (humans 0.0243–0.141).

### `bot:presser:4` (presser, seed 52844662, 25.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5317 | 0.41 | -24% |
| reverse_share | 0.8152 | 0.7703 | -6% |
| side_hold_ms | 336.6 | 200 | -91% ⚠ |
| lean_fight | 0.935 | 0.9812 | +9% |
| jumps_per_min | 0.336 | 0.2797 | -1% |
| crouch_per_min | 1.298 | 0.9988 | -3% |
| walk_hidden | 0 | 0.05353 | +51% ⚠ |
| burst_median | 7.121 | 8 | +22% |
| aim_height_firing | 0.3853 | 0.336 | -26% ⚠ |
| hold_angle | 0.2402 | 0.2625 | +7% |
| aim_error_fight_deg | 4.424 | 5.791 | +56% ⚠ |
| reaction_ms | 173.4 | 300 | +127% ⚠ |
| mp40_share | 0.06486 | 0 | -6% |

Style fingerprint (2026-10-03, 25.0 min, styles.py features 30/30): z-distance to the human cloud centre 6.7 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 6.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.888 (humans 0.358–0.767); lf_lean 0.981 (humans 0.457–0.948); ducked 0.00742 (humans 0.0109–0.0741); still 0.0711 (humans 0.0832–0.227); lf_height_frac 0.336 (humans 0.352–0.54); yaw_speed_p95 285 (humans 158–271); side_hold_p90_ms 550 (humans 700–950).

### `bot:strafer:5.2` (strafer, seed 3859071546, 25.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1614 | 0.1821 | +4% |
| reverse_share | 0.5449 | 0.5199 | -4% |
| side_hold_ms | 350 | 250 | -67% ⚠ |
| lean_fight | 0.5666 | 0.6957 | +26% ⚠ |
| jumps_per_min | 0.6862 | 0.3995 | -5% |
| crouch_per_min | 3.8 | 4.115 | +3% |
| walk_hidden | 0.04377 | 0.05979 | +15% |
| burst_median | 7.775 | 8 | +6% |
| aim_height_firing | 0.4661 | 0.441 | -13% |
| hold_angle | 0.2527 | 0.2924 | +13% |
| aim_error_fight_deg | 4.952 | 5.261 | +13% |
| reaction_ms | 115 | 150 | +35% ⚠ |
| mp40_share | 0.03683 | 0 | -4% |

Style fingerprint (2026-10-03, 25.0 min, styles.py features 30/30): z-distance to the human cloud centre 5.3 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 5.3 vs the within-person threshold 5.6 (as close as two captures of one human).

Features outside the human min–max: pure_strafe 0.495 (humans 0.191–0.489); side_hold_p90_ms 600 (humans 700–950).

### `bot:presser:4.2` (presser, seed 588158740, 25.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4812 | 0.3534 | -25% |
| reverse_share | 0.8389 | 0.8066 | -5% |
| side_hold_ms | 302.9 | 200 | -69% ⚠ |
| lean_fight | 0.9588 | 0.9835 | +5% |
| jumps_per_min | 1.077 | 0.7153 | -7% |
| crouch_per_min | 1.47 | 1.153 | -3% |
| walk_hidden | 0.004612 | 0.06064 | +53% ⚠ |
| burst_median | 7.045 | 9 | +49% ⚠ |
| aim_height_firing | 0.352 | 0.302 | -27% ⚠ |
| hold_angle | 0.2402 | 0.3185 | +25% ⚠ |
| aim_error_fight_deg | 4.382 | 6.64 | +92% ⚠ |
| reaction_ms | 183.1 | 300 | +117% ⚠ |
| mp40_share | 0.9584 | 1 | +4% |

Style fingerprint (2026-10-03, 25.2 min, styles.py features 30/30): z-distance to the human cloud centre 7.3 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 7.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.905 (humans 0.358–0.767); lf_lean 0.984 (humans 0.457–0.948); still 0.0804 (humans 0.0832–0.227); lf_err_med 6.64 (humans 4.08–6.63); lf_height_frac 0.302 (humans 0.352–0.54); yaw_speed_p95 296 (humans 158–271); side_hold_p90_ms 550 (humans 700–950).

### `bot:presser:5.2` (presser, seed 4131749178, 25.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5433 | 0.366 | -35% ⚠ |
| reverse_share | 0.823 | 0.7673 | -8% |
| side_hold_ms | 298.6 | 150 | -99% ⚠ |
| lean_fight | 0.9579 | 0.9836 | +5% |
| jumps_per_min | 0.5292 | 0.4769 | -1% |
| crouch_per_min | 2.785 | 2.822 | +0% |
| walk_hidden | 0.00312 | 0.04931 | +44% ⚠ |
| burst_median | 7.004 | 8 | +25% |
| aim_height_firing | 0.4126 | 0.374 | -21% |
| hold_angle | 0.2204 | 0.2372 | +5% |
| aim_error_fight_deg | 4.873 | 6.28 | +57% ⚠ |
| reaction_ms | 134.4 | 150 | +16% |
| mp40_share | 0.08844 | 0 | -9% |

Style fingerprint (2026-10-03, 25.2 min, styles.py features 30/30): z-distance to the human cloud centre 7.5 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.913 (humans 0.358–0.767); lf_lean 0.984 (humans 0.457–0.948); yaw_speed_p95 306 (humans 158–271); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 500 (humans 700–950).

### `bot:stopper:4.3` (stopper, seed 97502759, 27.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.1726 | +21% |
| reverse_share | 0.6408 | 0.6267 | -2% |
| side_hold_ms | 200 | 250 | +33% ⚠ |
| lean_fight | 0.8757 | 0.9203 | +9% |
| jumps_per_min | 1.606 | 1.627 | +0% |
| crouch_per_min | 2.514 | 2.329 | -2% |
| walk_hidden | 0.09232 | 0.08772 | -4% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4804 | 0.49 | +5% |
| hold_angle | 0.418 | 0.1842 | -76% ⚠ |
| aim_error_fight_deg | 5.509 | 10.56 | +206% ⚠ |
| reaction_ms | 127.6 | 200 | +72% ⚠ |
| mp40_share | 0.1179 | 0 | -12% |

Style fingerprint (2026-10-03, 27.1 min, styles.py features 30/30): z-distance to the human cloud centre 8.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.787 (humans 0.358–0.767); attack 0.268 (humans 0.276–0.478); lf_err_med 10.6 (humans 4.08–6.63); yaw_speed_p95 329 (humans 158–271); lf_retreat 0.167 (humans 0.0243–0.141).

### `bot:strafer:5.3` (strafer, seed 2592015535, 27.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.119 | 0.1956 | +15% |
| reverse_share | 0.4951 | 0.4589 | -5% |
| side_hold_ms | 295 | 250 | -30% ⚠ |
| lean_fight | 0.4565 | 0.4688 | +2% |
| jumps_per_min | 5.657 | 4.99 | -12% |
| crouch_per_min | 11.59 | 11.16 | -4% |
| walk_hidden | 0 | 0.03165 | +30% ⚠ |
| burst_median | 4.178 | 4 | -4% |
| aim_height_firing | 0.54 | 0.55 | +5% |
| hold_angle | 0.3001 | 0.2283 | -23% |
| aim_error_fight_deg | 6.024 | 11.6 | +227% ⚠ |
| reaction_ms | 165.9 | 250 | +84% ⚠ |
| mp40_share | 0.4858 | 1 | +51% ⚠ |

Style fingerprint (2026-10-03, 27.1 min, styles.py features 30/30): z-distance to the human cloud centre 10.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.268 (humans 0.358–0.767); ducked 0.129 (humans 0.0109–0.0741); attack 0.234 (humans 0.276–0.478); p_attack_given_los 0.487 (humans 0.547–0.861); lf_err_med 11.6 (humans 4.08–6.63); lf_height_frac 0.55 (humans 0.352–0.54); yaw_speed_p95 323 (humans 158–271); lf_retreat 0.171 (humans 0.0243–0.141).

### `bot:strafer:4.3` (strafer, seed 1086809828, 26.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1632 | 0.2388 | +15% |
| reverse_share | 0.4593 | 0.4414 | -3% |
| side_hold_ms | 314.9 | 300 | -10% |
| lean_fight | 0.5599 | 0.59 | +6% |
| jumps_per_min | 4.813 | 5.071 | +5% |
| crouch_per_min | 9.497 | 8.616 | -8% |
| walk_hidden | 0 | 0.03638 | +34% ⚠ |
| burst_median | 5.59 | 6 | +10% |
| aim_height_firing | 0.4741 | 0.508 | +18% |
| hold_angle | 0.3025 | 0.191 | -36% ⚠ |
| aim_error_fight_deg | 6.527 | 13.85 | +298% ⚠ |
| reaction_ms | 158.4 | 250 | +92% ⚠ |
| mp40_share | 0.9804 | 1 | +2% |

Style fingerprint (2026-10-03, 26.2 min, styles.py features 30/30): z-distance to the human cloud centre 12.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 12.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.107 (humans 0.0109–0.0741); attack 0.259 (humans 0.276–0.478); still 0.068 (humans 0.0832–0.227); lf_err_med 13.8 (humans 4.08–6.63); yaw_speed_p95 396 (humans 158–271); yaw_speed_p99 824 (humans 427–754); lf_retreat 0.208 (humans 0.0243–0.141).

### `bot:strafer:5.4` (strafer, seed 821368291, 26.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.166 | 0.2833 | +23% |
| reverse_share | 0.4801 | 0.4725 | -1% |
| side_hold_ms | 349.1 | 300 | -33% ⚠ |
| lean_fight | 0.6654 | 0.7175 | +10% |
| jumps_per_min | 5.052 | 4.156 | -17% |
| crouch_per_min | 10.37 | 9.226 | -11% |
| walk_hidden | 0.0009382 | 0.02684 | +25% |
| burst_median | 5.755 | 6 | +6% |
| aim_height_firing | 0.4434 | 0.446 | +1% |
| hold_angle | 0.3191 | 0.1591 | -52% ⚠ |
| aim_error_fight_deg | 4.261 | 11.21 | +283% ⚠ |
| reaction_ms | 175 | 300 | +125% ⚠ |
| mp40_share | 0.1202 | 0 | -12% |

Style fingerprint (2026-10-03, 26.2 min, styles.py features 30/30): z-distance to the human cloud centre 9.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.0942 (humans 0.0109–0.0741); attack 0.252 (humans 0.276–0.478); still 0.0626 (humans 0.0832–0.227); lf_err_med 11.2 (humans 4.08–6.63); yaw_speed_p95 340 (humans 158–271); lf_retreat 0.159 (humans 0.0243–0.141).

## The owner against the bots (`pstN@vsbot`)

Kills of bots 0, deaths to bots 0, suicides 0. 

0 statistics of the owner against bots fall outside the human CI (owner column below, marked •).

## All statistics

Pooled bots are reweighted to the people's family mix of duel time; humans and the owner are pooled over their duel time. ▲ marks a tell. n = units (ticks, runs, shots, sightings, ...) over the blocks.

### Movement

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time: hidden, not firing ▲ | 43.8% [40.7%, 46.4%] | 49.1% [47.4%, 51.2%] | - | 489160 |
| Share of duel time: hidden, firing ▲ | 13.1% [11.9%, 14.5%] | 15.8% [14.9%, 17.0%] | - | 489160 |
| Share of duel time: LOS, not firing ▲ | 9.9% [9.1%, 10.6%] | 7.0% [5.5%, 8.4%] | - | 489160 |
| Share of duel time: LOS firefight ▲ | 21.9% [20.6%, 23.3%] | 17.6% [16.5%, 18.9%] | - | 489160 |
| Share of duel time: reloading | 11.2% [9.6%, 13.1%] | 10.5% [9.6%, 11.5%] | - | 489160 |
| Key chord pure strafe (hidden, not firing) | 29.6% [27.3%, 31.5%] | 26.0% [23.5%, 29.4%] | - | 228160 |
| Key chord fwd diag (hidden, not firing) | 28.3% [26.6%, 29.6%] | 29.9% [26.7%, 33.2%] | - | 228160 |
| Key chord forward (hidden, not firing) ▲ | 14.2% [12.8%, 15.8%] | 11.9% [10.9%, 12.8%] | - | 228160 |
| Key chord neutral (hidden, not firing) | 21.5% [19.7%, 23.9%] | 26.1% [23.1%, 28.6%] | - | 228160 |
| Key chord back any (hidden, not firing) | 6.4% [5.7%, 6.9%] | 6.1% [5.3%, 7.1%] | - | 228160 |
| Key chord pure strafe (hidden, firing) | 43.7% [39.7%, 47.4%] | 44.9% [41.6%, 48.0%] | - | 59799 |
| Key chord fwd diag (hidden, firing) | 28.6% [25.6%, 31.4%] | 28.6% [23.9%, 32.8%] | - | 59799 |
| Key chord forward (hidden, firing) ▲ | 9.6% [7.8%, 12.4%] | 6.6% [5.7%, 7.4%] | - | 59799 |
| Key chord neutral (hidden, firing) | 12.5% [11.0%, 14.3%] | 13.2% [11.2%, 15.7%] | - | 59799 |
| Key chord back any (hidden, firing) | 5.6% [4.1%, 7.0%] | 6.7% [5.9%, 7.9%] | - | 59799 |
| Key chord pure strafe (LOS, not firing) | 36.2% [33.4%, 38.9%] | 26.7% [20.8%, 34.2%] | - | 49325 |
| Key chord fwd diag (LOS, not firing) | 33.6% [30.0%, 36.5%] | 29.4% [23.0%, 38.1%] | - | 49325 |
| Key chord forward (LOS, not firing) | 10.1% [8.7%, 12.0%] | 7.5% [5.9%, 9.5%] | - | 49325 |
| Key chord neutral (LOS, not firing) | 13.7% [11.8%, 16.5%] | 31.5% [15.3%, 45.4%] | - | 49325 |
| Key chord back any (LOS, not firing) | 6.3% [5.8%, 6.9%] | 4.8% [3.4%, 6.7%] | - | 49325 |
| Key chord pure strafe (LOS firefight) | 55.5% [49.8%, 59.9%] | 58.0% [53.1%, 62.2%] | - | 101810 |
| Key chord fwd diag (LOS firefight) | 32.3% [26.9%, 39.6%] | 30.5% [25.6%, 36.7%] | - | 101810 |
| Key chord forward (LOS firefight) ▲ | 1.8% [1.4%, 2.5%] | 1.2% [1.0%, 1.4%] | - | 101810 |
| Key chord neutral (LOS firefight) | 5.0% [3.9%, 6.3%] | 5.1% [3.9%, 6.3%] | - | 101810 |
| Key chord back any (LOS firefight) | 5.3% [4.1%, 6.2%] | 5.2% [4.5%, 6.1%] | - | 101810 |
| Key chord pure strafe (reloading) ▲ | 31.3% [27.0%, 34.4%] | 22.3% [19.6%, 24.4%] | - | 50059 |
| Key chord fwd diag (reloading) ▲ | 30.3% [26.5%, 33.2%] | 36.8% [34.1%, 39.9%] | - | 50059 |
| Key chord forward (reloading) ▲ | 14.8% [11.1%, 20.5%] | 26.1% [23.8%, 28.6%] | - | 50059 |
| Key chord neutral (reloading) ▲ | 13.9% [11.6%, 17.2%] | 8.3% [7.3%, 9.5%] | - | 50059 |
| Key chord back any (reloading) | 9.6% [7.2%, 10.9%] | 6.5% [5.3%, 7.6%] | - | 50059 |
| Strafe end is a direct reverse (hidden, not firing) | 54.2% [49.7%, 57.2%] | 46.1% [41.1%, 51.2%] | - | 18526 |
| Strafe end is a direct reverse (hidden, firing) | 72.0% [65.5%, 76.9%] | 67.2% [59.7%, 74.3%] | - | 6760 |
| Strafe end is a direct reverse (LOS, not firing) | 60.5% [53.1%, 65.0%] | 52.8% [45.1%, 60.4%] | - | 5190 |
| Strafe end is a direct reverse (LOS firefight) | 77.4% [70.4%, 82.8%] | 73.3% [65.9%, 80.6%] | - | 16680 |
| Strafe end is a direct reverse (reloading) ▲ | 58.5% [51.2%, 63.9%] | 37.7% [32.5%, 42.7%] | - | 4907 |
| Strafe switch hazard per tick at hold age 50 ms (LOS firefight) ▲ | 9.4% [6.8%, 11.9%] | 1.2% [0.9%, 1.6%] | - | 17509 |
| Strafe switch hazard per tick at hold age 100 ms (LOS firefight) ▲ | 21.5% [18.2%, 24.1%] | 5.8% [5.2%, 6.8%] | - | 15308 |
| Strafe switch hazard per tick at hold age 150 ms (LOS firefight) ▲ | 23.7% [23.0%, 24.5%] | 12.4% [11.1%, 13.9%] | - | 12151 |
| Strafe switch hazard per tick at hold age 200 ms (LOS firefight) ▲ | 22.6% [21.8%, 23.2%] | 17.0% [15.4%, 18.7%] | - | 9433 |
| Strafe switch hazard per tick at hold age 250 ms (LOS firefight) ▲ | 22.7% [21.6%, 23.7%] | 17.4% [16.0%, 19.0%] | - | 7384 |
| Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) ▲ | 22.8% [21.4%, 23.8%] | 15.6% [14.7%, 16.9%] | - | 16813 |
| Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) ▲ | 20.2% [18.7%, 21.9%] | 16.1% [14.9%, 17.5%] | - | 11390 |
| Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) | 16.4% [14.4%, 19.3%] | 13.7% [12.0%, 15.6%] | - | 1704 |
| Side-hold duration (LOS firefight) p25 ▲ | 100 ms [100 ms, 100 ms] | 200 ms [200 ms, 200 ms] | - | 15777 |
| Side-hold duration (LOS firefight) p50 ▲ | 200 ms [150 ms, 200 ms] | 300 ms [250 ms, 300 ms] | - | 15777 |
| Side-hold duration (LOS firefight) p75 ▲ | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | - | 15777 |
| Side-hold duration (LOS firefight) p90 ▲ | 500 ms [450 ms, 500 ms] | 700 ms [650 ms, 700 ms] | - | 15777 |
| Side-hold duration (all contexts) p50 ▲ | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | - | 50717 |
| Side-hold duration (all contexts) p90 ▲ | 600 ms [600 ms, 650 ms] | 800 ms [800 ms, 850 ms] | - | 50717 |
| Neutral gap between strafes (LOS firefight) p50 | 50 ms [50 ms, 50 ms] | 50 ms [50 ms, 50 ms] | - | 5261 |
| Neutral gap between strafes (LOS firefight) p90 | 100 ms [50 ms, 100 ms] | 100 ms [100 ms, 150 ms] | - | 5261 |
| Approaching >40 u/s in LOS firefight at distance (0, 96] | 37.5% [33.2%, 42.3%] | 37.6% [33.7%, 42.3%] | - | 10333 |
| Approaching >40 u/s in LOS firefight at distance (96, 160] | 38.2% [32.4%, 45.3%] | 36.4% [32.6%, 41.2%] | - | 15259 |
| Approaching >40 u/s in LOS firefight at distance (160, 224] | 36.0% [32.0%, 43.2%] | 35.4% [30.1%, 42.0%] | - | 17900 |
| Approaching >40 u/s in LOS firefight at distance (224, 288] | 37.3% [32.8%, 46.1%] | 38.2% [32.3%, 46.4%] | - | 16541 |
| Approaching >40 u/s in LOS firefight at distance (288, 384] | 40.7% [35.4%, 48.1%] | 39.4% [32.7%, 47.2%] | - | 19861 |
| Approaching >40 u/s in LOS firefight at distance (384, 512] | 40.7% [32.5%, 51.2%] | 41.8% [33.8%, 49.1%] | - | 14341 |
| Approaching >40 u/s in LOS firefight at distance (512, 768] | 39.5% [23.5%, 54.3%] | 33.1% [25.5%, 41.5%] | - | 7261 |
| Approaching >40 u/s in LOS firefight at distance (768, 1200] | 38.4% [7.9%, 64.1%] | 28.2% [17.6%, 42.7%] | - | 318 |
| Retreating >40 u/s in LOS firefight at distance (0, 96] | 23.1% [20.8%, 25.8%] | 22.2% [18.9%, 25.6%] | - | 10333 |
| Retreating >40 u/s in LOS firefight at distance (96, 160] ▲ | 17.9% [15.6%, 20.7%] | 10.5% [8.5%, 12.6%] | - | 15259 |
| Retreating >40 u/s in LOS firefight at distance (160, 224] ▲ | 13.8% [12.0%, 15.1%] | 6.9% [5.5%, 8.7%] | - | 17900 |
| Retreating >40 u/s in LOS firefight at distance (224, 288] ▲ | 9.9% [8.9%, 10.7%] | 5.3% [4.2%, 6.8%] | - | 16541 |
| Retreating >40 u/s in LOS firefight at distance (288, 384] | 8.5% [6.6%, 10.0%] | 5.9% [4.6%, 7.3%] | - | 19861 |
| Retreating >40 u/s in LOS firefight at distance (384, 512] | 6.5% [5.2%, 8.2%] | 4.8% [3.8%, 6.0%] | - | 14341 |
| Retreating >40 u/s in LOS firefight at distance (512, 768] | 7.0% [4.8%, 11.0%] | 4.0% [3.0%, 5.4%] | - | 7261 |
| Retreating >40 u/s in LOS firefight at distance (768, 1200] | 9.7% [4.3%, 23.3%] | 8.3% [1.5%, 15.5%] | - | 318 |
| Standing still (<5 u/s), all duel time ▲ | 9.0% [8.2%, 10.0%] | 15.7% [13.4%, 17.7%] | - | 489160 |
| Standing still (<5 u/s) (hidden, not firing) ▲ | 14.8% [13.4%, 16.8%] | 22.5% [19.5%, 25.1%] | - | 228160 |
| Standing still (<5 u/s) (hidden, firing) ▲ | 6.9% [6.5%, 7.3%] | 10.2% [9.1%, 11.7%] | - | 59799 |
| Standing still (<5 u/s) (LOS, not firing) ▲ | 6.6% [5.3%, 8.2%] | 29.6% [12.6%, 44.0%] | - | 49325 |
| Standing still (<5 u/s) (LOS firefight) ▲ | 1.4% [1.2%, 1.7%] | 2.3% [1.8%, 2.7%] | - | 101810 |
| Standing still (<5 u/s) (reloading) | 5.9% [4.5%, 7.7%] | 5.2% [4.3%, 6.2%] | - | 50059 |
| Lean held, all duel time ▲ | 74.7% [71.7%, 77.2%] | 59.7% [55.3%, 64.0%] | - | 489160 |
| Lean held in LOS firefights ▲ | 88.5% [86.7%, 89.8%] | 81.3% [76.6%, 85.8%] | - | 101810 |
| Jump presses per minute | 1.82/min [1.28/min, 2.33/min] | 1.87/min [1.41/min, 2.44/min] | - | 489160 |
| Crouch presses per minute | 4.64/min [3.39/min, 5.87/min] | 4.33/min [3.22/min, 5.51/min] | - | 489160 |
| Walk presses per minute ▲ | 6.40/min [5.90/min, 6.85/min] | 7.54/min [6.90/min, 8.11/min] | - | 489160 |
| Walking while hidden and not firing | 6.2% [5.2%, 7.2%] | 5.9% [5.0%, 7.0%] | - | 228160 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Press hazard with LOS and aim within 3 half-widths, LOS gained 0 ms ago | 25.7% [23.5%, 29.4%] | 28.2% [25.8%, 30.8%] | - | 1650 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 50-100 ms ago | 33.5% [29.6%, 39.2%] | 33.0% [30.8%, 35.6%] | - | 2482 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 150-250 ms ago | 33.7% [29.5%, 39.6%] | 35.2% [32.9%, 38.1%] | - | 2305 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago | 33.9% [28.1%, 41.8%] | 28.0% [25.2%, 32.0%] | - | 1935 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 550-1000 ms ago | 26.1% [21.8%, 32.2%] | 21.0% [16.7%, 24.6%] | - | 1955 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago ▲ | 21.4% [17.2%, 27.2%] | 7.4% [4.7%, 10.8%] | - | 2924 |
| Press hazard while hidden, LOS lost 0 ms ago | 11.6% [8.7%, 15.2%] | 12.3% [10.8%, 14.5%] | - | 2613 |
| Press hazard while hidden, LOS lost 50-100 ms ago | 11.3% [10.1%, 13.0%] | 9.5% [8.5%, 10.9%] | - | 3678 |
| Press hazard while hidden, LOS lost 150-250 ms ago ▲ | 8.9% [7.9%, 10.2%] | 4.6% [3.9%, 5.5%] | - | 5438 |
| Press hazard while hidden, LOS lost 300-500 ms ago ▲ | 11.4% [11.0%, 11.8%] | 7.0% [6.0%, 8.1%] | - | 17611 |
| Press hazard while hidden, LOS lost 550-1000 ms ago | 3.6% [3.2%, 4.0%] | 3.5% [3.2%, 3.9%] | - | 29692 |
| Press hazard while hidden, LOS lost over 1 s ago ▲ | 2.3% [2.0%, 2.7%] | 4.1% [3.5%, 4.6%] | - | 157160 |
| Release hazard, LOS, aim error in half-widths (0, 1] | 1.5% [1.2%, 1.9%] | 1.4% [1.1%, 1.7%] | - | 17704 |
| Release hazard, LOS, aim error in half-widths (1, 2] | 1.5% [1.2%, 1.8%] | 1.5% [1.3%, 1.9%] | - | 29953 |
| Release hazard, LOS, aim error in half-widths (2, 3] | 1.5% [1.3%, 1.8%] | 1.5% [1.2%, 2.0%] | - | 22655 |
| Release hazard, LOS, aim error in half-widths (3, 4] | 1.6% [1.4%, 1.9%] | 1.4% [1.0%, 2.0%] | - | 13315 |
| Release hazard, LOS, aim error in half-widths (4, 6] | 2.5% [2.1%, 2.8%] | 1.7% [1.3%, 2.3%] | - | 10829 |
| Release hazard, LOS, aim error in half-widths (6, 10] | 8.2% [6.6%, 10.0%] | 6.9% [4.4%, 9.9%] | - | 3894 |
| Release hazard, LOS, aim error in half-widths (10, 1000] ▲ | 19.0% [17.3%, 21.6%] | 32.7% [22.9%, 41.8%] | - | 1545 |
| Release hazard while hidden, LOS lost 0-100 ms ago | 4.7% [3.7%, 6.4%] | 6.3% [5.4%, 7.5%] | - | 14499 |
| Release hazard while hidden, LOS lost 150-250 ms ago | 11.5% [9.8%, 14.8%] | 16.5% [13.8%, 18.7%] | - | 8200 |
| Release hazard while hidden, LOS lost 300-450 ms ago | 29.5% [26.8%, 32.3%] | 31.5% [29.6%, 33.4%] | - | 8644 |
| Release hazard while hidden, LOS lost 500-950 ms ago ▲ | 14.1% [12.5%, 15.8%] | 10.1% [9.2%, 11.3%] | - | 7433 |
| Release hazard while hidden, LOS lost 1 s or more ago ▲ | 14.4% [13.8%, 14.8%] | 11.3% [9.8%, 13.5%] | - | 18576 |
| Fire held, LOS, aim error in half-widths (0, 0.5] | 84.5% [80.3%, 88.4%] | 89.6% [87.0%, 91.7%] | - | 5940 |
| Fire held, LOS, aim error in half-widths (0.5, 1] | 85.9% [81.9%, 89.6%] | 88.9% [86.9%, 90.5%] | - | 14929 |
| Fire held, LOS, aim error in half-widths (1, 1.5] | 85.9% [82.7%, 89.1%] | 86.3% [84.5%, 87.8%] | - | 18195 |
| Fire held, LOS, aim error in half-widths (1.5, 2] ▲ | 85.6% [83.8%, 87.9%] | 81.9% [79.8%, 83.7%] | - | 17757 |
| Fire held, LOS, aim error in half-widths (2, 3] | 84.1% [82.0%, 86.9%] | 81.0% [79.4%, 82.7%] | - | 27979 |
| Fire held, LOS, aim error in half-widths (3, 4] | 79.3% [76.6%, 82.8%] | 78.8% [76.1%, 81.2%] | - | 17460 |
| Fire held, LOS, aim error in half-widths (4, 6] | 67.6% [64.7%, 71.8%] | 67.7% [63.0%, 71.3%] | - | 16846 |
| Fire held, LOS, aim error in half-widths (6, 10] ▲ | 38.3% [33.6%, 44.4%] | 22.4% [17.1%, 27.1%] | - | 10509 |
| Fire held, LOS, aim error in half-widths (10, 20] ▲ | 12.5% [9.4%, 15.6%] | 4.6% [2.4%, 6.9%] | - | 8557 |
| Fire held, LOS, aim error in half-widths (20, 1000] ▲ | 5.4% [3.9%, 6.6%] | 0.9% [0.4%, 3.1%] | - | 12157 |
| Fire held without LOS | 23.4% [20.7%, 26.4%] | 24.5% [22.7%, 26.3%] | - | 273700 |
| Attack hold duration p50 | 200 ms [200 ms, 200 ms] | 150 ms [150 ms, 200 ms] | - | 12805 |
| Attack hold duration p90 | 1000 ms [950 ms, 1050 ms] | 900 ms [800 ms, 950 ms] | - | 12805 |
| Attack holds that are taps (<=100 ms) ▲ | 39.1% [37.5%, 40.4%] | 44.3% [41.8%, 46.5%] | - | 12805 |
| Gap between attack holds p50 | 300 ms [300 ms, 350 ms] | 400 ms [350 ms, 400 ms] | - | 12254 |

### Aim

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error firing with LOS at (0, 128] u ▲ | 19.18° [18.24°, 20.00°] | 14.28° [13.53°, 15.19°] | - | 15914 |
| Crosshair on body firing with LOS at (0, 128] u ▲ | 39.2% [37.3%, 41.4%] | 48.0% [44.9%, 51.5%] | - | 15914 |
| Median aim error firing with LOS at (128, 192] u ▲ | 10.93° [10.55°, 11.29°] | 8.99° [8.66°, 9.45°] | - | 16659 |
| Crosshair on body firing with LOS at (128, 192] u ▲ | 35.4% [33.8%, 36.6%] | 42.5% [39.8%, 44.7%] | - | 16659 |
| Median aim error firing with LOS at (192, 256] u ▲ | 8.07° [7.78°, 8.36°] | 6.68° [6.36°, 7.10°] | - | 16653 |
| Crosshair on body firing with LOS at (192, 256] u ▲ | 32.7% [31.7%, 33.7%] | 42.0% [39.5%, 44.4%] | - | 16653 |
| Median aim error firing with LOS at (256, 384] u ▲ | 6.02° [5.73°, 6.20°] | 5.23° [4.96°, 5.59°] | - | 29012 |
| Crosshair on body firing with LOS at (256, 384] u ▲ | 30.4% [29.0%, 31.8%] | 39.0% [36.0%, 41.4%] | - | 29012 |
| Median aim error firing with LOS at (384, 512] u | 4.34° [4.13°, 4.53°] | 3.92° [3.67°, 4.18°] | - | 15025 |
| Crosshair on body firing with LOS at (384, 512] u ▲ | 29.6% [27.4%, 32.3%] | 35.4% [32.6%, 38.3%] | - | 15025 |
| Median aim error firing with LOS at (512, 768] u | 3.35° [3.23°, 3.56°] | 3.07° [2.80°, 3.35°] | - | 8234 |
| Crosshair on body firing with LOS at (512, 768] u ▲ | 27.4% [25.1%, 28.9%] | 32.0% [29.1%, 35.1%] | - | 8234 |
| Median aim error firing with LOS at (768, 1200] u | 3.16° [2.41°, 4.04°] | 2.14° [1.84°, 2.58°] | - | 318 |
| Crosshair on body firing with LOS at (768, 1200] u | 22.4% [11.6%, 29.3%] | 31.5% [24.3%, 39.5%] | - | 318 |
| Aim height (fraction of body) firing with LOS | 0.419 [0.4, 0.434] | 0.446 [0.418, 0.468] | - | 101820 |
| Aim height (fraction of body) with LOS, not firing | 0.611 [0.582, 0.637] | 0.662 [0.576, 0.751] | - | 72076 |

### View

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw speed (hidden, not firing) p50 ▲ | 12°/s [11°/s, 12°/s] | 8°/s [6°/s, 10°/s] | - | 225880 |
| Yaw speed (hidden, not firing) p99 ▲ | 677°/s [639°/s, 699°/s] | 544°/s [521°/s, 580°/s] | - | 225880 |
| Mouse still between ticks (hidden, not firing) | 30.9% [30.3%, 31.8%] | 32.1% [28.8%, 36.0%] | - | 228160 |
| Yaw speed (hidden, firing) p50 ▲ | 33°/s [31°/s, 35°/s] | 23°/s [21°/s, 25°/s] | - | 59799 |
| Yaw speed (hidden, firing) p99 ▲ | 511°/s [423°/s, 555°/s] | 194°/s [182°/s, 204°/s] | - | 59799 |
| Mouse still between ticks (hidden, firing) ▲ | 7.7% [7.0%, 8.4%] | 10.9% [10.1%, 11.8%] | - | 59799 |
| Yaw speed (LOS, not firing) p50 ▲ | 33°/s [30°/s, 36°/s] | 11°/s [0°/s, 26°/s] | - | 49308 |
| Yaw speed (LOS, not firing) p99 ▲ | 801°/s [774°/s, 827°/s] | 583°/s [505°/s, 695°/s] | - | 49308 |
| Mouse still between ticks (LOS, not firing) ▲ | 19.3% [18.3%, 20.5%] | 36.7% [20.8%, 49.8%] | - | 49325 |
| Yaw speed (LOS firefight) p50 ▲ | 54°/s [51°/s, 58°/s] | 39°/s [36°/s, 41°/s] | - | 101810 |
| Yaw speed (LOS firefight) p99 ▲ | 680°/s [602°/s, 745°/s] | 305°/s [292°/s, 318°/s] | - | 101810 |
| Mouse still between ticks (LOS firefight) ▲ | 3.1% [2.9%, 3.3%] | 1.6% [1.4%, 1.8%] | - | 101810 |
| Yaw speed (reloading) p50 ▲ | 30°/s [28°/s, 33°/s] | 18°/s [16°/s, 20°/s] | - | 50059 |
| Yaw speed (reloading) p99 ▲ | 767°/s [726°/s, 818°/s] | 945°/s [909°/s, 991°/s] | - | 50059 |
| Mouse still between ticks (reloading) ▲ | 19.2% [17.6%, 20.5%] | 26.3% [25.0%, 27.6%] | - | 50059 |
| Peak yaw speed of 90-135 deg turns p50 | 649°/s [636°/s, 665°/s] | 619°/s [595°/s, 655°/s] | - | 3160 |
| Peak yaw speed of 135-180 deg turns p50 | 857°/s [829°/s, 891°/s] | 852°/s [828°/s, 887°/s] | - | 1942 |
| Peak yaw speed of 180-360 deg turns p50 | 1117°/s [957°/s, 1200°/s] | 1061°/s [1020°/s, 1126°/s] | - | 343 |

### Belief

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw error to the hidden opponent, last seen <=250 ms ▲ | 6.28° [5.80°, 6.69°] | 4.92° [4.56°, 5.26°] | - | 35214 |
| View within 30 deg of the hidden opponent, last seen <=250 ms ▲ | 85.9% [84.6%, 87.3%] | 92.7% [91.9%, 93.7%] | - | 35214 |
| Yaw error to the hidden opponent, last seen 250-500 ms | 6.34° [5.76°, 6.90°] | 6.66° [5.99°, 7.28°] | - | 19819 |
| View within 30 deg of the hidden opponent, last seen 250-500 ms ▲ | 89.8% [88.6%, 91.0%] | 92.2% [91.3%, 93.4%] | - | 19819 |
| Yaw error to the hidden opponent, last seen 0.5-1 s | 8.79° [7.84°, 9.93°] | 8.28° [7.48°, 9.11°] | - | 21035 |
| View within 30 deg of the hidden opponent, last seen 0.5-1 s | 86.3% [83.3%, 88.6%] | 88.0% [86.8%, 89.9%] | - | 21035 |
| Yaw error to the hidden opponent, last seen 1-2 s | 11.58° [9.86°, 13.29°] | 11.11° [9.95°, 12.13°] | - | 14585 |
| View within 30 deg of the hidden opponent, last seen 1-2 s | 76.8% [73.4%, 81.1%] | 78.9% [77.1%, 81.1%] | - | 14585 |
| Yaw error to the hidden opponent, last seen 2-4 s | 22.24° [18.68°, 26.07°] | 16.93° [15.18°, 19.08°] | - | 48288 |
| View within 30 deg of the hidden opponent, last seen 2-4 s ▲ | 56.4% [53.0%, 60.4%] | 64.3% [61.3%, 67.3%] | - | 48288 |
| Yaw error to the hidden opponent, last seen 4-8 s ▲ | 23.50° [19.27°, 28.07°] | 10.29° [9.19°, 11.80°] | - | 55865 |
| View within 30 deg of the hidden opponent, last seen 4-8 s ▲ | 55.6% [51.5%, 59.5%] | 76.4% [73.1%, 79.3%] | - | 55865 |
| Yaw error to the hidden opponent, last seen >8 s | 27.94° [19.15°, 55.14°] | 20.07° [12.82°, 29.16°] | - | 14018 |
| View within 30 deg of the hidden opponent, last seen >8 s | 51.7% [40.0%, 63.5%] | 65.1% [54.8%, 74.8%] | - | 14018 |
| Yaw error to the hidden opponent, last seen never seen this life | 19.47° [17.02°, 22.21°] | 19.23° [16.93°, 24.71°] | - | 106440 |
| View within 30 deg of the hidden opponent, last seen never seen this life | 63.2% [59.5%, 67.1%] | 62.5% [56.0%, 67.2%] | - | 106440 |

### Space

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) ▲ | 39.0% [32.2%, 40.9%] | 49.4% [44.0%, 50.8%] | - | 119440 |
| Share of duel time in the busiest 10% of 32u cells (dm/main) ▲ | 35.2% [31.9%, 35.5%] | 45.2% [40.5%, 48.6%] | - | 121380 |
| Share of duel time in the busiest 10% of 32u cells (dm/vents) | 42.6% [37.4%, 44.6%] | 44.3% [39.2%, 46.8%] | - | 120470 |
| Share of duel time in the busiest 10% of 32u cells (dm/downladder) ▲ | 40.3% [32.0%, 40.9%] | 61.4% [42.9%, 63.8%] | - | 127870 |

### Acquisition

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error at -500 ms from sight gain | 15.29° [13.62°, 16.99°] | 13.98° [12.90°, 15.10°] | - | 5526 |
| Median aim error at -200 ms from sight gain ▲ | 14.46° [12.74°, 16.05°] | 8.33° [7.70°, 8.92°] | - | 5536 |
| Median aim error at +0 ms from sight gain ▲ | 13.74° [11.78°, 15.35°] | 5.22° [4.88°, 5.61°] | - | 5548 |
| Median aim error at +100 ms from sight gain ▲ | 11.69° [9.92°, 13.21°] | 5.97° [5.54°, 6.43°] | - | 5548 |
| Median aim error at +200 ms from sight gain ▲ | 9.18° [7.75°, 10.36°] | 4.92° [4.54°, 5.23°] | - | 5548 |
| Median aim error at +300 ms from sight gain ▲ | 8.16° [6.91°, 9.10°] | 4.53° [4.12°, 4.88°] | - | 5470 |
| Median aim error at +500 ms from sight gain ▲ | 7.13° [6.36°, 8.10°] | 5.47° [5.08°, 5.88°] | - | 5176 |
| Median aim error at +1000 ms from sight gain | 8.05° [7.22°, 8.84°] | 7.25° [6.72°, 7.70°] | - | 4257 |
| Fire held at -500 ms from sight gain ▲ | 23.4% [21.4%, 25.8%] | 18.2% [16.5%, 20.2%] | - | 5548 |
| Fire held at -200 ms from sight gain | 20.0% [18.5%, 21.9%] | 21.7% [19.7%, 23.9%] | - | 5548 |
| Fire held at +0 ms from sight gain ▲ | 26.3% [24.2%, 28.8%] | 42.5% [39.1%, 46.0%] | - | 5548 |
| Fire held at +100 ms from sight gain ▲ | 37.8% [34.8%, 41.9%] | 61.4% [57.8%, 64.6%] | - | 5548 |
| Fire held at +200 ms from sight gain ▲ | 49.9% [46.4%, 54.3%] | 77.0% [73.9%, 79.5%] | - | 5548 |
| Fire held at +400 ms from sight gain ▲ | 66.3% [63.2%, 69.6%] | 87.3% [85.5%, 89.0%] | - | 5433 |
| Fire held at +700 ms from sight gain ▲ | 66.5% [62.7%, 68.9%] | 77.0% [74.3%, 79.7%] | - | 5191 |
| Fire held at +1000 ms from sight gain | 60.3% [58.2%, 62.0%] | 63.4% [60.9%, 66.4%] | - | 4905 |
| Fire held at +1450 ms from sight gain | 52.8% [51.2%, 54.6%] | 53.6% [49.3%, 58.5%] | - | 4477 |
| Reaction: first trigger press after a clean sighting p25 | 100 ms [100 ms, 150 ms] | 50 ms [50 ms, 100 ms] | - | 2896 |
| Reaction: first trigger press after a clean sighting p50 | 250 ms [200 ms, 250 ms] | 150 ms [150 ms, 200 ms] | - | 2896 |
| Reaction: first trigger press after a clean sighting p75 ▲ | 400 ms [350 ms, 450 ms] | 300 ms [250 ms, 300 ms] | - | 2896 |
| Reaction: first trigger press after a clean sighting p90 ▲ | 700 ms [650 ms, 750 ms] | 400 ms [400 ms, 450 ms] | - | 2896 |
| Crosshair within half-width+1.5 deg after a clean sighting p50 ▲ | 300 ms [300 ms, 300 ms] | 200 ms [200 ms, 250 ms] | - | 2698 |
| Crosshair already on the body at the first visible tick ▲ | 12.0% [11.1%, 13.0%] | 22.6% [20.9%, 24.4%] | - | 5548 |
| Fire already held on the tick before sight (prefire) ▲ | 22.5% [21.0%, 24.4%] | 35.4% [32.2%, 38.6%] | - | 5548 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shots per burst (consecutive 100 ms shots) p50 | 5 [5, 5] | 5 [4, 5] | - | 12168 |
| Shots per burst (consecutive 100 ms shots) p90 ▲ | 16 [15, 17] | 12 [11, 13] | - | 12168 |

### Combat

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shot accuracy (eligible SMG shots that hit) ▲ | 15.6% [14.6%, 16.6%] | 19.2% [18.1%, 20.3%] | - | 78139 |

### Lives

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Respawn delay after death p50 ▲ | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | - | 2288 |
| Life length (lives ending in death) p50 ▲ | 10.8 s [10.3 s, 11.9 s] | 7.1 s [5.8 s, 8.2 s] | - | 2292 |

### Reload

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Reload within 3 s of a kill, rounds left 0 | 100.0% [100.0%, 100.0%] | 95.5% [86.4%, 100.0%] | - | 39 |
| Reload within 3 s of a kill, rounds left 1-3 | 97.1% [94.1%, 99.5%] | 96.4% [92.6%, 99.2%] | - | 144 |
| Reload within 3 s of a kill, rounds left 4-6 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 166 |
| Reload within 3 s of a kill, rounds left 7-10 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 259 |
| Reload within 3 s of a kill, rounds left 11-15 | 99.6% [99.0%, 100.0%] | 98.8% [97.9%, 99.5%] | - | 455 |
| Reload within 3 s of a kill, rounds left 16-20 | 95.5% [92.7%, 97.8%] | 95.7% [93.5%, 97.7%] | - | 440 |
| Reload within 3 s of a kill, rounds left 21-25 | 87.5% [84.6%, 89.9%] | 83.1% [76.5%, 88.5%] | - | 449 |
| Reload within 3 s of a kill, rounds left 26-29 | 65.5% [57.0%, 74.0%] | 63.4% [52.8%, 75.0%] | - | 295 |
| Reload within 3 s of a kill, rounds left 30-32 | 79.5% [68.7%, 89.1%] | 73.7% [50.0%, 92.3%] | - | 45 |
| Delay from kill to reload (reloads within 3 s) p50 | 650 ms [650 ms, 650 ms] | 600 ms [600 ms, 650 ms] | - | 2084 |
| Reloads started with the opponent dead ▲ | 69.8% [65.5%, 74.8%] | 88.2% [85.5%, 90.4%] | - | 2917 |
| Reloads with the opponent alive that are forced (empty clip) ▲ | 92.1% [88.5%, 95.0%] | 77.7% [69.5%, 83.9%] | - | 829 |

### Fights

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Engagements that end in a kill ▲ | 89.6% [88.6%, 90.5%] | 79.9% [77.9%, 81.9%] | - | 5196 |
| First hitter wins (decisive engagements) | 62.8% [61.6%, 64.1%] | 65.4% [63.7%, 66.7%] | - | 4564 |
| First shooter wins (decisive engagements) | 52.3% [51.1%, 53.7%] | 55.0% [52.5%, 56.6%] | - | 4564 |
| Duration of decisive engagements p50 ▲ | 2000 ms [1900 ms, 2050 ms] | 1250 ms [1150 ms, 1351 ms] | - | 4564 |
| Engagement start distance p50 | 412 u [372 u, 455 u] | 427 u [398 u, 470 u] | - | 5196 |
| Decisive engagements won (subject's own) | 49.4% [47.0%, 51.9%] | 50.0% [45.6%, 54.2%] | - | 4564 |

### Perception

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time with a body part on screen and the centroid hidden | 8.2% [5.1%, 10.3%] | 10.5% [9.6%, 11.8%] | - | 489160 |
| Share of duel time with a body part of the enemy on screen ▲ | 39.8% [36.9%, 42.6%] | 34.0% [31.6%, 36.5%] | - | 489160 |
| Sightings (a body part coming on screen) per minute of duel time | 32.06/min [29.67/min, 34.20/min] | 33.29/min [31.94/min, 34.72/min] | - | 489160 |
| Spell with no body part on screen between two sightings p50 | 300 ms [200 ms, 400 ms] | 400 ms [300 ms, 500 ms] | - | 7763 |
| Spell with no body part on screen between two sightings p75 | 700 ms [600 ms, 850 ms] | 900 ms [800 ms, 1000 ms] | - | 7763 |
| Spell with no body part on screen between two sightings p90 | 1450 ms [1250 ms, 1700 ms] | 1750 ms [1600 ms, 1950 ms] | - | 7763 |
| Time from both alive to the first body part on screen p50 | 1400 ms [1250 ms, 1501 ms] | 1350 ms [1300 ms, 1400 ms] | - | 4592 |
| Time from both alive to the first body part on screen p75 ▲ | 2600 ms [2400 ms, 2750 ms] | 1900 ms [1800 ms, 2000 ms] | - | 4592 |
| Net distance covered in 2 s from a tick with no body part on screen p50 | 137 u [125 u, 149 u] | 158 u [146 u, 173 u] | - | 24440 |
| Net distance covered in 2 s from a tick with no body part on screen p25 | 69 u [62 u, 78 u] | 66 u [54 u, 78 u] | - | 24440 |
| Fire held with no body part of the enemy visible (loaded, not reloading) | 17.1% [15.6%, 18.7%] | 17.3% [15.8%, 19.2%] | - | 271000 |
| Fire held with no body part visible, 0-500ms after one was last on screen ▲ | 48.4% [46.8%, 50.0%] | 43.2% [40.9%, 45.8%] | - | 42537 |
| Fire held with no body part visible, 500-1000ms after one was last on screen | 24.9% [23.5%, 26.7%] | 26.4% [23.9%, 28.7%] | - | 20646 |
| Fire held with no body part visible, 1000-2000ms after one was last on screen | 17.6% [15.5%, 19.7%] | 20.5% [18.0%, 22.9%] | - | 17936 |
| Fire held with no body part visible, 2000-5000ms after one was last on screen | 11.2% [9.1%, 13.8%] | 7.7% [6.4%, 9.1%] | - | 69796 |
| Fire held with no body part visible, gt5000ms after one was last on screen | 6.9% [6.0%, 8.1%] | 4.3% [3.1%, 6.0%] | - | 51725 |
| Attack holds begun loaded that are taps (<=100 ms), with a body part visible ▲ | 11.2% [9.7%, 12.7%] | 15.4% [13.1%, 17.6%] | - | 4604 |
| Attack holds begun loaded that are taps (<=100 ms), with no body part visible | 44.8% [42.9%, 46.3%] | 45.9% [42.2%, 48.9%] | - | 6521 |
| Shots per burst whose first round left with a body part visible p50 | 6 [6, 6] | 6 [5, 6] | - | 7829 |
| Shots per burst whose first round left with a body part visible p90 ▲ | 18 [16, 19] | 13 [12, 14] | - | 7829 |
| SMG shots that hit, a part on screen, centroid hidden ▲ | 11.8% [11.4%, 12.2%] | 21.7% [18.9%, 24.3%] | - | 9390 |
| SMG shots that hit, centroid visible ▲ | 21.3% [20.1%, 22.6%] | 27.2% [25.8%, 28.5%] | - | 50745 |
| Centroid sightings with a body part on screen first ▲ | 33.5% [31.9%, 35.3%] | 85.9% [84.2%, 87.7%] | - | 5526 |
| Lead of the first part over the centroid p50 ▲ | 50 ms [50 ms, 50 ms] | 100 ms [100 ms, 100 ms] | - | 1801 |
| Median aim error at -500 ms from the first visible part | 13.85° [12.36°, 15.19°] | 15.23° [14.18°, 16.19°] | - | 3110 |
| Median aim error at -200 ms from the first visible part | 12.22° [11.06°, 13.57°] | 10.54° [9.80°, 11.23°] | - | 3110 |
| Median aim error at -100 ms from the first visible part ▲ | 11.98° [10.64°, 12.84°] | 8.00° [7.34°, 8.49°] | - | 3110 |
| Median aim error at +0 ms from the first visible part ▲ | 11.51° [10.39°, 12.62°] | 5.22° [4.91°, 5.53°] | - | 3110 |
| Median aim error at +100 ms from the first visible part ▲ | 9.76° [8.87°, 10.52°] | 4.87° [4.54°, 5.18°] | - | 3110 |
| Median aim error at +200 ms from the first visible part ▲ | 6.73° [6.25°, 7.08°] | 5.26° [4.86°, 5.58°] | - | 3110 |
| Fire already held on the tick before the first part (prefire) ▲ | 18.1% [16.6%, 19.8%] | 23.0% [20.3%, 25.5%] | - | 3110 |
| Reaction: first press after a clean sighting, from the first part p50 | 200 ms [200 ms, 250 ms] | 200 ms [200 ms, 250 ms] | - | 1877 |
| Reaction: mean first press after a clean sighting, from the first part | 248 ms [222 ms, 268 ms] | 232 ms [217 ms, 250 ms] | - | 1877 |
| Crosshair within half-width+1.5 deg after a clean sighting, from the first part p50 | 250 ms [250 ms, 250 ms] | 250 ms [200 ms, 250 ms] | - | 1805 |
| First hit after the first part p50 ▲ | 350 ms [350 ms, 400 ms] | 250 ms [200 ms, 250 ms] | - | 2010 |
| Part sightings whose visible bout ends without a hit ▲ | 35.9% [32.0%, 39.6%] | 27.5% [25.7%, 29.7%] | - | 3110 |
| Yaw error to the hidden enemy 500 ms before the first part ▲ | 12.09° [10.46°, 13.64°] | 14.63° [13.70°, 15.63°] | - | 3110 |
| Yaw error to where the enemy will appear, 500 ms before the first part | 12.90° [11.48°, 14.63°] | 10.65° [9.39°, 11.89°] | - | 3110 |
| Crosshair parked within 5 deg of where the enemy will appear, 500 ms before | 25.1% [22.7%, 27.7%] | 29.4% [27.1%, 32.4%] | - | 3110 |
| Crosshair closer to where the enemy will appear than to the enemy, -500 ms ▲ | 47.9% [46.0%, 50.5%] | 67.0% [65.6%, 68.8%] | - | 3110 |
| View turn toward the enemy over the last 500 ms before the first part ▲ | 6.88° [6.18°, 7.75°] | 13.01° [11.44°, 14.55°] | - | 3110 |
| View turn over the last 500 ms before the first part ▲ | 9.78° [9.06°, 10.50°] | 14.53° [13.34°, 15.90°] | - | 3110 |
| Speed 400-200 ms before the first part ▲ | 167 u/s [155 u/s, 175 u/s] | 188 u/s [182 u/s, 193 u/s] | - | 3110 |
| Speed 2-1 s before the first part | 156 u/s [150 u/s, 161 u/s] | 167 u/s [159 u/s, 176 u/s] | - | 1646 |
| Hidden, no sighting next: crosshair closer to where the enemy will be in 500 ms | 48.6% [47.4%, 49.7%] | 50.5% [49.0%, 51.6%] | - | 194550 |
| Yaw error to the corner, 500 ms before the first part ▲ | 11.04° [10.03°, 12.43°] | 5.81° [5.17°, 6.44°] | - | 2170 |
| Angle to the corner, 500 ms before the first part ▲ | 13.25° [12.22°, 14.24°] | 8.41° [7.70°, 9.19°] | - | 2170 |
| Pitch to the corner (- = corner above the crosshair), 500 ms before the first part | -2.15° [-2.55°, -1.75°] | -2.72° [-3.14°, -2.39°] | - | 2170 |
| Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part ▲ | 0.29° [-0.21°, 0.80°] | -1.77° [-2.04°, -1.43°] | - | 2170 |
| Yaw error to the appearance point (sightings with a corner), 500 ms before the first part | 11.47° [10.14°, 12.74°] | 10.34° [9.19°, 11.29°] | - | 2170 |
| Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part ▲ | 10.85° [9.36°, 12.47°] | 15.18° [14.24°, 16.01°] | - | 2170 |
| Crosshair within 5 deg of the corner, 500 ms before the first part ▲ | 19.3% [17.3%, 21.4%] | 29.2% [26.4%, 32.7%] | - | 2170 |
| Crosshair closer to the corner than to the enemy, 500 ms before the first part ▲ | 52.8% [49.3%, 56.2%] | 80.0% [78.8%, 81.1%] | - | 2170 |
| Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part ▲ | 42.3% [40.6%, 44.1%] | 48.2% [45.7%, 50.3%] | - | 2170 |
| Crosshair within 2 deg of the corner's edge, 500 ms before the first part ▲ | 12.7% [11.2%, 14.3%] | 23.7% [21.7%, 25.7%] | - | 2170 |
| Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part ▲ | 45.0% [42.8%, 46.6%] | 28.1% [26.8%, 29.4%] | - | 2170 |
| Angle to the corner at the first part ▲ | 9.78° [8.87°, 10.76°] | 5.74° [5.33°, 6.17°] | - | 2170 |
| Distance from the eye to the corner p50 | 242 u [214 u, 272 u] | 228 u [204 u, 248 u] | - | 2170 |
| Yaw error to the corner at -2000 ms (sightings hidden >= 2 s) | 15.72° [13.15°, 17.95°] | 14.66° [11.85°, 17.22°] | - | 576 |
| Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) ▲ | 12.31° [9.40°, 15.47°] | 5.78° [4.88°, 6.98°] | - | 576 |
| Yaw error to the corner at -500 ms (sightings hidden >= 2 s) ▲ | 11.05° [9.38°, 12.77°] | 4.66° [3.99°, 5.31°] | - | 576 |
| Yaw error to the corner at +0 ms (sightings hidden >= 2 s) ▲ | 8.21° [7.39°, 9.92°] | 3.54° [3.01°, 4.10°] | - | 576 |

Consistency: 112 pooled statistics (bots and owner together) were recomputed from the analysis scripts' own JSON; 0 differ.

## Notes

- perception statistics on the bots' logged body-part column (ext_vis_parts: their own perception); the people's are on the data repo's rebuilt column, accepted against the logged one on 2026-09-28
- 100% of the bots' duel time is against another bot (the logged opponent is the nearest living enemy); the human reference is humans against humans only
- engagements.py counts contact episodes only while exactly two players are alive; with more than two players in a session the fight statistics cover the phases where one player is dead
