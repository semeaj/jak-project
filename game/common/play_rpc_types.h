#pragma once

/*!
 * @file play_rpc_types.h
 * Types used for the play Remote Procedure Call between the EE and the IOP.
 * Note that PLAY and PLAYER are different.
 */
#include "common/versions/versions.h"

constexpr PerGameVersion<int> PLAY_RPC_ID(0xdeb6, 0xfab5, 0xfab5, 0xfab5);
// Jak X shifts all EE RPC ports up by one (see player_rpc_types.h).
constexpr PerGameVersion<int> PLAY_RPC_CHANNEL(5, 5, 5, 6);
