#!/bin/bash
# Clear the download quarantine from the two apps so macOS runs them in place
# instead of from a translocated copy that cannot see the game files.
cd "$(dirname "$0")" || exit 1
xattr -dr com.apple.quarantine launcher.app mohbots.app
