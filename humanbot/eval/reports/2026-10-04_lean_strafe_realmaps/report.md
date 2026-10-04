# Bot evaluation: ld_201

Generated 2026-10-04 by `humanbot/eval/compare.py`. Captures: `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`. 8 sessions on dm/crnodoors, dm/downladder, dm/main, dm/vents; capture dates 2026-10-04.
Human reference: `human_reference.json` (2026-10-02), 397 duel minutes, 72 player-sessions, 5 people.

## Bots in this capture

| Bot | Family | Seed | Sessions | Duel minutes |
|---|---|---:|---:|---:|
| `bot:stopper:4` | stopper | 711847621 | 1 | 25.5 |
| `bot:stopper:5` | stopper | 1529931226 | 1 | 25.5 |
| `bot:strafer:4` | strafer | 231333596 | 1 | 24.0 |
| `bot:presser:5` | presser | 1152489726 | 1 | 24.0 |
| `bot:stopper:4.2` | stopper | 2458011590 | 1 | 25.5 |
| `bot:strafer:5` | strafer | 4003987225 | 1 | 25.5 |
| `bot:strafer:4.2` | strafer | 3498264623 | 1 | 25.8 |
| `bot:stopper:5.2` | stopper | 2513918026 | 1 | 25.8 |
| `bot:presser:4` | presser | 52844662 | 1 | 26.9 |
| `bot:strafer:5.2` | strafer | 3859071546 | 1 | 26.9 |
| `bot:presser:4.2` | presser | 588158740 | 1 | 26.2 |
| `bot:presser:5.2` | presser | 4131749178 | 1 | 26.2 |
| `bot:stopper:4.3` | stopper | 97502759 | 1 | 27.7 |
| `bot:strafer:5.3` | strafer | 2592015535 | 1 | 27.7 |
| `bot:strafer:4.3` | strafer | 1086809828 | 1 | 26.8 |
| `bot:strafer:5.4` | strafer | 821368291 | 1 | 26.8 |

The owner (`pstN@vsbot`) has 0.0 duel minutes against the bots; those rows are never pooled with the bots.

Bot duel time against the owner 0%, against other bots 100%.

Family mix of bot duel time (observed → reweighted to the people's mix of duel time): presser 25% → 50%, stopper 31% → 26%, strafer 44% → 25%.

## Tells (137)

Statistics whose 95% CIs for the pooled bots and the pooled humans do not overlap, ranked by the standardized gap z = (bots − humans) / √(SE²bots + SE²humans). Only statistics with enough data on both sides are ranked.

| # | Statistic | Bots | Humans | z |
|---:|---|---|---|---:|
| 1 | Centroid sightings with a body part on screen first | 36.0% [33.5%, 39.0%] | 85.9% [84.2%, 87.7%] | -30.8 |
| 2 | Respawn delay after death p50 | 3000 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | +15.5 |
| 3 | Yaw speed (hidden, firing) p99 | 584°/s [524°/s, 633°/s] | 194°/s [182°/s, 204°/s] | +13.8 |
| 4 | Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part | 45.1% [43.4%, 47.3%] | 28.1% [26.8%, 29.4%] | +13.5 |
| 5 | Yaw speed (hidden, not firing) p99 | 845°/s [808°/s, 882°/s] | 544°/s [521°/s, 580°/s] | +12.5 |
| 6 | Fire held at +200 ms from sight gain | 46.1% [41.9%, 51.0%] | 77.0% [73.9%, 79.5%] | -11.3 |
| 7 | Fire held at +400 ms from sight gain | 63.2% [59.3%, 67.0%] | 87.3% [85.5%, 89.0%] | -11.2 |
| 8 | Crosshair already on the body at the first visible tick | 10.6% [9.3%, 11.8%] | 22.6% [20.9%, 24.4%] | -10.7 |
| 9 | Yaw speed (LOS firefight) p99 | 678°/s [624°/s, 749°/s] | 305°/s [292°/s, 318°/s] | +10.6 |
| 10 | Median aim error at +0 ms from sight gain | 16.07° [13.96°, 17.96°] | 5.22° [4.88°, 5.61°] | +10.4 |
| 11 | Median aim error at +0 ms from the first visible part | 12.22° [10.99°, 13.30°] | 5.22° [4.91°, 5.53°] | +10.4 |
| 12 | View within 30 deg of the hidden opponent, last seen 4-8 s | 54.2% [51.6%, 57.1%] | 76.4% [73.1%, 79.3%] | -10.1 |
| 13 | Mouse still between ticks (LOS firefight) | 3.0% [2.9%, 3.2%] | 1.6% [1.4%, 1.8%] | +9.8 |
| 14 | Crosshair closer to the corner than to the enemy, 500 ms before the first part | 55.6% [49.9%, 59.8%] | 80.0% [78.8%, 81.1%] | -9.8 |
| 15 | Yaw error to the corner at +0 ms (sightings hidden >= 2 s) | 12.58° [10.61°, 13.79°] | 3.54° [3.01°, 4.10°] | +9.6 |
| 16 | Side-hold duration (LOS firefight) p90 | 500 ms [500 ms, 550 ms] | 700 ms [650 ms, 700 ms] | -9.5 |
| 17 | Reaction: first trigger press after a clean sighting p90 | 700 ms [650 ms, 750 ms] | 400 ms [400 ms, 450 ms] | +9.4 |
| 18 | Fire held at +100 ms from sight gain | 35.3% [31.9%, 39.8%] | 61.4% [57.8%, 64.6%] | -9.4 |
| 19 | Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) | 22.1% [21.3%, 22.6%] | 15.6% [14.7%, 16.9%] | +9.3 |
| 20 | Median aim error at +100 ms from the first visible part | 10.48° [9.23°, 11.48°] | 4.87° [4.54°, 5.18°] | +9.1 |
| 21 | Yaw speed (hidden, firing) p50 | 34°/s [32°/s, 35°/s] | 23°/s [21°/s, 25°/s] | +9.0 |
| 22 | View within 30 deg of the hidden opponent, last seen <=250 ms | 81.0% [79.0%, 84.1%] | 92.7% [91.9%, 93.7%] | -8.7 |
| 23 | Fire held at +0 ms from sight gain | 24.8% [22.8%, 27.6%] | 42.5% [39.1%, 46.0%] | -8.6 |
| 24 | Duration of decisive engagements p50 | 2000 ms [1850 ms, 2100 ms] | 1250 ms [1150 ms, 1351 ms] | +8.5 |
| 25 | Yaw error to the hidden opponent, last seen 4-8 s | 24.82° [22.00°, 27.74°] | 10.29° [9.19°, 11.80°] | +8.3 |
| 26 | Side-hold duration (LOS firefight) p25 | 100 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | -8.1 |
| 27 | Engagements that end in a kill | 90.0% [88.8%, 91.3%] | 79.9% [77.9%, 81.9%] | +8.1 |
| 28 | Yaw error to the corner at -500 ms (sightings hidden >= 2 s) | 15.37° [13.11°, 17.64°] | 4.66° [3.99°, 5.31°] | +8.0 |
| 29 | Angle to the corner at the first part | 11.15° [9.85°, 12.27°] | 5.74° [5.33°, 6.17°] | +7.9 |
| 30 | Crosshair within half-width+1.5 deg after a clean sighting p50 | 300 ms [300 ms, 300 ms] | 200 ms [200 ms, 250 ms] | +7.7 |
| 31 | Strafe switch hazard per tick at hold age 150 ms (LOS firefight) | 23.4% [21.0%, 25.5%] | 12.4% [11.1%, 13.9%] | +7.7 |
| 32 | Strafe switch hazard per tick at hold age 100 ms (LOS firefight) | 20.5% [16.6%, 23.6%] | 5.8% [5.2%, 6.8%] | +7.6 |
| 33 | SMG shots that hit, a part on screen, centroid hidden | 11.6% [10.8%, 12.7%] | 21.7% [18.9%, 24.3%] | -7.6 |
| 34 | Median aim error at +300 ms from sight gain | 8.68° [7.82°, 9.61°] | 4.53° [4.12°, 4.88°] | +7.6 |
| 35 | Fire already held on the tick before sight (prefire) | 21.6% [20.2%, 23.5%] | 35.4% [32.2%, 38.6%] | -7.5 |
| 36 | Median aim error at -200 ms from sight gain | 16.12° [14.37°, 18.01°] | 8.33° [7.70°, 8.92°] | +7.5 |
| 37 | Median aim error at +200 ms from sight gain | 10.63° [9.04°, 11.82°] | 4.92° [4.54°, 5.23°] | +7.3 |
| 38 | Retreating >40 u/s in LOS firefight at distance (160, 224] | 14.2% [13.1%, 15.3%] | 6.9% [5.5%, 8.7%] | +7.2 |
| 39 | Median aim error at +100 ms from sight gain | 14.01° [11.65°, 15.89°] | 5.97° [5.54°, 6.43°] | +7.1 |
| 40 | Reloads started with the opponent dead | 69.2% [65.5%, 74.2%] | 88.2% [85.5%, 90.4%] | -7.1 |
| 41 | Median aim error at -100 ms from the first visible part | 12.55° [11.39°, 13.52°] | 8.00° [7.34°, 8.49°] | +7.0 |
| 42 | Crosshair closer to where the enemy will appear than to the enemy, -500 ms | 52.3% [47.7%, 55.9%] | 67.0% [65.6%, 68.8%] | -6.8 |
| 43 | Press hazard while hidden, LOS lost over 1 s ago | 2.1% [1.8%, 2.3%] | 4.1% [3.5%, 4.6%] | -6.8 |
| 44 | Yaw speed (LOS firefight) p50 | 54°/s [51°/s, 58°/s] | 39°/s [36°/s, 41°/s] | +6.7 |
| 45 | Yaw speed (reloading) p50 | 30°/s [27°/s, 33°/s] | 18°/s [16°/s, 20°/s] | +6.7 |
| 46 | Fire held at +700 ms from sight gain | 64.2% [61.5%, 66.8%] | 77.0% [74.3%, 79.7%] | -6.6 |
| 47 | Crosshair on body firing with LOS at (192, 256] u | 31.5% [30.0%, 33.0%] | 42.0% [39.5%, 44.4%] | -6.5 |
| 48 | Retreating >40 u/s in LOS firefight at distance (96, 160] | 18.9% [17.1%, 20.1%] | 10.5% [8.5%, 12.6%] | +6.5 |
| 49 | Median aim error firing with LOS at (192, 256] u | 8.35° [7.98°, 8.66°] | 6.68° [6.36°, 7.10°] | +6.4 |
| 50 | View within 30 deg of the hidden opponent, last seen 250-500 ms | 86.7% [85.5%, 88.0%] | 92.2% [91.3%, 93.4%] | -6.3 |
| 51 | Crosshair within 2 deg of the corner's edge, 500 ms before the first part | 12.5% [10.1%, 15.3%] | 23.7% [21.7%, 25.7%] | -6.2 |
| 52 | Mouse still between ticks (hidden, firing) | 7.9% [7.5%, 8.4%] | 10.9% [10.1%, 11.8%] | -6.2 |
| 53 | Yaw speed (LOS, not firing) p99 | 925°/s [859°/s, 977°/s] | 583°/s [505°/s, 695°/s] | +6.2 |
| 54 | Side-hold duration (LOS firefight) p75 | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | -6.2 |
| 55 | Median aim error firing with LOS at (128, 192] u | 11.13° [10.65°, 11.75°] | 8.99° [8.66°, 9.45°] | +6.1 |
| 56 | Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part | 0.55° [-0.08°, 1.25°] | -1.77° [-2.04°, -1.43°] | +6.0 |
| 57 | Side-hold duration (all contexts) p90 | 600 ms [600 ms, 650 ms] | 800 ms [800 ms, 850 ms] | -6.0 |
| 58 | Speed 400-200 ms before the first part | 165 u/s [160 u/s, 169 u/s] | 188 u/s [182 u/s, 193 u/s] | -5.9 |
| 59 | Time from both alive to the first body part on screen p75 | 2700 ms [2500 ms, 2913 ms] | 1900 ms [1800 ms, 2000 ms] | +5.9 |
| 60 | Angle to the corner, 500 ms before the first part | 13.51° [11.95°, 14.73°] | 8.41° [7.70°, 9.19°] | +5.9 |
| 61 | Mouse still between ticks (reloading) | 20.2% [18.7%, 22.0%] | 26.3% [25.0%, 27.6%] | -5.8 |
| 62 | Median aim error firing with LOS at (0, 128] u | 18.85° [17.18°, 20.03°] | 14.28° [13.53°, 15.19°] | +5.7 |
| 63 | SMG shots that hit, centroid visible | 21.6% [20.4%, 22.9%] | 27.2% [25.8%, 28.5%] | -5.5 |
| 64 | Crosshair within 5 deg of the corner, 500 ms before the first part | 17.4% [14.8%, 20.1%] | 29.2% [26.4%, 32.7%] | -5.5 |
| 65 | Lean held, all duel time | 75.5% [72.8%, 78.0%] | 59.7% [55.3%, 64.0%] | +5.4 |
| 66 | Strafe switch hazard per tick at hold age 50 ms (LOS firefight) | 7.8% [5.3%, 10.1%] | 1.2% [0.9%, 1.6%] | +5.4 |
| 67 | Fire held, LOS, aim error in half-widths (20, 1000] | 8.7% [5.5%, 10.8%] | 0.9% [0.4%, 3.1%] | +5.3 |
| 68 | Crosshair on body firing with LOS at (128, 192] u | 33.8% [31.8%, 35.9%] | 42.5% [39.8%, 44.7%] | -5.3 |
| 69 | Yaw error to the corner, 500 ms before the first part | 11.54° [9.97°, 13.86°] | 5.81° [5.17°, 6.44°] | +5.2 |
| 70 | Strafe switch hazard per tick at hold age 200 ms (LOS firefight) | 21.9% [21.3%, 22.7%] | 17.0% [15.4%, 18.7%] | +5.1 |
| 71 | View within 30 deg of the hidden opponent, last seen 1-2 s | 68.5% [65.6%, 71.8%] | 78.9% [77.1%, 81.1%] | -5.1 |
| 72 | Fire held, LOS, aim error in half-widths (10, 20] | 12.8% [10.5%, 15.2%] | 4.6% [2.4%, 6.9%] | +5.0 |
| 73 | Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) | 20.5% [19.4%, 21.9%] | 16.1% [14.9%, 17.5%] | +5.0 |
| 74 | View turn toward the enemy over the last 500 ms before the first part | 7.72° [6.20°, 8.98°] | 13.01° [11.44°, 14.55°] | -4.9 |
| 75 | Median aim error at +200 ms from the first visible part | 7.01° [6.40°, 7.64°] | 5.26° [4.86°, 5.58°] | +4.9 |
| 76 | Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) | 20.89° [15.89°, 27.52°] | 5.78° [4.88°, 6.98°] | +4.9 |
| 77 | Key chord back any (LOS, not firing) | 9.6% [8.6%, 10.5%] | 4.8% [3.4%, 6.7%] | +4.9 |
| 78 | Life length (lives ending in death) p50 | 11.1 s [10.2 s, 12.0 s] | 7.1 s [5.8 s, 8.2 s] | +4.8 |
| 79 | Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago | 20.0% [15.6%, 24.4%] | 7.4% [4.7%, 10.8%] | +4.7 |
| 80 | Press hazard while hidden, LOS lost 150-250 ms ago | 7.9% [6.8%, 9.0%] | 4.6% [3.9%, 5.5%] | +4.7 |
| 81 | Shots per burst whose first round left with a body part visible p90 | 18 [16, 19] | 13 [12, 14] | +4.7 |
| 82 | Shot accuracy (eligible SMG shots that hit) | 15.4% [14.4%, 16.5%] | 19.2% [18.1%, 20.3%] | -4.7 |
| 83 | Key chord neutral (reloading) | 13.5% [11.8%, 15.1%] | 8.3% [7.3%, 9.5%] | +4.7 |
| 84 | Strafe switch hazard per tick at hold age 250 ms (LOS firefight) | 22.1% [20.8%, 23.3%] | 17.4% [16.0%, 19.0%] | +4.6 |
| 85 | View turn over the last 500 ms before the first part | 10.43° [8.79°, 11.33°] | 14.53° [13.34°, 15.90°] | -4.5 |
| 86 | Standing still (<5 u/s), all duel time | 9.7% [8.8%, 10.5%] | 15.7% [13.4%, 17.7%] | -4.5 |
| 87 | Strafe end is a direct reverse (reloading) | 58.4% [53.8%, 62.3%] | 37.7% [32.5%, 42.7%] | +4.5 |
| 88 | Share of duel time: hidden, firing | 12.5% [11.6%, 13.4%] | 15.8% [14.9%, 17.0%] | -4.5 |
| 89 | Standing still (<5 u/s) (hidden, not firing) | 15.2% [13.6%, 16.8%] | 22.5% [19.5%, 25.1%] | -4.4 |
| 90 | Standing still (<5 u/s) (hidden, firing) | 6.8% [6.2%, 7.5%] | 10.2% [9.1%, 11.7%] | -4.3 |
| 91 | Yaw error to the hidden opponent, last seen <=250 ms | 6.69° [6.08°, 7.40°] | 4.92° [4.56°, 5.26°] | +4.3 |
| 92 | Part sightings whose visible bout ends without a hit | 36.2% [33.1%, 39.5%] | 27.5% [25.7%, 29.7%] | +4.3 |
| 93 | Fire held, LOS, aim error in half-widths (6, 10] | 37.3% [32.5%, 42.0%] | 22.4% [17.1%, 27.1%] | +4.2 |
| 94 | Side-hold duration (LOS firefight) p50 | 200 ms [150 ms, 200 ms] | 300 ms [250 ms, 300 ms] | -4.2 |
| 95 | Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) | 38.1% [32.7%, 40.0%] | 49.4% [44.0%, 50.8%] | -4.1 |
| 96 | Net distance covered in 2 s from a tick with no body part on screen p50 | 120 u [111 u, 132 u] | 158 u [146 u, 173 u] | -4.0 |
| 97 | Side-hold duration (all contexts) p50 | 200 ms [200 ms, 250 ms] | 300 ms [300 ms, 300 ms] | -4.0 |
| 98 | Yaw speed (reloading) p99 | 820°/s [770°/s, 853°/s] | 945°/s [909°/s, 991°/s] | -4.0 |
| 99 | Share of duel time in the busiest 10% of 32u cells (dm/downladder) | 36.8% [32.4%, 38.0%] | 61.4% [42.9%, 63.8%] | -3.9 |
| 100 | Crosshair on body firing with LOS at (0, 128] u | 40.1% [38.2%, 42.3%] | 48.0% [44.9%, 51.5%] | -3.9 |
| 101 | Attack holds that are taps (<=100 ms) | 38.8% [37.5%, 40.4%] | 44.3% [41.8%, 46.5%] | -3.8 |
| 102 | Crosshair on body firing with LOS at (256, 384] u | 32.4% [30.5%, 34.9%] | 39.0% [36.0%, 41.4%] | -3.8 |
| 103 | Release hazard while hidden, LOS lost 1 s or more ago | 15.0% [14.4%, 15.7%] | 11.3% [9.8%, 13.5%] | +3.7 |
| 104 | Median aim error at -500 ms from sight gain | 17.69° [15.72°, 19.28°] | 13.98° [12.90°, 15.10°] | +3.7 |
| 105 | Key chord pure strafe (reloading) | 29.9% [26.8%, 33.0%] | 22.3% [19.6%, 24.4%] | +3.7 |
| 106 | Fire held with no body part visible, 1000-2000ms after one was last on screen | 15.0% [13.9%, 16.5%] | 20.5% [18.0%, 22.9%] | -3.7 |
| 107 | View within 30 deg of the hidden opponent, last seen 0.5-1 s | 83.4% [81.8%, 85.5%] | 88.0% [86.8%, 89.9%] | -3.6 |
| 108 | Median aim error at -200 ms from the first visible part | 13.11° [11.98°, 14.17°] | 10.54° [9.80°, 11.23°] | +3.6 |
| 109 | Share of duel time: LOS, not firing | 10.2% [9.4%, 11.2%] | 7.0% [5.5%, 8.4%] | +3.5 |
| 110 | Median aim error at +500 ms from sight gain | 7.54° [6.62°, 8.74°] | 5.47° [5.08°, 5.88°] | +3.5 |
| 111 | Key chord forward (reloading) | 15.8% [11.8%, 21.4%] | 26.1% [23.8%, 28.6%] | -3.5 |
| 112 | Retreating >40 u/s in LOS firefight at distance (224, 288] | 8.8% [7.6%, 10.2%] | 5.3% [4.2%, 6.8%] | +3.5 |
| 113 | Peak yaw speed of 180-360 deg turns p50 | 1183°/s [1131°/s, 1200°/s] | 1061°/s [1020°/s, 1126°/s] | +3.5 |
| 114 | Retreating >40 u/s in LOS firefight at distance (512, 768] | 7.7% [6.4%, 9.7%] | 4.0% [3.0%, 5.4%] | +3.4 |
| 115 | View within 30 deg of the hidden opponent, last seen 2-4 s | 54.3% [50.1%, 59.2%] | 64.3% [61.3%, 67.3%] | -3.4 |
| 116 | Shots per burst (consecutive 100 ms shots) p90 | 15 [14, 17] | 12 [11, 13] | +3.3 |
| 117 | Yaw speed (hidden, not firing) p50 | 12°/s [11°/s, 12°/s] | 8°/s [6°/s, 10°/s] | +3.3 |
| 118 | Fire already held on the tick before the first part (prefire) | 17.3% [14.9%, 19.1%] | 23.0% [20.3%, 25.5%] | -3.2 |
| 119 | Fire held at -500 ms from sight gain | 22.3% [20.7%, 23.7%] | 18.2% [16.5%, 20.2%] | +3.2 |
| 120 | First hit after the first part p50 | 350 ms [350 ms, 400 ms] | 250 ms [200 ms, 250 ms] | +3.2 |
| 121 | Key chord fwd diag (reloading) | 30.7% [28.9%, 32.1%] | 36.8% [34.1%, 39.9%] | -3.1 |
| 122 | Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part | 42.4% [39.4%, 44.6%] | 48.2% [45.7%, 50.3%] | -3.1 |
| 123 | Sightings (a body part coming on screen) per minute of duel time | 30.15/min [29.13/min, 31.26/min] | 33.29/min [31.94/min, 34.72/min] | -3.1 |
| 124 | Yaw speed (LOS, not firing) p50 | 35°/s [29°/s, 43°/s] | 11°/s [0°/s, 26°/s] | +3.0 |
| 125 | Reaction: first trigger press after a clean sighting p75 | 400 ms [400 ms, 450 ms] | 300 ms [250 ms, 300 ms] | +2.9 |
| 126 | Yaw error to the hidden opponent, last seen 2-4 s | 24.77° [19.97°, 29.82°] | 16.93° [15.18°, 19.08°] | +2.8 |
| 127 | Reloads with the opponent alive that are forced (empty clip) | 89.3% [86.4%, 92.3%] | 77.7% [69.5%, 83.9%] | +2.8 |
| 128 | Key chord pure strafe (LOS, not firing) | 37.3% [35.3%, 39.4%] | 26.7% [20.8%, 34.2%] | +2.7 |
| 129 | Release hazard, LOS, aim error in half-widths (10, 1000] | 19.7% [17.7%, 21.9%] | 32.7% [22.9%, 41.8%] | -2.6 |
| 130 | Fire held with no body part visible, 2000-5000ms after one was last on screen | 11.0% [9.2%, 13.1%] | 7.7% [6.4%, 9.1%] | +2.6 |
| 131 | Yaw error to the hidden opponent, last seen 1-2 s | 14.34° [12.24°, 16.75°] | 11.11° [9.95°, 12.13°] | +2.5 |
| 132 | Attack holds begun loaded that are taps (<=100 ms), with a body part visible | 12.2% [11.1%, 13.1%] | 15.4% [13.1%, 17.6%] | -2.5 |
| 133 | Standing still (<5 u/s) (LOS, not firing) | 7.6% [4.9%, 10.4%] | 29.6% [12.6%, 44.0%] | -2.5 |
| 134 | Key chord forward (LOS firefight) | 1.9% [1.5%, 2.5%] | 1.2% [1.0%, 1.4%] | +2.5 |
| 135 | Key chord forward (hidden, firing) | 9.2% [7.5%, 11.7%] | 6.6% [5.7%, 7.4%] | +2.2 |
| 136 | Share of duel time with a body part on screen and the centroid hidden | 8.0% [5.2%, 9.5%] | 10.5% [9.6%, 11.8%] | -2.2 |
| 137 | Lead of the first part over the centroid p50 | 50 ms [50 ms, 50 ms] | 100 ms [100 ms, 100 ms] | - |

## Per bot

### `bot:stopper:4` (stopper, seed 711847621, 25.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.07034 | 0.1467 | +15% |
| reverse_share | 0.1382 | 0.1248 | -2% |
| side_hold_ms | 249.3 | 250 | +0% |
| lean_fight | 0.9549 | 0.9711 | +3% |
| jumps_per_min | 0.3303 | 0.07831 | -5% |
| crouch_per_min | 5.571 | 6.656 | +10% |
| walk_hidden | 0 | 0.03772 | +36% ⚠ |
| burst_median | 4.564 | 4 | -14% |
| aim_height_firing | 0.4682 | 0.4685 | +0% |
| hold_angle | 0.4674 | 0.2694 | -64% ⚠ |
| counter_strafe | 0.5118 | 0.4678 | -9% |
| lean_switch | 0.03089 | 0.02763 | -1% |
| lean_drop | 0.0475 | 0.04382 | -2% |
| aim_error_fight_deg | 6.249 | 5.683 | -23% |
| reaction_ms | 153.7 | 250 | +96% ⚠ |
| mp40_share | 0.02524 | 0 | -3% |

Style fingerprint (2026-10-04, 25.5 min, styles.py features 30/30): z-distance to the human cloud centre 6.8 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 7.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lf_neutral 0.131 (humans 0.00436–0.112); lean_any 0.923 (humans 0.358–0.767); lf_lean 0.971 (humans 0.457–0.948); p_attack_given_los 0.525 (humans 0.547–0.861); yaw_speed_p95 317 (humans 158–271); side_hold_p90_ms 650 (humans 700–950); reverse_share 0.125 (humans 0.138–0.839).

### `bot:stopper:5` (stopper, seed 1529931226, 25.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1013 | 0.194 | +18% |
| reverse_share | 0.3253 | 0.3173 | -1% |
| side_hold_ms | 247.2 | 250 | +2% |
| lean_fight | 0.8427 | 0.9014 | +12% |
| jumps_per_min | 5.122 | 5.325 | +4% |
| crouch_per_min | 11.7 | 12.84 | +11% |
| walk_hidden | 0.07078 | 0.08349 | +12% |
| burst_median | 4.132 | 5 | +22% |
| aim_height_firing | 0.4597 | 0.448 | -6% |
| hold_angle | 0.5274 | 0.3417 | -60% ⚠ |
| counter_strafe | 0.4586 | 0.4193 | -8% |
| lean_switch | 0.0024 | 0.002901 | +0% |
| lean_drop | 0.0475 | 0.05185 | +2% |
| aim_error_fight_deg | 4.835 | 4.84 | +0% |
| reaction_ms | 109.9 | 250 | +140% ⚠ |
| mp40_share | 0.06948 | 0 | -7% |

Style fingerprint (2026-10-04, 25.5 min, styles.py features 30/30): z-distance to the human cloud centre 7.4 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 6.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.13 (humans 0.0109–0.0741); yaw_speed_p95 312 (humans 158–271); yaw_speed_p99 765 (humans 427–754); side_hold_p90_ms 650 (humans 700–950).

### `bot:strafer:4` (strafer, seed 231333596, 24.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.166 | +20% |
| reverse_share | 0.4467 | 0.4566 | +1% |
| side_hold_ms | 326.3 | 250 | -51% ⚠ |
| lean_fight | 0.4941 | 0.5857 | +18% |
| jumps_per_min | 2.379 | 2 | -7% |
| crouch_per_min | 9.476 | 10.92 | +13% |
| walk_hidden | 0 | 0.03634 | +34% ⚠ |
| burst_median | 8 | 7 | -25% ⚠ |
| aim_height_firing | 0.4857 | 0.478 | -4% |
| hold_angle | 0.2514 | 0.3065 | +18% |
| counter_strafe | 0.4612 | 0.5743 | +22% |
| lean_switch | 0.0024 | 0.00322 | +0% |
| lean_drop | 0.1003 | 0.1023 | +1% |
| aim_error_fight_deg | 4.667 | 4.563 | -4% |
| reaction_ms | 140.8 | 250 | +109% ⚠ |
| mp40_share | 0.9062 | 1 | +9% |

Style fingerprint (2026-10-04, 24.0 min, styles.py features 30/30): z-distance to the human cloud centre 7.6 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 5.0 vs the within-person threshold 5.6 (as close as two captures of one human).

Features outside the human min–max: pure_strafe 0.491 (humans 0.191–0.489); lean_any 0.354 (humans 0.358–0.767); ducked 0.0936 (humans 0.0109–0.0741); still 0.0698 (humans 0.0832–0.227); yaw_speed_p95 352 (humans 158–271); yaw_speed_p99 796 (humans 427–754); side_hold_p90_ms 650 (humans 700–950).

### `bot:presser:5` (presser, seed 1152489726, 24.0 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4832 | 0.5519 | +13% |
| reverse_share | 0.8051 | 0.8109 | +1% |
| side_hold_ms | 295.2 | 200 | -63% ⚠ |
| lean_fight | 0.9204 | 0.9581 | +8% |
| jumps_per_min | 0.4317 | 0.3333 | -2% |
| crouch_per_min | 3.157 | 3.167 | +0% |
| walk_hidden | 0.02816 | 0.07769 | +47% ⚠ |
| burst_median | 6.498 | 7 | +13% |
| aim_height_firing | 0.417 | 0.408 | -5% |
| hold_angle | 0.2275 | 0.2778 | +16% |
| counter_strafe | 0.1778 | 0.1044 | -14% |
| lean_switch | 0.2893 | 0.3894 | +32% ⚠ |
| lean_drop | 0.06758 | 0.08142 | +6% |
| aim_error_fight_deg | 4.893 | 5.243 | +14% |
| reaction_ms | 152.5 | 250 | +98% ⚠ |
| mp40_share | 0.1123 | 1 | +89% ⚠ |

Style fingerprint (2026-10-04, 24.0 min, styles.py features 30/30): z-distance to the human cloud centre 7.3 (human captures: up to 7.4; inside the human spread). Nearest human capture at z-distance 7.2 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.868 (humans 0.358–0.767); lf_lean 0.958 (humans 0.457–0.948); still 0.0813 (humans 0.0832–0.227); yaw_speed_p95 345 (humans 158–271); yaw_speed_p99 785 (humans 427–754); side_hold_p90_ms 600 (humans 700–950).

### `bot:stopper:4.2` (stopper, seed 2458011590, 25.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1182 | 0.1694 | +10% |
| reverse_share | 0.4266 | 0.3901 | -5% |
| side_hold_ms | 200 | 250 | +33% ⚠ |
| lean_fight | 0.8817 | 0.9292 | +9% |
| jumps_per_min | 2.965 | 2.749 | -4% |
| crouch_per_min | 8.679 | 7.225 | -13% |
| walk_hidden | 0.1056 | 0.1127 | +7% |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4719 | 0.47 | -1% |
| hold_angle | 0.3772 | 0.1531 | -73% ⚠ |
| counter_strafe | 0.6632 | 0.5693 | -18% |
| lean_switch | 0.03686 | 0.03671 | -0% |
| lean_drop | 0.0475 | 0.0522 | +2% |
| aim_error_fight_deg | 4.42 | 8.259 | +156% ⚠ |
| reaction_ms | 139.9 | 250 | +110% ⚠ |
| mp40_share | 0.02794 | 0 | -3% |

Style fingerprint (2026-10-04, 25.5 min, styles.py features 30/30): z-distance to the human cloud centre 8.0 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.6 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.814 (humans 0.358–0.767); p_attack_given_los 0.541 (humans 0.547–0.861); lf_err_med 8.26 (humans 4.08–6.63); yaw_speed_p95 337 (humans 158–271); lf_retreat 0.166 (humans 0.0243–0.141).

### `bot:strafer:5` (strafer, seed 4003987225, 25.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.1435 | +15% |
| reverse_share | 0.4657 | 0.4657 | -0% |
| side_hold_ms | 270.9 | 300 | +19% |
| lean_fight | 0.5402 | 0.5151 | -5% |
| jumps_per_min | 4.852 | 5.458 | +11% |
| crouch_per_min | 3.183 | 2.906 | -3% |
| walk_hidden | 0.03847 | 0.05767 | +18% |
| burst_median | 5.686 | 5 | -17% |
| aim_height_firing | 0.4588 | 0.466 | +4% |
| hold_angle | 0.311 | 0.1814 | -42% ⚠ |
| counter_strafe | 0.4314 | 0.3599 | -14% |
| lean_switch | 0.009884 | 0.01759 | +2% |
| lean_drop | 0.1344 | 0.1401 | +3% |
| aim_error_fight_deg | 5.446 | 8.625 | +129% ⚠ |
| reaction_ms | 161.3 | 250 | +89% ⚠ |
| mp40_share | 0 | 0 | +0% |

Style fingerprint (2026-10-04, 25.5 min, styles.py features 30/30): z-distance to the human cloud centre 7.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 6.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.336 (humans 0.358–0.767); attack 0.273 (humans 0.276–0.478); lf_err_med 8.62 (humans 4.08–6.63); yaw_speed_p95 341 (humans 158–271); lf_retreat 0.16 (humans 0.0243–0.141).

### `bot:strafer:4.2` (strafer, seed 3498264623, 25.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1522 | 0.1825 | +6% |
| reverse_share | 0.4724 | 0.446 | -4% |
| side_hold_ms | 293.3 | 250 | -29% ⚠ |
| lean_fight | 0.6201 | 0.6043 | -3% |
| jumps_per_min | 3.829 | 4.383 | +10% |
| crouch_per_min | 9.64 | 9.62 | -0% |
| walk_hidden | 0 | 0.03152 | +30% ⚠ |
| burst_median | 4.228 | 5 | +19% |
| aim_height_firing | 0.4919 | 0.506 | +8% |
| hold_angle | 0.3757 | 0.1749 | -65% ⚠ |
| counter_strafe | 0.422 | 0.34 | -16% |
| lean_switch | 0.0024 | 0.004913 | +1% |
| lean_drop | 0.1599 | 0.1653 | +2% |
| aim_error_fight_deg | 5.803 | 9.044 | +132% ⚠ |
| reaction_ms | 133.2 | 250 | +117% ⚠ |
| mp40_share | 0.5841 | 1 | +42% ⚠ |

Style fingerprint (2026-10-04, 25.8 min, styles.py features 30/30): z-distance to the human cloud centre 9.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.0981 (humans 0.0109–0.0741); attack 0.265 (humans 0.276–0.478); p_attack_given_los 0.532 (humans 0.547–0.861); lf_err_med 9.04 (humans 4.08–6.63); yaw_speed_p95 377 (humans 158–271); yaw_speed_p99 784 (humans 427–754); lf_retreat 0.154 (humans 0.0243–0.141).

### `bot:stopper:5.2` (stopper, seed 2513918026, 25.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1336 | 0.1746 | +8% |
| reverse_share | 0.6777 | 0.6726 | -1% |
| side_hold_ms | 226 | 250 | +16% |
| lean_fight | 0.9588 | 0.976 | +3% |
| jumps_per_min | 2.944 | 3.181 | +4% |
| crouch_per_min | 3.426 | 3.336 | -1% |
| walk_hidden | 0.08429 | 0.1128 | +27% ⚠ |
| burst_median | 4 | 5 | +25% ⚠ |
| aim_height_firing | 0.4602 | 0.4615 | +1% |
| hold_angle | 0.3824 | 0.2069 | -57% ⚠ |
| counter_strafe | 0.4565 | 0.3171 | -27% ⚠ |
| lean_switch | 0.009658 | 0.00721 | -1% |
| lean_drop | 0.05146 | 0.04658 | -2% |
| aim_error_fight_deg | 4.291 | 7.936 | +148% ⚠ |
| reaction_ms | 113 | 200 | +87% ⚠ |
| mp40_share | 0.07877 | 0 | -8% |

Style fingerprint (2026-10-04, 25.8 min, styles.py features 30/30): z-distance to the human cloud centre 7.5 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.7 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.946 (humans 0.358–0.767); lf_lean 0.976 (humans 0.457–0.948); lf_err_med 7.94 (humans 4.08–6.63); yaw_speed_p95 335 (humans 158–271); lf_retreat 0.151 (humans 0.0243–0.141).

### `bot:presser:4` (presser, seed 52844662, 26.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5317 | 0.3582 | -34% ⚠ |
| reverse_share | 0.8152 | 0.771 | -6% |
| side_hold_ms | 336.6 | 200 | -91% ⚠ |
| lean_fight | 0.935 | 0.97 | +7% |
| jumps_per_min | 0.336 | 0.4466 | +2% |
| crouch_per_min | 1.298 | 1.414 | +1% |
| walk_hidden | 0 | 0.03153 | +30% ⚠ |
| burst_median | 7.121 | 7 | -3% |
| aim_height_firing | 0.3853 | 0.334 | -27% ⚠ |
| hold_angle | 0.2402 | 0.2452 | +2% |
| counter_strafe | 0.2193 | 0.2691 | +10% |
| lean_switch | 0.2917 | 0.3647 | +23% |
| lean_drop | 0.06673 | 0.08777 | +10% |
| aim_error_fight_deg | 4.424 | 5.606 | +48% ⚠ |
| reaction_ms | 173.4 | 300 | +127% ⚠ |
| mp40_share | 0.06486 | 0 | -6% |

Style fingerprint (2026-10-04, 26.9 min, styles.py features 30/30): z-distance to the human cloud centre 8.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.903 (humans 0.358–0.767); lf_lean 0.97 (humans 0.457–0.948); p_attack_given_los 0.519 (humans 0.547–0.861); still 0.0768 (humans 0.0832–0.227); lf_height_frac 0.334 (humans 0.352–0.54); yaw_speed_p95 372 (humans 158–271); yaw_speed_p99 869 (humans 427–754); side_hold_p90_ms 600 (humans 700–950).

### `bot:strafer:5.2` (strafer, seed 3859071546, 26.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1614 | 0.1844 | +4% |
| reverse_share | 0.5449 | 0.5118 | -5% |
| side_hold_ms | 350 | 250 | -67% ⚠ |
| lean_fight | 0.5666 | 0.5887 | +4% |
| jumps_per_min | 0.6862 | 0.4838 | -4% |
| crouch_per_min | 3.8 | 3.796 | -0% |
| walk_hidden | 0.04377 | 0.04408 | +0% |
| burst_median | 7.775 | 7 | -19% |
| aim_height_firing | 0.4661 | 0.46 | -3% |
| hold_angle | 0.2527 | 0.3377 | +28% ⚠ |
| counter_strafe | 0.3766 | 0.494 | +23% |
| lean_switch | 0.0024 | 0.006649 | +1% |
| lean_drop | 0.2667 | 0.2721 | +2% |
| aim_error_fight_deg | 4.952 | 5.214 | +11% |
| reaction_ms | 115 | 150 | +35% ⚠ |
| mp40_share | 0.03683 | 0 | -4% |

Style fingerprint (2026-10-04, 26.9 min, styles.py features 30/30): z-distance to the human cloud centre 8.0 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 6.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.494 (humans 0.191–0.489); still 0.0823 (humans 0.0832–0.227); yaw_speed_p95 386 (humans 158–271); yaw_speed_p99 840 (humans 427–754); side_hold_p90_ms 650 (humans 700–950).

### `bot:presser:4.2` (presser, seed 588158740, 26.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4812 | 0.3332 | -29% ⚠ |
| reverse_share | 0.8389 | 0.8013 | -5% |
| side_hold_ms | 302.9 | 200 | -69% ⚠ |
| lean_fight | 0.9588 | 0.9743 | +3% |
| jumps_per_min | 1.077 | 0.7638 | -6% |
| crouch_per_min | 1.47 | 1.031 | -4% |
| walk_hidden | 0.004612 | 0.03372 | +28% ⚠ |
| burst_median | 7.045 | 8 | +24% |
| aim_height_firing | 0.352 | 0.311 | -22% |
| hold_angle | 0.2402 | 0.2706 | +10% |
| counter_strafe | 0.2096 | 0.2709 | +12% |
| lean_switch | 0.261 | 0.3168 | +18% |
| lean_drop | 0.07026 | 0.08299 | +6% |
| aim_error_fight_deg | 4.382 | 6.203 | +74% ⚠ |
| reaction_ms | 183.1 | 350 | +167% ⚠ |
| mp40_share | 0.9584 | 1 | +4% |

Style fingerprint (2026-10-04, 26.2 min, styles.py features 30/30): z-distance to the human cloud centre 7.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.928 (humans 0.358–0.767); lf_lean 0.974 (humans 0.457–0.948); p_attack_given_los 0.524 (humans 0.547–0.861); lf_height_frac 0.311 (humans 0.352–0.54); yaw_speed_p95 338 (humans 158–271); yaw_speed_p99 767 (humans 427–754); side_hold_p90_ms 550 (humans 700–950).

### `bot:presser:5.2` (presser, seed 4131749178, 26.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5433 | 0.3653 | -35% ⚠ |
| reverse_share | 0.823 | 0.7738 | -7% |
| side_hold_ms | 298.6 | 150 | -99% ⚠ |
| lean_fight | 0.9579 | 0.9773 | +4% |
| jumps_per_min | 0.5292 | 0.3819 | -3% |
| crouch_per_min | 2.785 | 2.33 | -4% |
| walk_hidden | 0.00312 | 0.03056 | +26% ⚠ |
| burst_median | 7.004 | 8 | +25% |
| aim_height_firing | 0.4126 | 0.372 | -22% |
| hold_angle | 0.2204 | 0.355 | +44% ⚠ |
| counter_strafe | 0.1785 | 0.1328 | -9% |
| lean_switch | 0.2726 | 0.3325 | +19% |
| lean_drop | 0.05341 | 0.06363 | +5% |
| aim_error_fight_deg | 4.873 | 6.332 | +59% ⚠ |
| reaction_ms | 134.4 | 200 | +66% ⚠ |
| mp40_share | 0.08844 | 0 | -9% |

Style fingerprint (2026-10-04, 26.2 min, styles.py features 30/30): z-distance to the human cloud centre 8.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.921 (humans 0.358–0.767); lf_lean 0.977 (humans 0.457–0.948); yaw_speed_p95 355 (humans 158–271); yaw_speed_p99 794 (humans 427–754); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 550 (humans 700–950).

### `bot:stopper:4.3` (stopper, seed 97502759, 27.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.0656 | 0.1828 | +23% |
| reverse_share | 0.6408 | 0.6195 | -3% |
| side_hold_ms | 200 | 250 | +33% ⚠ |
| lean_fight | 0.8757 | 0.9315 | +11% |
| jumps_per_min | 1.606 | 2.059 | +8% |
| crouch_per_min | 2.514 | 2.276 | -2% |
| walk_hidden | 0.09232 | 0.09266 | +0% |
| burst_median | 4 | 4 | +0% |
| aim_height_firing | 0.4804 | 0.509 | +15% |
| hold_angle | 0.418 | 0.2381 | -59% ⚠ |
| counter_strafe | 0.5804 | 0.4384 | -27% ⚠ |
| lean_switch | 0.05689 | 0.06122 | +1% |
| lean_drop | 0.06114 | 0.05598 | -2% |
| aim_error_fight_deg | 5.509 | 11.76 | +254% ⚠ |
| reaction_ms | 127.6 | 200 | +72% ⚠ |
| mp40_share | 0.1179 | 0 | -12% |

Style fingerprint (2026-10-04, 27.7 min, styles.py features 30/30): z-distance to the human cloud centre 10.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 11.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.811 (humans 0.358–0.767); attack 0.261 (humans 0.276–0.478); lf_err_med 11.8 (humans 4.08–6.63); yaw_speed_p95 405 (humans 158–271); yaw_speed_p99 822 (humans 427–754); side_hold_p90_ms 650 (humans 700–950); lf_retreat 0.183 (humans 0.0243–0.141).

### `bot:strafer:5.3` (strafer, seed 2592015535, 27.7 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.119 | 0.2131 | +18% |
| reverse_share | 0.4951 | 0.4557 | -6% |
| side_hold_ms | 295 | 300 | +3% |
| lean_fight | 0.4565 | 0.3825 | -15% |
| jumps_per_min | 5.657 | 5.202 | -8% |
| crouch_per_min | 11.59 | 11.27 | -3% |
| walk_hidden | 0 | 0.02257 | +21% |
| burst_median | 4.178 | 4 | -4% |
| aim_height_firing | 0.54 | 0.549 | +5% |
| hold_angle | 0.3001 | 0.2459 | -18% |
| counter_strafe | 0.5889 | 0.4406 | -29% ⚠ |
| lean_switch | 0.03002 | 0.06433 | +11% |
| lean_drop | 0.2667 | 0.2757 | +4% |
| aim_error_fight_deg | 6.024 | 11.96 | +242% ⚠ |
| reaction_ms | 165.9 | 250 | +84% ⚠ |
| mp40_share | 0.4858 | 1 | +51% ⚠ |

Style fingerprint (2026-10-04, 27.7 min, styles.py features 30/30): z-distance to the human cloud centre 11.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.229 (humans 0.358–0.767); lf_lean 0.383 (humans 0.457–0.948); ducked 0.122 (humans 0.0109–0.0741); attack 0.233 (humans 0.276–0.478); p_attack_given_los 0.527 (humans 0.547–0.861); lf_err_med 12 (humans 4.08–6.63); lf_height_frac 0.549 (humans 0.352–0.54); yaw_speed_p95 381 (humans 158–271); yaw_speed_p99 841 (humans 427–754); lf_retreat 0.162 (humans 0.0243–0.141).

### `bot:strafer:4.3` (strafer, seed 1086809828, 26.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1632 | 0.2328 | +14% |
| reverse_share | 0.4593 | 0.4474 | -2% |
| side_hold_ms | 314.9 | 300 | -10% |
| lean_fight | 0.5599 | 0.5 | -12% |
| jumps_per_min | 4.813 | 4.769 | -1% |
| crouch_per_min | 9.497 | 9.092 | -4% |
| walk_hidden | 0 | 0.02256 | +21% |
| burst_median | 5.59 | 6 | +10% |
| aim_height_firing | 0.4741 | 0.514 | +21% |
| hold_angle | 0.3025 | 0.1935 | -35% ⚠ |
| counter_strafe | 0.2249 | 0.2878 | +12% |
| lean_switch | 0.03088 | 0.07629 | +14% |
| lean_drop | 0.2667 | 0.2833 | +8% |
| aim_error_fight_deg | 6.527 | 12.97 | +262% ⚠ |
| reaction_ms | 158.4 | 250 | +92% ⚠ |
| mp40_share | 0.9804 | 1 | +2% |

Style fingerprint (2026-10-04, 26.8 min, styles.py features 30/30): z-distance to the human cloud centre 13.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 12.6 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.295 (humans 0.358–0.767); ducked 0.101 (humans 0.0109–0.0741); attack 0.237 (humans 0.276–0.478); lf_err_med 13 (humans 4.08–6.63); yaw_speed_p95 472 (humans 158–271); yaw_speed_p99 965 (humans 427–754); lf_retreat 0.182 (humans 0.0243–0.141).

### `bot:strafer:5.4` (strafer, seed 821368291, 26.8 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.166 | 0.2791 | +22% |
| reverse_share | 0.4801 | 0.4666 | -2% |
| side_hold_ms | 349.1 | 300 | -33% ⚠ |
| lean_fight | 0.6654 | 0.748 | +16% |
| jumps_per_min | 5.052 | 4.658 | -7% |
| crouch_per_min | 10.37 | 8.235 | -20% |
| walk_hidden | 0.0009382 | 0.01813 | +16% |
| burst_median | 5.755 | 6 | +6% |
| aim_height_firing | 0.4434 | 0.454 | +6% |
| hold_angle | 0.3191 | 0.1825 | -44% ⚠ |
| counter_strafe | 0.4917 | 0.5219 | +6% |
| lean_switch | 0.0024 | 0.00271 | +0% |
| lean_drop | 0.0475 | 0.05217 | +2% |
| aim_error_fight_deg | 4.261 | 10.67 | +261% ⚠ |
| reaction_ms | 175 | 300 | +125% ⚠ |
| mp40_share | 0.1202 | 0 | -12% |

Style fingerprint (2026-10-04, 26.8 min, styles.py features 30/30): z-distance to the human cloud centre 10.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: ducked 0.0888 (humans 0.0109–0.0741); walk 0.0111 (humans 0.0126–0.0881); attack 0.231 (humans 0.276–0.478); p_attack_given_los 0.523 (humans 0.547–0.861); still 0.0527 (humans 0.0832–0.227); lf_err_med 10.7 (humans 4.08–6.63); yaw_speed_p95 420 (humans 158–271); yaw_speed_p99 875 (humans 427–754); lf_retreat 0.163 (humans 0.0243–0.141).

## The owner against the bots (`pstN@vsbot`)

Kills of bots 0, deaths to bots 0, suicides 0. 

0 statistics of the owner against bots fall outside the human CI (owner column below, marked •).

## All statistics

Pooled bots are reweighted to the people's family mix of duel time; humans and the owner are pooled over their duel time. ▲ marks a tell. n = units (ticks, runs, shots, sightings, ...) over the blocks.

### Movement

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time: hidden, not firing | 46.6% [44.2%, 48.8%] | 49.1% [47.4%, 51.2%] | - | 500060 |
| Share of duel time: hidden, firing ▲ | 12.5% [11.6%, 13.4%] | 15.8% [14.9%, 17.0%] | - | 500060 |
| Share of duel time: LOS, not firing ▲ | 10.2% [9.4%, 11.2%] | 7.0% [5.5%, 8.4%] | - | 500060 |
| Share of duel time: LOS firefight | 19.9% [18.6%, 21.6%] | 17.6% [16.5%, 18.9%] | - | 500060 |
| Share of duel time: reloading | 10.7% [9.4%, 12.1%] | 10.5% [9.6%, 11.5%] | - | 500060 |
| Key chord pure strafe (hidden, not firing) | 30.1% [28.1%, 31.9%] | 26.0% [23.5%, 29.4%] | - | 246650 |
| Key chord fwd diag (hidden, not firing) | 26.7% [25.0%, 28.1%] | 29.9% [26.7%, 33.2%] | - | 246650 |
| Key chord forward (hidden, not firing) | 13.1% [12.1%, 14.3%] | 11.9% [10.9%, 12.8%] | - | 246650 |
| Key chord neutral (hidden, not firing) | 22.2% [20.2%, 24.6%] | 26.1% [23.1%, 28.6%] | - | 246650 |
| Key chord back any (hidden, not firing) | 7.9% [7.0%, 8.6%] | 6.1% [5.3%, 7.1%] | - | 246650 |
| Key chord pure strafe (hidden, firing) | 44.1% [40.7%, 47.4%] | 44.9% [41.6%, 48.0%] | - | 57819 |
| Key chord fwd diag (hidden, firing) | 28.4% [25.7%, 31.1%] | 28.6% [23.9%, 32.8%] | - | 57819 |
| Key chord forward (hidden, firing) ▲ | 9.2% [7.5%, 11.7%] | 6.6% [5.7%, 7.4%] | - | 57819 |
| Key chord neutral (hidden, firing) | 12.3% [10.8%, 14.5%] | 13.2% [11.2%, 15.7%] | - | 57819 |
| Key chord back any (hidden, firing) | 6.1% [4.5%, 7.1%] | 6.7% [5.9%, 7.9%] | - | 57819 |
| Key chord pure strafe (LOS, not firing) ▲ | 37.3% [35.3%, 39.4%] | 26.7% [20.8%, 34.2%] | - | 51571 |
| Key chord fwd diag (LOS, not firing) | 29.4% [26.7%, 32.7%] | 29.4% [23.0%, 38.1%] | - | 51571 |
| Key chord forward (LOS, not firing) | 8.7% [7.0%, 10.8%] | 7.5% [5.9%, 9.5%] | - | 51571 |
| Key chord neutral (LOS, not firing) | 15.0% [12.0%, 17.8%] | 31.5% [15.3%, 45.4%] | - | 51571 |
| Key chord back any (LOS, not firing) ▲ | 9.6% [8.6%, 10.5%] | 4.8% [3.4%, 6.7%] | - | 51571 |
| Key chord pure strafe (LOS firefight) | 57.0% [52.6%, 61.4%] | 58.0% [53.1%, 62.2%] | - | 95697 |
| Key chord fwd diag (LOS firefight) | 30.1% [25.9%, 35.7%] | 30.5% [25.6%, 36.7%] | - | 95697 |
| Key chord forward (LOS firefight) ▲ | 1.9% [1.5%, 2.5%] | 1.2% [1.0%, 1.4%] | - | 95697 |
| Key chord neutral (LOS firefight) | 5.4% [4.3%, 6.8%] | 5.1% [3.9%, 6.3%] | - | 95697 |
| Key chord back any (LOS firefight) | 5.6% [4.9%, 6.4%] | 5.2% [4.5%, 6.1%] | - | 95697 |
| Key chord pure strafe (reloading) ▲ | 29.9% [26.8%, 33.0%] | 22.3% [19.6%, 24.4%] | - | 48325 |
| Key chord fwd diag (reloading) ▲ | 30.7% [28.9%, 32.1%] | 36.8% [34.1%, 39.9%] | - | 48325 |
| Key chord forward (reloading) ▲ | 15.8% [11.8%, 21.4%] | 26.1% [23.8%, 28.6%] | - | 48325 |
| Key chord neutral (reloading) ▲ | 13.5% [11.8%, 15.1%] | 8.3% [7.3%, 9.5%] | - | 48325 |
| Key chord back any (reloading) | 10.1% [7.1%, 11.6%] | 6.5% [5.3%, 7.6%] | - | 48325 |
| Strafe end is a direct reverse (hidden, not firing) | 54.2% [50.1%, 57.3%] | 46.1% [41.1%, 51.2%] | - | 20473 |
| Strafe end is a direct reverse (hidden, firing) | 72.9% [66.1%, 77.2%] | 67.2% [59.7%, 74.3%] | - | 6538 |
| Strafe end is a direct reverse (LOS, not firing) | 59.9% [49.8%, 65.6%] | 52.8% [45.1%, 60.4%] | - | 5318 |
| Strafe end is a direct reverse (LOS firefight) | 76.6% [69.6%, 82.5%] | 73.3% [65.9%, 80.6%] | - | 15304 |
| Strafe end is a direct reverse (reloading) ▲ | 58.4% [53.8%, 62.3%] | 37.7% [32.5%, 42.7%] | - | 4597 |
| Strafe switch hazard per tick at hold age 50 ms (LOS firefight) ▲ | 7.8% [5.3%, 10.1%] | 1.2% [0.9%, 1.6%] | - | 16135 |
| Strafe switch hazard per tick at hold age 100 ms (LOS firefight) ▲ | 20.5% [16.6%, 23.6%] | 5.8% [5.2%, 6.8%] | - | 14383 |
| Strafe switch hazard per tick at hold age 150 ms (LOS firefight) ▲ | 23.4% [21.0%, 25.5%] | 12.4% [11.1%, 13.9%] | - | 11436 |
| Strafe switch hazard per tick at hold age 200 ms (LOS firefight) ▲ | 21.9% [21.3%, 22.7%] | 17.0% [15.4%, 18.7%] | - | 8833 |
| Strafe switch hazard per tick at hold age 250 ms (LOS firefight) ▲ | 22.1% [20.8%, 23.3%] | 17.4% [16.0%, 19.0%] | - | 6992 |
| Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) ▲ | 22.1% [21.3%, 22.6%] | 15.6% [14.7%, 16.9%] | - | 15839 |
| Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) ▲ | 20.5% [19.4%, 21.9%] | 16.1% [14.9%, 17.5%] | - | 10917 |
| Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) | 18.1% [14.2%, 22.8%] | 13.7% [12.0%, 15.6%] | - | 1599 |
| Side-hold duration (LOS firefight) p25 ▲ | 100 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | - | 14458 |
| Side-hold duration (LOS firefight) p50 ▲ | 200 ms [150 ms, 200 ms] | 300 ms [250 ms, 300 ms] | - | 14458 |
| Side-hold duration (LOS firefight) p75 ▲ | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | - | 14458 |
| Side-hold duration (LOS firefight) p90 ▲ | 500 ms [500 ms, 550 ms] | 700 ms [650 ms, 700 ms] | - | 14458 |
| Side-hold duration (all contexts) p50 ▲ | 200 ms [200 ms, 250 ms] | 300 ms [300 ms, 300 ms] | - | 50975 |
| Side-hold duration (all contexts) p90 ▲ | 600 ms [600 ms, 650 ms] | 800 ms [800 ms, 850 ms] | - | 50975 |
| Neutral gap between strafes (LOS firefight) p50 | 50 ms [50 ms, 50 ms] | 50 ms [50 ms, 50 ms] | - | 4883 |
| Neutral gap between strafes (LOS firefight) p90 | 100 ms [50 ms, 150 ms] | 100 ms [100 ms, 150 ms] | - | 4883 |
| Approaching >40 u/s in LOS firefight at distance (0, 96] | 36.1% [33.7%, 39.4%] | 37.6% [33.7%, 42.3%] | - | 9520 |
| Approaching >40 u/s in LOS firefight at distance (96, 160] | 34.6% [31.7%, 38.4%] | 36.4% [32.6%, 41.2%] | - | 14262 |
| Approaching >40 u/s in LOS firefight at distance (160, 224] | 33.4% [30.2%, 37.2%] | 35.4% [30.1%, 42.0%] | - | 17136 |
| Approaching >40 u/s in LOS firefight at distance (224, 288] | 36.2% [32.9%, 42.2%] | 38.2% [32.3%, 46.4%] | - | 14668 |
| Approaching >40 u/s in LOS firefight at distance (288, 384] | 37.8% [34.4%, 42.8%] | 39.4% [32.7%, 47.2%] | - | 18758 |
| Approaching >40 u/s in LOS firefight at distance (384, 512] | 38.7% [28.2%, 50.1%] | 41.8% [33.8%, 49.1%] | - | 13147 |
| Approaching >40 u/s in LOS firefight at distance (512, 768] | 35.8% [22.5%, 47.6%] | 33.1% [25.5%, 41.5%] | - | 7940 |
| Approaching >40 u/s in LOS firefight at distance (768, 1200] | 16.8% [7.7%, 27.1%] | 28.2% [17.6%, 42.7%] | - | 266 |
| Retreating >40 u/s in LOS firefight at distance (0, 96] | 23.8% [21.4%, 26.0%] | 22.2% [18.9%, 25.6%] | - | 9520 |
| Retreating >40 u/s in LOS firefight at distance (96, 160] ▲ | 18.9% [17.1%, 20.1%] | 10.5% [8.5%, 12.6%] | - | 14262 |
| Retreating >40 u/s in LOS firefight at distance (160, 224] ▲ | 14.2% [13.1%, 15.3%] | 6.9% [5.5%, 8.7%] | - | 17136 |
| Retreating >40 u/s in LOS firefight at distance (224, 288] ▲ | 8.8% [7.6%, 10.2%] | 5.3% [4.2%, 6.8%] | - | 14668 |
| Retreating >40 u/s in LOS firefight at distance (288, 384] | 8.2% [7.1%, 9.3%] | 5.9% [4.6%, 7.3%] | - | 18758 |
| Retreating >40 u/s in LOS firefight at distance (384, 512] | 6.6% [4.7%, 8.9%] | 4.8% [3.8%, 6.0%] | - | 13147 |
| Retreating >40 u/s in LOS firefight at distance (512, 768] ▲ | 7.7% [6.4%, 9.7%] | 4.0% [3.0%, 5.4%] | - | 7940 |
| Retreating >40 u/s in LOS firefight at distance (768, 1200] | 14.4% [0.0%, 26.9%] | 8.3% [1.5%, 15.5%] | - | 266 |
| Standing still (<5 u/s), all duel time ▲ | 9.7% [8.8%, 10.5%] | 15.7% [13.4%, 17.7%] | - | 500060 |
| Standing still (<5 u/s) (hidden, not firing) ▲ | 15.2% [13.6%, 16.8%] | 22.5% [19.5%, 25.1%] | - | 246650 |
| Standing still (<5 u/s) (hidden, firing) ▲ | 6.8% [6.2%, 7.5%] | 10.2% [9.1%, 11.7%] | - | 57819 |
| Standing still (<5 u/s) (LOS, not firing) ▲ | 7.6% [4.9%, 10.4%] | 29.6% [12.6%, 44.0%] | - | 51571 |
| Standing still (<5 u/s) (LOS firefight) | 1.6% [1.2%, 2.0%] | 2.3% [1.8%, 2.7%] | - | 95697 |
| Standing still (<5 u/s) (reloading) | 6.0% [5.6%, 6.5%] | 5.2% [4.3%, 6.2%] | - | 48325 |
| Lean held, all duel time ▲ | 75.5% [72.8%, 78.0%] | 59.7% [55.3%, 64.0%] | - | 500060 |
| Lean held in LOS firefights | 87.3% [85.7%, 89.0%] | 81.3% [76.6%, 85.8%] | - | 95697 |
| Jump presses per minute | 1.88/min [1.32/min, 2.41/min] | 1.87/min [1.41/min, 2.44/min] | - | 500060 |
| Crouch presses per minute | 4.59/min [3.48/min, 5.76/min] | 4.33/min [3.22/min, 5.51/min] | - | 500060 |
| Walk presses per minute | 6.17/min [5.70/min, 6.99/min] | 7.54/min [6.90/min, 8.11/min] | - | 500060 |
| Walking while hidden and not firing | 5.3% [4.0%, 6.6%] | 5.9% [5.0%, 7.0%] | - | 246650 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Press hazard with LOS and aim within 3 half-widths, LOS gained 0 ms ago | 25.2% [18.7%, 30.8%] | 28.2% [25.8%, 30.8%] | - | 1639 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 50-100 ms ago | 30.3% [23.8%, 39.1%] | 33.0% [30.8%, 35.6%] | - | 2411 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 150-250 ms ago | 32.2% [25.6%, 41.2%] | 35.2% [32.9%, 38.1%] | - | 2316 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago | 33.9% [29.0%, 44.0%] | 28.0% [25.2%, 32.0%] | - | 1865 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 550-1000 ms ago | 25.2% [17.4%, 32.9%] | 21.0% [16.7%, 24.6%] | - | 2084 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago ▲ | 20.0% [15.6%, 24.4%] | 7.4% [4.7%, 10.8%] | - | 3003 |
| Press hazard while hidden, LOS lost 0 ms ago | 9.2% [8.2%, 11.3%] | 12.3% [10.8%, 14.5%] | - | 3415 |
| Press hazard while hidden, LOS lost 50-100 ms ago | 9.9% [8.8%, 11.1%] | 9.5% [8.5%, 10.9%] | - | 4503 |
| Press hazard while hidden, LOS lost 150-250 ms ago ▲ | 7.9% [6.8%, 9.0%] | 4.6% [3.9%, 5.5%] | - | 5832 |
| Press hazard while hidden, LOS lost 300-500 ms ago | 8.3% [6.9%, 9.9%] | 7.0% [6.0%, 8.1%] | - | 17689 |
| Press hazard while hidden, LOS lost 550-1000 ms ago | 3.3% [2.8%, 3.8%] | 3.5% [3.2%, 3.9%] | - | 28398 |
| Press hazard while hidden, LOS lost over 1 s ago ▲ | 2.1% [1.8%, 2.3%] | 4.1% [3.5%, 4.6%] | - | 175910 |
| Release hazard, LOS, aim error in half-widths (0, 1] | 1.4% [1.2%, 1.7%] | 1.4% [1.1%, 1.7%] | - | 16993 |
| Release hazard, LOS, aim error in half-widths (1, 2] | 1.4% [1.2%, 1.6%] | 1.5% [1.3%, 1.9%] | - | 28329 |
| Release hazard, LOS, aim error in half-widths (2, 3] | 1.4% [1.1%, 1.7%] | 1.5% [1.2%, 2.0%] | - | 20977 |
| Release hazard, LOS, aim error in half-widths (3, 4] | 1.6% [1.2%, 2.0%] | 1.4% [1.0%, 2.0%] | - | 12054 |
| Release hazard, LOS, aim error in half-widths (4, 6] | 2.4% [1.9%, 2.9%] | 1.7% [1.3%, 2.3%] | - | 9780 |
| Release hazard, LOS, aim error in half-widths (6, 10] | 8.6% [6.5%, 10.9%] | 6.9% [4.4%, 9.9%] | - | 3588 |
| Release hazard, LOS, aim error in half-widths (10, 1000] ▲ | 19.7% [17.7%, 21.9%] | 32.7% [22.9%, 41.8%] | - | 2132 |
| Release hazard while hidden, LOS lost 0-100 ms ago | 5.2% [4.6%, 6.2%] | 6.3% [5.4%, 7.5%] | - | 13856 |
| Release hazard while hidden, LOS lost 150-250 ms ago | 11.6% [10.1%, 14.1%] | 16.5% [13.8%, 18.7%] | - | 7536 |
| Release hazard while hidden, LOS lost 300-450 ms ago | 27.2% [24.4%, 31.1%] | 31.5% [29.6%, 33.4%] | - | 7739 |
| Release hazard while hidden, LOS lost 500-950 ms ago | 12.3% [10.7%, 14.2%] | 10.1% [9.2%, 11.3%] | - | 6878 |
| Release hazard while hidden, LOS lost 1 s or more ago ▲ | 15.0% [14.4%, 15.7%] | 11.3% [9.8%, 13.5%] | - | 18628 |
| Fire held, LOS, aim error in half-widths (0, 0.5] | 83.0% [75.7%, 88.1%] | 89.6% [87.0%, 91.7%] | - | 5932 |
| Fire held, LOS, aim error in half-widths (0.5, 1] | 84.8% [79.5%, 88.8%] | 88.9% [86.9%, 90.5%] | - | 14325 |
| Fire held, LOS, aim error in half-widths (1, 1.5] | 85.0% [79.7%, 88.6%] | 86.3% [84.5%, 87.8%] | - | 17459 |
| Fire held, LOS, aim error in half-widths (1.5, 2] | 84.7% [82.0%, 88.1%] | 81.9% [79.8%, 83.7%] | - | 16712 |
| Fire held, LOS, aim error in half-widths (2, 3] | 82.8% [80.4%, 86.1%] | 81.0% [79.4%, 82.7%] | - | 26380 |
| Fire held, LOS, aim error in half-widths (3, 4] | 77.4% [73.3%, 82.0%] | 78.8% [76.1%, 81.2%] | - | 16187 |
| Fire held, LOS, aim error in half-widths (4, 6] | 66.0% [61.3%, 72.2%] | 67.7% [63.0%, 71.3%] | - | 15519 |
| Fire held, LOS, aim error in half-widths (6, 10] ▲ | 37.3% [32.5%, 42.0%] | 22.4% [17.1%, 27.1%] | - | 10201 |
| Fire held, LOS, aim error in half-widths (10, 20] ▲ | 12.8% [10.5%, 15.2%] | 4.6% [2.4%, 6.9%] | - | 8449 |
| Fire held, LOS, aim error in half-widths (20, 1000] ▲ | 8.7% [5.5%, 10.8%] | 0.9% [0.4%, 3.1%] | - | 15228 |
| Fire held without LOS | 21.1% [19.4%, 23.2%] | 24.5% [22.7%, 26.3%] | - | 290540 |
| Attack hold duration p50 | 200 ms [200 ms, 200 ms] | 150 ms [150 ms, 200 ms] | - | 12422 |
| Attack hold duration p90 | 950 ms [900 ms, 1000 ms] | 900 ms [800 ms, 950 ms] | - | 12422 |
| Attack holds that are taps (<=100 ms) ▲ | 38.8% [37.5%, 40.4%] | 44.3% [41.8%, 46.5%] | - | 12422 |
| Gap between attack holds p50 | 350 ms [300 ms, 350 ms] | 400 ms [350 ms, 400 ms] | - | 11850 |

### Aim

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error firing with LOS at (0, 128] u ▲ | 18.85° [17.18°, 20.03°] | 14.28° [13.53°, 15.19°] | - | 14111 |
| Crosshair on body firing with LOS at (0, 128] u ▲ | 40.1% [38.2%, 42.3%] | 48.0% [44.9%, 51.5%] | - | 14111 |
| Median aim error firing with LOS at (128, 192] u ▲ | 11.13° [10.65°, 11.75°] | 8.99° [8.66°, 9.45°] | - | 15648 |
| Crosshair on body firing with LOS at (128, 192] u ▲ | 33.8% [31.8%, 35.9%] | 42.5% [39.8%, 44.7%] | - | 15648 |
| Median aim error firing with LOS at (192, 256] u ▲ | 8.35° [7.98°, 8.66°] | 6.68° [6.36°, 7.10°] | - | 15365 |
| Crosshair on body firing with LOS at (192, 256] u ▲ | 31.5% [30.0%, 33.0%] | 42.0% [39.5%, 44.4%] | - | 15365 |
| Median aim error firing with LOS at (256, 384] u | 5.72° [5.46°, 5.88°] | 5.23° [4.96°, 5.59°] | - | 27505 |
| Crosshair on body firing with LOS at (256, 384] u ▲ | 32.4% [30.5%, 34.9%] | 39.0% [36.0%, 41.4%] | - | 27505 |
| Median aim error firing with LOS at (384, 512] u | 4.21° [3.93°, 4.45°] | 3.92° [3.67°, 4.18°] | - | 14040 |
| Crosshair on body firing with LOS at (384, 512] u | 30.1% [27.2%, 32.9%] | 35.4% [32.6%, 38.3%] | - | 14040 |
| Median aim error firing with LOS at (512, 768] u | 3.22° [3.06°, 3.44°] | 3.07° [2.80°, 3.35°] | - | 8762 |
| Crosshair on body firing with LOS at (512, 768] u | 28.2% [25.4%, 30.6%] | 32.0% [29.1%, 35.1%] | - | 8762 |
| Median aim error firing with LOS at (768, 1200] u | 3.13° [2.25°, 4.09°] | 2.14° [1.84°, 2.58°] | - | 266 |
| Crosshair on body firing with LOS at (768, 1200] u | 23.1% [11.3%, 35.8%] | 31.5% [24.3%, 39.5%] | - | 266 |
| Aim height (fraction of body) firing with LOS | 0.425 [0.408, 0.442] | 0.446 [0.418, 0.468] | - | 95697 |
| Aim height (fraction of body) with LOS, not firing | 0.613 [0.585, 0.649] | 0.662 [0.576, 0.751] | - | 72689 |

### View

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw speed (hidden, not firing) p50 ▲ | 12°/s [11°/s, 12°/s] | 8°/s [6°/s, 10°/s] | - | 244450 |
| Yaw speed (hidden, not firing) p99 ▲ | 845°/s [808°/s, 882°/s] | 544°/s [521°/s, 580°/s] | - | 244450 |
| Mouse still between ticks (hidden, not firing) | 30.1% [29.0%, 31.1%] | 32.1% [28.8%, 36.0%] | - | 246650 |
| Yaw speed (hidden, firing) p50 ▲ | 34°/s [32°/s, 35°/s] | 23°/s [21°/s, 25°/s] | - | 57819 |
| Yaw speed (hidden, firing) p99 ▲ | 584°/s [524°/s, 633°/s] | 194°/s [182°/s, 204°/s] | - | 57819 |
| Mouse still between ticks (hidden, firing) ▲ | 7.9% [7.5%, 8.4%] | 10.9% [10.1%, 11.8%] | - | 57819 |
| Yaw speed (LOS, not firing) p50 ▲ | 35°/s [29°/s, 43°/s] | 11°/s [0°/s, 26°/s] | - | 51563 |
| Yaw speed (LOS, not firing) p99 ▲ | 925°/s [859°/s, 977°/s] | 583°/s [505°/s, 695°/s] | - | 51563 |
| Mouse still between ticks (LOS, not firing) | 18.7% [16.1%, 21.2%] | 36.7% [20.8%, 49.8%] | - | 51571 |
| Yaw speed (LOS firefight) p50 ▲ | 54°/s [51°/s, 58°/s] | 39°/s [36°/s, 41°/s] | - | 95697 |
| Yaw speed (LOS firefight) p99 ▲ | 678°/s [624°/s, 749°/s] | 305°/s [292°/s, 318°/s] | - | 95697 |
| Mouse still between ticks (LOS firefight) ▲ | 3.0% [2.9%, 3.2%] | 1.6% [1.4%, 1.8%] | - | 95697 |
| Yaw speed (reloading) p50 ▲ | 30°/s [27°/s, 33°/s] | 18°/s [16°/s, 20°/s] | - | 48325 |
| Yaw speed (reloading) p99 ▲ | 820°/s [770°/s, 853°/s] | 945°/s [909°/s, 991°/s] | - | 48325 |
| Mouse still between ticks (reloading) ▲ | 20.2% [18.7%, 22.0%] | 26.3% [25.0%, 27.6%] | - | 48325 |
| Peak yaw speed of 90-135 deg turns p50 | 640°/s [631°/s, 655°/s] | 619°/s [595°/s, 655°/s] | - | 3835 |
| Peak yaw speed of 135-180 deg turns p50 | 872°/s [856°/s, 886°/s] | 852°/s [828°/s, 887°/s] | - | 3210 |
| Peak yaw speed of 180-360 deg turns p50 ▲ | 1183°/s [1131°/s, 1200°/s] | 1061°/s [1020°/s, 1126°/s] | - | 623 |

### Belief

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw error to the hidden opponent, last seen <=250 ms ▲ | 6.69° [6.08°, 7.40°] | 4.92° [4.56°, 5.26°] | - | 36217 |
| View within 30 deg of the hidden opponent, last seen <=250 ms ▲ | 81.0% [79.0%, 84.1%] | 92.7% [91.9%, 93.7%] | - | 36217 |
| Yaw error to the hidden opponent, last seen 250-500 ms | 6.89° [6.35°, 7.58°] | 6.66° [5.99°, 7.28°] | - | 19114 |
| View within 30 deg of the hidden opponent, last seen 250-500 ms ▲ | 86.7% [85.5%, 88.0%] | 92.2% [91.3%, 93.4%] | - | 19114 |
| Yaw error to the hidden opponent, last seen 0.5-1 s | 9.00° [7.76°, 10.38°] | 8.28° [7.48°, 9.11°] | - | 19659 |
| View within 30 deg of the hidden opponent, last seen 0.5-1 s ▲ | 83.4% [81.8%, 85.5%] | 88.0% [86.8%, 89.9%] | - | 19659 |
| Yaw error to the hidden opponent, last seen 1-2 s ▲ | 14.34° [12.24°, 16.75°] | 11.11° [9.95°, 12.13°] | - | 12847 |
| View within 30 deg of the hidden opponent, last seen 1-2 s ▲ | 68.5% [65.6%, 71.8%] | 78.9% [77.1%, 81.1%] | - | 12847 |
| Yaw error to the hidden opponent, last seen 2-4 s ▲ | 24.77° [19.97°, 29.82°] | 16.93° [15.18°, 19.08°] | - | 44942 |
| View within 30 deg of the hidden opponent, last seen 2-4 s ▲ | 54.3% [50.1%, 59.2%] | 64.3% [61.3%, 67.3%] | - | 44942 |
| Yaw error to the hidden opponent, last seen 4-8 s ▲ | 24.82° [22.00°, 27.74°] | 10.29° [9.19°, 11.80°] | - | 58855 |
| View within 30 deg of the hidden opponent, last seen 4-8 s ▲ | 54.2% [51.6%, 57.1%] | 76.4% [73.1%, 79.3%] | - | 58855 |
| Yaw error to the hidden opponent, last seen >8 s | 29.59° [24.26°, 32.76°] | 20.07° [12.82°, 29.16°] | - | 22945 |
| View within 30 deg of the hidden opponent, last seen >8 s | 50.5% [47.2%, 56.4%] | 65.1% [54.8%, 74.8%] | - | 22945 |
| Yaw error to the hidden opponent, last seen never seen this life | 25.95° [20.87°, 31.84°] | 19.23° [16.93°, 24.71°] | - | 117090 |
| View within 30 deg of the hidden opponent, last seen never seen this life | 54.3% [48.1%, 60.5%] | 62.5% [56.0%, 67.2%] | - | 117090 |

### Space

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) ▲ | 38.1% [32.7%, 40.0%] | 49.4% [44.0%, 50.8%] | - | 118890 |
| Share of duel time in the busiest 10% of 32u cells (dm/main) | 41.1% [30.9%, 41.4%] | 45.2% [40.5%, 48.6%] | - | 122990 |
| Share of duel time in the busiest 10% of 32u cells (dm/vents) | 41.8% [37.5%, 43.1%] | 44.3% [39.2%, 46.8%] | - | 127330 |
| Share of duel time in the busiest 10% of 32u cells (dm/downladder) ▲ | 36.8% [32.4%, 38.0%] | 61.4% [42.9%, 63.8%] | - | 130840 |

### Acquisition

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error at -500 ms from sight gain ▲ | 17.69° [15.72°, 19.28°] | 13.98° [12.90°, 15.10°] | - | 5193 |
| Median aim error at -200 ms from sight gain ▲ | 16.12° [14.37°, 18.01°] | 8.33° [7.70°, 8.92°] | - | 5203 |
| Median aim error at +0 ms from sight gain ▲ | 16.07° [13.96°, 17.96°] | 5.22° [4.88°, 5.61°] | - | 5208 |
| Median aim error at +100 ms from sight gain ▲ | 14.01° [11.65°, 15.89°] | 5.97° [5.54°, 6.43°] | - | 5208 |
| Median aim error at +200 ms from sight gain ▲ | 10.63° [9.04°, 11.82°] | 4.92° [4.54°, 5.23°] | - | 5208 |
| Median aim error at +300 ms from sight gain ▲ | 8.68° [7.82°, 9.61°] | 4.53° [4.12°, 4.88°] | - | 5156 |
| Median aim error at +500 ms from sight gain ▲ | 7.54° [6.62°, 8.74°] | 5.47° [5.08°, 5.88°] | - | 4816 |
| Median aim error at +1000 ms from sight gain | 7.76° [6.96°, 8.59°] | 7.25° [6.72°, 7.70°] | - | 3976 |
| Fire held at -500 ms from sight gain ▲ | 22.3% [20.7%, 23.7%] | 18.2% [16.5%, 20.2%] | - | 5208 |
| Fire held at -200 ms from sight gain | 19.4% [17.9%, 21.4%] | 21.7% [19.7%, 23.9%] | - | 5208 |
| Fire held at +0 ms from sight gain ▲ | 24.8% [22.8%, 27.6%] | 42.5% [39.1%, 46.0%] | - | 5208 |
| Fire held at +100 ms from sight gain ▲ | 35.3% [31.9%, 39.8%] | 61.4% [57.8%, 64.6%] | - | 5208 |
| Fire held at +200 ms from sight gain ▲ | 46.1% [41.9%, 51.0%] | 77.0% [73.9%, 79.5%] | - | 5208 |
| Fire held at +400 ms from sight gain ▲ | 63.2% [59.3%, 67.0%] | 87.3% [85.5%, 89.0%] | - | 5103 |
| Fire held at +700 ms from sight gain ▲ | 64.2% [61.5%, 66.8%] | 77.0% [74.3%, 79.7%] | - | 4861 |
| Fire held at +1000 ms from sight gain | 60.8% [57.8%, 63.1%] | 63.4% [60.9%, 66.4%] | - | 4597 |
| Fire held at +1450 ms from sight gain | 54.1% [51.7%, 57.2%] | 53.6% [49.3%, 58.5%] | - | 4206 |
| Reaction: first trigger press after a clean sighting p25 | 150 ms [100 ms, 150 ms] | 50 ms [50 ms, 100 ms] | - | 2763 |
| Reaction: first trigger press after a clean sighting p50 | 250 ms [200 ms, 250 ms] | 150 ms [150 ms, 200 ms] | - | 2763 |
| Reaction: first trigger press after a clean sighting p75 ▲ | 400 ms [400 ms, 450 ms] | 300 ms [250 ms, 300 ms] | - | 2763 |
| Reaction: first trigger press after a clean sighting p90 ▲ | 700 ms [650 ms, 750 ms] | 400 ms [400 ms, 450 ms] | - | 2763 |
| Crosshair within half-width+1.5 deg after a clean sighting p50 ▲ | 300 ms [300 ms, 300 ms] | 200 ms [200 ms, 250 ms] | - | 2579 |
| Crosshair already on the body at the first visible tick ▲ | 10.6% [9.3%, 11.8%] | 22.6% [20.9%, 24.4%] | - | 5208 |
| Fire already held on the tick before sight (prefire) ▲ | 21.6% [20.2%, 23.5%] | 35.4% [32.2%, 38.6%] | - | 5208 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shots per burst (consecutive 100 ms shots) p50 | 5 [4, 5] | 5 [4, 5] | - | 11804 |
| Shots per burst (consecutive 100 ms shots) p90 ▲ | 15 [14, 17] | 12 [11, 13] | - | 11804 |

### Combat

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shot accuracy (eligible SMG shots that hit) ▲ | 15.4% [14.4%, 16.5%] | 19.2% [18.1%, 20.3%] | - | 74148 |

### Lives

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Respawn delay after death p50 ▲ | 3000 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | - | 2199 |
| Life length (lives ending in death) p50 ▲ | 11.1 s [10.2 s, 12.0 s] | 7.1 s [5.8 s, 8.2 s] | - | 2203 |

### Reload

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Reload within 3 s of a kill, rounds left 0 | 100.0% [100.0%, 100.0%] | 95.5% [86.4%, 100.0%] | - | 51 |
| Reload within 3 s of a kill, rounds left 1-3 | 97.7% [96.1%, 99.4%] | 96.4% [92.6%, 99.2%] | - | 118 |
| Reload within 3 s of a kill, rounds left 4-6 | 100.0% [100.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 145 |
| Reload within 3 s of a kill, rounds left 7-10 | 99.7% [99.0%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 246 |
| Reload within 3 s of a kill, rounds left 11-15 | 97.9% [96.6%, 98.9%] | 98.8% [97.9%, 99.5%] | - | 406 |
| Reload within 3 s of a kill, rounds left 16-20 | 96.7% [95.4%, 98.2%] | 95.7% [93.5%, 97.7%] | - | 477 |
| Reload within 3 s of a kill, rounds left 21-25 | 86.4% [82.8%, 90.9%] | 83.1% [76.5%, 88.5%] | - | 435 |
| Reload within 3 s of a kill, rounds left 26-29 | 69.7% [65.1%, 73.5%] | 63.4% [52.8%, 75.0%] | - | 279 |
| Reload within 3 s of a kill, rounds left 30-32 | 73.5% [61.3%, 90.2%] | 73.7% [50.0%, 92.3%] | - | 45 |
| Delay from kill to reload (reloads within 3 s) p50 | 650 ms [650 ms, 650 ms] | 600 ms [600 ms, 650 ms] | - | 2018 |
| Reloads started with the opponent dead ▲ | 69.2% [65.5%, 74.2%] | 88.2% [85.5%, 90.4%] | - | 2802 |
| Reloads with the opponent alive that are forced (empty clip) ▲ | 89.3% [86.4%, 92.3%] | 77.7% [69.5%, 83.9%] | - | 782 |

### Fights

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Engagements that end in a kill ▲ | 90.0% [88.8%, 91.3%] | 79.9% [77.9%, 81.9%] | - | 4952 |
| First hitter wins (decisive engagements) | 63.0% [61.5%, 64.5%] | 65.4% [63.7%, 66.7%] | - | 4392 |
| First shooter wins (decisive engagements) | 51.8% [49.9%, 53.4%] | 55.0% [52.5%, 56.6%] | - | 4392 |
| Duration of decisive engagements p50 ▲ | 2000 ms [1850 ms, 2100 ms] | 1250 ms [1150 ms, 1351 ms] | - | 4392 |
| Engagement start distance p50 | 410 u [367 u, 447 u] | 427 u [398 u, 470 u] | - | 4952 |
| Decisive engagements won (subject's own) | 49.2% [46.3%, 52.1%] | 50.0% [45.6%, 54.2%] | - | 4392 |

### Perception

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time with a body part on screen and the centroid hidden ▲ | 8.0% [5.2%, 9.5%] | 10.5% [9.6%, 11.8%] | - | 500060 |
| Share of duel time with a body part of the enemy on screen | 37.4% [34.9%, 40.0%] | 34.0% [31.6%, 36.5%] | - | 500060 |
| Sightings (a body part coming on screen) per minute of duel time ▲ | 30.15/min [29.13/min, 31.26/min] | 33.29/min [31.94/min, 34.72/min] | - | 500060 |
| Spell with no body part on screen between two sightings p50 | 250 ms [200 ms, 400 ms] | 400 ms [300 ms, 500 ms] | - | 7404 |
| Spell with no body part on screen between two sightings p75 | 700 ms [600 ms, 850 ms] | 900 ms [800 ms, 1000 ms] | - | 7404 |
| Spell with no body part on screen between two sightings p90 | 1500 ms [1250 ms, 1750 ms] | 1750 ms [1600 ms, 1950 ms] | - | 7404 |
| Time from both alive to the first body part on screen p50 | 1450 ms [1300 ms, 1550 ms] | 1350 ms [1300 ms, 1400 ms] | - | 4414 |
| Time from both alive to the first body part on screen p75 ▲ | 2700 ms [2500 ms, 2913 ms] | 1900 ms [1800 ms, 2000 ms] | - | 4414 |
| Net distance covered in 2 s from a tick with no body part on screen p50 ▲ | 120 u [111 u, 132 u] | 158 u [146 u, 173 u] | - | 26564 |
| Net distance covered in 2 s from a tick with no body part on screen p25 | 60 u [54 u, 68 u] | 66 u [54 u, 78 u] | - | 26564 |
| Fire held with no body part of the enemy visible (loaded, not reloading) | 15.2% [14.0%, 16.4%] | 17.3% [15.8%, 19.2%] | - | 289350 |
| Fire held with no body part visible, 0-500ms after one was last on screen | 47.1% [45.6%, 48.7%] | 43.2% [40.9%, 45.8%] | - | 39644 |
| Fire held with no body part visible, 500-1000ms after one was last on screen | 23.4% [22.0%, 25.1%] | 26.4% [23.9%, 28.7%] | - | 19056 |
| Fire held with no body part visible, 1000-2000ms after one was last on screen ▲ | 15.0% [13.9%, 16.5%] | 20.5% [18.0%, 22.9%] | - | 17181 |
| Fire held with no body part visible, 2000-5000ms after one was last on screen ▲ | 11.0% [9.2%, 13.1%] | 7.7% [6.4%, 9.1%] | - | 68281 |
| Fire held with no body part visible, gt5000ms after one was last on screen | 6.3% [5.5%, 7.5%] | 4.3% [3.1%, 6.0%] | - | 63421 |
| Attack holds begun loaded that are taps (<=100 ms), with a body part visible ▲ | 12.2% [11.1%, 13.1%] | 15.4% [13.1%, 17.6%] | - | 4354 |
| Attack holds begun loaded that are taps (<=100 ms), with no body part visible | 41.5% [39.0%, 44.0%] | 45.9% [42.2%, 48.9%] | - | 6008 |
| Shots per burst whose first round left with a body part visible p50 | 6 [6, 6] | 6 [5, 6] | - | 7573 |
| Shots per burst whose first round left with a body part visible p90 ▲ | 18 [16, 19] | 13 [12, 14] | - | 7573 |
| SMG shots that hit, a part on screen, centroid hidden ▲ | 11.6% [10.8%, 12.7%] | 21.7% [18.9%, 24.3%] | - | 9094 |
| SMG shots that hit, centroid visible ▲ | 21.6% [20.4%, 22.9%] | 27.2% [25.8%, 28.5%] | - | 47669 |
| Centroid sightings with a body part on screen first ▲ | 36.0% [33.5%, 39.0%] | 85.9% [84.2%, 87.7%] | - | 5193 |
| Lead of the first part over the centroid p50 ▲ | 50 ms [50 ms, 50 ms] | 100 ms [100 ms, 100 ms] | - | 1748 |
| Median aim error at -500 ms from the first visible part | 15.78° [14.42°, 16.93°] | 15.23° [14.18°, 16.19°] | - | 2820 |
| Median aim error at -200 ms from the first visible part ▲ | 13.11° [11.98°, 14.17°] | 10.54° [9.80°, 11.23°] | - | 2820 |
| Median aim error at -100 ms from the first visible part ▲ | 12.55° [11.39°, 13.52°] | 8.00° [7.34°, 8.49°] | - | 2820 |
| Median aim error at +0 ms from the first visible part ▲ | 12.22° [10.99°, 13.30°] | 5.22° [4.91°, 5.53°] | - | 2820 |
| Median aim error at +100 ms from the first visible part ▲ | 10.48° [9.23°, 11.48°] | 4.87° [4.54°, 5.18°] | - | 2820 |
| Median aim error at +200 ms from the first visible part ▲ | 7.01° [6.40°, 7.64°] | 5.26° [4.86°, 5.58°] | - | 2820 |
| Fire already held on the tick before the first part (prefire) ▲ | 17.3% [14.9%, 19.1%] | 23.0% [20.3%, 25.5%] | - | 2820 |
| Reaction: first press after a clean sighting, from the first part p50 | 250 ms [200 ms, 250 ms] | 200 ms [200 ms, 250 ms] | - | 1714 |
| Reaction: mean first press after a clean sighting, from the first part | 247 ms [220 ms, 275 ms] | 232 ms [217 ms, 250 ms] | - | 1714 |
| Crosshair within half-width+1.5 deg after a clean sighting, from the first part p50 | 250 ms [250 ms, 300 ms] | 250 ms [200 ms, 250 ms] | - | 1662 |
| First hit after the first part p50 ▲ | 350 ms [350 ms, 400 ms] | 250 ms [200 ms, 250 ms] | - | 1823 |
| Part sightings whose visible bout ends without a hit ▲ | 36.2% [33.1%, 39.5%] | 27.5% [25.7%, 29.7%] | - | 2820 |
| Yaw error to the hidden enemy 500 ms before the first part | 13.70° [12.48°, 15.20°] | 14.63° [13.70°, 15.63°] | - | 2820 |
| Yaw error to where the enemy will appear, 500 ms before the first part | 13.27° [11.78°, 14.44°] | 10.65° [9.39°, 11.89°] | - | 2820 |
| Crosshair parked within 5 deg of where the enemy will appear, 500 ms before | 26.3% [23.5%, 29.5%] | 29.4% [27.1%, 32.4%] | - | 2820 |
| Crosshair closer to where the enemy will appear than to the enemy, -500 ms ▲ | 52.3% [47.7%, 55.9%] | 67.0% [65.6%, 68.8%] | - | 2820 |
| View turn toward the enemy over the last 500 ms before the first part ▲ | 7.72° [6.20°, 8.98°] | 13.01° [11.44°, 14.55°] | - | 2820 |
| View turn over the last 500 ms before the first part ▲ | 10.43° [8.79°, 11.33°] | 14.53° [13.34°, 15.90°] | - | 2820 |
| Speed 400-200 ms before the first part ▲ | 165 u/s [160 u/s, 169 u/s] | 188 u/s [182 u/s, 193 u/s] | - | 2820 |
| Speed 2-1 s before the first part | 153 u/s [149 u/s, 160 u/s] | 167 u/s [159 u/s, 176 u/s] | - | 1602 |
| Hidden, no sighting next: crosshair closer to where the enemy will be in 500 ms | 48.9% [48.2%, 49.6%] | 50.5% [49.0%, 51.6%] | - | 212700 |
| Yaw error to the corner, 500 ms before the first part ▲ | 11.54° [9.97°, 13.86°] | 5.81° [5.17°, 6.44°] | - | 1948 |
| Angle to the corner, 500 ms before the first part ▲ | 13.51° [11.95°, 14.73°] | 8.41° [7.70°, 9.19°] | - | 1948 |
| Pitch to the corner (- = corner above the crosshair), 500 ms before the first part | -2.15° [-2.45°, -1.68°] | -2.72° [-3.14°, -2.39°] | - | 1948 |
| Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part ▲ | 0.55° [-0.08°, 1.25°] | -1.77° [-2.04°, -1.43°] | - | 1948 |
| Yaw error to the appearance point (sightings with a corner), 500 ms before the first part | 12.33° [10.38°, 13.50°] | 10.34° [9.19°, 11.29°] | - | 1948 |
| Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part | 13.48° [11.78°, 15.29°] | 15.18° [14.24°, 16.01°] | - | 1948 |
| Crosshair within 5 deg of the corner, 500 ms before the first part ▲ | 17.4% [14.8%, 20.1%] | 29.2% [26.4%, 32.7%] | - | 1948 |
| Crosshair closer to the corner than to the enemy, 500 ms before the first part ▲ | 55.6% [49.9%, 59.8%] | 80.0% [78.8%, 81.1%] | - | 1948 |
| Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part ▲ | 42.4% [39.4%, 44.6%] | 48.2% [45.7%, 50.3%] | - | 1948 |
| Crosshair within 2 deg of the corner's edge, 500 ms before the first part ▲ | 12.5% [10.1%, 15.3%] | 23.7% [21.7%, 25.7%] | - | 1948 |
| Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part ▲ | 45.1% [43.4%, 47.3%] | 28.1% [26.8%, 29.4%] | - | 1948 |
| Angle to the corner at the first part ▲ | 11.15° [9.85°, 12.27°] | 5.74° [5.33°, 6.17°] | - | 1948 |
| Distance from the eye to the corner p50 | 230 u [192 u, 270 u] | 228 u [204 u, 248 u] | - | 1948 |
| Yaw error to the corner at -2000 ms (sightings hidden >= 2 s) | 22.16° [15.49°, 30.82°] | 14.66° [11.85°, 17.22°] | - | 566 |
| Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) ▲ | 20.89° [15.89°, 27.52°] | 5.78° [4.88°, 6.98°] | - | 566 |
| Yaw error to the corner at -500 ms (sightings hidden >= 2 s) ▲ | 15.37° [13.11°, 17.64°] | 4.66° [3.99°, 5.31°] | - | 566 |
| Yaw error to the corner at +0 ms (sightings hidden >= 2 s) ▲ | 12.58° [10.61°, 13.79°] | 3.54° [3.01°, 4.10°] | - | 566 |

Consistency: 112 pooled statistics (bots and owner together) were recomputed from the analysis scripts' own JSON; 0 differ.

## Notes

- perception statistics on the bots' logged body-part column (ext_vis_parts: their own perception); the people's are on the data repo's rebuilt column, accepted against the logged one on 2026-09-28
- 100% of the bots' duel time is against another bot (the logged opponent is the nearest living enemy); the human reference is humans against humans only
- engagements.py counts contact episodes only while exactly two players are alive; with more than two players in a session the fight statistics cover the phases where one player is dead
