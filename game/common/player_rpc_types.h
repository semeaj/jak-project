#pragma once

/*!
 * @file player_rpc_types.h
 * Types used for the player Remote Procedure Call between the EE and the IOP.
 * Note that PLAY and PLAYER are different.
 */
#include "common/versions/versions.h"

constexpr PerGameVersion<int> PLAYER_RPC_ID(0xdeb1, 0xfab0, 0xfab0, 0xfab0);
// Jak X shifted every EE-side RPC port up by one (GOAL creates its rpc-buffer-pairs on
// ports 1-6 instead of 0-5), so the channel number is per-game like the RPC IDs above.
constexpr PerGameVersion<int> PLAYER_RPC_CHANNEL(0, 0, 0, 1);
