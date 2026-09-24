/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.
===========================================================================
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    void CG_ClientTelemetryInit(void);
    void CG_ClientTelemetryFrame(void);
    void CG_ClientTelemetryShutdown(void);

#ifdef __cplusplus
}
#endif
