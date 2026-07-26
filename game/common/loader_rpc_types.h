#pragma once

/*!
 * @file loader_rpc_types.h
 * Types used for the Loader Remote Procedure Call between the EE and the IOP
 */

#include "common/versions/versions.h"

constexpr PerGameVersion<int> LOADER_RPC_ID(0xdeb2, 0xfab1, 0xfab1, 0xfab1);
// Jak X shifts all EE RPC ports up by one (see player_rpc_types.h).
constexpr PerGameVersion<int> LOADER_RPC_CHANNEL(1, 1, 1, 2);
