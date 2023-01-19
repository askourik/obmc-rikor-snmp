/**
 * @brief RIKOR OID helper
 */

#pragma once

#define RIKOR_OID(args...)                                                     \
    {                                                                          \
        1, 3, 6, 1, 4, 1, 65321, ##args                                        \
    }
