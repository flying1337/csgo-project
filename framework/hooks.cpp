#include "../cheat.h"
#include "config.h"

fnPresent oPresent = 0;
fnReset oReset = 0;
WNDPROC oWndProc = 0;
fnPaintTraverse oPaintTraverse = 0;
fnLockCursor oLockCursor = 0;
fnCreateMove oCreateMove = 0;
fnDoPostScreenSpaceEffects oDoPostScreenSpaceEffects = 0;
fnDrawModelExecute oDrawModelExecute = 0;
fnFrameStageNotify oFrameStageNotify = 0;
fnListLeavesInBox oListLeavesInBox = 0;
fnOverrideView oOverrideView = 0;
fnGetScreenAspectRatio oGetScreenAspectRatio = 0;
fnDoExtraBonesProcessing oDoExtraBonesProcessing = 0;
fnEyeAngles oEyeAngles = 0;
fnUpdateClientsideAnimation oUpdateClientsideAnimation = 0;
fnStandartBlendingRules oStandartBlendingRules = 0;
fnGetViewModelFOV oGetViewModelFOV = 0;
fnPacketStart oPacketStart = 0;
fnFireEvent oFireEvent = 0;
fnPostDataUpdate oPostDataUpdate = 0;
fnSendNetMsg oSendNetMsg = 0;
fnWriteUsercmdDeltaToBuffer oWriteUsercmdDeltaToBuffer = 0;
fnIN_KeyEvent oIN_KeyEvent = 0;
fnCL_SendMove oCL_SendMove = 0;
fnSetChoked oSetChoked = 0;
fnHlCreateMove oHlCreateMove = 0;
fnRunCommand oRunCommand = 0;
fnClGrenadePreview_get_bool oGrenadePreview_get_bool = 0;