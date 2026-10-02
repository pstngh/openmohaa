# Bot evaluation: 2026-10-02_baseline_realmaps

Generated 2026-10-02 by `humanbot/eval/compare.py`. Captures: `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`, `telemetry`. 8 sessions on dm/crnodoors, dm/downladder, dm/main, dm/vents; capture dates 2026-10-02.
Human reference: `human_reference.json` (2026-10-02), 397 duel minutes, 72 player-sessions, 5 people.

## Bots in this capture

| Bot | Family | Seed | Sessions | Duel minutes |
|---|---|---:|---:|---:|
| `bot:presser:4` | presser | 98799799 | 1 | 31.9 |
| `bot:strafer:5` | strafer | 3439481337 | 1 | 31.9 |
| `bot:stopper:4` | stopper | 4094642640 | 1 | 33.3 |
| `bot:presser:5` | presser | 3362912357 | 1 | 33.3 |
| `bot:strafer:4` | strafer | 3622030457 | 1 | 35.2 |
| `bot:stopper:5` | stopper | 786231007 | 1 | 35.2 |
| `bot:presser:4.2` | presser | 456908850 | 1 | 34.1 |
| `bot:presser:5.2` | presser | 4270387778 | 1 | 34.1 |
| `bot:presser:4.3` | presser | 2606960606 | 1 | 39.5 |
| `bot:strafer:5.2` | strafer | 159436562 | 1 | 39.5 |
| `bot:presser:4.4` | presser | 1810870783 | 1 | 39.5 |
| `bot:strafer:5.3` | strafer | 1227344170 | 1 | 39.5 |
| `bot:presser:4.5` | presser | 880916664 | 1 | 35.9 |
| `bot:strafer:5.4` | strafer | 1562960504 | 1 | 35.9 |
| `bot:strafer:4.2` | strafer | 640050134 | 1 | 35.6 |
| `bot:strafer:5.5` | strafer | 1110590673 | 1 | 35.6 |

The owner (`pstN@vsbot`) has 0.0 duel minutes against the bots; those rows are never pooled with the bots.

Bot duel time against the owner 0%, against other bots 100%.

Family mix of bot duel time (observed → reweighted to the styles.json targets): presser 44% → 50%, stopper 12% → 17%, strafer 44% → 33%.

## Tells (158)

Statistics whose 95% CIs for the pooled bots and the pooled humans do not overlap, ranked by the standardized gap z = (bots − humans) / √(SE²bots + SE²humans). Only statistics with enough data on both sides are ranked.

| # | Statistic | Bots | Humans | z |
|---:|---|---|---|---:|
| 1 | Centroid sightings with a body part on screen first | 38.3% [35.8%, 40.3%] | 85.9% [84.2%, 87.7%] | -31.1 |
| 2 | Side-hold duration (all contexts) p50 | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | -28.3 |
| 3 | Reloads started with the opponent dead | 24.2% [20.3%, 27.9%] | 88.2% [85.5%, 90.4%] | -26.2 |
| 4 | Crosshair closer to the corner than to the enemy, 500 ms before the first part | 35.1% [30.7%, 39.4%] | 80.0% [78.8%, 81.1%] | -19.7 |
| 5 | Shot accuracy (eligible SMG shots that hit) | 5.3% [4.5%, 6.0%] | 19.2% [18.1%, 20.3%] | -19.4 |
| 6 | Respawn delay after death p50 | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | +17.2 |
| 7 | Fire held at +400 ms from sight gain | 57.4% [54.0%, 60.5%] | 87.3% [85.5%, 89.0%] | -15.5 |
| 8 | Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part | 60.9% [57.4%, 64.8%] | 28.1% [26.8%, 29.4%] | +15.2 |
| 9 | Attack holds that are taps (<=100 ms) | 21.8% [19.3%, 23.8%] | 44.3% [41.8%, 46.5%] | -13.4 |
| 10 | Crosshair closer to where the enemy will appear than to the enemy, -500 ms | 39.4% [35.8%, 43.1%] | 67.0% [65.6%, 68.8%] | -13.2 |
| 11 | Fire held at +200 ms from sight gain | 49.3% [45.9%, 52.0%] | 77.0% [73.9%, 79.5%] | -13.0 |
| 12 | Share of duel time with a body part on screen and the centroid hidden | 2.5% [1.9%, 3.2%] | 10.5% [9.6%, 11.8%] | -12.1 |
| 13 | Walk presses per minute | 1.97/min [1.37/min, 2.61/min] | 7.54/min [6.90/min, 8.11/min] | -12.0 |
| 14 | Median aim error firing with LOS at (0, 128] u | 21.13° [20.05°, 21.81°] | 14.28° [13.53°, 15.19°] | +11.8 |
| 15 | View turn toward the enemy over the last 500 ms before the first part | 2.66° [1.81°, 4.02°] | 13.01° [11.44°, 14.55°] | -10.6 |
| 16 | Key chord fwd diag (hidden, not firing) | 8.7% [6.3%, 11.5%] | 29.9% [26.7%, 33.2%] | -10.4 |
| 17 | Yaw speed (LOS firefight) p99 | 878°/s [761°/s, 972°/s] | 305°/s [292°/s, 318°/s] | +10.4 |
| 18 | Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) | 27.5% [25.6%, 29.2%] | 15.6% [14.7%, 16.9%] | +10.4 |
| 19 | Fire held at +700 ms from sight gain | 55.5% [52.2%, 57.8%] | 77.0% [74.3%, 79.7%] | -10.4 |
| 20 | Side-hold duration (all contexts) p90 | 450 ms [450 ms, 500 ms] | 800 ms [800 ms, 850 ms] | -10.3 |
| 21 | Side-hold duration (LOS firefight) p90 | 450 ms [450 ms, 500 ms] | 700 ms [650 ms, 700 ms] | -10.3 |
| 22 | Yaw speed (reloading) p99 | 606°/s [559°/s, 657°/s] | 945°/s [909°/s, 991°/s] | -10.1 |
| 23 | Fire held at -500 ms from sight gain | 35.7% [32.8%, 38.1%] | 18.2% [16.5%, 20.2%] | +10.0 |
| 24 | Speed 400-200 ms before the first part | 129 u/s [120 u/s, 140 u/s] | 188 u/s [182 u/s, 193 u/s] | -9.7 |
| 25 | Crosshair already on the body at the first visible tick | 11.9% [10.7%, 13.3%] | 22.6% [20.9%, 24.4%] | -9.4 |
| 26 | Part sightings whose visible bout ends without a hit | 53.3% [49.0%, 57.9%] | 27.5% [25.7%, 29.7%] | +9.4 |
| 27 | Engagements that end in a kill | 56.7% [51.7%, 60.5%] | 79.9% [77.9%, 81.9%] | -9.3 |
| 28 | Retreating >40 u/s in LOS firefight at distance (160, 224] | 19.1% [17.4%, 21.5%] | 6.9% [5.5%, 8.7%] | +8.9 |
| 29 | Fire held, LOS, aim error in half-widths (6, 10] | 60.3% [53.9%, 65.8%] | 22.4% [17.1%, 27.1%] | +8.9 |
| 30 | Release hazard while hidden, LOS lost 300-450 ms ago | 17.5% [14.8%, 19.8%] | 31.5% [29.6%, 33.4%] | -8.7 |
| 31 | Median aim error firing with LOS at (128, 192] u | 12.81° [12.01°, 13.49°] | 8.99° [8.66°, 9.45°] | +8.5 |
| 32 | Reaction: first trigger press after a clean sighting p90 | 850 ms [800 ms, 1000 ms] | 400 ms [400 ms, 450 ms] | +8.5 |
| 33 | Fire already held on the tick before the first part (prefire) | 38.0% [35.4%, 40.0%] | 23.0% [20.3%, 25.5%] | +8.4 |
| 34 | Key chord forward (hidden, not firing) | 5.9% [4.8%, 6.8%] | 11.9% [10.9%, 12.8%] | -8.4 |
| 35 | Crosshair within 2 deg of the corner's edge, 500 ms before the first part | 8.6% [5.7%, 11.2%] | 23.7% [21.7%, 25.7%] | -8.3 |
| 36 | Speed 2-1 s before the first part | 121 u/s [114 u/s, 129 u/s] | 167 u/s [159 u/s, 176 u/s] | -8.3 |
| 37 | Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part | 9.17° [8.31°, 10.37°] | 15.18° [14.24°, 16.01°] | -8.3 |
| 38 | Key chord forward (reloading) | 14.2% [12.4%, 15.4%] | 26.1% [23.8%, 28.6%] | -8.3 |
| 39 | Side-hold duration (LOS firefight) p75 | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | -8.3 |
| 40 | SMG shots that hit, a part on screen, centroid hidden | 8.8% [7.3%, 11.0%] | 21.7% [18.9%, 24.3%] | -8.2 |
| 41 | Key chord neutral (reloading) | 27.2% [21.4%, 30.8%] | 8.3% [7.3%, 9.5%] | +8.2 |
| 42 | Yaw error to the corner at -500 ms (sightings hidden >= 2 s) | 17.67° [14.94°, 20.66°] | 4.66° [3.99°, 5.31°] | +8.1 |
| 43 | Fire held at +100 ms from sight gain | 42.7% [39.5%, 45.2%] | 61.4% [57.8%, 64.6%] | -8.0 |
| 44 | Fire held without LOS | 37.8% [35.2%, 41.0%] | 24.5% [22.7%, 26.3%] | +7.8 |
| 45 | Fire held at -200 ms from sight gain | 35.2% [32.3%, 37.6%] | 21.7% [19.7%, 23.9%] | +7.7 |
| 46 | View turn over the last 500 ms before the first part | 6.84° [6.09°, 8.78°] | 14.53° [13.34°, 15.90°] | -7.7 |
| 47 | Release hazard while hidden, LOS lost 150-250 ms ago | 6.0% [4.5%, 7.6%] | 16.5% [13.8%, 18.7%] | -7.6 |
| 48 | Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part | 30.5% [26.8%, 34.3%] | 48.2% [45.7%, 50.3%] | -7.6 |
| 49 | Life length (lives ending in death) p50 | 26.2 s [22.0 s, 31.8 s] | 7.1 s [5.8 s, 8.2 s] | +7.6 |
| 50 | Crosshair on body firing with LOS at (128, 192] u | 29.5% [27.0%, 31.8%] | 42.5% [39.8%, 44.7%] | -7.5 |
| 51 | Share of duel time: LOS firefight | 7.5% [5.0%, 9.8%] | 17.6% [16.5%, 18.9%] | -7.3 |
| 52 | Yaw speed (hidden, firing) p50 | 13°/s [12°/s, 15°/s] | 23°/s [21°/s, 25°/s] | -7.3 |
| 53 | Duration of decisive engagements p50 | 2000 ms [1850 ms, 2151 ms] | 1250 ms [1150 ms, 1351 ms] | +7.1 |
| 54 | Yaw speed (hidden, firing) p99 | 350°/s [307°/s, 387°/s] | 194°/s [182°/s, 204°/s] | +7.1 |
| 55 | Strafe switch hazard per tick at hold age 250 ms (LOS firefight) | 26.7% [24.5%, 28.8%] | 17.4% [16.0%, 19.0%] | +7.0 |
| 56 | Crosshair within 5 deg of the corner, 500 ms before the first part | 10.9% [6.8%, 14.5%] | 29.2% [26.4%, 32.7%] | -7.0 |
| 57 | Strafe switch hazard per tick at hold age 50 ms (LOS firefight) | 7.1% [4.8%, 8.2%] | 1.2% [0.9%, 1.6%] | +7.0 |
| 58 | Yaw error to the corner at +0 ms (sightings hidden >= 2 s) | 9.76° [8.53°, 11.69°] | 3.54° [3.01°, 4.10°] | +7.0 |
| 59 | Attack hold duration p90 | 1900 ms [1650 ms, 2250 ms] | 900 ms [800 ms, 950 ms] | +6.8 |
| 60 | Crosshair on body firing with LOS at (192, 256] u | 28.4% [25.9%, 32.0%] | 42.0% [39.5%, 44.4%] | -6.6 |
| 61 | Key chord forward (hidden, firing) | 3.1% [2.3%, 3.5%] | 6.6% [5.7%, 7.4%] | -6.4 |
| 62 | Retreating >40 u/s in LOS firefight at distance (96, 160] | 23.7% [20.8%, 27.7%] | 10.5% [8.5%, 12.6%] | +6.3 |
| 63 | Key chord fwd diag (reloading) | 21.0% [17.8%, 25.4%] | 36.8% [34.1%, 39.9%] | -6.3 |
| 64 | Attack hold duration p50 | 450 ms [400 ms, 550 ms] | 150 ms [150 ms, 200 ms] | +6.3 |
| 65 | Release hazard while hidden, LOS lost 500-950 ms ago | 5.0% [3.8%, 6.1%] | 10.1% [9.2%, 11.3%] | -6.2 |
| 66 | Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) | 24.0% [22.1%, 26.0%] | 16.1% [14.9%, 17.5%] | +6.2 |
| 67 | Crosshair on body firing with LOS at (256, 384] u | 27.8% [25.5%, 30.4%] | 39.0% [36.0%, 41.4%] | -6.1 |
| 68 | Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part | 7.63° [5.04°, 11.24°] | -1.77° [-2.04°, -1.43°] | +6.0 |
| 69 | Yaw error to the hidden enemy 500 ms before the first part | 9.76° [8.73°, 11.20°] | 14.63° [13.70°, 15.63°] | -6.0 |
| 70 | Angle to the corner at the first part | 11.15° [9.80°, 13.07°] | 5.74° [5.33°, 6.17°] | +6.0 |
| 71 | Median aim error at +0 ms from the first visible part | 11.60° [10.05°, 13.62°] | 5.22° [4.91°, 5.53°] | +5.9 |
| 72 | Retreating >40 u/s in LOS firefight at distance (224, 288] | 16.5% [12.7%, 19.5%] | 5.3% [4.2%, 6.8%] | +5.9 |
| 73 | Angle to the corner, 500 ms before the first part | 17.56° [14.23°, 20.36°] | 8.41° [7.70°, 9.19°] | +5.9 |
| 74 | Yaw error to the corner, 500 ms before the first part | 16.16° [12.71°, 19.14°] | 5.81° [5.17°, 6.44°] | +5.8 |
| 75 | Mouse still between ticks (LOS firefight) | 2.7% [2.4%, 3.1%] | 1.6% [1.4%, 1.8%] | +5.8 |
| 76 | Approaching >40 u/s in LOS firefight at distance (96, 160] | 20.0% [16.5%, 23.4%] | 36.4% [32.6%, 41.2%] | -5.8 |
| 77 | Key chord pure strafe (hidden, firing) | 57.3% [55.9%, 62.3%] | 44.9% [41.6%, 48.0%] | +5.7 |
| 78 | Mouse still between ticks (hidden, firing) | 7.9% [7.3%, 8.6%] | 10.9% [10.1%, 11.8%] | -5.6 |
| 79 | Key chord fwd diag (hidden, firing) | 14.0% [12.2%, 16.4%] | 28.6% [23.9%, 32.8%] | -5.6 |
| 80 | Release hazard, LOS, aim error in half-widths (10, 1000] | 5.3% [3.8%, 8.1%] | 32.7% [22.9%, 41.8%] | -5.6 |
| 81 | Fire held, LOS, aim error in half-widths (10, 20] | 27.9% [19.4%, 35.8%] | 4.6% [2.4%, 6.9%] | +5.5 |
| 82 | View within 30 deg of the hidden opponent, last seen 4-8 s | 56.7% [50.4%, 61.8%] | 76.4% [73.1%, 79.3%] | -5.5 |
| 83 | Fire held at +1000 ms from sight gain | 51.9% [48.0%, 54.2%] | 63.4% [60.9%, 66.4%] | -5.4 |
| 84 | SMG shots that hit, centroid visible | 19.9% [17.8%, 21.9%] | 27.2% [25.8%, 28.5%] | -5.4 |
| 85 | Median aim error at +0 ms from sight gain | 13.65° [11.14°, 17.21°] | 5.22° [4.88°, 5.61°] | +5.3 |
| 86 | Release hazard while hidden, LOS lost 1 s or more ago | 5.7% [4.6%, 6.8%] | 11.3% [9.8%, 13.5%] | -5.2 |
| 87 | Median aim error firing with LOS at (192, 256] u | 8.89° [8.03°, 9.50°] | 6.68° [6.36°, 7.10°] | +5.1 |
| 88 | Median aim error at +100 ms from the first visible part | 9.79° [8.08°, 11.58°] | 4.87° [4.54°, 5.18°] | +5.1 |
| 89 | Approaching >40 u/s in LOS firefight at distance (160, 224] | 17.9% [14.7%, 22.3%] | 35.4% [30.1%, 42.0%] | -4.9 |
| 90 | Key chord back any (reloading) | 10.2% [9.4%, 10.9%] | 6.5% [5.3%, 7.6%] | +4.9 |
| 91 | Strafe switch hazard per tick at hold age 200 ms (LOS firefight) | 24.7% [21.7%, 27.4%] | 17.0% [15.4%, 18.7%] | +4.9 |
| 92 | Crosshair on body firing with LOS at (0, 128] u | 38.0% [36.0%, 40.3%] | 48.0% [44.9%, 51.5%] | -4.9 |
| 93 | Press hazard while hidden, LOS lost 0 ms ago | 5.9% [4.3%, 7.5%] | 12.3% [10.8%, 14.5%] | -4.9 |
| 94 | Release hazard while hidden, LOS lost 0-100 ms ago | 3.2% [2.5%, 3.8%] | 6.3% [5.4%, 7.5%] | -4.8 |
| 95 | Release hazard, LOS, aim error in half-widths (1, 2] | 0.7% [0.5%, 0.8%] | 1.5% [1.3%, 1.9%] | -4.8 |
| 96 | Yaw speed (hidden, not firing) p99 | 652°/s [620°/s, 683°/s] | 544°/s [521°/s, 580°/s] | +4.8 |
| 97 | Hidden, no sighting next: crosshair closer to where the enemy will be in 500 ms | 47.1% [46.4%, 47.8%] | 50.5% [49.0%, 51.6%] | -4.7 |
| 98 | Walking while hidden and not firing | 2.5% [1.6%, 3.5%] | 5.9% [5.0%, 7.0%] | -4.7 |
| 99 | Approaching >40 u/s in LOS firefight at distance (288, 384] | 16.6% [12.7%, 24.9%] | 39.4% [32.7%, 47.2%] | -4.6 |
| 100 | Yaw speed (reloading) p50 | 11°/s [9°/s, 13°/s] | 18°/s [16°/s, 20°/s] | -4.6 |
| 101 | Median aim error at +100 ms from sight gain | 12.39° [9.79°, 15.23°] | 5.97° [5.54°, 6.43°] | +4.5 |
| 102 | Standing still (<5 u/s) (reloading) | 10.8% [8.4%, 13.0%] | 5.2% [4.3%, 6.2%] | +4.4 |
| 103 | Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) | 17.99° [13.41°, 23.02°] | 5.78° [4.88°, 6.98°] | +4.4 |
| 104 | Strafe switch hazard per tick at hold age 150 ms (LOS firefight) | 21.0% [17.5%, 24.2%] | 12.4% [11.1%, 13.9%] | +4.4 |
| 105 | Median aim error at -500 ms from the first visible part | 11.07° [9.78°, 12.50°] | 15.23° [14.18°, 16.19°] | -4.3 |
| 106 | Shots per burst (consecutive 100 ms shots) p90 | 19 [16, 23] | 12 [11, 13] | +4.3 |
| 107 | Release hazard, LOS, aim error in half-widths (2, 3] | 0.6% [0.4%, 0.8%] | 1.5% [1.2%, 2.0%] | -4.1 |
| 108 | Approaching >40 u/s in LOS firefight at distance (224, 288] | 20.1% [16.4%, 26.3%] | 38.2% [32.3%, 46.4%] | -4.1 |
| 109 | Delay from kill to reload (reloads within 3 s) p50 | 700 ms [700 ms, 700 ms] | 600 ms [600 ms, 650 ms] | +4.1 |
| 110 | Reaction: first trigger press after a clean sighting p75 | 450 ms [412 ms, 500 ms] | 300 ms [250 ms, 300 ms] | +4.1 |
| 111 | Yaw error to the hidden opponent, last seen never seen this life | 11.40° [9.86°, 13.11°] | 19.23° [16.93°, 24.71°] | -3.9 |
| 112 | Key chord pure strafe (LOS firefight) | 69.8% [66.3%, 72.8%] | 58.0% [53.1%, 62.2%] | +3.9 |
| 113 | Key chord neutral (hidden, not firing) | 46.0% [36.3%, 55.7%] | 26.1% [23.1%, 28.6%] | +3.9 |
| 114 | Strafe switch hazard per tick at hold age 100 ms (LOS firefight) | 14.9% [10.2%, 19.1%] | 5.8% [5.2%, 6.8%] | +3.9 |
| 115 | Yaw speed (LOS firefight) p50 | 60°/s [49°/s, 71°/s] | 39°/s [36°/s, 41°/s] | +3.8 |
| 116 | Key chord fwd diag (LOS firefight) | 16.4% [12.1%, 20.7%] | 30.5% [25.6%, 36.7%] | -3.7 |
| 117 | Median aim error firing with LOS at (256, 384] u | 6.40° [5.87°, 6.82°] | 5.23° [4.96°, 5.59°] | +3.7 |
| 118 | Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) | 23.2% [19.7%, 29.0%] | 13.7% [12.0%, 15.6%] | +3.7 |
| 119 | Fire held at +0 ms from sight gain | 34.5% [31.2%, 36.8%] | 42.5% [39.1%, 46.0%] | -3.6 |
| 120 | Median aim error at -200 ms from sight gain | 13.81° [11.44°, 17.22°] | 8.33° [7.70°, 8.92°] | +3.6 |
| 121 | Median aim error at +200 ms from sight gain | 9.77° [7.83°, 12.73°] | 4.92° [4.54°, 5.23°] | +3.6 |
| 122 | Reaction: mean first press after a clean sighting, from the first part | 301 ms [268 ms, 338 ms] | 232 ms [217 ms, 250 ms] | +3.5 |
| 123 | Approaching >40 u/s in LOS firefight at distance (384, 512] | 18.4% [10.2%, 29.8%] | 41.8% [33.8%, 49.1%] | -3.5 |
| 124 | Side-hold duration (LOS firefight) p25 | 150 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | -3.5 |
| 125 | Median aim error at +300 ms from sight gain | 8.61° [6.97°, 11.40°] | 4.53° [4.12°, 4.88°] | +3.5 |
| 126 | Yaw speed (LOS, not firing) p99 | 767°/s [716°/s, 808°/s] | 583°/s [505°/s, 695°/s] | +3.5 |
| 127 | Approaching >40 u/s in LOS firefight at distance (0, 96] | 26.6% [21.9%, 30.7%] | 37.6% [33.7%, 42.3%] | -3.4 |
| 128 | Press hazard while hidden, LOS lost 50-100 ms ago | 5.7% [3.8%, 7.3%] | 9.5% [8.5%, 10.9%] | -3.3 |
| 129 | Median aim error at -100 ms from the first visible part | 11.48° [9.95°, 13.79°] | 8.00° [7.34°, 8.49°] | +3.3 |
| 130 | Mouse still between ticks (hidden, not firing) | 38.6% [36.8%, 40.6%] | 32.1% [28.8%, 36.0%] | +3.3 |
| 131 | Strafe end is a direct reverse (reloading) | 51.4% [44.6%, 57.9%] | 37.7% [32.5%, 42.7%] | +3.3 |
| 132 | Key chord fwd diag (LOS, not firing) | 12.7% [7.7%, 18.8%] | 29.4% [23.0%, 38.1%] | -3.3 |
| 133 | Fire held, LOS, aim error in half-widths (4, 6] | 78.1% [73.4%, 82.2%] | 67.7% [63.0%, 71.3%] | +3.2 |
| 134 | Fire held, LOS, aim error in half-widths (20, 1000] | 24.3% [12.3%, 39.9%] | 0.9% [0.4%, 3.1%] | +3.1 |
| 135 | Key chord back any (hidden, firing) | 8.7% [8.1%, 9.3%] | 6.7% [5.9%, 7.9%] | +3.1 |
| 136 | Yaw speed (hidden, not firing) p50 | 4°/s [3°/s, 6°/s] | 8°/s [6°/s, 10°/s] | -3.1 |
| 137 | View within 30 deg of the hidden opponent, last seen never seen this life | 73.2% [68.5%, 77.0%] | 62.5% [56.0%, 67.2%] | +3.0 |
| 138 | Key chord pure strafe (reloading) | 27.4% [25.4%, 31.1%] | 22.3% [19.6%, 24.4%] | +2.9 |
| 139 | View within 30 deg of the hidden opponent, last seen <=250 ms | 68.9% [56.2%, 87.8%] | 92.7% [91.9%, 93.7%] | -2.8 |
| 140 | Yaw error to the hidden opponent, last seen 4-8 s | 20.82° [16.30°, 29.28°] | 10.29° [9.19°, 11.80°] | +2.8 |
| 141 | Key chord forward (LOS firefight) | 0.8% [0.6%, 1.0%] | 1.2% [1.0%, 1.4%] | -2.7 |
| 142 | Fire held, LOS, aim error in half-widths (2, 3] | 87.8% [83.2%, 91.7%] | 81.0% [79.4%, 82.7%] | +2.7 |
| 143 | Crosshair on body firing with LOS at (384, 512] u | 25.4% [18.0%, 30.4%] | 35.4% [32.6%, 38.3%] | -2.7 |
| 144 | Fire held at +1450 ms from sight gain | 46.3% [43.1%, 48.8%] | 53.6% [49.3%, 58.5%] | -2.6 |
| 145 | Median aim error at +500 ms from sight gain | 9.18° [7.01°, 11.95°] | 5.47° [5.08°, 5.88°] | +2.6 |
| 146 | Standing still (<5 u/s) (hidden, firing) | 7.7% [5.2%, 8.5%] | 10.2% [9.1%, 11.7%] | -2.5 |
| 147 | View within 30 deg of the hidden opponent, last seen 2-4 s | 56.9% [51.0%, 61.0%] | 64.3% [61.3%, 67.3%] | -2.5 |
| 148 | View within 30 deg of the hidden opponent, last seen 250-500 ms | 75.4% [61.9%, 87.5%] | 92.2% [91.3%, 93.4%] | -2.4 |
| 149 | Retreating >40 u/s in LOS firefight at distance (384, 512] | 7.6% [6.2%, 10.3%] | 4.8% [3.8%, 6.0%] | +2.4 |
| 150 | Median aim error at +200 ms from the first visible part | 7.05° [5.83°, 8.52°] | 5.26° [4.86°, 5.58°] | +2.4 |
| 151 | First hit after the first part p50 | 300 ms [300 ms, 350 ms] | 250 ms [200 ms, 250 ms] | +2.4 |
| 152 | Yaw error to the corner at -2000 ms (sightings hidden >= 2 s) | 22.12° [17.71°, 27.51°] | 14.66° [11.85°, 17.22°] | +2.4 |
| 153 | Median aim error at +1000 ms from sight gain | 9.94° [7.74°, 12.22°] | 7.25° [6.72°, 7.70°] | +2.2 |
| 154 | View within 30 deg of the hidden opponent, last seen 0.5-1 s | 78.2% [67.6%, 84.9%] | 88.0% [86.8%, 89.9%] | -2.2 |
| 155 | Reload within 3 s of a kill, rounds left 1-3 | 100.0% [100.0%, 100.0%] | 96.4% [92.6%, 99.2%] | +2.1 |
| 156 | Yaw error to the hidden opponent, last seen 2-4 s | 22.10° [19.20°, 28.83°] | 16.93° [15.18°, 19.08°] | +1.9 |
| 157 | Yaw error to the hidden opponent, last seen <=250 ms | 12.79° [6.71°, 23.02°] | 4.92° [4.56°, 5.26°] | +1.8 |
| 158 | Median aim error firing with LOS at (384, 512] u | 5.24° [4.57°, 7.18°] | 3.92° [3.67°, 4.18°] | +1.2 |

## Per bot

### `bot:presser:4` (presser, seed 98799799, 31.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4709 | 0.3293 | -28% ⚠ |
| reverse_share | 0.8143 | 0.7839 | -4% |
| side_hold_ms | 303.8 | 200 | -69% ⚠ |
| lean_fight | 0.9578 | 0.9834 | +5% |
| jumps_per_min | 0.289 | 0.5021 | +4% |
| crouch_per_min | 2.432 | 2.762 | +3% |
| walk_hidden | 0.03529 | 0.05914 | +46% ⚠ |
| burst_median | 7 | 10.5 | +88% ⚠ |
| aim_height_firing | 0.3649 | 0.325 | -21% |
| aim_error_fight_deg | 5.46 | 7.557 | +85% ⚠ |
| reaction_ms | 171.1 | 350 | +179% ⚠ |
| mp40_share | 0.9398 | 1 | +6% |

Style fingerprint (2026-10-02, 31.9 min, styles.py features 30/30): z-distance to the human cloud centre 7.5 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.923 (humans 0.358–0.767); lf_lean 0.983 (humans 0.457–0.948); p_attack_given_los 0.469 (humans 0.547–0.861); still 0.0752 (humans 0.0832–0.227); lf_err_med 7.56 (humans 4.08–6.63); lf_miss_units_med 39.2 (humans 22.2–36.1); lf_on_target 0.274 (humans 0.274–0.515); lf_height_frac 0.325 (humans 0.352–0.54); side_hold_p90_ms 500 (humans 700–950).

### `bot:strafer:5` (strafer, seed 3439481337, 31.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1584 | 0.1211 | -7% |
| reverse_share | 0.4955 | 0.4642 | -4% |
| side_hold_ms | 350 | 250 | -67% ⚠ |
| lean_fight | 0.6796 | 0.7364 | +11% |
| jumps_per_min | 5.657 | 6.433 | +14% |
| crouch_per_min | 1.588 | 1.004 | -6% |
| walk_hidden | 0 | 0.02092 | +40% ⚠ |
| burst_median | 4.008 | 5 | +25% |
| aim_height_firing | 0.5028 | 0.479 | -13% |
| aim_error_fight_deg | 6.323 | 7.167 | +34% ⚠ |
| reaction_ms | 103.1 | 150 | +47% ⚠ |
| mp40_share | 0.8665 | 1 | +13% |

Style fingerprint (2026-10-02, 31.9 min, styles.py features 30/30): z-distance to the human cloud centre 8.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.495 (humans 0.191–0.489); fwd_diag 0.142 (humans 0.152–0.491); walk 0.00774 (humans 0.0126–0.0881); p_attack_given_los 0.495 (humans 0.547–0.861); still 0.0648 (humans 0.0832–0.227); lf_err_med 7.17 (humans 4.08–6.63); lf_miss_units_med 38.2 (humans 22.2–36.1); lf_on_target 0.264 (humans 0.274–0.515); side_hold_p90_ms 600 (humans 700–950); lf_retreat 0.204 (humans 0.0243–0.141).

### `bot:stopper:4` (stopper, seed 4094642640, 33.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1099 | 0.07031 | -8% |
| reverse_share | 0.1425 | 0.1002 | -6% |
| side_hold_ms | 216.1 | 150 | -44% ⚠ |
| lean_fight | 0.8376 | 0.8235 | -3% |
| jumps_per_min | 2.681 | 2.881 | +4% |
| crouch_per_min | 1.925 | 1.741 | -2% |
| walk_hidden | 0.02846 | 0.03417 | +11% |
| burst_median | 3.132 | 4 | +22% |
| aim_height_firing | 0.4975 | 0.461 | -19% |
| aim_error_fight_deg | 4.66 | 4.168 | -20% |
| reaction_ms | 159.5 | 250 | +90% ⚠ |
| mp40_share | 0.1053 | 0 | -11% |

Style fingerprint (2026-10-02, 33.3 min, styles.py features 30/30): z-distance to the human cloud centre 11.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0509 (humans 0.152–0.491); neutral 0.555 (humans 0.0872–0.296); lf_neutral 0.202 (humans 0.00436–0.112); attack 0.246 (humans 0.276–0.478); p_attack_given_los 0.501 (humans 0.547–0.861); speed_med 65.2 (humans 136–200); still 0.349 (humans 0.0832–0.227); yaw_speed_med 7.7 (humans 12.6–30); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 500 (humans 700–950); reverse_share 0.1 (humans 0.138–0.839); lf_approach 0.121 (humans 0.131–0.64).

### `bot:presser:5` (presser, seed 3362912357, 33.3 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.3871 | 0.3721 | -3% |
| reverse_share | 0.8213 | 0.7753 | -7% |
| side_hold_ms | 255.4 | 150 | -70% ⚠ |
| lean_fight | 0.906 | 0.9431 | +7% |
| jumps_per_min | 1.019 | 1.23 | +4% |
| crouch_per_min | 1.165 | 0.8703 | -3% |
| walk_hidden | 0.03838 | 0.05391 | +30% ⚠ |
| burst_median | 7 | 10 | +75% ⚠ |
| aim_height_firing | 0.3734 | 0.308 | -35% ⚠ |
| aim_error_fight_deg | 4.36 | 4.441 | +3% |
| reaction_ms | 178.5 | 400 | +222% ⚠ |
| mp40_share | 0.892 | 1 | +11% |

Style fingerprint (2026-10-02, 33.3 min, styles.py features 30/30): z-distance to the human cloud centre 8.6 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: neutral 0.308 (humans 0.0872–0.296); lf_back_any 0.0158 (humans 0.0205–0.123); lean_any 0.797 (humans 0.358–0.767); p_attack_given_los 0.473 (humans 0.547–0.861); speed_med 102 (humans 136–200); lf_height_frac 0.308 (humans 0.352–0.54); yaw_speed_med 5.92 (humans 12.6–30); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 400 (humans 700–950).

### `bot:strafer:4` (strafer, seed 3622030457, 35.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1509 | 0.08541 | -13% |
| reverse_share | 0.4882 | 0.4071 | -12% |
| side_hold_ms | 323.1 | 200 | -82% ⚠ |
| lean_fight | 0.6674 | 0.7547 | +17% |
| jumps_per_min | 3.413 | 2.958 | -8% |
| crouch_per_min | 7.298 | 7.509 | +2% |
| walk_hidden | 0.03936 | 0.03595 | -7% |
| burst_median | 3 | 3.5 | +12% |
| aim_height_firing | 0.4502 | 0.3965 | -29% ⚠ |
| aim_error_fight_deg | 6.413 | 7.486 | +44% ⚠ |
| reaction_ms | 136.5 | 150 | +14% |
| mp40_share | 0.9311 | 1 | +7% |

Style fingerprint (2026-10-02, 35.2 min, styles.py features 30/30): z-distance to the human cloud centre 8.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 8.3 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.505 (humans 0.191–0.489); fwd_diag 0.111 (humans 0.152–0.491); lf_pure_strafe 0.777 (humans 0.362–0.752); ducked 0.0819 (humans 0.0109–0.0741); attack 0.237 (humans 0.276–0.478); p_attack_given_los 0.498 (humans 0.547–0.861); speed_med 123 (humans 136–200); still 0.0711 (humans 0.0832–0.227); lf_err_med 7.49 (humans 4.08–6.63); yaw_speed_p95 287 (humans 158–271); side_hold_p90_ms 500 (humans 700–950); lf_retreat 0.17 (humans 0.0243–0.141).

### `bot:stopper:5` (stopper, seed 786231007, 35.2 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.07938 | 0.03682 | -8% |
| reverse_share | 0.1988 | 0.1453 | -8% |
| side_hold_ms | 217.3 | 150 | -45% ⚠ |
| lean_fight | 0.9448 | 0.975 | +6% |
| jumps_per_min | 1.346 | 1.138 | -4% |
| crouch_per_min | 1.362 | 1.735 | +4% |
| walk_hidden | 0.02934 | 0.0219 | -14% |
| burst_median | 4.394 | 5 | +15% |
| aim_height_firing | 0.4558 | 0.438 | -9% |
| aim_error_fight_deg | 6.294 | 8.374 | +85% ⚠ |
| reaction_ms | 111.7 | 200 | +88% ⚠ |
| mp40_share | 0.01466 | 0 | -1% |

Style fingerprint (2026-10-02, 35.2 min, styles.py features 30/30): z-distance to the human cloud centre 12.8 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 11.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0463 (humans 0.152–0.491); neutral 0.525 (humans 0.0872–0.296); lf_fwd_diag 0.0368 (humans 0.0658–0.568); lf_neutral 0.214 (humans 0.00436–0.112); lean_any 0.872 (humans 0.358–0.767); lf_lean 0.975 (humans 0.457–0.948); speed_med 78.4 (humans 136–200); still 0.294 (humans 0.0832–0.227); lf_err_med 8.38 (humans 4.08–6.63); lf_miss_units_med 38.3 (humans 22.2–36.1); lf_on_target 0.243 (humans 0.274–0.515); yaw_speed_med 12.3 (humans 12.6–30); yaw_speed_p95 294 (humans 158–271); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 500 (humans 700–950); lf_approach 0.0973 (humans 0.131–0.64); lf_retreat 0.218 (humans 0.0243–0.141).

### `bot:presser:4.2` (presser, seed 456908850, 34.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4664 | 0.2035 | -51% ⚠ |
| reverse_share | 0.8352 | 0.8038 | -4% |
| side_hold_ms | 284.9 | 200 | -57% ⚠ |
| lean_fight | 0.9039 | 0.9654 | +12% |
| jumps_per_min | 1.656 | 1.963 | +6% |
| crouch_per_min | 2.875 | 2.754 | -1% |
| walk_hidden | 0.02447 | 0.02309 | -3% |
| burst_median | 6.703 | 9 | +57% ⚠ |
| aim_height_firing | 0.352 | 0.299 | -28% ⚠ |
| aim_error_fight_deg | 5.043 | 12.46 | +302% ⚠ |
| reaction_ms | 151.2 | 250 | +99% ⚠ |
| mp40_share | 1 | 1 | +0% |

Style fingerprint (2026-10-02, 34.1 min, styles.py features 30/30): z-distance to the human cloud centre 11.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 11.5 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.836 (humans 0.358–0.767); lf_lean 0.965 (humans 0.457–0.948); walk 0.00981 (humans 0.0126–0.0881); speed_med 116 (humans 136–200); lf_err_med 12.5 (humans 4.08–6.63); lf_height_frac 0.299 (humans 0.352–0.54); yaw_speed_med 12.1 (humans 12.6–30); yaw_speed_p95 300 (humans 158–271); side_hold_p90_ms 450 (humans 700–950); lf_retreat 0.196 (humans 0.0243–0.141).

### `bot:presser:5.2` (presser, seed 4270387778, 34.1 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4984 | 0.2459 | -49% ⚠ |
| reverse_share | 0.8144 | 0.7803 | -5% |
| side_hold_ms | 310.4 | 200 | -74% ⚠ |
| lean_fight | 0.9588 | 0.9876 | +6% |
| jumps_per_min | 0.8428 | 0.5273 | -6% |
| crouch_per_min | 3.343 | 3.457 | +1% |
| walk_hidden | 0.052 | 0.03973 | -24% |
| burst_median | 6.077 | 8 | +48% ⚠ |
| aim_height_firing | 0.3861 | 0.373 | -7% |
| aim_error_fight_deg | 5.353 | 11.33 | +243% ⚠ |
| reaction_ms | 115.5 | 250 | +134% ⚠ |
| mp40_share | 0.07125 | 0 | -7% |

Style fingerprint (2026-10-02, 34.1 min, styles.py features 30/30): z-distance to the human cloud centre 10.5 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 11.2 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.507 (humans 0.191–0.489); lean_any 0.939 (humans 0.358–0.767); lf_lean 0.988 (humans 0.457–0.948); speed_med 127 (humans 136–200); still 0.0709 (humans 0.0832–0.227); lf_err_med 11.3 (humans 4.08–6.63); yaw_speed_p95 318 (humans 158–271); side_hold_p90_ms 450 (humans 700–950); lf_retreat 0.188 (humans 0.0243–0.141).

### `bot:presser:4.3` (presser, seed 2606960606, 39.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4487 | 0.1667 | -55% ⚠ |
| reverse_share | 0.8082 | 0.7119 | -14% |
| side_hold_ms | 293.3 | 150 | -96% ⚠ |
| lean_fight | 0.8992 | 1 | +20% |
| jumps_per_min | 0.289 | 0.4055 | +2% |
| crouch_per_min | 0.8978 | 0.7097 | -2% |
| walk_hidden | 0.02612 | 0.03556 | +18% |
| burst_median | 6.566 | 8 | +36% ⚠ |
| aim_height_firing | 0.352 | 0.1495 | -108% ⚠ |
| aim_error_fight_deg | 4.078 | 4.644 | +23% |
| reaction_ms | 156 | - | - |
| mp40_share | 0.1745 | 0 | -17% |

Style fingerprint (2026-10-02, 39.5 min, styles.py features 30/30): z-distance to the human cloud centre 16.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 14.1 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0759 (humans 0.152–0.491); neutral 0.503 (humans 0.0872–0.296); lf_pure_strafe 0.762 (humans 0.362–0.752); lf_back_any 0 (humans 0.0205–0.123); lf_lean 1 (humans 0.457–0.948); ducked 0.00591 (humans 0.0109–0.0741); attack 0.122 (humans 0.276–0.478); p_attack_given_los 0.0282 (humans 0.547–0.861); speed_med 42.9 (humans 136–200); still 0.391 (humans 0.0832–0.227); lf_miss_units_med 46.2 (humans 22.2–36.1); lf_on_target 0.214 (humans 0.274–0.515); lf_height_frac 0.149 (humans 0.352–0.54); yaw_speed_med 1.98 (humans 12.6–30); zero_yaw_share 0.409 (humans 0.137–0.309); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 300 (humans 700–950).

### `bot:strafer:5.2` (strafer, seed 159436562, 39.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.127 | 0.03658 | -18% |
| reverse_share | 0.4895 | 0.4053 | -12% |
| side_hold_ms | 321 | 200 | -81% ⚠ |
| lean_fight | 0.5599 | 0.8293 | +54% ⚠ |
| jumps_per_min | 5.657 | 7.477 | +34% ⚠ |
| crouch_per_min | 7.196 | 4.461 | -26% ⚠ |
| walk_hidden | 0 | 0 | +0% |
| burst_median | 6.989 | 9 | +50% ⚠ |
| aim_height_firing | 0.5334 | 0.63 | +51% ⚠ |
| aim_error_fight_deg | 4.686 | 4.594 | -4% |
| reaction_ms | 139.6 | 1300 | +1160% ⚠ |
| mp40_share | 0.03953 | 0 | -4% |

Style fingerprint (2026-10-02, 39.5 min, styles.py features 30/30): z-distance to the human cloud centre 15.9 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 14.4 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0765 (humans 0.152–0.491); neutral 0.382 (humans 0.0872–0.296); lf_pure_strafe 0.756 (humans 0.362–0.752); lf_fwd_diag 0.0366 (humans 0.0658–0.568); lf_back_any 0.146 (humans 0.0205–0.123); lean_any 0.211 (humans 0.358–0.767); ducked 0.0843 (humans 0.0109–0.0741); walk 0 (humans 0.0126–0.0881); attack 0.066 (humans 0.276–0.478); p_attack_given_los 0.0571 (humans 0.547–0.861); speed_med 104 (humans 136–200); still 0.258 (humans 0.0832–0.227); lf_miss_units_med 54.9 (humans 22.2–36.1); lf_on_target 0.171 (humans 0.274–0.515); lf_height_frac 0.63 (humans 0.352–0.54); yaw_speed_med 2.32 (humans 12.6–30); zero_yaw_share 0.427 (humans 0.137–0.309); side_hold_p90_ms 600 (humans 700–950); lf_approach 0.0854 (humans 0.131–0.64); lf_retreat 0.268 (humans 0.0243–0.141).

### `bot:presser:4.4` (presser, seed 1810870783, 39.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.4494 | 0.08796 | -71% ⚠ |
| reverse_share | 0.8331 | 0.7555 | -11% |
| side_hold_ms | 258.4 | 150 | -72% ⚠ |
| lean_fight | 0.9507 | 0.9985 | +10% |
| jumps_per_min | 0.9604 | 1.241 | +5% |
| crouch_per_min | 0.8978 | 0.608 | -3% |
| walk_hidden | 0.01436 | 0.002606 | -23% |
| burst_median | 7 | 12 | +125% ⚠ |
| aim_height_firing | 0.3627 | 5.394 | +2676% ⚠ |
| aim_error_fight_deg | 4.345 | 73.78 | +2827% ⚠ |
| reaction_ms | 124.8 | - | - |
| mp40_share | 0.9736 | 1 | +3% |

Style fingerprint (2026-10-02, 39.5 min, styles.py features 30/30): z-distance to the human cloud centre 137.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 136.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.185 (humans 0.191–0.489); fwd_diag 0.0412 (humans 0.152–0.491); neutral 0.706 (humans 0.0872–0.296); lf_pure_strafe 0.775 (humans 0.362–0.752); lean_any 0.83 (humans 0.358–0.767); lf_lean 0.998 (humans 0.457–0.948); walk 0.00234 (humans 0.0126–0.0881); attack 0.0801 (humans 0.276–0.478); p_attack_given_los 0.143 (humans 0.547–0.861); speed_med 0 (humans 136–200); still 0.616 (humans 0.0832–0.227); lf_err_med 73.8 (humans 4.08–6.63); lf_miss_units_med 466 (humans 22.2–36.1); lf_on_target 0.0309 (humans 0.274–0.515); lf_height_frac 5.39 (humans 0.352–0.54); los_nofire_height_frac 5.43 (humans 0.484–1.6); yaw_speed_med 5.7 (humans 12.6–30); zero_yaw_share 0.363 (humans 0.137–0.309); side_hold_med_ms 150 (humans 200–350); side_hold_p90_ms 350 (humans 700–950); lf_retreat 0.228 (humans 0.0243–0.141).

### `bot:strafer:5.3` (strafer, seed 1227344170, 39.5 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.101 | 0.1 | -0% |
| reverse_share | 0.4838 | 0.3959 | -13% |
| side_hold_ms | 350 | 250 | -67% ⚠ |
| lean_fight | 0.6051 | 0.7312 | +25% ⚠ |
| jumps_per_min | 5.657 | 6.359 | +13% |
| crouch_per_min | 6.111 | 4.56 | -15% |
| walk_hidden | 0.02332 | 0.01373 | -18% |
| burst_median | 3.46 | 4 | +14% |
| aim_height_firing | 0.5113 | -3.748 | -2266% ⚠ |
| aim_error_fight_deg | 6.476 | 78.99 | +2952% ⚠ |
| reaction_ms | 100.4 | 850 | +750% ⚠ |
| mp40_share | 0 | 0 | +0% |

Style fingerprint (2026-10-02, 39.5 min, styles.py features 30/30): z-distance to the human cloud centre 126.2 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 124.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0836 (humans 0.152–0.491); lf_neutral 0.212 (humans 0.00436–0.112); lean_any 0.272 (humans 0.358–0.767); ducked 0.0889 (humans 0.0109–0.0741); attack 0.0243 (humans 0.276–0.478); p_attack_given_los 0.0275 (humans 0.547–0.861); speed_med 124 (humans 136–200); lf_err_med 79 (humans 4.08–6.63); lf_miss_units_med 412 (humans 22.2–36.1); lf_on_target 0.075 (humans 0.274–0.515); lf_height_frac -3.75 (humans 0.352–0.54); los_nofire_height_frac -3.77 (humans 0.484–1.6); yaw_speed_med 9.78 (humans 12.6–30); zero_yaw_share 0.362 (humans 0.137–0.309); side_hold_p90_ms 550 (humans 700–950); lf_retreat 0.444 (humans 0.0243–0.141).

### `bot:presser:4.5` (presser, seed 880916664, 35.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.5302 | 0.1966 | -65% ⚠ |
| reverse_share | 0.7684 | 0.7494 | -3% |
| side_hold_ms | 256.8 | 200 | -38% ⚠ |
| lean_fight | 0.8724 | 0.955 | +16% |
| jumps_per_min | 1.318 | 0.9192 | -7% |
| crouch_per_min | 1.511 | 1.838 | +3% |
| walk_hidden | 0.004455 | 0.01298 | +16% |
| burst_median | 7 | 9 | +50% ⚠ |
| aim_height_firing | 0.3676 | 0.277 | -48% ⚠ |
| aim_error_fight_deg | 4.955 | 10.26 | +216% ⚠ |
| reaction_ms | 120.4 | 250 | +130% ⚠ |
| mp40_share | 0.9629 | 1 | +4% |

Style fingerprint (2026-10-02, 35.9 min, styles.py features 30/30): z-distance to the human cloud centre 9.7 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 9.9 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: lean_any 0.795 (humans 0.358–0.767); lf_lean 0.955 (humans 0.457–0.948); walk 0.00415 (humans 0.0126–0.0881); speed_med 115 (humans 136–200); lf_err_med 10.3 (humans 4.08–6.63); lf_height_frac 0.277 (humans 0.352–0.54); yaw_speed_p95 298 (humans 158–271); side_hold_p90_ms 400 (humans 700–950).

### `bot:strafer:5.4` (strafer, seed 1562960504, 35.9 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.08235 | 0.04872 | -7% |
| reverse_share | 0.4888 | 0.4402 | -7% |
| side_hold_ms | 350 | 250 | -67% ⚠ |
| lean_fight | 0.5942 | 0.709 | +23% |
| jumps_per_min | 3.055 | 2.897 | -3% |
| crouch_per_min | 3.608 | 3.649 | +0% |
| walk_hidden | 0.009705 | 0.005741 | -8% |
| burst_median | 3.747 | 4 | +6% |
| aim_height_firing | 0.5109 | 0.478 | -18% |
| aim_error_fight_deg | 5.215 | 13.32 | +330% ⚠ |
| reaction_ms | 151.6 | 250 | +98% ⚠ |
| mp40_share | 0.07744 | 0 | -8% |

Style fingerprint (2026-10-02, 35.9 min, styles.py features 30/30): z-distance to the human cloud centre 13.4 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 12.7 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: pure_strafe 0.541 (humans 0.191–0.489); fwd_diag 0.0912 (humans 0.152–0.491); lf_pure_strafe 0.791 (humans 0.362–0.752); lf_fwd_diag 0.0487 (humans 0.0658–0.568); walk 0.00276 (humans 0.0126–0.0881); p_attack_given_los 0.534 (humans 0.547–0.861); speed_med 130 (humans 136–200); still 0.0696 (humans 0.0832–0.227); lf_err_med 13.3 (humans 4.08–6.63); yaw_speed_p95 293 (humans 158–271); side_hold_p90_ms 500 (humans 700–950); lf_approach 0.122 (humans 0.131–0.64); lf_retreat 0.303 (humans 0.0243–0.141).

### `bot:strafer:4.2` (strafer, seed 640050134, 35.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.06623 | 0.03073 | -7% |
| reverse_share | 0.4864 | 0.4372 | -7% |
| side_hold_ms | 280.8 | 200 | -54% ⚠ |
| lean_fight | 0.6175 | 0.6967 | +16% |
| jumps_per_min | 2.245 | 2.301 | +1% |
| crouch_per_min | 11.47 | 12.15 | +6% |
| walk_hidden | 0.04578 | 0.05203 | +12% |
| burst_median | 5.551 | 8 | +61% ⚠ |
| aim_height_firing | 0.54 | 0.508 | -17% |
| aim_error_fight_deg | 5.818 | 10.62 | +195% ⚠ |
| reaction_ms | 116.8 | 150 | +33% ⚠ |
| mp40_share | 0.904 | 1 | +10% |

Style fingerprint (2026-10-02, 35.6 min, styles.py features 30/30): z-distance to the human cloud centre 11.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 10.0 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.0765 (humans 0.152–0.491); neutral 0.316 (humans 0.0872–0.296); lf_pure_strafe 0.786 (humans 0.362–0.752); lf_fwd_diag 0.0307 (humans 0.0658–0.568); ducked 0.11 (humans 0.0109–0.0741); speed_med 106 (humans 136–200); lf_err_med 10.6 (humans 4.08–6.63); yaw_speed_p95 287 (humans 158–271); side_hold_p90_ms 500 (humans 700–950); lf_approach 0.122 (humans 0.131–0.64); lf_retreat 0.22 (humans 0.0243–0.141).

### `bot:strafer:5.5` (strafer, seed 1110590673, 35.6 duel minutes)

Dial recovery: realised dial (fit_styles.py definitions, all of the bot's duel time) against the dial drawn in its `bot_style` event; the error is in % of the human range (max − min). |error| ≥ 25% is marked.

| Dial | Drawn | Realised | Error (% of human range) |
|---|---:|---:|---:|
| fwd_diag_fight | 0.1873 | 0.07773 | -21% |
| reverse_share | 0.4789 | 0.4086 | -10% |
| side_hold_ms | 300.9 | 200 | -67% ⚠ |
| lean_fight | 0.4565 | 0.5192 | +12% |
| jumps_per_min | 3.482 | 4.069 | +11% |
| crouch_per_min | 10.24 | 9.851 | -4% |
| walk_hidden | 0.04196 | 0.04403 | +4% |
| burst_median | 4.309 | 5 | +17% |
| aim_height_firing | 0.4962 | 0.43 | -35% ⚠ |
| aim_error_fight_deg | 4.947 | 8.347 | +138% ⚠ |
| reaction_ms | 169.6 | 300 | +130% ⚠ |
| mp40_share | 0.01851 | 0 | -2% |

Style fingerprint (2026-10-02, 35.6 min, styles.py features 30/30): z-distance to the human cloud centre 9.1 (human captures: up to 7.4; OUTSIDE the human spread). Nearest human capture at z-distance 7.8 vs the within-person threshold 5.6 (farther than two captures of one human).

Features outside the human min–max: fwd_diag 0.114 (humans 0.152–0.491); lf_pure_strafe 0.78 (humans 0.362–0.752); lean_any 0.319 (humans 0.358–0.767); ducked 0.117 (humans 0.0109–0.0741); speed_med 121 (humans 136–200); lf_err_med 8.35 (humans 4.08–6.63); yaw_speed_med 12 (humans 12.6–30); side_hold_p90_ms 500 (humans 700–950); lf_approach 0.131 (humans 0.131–0.64); lf_retreat 0.152 (humans 0.0243–0.141).

## The owner against the bots (`pstN@vsbot`)

Kills of bots 0, deaths to bots 0, suicides 0. 

0 statistics of the owner against bots fall outside the human CI (owner column below, marked •).

## All statistics

Pooled bots are reweighted to the human family mix; humans and the owner are pooled over their duel time. ▲ marks a tell. n = units (ticks, runs, shots, sightings, ...) over the blocks.

### Movement

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time: hidden, not firing | 53.8% [45.8%, 63.1%] | 49.1% [47.4%, 51.2%] | - | 683870 |
| Share of duel time: hidden, firing | 19.1% [14.8%, 22.7%] | 15.8% [14.9%, 17.0%] | - | 683870 |
| Share of duel time: LOS, not firing | 4.0% [3.1%, 5.5%] | 7.0% [5.5%, 8.4%] | - | 683870 |
| Share of duel time: LOS firefight ▲ | 7.5% [5.0%, 9.8%] | 17.6% [16.5%, 18.9%] | - | 683870 |
| Share of duel time: reloading | 15.6% [11.4%, 19.0%] | 10.5% [9.6%, 11.5%] | - | 683870 |
| Key chord pure strafe (hidden, not firing) | 31.4% [26.0%, 36.9%] | 26.0% [23.5%, 29.4%] | - | 373140 |
| Key chord fwd diag (hidden, not firing) ▲ | 8.7% [6.3%, 11.5%] | 29.9% [26.7%, 33.2%] | - | 373140 |
| Key chord forward (hidden, not firing) ▲ | 5.9% [4.8%, 6.8%] | 11.9% [10.9%, 12.8%] | - | 373140 |
| Key chord neutral (hidden, not firing) ▲ | 46.0% [36.3%, 55.7%] | 26.1% [23.1%, 28.6%] | - | 373140 |
| Key chord back any (hidden, not firing) | 8.1% [6.9%, 9.2%] | 6.1% [5.3%, 7.1%] | - | 373140 |
| Key chord pure strafe (hidden, firing) ▲ | 57.3% [55.9%, 62.3%] | 44.9% [41.6%, 48.0%] | - | 126730 |
| Key chord fwd diag (hidden, firing) ▲ | 14.0% [12.2%, 16.4%] | 28.6% [23.9%, 32.8%] | - | 126730 |
| Key chord forward (hidden, firing) ▲ | 3.1% [2.3%, 3.5%] | 6.6% [5.7%, 7.4%] | - | 126730 |
| Key chord neutral (hidden, firing) | 16.8% [11.0%, 18.5%] | 13.2% [11.2%, 15.7%] | - | 126730 |
| Key chord back any (hidden, firing) ▲ | 8.7% [8.1%, 9.3%] | 6.7% [5.9%, 7.9%] | - | 126730 |
| Key chord pure strafe (LOS, not firing) | 34.4% [27.8%, 41.6%] | 26.7% [20.8%, 34.2%] | - | 28757 |
| Key chord fwd diag (LOS, not firing) ▲ | 12.7% [7.7%, 18.8%] | 29.4% [23.0%, 38.1%] | - | 28757 |
| Key chord forward (LOS, not firing) | 6.2% [4.6%, 7.7%] | 7.5% [5.9%, 9.5%] | - | 28757 |
| Key chord neutral (LOS, not firing) | 40.0% [26.8%, 52.7%] | 31.5% [15.3%, 45.4%] | - | 28757 |
| Key chord back any (LOS, not firing) | 6.8% [5.3%, 8.1%] | 4.8% [3.4%, 6.7%] | - | 28757 |
| Key chord pure strafe (LOS firefight) ▲ | 69.8% [66.3%, 72.8%] | 58.0% [53.1%, 62.2%] | - | 50563 |
| Key chord fwd diag (LOS firefight) ▲ | 16.4% [12.1%, 20.7%] | 30.5% [25.6%, 36.7%] | - | 50563 |
| Key chord forward (LOS firefight) ▲ | 0.8% [0.6%, 1.0%] | 1.2% [1.0%, 1.4%] | - | 50563 |
| Key chord neutral (LOS firefight) | 7.2% [4.4%, 8.9%] | 5.1% [3.9%, 6.3%] | - | 50563 |
| Key chord back any (LOS firefight) | 5.8% [4.7%, 7.2%] | 5.2% [4.5%, 6.1%] | - | 50563 |
| Key chord pure strafe (reloading) ▲ | 27.4% [25.4%, 31.1%] | 22.3% [19.6%, 24.4%] | - | 104680 |
| Key chord fwd diag (reloading) ▲ | 21.0% [17.8%, 25.4%] | 36.8% [34.1%, 39.9%] | - | 104680 |
| Key chord forward (reloading) ▲ | 14.2% [12.4%, 15.4%] | 26.1% [23.8%, 28.6%] | - | 104680 |
| Key chord neutral (reloading) ▲ | 27.2% [21.4%, 30.8%] | 8.3% [7.3%, 9.5%] | - | 104680 |
| Key chord back any (reloading) ▲ | 10.2% [9.4%, 10.9%] | 6.5% [5.3%, 7.6%] | - | 104680 |
| Strafe end is a direct reverse (hidden, not firing) | 53.0% [49.5%, 57.2%] | 46.1% [41.1%, 51.2%] | - | 37584 |
| Strafe end is a direct reverse (hidden, firing) | 68.8% [65.2%, 76.6%] | 67.2% [59.7%, 74.3%] | - | 20110 |
| Strafe end is a direct reverse (LOS, not firing) | 55.8% [49.6%, 63.6%] | 52.8% [45.1%, 60.4%] | - | 2857 |
| Strafe end is a direct reverse (LOS firefight) | 70.3% [62.8%, 81.3%] | 73.3% [65.9%, 80.6%] | - | 8774 |
| Strafe end is a direct reverse (reloading) ▲ | 51.4% [44.6%, 57.9%] | 37.7% [32.5%, 42.7%] | - | 12041 |
| Strafe switch hazard per tick at hold age 50 ms (LOS firefight) ▲ | 7.1% [4.8%, 8.2%] | 1.2% [0.9%, 1.6%] | - | 9166 |
| Strafe switch hazard per tick at hold age 100 ms (LOS firefight) ▲ | 14.9% [10.2%, 19.1%] | 5.8% [5.2%, 6.8%] | - | 8336 |
| Strafe switch hazard per tick at hold age 150 ms (LOS firefight) ▲ | 21.0% [17.5%, 24.2%] | 12.4% [11.1%, 13.9%] | - | 7075 |
| Strafe switch hazard per tick at hold age 200 ms (LOS firefight) ▲ | 24.7% [21.7%, 27.4%] | 17.0% [15.4%, 18.7%] | - | 5634 |
| Strafe switch hazard per tick at hold age 250 ms (LOS firefight) ▲ | 26.7% [24.5%, 28.8%] | 17.4% [16.0%, 19.0%] | - | 4197 |
| Strafe switch hazard per tick at hold age 300-450 ms (LOS firefight) ▲ | 27.5% [25.6%, 29.2%] | 15.6% [14.7%, 16.9%] | - | 7907 |
| Strafe switch hazard per tick at hold age 500-1000 ms (LOS firefight) ▲ | 24.0% [22.1%, 26.0%] | 16.1% [14.9%, 17.5%] | - | 3526 |
| Strafe switch hazard per tick at hold age 1050-1500 ms (LOS firefight) ▲ | 23.2% [19.7%, 29.0%] | 13.7% [12.0%, 15.6%] | - | 348 |
| Side-hold duration (LOS firefight) p25 ▲ | 150 ms [100 ms, 150 ms] | 200 ms [200 ms, 200 ms] | - | 8658 |
| Side-hold duration (LOS firefight) p50 | 200 ms [200 ms, 250 ms] | 300 ms [250 ms, 300 ms] | - | 8658 |
| Side-hold duration (LOS firefight) p75 ▲ | 300 ms [300 ms, 350 ms] | 450 ms [449 ms, 450 ms] | - | 8658 |
| Side-hold duration (LOS firefight) p90 ▲ | 450 ms [450 ms, 500 ms] | 700 ms [650 ms, 700 ms] | - | 8658 |
| Side-hold duration (all contexts) p50 ▲ | 200 ms [200 ms, 200 ms] | 300 ms [300 ms, 300 ms] | - | 81072 |
| Side-hold duration (all contexts) p90 ▲ | 450 ms [450 ms, 500 ms] | 800 ms [800 ms, 850 ms] | - | 81072 |
| Neutral gap between strafes (LOS firefight) p50 | 50 ms [50 ms, 50 ms] | 50 ms [50 ms, 50 ms] | - | 2618 |
| Neutral gap between strafes (LOS firefight) p90 | 50 ms [50 ms, 100 ms] | 100 ms [100 ms, 150 ms] | - | 2618 |
| Approaching >40 u/s in LOS firefight at distance (0, 96] ▲ | 26.6% [21.9%, 30.7%] | 37.6% [33.7%, 42.3%] | - | 7922 |
| Approaching >40 u/s in LOS firefight at distance (96, 160] ▲ | 20.0% [16.5%, 23.4%] | 36.4% [32.6%, 41.2%] | - | 10220 |
| Approaching >40 u/s in LOS firefight at distance (160, 224] ▲ | 17.9% [14.7%, 22.3%] | 35.4% [30.1%, 42.0%] | - | 9264 |
| Approaching >40 u/s in LOS firefight at distance (224, 288] ▲ | 20.1% [16.4%, 26.3%] | 38.2% [32.3%, 46.4%] | - | 5441 |
| Approaching >40 u/s in LOS firefight at distance (288, 384] ▲ | 16.6% [12.7%, 24.9%] | 39.4% [32.7%, 47.2%] | - | 6970 |
| Approaching >40 u/s in LOS firefight at distance (384, 512] ▲ | 18.4% [10.2%, 29.8%] | 41.8% [33.8%, 49.1%] | - | 6085 |
| Approaching >40 u/s in LOS firefight at distance (512, 768] | 27.9% [11.8%, 39.1%] | 33.1% [25.5%, 41.5%] | - | 3926 |
| Approaching >40 u/s in LOS firefight at distance (768, 1200] | 15.4% [7.6%, 20.2%] | 28.2% [17.6%, 42.7%] | - | 735 |
| Retreating >40 u/s in LOS firefight at distance (0, 96] | 27.0% [23.9%, 30.8%] | 22.2% [18.9%, 25.6%] | - | 7922 |
| Retreating >40 u/s in LOS firefight at distance (96, 160] ▲ | 23.7% [20.8%, 27.7%] | 10.5% [8.5%, 12.6%] | - | 10220 |
| Retreating >40 u/s in LOS firefight at distance (160, 224] ▲ | 19.1% [17.4%, 21.5%] | 6.9% [5.5%, 8.7%] | - | 9264 |
| Retreating >40 u/s in LOS firefight at distance (224, 288] ▲ | 16.5% [12.7%, 19.5%] | 5.3% [4.2%, 6.8%] | - | 5441 |
| Retreating >40 u/s in LOS firefight at distance (288, 384] | 9.8% [6.3%, 13.8%] | 5.9% [4.6%, 7.3%] | - | 6970 |
| Retreating >40 u/s in LOS firefight at distance (384, 512] ▲ | 7.6% [6.2%, 10.3%] | 4.8% [3.8%, 6.0%] | - | 6085 |
| Retreating >40 u/s in LOS firefight at distance (512, 768] | 8.0% [5.2%, 11.2%] | 4.0% [3.0%, 5.4%] | - | 3926 |
| Retreating >40 u/s in LOS firefight at distance (768, 1200] | 10.5% [2.3%, 15.4%] | 8.3% [1.5%, 15.5%] | - | 735 |
| Standing still (<5 u/s), all duel time | 21.9% [14.8%, 30.4%] | 15.7% [13.4%, 17.7%] | - | 683870 |
| Standing still (<5 u/s) (hidden, not firing) | 32.3% [22.7%, 42.7%] | 22.5% [19.5%, 25.1%] | - | 373140 |
| Standing still (<5 u/s) (hidden, firing) ▲ | 7.7% [5.2%, 8.5%] | 10.2% [9.1%, 11.7%] | - | 126730 |
| Standing still (<5 u/s) (LOS, not firing) | 28.7% [14.8%, 41.4%] | 29.6% [12.6%, 44.0%] | - | 28757 |
| Standing still (<5 u/s) (LOS firefight) | 2.1% [1.6%, 2.6%] | 2.3% [1.8%, 2.7%] | - | 50563 |
| Standing still (<5 u/s) (reloading) ▲ | 10.8% [8.4%, 13.0%] | 5.2% [4.3%, 6.2%] | - | 104680 |
| Lean held, all duel time | 66.0% [61.1%, 70.8%] | 59.7% [55.3%, 64.0%] | - | 683870 |
| Lean held in LOS firefights | 87.0% [81.9%, 92.4%] | 81.3% [76.6%, 85.8%] | - | 50563 |
| Jump presses per minute | 2.38/min [1.87/min, 2.95/min] | 1.87/min [1.41/min, 2.44/min] | - | 683870 |
| Crouch presses per minute | 3.25/min [2.25/min, 4.22/min] | 4.33/min [3.22/min, 5.51/min] | - | 683870 |
| Walk presses per minute ▲ | 1.97/min [1.37/min, 2.61/min] | 7.54/min [6.90/min, 8.11/min] | - | 683870 |
| Walking while hidden and not firing ▲ | 2.5% [1.6%, 3.5%] | 5.9% [5.0%, 7.0%] | - | 373140 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Press hazard with LOS and aim within 3 half-widths, LOS gained 0 ms ago | 31.2% [23.5%, 38.6%] | 28.2% [25.8%, 30.8%] | - | 536 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 50-100 ms ago | 35.9% [28.7%, 44.4%] | 33.0% [30.8%, 35.6%] | - | 650 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 150-250 ms ago | 31.0% [25.7%, 40.1%] | 35.2% [32.9%, 38.1%] | - | 592 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 300-500 ms ago | 29.0% [24.8%, 36.2%] | 28.0% [25.2%, 32.0%] | - | 462 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained 550-1000 ms ago | 23.0% [16.2%, 35.3%] | 21.0% [16.7%, 24.6%] | - | 483 |
| Press hazard with LOS and aim within 3 half-widths, LOS gained over 1 s ago | 14.0% [9.7%, 21.7%] | 7.4% [4.7%, 10.8%] | - | 2005 |
| Press hazard while hidden, LOS lost 0 ms ago ▲ | 5.9% [4.3%, 7.5%] | 12.3% [10.8%, 14.5%] | - | 818 |
| Press hazard while hidden, LOS lost 50-100 ms ago ▲ | 5.7% [3.8%, 7.3%] | 9.5% [8.5%, 10.9%] | - | 1343 |
| Press hazard while hidden, LOS lost 150-250 ms ago | 4.2% [3.1%, 5.4%] | 4.6% [3.9%, 5.5%] | - | 1928 |
| Press hazard while hidden, LOS lost 300-500 ms ago | 8.8% [7.8%, 9.9%] | 7.0% [6.0%, 8.1%] | - | 5440 |
| Press hazard while hidden, LOS lost 550-1000 ms ago | 3.7% [2.9%, 4.5%] | 3.5% [3.2%, 3.9%] | - | 10515 |
| Press hazard while hidden, LOS lost over 1 s ago | 3.9% [3.4%, 4.5%] | 4.1% [3.5%, 4.6%] | - | 191500 |
| Release hazard, LOS, aim error in half-widths (0, 1] | 0.8% [0.5%, 1.1%] | 1.4% [1.1%, 1.7%] | - | 7263 |
| Release hazard, LOS, aim error in half-widths (1, 2] ▲ | 0.7% [0.5%, 0.8%] | 1.5% [1.3%, 1.9%] | - | 14237 |
| Release hazard, LOS, aim error in half-widths (2, 3] ▲ | 0.6% [0.4%, 0.8%] | 1.5% [1.2%, 2.0%] | - | 11269 |
| Release hazard, LOS, aim error in half-widths (3, 4] | 0.7% [0.4%, 1.1%] | 1.4% [1.0%, 2.0%] | - | 6689 |
| Release hazard, LOS, aim error in half-widths (4, 6] | 1.1% [0.8%, 1.4%] | 1.7% [1.3%, 2.3%] | - | 5960 |
| Release hazard, LOS, aim error in half-widths (6, 10] | 3.7% [2.4%, 4.7%] | 6.9% [4.4%, 9.9%] | - | 2599 |
| Release hazard, LOS, aim error in half-widths (10, 1000] ▲ | 5.3% [3.8%, 8.1%] | 32.7% [22.9%, 41.8%] | - | 1871 |
| Release hazard while hidden, LOS lost 0-100 ms ago ▲ | 3.2% [2.5%, 3.8%] | 6.3% [5.4%, 7.5%] | - | 6366 |
| Release hazard while hidden, LOS lost 150-250 ms ago ▲ | 6.0% [4.5%, 7.6%] | 16.5% [13.8%, 18.7%] | - | 3730 |
| Release hazard while hidden, LOS lost 300-450 ms ago ▲ | 17.5% [14.8%, 19.8%] | 31.5% [29.6%, 33.4%] | - | 4102 |
| Release hazard while hidden, LOS lost 500-950 ms ago ▲ | 5.0% [3.8%, 6.1%] | 10.1% [9.2%, 11.3%] | - | 5797 |
| Release hazard while hidden, LOS lost 1 s or more ago ▲ | 5.7% [4.6%, 6.8%] | 11.3% [9.8%, 13.5%] | - | 104810 |
| Fire held, LOS, aim error in half-widths (0, 0.5] | 87.4% [81.3%, 92.4%] | 89.6% [87.0%, 91.7%] | - | 2365 |
| Fire held, LOS, aim error in half-widths (0.5, 1] | 85.9% [80.2%, 90.7%] | 88.9% [86.9%, 90.5%] | - | 6069 |
| Fire held, LOS, aim error in half-widths (1, 1.5] | 86.2% [80.6%, 90.6%] | 86.3% [84.5%, 87.8%] | - | 7939 |
| Fire held, LOS, aim error in half-widths (1.5, 2] | 88.4% [82.8%, 92.9%] | 81.9% [79.8%, 83.7%] | - | 8443 |
| Fire held, LOS, aim error in half-widths (2, 3] ▲ | 87.8% [83.2%, 91.7%] | 81.0% [79.4%, 82.7%] | - | 12923 |
| Fire held, LOS, aim error in half-widths (3, 4] | 84.8% [79.7%, 89.3%] | 78.8% [76.1%, 81.2%] | - | 7938 |
| Fire held, LOS, aim error in half-widths (4, 6] ▲ | 78.1% [73.4%, 82.2%] | 67.7% [63.0%, 71.3%] | - | 7630 |
| Fire held, LOS, aim error in half-widths (6, 10] ▲ | 60.3% [53.9%, 65.8%] | 22.4% [17.1%, 27.1%] | - | 4300 |
| Fire held, LOS, aim error in half-widths (10, 20] ▲ | 27.9% [19.4%, 35.8%] | 4.6% [2.4%, 6.9%] | - | 2416 |
| Fire held, LOS, aim error in half-widths (20, 1000] ▲ | 24.3% [12.3%, 39.9%] | 0.9% [0.4%, 3.1%] | - | 5073 |
| Fire held without LOS ▲ | 37.8% [35.2%, 41.0%] | 24.5% [22.7%, 26.3%] | - | 336380 |
| Attack hold duration p50 ▲ | 450 ms [400 ms, 550 ms] | 150 ms [150 ms, 200 ms] | - | 10215 |
| Attack hold duration p90 ▲ | 1900 ms [1650 ms, 2250 ms] | 900 ms [800 ms, 950 ms] | - | 10215 |
| Attack holds that are taps (<=100 ms) ▲ | 21.8% [19.3%, 23.8%] | 44.3% [41.8%, 46.5%] | - | 10215 |
| Gap between attack holds p50 | 450 ms [400 ms, 550 ms] | 400 ms [350 ms, 400 ms] | - | 9898 |

### Aim

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error firing with LOS at (0, 128] u ▲ | 21.13° [20.05°, 21.81°] | 14.28° [13.53°, 15.19°] | - | 11131 |
| Crosshair on body firing with LOS at (0, 128] u ▲ | 38.0% [36.0%, 40.3%] | 48.0% [44.9%, 51.5%] | - | 11131 |
| Median aim error firing with LOS at (128, 192] u ▲ | 12.81° [12.01°, 13.49°] | 8.99° [8.66°, 9.45°] | - | 9345 |
| Crosshair on body firing with LOS at (128, 192] u ▲ | 29.5% [27.0%, 31.8%] | 42.5% [39.8%, 44.7%] | - | 9345 |
| Median aim error firing with LOS at (192, 256] u ▲ | 8.89° [8.03°, 9.50°] | 6.68° [6.36°, 7.10°] | - | 8484 |
| Crosshair on body firing with LOS at (192, 256] u ▲ | 28.4% [25.9%, 32.0%] | 42.0% [39.5%, 44.4%] | - | 8484 |
| Median aim error firing with LOS at (256, 384] u ▲ | 6.40° [5.87°, 6.82°] | 5.23° [4.96°, 5.59°] | - | 9710 |
| Crosshair on body firing with LOS at (256, 384] u ▲ | 27.8% [25.5%, 30.4%] | 39.0% [36.0%, 41.4%] | - | 9710 |
| Median aim error firing with LOS at (384, 512] u ▲ | 5.24° [4.57°, 7.18°] | 3.92° [3.67°, 4.18°] | - | 7186 |
| Crosshair on body firing with LOS at (384, 512] u ▲ | 25.4% [18.0%, 30.4%] | 35.4% [32.6%, 38.3%] | - | 7186 |
| Median aim error firing with LOS at (512, 768] u | 3.37° [2.91°, 4.13°] | 3.07° [2.80°, 3.35°] | - | 3973 |
| Crosshair on body firing with LOS at (512, 768] u | 26.5% [20.0%, 31.7%] | 32.0% [29.1%, 35.1%] | - | 3973 |
| Median aim error firing with LOS at (768, 1200] u | 2.43° [2.18°, 3.52°] | 2.14° [1.84°, 2.58°] | - | 735 |
| Crosshair on body firing with LOS at (768, 1200] u | 24.8% [15.5%, 29.6%] | 31.5% [24.3%, 39.5%] | - | 735 |
| Aim height (fraction of body) firing with LOS | 0.401 [0.357, 0.443] | 0.446 [0.418, 0.468] | - | 50564 |
| Aim height (fraction of body) with LOS, not firing | 0.628 [0.586, 0.7] | 0.662 [0.576, 0.751] | - | 50411 |

### View

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw speed (hidden, not firing) p50 ▲ | 4°/s [3°/s, 6°/s] | 8°/s [6°/s, 10°/s] | - | 372500 |
| Yaw speed (hidden, not firing) p99 ▲ | 652°/s [620°/s, 683°/s] | 544°/s [521°/s, 580°/s] | - | 372500 |
| Mouse still between ticks (hidden, not firing) ▲ | 38.6% [36.8%, 40.6%] | 32.1% [28.8%, 36.0%] | - | 373140 |
| Yaw speed (hidden, firing) p50 ▲ | 13°/s [12°/s, 15°/s] | 23°/s [21°/s, 25°/s] | - | 126730 |
| Yaw speed (hidden, firing) p99 ▲ | 350°/s [307°/s, 387°/s] | 194°/s [182°/s, 204°/s] | - | 126730 |
| Mouse still between ticks (hidden, firing) ▲ | 7.9% [7.3%, 8.6%] | 10.9% [10.1%, 11.8%] | - | 126730 |
| Yaw speed (LOS, not firing) p50 | 11°/s [6°/s, 19°/s] | 11°/s [0°/s, 26°/s] | - | 28753 |
| Yaw speed (LOS, not firing) p99 ▲ | 767°/s [716°/s, 808°/s] | 583°/s [505°/s, 695°/s] | - | 28753 |
| Mouse still between ticks (LOS, not firing) | 35.7% [30.5%, 41.8%] | 36.7% [20.8%, 49.8%] | - | 28757 |
| Yaw speed (LOS firefight) p50 ▲ | 60°/s [49°/s, 71°/s] | 39°/s [36°/s, 41°/s] | - | 50563 |
| Yaw speed (LOS firefight) p99 ▲ | 878°/s [761°/s, 972°/s] | 305°/s [292°/s, 318°/s] | - | 50563 |
| Mouse still between ticks (LOS firefight) ▲ | 2.7% [2.4%, 3.1%] | 1.6% [1.4%, 1.8%] | - | 50563 |
| Yaw speed (reloading) p50 ▲ | 11°/s [9°/s, 13°/s] | 18°/s [16°/s, 20°/s] | - | 104680 |
| Yaw speed (reloading) p99 ▲ | 606°/s [559°/s, 657°/s] | 945°/s [909°/s, 991°/s] | - | 104680 |
| Mouse still between ticks (reloading) | 27.9% [26.7%, 29.1%] | 26.3% [25.0%, 27.6%] | - | 104680 |
| Peak yaw speed of 90-135 deg turns p50 | 644°/s [612°/s, 669°/s] | 619°/s [595°/s, 655°/s] | - | 2847 |
| Peak yaw speed of 135-180 deg turns p50 | 868°/s [843°/s, 892°/s] | 852°/s [828°/s, 887°/s] | - | 1613 |
| Peak yaw speed of 180-360 deg turns p50 | 1106°/s [1009°/s, 1200°/s] | 1061°/s [1020°/s, 1126°/s] | - | 324 |

### Belief

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Yaw error to the hidden opponent, last seen <=250 ms ▲ | 12.79° [6.71°, 23.02°] | 4.92° [4.56°, 5.26°] | - | 25262 |
| View within 30 deg of the hidden opponent, last seen <=250 ms ▲ | 68.9% [56.2%, 87.8%] | 92.7% [91.9%, 93.7%] | - | 25262 |
| Yaw error to the hidden opponent, last seen 250-500 ms | 10.12° [6.89°, 17.20°] | 6.66° [5.99°, 7.28°] | - | 12889 |
| View within 30 deg of the hidden opponent, last seen 250-500 ms ▲ | 75.4% [61.9%, 87.5%] | 92.2% [91.3%, 93.4%] | - | 12889 |
| Yaw error to the hidden opponent, last seen 0.5-1 s | 10.46° [8.50°, 14.54°] | 8.28° [7.48°, 9.11°] | - | 16302 |
| View within 30 deg of the hidden opponent, last seen 0.5-1 s ▲ | 78.2% [67.6%, 84.9%] | 88.0% [86.8%, 89.9%] | - | 16302 |
| Yaw error to the hidden opponent, last seen 1-2 s | 11.95° [10.31°, 15.63°] | 11.11° [9.95°, 12.13°] | - | 20438 |
| View within 30 deg of the hidden opponent, last seen 1-2 s | 77.0% [69.5%, 81.5%] | 78.9% [77.1%, 81.1%] | - | 20438 |
| Yaw error to the hidden opponent, last seen 2-4 s ▲ | 22.10° [19.20°, 28.83°] | 16.93° [15.18°, 19.08°] | - | 35149 |
| View within 30 deg of the hidden opponent, last seen 2-4 s ▲ | 56.9% [51.0%, 61.0%] | 64.3% [61.3%, 67.3%] | - | 35149 |
| Yaw error to the hidden opponent, last seen 4-8 s ▲ | 20.82° [16.30°, 29.28°] | 10.29° [9.19°, 11.80°] | - | 57157 |
| View within 30 deg of the hidden opponent, last seen 4-8 s ▲ | 56.7% [50.4%, 61.8%] | 76.4% [73.1%, 79.3%] | - | 57157 |
| Yaw error to the hidden opponent, last seen >8 s | 19.99° [11.15°, 29.89°] | 20.07° [12.82°, 29.16°] | - | 242690 |
| View within 30 deg of the hidden opponent, last seen >8 s | 59.3% [50.1%, 71.7%] | 65.1% [54.8%, 74.8%] | - | 242690 |
| Yaw error to the hidden opponent, last seen never seen this life ▲ | 11.40° [9.86°, 13.11°] | 19.23° [16.93°, 24.71°] | - | 173000 |
| View within 30 deg of the hidden opponent, last seen never seen this life ▲ | 73.2% [68.5%, 77.0%] | 62.5% [56.0%, 67.2%] | - | 173000 |

### Space

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time in the busiest 10% of 32u cells (dm/crnodoors) | 39.6% [34.2%, 49.9%] | 49.4% [44.0%, 50.8%] | - | 156450 |
| Share of duel time in the busiest 10% of 32u cells (dm/main) | 41.2% [32.4%, 42.3%] | 45.2% [40.5%, 48.6%] | - | 166310 |
| Share of duel time in the busiest 10% of 32u cells (dm/vents) | 66.3% [44.3%, 82.8%] | 44.3% [39.2%, 46.8%] | - | 189430 |
| Share of duel time in the busiest 10% of 32u cells (dm/downladder) | 46.6% [30.1%, 47.7%] | 61.4% [42.9%, 63.8%] | - | 171680 |

### Acquisition

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Median aim error at -500 ms from sight gain | 12.68° [11.33°, 14.85°] | 13.98° [12.90°, 15.10°] | - | 2155 |
| Median aim error at -200 ms from sight gain ▲ | 13.81° [11.44°, 17.22°] | 8.33° [7.70°, 8.92°] | - | 2157 |
| Median aim error at +0 ms from sight gain ▲ | 13.65° [11.14°, 17.21°] | 5.22° [4.88°, 5.61°] | - | 2162 |
| Median aim error at +100 ms from sight gain ▲ | 12.39° [9.79°, 15.23°] | 5.97° [5.54°, 6.43°] | - | 2162 |
| Median aim error at +200 ms from sight gain ▲ | 9.77° [7.83°, 12.73°] | 4.92° [4.54°, 5.23°] | - | 2162 |
| Median aim error at +300 ms from sight gain ▲ | 8.61° [6.97°, 11.40°] | 4.53° [4.12°, 4.88°] | - | 2145 |
| Median aim error at +500 ms from sight gain ▲ | 9.18° [7.01°, 11.95°] | 5.47° [5.08°, 5.88°] | - | 2019 |
| Median aim error at +1000 ms from sight gain ▲ | 9.94° [7.74°, 12.22°] | 7.25° [6.72°, 7.70°] | - | 1807 |
| Fire held at -500 ms from sight gain ▲ | 35.7% [32.8%, 38.1%] | 18.2% [16.5%, 20.2%] | - | 2162 |
| Fire held at -200 ms from sight gain ▲ | 35.2% [32.3%, 37.6%] | 21.7% [19.7%, 23.9%] | - | 2162 |
| Fire held at +0 ms from sight gain ▲ | 34.5% [31.2%, 36.8%] | 42.5% [39.1%, 46.0%] | - | 2162 |
| Fire held at +100 ms from sight gain ▲ | 42.7% [39.5%, 45.2%] | 61.4% [57.8%, 64.6%] | - | 2162 |
| Fire held at +200 ms from sight gain ▲ | 49.3% [45.9%, 52.0%] | 77.0% [73.9%, 79.5%] | - | 2162 |
| Fire held at +400 ms from sight gain ▲ | 57.4% [54.0%, 60.5%] | 87.3% [85.5%, 89.0%] | - | 2119 |
| Fire held at +700 ms from sight gain ▲ | 55.5% [52.2%, 57.8%] | 77.0% [74.3%, 79.7%] | - | 2047 |
| Fire held at +1000 ms from sight gain ▲ | 51.9% [48.0%, 54.2%] | 63.4% [60.9%, 66.4%] | - | 1985 |
| Fire held at +1450 ms from sight gain ▲ | 46.3% [43.1%, 48.8%] | 53.6% [49.3%, 58.5%] | - | 1902 |
| Reaction: first trigger press after a clean sighting p25 | 100 ms [100 ms, 150 ms] | 50 ms [50 ms, 100 ms] | - | 692 |
| Reaction: first trigger press after a clean sighting p50 | 250 ms [200 ms, 250 ms] | 150 ms [150 ms, 200 ms] | - | 692 |
| Reaction: first trigger press after a clean sighting p75 ▲ | 450 ms [412 ms, 500 ms] | 300 ms [250 ms, 300 ms] | - | 692 |
| Reaction: first trigger press after a clean sighting p90 ▲ | 850 ms [800 ms, 1000 ms] | 400 ms [400 ms, 450 ms] | - | 692 |
| Crosshair within half-width+1.5 deg after a clean sighting p50 | 300 ms [250 ms, 300 ms] | 200 ms [200 ms, 250 ms] | - | 776 |
| Crosshair already on the body at the first visible tick ▲ | 11.9% [10.7%, 13.3%] | 22.6% [20.9%, 24.4%] | - | 2162 |
| Fire already held on the tick before sight (prefire) | 35.0% [31.7%, 37.7%] | 35.4% [32.2%, 38.6%] | - | 2162 |

### Trigger

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shots per burst (consecutive 100 ms shots) p50 | 6 [5, 7] | 5 [4, 5] | - | 8129 |
| Shots per burst (consecutive 100 ms shots) p90 ▲ | 19 [16, 23] | 12 [11, 13] | - | 8129 |

### Combat

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Shot accuracy (eligible SMG shots that hit) ▲ | 5.3% [4.5%, 6.0%] | 19.2% [18.1%, 20.3%] | - | 63407 |

### Lives

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Respawn delay after death p50 ▲ | 3050 ms [3000 ms, 3050 ms] | 2450 ms [2400 ms, 2450 ms] | - | 635 |
| Life length (lives ending in death) p50 ▲ | 26.2 s [22.0 s, 31.8 s] | 7.1 s [5.8 s, 8.2 s] | - | 635 |

### Reload

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Reload within 3 s of a kill, rounds left 0 | 100.0% [100.0%, 100.0%] | 95.5% [86.4%, 100.0%] | - | 15 |
| Reload within 3 s of a kill, rounds left 1-3 ▲ | 100.0% [100.0%, 100.0%] | 96.4% [92.6%, 99.2%] | - | 54 |
| Reload within 3 s of a kill, rounds left 4-6 | 98.7% [94.5%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 57 |
| Reload within 3 s of a kill, rounds left 7-10 | 98.4% [95.5%, 100.0%] | 100.0% [100.0%, 100.0%] | - | 68 |
| Reload within 3 s of a kill, rounds left 11-15 | 96.8% [91.4%, 100.0%] | 98.8% [97.9%, 99.5%] | - | 90 |
| Reload within 3 s of a kill, rounds left 16-20 | 95.4% [91.6%, 99.6%] | 95.7% [93.5%, 97.7%] | - | 89 |
| Reload within 3 s of a kill, rounds left 21-25 | 80.4% [74.0%, 85.7%] | 83.1% [76.5%, 88.5%] | - | 106 |
| Reload within 3 s of a kill, rounds left 26-29 | 60.8% [49.7%, 73.8%] | 63.4% [52.8%, 75.0%] | - | 110 |
| Reload within 3 s of a kill, rounds left 30-32 | 58.0% [42.5%, 76.8%] | 73.7% [50.0%, 92.3%] | - | 31 |
| Delay from kill to reload (reloads within 3 s) p50 ▲ | 700 ms [700 ms, 700 ms] | 600 ms [600 ms, 650 ms] | - | 534 |
| Reloads started with the opponent dead ▲ | 24.2% [20.3%, 27.9%] | 88.2% [85.5%, 90.4%] | - | 2220 |
| Reloads with the opponent alive that are forced (empty clip) | 85.9% [83.7%, 87.7%] | 77.7% [69.5%, 83.9%] | - | 1685 |

### Fights

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Engagements that end in a kill ▲ | 56.7% [51.7%, 60.5%] | 79.9% [77.9%, 81.9%] | - | 2254 |
| First hitter wins (decisive engagements) | 68.9% [66.3%, 70.9%] | 65.4% [63.7%, 66.7%] | - | 1260 |
| First shooter wins (decisive engagements) | 52.1% [50.5%, 53.9%] | 55.0% [52.5%, 56.6%] | - | 1260 |
| Duration of decisive engagements p50 ▲ | 2000 ms [1850 ms, 2151 ms] | 1250 ms [1150 ms, 1351 ms] | - | 1260 |
| Engagement start distance p50 | 339 u [291 u, 405 u] | 427 u [398 u, 470 u] | - | 2254 |
| Decisive engagements won (subject's own) | 49.6% [45.7%, 53.5%] | 50.0% [45.6%, 54.2%] | - | 1260 |

### Perception

| Statistic | Bots | Humans | Owner | n bots |
|---|---|---|---|---:|
| Share of duel time with a body part on screen and the centroid hidden ▲ | 2.5% [1.9%, 3.2%] | 10.5% [9.6%, 11.8%] | - | 683870 |
| SMG shots that hit, a part on screen, centroid hidden ▲ | 8.8% [7.3%, 11.0%] | 21.7% [18.9%, 24.3%] | - | 2389 |
| SMG shots that hit, centroid visible ▲ | 19.9% [17.8%, 21.9%] | 27.2% [25.8%, 28.5%] | - | 15444 |
| Centroid sightings with a body part on screen first ▲ | 38.3% [35.8%, 40.3%] | 85.9% [84.2%, 87.7%] | - | 2155 |
| Lead of the first part over the centroid p50 | 100 ms [50 ms, 100 ms] | 100 ms [100 ms, 100 ms] | - | 831 |
| Median aim error at -500 ms from the first visible part ▲ | 11.07° [9.78°, 12.50°] | 15.23° [14.18°, 16.19°] | - | 1281 |
| Median aim error at -200 ms from the first visible part | 11.36° [9.66°, 13.36°] | 10.54° [9.80°, 11.23°] | - | 1281 |
| Median aim error at -100 ms from the first visible part ▲ | 11.48° [9.95°, 13.79°] | 8.00° [7.34°, 8.49°] | - | 1281 |
| Median aim error at +0 ms from the first visible part ▲ | 11.60° [10.05°, 13.62°] | 5.22° [4.91°, 5.53°] | - | 1281 |
| Median aim error at +100 ms from the first visible part ▲ | 9.79° [8.08°, 11.58°] | 4.87° [4.54°, 5.18°] | - | 1281 |
| Median aim error at +200 ms from the first visible part ▲ | 7.05° [5.83°, 8.52°] | 5.26° [4.86°, 5.58°] | - | 1281 |
| Fire already held on the tick before the first part (prefire) ▲ | 38.0% [35.4%, 40.0%] | 23.0% [20.3%, 25.5%] | - | 1281 |
| Reaction: first press after a clean sighting, from the first part p50 | 200 ms [200 ms, 250 ms] | 200 ms [200 ms, 250 ms] | - | 403 |
| Reaction: mean first press after a clean sighting, from the first part ▲ | 301 ms [268 ms, 338 ms] | 232 ms [217 ms, 250 ms] | - | 403 |
| Crosshair within half-width+1.5 deg after a clean sighting, from the first part p50 | 250 ms [200 ms, 251 ms] | 250 ms [200 ms, 250 ms] | - | 543 |
| First hit after the first part p50 ▲ | 300 ms [300 ms, 350 ms] | 250 ms [200 ms, 250 ms] | - | 587 |
| Part sightings whose visible bout ends without a hit ▲ | 53.3% [49.0%, 57.9%] | 27.5% [25.7%, 29.7%] | - | 1281 |
| Yaw error to the hidden enemy 500 ms before the first part ▲ | 9.76° [8.73°, 11.20°] | 14.63° [13.70°, 15.63°] | - | 1281 |
| Yaw error to where the enemy will appear, 500 ms before the first part | 11.73° [9.89°, 14.52°] | 10.65° [9.39°, 11.89°] | - | 1281 |
| Crosshair parked within 5 deg of where the enemy will appear, 500 ms before | 25.0% [20.6%, 29.0%] | 29.4% [27.1%, 32.4%] | - | 1281 |
| Crosshair closer to where the enemy will appear than to the enemy, -500 ms ▲ | 39.4% [35.8%, 43.1%] | 67.0% [65.6%, 68.8%] | - | 1281 |
| View turn toward the enemy over the last 500 ms before the first part ▲ | 2.66° [1.81°, 4.02°] | 13.01° [11.44°, 14.55°] | - | 1281 |
| View turn over the last 500 ms before the first part ▲ | 6.84° [6.09°, 8.78°] | 14.53° [13.34°, 15.90°] | - | 1281 |
| Speed 400-200 ms before the first part ▲ | 129 u/s [120 u/s, 140 u/s] | 188 u/s [182 u/s, 193 u/s] | - | 1281 |
| Speed 2-1 s before the first part ▲ | 121 u/s [114 u/s, 129 u/s] | 167 u/s [159 u/s, 176 u/s] | - | 1014 |
| Hidden, no sighting next: crosshair closer to where the enemy will be in 500 ms ▲ | 47.1% [46.4%, 47.8%] | 50.5% [49.0%, 51.6%] | - | 515410 |
| Yaw error to the corner, 500 ms before the first part ▲ | 16.16° [12.71°, 19.14°] | 5.81° [5.17°, 6.44°] | - | 799 |
| Angle to the corner, 500 ms before the first part ▲ | 17.56° [14.23°, 20.36°] | 8.41° [7.70°, 9.19°] | - | 799 |
| Pitch to the corner (- = corner above the crosshair), 500 ms before the first part | -2.89° [-3.81°, -2.38°] | -2.72° [-3.14°, -2.39°] | - | 799 |
| Crosshair past the corner's edge (+ open side, - cover), 500 ms before the first part ▲ | 7.63° [5.04°, 11.24°] | -1.77° [-2.04°, -1.43°] | - | 799 |
| Yaw error to the appearance point (sightings with a corner), 500 ms before the first part | 12.23° [9.54°, 14.46°] | 10.34° [9.19°, 11.29°] | - | 799 |
| Yaw error to the hidden enemy (sightings with a corner), 500 ms before the first part ▲ | 9.17° [8.31°, 10.37°] | 15.18° [14.24°, 16.01°] | - | 799 |
| Crosshair within 5 deg of the corner, 500 ms before the first part ▲ | 10.9% [6.8%, 14.5%] | 29.2% [26.4%, 32.7%] | - | 799 |
| Crosshair closer to the corner than to the enemy, 500 ms before the first part ▲ | 35.1% [30.7%, 39.4%] | 80.0% [78.8%, 81.1%] | - | 799 |
| Crosshair on the cover side of the corner (> 2 deg), 500 ms before the first part ▲ | 30.5% [26.8%, 34.3%] | 48.2% [45.7%, 50.3%] | - | 799 |
| Crosshair within 2 deg of the corner's edge, 500 ms before the first part ▲ | 8.6% [5.7%, 11.2%] | 23.7% [21.7%, 25.7%] | - | 799 |
| Crosshair out past the corner on the open side (> 2 deg), 500 ms before the first part ▲ | 60.9% [57.4%, 64.8%] | 28.1% [26.8%, 29.4%] | - | 799 |
| Angle to the corner at the first part ▲ | 11.15° [9.80°, 13.07°] | 5.74° [5.33°, 6.17°] | - | 799 |
| Distance from the eye to the corner p50 | 171 u [130 u, 220 u] | 228 u [204 u, 248 u] | - | 799 |
| Yaw error to the corner at -2000 ms (sightings hidden >= 2 s) ▲ | 22.12° [17.71°, 27.51°] | 14.66° [11.85°, 17.22°] | - | 431 |
| Yaw error to the corner at -1000 ms (sightings hidden >= 2 s) ▲ | 17.99° [13.41°, 23.02°] | 5.78° [4.88°, 6.98°] | - | 431 |
| Yaw error to the corner at -500 ms (sightings hidden >= 2 s) ▲ | 17.67° [14.94°, 20.66°] | 4.66° [3.99°, 5.31°] | - | 431 |
| Yaw error to the corner at +0 ms (sightings hidden >= 2 s) ▲ | 9.76° [8.53°, 11.69°] | 3.54° [3.01°, 4.10°] | - | 431 |

Consistency: 112 pooled statistics (bots and owner together) were recomputed from the analysis scripts' own JSON; 0 differ.

## Notes

- perception statistics on the bots' logged body-part column (ext_vis_parts: their own perception); the people's are on the data repo's rebuilt column, accepted against the logged one on 2026-09-28
- 100% of the bots' duel time is against another bot (the logged opponent is the nearest living enemy); the human reference is humans against humans only
- engagements.py counts contact episodes only while exactly two players are alive; with more than two players in a session the fight statistics cover the phases where one player is dead
