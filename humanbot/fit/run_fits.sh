#!/bin/sh
# Refit the human-bot model from the private recordings and rebuild everything
# derived from them. Needs the openmohaa-movement checkout (HB_MOVEMENT_REPO)
# with its captures pulled; caches go to humanbot/cache (git-ignored).
#
#   humanbot/fit/run_fits.sh
#
# Writes humanbot/model/{shared,styles,calibration}.json, humanbot/maps/*.json,
# code/humanbot/hb_embedded.cpp and the replay files for hb_replay.
set -eu
cd "$(dirname "$0")"
PY=${PYTHON:-python3}
$PY fit_keys.py
$PY fit_movement.py
$PY fit_trigger.py
$PY fit_view.py
$PY fit_weapon.py
$PY fit_maps.py
$PY fit_styles.py
$PY fit_velocity_response.py
$PY assemble_model.py
$PY ../tools/embed_model.py
$PY export_replay.py
